#include <iostream>
#include "Renderer.h"
#include "Canvas.h"
#include "Random.h"
#include "Scene.h"
#include "Camera.h"
//#include "Material.h"
#include "Sphere.h"

int main(int argc, char* argv[])
{
    seedRandom(static_cast<unsigned int>(time(nullptr)));
    std::cout << "Hello world!\n";
    Renderer renderer = Renderer();
    renderer.Initialize();
    renderer.CreateWindow("uwu", 400, 300);

    Canvas canvas(400, 300, renderer);

    float aspectRatio = (canvas.GetSize().x / (float)canvas.GetSize().y);

    std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 0, 1 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, 70.0f, aspectRatio);
    Scene scene(20);
    scene.SetCamera(camera);
    
     //create material
    auto lambertian = std::make_shared<Lambertian>(color3_t{ 1, 0, 0 });
    auto metal = std::make_shared<Metal>(color3_t{ 1,0,1 }, 0.0f);

    for (int i = 0; i < 20; i++) {
        std::shared_ptr<Material> material = (random01() < 0.5f) ? std::dynamic_pointer_cast<Material>(lambertian) : std::dynamic_pointer_cast<Material>(metal);
        auto sphere = std::make_unique<Sphere>(random(glm::vec3{-10,-10,-10}, glm::vec3{8,8,-1}), 1, material);
        scene.AddObject(std::move(sphere));
    }

    bool isOld = false;
    if (!isOld) {
        canvas.Clear({ 0,0,0,1 });
        scene.Render(canvas,50);
        canvas.Update();
    }

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                }
                break;
        }
        if (isOld) {
            canvas.Clear({ 0,0,0,1 });
            scene.Render(canvas);
            canvas.Update();
        }
        
        renderer.PresentCanvas(canvas);
    }
    renderer.Shutdown();

    return 0;
}