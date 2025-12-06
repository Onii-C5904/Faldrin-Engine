//
// Created by kristijan on 9/12/25.
//

#include <raylib.h>
#include "Player.h"
#include "../helper.h"
#include <vector>
#include <iostream>

#include "Physics.h"
#include "temp.h"
#include "../rendering/Rendering.h"


class Engine {
public:
    void run() {
        init();
        SetTargetFPS(60);
        while (running) {
            pollInput();
            update();
            animate();
            render();
        }
        cleanUp();

    }

private:
    bool running = true;
    Camera2D camera;
    Player player;
    std::vector<Map> maps{MAPID_COUNT};
    MAPID activeMap = HOUSE;
    std::vector<Box2D*> collisionBoxes;


    void init() {
        InitWindow(1280, 720, "Cozy Farming Game");

        camera = {0};
        camera.target = {0, 0};
        camera.zoom = 3.0f;
        camera.offset = {GetScreenWidth()/2.0f - 8, GetScreenHeight()/2.0f - 8}; // 8 is half the player sprite w and h


        player.init({0,0}, 1, &collisionBoxes);
        collisionBoxes.emplace_back(&player.physicsBox);

        tmx::Map tempMap;
        if (!tempMap.load("assets/maps/playerHouse.tmx")) {
            running = false;
            cleanUp();
            THROW_ERROR("Failed to load map");
        }

        maps[HOUSE].loadMap(&tempMap, HOUSE, &player, &collisionBoxes);

/*
        tmx::Map tempMap2;
        if (!tempMap2.load("assets/maps/mainIsland.tmx")) {
            running = false;
            cleanUp();
            THROW_ERROR("Failed to load map");
        }

        maps[MAIN_ISLAND].loadMap(&tempMap2, player.position, &collisionBoxes);
*/

    }

    void updateCamera() {
        camera.target = *player.position;
    }


    void update() {
        maps[activeMap].update();
        player.update();
        updateCamera();

    }

    void pollInput() {
        PollInputEvents();
        if (IsKeyPressed(KEY_ESCAPE)) {
            running = false;
        }

        if (IsKeyDown(KEY_W)){player.up = true;}
        if (IsKeyDown(KEY_S)){player.down = true;}
        if (IsKeyDown(KEY_A)){player.left = true;}
        if (IsKeyDown(KEY_D)){player.right = true;}

        if (IsKeyUp(KEY_W)){player.up = false;}
        if (IsKeyUp(KEY_S)){player.down = false;}
        if (IsKeyUp(KEY_A)){player.left = false;}
        if (IsKeyUp(KEY_D)){player.right = false;}


    }

    void animate() {
        maps[activeMap].animate();
    }


    void render() {
        std::vector<SpriteInstance> sprites {SpriteInstance {*player.position - player.physicsBox.offset, player.getSprite()} };
        auto renderQueue = buildRenderQueue(sprites, maps[activeMap].layerTextures, maps[activeMap].animatedLayerTextures);

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode2D(camera);

                for (const auto& item : renderQueue) {
                    item.render();
                }
                //maps[activeMap].render();
                //player.render();

                for (auto& box : collisionBoxes) {
                    box->render();
                }

            EndMode2D();

        // DEBUG DRAWS
        DrawText(("Frame Time: " + std::to_string(GetFrameTime())).c_str(), 10, 10, 20, GREEN);
        DrawText(("Frame Rate: " + std::to_string(1.0f / GetFrameTime())).c_str(), 10, 30, 20, GREEN);
        DrawText(("Position X: " + std::to_string(player.position->x)).c_str(), 10, 50, 20, GREEN);
        DrawText(("Position Y: " + std::to_string(player.position->y)).c_str(), 10, 70, 20, GREEN);

        EndDrawing();
    }

    void cleanUp() {
        CloseWindow();
    }

};




