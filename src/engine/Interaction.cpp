//
// Created by kristijan on 12/3/25.
//

#include "Interaction.h"


// INTERACTION OBJECT 2D
void InteractionObject::update(Box2D* playerCollisionBox){
    if (InteractionObjectCollisionCheck(this, playerCollisionBox)) {
        if (function) {
            function();
        }
    }
}

// Defenition so it compiles
void InteractionObject::render(Color color, bool wireFrameEnabled) {};




// INTERACTION BOX 2D
void InteractionBox2D::render(Color color, bool wireFrameEnabled = false) {
    if (!wireFrameEnabled){ DrawRectangle(position.x, position.y, width, height, {color.r, color.g, color.b, 230}); }
    DrawRectangleLines(position.x, position.y, width, height, color);
}

std::vector<Vector2> InteractionBox2D::getPoints() {
    return {position, Vector2{position.x + width, position.y},Vector2{position.x + width, position.y + height}, Vector2{position.x, position.y + height}};
}

Rectangle InteractionBox2D::toRect() {
    return {position.x, position.y, width, height};
}

// INTERACTION CIRCLE 2D
void InteractionCircle2D::render(Color color, bool wireFrameEnabled = false) {
    if (!wireFrameEnabled){ DrawCircle(position.x, position.y, radius, {color.r, color.g, color.b, 230}); }
    DrawCircleLines(position.x, position.y, radius, color);
}

// INTERACTION SHAPE 2D
void InteractionShape2D::render(Color color, bool wireFrameEnabled = false) {
    int numLines = points.size();
    for (int i = 0; i < numLines; i++) {
        if (i == numLines - 1) {
            DrawLine(points[i].x, points[i].y, points[0].x, points[0].y, color);
        }
        else {
            DrawLine(points[i].x, points[i].y, points[i+1].x, points[i+1].y, color);
        }
    }
}