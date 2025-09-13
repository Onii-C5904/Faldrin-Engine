//
// Created by kristijan on 9/12/25.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include <raymath.h>
#include "../rendering/Sprite.h"
#include <string>


class Player {

public:
    void init(Vector2 pos, float speed);
    void update();
    void render();
    Vector2 position = {0,0};
    float speed = 0.0;
    Vector2 velocity = {0,0};
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    Vector2 dir = {0,0};

private:
    Sprite sprite;

};



#endif //PLAYER_H
