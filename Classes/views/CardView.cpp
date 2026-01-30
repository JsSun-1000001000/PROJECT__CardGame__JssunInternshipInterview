#include "CardView.h"
// views/CardView.cpp
#include "utils/CardTextureUtils.h"  // 工具类，用于加载卡牌纹理

CardView* CardView::create(CardModel* cardModel)
{
    auto view = new (std::nothrow) CardView();
    if (view && view->init(cardModel))
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool CardView::init(CardModel* cardModel)
{
    _cardModel = cardModel;
    if (!_cardModel) return false;

    // 初始化容器
    if (!Sprite::init()) return false;

    // 创建卡牌正反面精灵
    _frontSprite = CardTextureUtils::createCardFrontSprite(_cardModel);
    _backSprite = CardTextureUtils::createCardBackSprite();

    if (!_frontSprite || !_backSprite) return false;

    _frontSprite->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _backSprite->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);

    this->addChild(_frontSprite);
    this->addChild(_backSprite);
    this->setContentSize(_frontSprite->getContentSize());

    // ?????????
    this->setPosition(_cardModel->position);
    updateView(true);

    // 注册点击事件
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
    listener->setSwallowTouches(true);
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void CardView::updateView(bool force)
{
    if (!_cardModel) return;

    // 状态未变化且不强制刷新时跳过
    if (!force && _cardModel->state == _lastState) return;

    _lastState = _cardModel->state;

    switch (_cardModel->state)
    {
    case CardModel::State::COVERED:
        _frontSprite->setVisible(false);
        _backSprite->setVisible(true);
        setClickable(true);
        break;
    case CardModel::State::FLIPPED:
        _frontSprite->setVisible(true);
        _backSprite->setVisible(false);
        setClickable(true);
        break;
    case CardModel::State::ELIMINATED:
        _frontSprite->setVisible(false);
        _backSprite->setVisible(false);
        setClickable(false);
        break;
    default:
        break;
    }

    // 更新位置
    if (this->getPosition() != _cardModel->position)
    {
        this->setPosition(_cardModel->position);
    }
}

void CardView::playFlipAnimation(std::function<void()> callback)
{
    if (_isAnimating) return;
    _isAnimating = true;

    // 缩放X轴实现翻转动画
    auto scaleToBack = cocos2d::ScaleTo::create(0.2f, 0, 1);
    auto scaleToFront = cocos2d::ScaleTo::create(0.2f, 1, 1);

    auto callAfterFlip = cocos2d::CallFunc::create([=]() {
        updateView(true);  // 切换纹理
        });

    auto callAfterAnimation = cocos2d::CallFunc::create([=]() {
        _isAnimating = false;
        if (callback) callback();
        });

    this->runAction(cocos2d::Sequence::create(
        scaleToBack,
        callAfterFlip,
        scaleToFront,
        callAfterAnimation,
        nullptr
    ));
}

void CardView::playEliminateAnimation(std::function<void()> callback)
{
    if (_isAnimating) return;
    _isAnimating = true;

    auto fadeOut = cocos2d::FadeOut::create(0.3f);
    auto scaleOut = cocos2d::ScaleTo::create(0.3f, 0);
    auto spawn = cocos2d::Spawn::create(fadeOut, scaleOut, nullptr);

    auto callAfterAnimation = cocos2d::CallFunc::create([=]() {
        _isAnimating = false;
        updateView(true);  // 同步消除状态
        if (callback) callback();
        });

    this->runAction(cocos2d::Sequence::create(spawn, callAfterAnimation, nullptr));
}

void CardView::moveToPosition(const cocos2d::Vec2& targetPos, float duration, std::function<void()> callback)
{
    if (_isAnimating) return;
    _isAnimating = true;

    auto moveTo = cocos2d::MoveTo::create(duration, targetPos);
    auto callAfterAnimation = cocos2d::CallFunc::create([=]() {
        _isAnimating = false;
        if (callback) callback();
        });

    this->runAction(cocos2d::Sequence::create(moveTo, callAfterAnimation, nullptr));
}

void CardView::updateTexture()
{
    if (!_cardModel) return;

    // 刷新正面纹理
    auto newFrontSprite = CardTextureUtils::createCardFrontSprite(_cardModel);
    if (newFrontSprite)
    {
        _frontSprite->setTexture(newFrontSprite->getTexture());
        newFrontSprite->release();
    }

    // 位置同步
    this->setPosition(_cardModel->position);
}

void CardView::setClickable(bool enable)
{
    this->setOpacity(enable ? 255 : 150);
}

bool CardView::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!isVisible() || !_cardModel || _cardModel->state == CardModel::State::ELIMINATED)
    {
        return false;
    }

    auto touchPos = this->convertTouchToNodeSpace(touch);
    auto rect = cocos2d::Rect(-this->getContentSize().width / 2, -this->getContentSize().height / 2,
        this->getContentSize().width, this->getContentSize().height);

    if (rect.containsPoint(touchPos))
    {
        if (onCardClicked)
        {
            onCardClicked(_cardModel->cardId);
        }
        return true;
    }
    return false;
}


void CardView::playFailedAnimation()
{
    if (_isAnimating) return;
    _isAnimating = true;

    auto rotateRight = cocos2d::RotateBy::create(0.1f, 10);
    auto rotateLeft = cocos2d::RotateBy::create(0.1f, -20);
    auto rotateBack = cocos2d::RotateBy::create(0.1f, 10);

    auto callAfterAnimation = cocos2d::CallFunc::create([=]() {
        _isAnimating = false;
        });

    this->runAction(cocos2d::Sequence::create(
        rotateRight, rotateLeft, rotateBack, callAfterAnimation, nullptr
    ));
}