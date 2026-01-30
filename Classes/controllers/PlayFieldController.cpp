#include "PlayFieldController.h"
#include "GameController.h"
#include "services/RecordService.h"

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
    // ?????????????????????
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

    // ??2????????????????1?
    if (_matchingService->isCardMatchable(cardModel))
    {
        auto baseCard = _gameModel->getBaseCard();
        auto oldBaseId = baseCard ? baseCard->cardId : -1;
        _gameModel->moveCard(cardModel, CardAreaType::CAT_BASE_STACK);
        _matchingService->updateCurrentBaseCard(cardModel);

        // ????????
        if (oldBaseId >= 0) {
            _gameView->moveCardViewToReserveLayer(oldBaseId, nullptr);
        }
        cocos2d::Vec2 oldPos = cardModel->position;
        _gameView->moveCardViewToBaseLayer(cardId, [=]() {
            revealUnderlyingCards(cardModel);
            auto gameController = dynamic_cast<GameController*>(this->getParent());
            if (gameController) gameController->checkAndNotifyGameCompletion();
        });
        RecordService::getInstance()->addUndoRecord(cardId, oldBaseId, true, oldPos);
    }
    else
    {
        // ?????????????????????
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

    // ????????????????
    return cardModel->isRevealed;
}

void PlayFieldController::revealUnderlyingCards(CardModel* removedCard)
{
    // ????????????????????????????
    auto playfieldCards = _gameModel->getPlayfieldCards();
    for (auto cardModel : playfieldCards)
    {
        if (cardModel->isCoveredBy(removedCard))
        {
            cardModel->isRevealed = true;
            auto cardView = _gameView->getCardView(cardModel->cardId);
            if (cardView)
            {
                cardView->playFlipAnimation(); // ?????????
            }
        }
    }
}