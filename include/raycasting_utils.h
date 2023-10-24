#include <SDL2/SDL.h>

#ifndef RAYCASTING_UTILS_H
#define RAYCASTING_UTILS_H

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

typedef struct
{
    Point start;
    Point end;
    Color color;
    int length;
} Ray;

typedef struct
{
    Point position;
    double angle;
    double speed;
    double fov;
    double viewDistance;
    Color color;
    Ray ray;

} Player;

Point calculateLineEndpoint(Point playerPosition, double angle, int length);
Point rayPlaneIntersection(Point rayOrigin, Vector rayDirection, double wallPointX);
Point calculateMovementVector(double angle, double speed);
Player createPlayer(Point position, double angle, double speed, double fov, double viewDistance, Color color, int rayLength);
int findLineIntersection(Line line1, Line line2, Point *intersectionPoint);
void drawLine(SDL_Renderer *renderer, Line line, SDL_Color color);
void drawLineAtAnAngle(SDL_Renderer *renderer, Point start, double angle, int length, SDL_Color color, Line *resultLine);
void drawPoint(SDL_Renderer *renderer, Point center, int radius, SDL_Color color);


#endif