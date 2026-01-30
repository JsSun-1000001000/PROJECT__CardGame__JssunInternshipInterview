#include "GameView.h"
#include "models/CardModel.h"
#include "utils/CardTextureUtils.h"

GameView* GameView::create()
{
    auto view = new (std::nothrow) GameView();
    if (view && view->init())
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool GameView::init()
{
    if (!Node::init()) return false;

    // ????????????(1080*2080)
    cocos2d::Size designSize = cocos2d::Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

    this->setContentSize(designSize);

    this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    this->setPosition(this->getContentSize() / 2);

    initLayout();

    return true;
}

void GameView::initLayout()
{
    const float DESIGN_WIDTH = 1080.0f;
    const float DESIGN_HEIGHT = 2080.0f;
    const float VISIBLE_HEIGHT = 600.0f;   // FIXED_WIDTH 下可见高度约 607
    const float STACKS_HEIGHT = 180.0f;
    const float PLAYFIELD_HEIGHT = VISIBLE_HEIGHT - STACKS_HEIGHT;

    // 添加背景色（避免黑屏）
    auto bgLayer = cocos2d::LayerColor::create(cocos2d::Color4B(34, 139, 34, 255), DESIGN_WIDTH, DESIGN_HEIGHT);
    bgLayer->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    bgLayer->setPosition(DESIGN_WIDTH / 2, DESIGN_HEIGHT / 2);
    this->addChild(bgLayer, -1);

    // 主牌区：放在可见区 y 180~600，避免被裁掉
    _playfieldLayer = cocos2d::Node::create();
    _playfieldLayer->setContentSize(cocos2d::Size(DESIGN_WIDTH, PLAYFIELD_HEIGHT));
    _playfieldLayer->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
    _playfieldLayer->setPosition(DESIGN_WIDTH / 2, STACKS_HEIGHT);
    this->addChild(_playfieldLayer);

    // ?????????????(300*580)
    _baseCardLayer = cocos2d::Node::create();
    _baseCardLayer->setContentSize(cocos2d::Size(300, STACKS_HEIGHT));
    _baseCardLayer->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _baseCardLayer->setPosition(cocos2d::Vec2(200, STACKS_HEIGHT / 2));
    this->addChild(_baseCardLayer);

    // ??????????????(300*580)
    _reserveLayer = cocos2d::Node::create();
    _reserveLayer->setContentSize(cocos2d::Size(300, STACKS_HEIGHT));
    _reserveLayer->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _reserveLayer->setPosition(cocos2d::Vec2(DESIGN_WIDTH - 200, STACKS_HEIGHT / 2));
    this->addChild(_reserveLayer);

    // ?????????????????
#ifdef DEBUG
    auto playfieldBorder = cocos2d::DrawNode::create();
    playfieldBorder->drawRect(cocos2d::Vec2::ZERO, _playfieldLayer->getContentSize(), cocos2d::Color4F::RED);
    _playfieldLayer->addChild(playfieldBorder);

    auto baseBorder = cocos2d::DrawNode::create();
    baseBorder->drawRect(cocos2d::Vec2::ZERO, _baseCardLayer->getContentSize(), cocos2d::Color4F::GREEN);
    _baseCardLayer->addChild(baseBorder);

    auto reserveBorder = cocos2d::DrawNode::create();
    reserveBorder->drawRect(cocos2d::Vec2::ZERO, _reserveLayer->getContentSize(), cocos2d::Color4F::BLUE);
    _reserveLayer->addChild(reserveBorder);
#endif

    // 回退按钮
    auto undoLabel = cocos2d::Label::createWithTTF("回退", "fonts/arial.ttf", 36);
    auto undoItem = cocos2d::MenuItemLabel::create(undoLabel, [this](cocos2d::Ref*) {
        if (onUndoClicked) onUndoClicked();
    });
    undoItem->setPosition(DESIGN_WIDTH - 150, STACKS_HEIGHT / 2);
    auto undoMenu = cocos2d::Menu::create(undoItem, nullptr);
    undoMenu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(undoMenu, 10);
}

CardView* GameView::createCardView(CardModel* cardModel)
{
    if (!cardModel) return nullptr;

    // ??????
    auto it = _cardViewMap.find(cardModel->cardId);
    if (it != _cardViewMap.end())
    {
        it->second->updateView(true); // ??????
        return it->second;
    }

    auto cardView = CardView::create(cardModel);
    if (cardView)
    {
        addCardToLayer(cardView, cardModel);
        _cardViewMap[cardModel->cardId] = cardView;

        // ???????????
        cardView->onCardClicked = [=](int cardId) {
            if (onCardClicked) onCardClicked(cardId);
            };
    }

    return cardView;
}

void GameView::removeCardView(int cardId)
{
    auto it = _cardViewMap.find(cardId);
    if (it != _cardViewMap.end())
    {
        it->second->removeFromParent();
        _cardViewMap.erase(it);
    }
}

CardView* GameView::getCardView(int cardId)
{
    auto it = _cardViewMap.find(cardId);
    return (it != _cardViewMap.end()) ? it->second : nullptr;
}

void GameView::refreshAllCardViews()
{
    for (auto& pair : _cardViewMap)
    {
        pair.second->updateView(true);
    }
}

void GameView::addCardToLayer(CardView* cardView, CardModel* cardModel)
{
    if (!cardView || !cardModel) return;

    // ???????????????
    if (cardModel->state == CardModel::State::ELIMINATED)
    {
        return; // ?????????
    }

    // ?? areaType ??????
    if (cardModel->areaType == CardAreaType::CAT_BASE_STACK)
    {
        _baseCardLayer->addChild(cardView);
        cardView->setPosition(_baseCardLayer->getContentSize() / 2);
        return;
    }

    if (cardModel->areaType == CardAreaType::CAT_RESERVE_STACK)
    {
        _reserveLayer->addChild(cardView);
        cardView->setPosition(_reserveLayer->getContentSize() / 2);
        return;
    }
    else
    {
        _playfieldLayer->addChild(cardView);
    }

    cardView->setPosition(cardModel->position);
}

cocos2d::Node* GameView::getReserveLayer() const
{
    return _reserveLayer;
}

cocos2d::Vec2 GameView::getBaseCardPosition() const
{
    if (!_baseCardLayer) return cocos2d::Vec2::ZERO;
    return _baseCardLayer->convertToWorldSpace(_baseCardLayer->getContentSize() / 2);
}

void GameView::removeReserveCardView(int cardId)
{
    removeCardView(cardId);
}

CardView* GameView::updateBaseCardView(CardModel* newBaseCard)
{
    if (!newBaseCard) return nullptr;
    auto view = getCardView(newBaseCard->cardId);
    if (view) {
        view->updateView(true);
    }
    return view;
}

void GameView::moveCardViewToBaseLayer(int cardId, std::function<void()> onComplete)
{
    auto view = getCardView(cardId);
    if (!view || !_baseCardLayer) return;
    auto targetPos = _baseCardLayer->getContentSize() / 2;
    view->retain();
    view->removeFromParent();
    _baseCardLayer->addChild(view);
    view->release();
    view->runAction(cocos2d::Sequence::create(
        cocos2d::MoveTo::create(0.3f, targetPos),
        cocos2d::CallFunc::create([onComplete]() { if (onComplete) onComplete(); }),
        nullptr
    ));
}

void GameView::moveCardViewToReserveLayer(int cardId, std::function<void()> onComplete)
{
    auto view = getCardView(cardId);
    if (!view || !_reserveLayer) return;
    auto targetPos = _reserveLayer->getContentSize() / 2;
    view->retain();
    view->removeFromParent();
    _reserveLayer->addChild(view);
    view->release();
    view->runAction(cocos2d::Sequence::create(
        cocos2d::MoveTo::create(0.3f, targetPos),
        cocos2d::CallFunc::create([onComplete]() { if (onComplete) onComplete(); }),
        nullptr
    ));
}

void GameView::moveCardViewToPlayfield(int cardId, const cocos2d::Vec2& position, std::function<void()> onComplete)
{
    auto view = getCardView(cardId);
    if (!view || !_playfieldLayer) return;
    view->retain();
    view->removeFromParent();
    _playfieldLayer->addChild(view);
    view->release();
    view->runAction(cocos2d::Sequence::create(
        cocos2d::MoveTo::create(0.3f, position),
        cocos2d::CallFunc::create([onComplete]() { if (onComplete) onComplete(); }),
        nullptr
    ));
}