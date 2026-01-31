/*****************************************************************//**
 * @file    CardView.h
 * @author  59641
 * @date    January 2026
 *********************************************************************/
#pragma once

#include "cocos2d.h"
#include "models/CardModel.h"      
#include "configs/models/CardResConfig.h"   
#include "managers/CardManager.h"


class CardManager;
USING_NS_CC;


/**
 * @brief 卡牌视图类，负责卡牌的显示
 */
class CardView : public Node {
public:
    /**
     * @brief 静态创建方法，使用工厂模式
     * @param model 卡牌模型，包含颜色和点数
     * @param offset 偏移量
     * @return CardView* 创建的卡牌视图
     */
    static CardView* create(const CardModel& model, const Vec2& offset);

    /**
     * @brief 点击回调类型定义
     */
    using ClickCallback = std::function<void(CardView* cardView)>;

    /**
     * @brief 设置点击回调
     * @param callback 点击回调函数
     */
    void setClickCallback(const ClickCallback& callback);

    /**
     * @brief 加载背景资源
     */
    void loadBackground();

    /**
     * @brief 加载小数字资源
     * @param model 卡牌模型
     */
    void loadSmallNumber(const CardModel& model);

    /**
     * @brief 加载大数字资源
     * @param model 卡牌模型
     */
    void loadBigNumber(const CardModel& model);

    /**
     * @brief 加载花色图标资源
     * @param model 卡牌模型
     */
    void loadSuitIcon(const CardModel& model);

    /**
     * @brief 初始化方法
     * @param model 卡牌模型
     * @param offset 偏移量
     * @return bool 初始化是否成功
     */
    bool init(const CardModel& model, const Vec2& offset);

    /**
     * @brief 检查触摸点是否在卡牌内部
     * @param touchPos 触摸位置
     * @return bool 是否在内部
     */
    bool isTouchInside(const cocos2d::Vec2& touchPos);

    CardManager* _cardManager;     ///< 关联的 CardManager 实例

private:
    Sprite* _background = nullptr;  ///< 卡牌背景
    Sprite* _smallNumber = nullptr; ///< 左上角小数字
    Sprite* _bigNumber = nullptr;   ///< 中心大数字
    Sprite* _suitIcon = nullptr;    ///< 右上角花色图标

    const Vec2 _smallNumberPos = Vec2(-80, 130);  ///< 左上角小数字位置，相对于卡牌中心
    const Vec2 _suitIconPos = Vec2(80, 130);     ///< 右上角花色图标位置，相对于卡牌中心
    const Vec2 _bigNumberPos = Vec2(0, 0);       ///< 中心大数字位置，相对于卡牌中心

    ClickCallback _clickCallback;  ///< 点击回调函数
    bool _isSelected;              ///< 标记是否选中状态
};


