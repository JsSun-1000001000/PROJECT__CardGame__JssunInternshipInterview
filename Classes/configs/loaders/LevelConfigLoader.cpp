#include "LevelConfigLoader.h"
#include "cocos2d.h"
#include "utils/CardEnum.h"
#include "document.h"
#include "filereadstream.h"
#include <cstdio>

USING_NS_CC;

// JSON 中 CardFace: 0-12 对应 CFT_ACE 到 CFT_KING
static CardFaceType jsonFaceToEnum(int face) {
    if (face >= 0 && face <= 12) {
        return static_cast<CardFaceType>(face);
    }
    return CardFaceType::CFT_NONE;
}

// JSON 中 CardSuit: 0-3 对应 CST_CLUBS, CST_DIAMONDS, CST_HEARTS, CST_SPADES
static CardSuitType jsonSuitToEnum(int suit) {
    if (suit >= 0 && suit <= 3) {
        return static_cast<CardSuitType>(suit);
    }
    return CardSuitType::CST_NONE;
}

LevelConfig* LevelConfigLoader::loadLevel(int levelId)
{
    char pathBuf[256];
    snprintf(pathBuf, sizeof(pathBuf), "levels/level_%d.json", levelId);

    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(pathBuf);
    if (fullPath.empty()) {
        CCLOG("LevelConfigLoader: level file not found: %s", pathBuf);
        return nullptr;
    }

    FILE* fp = fopen(fullPath.c_str(), "rb");
    if (!fp) {
        CCLOG("LevelConfigLoader: cannot open file: %s", fullPath.c_str());
        return nullptr;
    }

    char readBuf[65536];
    rapidjson::FileReadStream is(fp, readBuf, sizeof(readBuf));
    rapidjson::Document doc;
    doc.ParseStream(is);
    fclose(fp);

    if (doc.HasParseError()) {
        CCLOG("LevelConfigLoader: JSON parse error");
        return nullptr;
    }

    auto levelConfig = LevelConfig::create();
    if (!levelConfig) return nullptr;

    // 解析 Playfield 主牌区
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const auto& arr = doc["Playfield"].GetArray();
        for (rapidjson::SizeType i = 0; i < arr.Size(); ++i) {
            const auto& item = arr[i];
            CardConfig cfg;
            cfg.faceType = jsonFaceToEnum(item.HasMember("CardFace") ? item["CardFace"].GetInt() : 0);
            cfg.suitType = jsonSuitToEnum(item.HasMember("CardSuit") ? item["CardSuit"].GetInt() : 0);
            if (item.HasMember("Position") && item["Position"].IsObject()) {
                cfg.position.x = item["Position"].HasMember("x") ? item["Position"]["x"].GetFloat() : 0;
                cfg.position.y = item["Position"].HasMember("y") ? item["Position"]["y"].GetFloat() : 0;
            }
            levelConfig->playfieldCards.push_back(cfg);
        }
    }

    // 解析 Stack 卡堆区：第一张为底牌，其余为备用牌
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const auto& arr = doc["Stack"].GetArray();
        for (rapidjson::SizeType i = 0; i < arr.Size(); ++i) {
            const auto& item = arr[i];
            CardConfig cfg;
            cfg.faceType = jsonFaceToEnum(item.HasMember("CardFace") ? item["CardFace"].GetInt() : 0);
            cfg.suitType = jsonSuitToEnum(item.HasMember("CardSuit") ? item["CardSuit"].GetInt() : 0);
            if (item.HasMember("Position") && item["Position"].IsObject()) {
                cfg.position.x = item["Position"].HasMember("x") ? item["Position"]["x"].GetFloat() : 0;
                cfg.position.y = item["Position"].HasMember("y") ? item["Position"]["y"].GetFloat() : 0;
            }
            if (i == 0) {
                levelConfig->baseCard = cfg;
            } else {
                levelConfig->reserveCards.push_back(cfg);
            }
        }
    }

    // 若 Stack 为空，设置默认底牌（A）以便游戏能启动
    if (levelConfig->baseCard.faceType == CardFaceType::CFT_NONE) {
        levelConfig->baseCard.faceType = CardFaceType::CFT_ACE;
        levelConfig->baseCard.suitType = CardSuitType::CST_CLUBS;
    }

    return levelConfig;
}
