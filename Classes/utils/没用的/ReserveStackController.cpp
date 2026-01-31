#include "ReserveStackController.h"
#include "models/CardModel.h"

ReserveStackController* ReserveStackController::create(GameModel* gameModel, GameView* gameView)
{
    auto controller = new (std::nothrow) ReserveStackController();
    if (controller && controller->init(gameModel, gameView))
    {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

bool ReserveStackController::init(GameModel* gameModel, GameView* gameView)
{
    if (!Node::init()) return false;

    _gameModel = gameModel;
    _gameView = gameView;
    _matchingService = MatchingService::getInstance();

    // 注册备用牌点击回调
    _gameView->getReserveLayer()->setTouchEnabled(true);
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event) {
        handleReserveCardClick();
        };
    _gameView->getReserveLayer()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _gameView->getReserveLayer());

    return true;
}

void ReserveStackController::handleReserveCardClick()
{
    // 游戏结束或暂停时不响应
    if (_gameModel->getGameState() == GameState::GAME_OVER ||
        _gameModel->getGameState() == GameState::PAUSED)
    {
        return;
    }

    // 如果备用堆为空，直接显示提示
    if (_gameModel->getReserveStack().empty())
    {
        showNoCardTip();
        return;
    }

    // 如果还有可匹配的牌，不允许抽牌（根据规则调整）
    if (hasMatchingCard())
    {
        return;
    }

    // 抽牌替换底牌
    drawNewBaseCard();
}

bool ReserveStackController::hasMatchingCard() const
{
    const auto& desktopCards = _gameModel->getDesktopCards();
    auto currentBaseCard = _gameModel->getBaseCard();
    if (!currentBaseCard) return false;

    // 检查桌面牌是否有匹配项
    for (const auto& card : desktopCards)
    {
        if (_matchingService->isPointDifferenceOne(const_cast<CardModel*>(card), currentBaseCard))
        {
            return true;
        }
    }

    // 检查手牌区是否有匹配项
    const auto& handCards = _gameModel->getHandCards();
    for (const auto& card : handCards)
    {
        if (_matchingService->isPointDifferenceOne(const_cast<CardModel*>(card), currentBaseCard))
        {
            return true;
        }
    }

    return false;
}

void ReserveStackController::drawNewBaseCard()
{
    // 取出堆顶备用牌
    auto newBaseCard = _gameModel->getReserveStack().back();
    _gameModel->getReserveStack().pop_back();

    // 移除备用牌视图
    _gameView->removeReserveCardView(newBaseCard->cardId);

    // 更新底牌模型
    _gameModel->setBaseCard(newBaseCard);
    newBaseCard->areaType = CardAreaType::CAT_BASE_STACK;

    // 执行底牌切换动画
    auto baseCardView = _gameView->updateBaseCardView(newBaseCard);
    baseCardView->runAction(cocos2d::Sequence::create(
        cocos2d::ScaleTo::create(0.2f, 1.2f),
        cocos2d::ScaleTo::create(0.2f, 1.0f),
        nullptr
    ));
}

void ReserveStackController::showNoCardTip() const
{
    auto tip = cocos2d::Label::createWithTTF("无牌可抽", "fonts/arial.ttf", 30);
    tip->setColor(cocos2d::Color3B::RED);
    tip->setPosition(_gameView->getReserveLayer()->getContentSize() / 2);
    _gameView->getReserveLayer()->addChild(tip);

    // 2秒后自动销毁提示
    tip->runAction(cocos2d::Sequence::create(
        cocos2d::DelayTime::create(2.0f),
        cocos2d::RemoveSelf::create(),
        nullptr
    ));
}