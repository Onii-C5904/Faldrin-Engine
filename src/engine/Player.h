//
// Created by kristijan on 9/12/25.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

#include "../rendering/Sprite.h"
#include "Physics.h"


class Player {

public:
    void init(Vector2 pos, float speed, std::vector<Box2D*>* colBoxes);
    void update();
    void render();
    Sprite* getSprite();
    Box2D physicsBox;
    Vector2* position = &physicsBox.position;
    Vector2* dir = &physicsBox.direction;
    Vector2* velocity = &physicsBox.velocity;
    std::vector<Box2D*>* collisionBoxes;

    float speed = 0.0;
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;



private:
    Sprite sprite;

};



#endif //PLAYER_H
