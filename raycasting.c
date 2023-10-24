#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FOV 60 // Field of view in degrees

#define GREEN 0, 255, 0, 255
#define WHITE 255, 255, 255, 255
#define RED 255, 0, 0, 255

typedef struct
{
    double x;
    double y;
} Point;

typedef struct
{
    double dx;
    double dy;
} Vector;

typedef struct
{
    Point start;
    Point end;
} Line;

int findLineIntersection(Line line1, Line line2, Point *intersection)
{
    // Extract coordinates from line structures
    double x1 = line1.start.x;
    double y1 = line1.start.y;
    double x2 = line1.end.x;
    double y2 = line1.end.y;
    double x3 = line2.start.x;
    double y3 = line2.start.y;
    double x4 = line2.end.x;
    double y4 = line2.end.y;

    double den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (den == 0)
    {
        return 0; // Lines are parallel or coincident
    }

    intersection->x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / den;
    intersection->y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / den;

    return 1; // Intersection found
}

Point rayPlaneIntersection(Point rayOrigin, Vector rayDirection, double wallPointX)
{
    Point intersection;

    // Calculate t, the distance along the ray from the origin to the intersection point.
    double t = (wallPointX - rayOrigin.x) / rayDirection.dx;

    // Use t to calculate the x and y coordinates of the intersection point.
    intersection.x = rayOrigin.x + t * rayDirection.dx;
    intersection.y = rayOrigin.y + t * rayDirection.dy;

    return intersection;
}

void drawLine(SDL_Renderer *renderer, Line line, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawLine(renderer, (int)line.start.x, (int)line.start.y, (int)line.end.x, (int)line.end.y);
}

void drawPoint(SDL_Renderer *renderer, Point center, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    for (int dy = 0; dy <= radius; dy++)
    {
        int dx = (int)round(sqrt(radius * radius - dy * dy));
        SDL_Rect rect1 = {(int)(center.x - dx), (int)(center.y + dy), 2 * dx, 1};
        SDL_Rect rect2 = {(int)(center.x - dx), (int)(center.y - dy), 2 * dx, 1};
        SDL_RenderFillRect(renderer, &rect1);
        SDL_RenderFillRect(renderer, &rect2);
    }
}

Point calculateLineEndpoint(Point playerPosition, double angle, int length)
{
    Point endpoint;
    endpoint.x = playerPosition.x + length * cos(angle);
    endpoint.y = playerPosition.y + length * sin(angle);
    return endpoint;
}

Point calculateMovementVector(double angle, double speed)
{
    Point movement;
    movement.x = speed * cos(angle);
    movement.y = speed * sin(angle);
    return movement;
}

void initSDLWindow()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Window *window = SDL_CreateWindow("Raycasting Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window)
    {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer)
    {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
}

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
    Line wall = {{200, 400}, {300, 200}};
    double playerAngle = 0.0;

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

        
        Point lineEndpoint = calculateLineEndpoint(playerPosition, playerAngle, 100);
        Line ray = {playerPosition, lineEndpoint};

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render the line representing the wall
        drawPoint(renderer, playerPosition, 10, GREEN);
        drawLine(renderer, ray, WHITE);

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