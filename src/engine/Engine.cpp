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
            render();
        }
        cleanUp();

    }

private:
    bool running = true;
    Camera2D camera;
    Player player;
    Map map;


    void init() {
        InitWindow(1280, 720, "Cozy Farming Game");

        camera = {0};
        camera.target = {0, 0};
        camera.zoom = 3.0f;
        camera.offset = {GetScreenWidth()/2.0f - 8, GetScreenHeight()/2.0f - 8}; // 8 is half the player sprite w and h


        player.init({0,0}, 1);

        tmx::Map tempMap;
        if (!tempMap.load("assets/maps/playerHouse.tmx")) {
            running = false;
            cleanUp();
            THROW_ERROR("Failed to load map");
        }

        map.loadMap(&tempMap, &player.position);

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


    void render() {
        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode2D(camera);

                map.render();
                player.render();

            EndMode2D();
        EndDrawing();
    }

    void cleanUp() {
        CloseWindow();
    }

};




