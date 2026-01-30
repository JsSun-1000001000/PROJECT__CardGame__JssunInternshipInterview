#include "StackController.h"
#include "GameController.h"
#include "services/RecordService.h"

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
    // ??????????
    auto baseCard = _gameModel->getBaseCard();
    if (baseCard && _gameView->getCardView(baseCard->cardId) == nullptr)
    {
        _gameView->createCardView(baseCard);
        updateBaseCardView();
    }

    // ????????????
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
    // ????????????????????????????????
    CCLOG("Base card clicked, but not playable in current mode");
}

void StackController::handleReserveCardClick(int cardId)
{
    auto cardModel = _gameModel->getCardById(cardId);
    if (!cardModel) return;

    // ??1???????? - ???????????????
    auto oldBase = _gameModel->getBaseCard();
    _gameModel->setBaseCard(cardModel);
    _matchingService->updateCurrentBaseCard(cardModel);

    // ????????
    if (oldBase && oldBase->cardId != cardId) {
        _gameView->moveCardViewToReserveLayer(oldBase->cardId, nullptr);
    }
    // ????????
    _gameView->moveCardViewToBaseLayer(cardId, [=]() {
        updateBaseCardView();
    });
    RecordService::getInstance()->addUndoRecord(cardId, oldBase ? oldBase->cardId : -1, false, cocos2d::Vec2::ZERO);
}

void StackController::updateBaseCardView()
{
    auto baseCard = _gameModel->getBaseCard();
    if (!baseCard) return;

    auto baseCardView = _gameView->getCardView(baseCard->cardId);
    if (baseCardView)
    {
        // ?????????????
        baseCardView->runAction(cocos2d::Sequence::create(
            cocos2d::ScaleTo::create(0.1f, 1.2f),
            cocos2d::ScaleTo::create(0.1f, 1.0f),
            nullptr
        ));
    }
}

void StackController::shuffleReserveCards()
{
    // ?????????????????????????
    CCLOG("Reserve cards shuffled");
}