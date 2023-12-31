#include "raycasting_utils.h"
#include <math.h>
#include <stdio.h>

Point calculateLineEndpoint(Point playerPosition, double angle, int length)
{
    Point endpoint;
    endpoint.x = playerPosition.x + length * cos(angle);
    endpoint.y = playerPosition.y + length * sin(angle);
    return endpoint;
}

Player createPlayer(Point position, double angle, double speed, double fov, double viewDistance, Color color, int rayLength)
{
    Player player;
    player.position = position;
    player.angle = angle;
    player.speed = speed;
    player.fov = fov;
    player.viewDistance = viewDistance;
    player.color = color;

    player.ray.start = position;
    player.ray.length = rayLength;
    player.ray.color = GREEN;

    return player;
}

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

Point calculateMovementVector(double angle, double speed)
{
    Point movement;
    movement.x = speed * cos(angle);
    movement.y = speed * sin(angle);
    return movement;
}

void drawRays(SDL_Renderer *renderer, Point playerPosition, double playerAngle, int numRays, int rayLength, Line *walls, int numWalls, SDL_Color color)
{
    double angleIncrement = FOV / (numRays - 1);

    drawPoint(renderer, playerPosition, 10, GREEN);
    for (int i = 0; i < numRays; i++)
    {
        double rayAngle = playerAngle - (FOV / 2) + i * angleIncrement;

        Point lineEndPoint = calculateLineEndpoint(playerPosition, rayAngle, rayLength);
        Line ray = {playerPosition, lineEndPoint, color};

        Point intersectionPoint;
        int hasIntersection = 0;

        for (int j = 0; j < numWalls; j++)
        {
            if (findLineIntersection(ray, walls[j], &intersectionPoint) == 1)
            {
                hasIntersection = 1;
                ray.end = intersectionPoint;
                break;
            }
        }

        // !hasIntersection ? ray.end = lineEndPoint : drawPoint(renderer, intersectionPoint, 4, RED);

        if (!hasIntersection)
        {
            ray.end = lineEndPoint;
        }
        else
        {
            drawPoint(renderer, intersectionPoint, 4, RED);
        }

        drawLine(renderer, ray, color);
    }
}

void movePlayer(SDL_Event event, Line *walls, int numOfWalls, Point *playerPosition, double *playerAngle)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_LEFT:
            *playerAngle -= 0.1; // Move left
            break;
        case SDLK_RIGHT:
            *playerAngle += 0.1; // Move right
            break;
        case SDLK_UP:
        {
            int collision = checkPlayerWallCollision(walls, 2, *playerPosition, *playerAngle, PLAYER_COLLISION_DIST);
            if (collision != 1)
            {
                Point movement = calculateMovementVector(*playerAngle, PLAYER_SPEED);
                playerPosition->x += movement.x;
                playerPosition->y += movement.y;
            }
        }
        break;
        case SDLK_DOWN:
        {
            double angle = *playerAngle + M_PI;
            int collision = checkPlayerWallCollision(walls, 2, *playerPosition, angle, PLAYER_COLLISION_DIST);
            if (collision != 1)
            {
                Point movement = calculateMovementVector(*playerAngle, PLAYER_SPEED);
                playerPosition->x -= movement.x;
                playerPosition->y -= movement.y;
            }
        }
        break;
        }
    }
}

int checkPlayerWallCollision(Line *walls, int numWalls, Point playerPosition, double playerAngle, int playerMovementLength)
{
    Line playerLine = {
        .start = playerPosition,
        .end = calculateLineEndpoint(playerPosition, playerAngle, playerMovementLength)};

    for (int i = 0; i < numWalls; i++)
    {
        Point intersectionPoint;
        if (findLineIntersection(playerLine, walls[i], &intersectionPoint))
        {
            // Intersection detected, handle it here if needed
            return 1; // Collision detected
        }
    }

    return 0; // No collision
}
