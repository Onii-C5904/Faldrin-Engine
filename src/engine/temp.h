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
};

struct TMXTexture {
    Texture2D* texture;
    Rectangle* clipRect;
    int x;
    int y;
    bool draw;
};

struct Map {
    std::vector<std::vector<TMXTexture>> layerTextures;
    std::vector<Texture2D> tilesetTextures;
    std::vector<Rectangle> tilesetClipRects;
    std::vector<PlayerSpawn> playerSpawns;
    PlayerSpawn defaultPlayerSpawn;


    void render() {

        for (auto& layer : layerTextures) {
            for (auto& texture : layer) {
                if (texture.draw) {
                    DrawTextureRec(*texture.texture, *texture.clipRect, {static_cast<float>(texture.x), static_cast<float>(texture.y)}, RAYWHITE);
                }
            }
        }
    };


    Map loadMap(tmx::Map *map, Vector2* playerPos) {
        uint32_t tilesetTileCount = 0;
        std::vector<std::string> tilesetTexturePaths;
        std::vector<std::pair<uint32_t, uint32_t>> tilesetIDs;
        const auto& tilesets = map->getTilesets();
        std::vector<tmx::Tileset::Tile> tile;
        int tileSize = 16;
        std::vector<Tile> tileData;
        int counter = 0;

        for(const auto& tileset : tilesets){
            tilesetTileCount += tileset.getTileCount();
            tilesetIDs.emplace_back(tileset.getFirstGID(), tileset.getLastGID());
            tilesetTexturePaths.emplace_back(tileset.getImagePath());
            tilesetTextures.emplace_back(LoadTexture(tileset.getImagePath().c_str()));

            for (const auto& t : tileset.getTiles()) {
                counter++;
                tilesetClipRects.emplace_back(Rectangle {static_cast<float>(t.imagePosition.x), static_cast<float>(t.imagePosition.y), static_cast<float>(t.imageSize.x), static_cast<float>(t.imageSize.y)});
            }

        }





        int currentLayer = 0;
        layerTextures.resize(map->getLayers().size());
        for (const auto& layer : map->getLayers()) {
            tileData.clear();
            tmx::Layer::Type layerType = layer->getType();

            if (layerType == tmx::Layer::Type::Tile) {
                tmx::TileLayer tileLayer = layer->getLayerAs<tmx::TileLayer>();
                std::vector<tmx::TileLayer::Chunk> chunks = tileLayer.getChunks();
                for (const auto& chunk : chunks) {
                    int chunkXOffset = chunk.position.x * chunk.size.x;
                    int chunkYOffset = chunk.position.y * chunk.size.y;
                    tileData.resize(chunk.tiles.size());
                    int counter = 0;

                    for (int j = 0; j < chunk.tiles.size() / chunk.size.y; j++) {
                        for (int z = 0; z < chunk.tiles.size() / chunk.size.x; z++) {
                            tileData[counter].id = chunk.tiles[counter].ID;
                            tileData[counter].x = tileSize * z + chunkXOffset;
                            tileData[counter].y = tileSize * j + chunkYOffset;
                            counter++;
                        }
                    }

                    for (int i = 0; i < tileData.size(); i++) {
                        TMXTexture textureData;

                        int tilesetIndex = getTileSet(tileData[i].id, tilesetIDs);


                        if (tilesetIndex >= 0) {
                            textureData.texture = &tilesetTextures[tilesetIndex];
                            textureData.draw = true;
                        }
                        else {
                            textureData.draw = false;
                        }

                        for (int j = 0; j < tilesetClipRects.size(); j++) {
                            if (j +1 == tileData[i].id) {
                                textureData.clipRect = &tilesetClipRects[j];
                            }
                        }


                        textureData.x = tileData[i].x;
                        textureData.y = tileData[i].y;

                        layerTextures[currentLayer].emplace_back(textureData);
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

                        int tilesetIndex = getTileSet(tileData[i].id, tilesetIDs);

                        if (tilesetIndex >= 0) {
                            layerTextures[currentLayer][i].texture = &tilesetTextures[tilesetIndex];
                            layerTextures[currentLayer][i].draw = true;
                        }
                        else {
                            layerTextures[currentLayer][i].draw = false;
                        }

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
