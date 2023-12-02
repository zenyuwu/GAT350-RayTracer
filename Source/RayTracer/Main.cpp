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
    Scene scene;
    scene.SetCamera(camera);
    
     //create material
    auto material = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });
    for (int i = 0; i < 20; i++) {
        auto sphere = std::make_unique<Sphere>(random(glm::vec3{-10,-10,-10}, glm::vec3{8,8,-1}), 1, material);
        scene.AddObject(std::move(sphere));
    }
    //auto sphere = std::make_unique<Sphere>(glm::vec3{ 0,0,-1 }, 1, material);
    //scene.AddObject(std::move(sphere));

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
        
        canvas.Clear({ 0,0,0,1 });
       /* for (int i = 0; i < 1000; i++) {
            canvas.DrawPoint({ random01() * canvas.GetSize().x, random01() * canvas.GetSize().y}, {random01(), random01(), random01(), 1});
        }*/
        scene.Render(canvas);
        canvas.Update();

        renderer.PresentCanvas(canvas);
    }
    renderer.Shutdown();

    return 0;
}