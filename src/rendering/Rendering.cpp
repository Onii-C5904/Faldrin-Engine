//
// Created by kristijan on 12/6/25.
//


#include "Rendering.h"


Rectangle TMXAnimatedTexture::createAnimationFrameRect() const {
    return Rectangle{static_cast<float>(16 * currentFrame),0,16,16};
}

std::vector<RenderItem> buildRenderQueue(std::vector<SpriteInstance>& sprites, std::vector<std::vector<TMXTexture>>& textures, std::vector<std::vector<TMXAnimatedTexture>>& animatedTextures) {
    std::vector<RenderItem> renderItems;

    renderItems.reserve(sprites.size() + animatedTextures.size() + animatedTextures.size());
    for (const auto& sprite : sprites) {
        RenderItem item;
        item.y = sprite.position.y;
        item.render = [spr = sprite.sprite, pos = sprite.position]() {spr->render(pos);};
        renderItems.push_back(item);
    }
    for (const auto& layer : textures) {
        for (const auto& t : layer) {
            RenderItem item;
            item.y = static_cast<float>(t.y);
            item.render = [t]() {
                Rectangle src = *t.clipRect;
                Vector2 dest{ static_cast<float>(t.x), static_cast<float>(t.y) };
                DrawTextureRec(*t.texture, src, dest, WHITE);
            };
            renderItems.push_back(std::move(item));
        }
    }

    for (const auto& layer : animatedTextures) {
        for (const auto& a : layer) {
            RenderItem item;
            item.y = static_cast<float>(a.y);
            item.render = [a]() {
                Rectangle src = a.createAnimationFrameRect();
                Vector2 dest{ static_cast<float>(a.x), static_cast<float>(a.y) };
                DrawTextureRec(*a.texture, src, dest, WHITE);
            };
            renderItems.push_back(std::move(item));
        }
    }

    // Sort by y
    std::sort(renderItems.begin(), renderItems.end(),
              [](const RenderItem& lhs, const RenderItem& rhs) {
                  return lhs.y < rhs.y;
              });

    return renderItems;
}