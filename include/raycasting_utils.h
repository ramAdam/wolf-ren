
#ifndef RAYCASTING_UTILS_H
#define RAYCASTING_UTILS_H

#include <SDL2/SDL.h>

typedef SDL_Color Color;

#define RED \
    (Color) { 255, 0, 0, 255 }
#define GREEN \
    (Color) { 0, 255, 0, 255 }
#define BLUE \
    (Color) { 0, 0, 255, 255 }
#define WHITE \
    (Color) { 255, 255, 255, 255 }
#define BLACK \
    (Color) { 0, 0, 0, 255 }

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define PLAYER_SPEED 5
#define FOV 60.0

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

/**
 * Calculates the endpoint of a line based on a given position, angle, and length.
 * @param playerPosition The starting position of the line.
 * @param angle The angle at which the line extends.
 * @param length The length of the line.
 * @return The calculated endpoint of the line.
 */
Point calculateLineEndpoint(Point playerPosition, double angle, int length);

/**
 * Calculates the intersection point between a ray and a plane defined by a wall.
 * @param rayOrigin The starting point of the ray.
 * @param rayDirection The direction vector of the ray.
 * @param wallPointX The x-coordinate of the wall.
 * @return The intersection point.
 */
Point rayPlaneIntersection(Point rayOrigin, Vector rayDirection, double wallPointX);

/**
 * Calculates the movement vector based on a given angle and speed.
 * @param angle The angle at which the movement occurs.
 * @param speed The speed of the movement.
 * @return The calculated movement vector.
 */
Point calculateMovementVector(double angle, double speed);

/**
 * Creates a player with specified attributes.
 * @param position The initial position of the player.
 * @param angle The initial angle of the player's orientation.
 * @param speed The movement speed of the player.
 * @param fov The field of view in degrees.
 * @param viewDistance The maximum distance the player can see.
 * @param color The color of the player.
 * @param rayLength The length of the ray used for calculations.
 * @return The created player.
 */
Player createPlayer(Point position, double angle, double speed, double fov, double viewDistance, Color color, int rayLength);

/**
 * Finds the intersection point between two lines.
 * @param line1 The first line.
 * @param line2 The second line.
 * @param intersectionPoint A pointer to store the intersection point.
 * @return 1 if an intersection is found, otherwise 0.
 */
int findLineIntersection(Line line1, Line line2, Point *intersectionPoint);

/**
 * Draws a line using the specified renderer and color.
 * @param renderer The renderer used for drawing.
 * @param line The line to be drawn.
 * @param color The color of the line.
 */
void drawLine(SDL_Renderer *renderer, Line line, SDL_Color color);

/**
 * Draws a line at a specific angle and calculates the endpoint.
 * @param renderer The renderer used for drawing.
 * @param start The starting point of the line.
 * @param angle The angle at which the line extends.
 * @param length The length of the line.
 * @param color The color of the line.
 * @param resultLine A pointer to store the resulting line.
 */
void drawLineAtAnAngle(SDL_Renderer *renderer, Point start, double angle, int length, SDL_Color color, Line *resultLine);

/**
 * Draws a point at the specified center with a given radius and color.
 * @param renderer The renderer used for drawing.
 * @param center The center of the point.
 * @param radius The radius of the point.
 * @param color The color of the point.
 */
void drawPoint(SDL_Renderer *renderer, Point center, int radius, SDL_Color color);

/**
 * Draws rays from the player to the walls.
 * @param renderer The renderer used for drawing.
 * @param playerPosition The position of the player.
 * @param playerAngle The angle of the player.
 * @param numRays The number of rays to be drawn.
 * @param FOV The field of view in degrees.
 * @param rayLength The length of the ray.
 * @param walls The walls to be checked for intersections.
 * @param numWalls The number of walls.
 * @param color The color of the ray.
 */
void drawRays(SDL_Renderer *renderer, Point playerPosition, double playerAngle, int numRays, int rayLength, Line *walls, int numWalls, SDL_Color color);

#endif