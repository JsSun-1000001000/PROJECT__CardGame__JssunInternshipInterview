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

    // 设置容器大小为设计分辨率(1080*2080)
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
    const float PLAYFIELD_HEIGHT = 1500.0f; // 主牌区高度
    const float STACKS_HEIGHT = 580.0f;     // 牌堆区高度

    // 主牌区层：覆盖屏幕上半部分(1080*1500)
    _playfieldLayer = cocos2d::Node::create();
    _playfieldLayer->setContentSize(cocos2d::Size(DESIGN_WIDTH, PLAYFIELD_HEIGHT));
    _playfieldLayer->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
    _playfieldLayer->setPosition(cocos2d::Vec2(DESIGN_WIDTH / 2, STACKS_HEIGHT / 2 + 10)); // 与牌堆区间隔10像素
    this->addChild(_playfieldLayer);

    // 底牌堆层：屏幕下半部分左侧(300*580)
    _baseCardLayer = cocos2d::Node::create();
    _baseCardLayer->setContentSize(cocos2d::Size(300, STACKS_HEIGHT));
    _baseCardLayer->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _baseCardLayer->setPosition(cocos2d::Vec2(200, STACKS_HEIGHT / 2)); // 从左边界(50px)缩进
    this->addChild(_baseCardLayer);

    // 备用牌堆层：屏幕下半部分右侧(300*580)
    _reserveLayer = cocos2d::Node::create();
    _reserveLayer->setContentSize(cocos2d::Size(300, STACKS_HEIGHT));
    _reserveLayer->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _reserveLayer->setPosition(cocos2d::Vec2(DESIGN_WIDTH - 200, STACKS_HEIGHT / 2)); // 从右边界(50px)缩进
    this->addChild(_reserveLayer);

    // 调试用区域边框（开发完成后可删除）
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

    // 避免重复创建
    auto it = _cardViewMap.find(cardModel->cardId);
    if (it != _cardViewMap.end())
    {
        it->second->updateView(true); // 强制刷新状态
        return it->second;
    }

    auto cardView = CardView::create(cardModel);
    if (cardView)
    {
        addCardToLayer(cardView, cardModel);
        _cardViewMap[cardModel->cardId] = cardView;

        // 转发点击事件到控制器层
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

    // 根据卡牌类型和状态添加到对应层
    if (cardModel->state == CardModel::State::ELIMINATED)
    {
        return; // 已消除的卡牌不显示
    }

    // 底牌堆中的卡牌添加到底牌层
    if (cardModel->cardId == 0) // 约定底牌ID为0
    {
        _baseCardLayer->addChild(cardView);
        cardView->setPosition(_baseCardLayer->getContentSize() / 2); // 居中显示
        return;
    }

    // 备用牌堆中的卡牌添加到备用层
    bool isReserveCard = false;
    // 这里需要根据自定义逻辑判断是否为备用牌（比如通过position或者特殊ID规则）
    if (cardModel->position.x > 800) // 简化判断：X坐标大于800的牌放在备用区
    {
        isReserveCard = true;
    }

    if (isReserveCard)
    {
        _reserveLayer->addChild(cardView);
    }
    else
    {
        _playfieldLayer->addChild(cardView);
    }

    // 同步模型中的位置
    cardView->setPosition(cardModel->position);
}