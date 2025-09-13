//
// Created by kristijan on 9/12/25.
//

#include "Player.h"

void Player::init(Vector2 pos, float speed) {
    position = pos;
    this->speed = speed;
    sprite.load("assets/Characters/Premium Charakter Spritesheet.png", TEXTURE_FILTER_POINT, {16,16}, {16,16});
}

void Player::render() {
    sprite.render(position);
}

void Player::update() {
    // INPUT
    if (left && right) {dir.x = 0;}
    else if(left){dir.x = -1;}
    else if (right) {dir.x = 1;}
    else {dir.x = 0;}

    if (up && down) {dir.y = 0;}
    else if(up){dir.y = -1;}
    else if (down) {dir.y = 1;}
    else {dir.y = 0;}

    dir = Vector2Normalize(dir);

    velocity = dir * speed;

    position += velocity;
}
