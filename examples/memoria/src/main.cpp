#include "assets_manager.hpp"
#include "camera.hpp"
#include "constants.hpp"
#include "mesh.hpp"
#include "object.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "utils.hpp"
#include "window.hpp"
#include <SDL2/SDL.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

void windowResize(SDL_Event &event, MyCamera &camera);
MyCamera getDefaultCamera(const MyWindow &window);
void handleInput(SDL_Event &event, bool &running);

int main() {
  try {
    MyWindow window("Memoria", DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT,
                    NIGHT_CLEAR_COLOR);
    if (!window.init()) {
      return 1;
    }
    MyCamera camera = getDefaultCamera(window);
    SDL_Event event;
    bool running = true;

    AssetsManager assetsManager;
    assetsManager.loadTextures();
    assetsManager.loadModel("ballModel", "ball");
    assetsManager.loadModel("diamondModel", "diamond");
    assetsManager.loadShader("basicShader", "basic");
    assetsManager.loadShader("phongShader", "phong");
    assetsManager.loadShader("glassShader", "glass");

    std::shared_ptr<MyMesh> ballMesh = assetsManager.getModel("ballModel");
    std::shared_ptr<MyMesh> diamondMesh =
        assetsManager.getModel("diamondModel");
    std::shared_ptr<MyShader> basic_shader =
        assetsManager.getShader("basicShader");
    std::shared_ptr<MyShader> phong_shader =
        assetsManager.getShader("phongShader");
    std::shared_ptr<MyShader> glass_shader =
        assetsManager.getShader("glassShader");
    GLuint woodTextureId = assetsManager.getTexture("tree-bark.jpg");

    Material mat = {{0.5, 0.3, 0.36}, 50.0};

    std::shared_ptr<MyObject> wood_ball =
        std::make_shared<MyObject>(ballMesh, mat, basic_shader, false);
    std::shared_ptr<MyObject> normal_ball =
        std::make_shared<MyObject>(ballMesh, mat, basic_shader, false);
    std::shared_ptr<MyObject> shiny_diamond =
        std::make_shared<MyObject>(diamondMesh, mat, basic_shader, false);
    std::shared_ptr<MyObject> rough_glass =
        std::make_shared<MyObject>(diamondMesh, mat, basic_shader, false);

    wood_ball->repositionObject({-2.0, 0.0, -2.0});
    normal_ball->repositionObject({-2.0, 0.0, 2.0});
    shiny_diamond->repositionObject({2.0, 0.0, 2.0});
    rough_glass->repositionObject({2.0, 0.0, -2.0});

    MyScene main_scene(camera);

    main_scene.addSceneShaders("basicShader", basic_shader);
    main_scene.addSceneShaders("phongShader", phong_shader);
    main_scene.addSceneShaders("glassShader", glass_shader);

    main_scene.addSceneObjects("wood_ball", wood_ball);
    main_scene.addSceneObjects("normal_ball", normal_ball);
    main_scene.addSceneObjects("shiny_diamond", shiny_diamond);
    main_scene.addSceneObjects("rough_glass", rough_glass);

    float lastTime = SDL_GetTicks() / 1000.0f;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(
        [](void *arg) {
          MyScene *scene = static_cast<MyScene *>(arg);
          scene->renderScene(static_cast<MyWindow &>(scene->getWindow()));
        },
        &main_scene, 0, 1);
#else

    while (running) {
      const int frameDelay = 16; // Approx 60 FPS (1000ms / 60)
      SDL_Delay(frameDelay);

      float currentTime = SDL_GetTicks() / 1000.0f; // Current time in seconds
      float deltaTime = currentTime - lastTime; // Time elapsed since last frame
      lastTime = currentTime;

      handleInput(event, running);

      for (const auto &[key, object] : main_scene.getAllSceneObjects()) {
        main_scene.animateObject(object, deltaTime);
      }

      main_scene.renderScene(window);
    }
#endif

    return 0;
  } catch (const std::exception &e) {
    printErr(e);
    return 1;
  }
}

MyCamera getDefaultCamera(const MyWindow &window) {
  glm::vec3 position = {0.0, 10.0, 15.0};
  glm::vec3 target = {0.0, 0.0, 0.0};
  glm::vec3 upDir = {0.0, 1.0, 0.0};
  float aspectRatio =
      static_cast<float>(window.getWidth()) / window.getHeight();

  MyCamera camera(position, target, upDir, aspectRatio);
  return camera;
}

void windowResize(SDL_Event &event, MyCamera &camera) {
  int newWidth = event.window.data1;
  int newHeight = event.window.data2;

  glViewport(0, 0, newWidth, newHeight);

  camera.setAspectRatio(static_cast<float>(newWidth) / newHeight);
  camera.updateProjectionMatrix();
}

void handleInput(SDL_Event &event, bool &running) {
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      running = false;
    }
  }
}

// eof