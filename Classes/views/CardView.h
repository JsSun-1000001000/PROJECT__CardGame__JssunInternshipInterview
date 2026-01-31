#pragma once

#include "cocos2d.h"
#include "models/CardModel.h"      
#include "configs/models/CardResConfig.h"   
#include "managers/CardManager.h"


class CardManager;
USING_NS_CC;


class CardView : public Node {
public:
    /**
     * @brief 静态创建方法（工厂模式）
     * @param model 卡牌数据模型（包含花色、点数）
     */
    static CardView* create(const CardModel& model, const Vec2& offset);
    // 卡牌点击回调类型
    using ClickCallback = std::function<void(CardView* cardView)>;
    // 设置点击回调
    void setClickCallback(const ClickCallback& callback);

    // 资源加载
    void loadBackground();
    void loadSmallNumber(const CardModel& model);
    void loadBigNumber(const CardModel& model);
    void loadSuitIcon(const CardModel& model);

    bool init(const CardModel& model, const Vec2& offset);  // 初始化

    // 检查触摸点是否在卡牌区域内
    bool isTouchInside(const cocos2d::Vec2& touchPos);

    CardManager* _cardManager;     // 添加 CardManager 实例

private:
    // 子节点成员 
    Sprite* _background = nullptr;  // 卡牌背景
    Sprite* _smallNumber = nullptr; // 左上角小数字
    Sprite* _bigNumber = nullptr;   // 中间大数字
    Sprite* _suitIcon = nullptr;    // 右上角花色图标

    // 布局参数（可根据美术资源调整）
    const Vec2 _smallNumberPos = Vec2(-80, 130);  // 左上角小数字位置（相对背景中心）
    const Vec2 _suitIconPos = Vec2(80, 130);     // 右上角花色图标位置（相对背景中心）
    const Vec2 _bigNumberPos = Vec2(0, 0);       // 中间大数字位置（相对背景中心）

    ClickCallback _clickCallback;  // 点击回调函数
    bool _isSelected;              // 卡牌是否被选中状态
};


