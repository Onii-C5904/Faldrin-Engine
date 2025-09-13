//
// Created by kristijan on 9/12/25.
//

#include <raylib.h>
#include "Player.h"



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

    void init() {
        InitWindow(1280, 720, "Cozy Farming Game");

        camera = {0};
        camera.target = {0, 0};
        camera.zoom = 3.0f;
        camera.offset = {0, 0};


        player.init({0,0}, 1);
    }


    void update() {
        player.update();
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

        player.render();

        EndMode2D();
        EndDrawing();
    }

    void cleanUp() {
        CloseWindow();
    }

};




