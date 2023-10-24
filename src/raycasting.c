#include <SDL2/SDL.h>
#include "raycasting_utils.h"

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Raycasting Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window)
    {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer)
    {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    /**********************************************************************************************/

    Point playerPosition = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    Line ray;
    Line wall = {{200, 400}, {0, 0}, RED};

    Point interserctionPoint;
    double playerAngle = 0.0;
    Point lineEndPoint = calculateLineEndpoint(playerPosition, playerAngle, 300);

    int radius = 10;

    int running = 1;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    playerAngle -= 0.1; // Move left
                    break;
                case SDLK_RIGHT:
                    playerAngle += 0.1; // Move right
                    break;
                case SDLK_UP:
                {
                    Point movement = calculateMovementVector(playerAngle, 5); // Adjust speed as needed

                    playerPosition.x += movement.x;
                    playerPosition.y += movement.y;
                }
                break;
                case SDLK_DOWN:
                {
                    Point movement = calculateMovementVector(playerAngle, 5); // Adjust speed as needed

                    playerPosition.x -= movement.x;
                    playerPosition.y -= movement.y;
                }
                break;
                }
            }
        }

        lineEndPoint = calculateLineEndpoint(playerPosition, playerAngle, 300);
        ray = (Line){playerPosition, lineEndPoint, GREEN};

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawPoint(renderer, playerPosition, radius, GREEN);
        drawLine(renderer, ray, WHITE);

        // Draw wall
        drawLineAtAnAngle(renderer, wall.start, 0, 500, RED, &wall);

        if (findLineIntersection(ray, wall, &interserctionPoint) == 1)
        {
            drawPoint(renderer, interserctionPoint, 10, BLUE);
        }
   

        // ... (previous code)

        // Present the rendered frame
        SDL_RenderPresent(renderer);

        // Add a small delay to control the frame rate (optional)
        SDL_Delay(16); // Approximately 60 frames per second
    }
    /**************************************************************/
    // Cleanup resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}