//
// Created by kristijan on 9/13/25.
//

#ifndef TEMP_H
#define TEMP_H

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>


struct PlayerSpawn;

inline int getTileSet(uint32_t id, std::vector<std::pair<uint32_t, uint32_t>> tileSetIds) {
    for (int i = 0; i < tileSetIds.size(); i++) {
        const int min = tileSetIds[i].first;
        const int max = tileSetIds[i].second;
        if ((static_cast<int>(id) >= min) && (static_cast<int>(id) <= max)) {
            return i;
        }
    }

    return -1;
}


struct Door {
    int doorID;
    int mapID;
    PlayerSpawn* playerSpawn;
    Door* linkedDoor;
};

struct PlayerSpawn {
    Vector2 position;
};


struct Tile {
    uint32_t id;
    int x;
    int y;
    bool animated;
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

    Rectangle createAnimationFrameRect() {
        return Rectangle{static_cast<float>(16 * currentFrame),0,16,16};
    }
};

struct Map {
    std::vector<std::vector<TMXTexture>> layerTextures;
    std::vector<std::vector<TMXAnimatedTexture>> animatedLayerTextures;
    std::vector<Texture2D> tilesetTextures;
    std::vector<Rectangle> tilesetClipRects;
    std::vector<PlayerSpawn> playerSpawns;
    PlayerSpawn defaultPlayerSpawn;


    void animate() {
        double currentFrameTime = GetTime()*1000;
        for (auto& layer : animatedLayerTextures) {
            for (auto& animationTexture : layer) {
                if (currentFrameTime - animationTexture.lastFrameTime >= animationTexture.frameDelayTimes[animationTexture.currentFrame]) {
                    animationTexture.currentFrame = (animationTexture.currentFrame + 1) % animationTexture.frameCount;
                    animationTexture.lastFrameTime = currentFrameTime;
                }
            }
        }
    }

    void render() {
        for (int layer = 0; layer < animatedLayerTextures.size(); layer++) {

            // REGULAR TILES
            for (int texture = 0; texture < layerTextures[layer].size(); texture++) {
                DrawTextureRec(*layerTextures[layer][texture].texture, *layerTextures[layer][texture].clipRect, {static_cast<float>(layerTextures[layer][texture].x), static_cast<float>(layerTextures[layer][texture].y)}, RAYWHITE);
            }

            // ANIMATED TILES
            for (int texture = 0; texture < animatedLayerTextures[layer].size(); texture++) {
                DrawTextureRec(*animatedLayerTextures[layer][texture].texture, animatedLayerTextures[layer][texture].createAnimationFrameRect(), {static_cast<float>(animatedLayerTextures[layer][texture].x), static_cast<float>(animatedLayerTextures[layer][texture].y)}, RAYWHITE);
            }
        }

    };


    Map loadMap(tmx::Map *map, Vector2* playerPos) {
        uint32_t tilesetTileCount = 0;
        std::vector<std::string> tilesetTexturePaths;
        std::vector<std::pair<uint32_t, uint32_t>> tilesetIDs;
        std::vector<tmx::Tileset> tilesets = map->getTilesets();
        std::vector<bool> animatedTileset;
        std::vector<tmx::Tileset::Tile> tile;
        int tileSize = 16;
        std::vector<Tile> tileData;
        int counter = 0;

        for(const auto& tileset : tilesets){
            bool animated = false;
            tilesetTileCount += tileset.getTileCount();
            tilesetIDs.emplace_back(tileset.getFirstGID(), tileset.getLastGID());
            tilesetTexturePaths.emplace_back(tileset.getImagePath());
            tilesetTextures.emplace_back(LoadTexture(tileset.getImagePath().c_str()));


            for (const auto& t : tileset.getTiles()) {
                if (!t.animation.frames.empty()) { animated = true; }

                std::cout << "ID: " << t.ID << " Pos: " << t.imagePosition.x << ", " << t.imagePosition.y << " Size: " << t.imageSize.x << ", " << t.imageSize.y << std::endl;
                tilesetClipRects.emplace_back(Rectangle {static_cast<float>(t.imagePosition.x), static_cast<float>(t.imagePosition.y), static_cast<float>(t.imageSize.x), static_cast<float>(t.imageSize.y)});

            }

            animatedTileset.emplace_back(animated);


        }





        int currentLayer = 0;
        layerTextures.resize(map->getLayers().size());
        animatedLayerTextures.resize(map->getLayers().size());
        for (const auto& layer : map->getLayers()) {
            tileData.clear();
            tmx::Layer::Type layerType = layer->getType();

            if (layerType == tmx::Layer::Type::Tile) {
                tmx::TileLayer tileLayer = layer->getLayerAs<tmx::TileLayer>();
                std::vector<tmx::TileLayer::Chunk> chunks = tileLayer.getChunks();
                for (const auto& chunk : chunks) {
                    int chunkXOffset = chunk.position.x * chunk.size.x;
                    int chunkYOffset = chunk.position.y * chunk.size.y;
                    tileData.clear();
                    tileData.resize(0);
                    int counter = 0;

                    for (int j = 0; j < chunk.tiles.size() / chunk.size.y; j++) {
                        for (int z = 0; z < chunk.tiles.size() / chunk.size.x; z++) {
                            if (chunk.tiles[counter].ID != 0) {
                                Tile td;
                                td.id = chunk.tiles[counter].ID;
                                td.x = tileSize * z + chunkXOffset;
                                td.y = tileSize * j + chunkYOffset;
                                tileData.push_back(td);
                            }

                            counter++;
                        }
                    }

                    for (int i = 0; i < tileData.size(); i++) {
                        int tilesetID = getTileSet(tileData[i].id, tilesetIDs);

                        if (animatedTileset[tilesetID]) {
                            TMXAnimatedTexture textureData;

                            textureData.texture = &tilesetTextures[tilesetID];
                            textureData.frameCount = tilesets[tilesetID].getTileCount();
                            textureData.x = tileData[i].x;
                            textureData.y = tileData[i].y;

                            for (auto& frame : tilesets[tilesetID].getTiles()[0].animation.frames) {
                                textureData.frameDelayTimes.emplace_back(frame.duration);
                            }

                            animatedLayerTextures[currentLayer].emplace_back(textureData);



                        }

                        else {
                            TMXTexture textureData;

                            textureData.texture = &tilesetTextures[tilesetID];


                            for (int j = 0; j < tilesetClipRects.size(); j++) {
                                if (j + 1 == tileData[i].id) {
                                    std::cout << "ID: " << tileData[i].id << std::endl;
                                    textureData.clipRect = &tilesetClipRects[j];
                                }
                            }


                            textureData.x = tileData[i].x;
                            textureData.y = tileData[i].y;

                            layerTextures[currentLayer].emplace_back(textureData);
                        }

                    }


                }



                std::cout << "Tile layer\n";
            }

            else if (layerType == tmx::Layer::Type::Object) {
                tmx::ObjectGroup objectLayer = layer->getLayerAs<tmx::ObjectGroup>();

                if (objectLayer.getName() == "lightSource"){log("light");}

                else if (objectLayer.getName() == "playerSpawn") {
                    log("player spawn");
                    std::vector<tmx::Object> playerSpawnObjects = objectLayer.getObjects();
                    for (const auto& spawn : playerSpawnObjects) {
                        playerSpawns.emplace_back(PlayerSpawn{spawn.getPosition().x - (tileSize/2.0f), spawn.getPosition().y - (tileSize/2.0f)});
                    }
                    defaultPlayerSpawn = playerSpawns[0];
                    playerPos->x = defaultPlayerSpawn.position.x;
                    playerPos->y = defaultPlayerSpawn.position.y;
                }

                else {
                    std::vector<tmx::Object> objects = objectLayer.getObjects();
                    std::vector<Tile> tileData{objects.size()};
                    for (int i = 0; i < objects.size(); i++) {
                        tileData[i].id = objects[i].getTileID();
                        tileData[i].x = objects[i].getPosition().x;
                        tileData[i].y = objects[i].getPosition().y;
                    }


                    layerTextures[currentLayer].resize(tileData.size());
                    for (int i = 0; i < tileData.size(); i++) {

                        layerTextures[currentLayer][i].texture = &tilesetTextures[getTileSet(tileData[i].id, tilesetIDs)];

                        for (int j = 0; j < tilesetClipRects.size(); j++) {
                            if (j +1 == tileData[i].id) {
                                layerTextures[currentLayer][i].clipRect = &tilesetClipRects[j];
                            }
                        }

                        layerTextures[currentLayer][i].x = tileData[i].x;
                        layerTextures[currentLayer][i].y = tileData[i].y - 16;
                    }
                }



                std::cout << "Object layer\n";
            }
            else if (layerType == tmx::Layer::Type::Image) {
                std::cout << "Image layer\n";
            }
            else if (layerType == tmx::Layer::Type::Group) {
                std::cout << "Group layer\n";
            }
            else {
                std::cout << "Unsupported layer type\n";
            }

            currentLayer++;

        }
        return *this;
    }
};




#endif //TEMP_H
