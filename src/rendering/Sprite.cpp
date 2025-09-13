//
// Created by kristijan on 9/12/25.
//

#include "Sprite.h"


void Sprite::load(const char *fileName, int filter, Vector2 size, Vector2 offset) {
    this->offset = offset;
    clipRect = {offset.x,offset.y, size.x, size.y};
    texture = LoadTexture(fileName);
    SetTextureFilter(texture, filter);
}

void Sprite::render(Vector2 pos) {
    DrawTextureRec(texture, clipRect, pos, WHITE);
}

void Sprite::setFilter(int filter) {
    SetTextureFilter(texture, filter);
}
