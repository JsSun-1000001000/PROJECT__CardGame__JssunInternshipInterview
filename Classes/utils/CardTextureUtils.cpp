#include "CardTextureUtils.h"


cocos2d::Sprite* CardTextureUtils::createCardFrontSprite(CardModel* cardModel, bool isBig)
{
    if (!cardModel) return nullptr;

    // 加载卡牌数字资源
    std::string numberFolder = isBig ? "big_" : "small_"; // 根据大小选择资源目录
    std::string numberColorFolder =
        (cardModel->suitType == CardSuitType::CST_HEARTS || cardModel->suitType == CardSuitType::CST_DIAMONDS)
        ? "red_"
        : "black_";

    std::string numberFile = "../Resources/res/number/" + numberFolder + numberColorFolder +
        std::to_string(static_cast<int>(cardModel->faceType)) + ".png";

    // 创建数字图片精灵
    auto numberSprite = cocos2d::Sprite::create(numberFile);
    if (!numberSprite)
    {
        CCLOG("Failed to load card number texture: %s", numberFile.c_str());
        return nullptr;
    }

    // 加载卡牌花色资源
    std::string suitFile = "../Resources/res/suits/";
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

    // 创建花色图片精灵
    auto suitSprite = cocos2d::Sprite::create(suitFile);
    if (!suitSprite)
    {
        CCLOG("Failed to load card suit texture: %s", suitFile.c_str());
        return nullptr;
    }

    // 加载卡面背景资源
    std::string backgroundFile = "../Resources/res/card_general.png";
    auto backgroundSprite = cocos2d::Sprite::create(backgroundFile);
    if (!backgroundSprite)
    {
        CCLOG("Failed to load card background texture: %s", backgroundFile.c_str());
        return nullptr;
    }

    // 将数字和花色添加到卡面背景上
    numberSprite->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);
    numberSprite->setPosition(cocos2d::Vec2(20, backgroundSprite->getContentSize().height - 20)); // 调整数字位置
    backgroundSprite->addChild(numberSprite);

    suitSprite->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    suitSprite->setPosition(backgroundSprite->getContentSize() / 2); // 花色图片居中
    backgroundSprite->addChild(suitSprite);

    return backgroundSprite;
}

cocos2d::Sprite* CardTextureUtils::createCardBackSprite()
{
    // 加载卡牌背板资源
    std::string backFile = "../Resources/res/card_general.png";
    auto backSprite = cocos2d::Sprite::create(backFile);
    if (!backSprite)
    {
        CCLOG("Failed to load card back texture: %s", backFile.c_str());
        return nullptr;
    }
    return backSprite;
}