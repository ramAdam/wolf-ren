#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FOV 60 // Field of view in degrees


typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    double dx;
    double dy;
} Vector;

Point rayPlaneIntersection(Point rayOrigin, Vector rayDirection, double wallPointX) {
    Point intersection;

    double t = (wallPointX - rayOrigin.x) / rayDirection.dx;
    intersection.x = rayOrigin.x + t * rayDirection.dx;
    intersection.y = rayOrigin.y + t * rayDirection.dy;

    return intersection;
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Raycasting Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

   int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Update game state here

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render game objects here
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // SDL_RenderDrawPoint(renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

         // Draw a simple vertical wall
        for (int i = 10; i < SCREEN_HEIGHT; i++) {
            SDL_RenderDrawPoint(renderer, 400, i);
        }

        // Present the rendered frame
        SDL_RenderPresent(renderer);

        // Add a small delay to control the frame rate (optional)
        SDL_Delay(16); // Approximately 60 frames per second
    }

    // Cleanup resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}