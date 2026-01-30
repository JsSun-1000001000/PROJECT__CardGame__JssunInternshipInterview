#include "GameController.h"
#include "services/RecordService.h"

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

bool GameController::init(GameModel* model, GameView* view)
{
    if (!Node::init()) return false;

    _gameModel = model;
    _gameView = view;
    _isPaused = false;
    _isGameOver = false;

    // 初始化服务层
    _matchingService = MatchingService::getInstance();

    // 初始化子控制器
    _playFieldController = PlayFieldController::create(model, view);
    _stackController = StackController::create(model, view);
    this->addChild(_playFieldController);
    this->addChild(_stackController);

    // 绑定全局点击事件
    _gameView->onCardClicked = CC_CALLBACK_1(GameController::onCardClicked, this);

    return true;
}

void GameController::startGame()
{
    CCLOG("Game started");
    _isGameOver = false;
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
    // 可以在这里添加游戏结束逻辑，比如显示结算界面
}

void GameController::onCardClicked(int cardId)
{
    if (isGamePaused() || isGameOver()) return;

    // 检查卡牌归属并分发到对应控制器处理
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

    // 更新游戏状态
    _gameModel->moveCard(movedCard, CardAreaType::CAT_BASE_STACK);
    _matchingService->updateCurrentBaseCard(movedCard);

    // 记录操作（可选）
    RecordService::getInstance()->addOperation(movedCard->cardId, movedCard->areaType);

    // 检查游戏是否完成
    checkGameCompletion();
}

void GameController::onMatchFailed(CardModel* movedCard)
{
    if (!movedCard) return;
    CCLOG("Card %d match failed", movedCard->cardId);
    // 可以添加失败反馈，比如卡牌震动动画
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

void GameController::checkGameCompletion()
{
    if (_playFieldController->getPlayFieldCardCount() == 0)
    {
        endGame();
        CCLOG("Game completed! All cards matched.");
    }
}