#include "DesktopAreaController.h"
#include "MatchingService.h"

DesktopAreaController* DesktopAreaController::create(GameModel* model, GameView* view)
{
    auto controller = new (std::nothrow) DesktopAreaController();
    if (controller && controller->init(model, view))
    {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

bool DesktopAreaController::init(GameModel* model, GameView* view)
{
    if (!Node::init()) return false;

    _gameModel = model;
    _gameView = view;
    _matchingService = MatchingService::getInstance();

    return true;
}

void DesktopAreaController::handleCardClick(int cardId)
{
    auto cardModel = _gameModel->getDesktopCardById(cardId);
    auto baseCard = _gameModel->getBaseCard();
    if (!cardModel || !baseCard) return;

    // ¼ì²éÆ¥Åä¹æÔò
    if (_matchingService->isPointDifferenceOne(cardModel, baseCard))
    {
        // Æ¥Åä³É¹¦£º¸üÐÂModelºÍView
        _gameModel->moveToBaseCard(cardModel);
        auto cardView = _gameView->getDesktopCardView(cardId);
        if (cardView)
        {
            auto basePosition = _gameView->getBaseCardPosition();
            cardView->runAction(cocos2d::Sequence::create(
                cocos2d::MoveTo::create(0.3f, basePosition), // ÒÆ¶¯µ½µ×ÅÆÎ»ÖÃ
                cocos2d::CallFunc::create([=]() {
                    _gameView->removeDesktopCardView(cardId); // ÒÆ³ýÅÆ
                    }),
                nullptr
            ));
        }
    }
    else
    {
        // Æ¥ÅäÊ§°Ü£º²¥·ÅÊ§°Ü¶¯»­
        auto cardView = _gameView->getDesktopCardView(cardId);
        if (cardView)
        {
            cardView->runAction(cocos2d::Sequence::create(
                cocos2d::RotateBy::create(0.1f, 5),
                cocos2d::RotateBy::create(0.1f, -10),
                cocos2d::RotateBy::create(0.1f, 5),
                nullptr
            ));
        }
    }
}