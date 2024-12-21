#ifndef INPUT_HANDLER
#define INPUT_HANDLER

#include <SDL2/SDL.h>
#include <iostream>
#include "scene.hpp"

void handleInput(SDL_Event &event, bool &running, MyScene &scene,
                 MyCamera &camera, MyWindow &window) {
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      running = false;
    } else if (event.type == SDL_KEYDOWN) {
      // Log the key pressed
      std::cout << "Key pressed: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;

      // Handle specific keys for movement
      switch (event.key.keysym.sym) {
        case SDLK_w:
          scene.moveCamera(camera, glm::vec3(0.0f, 0.0f, -1.0f)); // Move forward
          break;
        case SDLK_s:
          scene.moveCamera(camera, glm::vec3(0.0f, 0.0f, 1.0f));  // Move backward
          break;
        case SDLK_a:
          scene.moveCamera(camera, glm::vec3(-1.0f, 0.0f, 0.0f)); // Move left
          break;
        case SDLK_d:
          scene.moveCamera(camera, glm::vec3(1.0f, 0.0f, 0.0f));  // Move right
          break;
        case SDLK_f: {
          Uint32 flags = SDL_GetWindowFlags(window.getSDLWindow());
          if (flags & SDL_WINDOW_FULLSCREEN) {
            SDL_SetWindowFullscreen(window.getSDLWindow(), 0);
          } else {
            SDL_SetWindowFullscreen(window.getSDLWindow(), SDL_WINDOW_FULLSCREEN);
          }
          break;
        }
      }
    } else if (event.type == SDL_MOUSEBUTTONDOWN &&
               event.button.button == SDL_BUTTON_LEFT) {
      int mouseX, mouseY;
      SDL_GetMouseState(&mouseX, &mouseY);
      scene.handleMouseClick(mouseX, mouseY, camera, window.getWidth(),
                             window.getHeight());
    }
  }
}

#endif
