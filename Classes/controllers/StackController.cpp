#include "StackController.h"
#include "GameController.h"

StackController* StackController::create(GameModel* model, GameView* view)
{
    auto controller = new (std::nothrow) StackController();
    if (controller && controller->init(model, view))
    {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

bool StackController::init(GameModel* model, GameView* view)
{
    if (!Node::init()) return false;

    _gameModel = model;
    _gameView = view;
    _matchingService = MatchingService::getInstance();

    return true;
}

void StackController::initStacks()
{
    // 初始化底牌堆
    auto baseCard = _gameModel->getBaseCard();
    if (baseCard && _gameView->getCardView(baseCard->cardId) == nullptr)
    {
        _gameView->createCardView(baseCard);
        updateBaseCardView();
    }

    // 初始化备用牌堆
    auto reserveCards = _gameModel->getReserveCards();
    for (auto cardModel : reserveCards)
    {
        if (_gameView->getCardView(cardModel->cardId) == nullptr)
        {
            _gameView->createCardView(cardModel);
        }
    }
    CCLOG("Stacks initialized with base card %d and %zu reserve cards",
        baseCard ? baseCard->cardId : -1, reserveCards.size());
}

void StackController::handleBaseCardClick(int cardId)
{
    // 底牌堆通常不可点击，除非有特殊玩法需求
    CCLOG("Base card clicked, but not playable in current mode");
}

void StackController::handleReserveCardClick(int cardId)
{
    auto cardModel = _gameModel->getCardById(cardId);
    if (!cardModel) return;

    // 检查是否匹配当前底牌
    if (_matchingService->isCardMatchable(cardModel))
    {
        // 匹配成功，切换底牌
        _gameModel->setBaseCard(cardModel);
        updateBaseCardView();

        // 从备用牌堆移除卡牌
        _gameView->removeCardView(cardId);
        CCLOG("Reserve card %d matched and moved to base", cardId);
    }
    else
    {
        // 匹配失败，反馈给用户
        auto gameController = dynamic_cast<GameController*>(this->getParent());
        if (gameController)
        {
            gameController->onMatchFailed(cardModel);
        }
    }
}

void StackController::updateBaseCardView()
{
    auto baseCard = _gameModel->getBaseCard();
    if (!baseCard) return;

    auto baseCardView = _gameView->getCardView(baseCard->cardId);
    if (baseCardView)
    {
        // 执行底牌切换动画
        baseCardView->runAction(cocos2d::Sequence::create(
            cocos2d::ScaleTo::create(0.1f, 1.2f),
            cocos2d::ScaleTo::create(0.1f, 1.0f),
            nullptr
        ));
    }
}

void StackController::shuffleReserveCards()
{
    // 可以在这里实现备用牌堆洗牌逻辑
    CCLOG("Reserve cards shuffled");
}