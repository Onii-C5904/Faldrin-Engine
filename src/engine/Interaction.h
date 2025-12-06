//
// Created by kristijan on 12/3/25.
//

#pragma once

#ifndef INTERACTION_H
#define INTERACTION_H
#include <raylib.h>
#include <vector>
#include <functional>
#include "Physics.h"

struct Box2D;

enum InteractionObjectType {
    INTERACTION_OBJECT,
    INTERACTION_BOX_2D,
    INTERACTION_CIRCLE_2D,
    INTERACTION_SHAPE_2D
};


struct InteractionObject {
    Vector2 position;
    std::function<void()> function = nullptr;
    bool active;
    InteractionObjectType type;

    virtual void render(Color color, bool wireFrameEnabled);
    void update(Box2D* playerCollisionBox);
    std::vector<Box2D*> getOverlappingBoxes();

private:
    std::vector<Box2D*> overlappingBoxes;
};

struct InteractionBox2D : InteractionObject {
    float width, height;

    InteractionBox2D() {
        type = INTERACTION_BOX_2D;
        active = true;
    }

    std::vector<Vector2> getPoints();
    Rectangle toRect();

    void render(Color color, bool wireFrameEnabled) override;
};

struct InteractionCircle2D : InteractionObject {
    float radius;

    InteractionCircle2D() {
        type = INTERACTION_CIRCLE_2D;
        active = true;
    }

    void render(Color color, bool wireFrameEnabled) override;
};

struct InteractionShape2D : InteractionObject {
    std::vector<Vector2> points;

    InteractionShape2D() {
        type = INTERACTION_SHAPE_2D;
        active = true;
    }

    void render(Color color, bool wireFrameEnabled) override;
};



#endif //INTERACTION_H
