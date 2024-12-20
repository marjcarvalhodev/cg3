#ifndef INPUT_HANDLER
#define INPUT_HANDLER

#include <SDL2/SDL.h>
#include <iostream>
#include "scene.hpp"

void handleInput(SDL_Event &event, bool &running, MyScene &scene,
                 MyCamera &camera, MyWindow &window) {
  const Uint8 *keyState = SDL_GetKeyboardState(NULL);

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      running = false;
    } else if (event.type == SDL_KEYDOWN) {
      std::cout << "click button, any" << std::endl;
      if (keyState[SDL_SCANCODE_W]) {
        scene.moveCamera(camera, glm::vec3(0.0f, 0.0f, -1.0f)); // Move forward
      }
      if (keyState[SDL_SCANCODE_S]) {
        scene.moveCamera(camera, glm::vec3(0.0f, 0.0f, 1.0f)); // Move backward
      }
      if (keyState[SDL_SCANCODE_A]) {
        scene.moveCamera(camera, glm::vec3(-1.0f, 0.0f, 0.0f)); // Move left
      }
      if (keyState[SDL_SCANCODE_D]) {
        scene.moveCamera(camera, glm::vec3(1.0f, 0.0f, 0.0f)); // Move right
      }
    } else if (event.type == SDL_MOUSEBUTTONDOWN &&
               event.button.button == SDL_BUTTON_LEFT) {
      int mouseX, mouseY;
      SDL_GetMouseState(&mouseX, &mouseY);
      scene.handleMouseClick(mouseX, mouseY, camera, window.getWidth(),
                             window.getHeight());
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f) {
      Uint32 flags = SDL_GetWindowFlags(window.getSDLWindow());
      if (flags & SDL_WINDOW_FULLSCREEN) {
        SDL_SetWindowFullscreen(window.getSDLWindow(), 0);
      } else {
        SDL_SetWindowFullscreen(window.getSDLWindow(), SDL_WINDOW_FULLSCREEN);
      }
    }
  }
}

#endif