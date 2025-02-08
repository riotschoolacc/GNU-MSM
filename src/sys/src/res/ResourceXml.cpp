// ResourceXml.cpp
#include "../Singleton.h"
#include <iostream>
#include <tinyxml2.h>
#include <string>
#include <vector>

using namespace tinyxml2;

void parseTextureAtlas(const char* filename, TextureAtlas& atlas, std::vector<Sprite>& sprites) {
    XMLDocument doc;
    XMLError eResult = doc.LoadFile(filename);

    if (eResult != XML_SUCCESS) {
        std::cerr << "Error loading file: " << filename << std::endl;
        return;
    }

    XMLNode* pRoot = doc.FirstChildElement("TextureAtlas");

    if (pRoot == nullptr) {
        std::cerr << "No TextureAtlas node found." << std::endl;
        return;
    }

    // Removed duplicate search for TextureAtlas node
    XMLElement* pAtlasElement = pRoot->ToElement();
    if (pAtlasElement) {
        if (strstr(pAtlasElement->Attribute("imagePath"), ".png")) {
            std::string newPath(pAtlasElement->Attribute("imagePath"));
            size_t pos = newPath.rfind(".png");
            if (pos != std::string::npos) {
                newPath.replace(pos, 4, ".avif");
            }
            atlas.imagePath = newPath;
        }
        atlas.width = pAtlasElement->IntAttribute("width");
        atlas.height = pAtlasElement->IntAttribute("height");
    }

    XMLElement* pSpriteElement = pRoot->FirstChildElement("sprite");
    while (pSpriteElement != nullptr) {
        Sprite sprite;
        sprite.name = pSpriteElement->Attribute("n");

        sprite.x = pSpriteElement->IntAttribute("x");
        sprite.y = pSpriteElement->IntAttribute("y");
        sprite.width = pSpriteElement->IntAttribute("w");
        sprite.height = pSpriteElement->IntAttribute("h");
        sprite.pivotX = pSpriteElement->FloatAttribute("pX", 0.5f);
        sprite.pivotY = pSpriteElement->FloatAttribute("pY", 0.5f);
        sprite.offsetX = pSpriteElement->IntAttribute("oX", 0);
        sprite.offsetY = pSpriteElement->IntAttribute("oY", 0);
        sprite.offsetWidth = pSpriteElement->IntAttribute("oW", sprite.width);
        sprite.offsetHeight = pSpriteElement->IntAttribute("oH", sprite.height);
        sprite.rotated = (pSpriteElement->Attribute("r") != nullptr);

        sprites.push_back(sprite);

        pSpriteElement = pSpriteElement->NextSiblingElement("sprite");
    }
}
