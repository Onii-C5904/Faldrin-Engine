//
// Created by kristijan on 9/12/25.
//

#include <raylib.h>
#include "Player.h"
#include "../helper.h"
#include <vector>
#include <iostream>

#include "temp.h"


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
    std::vector<Map> maps{2};


    void init() {
        InitWindow(1280, 720, "Cozy Farming Game");

        camera = {0};
        camera.target = {0, 0};
        camera.zoom = 3.0f;
        camera.offset = {GetScreenWidth()/2.0f - 8, GetScreenHeight()/2.0f - 8}; // 8 is half the player sprite w and h


        player.init({0,0}, 1);
/*
        tmx::Map tempMap;
        if (!tempMap.load("assets/maps/playerHouse.tmx")) {
            running = false;
            cleanUp();
            THROW_ERROR("Failed to load map");
        }

        maps[0].loadMap(&tempMap, &player.position);
*/
        tmx::Map tempMap2;
        if (!tempMap2.load("assets/maps/mainIsland.tmx")) {
            running = false;
            cleanUp();
            THROW_ERROR("Failed to load map");
        }

        maps[1].loadMap(&tempMap2, &player.position);

    }

    void updateCamera() {
        camera.target = player.position;
    }


    void update() {
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
        maps[1].animate();
    }


    void render() {
        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode2D(camera);

                maps[1].render();
                player.render();

            EndMode2D();

        // DEBUG DRAWS
        DrawText(("Frame Time: " + std::to_string(GetFrameTime())).c_str(), 10, 10, 20, GREEN);
        DrawText(("Frame Rate: " + std::to_string(1.0f / GetFrameTime())).c_str(), 10, 30, 20, GREEN);

        EndDrawing();
    }

    void cleanUp() {
        CloseWindow();
    }

};




