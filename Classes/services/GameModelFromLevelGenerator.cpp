#include "GameModelFromLevelGenerator.h"


GameModel* GameModelFromLevelGenerator::generateGameModel(const LevelConfig* levelConfig, int levelId)
{
    if (!levelConfig) return nullptr;

    auto gameModel = GameModel::create();
    gameModel->currentLevelId = levelId;
    int cardId = 0;

    // ???????????????
    for (const auto& cardConfig : levelConfig->playfieldCards)
    {
        auto cardModel = createCardModelFromConfig(cardConfig, cardId++);
        cardModel->state = CardModel::State::COVERED;
        cardModel->areaType = CardAreaType::CAT_PLAYFIELD;
        gameModel->playfieldCards.push_back(cardModel);
    }

    // ????????????
    auto baseModel = createCardModelFromConfig(levelConfig->baseCard, cardId++);
    baseModel->state = CardModel::State::FLIPPED;
    baseModel->areaType = CardAreaType::CAT_BASE_STACK;
    gameModel->baseCard = baseModel;

    // ??????????????
    for (const auto& cardConfig : levelConfig->reserveCards)
    {
        auto cardModel = createCardModelFromConfig(cardConfig, cardId++);
        cardModel->state = CardModel::State::COVERED;
        cardModel->areaType = CardAreaType::CAT_RESERVE_STACK;
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