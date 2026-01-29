#include "HandAreaController.h"

HandAreaController* HandAreaController::create(GameModel* model, GameView* view)
{
    auto controller = new (std::nothrow) HandAreaController();
    if (controller && controller->init(model, view))
    {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

bool HandAreaController::init(GameModel* model, GameView* view)
{
    if (!Node::init()) return false;

    _gameModel = model;
    _gameView = view;

    return true;
}

void HandAreaController::handleCardClick(int cardId)
{
    auto cardModel = _gameModel->getHandCardById(cardId);
    if (!cardModel) return;

    // 如果点击的已经是顶部牌，直接返回
    if (cardModel->isTop) return;

    // 更新顶部牌逻辑
    auto oldTopCard = _gameModel->getTopHandCard();
    if (oldTopCard) oldTopCard->isTop = false;

    _gameModel->setTopHandCard(cardModel);
    cardModel->isTop = true;

    // 设置新顶部牌的动画
    auto cardView = _gameView->getHandCardView(cardId);
    auto topPosition = _gameView->getTopHandCardPosition();
    if (cardView) cardView->runAction(cocos2d::MoveTo::create(0.3f, topPosition));
}