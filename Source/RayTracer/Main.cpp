#include <iostream>
#include "Renderer.h"
#include "Canvas.h"
#include "Random.h"

int main(int argc, char* argv[])
{
    std::cout << "Hello world!\n";
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("uwu", 400, 300);

    Canvas canvas(400, 300, renderer);

    float aspectRatio = (canvas.GetSize().x / (float)canvas.GetSize().y);

    //Scene scene;
    //scene.SetCamera(camera);

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
        for (int i = 0; i < 1000; i++) {
            canvas.DrawPoint({ random01() * canvas.GetSize().x, random01() * canvas.GetSize().y}, {random01(), random01(), random01(), 1});
        }
        //scene.Render(canvas);
        canvas.Update();
        renderer.PresentCanvas(canvas);
    }
    renderer.Shutdown();

    return 0;
}