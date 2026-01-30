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
    const float PLAYFIELD_HEIGHT = 1500.0f; // ?????
    const float STACKS_HEIGHT = 580.0f;     // ?????

    // ?????????????(1080*1500)
    _playfieldLayer = cocos2d::Node::create();
    _playfieldLayer->setContentSize(cocos2d::Size(DESIGN_WIDTH, PLAYFIELD_HEIGHT));
    _playfieldLayer->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
    _playfieldLayer->setPosition(cocos2d::Vec2(DESIGN_WIDTH / 2, STACKS_HEIGHT / 2 + 10)); // ??????10??
    this->addChild(_playfieldLayer);

    // ?????????????(300*580)
    _baseCardLayer = cocos2d::Node::create();
    _baseCardLayer->setContentSize(cocos2d::Size(300, STACKS_HEIGHT));
    _baseCardLayer->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _baseCardLayer->setPosition(cocos2d::Vec2(200, STACKS_HEIGHT / 2)); // ????(50px)??
    this->addChild(_baseCardLayer);

    // ??????????????(300*580)
    _reserveLayer = cocos2d::Node::create();
    _reserveLayer->setContentSize(cocos2d::Size(300, STACKS_HEIGHT));
    _reserveLayer->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _reserveLayer->setPosition(cocos2d::Vec2(DESIGN_WIDTH - 200, STACKS_HEIGHT / 2)); // ????(50px)??
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
    }
    else
    {
        _playfieldLayer->addChild(cardView);
    }

    // ????????
    cardView->setPosition(cardModel->position);
}

cocos2d::Node* GameView::getReserveLayer() const
{
    return _reserveLayer;
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