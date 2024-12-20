#include "assets_manager.hpp"
#include <unistd.h>

namespace fs = std::filesystem;

AssetsManager::AssetsManager() {

  std::string baseDir = "";

  char path[1024];
  ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
  if (count != -1) {
    path[count] = '\0';
    baseDir = fs::path(path).parent_path().string();
  }

  basePath = baseDir + "/assets";

  // #ifdef __EMSCRIPTEN__
  //   // Assume the basePath exists in WebAssembly builds
  //   this->basePath = "/" + basePath;
  //   std::cout << "WebAssembly build: Setting basePath as: " << this->basePath
  //             << std::endl;
  // #else
  if (!fs::exists(basePath)) {
    throw std::runtime_error("Assets directory not found: " + basePath);
  }
  this->basePath = basePath;
  // #endif
}

AssetsManager::~AssetsManager() {}

std::string AssetsManager::readFile(const std::string &fileName) const {
  std::cout << "readFile: Trying to load file: " << fileName << std::endl;
#ifdef __EMSCRIPTEN__
  // Use Emscripten's filesystem API
  std::string filePath = basePath + "/" + fileName;
  std::ifstream file(filePath);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file in WASM environment: " +
                             filePath);
  }
#else
  // Use standard filesystem for native builds
  fs::path filePath = fs::path(basePath) / fileName;

  if (!fs::exists(filePath)) {
    throw std::runtime_error("File not found: " + filePath.string());
  }

  std::ifstream file(filePath);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filePath.string());
  }
#endif

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  return buffer.str();
}

void AssetsManager::loadModel(const std::string &key,
                              const std::string &modelName) {
  std::string path =
      basePath + "/models/" + modelName + "/" + modelName + ".obj";

  MeshData meshData = MeshLoader::loadModel(path);
  if (meshData.isEmpty()) {
    std::cerr << "Failed to load mesh." << std::endl;
  }
  models[key] = std::make_shared<MyMesh>(MyMesh(meshData));
}

void AssetsManager::loadShader(const std::string &key,
                               const std::string &shaderName) {
#ifdef __EMSCRIPTEN__
  std::string shadersFolder = "shaders-webgl/" + shaderName;
#else
  std::string shadersFolder = "shaders/" + shaderName;
#endif
  std::cout << "loadShader: Trying to load from: " << shadersFolder
            << std::endl;

  std::string vertexShaderSource = readFile(shadersFolder + "/vertex.glsl");
  std::string fragmentShaderSource = readFile(shadersFolder + "/fragment.glsl");

  ShaderSources sources = {vertexShaderSource, fragmentShaderSource};
  shaders[key] = std::make_shared<MyShader>(sources);
}

void AssetsManager::preloadAllTextures() {
  std::string texturesPath = basePath + "/textures/";

  for (const auto &entry : fs::directory_iterator(texturesPath)) {
    if (entry.is_regular_file()) {
      std::string filePath = entry.path().string();
      std::string fileName = entry.path().filename().string();

      // Load the texture and store it in the map
      GLuint textureID = TextureLoader::loadTextureFromFile(filePath);
      textures[fileName] = textureID;

      std::cout << "Loaded texture: " << fileName << " (ID: " << textureID << ")\n";
    }
  }
}

// eof