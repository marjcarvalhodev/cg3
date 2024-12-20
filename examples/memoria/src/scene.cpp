#include "scene.hpp"
#include <iostream>

MyScene::MyScene(const MyCamera &camera, const GameGrid &grid)
    : camera(camera), grid(grid) {}

MyScene::~MyScene() {}

MyScene MyScene::setupScene(MyCamera &camera, AssetsManager &assetsManager) {
  GameGrid gameGrid(4, 2.0); // 4x4 grid, 2.0f cell size
  MyScene scene(camera, gameGrid);

  scene.initializeShaders(assetsManager);
  scene.initializeModels(assetsManager);
  scene.initializeObjects(assetsManager);

  return scene;
}

void MyScene::animateObject(std::shared_ptr<MyObject> object, float deltaTime) {
  float angle =
      glm::radians(30.0f * deltaTime); // Rotate by 30 degrees per second
  glm::mat4 rotation =
      glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
  object->spin(rotation);
}

void MyScene::renderScene(MyWindow &window) const {
  window.clearBuffers();

  // Define global light properties
  glm::vec3 globalLightPos = glm::vec3(5.0f, 10.0f, 5.0f);
  glm::vec3 globalLightColor = glm::vec3(1.0f);

  // Update global uniforms in all shaders
  for (const auto &[key, shader] : scene_shaders) {
    shader->updateGlobalUniforms(camera.getViewMatrix(),
                                 camera.getProjectionMatrix(),
                                 camera.getPosition(), globalLightColor);

    shader->setUniform("lightPos", globalLightPos); // Update global light
    shader->setUniform("ambientStrength", 0.5f);    // Adjust as needed
    shader->setUniform("specularStrength", 0.5f);   // Adjust as needed
  }

  // Step 1: Render opaque objects
  for (const auto &[key, object] : scene_objects) {
    if (!object->isTransparent) {
      object->render(camera.getViewMatrix(), camera.getProjectionMatrix(),
                     camera.getPosition());
    }
  }

  // Step 2: Sort transparent objects based on distance to camera
  std::vector<std::shared_ptr<MyObject>> transparentObjects;
  for (const auto &[key, object] : scene_objects) {
    if (object->isTransparent) {
      transparentObjects.push_back(object);
    }
  }

  std::sort(transparentObjects.begin(), transparentObjects.end(),
            [&](const std::shared_ptr<MyObject> &a,
                const std::shared_ptr<MyObject> &b) {
              float distA =
                  glm::distance(camera.getPosition(), a->getPosition());
              float distB =
                  glm::distance(camera.getPosition(), b->getPosition());
              return distA > distB; // Sort back to front
            });

  // Step 3: Enable blending and render transparent objects
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(GL_FALSE); // Disable depth writes for transparency

  for (const auto &object : transparentObjects) {
    object->render(camera.getViewMatrix(), camera.getProjectionMatrix(),
                   camera.getPosition());
  }

  // Restore default settings
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);

  window.swapBuffers();
}

void MyScene::handleMouseClick(int mouseX, int mouseY, const MyCamera &camera,
                               int windowWidth, int windowHeight) {
  glm::vec3 rayOrigin = camera.getPosition();
  glm::vec3 rayDir = calculateRayDirection(mouseX, mouseY, windowWidth,
                                           windowHeight, camera.getViewMatrix(),
                                           camera.getProjectionMatrix());

  glm::vec2 hitCell;
  bool hit = grid.intersectsRay(rayOrigin, rayDir, hitCell, 1.0);

  if (hit) {
    std::cout << "Hit cell: (" << hitCell.x << ", " << hitCell.y << ")"
              << std::endl;
    int cellValue =
        grid.getCell(static_cast<int>(hitCell.x), static_cast<int>(hitCell.y));
    if (cellValue == 1) {
      std::cout << "Hit wood_ball!" << std::endl;
    }
  }
  // for (const auto &[key, object] : scene_objects) {
  //   if (object->intersectsRay(rayOrigin, rayDir)) {
  //     std::cout << "Clicked on object: " << key << std::endl;
  //     object->onClick(); // Call object-specific logic
  //   }
  // }
}

void MyScene::initializeShaders(AssetsManager &assetsManager) {

  std::cout << "initializeShaders: inicio" << std::endl;
  std::vector<std::string> shaderNames = {"phong", "glass", "fog"};

  for (const auto &name : shaderNames) {
    loadAndRegisterShader(assetsManager, name);
    std::string shaderName = name + "Shader";
    std::shared_ptr<MyShader> shader = assetsManager.getShader(shaderName);
    addSceneShaders(shaderName, shader);
  }

  // auto fogShader = assetsManager.getShader("fogShader");
  // fogShader->setUniform("fogStart", 5.0f);
  // fogShader->setUniform("fogEnd", 20.0f);
  // fogShader->setUniform("fogColor", glm::vec3(0.5f, 0.5f, 0.5f));

  std::cout << "initializeShaders: fim" << std::endl;
}

void MyScene::initializeModels(AssetsManager &assetsManager) {
  std::cout << "initializeModels: inicio" << std::endl;
  std::vector<std::string> modelNames = {"ball", "diamond"};
  for (const auto &name : modelNames) {
    std::shared_ptr<MyMesh> mesh = createMeshFromModel(assetsManager, name);
    addSceneMeshes(name, mesh);
  }
  std::cout << "initializeModels: inicio" << std::endl;
  // MeshData floorMeshData = createMeshDataFromVertices(floorVertices);
  // meshes["floor"] = std::make_shared<MyMesh>(floorMeshData);
}

void MyScene::initializeObjects(AssetsManager &assetsManager) {
  std::cout << "initializeObjects: inicio" << std::endl;

  auto phongShader = getSceneShader("phongShader");
  auto glassShader = getSceneShader("glassShader");
  std::cout << "initializeObjects: pegou shaders" << std::endl;

  assetsManager.preloadAllTextures();
  std::cout << "initializeObjects: carregou texturas" << std::endl;

  GLuint woodTextureId = assetsManager.getTexture("tree-bark.jpg");
  GLuint floorTextureId = assetsManager.getTexture("floor.jpg");
  std::cout << "initializeObjects: pegou texturas" << std::endl;

  Material floorMaterial = {{0.6f, 0.6f, 0.6f}, 32.0f};
  Material mat = {{0.5, 0.3, 0.36}, 50.0};

  std::vector<
      std::tuple<std::string, glm::vec3, std::shared_ptr<MyMesh>, Material,
                 std::shared_ptr<MyShader>, bool, GLuint, int>>
      objects = {
          {"wood_ball",
           {-2.0f, 0.0f, -2.0f},
           getSceneMesh("ball"),
           mat,
           phongShader,
           false,
           woodTextureId,
           GL_TRIANGLES},
          {"normal_ball",
           {-2.0f, 0.0f, 2.0f},
           getSceneMesh("ball"),
           mat,
           phongShader,
           false,
           0,
           GL_TRIANGLES},
          {"shiny_diamond",
           {2.0f, 0.0f, 2.0f},
           getSceneMesh("diamond"),
           mat,
           glassShader,
           true,
           0,
           GL_TRIANGLES},
          {"rough_glass",
           {2.0f, 0.0f, -2.0f},
           getSceneMesh("diamond"),
           mat,
           glassShader,
           false,
           0,
           GL_TRIANGLES}
          //  {"floor",
          //   {0.0f, 0.0f, 0.0f},
          //   meshes.at("floor"),
          //   floorMaterial,
          //   phongShader,
          //   false,
          //   floorTextureId,
          //   GL_TRIANGLES}
      };

  for (const auto &[name, position, mesh, material, shader, isTransparent,
                    textureId, drawType] : objects) {
    auto object = createObject(mesh, material, shader, isTransparent, textureId,
                               drawType);
    object->repositionObject(position);
    addSceneObjects(name, object);
  }

  std::cout << "initializeObjects: fim" << std::endl;
}

// eof