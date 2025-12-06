//
// Created by kristijan on 11/29/25.
//

#include "Physics.h"

#include <iostream>
#include <bits/ostream.tcc>


std::vector<Vector2> Box2D::getPoints() const {
    return std::vector<Vector2> {position, Vector2{position.x + width, position.y},Vector2{position.x + width, position.y + height}, Vector2{position.x, position.y + height}};
}

void Box2D::render() const {
    DrawRectangle(position.x, position.y, width, height, {245, 40, 145, 153});
    DrawRectangleLines(position.x, position.y, width, height, {245, 40, 145, 255});
}

void Box2D::update() {
    position += velocity;
}



// UNIVERSAL PHYISICS FUNCTIONS
bool SATCollisionCheckAndResolution(Box2D* selfPhysicsBox, std::vector<Vector2> self, std::vector<Vector2> target) {
    // CONSTRUCT NORMALSd
    std::vector<Vector2> normals(self.size() + target.size());

    for (int i = 0; i < self.size(); i++) {
        if (i != self.size() - 1) { normals[i] = Vector2Normalize({self[i+1].y - self[i].y, self[i+1].x - self[i].x});}

        else { normals[i] = Vector2Normalize({self[0].y - self[i].y, self[0].x - self[i].x});}
    }

    for (int i = 0; i < target.size(); i++) {
        if (i != target.size() - 1) { normals[i + self.size()] = Vector2Normalize({target[i+1].y - target[i].y, target[i+1].x - target[i].x});}

        else { normals[i + self.size()] = Vector2Normalize({target[0].y - target[i].y, target[0].x - target[i].x});}
    }

    // FIND PROJECTION AXES
    std::vector<Vector2> projectionAxes;
    projectionAxes.reserve(normals.size());

    for (const auto& normal : normals) {
        if (projectionAxes.empty() < 1) {
            projectionAxes.push_back(normal);
        }

        else {
            bool found = false;
            for (const auto& axis : projectionAxes) {
                if (normal == axis){ found = true; break; }
            }
            if (!found) {
                projectionAxes.push_back(normal);
            }
        }
    }

    // PROJECT POINTS ONTO AXES
    float projectionOverlaps[projectionAxes.size()];
    bool negateVector[projectionAxes.size()] {false};
    for (int i = 0; i < projectionAxes.size(); i++) {

        float AProjections[self.size()];
        for (int j = 0; j < self.size(); j++) {
            AProjections[j] = Vector2DotProduct(self[j], projectionAxes[i]);
        }

        float AMin = *std::min_element(AProjections, AProjections + self.size());
        float AMax = *std::max_element(AProjections, AProjections + self.size());

        float BProjections[target.size()];
        for (int j = 0; j < target.size(); j++) {
            BProjections[j] = Vector2DotProduct(target[j], projectionAxes[i]);
        }

        float BMin = *std::min_element(BProjections, BProjections + target.size());
        float BMax = *std::max_element(BProjections, BProjections + target.size());

        if (BMin > AMax || BMax < AMin) {return false;} // CHECK COLLISION

        // DETERMINE MINIMUM OVERLAP AND NEGATE
        if (AMax - BMin <= BMax - AMin) { projectionOverlaps[i] = AMax - BMin; }
        else { projectionOverlaps[i] = BMax - AMin; negateVector[i] = true;}

    }

    // MINIMUM TRANSLATION VECTOR
    std::vector<std::pair<std::pair<Vector2, bool>, float>> overlaps(projectionAxes.size());
    for (int i = 0; i < projectionAxes.size(); i++) {
        overlaps[i] = {{projectionAxes[i], negateVector[i]}, projectionOverlaps[i]};
    }

    std::ranges::sort(overlaps, [](const std::pair<std::pair<Vector2, bool>, float>& a, const std::pair<std::pair<Vector2, bool>, float>& b) {return a.second < b.second;});

    if (overlaps[0].first.second) {
        selfPhysicsBox->velocity = Vector2Subtract(selfPhysicsBox->velocity, Vector2Negate(Vector2Scale(overlaps[0].first.first, overlaps[0].second)));
    }
    else {
        selfPhysicsBox->velocity = Vector2Subtract(selfPhysicsBox->velocity, Vector2Scale(overlaps[0].first.first, overlaps[0].second));
    }

    return true;

}

bool SATCollisionCheck(std::vector<Vector2> self, std::vector<Vector2> target) {
    // CONSTRUCT NORMALSd
    std::vector<Vector2> normals(self.size() + target.size());

    for (int i = 0; i < self.size(); i++) {
        if (i != self.size() - 1) { normals[i] = Vector2Normalize({self[i+1].y - self[i].y, self[i+1].x - self[i].x});}

        else { normals[i] = Vector2Normalize({self[0].y - self[i].y, self[0].x - self[i].x});}
    }

    for (int i = 0; i < target.size(); i++) {
        if (i != target.size() - 1) { normals[i + self.size()] = Vector2Normalize({target[i+1].y - target[i].y, target[i+1].x - target[i].x});}

        else { normals[i + self.size()] = Vector2Normalize({target[0].y - target[i].y, target[0].x - target[i].x});}
    }

    // FIND PROJECTION AXES
    std::vector<Vector2> projectionAxes;
    projectionAxes.reserve(normals.size());

    for (const auto& normal : normals) {
        if (projectionAxes.empty() < 1) {
            projectionAxes.push_back(normal);
        }

        else {
            bool found = false;
            for (const auto& axis : projectionAxes) {
                if (normal == axis){ found = true; break; }
            }
            if (!found) {
                projectionAxes.push_back(normal);
            }
        }
    }

    // PROJECT POINTS ONTO AXES
    for (int i = 0; i < projectionAxes.size(); i++) {

        float AProjections[self.size()];
        for (int j = 0; j < self.size(); j++) {
            AProjections[j] = Vector2DotProduct(self[j], projectionAxes[i]);
        }

        float AMin = *std::min_element(AProjections, AProjections + self.size());
        float AMax = *std::max_element(AProjections, AProjections + self.size());

        float BProjections[target.size()];
        for (int j = 0; j < target.size(); j++) {
            BProjections[j] = Vector2DotProduct(target[j], projectionAxes[i]);
        }

        float BMin = *std::min_element(BProjections, BProjections + target.size());
        float BMax = *std::max_element(BProjections, BProjections + target.size());

        if (BMin > AMax || BMax < AMin) {return false;} // CHECK COLLISION
    }
    return true;
}

void slideAndCollide(Box2D* box, std::vector<Box2D*>* collisionBoxes) {

    for (auto& box2D : *collisionBoxes) {
        if (box != box2D) {

            std::vector<Vector2> boxPoints = box->getPoints();
            for (auto & boxPoint : boxPoints) {
                boxPoint += box->velocity;
            }
            SATCollisionCheckAndResolution(box, boxPoints, box2D->getPoints());
        }
    }

}

bool InteractionObjectCollisionCheck(InteractionObject* obj1, Box2D* collisionBox) {
    if (obj1->type == INTERACTION_BOX_2D) {
        return CheckCollisionRecs(dynamic_cast<InteractionBox2D *>(obj1)->toRect(), {collisionBox->position.x, collisionBox->position.y, collisionBox->width, collisionBox->height});
    }

    if (obj1->type == INTERACTION_CIRCLE_2D) {
        return CheckCollisionCircleRec(obj1->position, dynamic_cast<InteractionCircle2D *>(obj1)->radius, {collisionBox->position.x, collisionBox->position.y, collisionBox->width, collisionBox->height});
    }

    if (obj1->type == INTERACTION_SHAPE_2D) {
        return SATCollisionCheck(dynamic_cast<InteractionShape2D *>(obj1)->points, collisionBox->getPoints());
    }

    return false;
}