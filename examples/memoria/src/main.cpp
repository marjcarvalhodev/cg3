#include "assets_manager.hpp"
#include "camera.hpp"
#include "constants.hpp"
#include "input_handler.hpp"
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

int main() {
  try {
    MyWindow window("Memoria", DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT,
                    NIGHT_CLEAR_COLOR);
    if (!window.init()) {
      return 1;
    }
    MyCamera camera = getDefaultCamera(window);
    AssetsManager assetsManager;

    MyScene main_scene = MyScene::setupScene(camera, assetsManager);

    float lastTime = SDL_GetTicks() / 1000.0f;
    const Uint32 frameDelay = 1000 / 60;
    Uint32 lastRenderTime = SDL_GetTicks();
    SDL_Event event;
    bool running = true;

    while (running) {
      handleInput(event, running, main_scene, camera, window);

      Uint32 currentTime = SDL_GetTicks();
      float deltaTime = (currentTime - lastTime) / 1000.0f;
      lastTime = currentTime;

      if (currentTime - lastRenderTime >= frameDelay) {
        main_scene.renderScene(window);
        lastRenderTime = currentTime;
      }

      SDL_Delay(1);
    }

    return 0;
  } catch (const std::exception &e) {
    // printErr(e);
    return 1;
  }
}

MyCamera getDefaultCamera(const MyWindow &window) {
  glm::vec3 position = {0.0, 10.0, 0.0};
  glm::vec3 target = {0.0, 0.0, 0.0};
  glm::vec3 upDir = {0.0, 0.0, -1.0};
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

// eof