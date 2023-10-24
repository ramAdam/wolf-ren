#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

typedef SDL_Color Color;

// Define some constant colors
#define RED \
    (Color) { 255, 0, 0, 255 }
#define GREEN \
    (Color) { 0, 255, 0, 255 }
#define BLUE \
    (Color) { 0, 0, 255, 255 }
#define WHITE \
    (Color) { 255, 255, 255, 255 }
#define GREEN \
    (Color) { 0, 255, 0, 255 }

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FOV 60 // Field of view in degrees

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
    Color color;
} Line;

int findLineIntersection(Line line1, Line line2, Point *intersectionPoint)
{
    int x1 = line1.start.x;
    int y1 = line1.start.y;
    int x2 = line1.end.x;
    int y2 = line1.end.y;

    int x3 = line2.start.x;
    int y3 = line2.start.y;
    int x4 = line2.end.x;
    int y4 = line2.end.y;

    int denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (denominator != 0)
    {
        int px = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denominator;
        int py = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denominator;

        if ((px >= fmin(x1, x2) && px <= fmax(x1, x2)) &&
            (py >= fmin(y1, y2) && py <= fmax(y1, y2)) &&
            (px >= fmin(x3, x4) && px <= fmax(x3, x4)) &&
            (py >= fmin(y3, y4) && py <= fmax(y3, y4)))
        {
            intersectionPoint->x = px;
            intersectionPoint->y = py;
            return 1; // Intersection found
        }
    }

    return 0; // No intersection fou
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

void drawLine(SDL_Renderer *renderer, Line line, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    // SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawLine(renderer, (int)line.start.x, (int)line.start.y, (int)line.end.x, (int)line.end.y);
}

// draw a line at an angle
void drawLineAtAnAngle(SDL_Renderer *renderer, Point start, double angle, int length, SDL_Color color, Line *resultLine)
{
    Point endPoint;
    endPoint.x = start.x + length * cos(angle);
    endPoint.y = start.y + length * sin(angle);

    if (resultLine != NULL)
    {
        resultLine->start = start;
        resultLine->end = endPoint;
        resultLine->color = color;
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, start.x, start.y, endPoint.x, endPoint.y);
}

void drawPoint(SDL_Renderer *renderer, Point center, int radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
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