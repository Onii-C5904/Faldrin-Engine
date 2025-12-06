//
// Created by kristijan on 12/6/25.
//

#pragma once

#ifndef RENDERING_H
#define RENDERING_H

#include <vector>
#include <raylib.h>
#include <functional>
#include <algorithm>
#include "Sprite.h"

struct RenderItem {
    float y;
    std::function<void()> render;
};

struct SpriteInstance {
    Vector2 position;
    Sprite* sprite;
};

struct TMXTexture {
    Texture2D* texture;
    Rectangle* clipRect;
    int x;
    int y;
};

struct TMXAnimatedTexture {
    Texture2D* texture;
    std::vector<float> frameDelayTimes;
    double lastFrameTime = 0.0;
    int frameCount = 0;
    int currentFrame = 0;
    int x;
    int y;

    [[nodiscard]] Rectangle createAnimationFrameRect() const;
};


std::vector<RenderItem> buildRenderQueue(std::vector<SpriteInstance>& sprites, std::vector<std::vector<TMXTexture>>& textures, std::vector<std::vector<TMXAnimatedTexture>>& animatedTextures);


#endif //RENDERING_H
