/*****************************************************************//**
 * @file    CardManager.h
 * @author  59641
 * @date    January 2026
 *********************************************************************/
#pragma once
#include "models/CardModel.h"
#include "views/CardView.h"
#include "cocos2d.h"


class CardView;

/**
 * @brief 卡牌管理器类，负责管理单个卡牌的模型和视图
 */
class CardManager {
public:
    /**
     * @brief 构造函数
     * @param model 卡牌模型
     */
    CardManager(const CardModel& model);

    /**
     * @brief 析构函数
     */
    ~CardManager();

    /**
     * @brief 设置卡牌模型和视图
     * @param model 卡牌模型
     * @param view 卡牌视图
     */
    void setCard(const CardModel& model, CardView* view);

    /**
     * @brief 设置触摸事件
     */
    void setupTouchEvents();

    /**
     * @brief 触摸开始事件
     * @param touch 触摸对象
     * @param event 事件对象
     * @return bool 是否处理
     */
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    /**
     * @brief 触摸移动事件
     * @param touch 触摸对象
     * @param event 事件对象
     */
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

    /**
     * @brief 触摸结束事件
     * @param touch 触摸对象
     * @param event 事件对象
     */
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    /**
     * @brief 触摸取消事件
     * @param touch 触摸对象
     * @param event 事件对象
     */
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);

    /**
     * @brief 设置卡牌点击回调
     * @param callback 回调函数
     */
    void setCardClickedCallback(const std::function<void(CardModel&)>& callback);

    /**
     * @brief 获取视图
     * @return CardView* 卡牌视图指针
     */
    CardView* getView() { 
        return _view; 
    }

    /**
     * @brief 获取模型
     * @return CardModel 卡牌模型
     */
    CardModel getModel() {
        return _model; 
    }

private:
    CardModel _model; ///< 卡牌模型
    CardView* _view; ///< 卡牌视图
    bool _isSelected; ///< 是否选中
    std::function<void(CardModel&)> _cardClickedCallback; ///< 卡牌点击回调
};
