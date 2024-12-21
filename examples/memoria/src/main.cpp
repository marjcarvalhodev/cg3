#include "constants.hpp"
#include "utils.hpp"
#include <SDL2/SDL.h>
#include "window.hpp"
#include "camera.hpp"
#include "assets_manager.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "object.hpp"
#include "scene.hpp"

#include <SDL_events.h>
#include <SDL_keycode.h>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

void windowResize(SDL_Event &event, MyCamera &camera);
MyCamera getDefaultCamera(const MyWindow &window);
void handleInput(SDL_Event &event, bool &running, MyCamera &camera);
void handleKey(int sym, bool &running, MyCamera &camera);

int main()
{
    try
    {
        MyWindow window("3D-Viewer", DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT, NIGHT_CLEAR_COLOR);
        if (!window.init())
        {
            return 1;
        }
        static MyCamera camera = getDefaultCamera(window);
        SDL_Event event;
        bool running = true;

        AssetsManager assetsManager;
        assetsManager.loadTextures();

        assetsManager.loadModel("ballModel", "ball");
        std::shared_ptr<MyMesh> ballMesh = assetsManager.getModel("ballModel");

        assetsManager.loadModel("diamondModel", "diamond");
        std::shared_ptr<MyMesh> diamondMesh = assetsManager.getModel("diamondModel");

        assetsManager.loadShader("basicShader", "basic");
        std::shared_ptr<MyShader> basic_shader = assetsManager.getShader("basicShader");

        assetsManager.loadShader("phongShader", "phong");
        std::shared_ptr<MyShader> phong_shader = assetsManager.getShader("phongShader");

        assetsManager.loadShader("glassShader", "glass");
        std::shared_ptr<MyShader> glass_shader = assetsManager.getShader("glassShader");

        Material matRuby = {{0.7, 0.33, 0.33}, 150.0};
        Material matMetal = {{0.33, 0.43, 0.53}, 300.0};
        Material mat = {{0.5, 0.5, 0.5}, 50.0};
        
        GLuint woodTextureId = assetsManager.getTexture("tree-bark.jpg");

        std::shared_ptr<MyObject> wood_ball = std::make_shared<MyObject>(ballMesh, mat, basic_shader, false, woodTextureId);
        std::shared_ptr<MyObject> normal_ball = std::make_shared<MyObject>(ballMesh, mat, phong_shader, false);
        std::shared_ptr<MyObject> shiny_diamond = std::make_shared<MyObject>(diamondMesh, matRuby, glass_shader, true);
        std::shared_ptr<MyObject> rough_glass = std::make_shared<MyObject>(diamondMesh, matMetal, glass_shader, false, woodTextureId);

        MyScene main_scene(camera);

        wood_ball->repositionObject({-2.0, 0.0, -2.0});
        normal_ball->repositionObject({-2.0, 0.0, 2.0});
        shiny_diamond->repositionObject({2.0, 0.0, 2.0});
        rough_glass->repositionObject({2.0, 0.0, -2.0});

        main_scene.addSceneObjects("shiny_ball_2", wood_ball);
        main_scene.addSceneObjects("shiny_ball_1", normal_ball);
        main_scene.addSceneObjects("shiny_diamond_1", shiny_diamond);
        main_scene.addSceneObjects("shiny_diamond_2", rough_glass);

        float lastTime = SDL_GetTicks() / 1000.0f; // Initialize lastTime in seconds

        while (running)
        {
            float currentTime = SDL_GetTicks() / 1000.0f; // Current time in seconds
            float deltaTime = currentTime - lastTime;     // Time elapsed since last frame
            lastTime = currentTime;

            handleInput(event, running, camera);

            for (const auto &[key, object] : main_scene.getAllSceneObjects())
            {
                main_scene.animateObject(object, deltaTime);
            }

            main_scene.renderScene(window);
        }

        return 0;
    }
    catch (const std::exception &e)
    {
        printErr(e);
        return 1;
    }
}

MyCamera getDefaultCamera(const MyWindow &window)
{
    glm::vec3 position = {0.0, 10.0, 15.0};
    glm::vec3 target = {0.0, 0.0, 0.0};
    glm::vec3 upDir = {0.0, 1.0, 0.0};
    float aspectRatio = static_cast<float>(window.getWidth()) / window.getHeight();

    MyCamera camera(position, target, upDir, aspectRatio);
    return camera;
}

void windowResize(SDL_Event &event, MyCamera &camera)
{
    int newWidth = event.window.data1;
    int newHeight = event.window.data2;

    glViewport(0, 0, newWidth, newHeight);

    camera.setAspectRatio(static_cast<float>(newWidth) / newHeight);
    camera.updateProjectionMatrix();
}

void handleInput(SDL_Event &event, bool &running, MyCamera &camera)
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            case SDL_KEYDOWN: handleKey(event.key.keysym.sym, running, camera);
                break;
            case SDL_QUIT: running = false;
                break;
        }
    }
}

void handleKey(int sym, bool &running, MyCamera &camera)
{
    switch (sym) {
        case SDLK_ESCAPE: running = false;
            break;
        case SDLK_w: camera.setPosition(glm::vec3(0.0f,1.0f,0.0f));
            break;
        case SDLK_a: break;
        case SDLK_s: break;
        case SDLK_d: break;
    }
}

// eof