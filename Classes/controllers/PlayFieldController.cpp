#include "PlayFieldController.h"
#include "GameController.h"

PlayFieldController* PlayFieldController::create(GameModel* model, GameView* view)
{
    auto controller = new (std::nothrow) PlayFieldController();
    if (controller && controller->init(model, view))
    {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

bool PlayFieldController::init(GameModel* model, GameView* view)
{
    if (!Node::init()) return false;

    _gameModel = model;
    _gameView = view;
    _matchingService = MatchingService::getInstance();

    return true;
}

void PlayFieldController::initPlayField()
{
    // 从游戏模型加载主牌区卡牌
    auto playfieldCards = _gameModel->getPlayfieldCards();
    for (auto cardModel : playfieldCards)
    {
        if (_gameView->getCardView(cardModel->cardId) == nullptr)
        {
            _gameView->createCardView(cardModel);
        }
    }
    CCLOG("Playfield initialized with %zu cards", playfieldCards.size());
}

void PlayFieldController::handleCardClick(int cardId)
{
    auto cardModel = _gameModel->getCardById(cardId);
    if (!cardModel || !isCardPlayable(cardId))
    {
        return;
    }

    // 检查是否匹配当前底牌
    if (_matchingService->isCardMatchable(cardModel))
    {
        // 匹配成功，通知主控制器处理
        auto gameController = dynamic_cast<GameController*>(this->getParent());
        if (gameController)
        {
            gameController->onMatchSuccess(cardModel);
        }

        // 从主牌区移除卡牌
        _gameView->removeCardView(cardId);
        revealUnderlyingCards(cardModel);
    }
    else
    {
        // 匹配失败，通知主控制器处理
        auto gameController = dynamic_cast<GameController*>(this->getParent());
        if (gameController)
        {
            gameController->onMatchFailed(cardModel);
        }
    }
}

int PlayFieldController::getPlayFieldCardCount() const
{
    return static_cast<int>(_gameModel->getPlayfieldCards().size());
}

bool PlayFieldController::isCardPlayable(int cardId)
{
    auto cardModel = _gameModel->getCardById(cardId);
    if (!cardModel || cardModel->state == CardModel::State::ELIMINATED)
    {
        return false;
    }

    // 检查卡牌是否被上层卡牌覆盖
    return cardModel->isRevealed;
}

void PlayFieldController::revealUnderlyingCards(CardModel* removedCard)
{
    // 检查是否有下层卡牌被当前移除的卡牌覆盖
    auto playfieldCards = _gameModel->getPlayfieldCards();
    for (auto cardModel : playfieldCards)
    {
        if (cardModel->isCoveredBy(removedCard))
        {
            cardModel->isRevealed = true;
            auto cardView = _gameView->getCardView(cardModel->cardId);
            if (cardView)
            {
                cardView->playFlipAnimation(); // 执行翻转动画
            }
        }
    }
}