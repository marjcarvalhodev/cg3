#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>

class MyWindow
{
private:
    SDL_Window *window;
    SDL_GLContext context;
    int width, height;
    std::string title;
    bool initialized;
    glm::vec4 clearColor;

public:
    MyWindow(const std::string &title, int width, int height, glm::vec4 clearColor);

    ~MyWindow();

    bool init();

    void setClearColor();

    void clearBuffers();

    void swapBuffers();

    bool shouldClose();

    void cleanUp();

    int getWidth() const;

    int getHeight() const;
};

#endif