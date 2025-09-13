//
// Created by kristijan on 9/12/25.
//

#ifndef SPRITE_H
#define SPRITE_H

#include <raylib.h>

class Sprite {
public:
    void load(const char *fileName, int filter, Vector2 size, Vector2 offset = {0,0});
    Rectangle clipRect;
    Vector2 offset;
    void render(Vector2 pos);
    void setFilter(int filter);

private:
    Texture2D texture;

};



#endif //SPRITE_H
