#include "raycasting.h"

int main()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (initializeSDL(&window, &renderer) != 0)
    {
        return 1; // Error occurred during initialization
    }

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
                    Point movement = calculateMovementVector(playerAngle, PLAYER_SPEED); // Adjust speed as needed

                    playerPosition.x += movement.x;
                    playerPosition.y += movement.y;
                }
                break;
                case SDLK_DOWN:
                {
                    Point movement = calculateMovementVector(playerAngle, PLAYER_SPEED); // Adjust speed as needed

                    playerPosition.x -= movement.x;
                    playerPosition.y -= movement.y;
                }
                break;
                }
            }
        }
        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
        SDL_RenderClear(renderer);

        lineEndPoint = calculateLineEndpoint(playerPosition, playerAngle, 300);
        ray = (Line){playerPosition, lineEndPoint, GREEN};


        drawPoint(renderer, playerPosition, radius, GREEN);
        drawLine(renderer, ray, WHITE);

        // Draw wall
        drawLineAtAnAngle(renderer, wall.start, 0, 600, RED, &wall);

        if (findLineIntersection(ray, wall, &interserctionPoint) == 1)
        {
            drawPoint(renderer, interserctionPoint, 4, GREEN);
        }



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