//
// Created by kristijan on 9/18/25.
//

#pragma once


#ifndef PHYSICS_H
#define PHYSICS_H

#define UP_NORMAL {0, -1}
#define DOWN_NORMAL {0, 1}
#define LEFT_NORMAL {-1, 0}
#define RIGHT_NORMAL {1, 0}

#include <raylib.h>
#include <vector>
#include <algorithm>
#include <raymath.h>
#include "Interaction.h"

struct Box2D;
struct Polygon2D;
struct Circle2D;
struct InteractionObject;





struct Box2D {
    Vector2 position = {0, 0};
    Vector2 velocity = {0, 0};
    Vector2 direction = {0, 0};
    Vector2 offset = {0, 0};
    float width;
    float height;

    std::vector<Vector2> getPoints() const;
    void render() const;
    void update();
};




struct Polygon2D {
    Vector2 position = {0, 0};
    Vector2 velocity = {0, 0};
    Vector2 direction = {0, 0};
    std::vector<Vector2> vertices;

};

struct Circle2D {
    Vector2 position = {0, 0};
    Vector2 velocity = {0, 0};
    Vector2 direction = {0, 0};
    float radius;
};




// UNIVERSAL PHYISICS FUNCTIONS
bool SATCollisionCheckAndResolution(std::vector<Vector2> self, std::vector<Vector2> target);

bool SATCollisionCheck(std::vector<Vector2> self, std::vector<Vector2> target);

void slideAndCollide(Box2D* box, std::vector<Box2D*>* collisionBoxes);

bool InteractionObjectCollisionCheck(InteractionObject* obj1, Box2D* collisionBox);

#endif //PHYSICS_H
