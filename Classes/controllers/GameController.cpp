#include "GameController.h"
#include "services/RecordService.h"
#include "models/CardModel.h"

GameController* GameController::create(GameModel* model, GameView* view)
{
    auto controller = new (std::nothrow) GameController();
    if (controller && controller->init(model, view))
    {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

GameController::~GameController()
{
    if (_gameModel) {
        _gameModel->release();
        _gameModel = nullptr;
    }
}

bool GameController::init(GameModel* model, GameView* view)
{
    if (!Node::init()) return false;

    _gameModel = model;
    if (_gameModel) _gameModel->retain();
    _gameView = view;
    _isPaused = false;
    _isGameOver = false;

    // ??????????
    _matchingService = MatchingService::getInstance();

    // ?? GameView ??????z=0??????? GameView ?
    this->addChild(_gameView, 0);

    // ???????
    _playFieldController = PlayFieldController::create(model, view);
    _stackController = StackController::create(model, view);
    this->addChild(_playFieldController, 1);
    this->addChild(_stackController, 1);

    // 绑定回调
    _gameView->onCardClicked = [this](int cardId) { this->onCardClicked(cardId); };
    _gameView->onUndoClicked = [this]() { this->onUndoClicked(); };

    return true;
}

void GameController::startGame()
{
    CCLOG("Game started");
    _isGameOver = false;
    RecordService::getInstance()->clearRecords();
    _matchingService->updateCurrentBaseCard(_gameModel->getBaseCard());
    _playFieldController->initPlayField();
    _stackController->initStacks();
}

void GameController::pauseGame()
{
    if (_isGameOver) return;
    _isPaused = true;
    CCLOG("Game paused");
}

void GameController::resumeGame()
{
    if (_isGameOver) return;
    _isPaused = false;
    CCLOG("Game resumed");
}

void GameController::endGame()
{
    _isGameOver = true;
    CCLOG("Game ended");
    // ????????????????????????????????????????
}

void GameController::onCardClicked(int cardId)
{
    if (isGamePaused() || isGameOver()) return;

    // ????????????????????????????
    auto cardModel = _gameModel->getCardById(cardId);
    if (!cardModel) return;

    switch (cardModel->areaType)
    {
    case CardAreaType::CAT_PLAYFIELD:
        _playFieldController->handleCardClick(cardId);
        break;
    case CardAreaType::CAT_BASE_STACK:
        _stackController->handleBaseCardClick(cardId);
        break;
    case CardAreaType::CAT_RESERVE_STACK:
        _stackController->handleReserveCardClick(cardId);
        break;
    default:
        CCLOG("Unknown card area type");
    }
}

void GameController::onMatchSuccess(CardModel* movedCard)
{
    if (!movedCard) return;

    // ?????????
    _gameModel->moveCard(movedCard, CardAreaType::CAT_BASE_STACK);
    _matchingService->updateCurrentBaseCard(movedCard);

    // ??????????????
    RecordService::getInstance()->addOperation(movedCard->cardId, movedCard->areaType);

    // ????????????
    checkGameCompletion();
}

void GameController::onMatchFailed(CardModel* movedCard)
{
    if (!movedCard) return;
    CCLOG("Card %d match failed", movedCard->cardId);
    // ????????????????????????????
    auto cardView = _gameView->getCardView(movedCard->cardId);
    if (cardView)
    {
        cardView->playFailedAnimation();
    }
}

bool GameController::isGamePaused() const
{
    return _isPaused;
}

bool GameController::isGameOver() const
{
    return _isGameOver;
}

void GameController::checkAndNotifyGameCompletion()
{
    checkGameCompletion();
}

void GameController::checkGameCompletion()
{
    if (_playFieldController->getPlayFieldCardCount() == 0)
    {
        endGame();
        CCLOG("Game completed! All cards matched.");
    }
}

void GameController::onUndoClicked()
{
    if (!RecordService::getInstance()->canUndo()) return;
    UndoEntry entry = RecordService::getInstance()->popUndoRecord();
    if (entry.movedCardId < 0) return;

    CardModel* movedCard = _gameModel->getCardById(entry.movedCardId);
    if (!movedCard) return;

    CardModel* oldBaseCard = (entry.oldBaseId >= 0) ? _gameModel->getCardById(entry.oldBaseId) : nullptr;
    CardModel* currentBase = _gameModel->getBaseCard();
    if (currentBase != movedCard) return;

    if (entry.fromPlayfield) {
        _gameModel->baseCard = nullptr;
        movedCard->areaType = CardAreaType::CAT_PLAYFIELD;
        movedCard->position = entry.oldPosition;
        _gameModel->playfieldCards.push_back(movedCard);
        if (oldBaseCard) _gameModel->setBaseCard(oldBaseCard);
        _matchingService->updateCurrentBaseCard(_gameModel->getBaseCard());
        _gameView->moveCardViewToPlayfield(entry.movedCardId, entry.oldPosition, nullptr);
        if (entry.oldBaseId >= 0) _gameView->moveCardViewToBaseLayer(entry.oldBaseId, nullptr);
    } else {
        _gameModel->baseCard = nullptr;
        movedCard->areaType = CardAreaType::CAT_RESERVE_STACK;
        _gameModel->reserveCards.push_back(movedCard);
        if (oldBaseCard) _gameModel->setBaseCard(oldBaseCard);
        _matchingService->updateCurrentBaseCard(_gameModel->getBaseCard());
        _gameView->moveCardViewToReserveLayer(entry.movedCardId, nullptr);
        if (entry.oldBaseId >= 0) _gameView->moveCardViewToBaseLayer(entry.oldBaseId, nullptr);
    }
}