#include "GameModelFromLevelGenerator.h"


GameModel* GameModelFromLevelGenerator::generateGameModel(const LevelConfig* levelConfig, int levelId)
{
    if (!levelConfig) return nullptr;

    auto gameModel = GameModel::create();
    gameModel->currentLevelId = levelId;
    int cardId = 0;

    // 生成主牌区卡牌：初始状态为覆盖
    for (const auto& cardConfig : levelConfig->playfieldCards)
    {
        auto cardModel = createCardModelFromConfig(cardConfig, cardId++);
        cardModel->state = CardModel::State::COVERED;
        gameModel->playfieldCards.push_back(cardModel);
    }

    // 生成底牌：初始状态为翻开
    auto baseModel = createCardModelFromConfig(levelConfig->baseCard, cardId++);
    baseModel->state = CardModel::State::FLIPPED;
    gameModel->baseCard = baseModel;

    // 生成备用牌堆：初始状态为覆盖
    for (const auto& cardConfig : levelConfig->reserveCards)
    {
        auto cardModel = createCardModelFromConfig(cardConfig, cardId++);
        cardModel->state = CardModel::State::COVERED;
        gameModel->reserveCards.push_back(cardModel);
    }

    // 默认主牌区第一个卡牌为顶部可点击卡牌（可根据关卡配置调整）
    if (!gameModel->playfieldCards.empty())
    {
        gameModel->topPlayfieldCardId = gameModel->playfieldCards.front()->cardId;
    }

    return gameModel;
}

CardModel* GameModelFromLevelGenerator::createCardModelFromConfig(const CardConfig& config, int cardId)
{
    auto cardModel = CardModel::create();
    cardModel->cardId = cardId;
    cardModel->faceType = config.faceType;
    cardModel->suitType = config.suitType;
    cardModel->position = config.position;
    return cardModel;
}