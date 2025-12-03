//
// Created by kristijan on 9/12/25.
//

#include "Player.h"

void Player::init(Vector2 pos, float speed, std::vector<Box2D*>* colBoxes) {
    this->speed = speed;
    sprite.load("assets/Characters/Premium Charakter Spritesheet.png", TEXTURE_FILTER_POINT, {16,16}, {16,16});
    physicsBox.position = pos;
    physicsBox.width = 10;
    physicsBox.height = 4;
    physicsBox.offset = {3, 12};
    collisionBoxes = colBoxes;
}

void Player::render() {
    sprite.render(*position - physicsBox.offset);
}

void Player::update() {
    // INPUT
    if (left && right) {dir->x = 0;}
    else if(left){dir->x = -1;}
    else if (right) {dir->x = 1;}
    else {dir->x = 0;}

    if (up && down) {dir->y = 0;}
    else if(up){dir->y = -1;}
    else if (down) {dir->y = 1;}
    else {dir->y = 0;}

    *velocity = *dir * speed;

    slideAndCollide(&physicsBox, collisionBoxes);

    *position += *velocity;

}
