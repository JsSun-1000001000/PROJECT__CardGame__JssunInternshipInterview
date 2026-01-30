#include "CardTextureUtils.h"

// CardFaceType 0-12 ?? A, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K
static const char* kFaceTypeToFileName[] = {
    "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"
};

cocos2d::Sprite* CardTextureUtils::createCardFrontSprite(CardModel* cardModel, bool isBig)
{
    if (!cardModel) return nullptr;

    int faceIdx = static_cast<int>(cardModel->faceType);
    if (faceIdx < 0 || faceIdx >= 13) {
        CCLOG("CardTextureUtils: invalid faceType %d", faceIdx);
        return nullptr;
    }

    std::string numberFolder = isBig ? "big_" : "small_";
    std::string numberColorFolder =
        (cardModel->suitType == CardSuitType::CST_HEARTS || cardModel->suitType == CardSuitType::CST_DIAMONDS)
        ? "red_"
        : "black_";

    std::string numberFile = "res/number/" + numberFolder + numberColorFolder +
        std::string(kFaceTypeToFileName[faceIdx]) + ".png";

    auto numberSprite = cocos2d::Sprite::create(numberFile);
    if (!numberSprite)
    {
        CCLOG("Failed to load card number texture: %s", numberFile.c_str());
        return nullptr;
    }

    std::string suitFile = "res/suits/";
    switch (cardModel->suitType)
    {
    case CardSuitType::CST_CLUBS:
        suitFile += "club.png";
        break;
    case CardSuitType::CST_DIAMONDS:
        suitFile += "diamond.png";
        break;
    case CardSuitType::CST_HEARTS:
        suitFile += "heart.png";
        break;
    case CardSuitType::CST_SPADES:
        suitFile += "spade.png";
        break;
    default:
        CCLOG("Invalid CardSuitType");
        return nullptr;
    }

    auto suitSprite = cocos2d::Sprite::create(suitFile);
    if (!suitSprite)
    {
        CCLOG("Failed to load card suit texture: %s", suitFile.c_str());
        return nullptr;
    }

    std::string backgroundFile = "res/card_general.png";
    auto backgroundSprite = cocos2d::Sprite::create(backgroundFile);
    if (!backgroundSprite)
    {
        CCLOG("Failed to load card background texture: %s", backgroundFile.c_str());
        return nullptr;
    }

    numberSprite->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);
    numberSprite->setPosition(cocos2d::Vec2(20, backgroundSprite->getContentSize().height - 20));
    backgroundSprite->addChild(numberSprite);

    suitSprite->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    suitSprite->setPosition(backgroundSprite->getContentSize() / 2);
    backgroundSprite->addChild(suitSprite);

    return backgroundSprite;
}

cocos2d::Sprite* CardTextureUtils::createCardBackSprite()
{
    std::string backFile = "res/card_general.png";
    auto backSprite = cocos2d::Sprite::create(backFile);
    if (!backSprite)
    {
        CCLOG("Failed to load card back texture: %s", backFile.c_str());
        return nullptr;
    }
    return backSprite;
}
