#pragma once

#include "cocos2d.h"
#include "models/GameModel.h"
#include "CardView.h"
#include <unordered_map>

/**
 * @brief 全局游戏视图容器，负责各区域布局和视图管理
 * @note 遵循MVC设计，不持有业务数据，仅作为视图容器管理者
 */
class GameView : public cocos2d::Node
{
public:
    static GameView* create();

    bool init() override;

    /**
     * @brief 初始化游戏视图布局
     */
    void initLayout();

    /**
     * @brief 创建卡牌视图并添加到对应区域
     * @param cardModel 卡牌数据模型
     * @return 创建的卡牌视图实例
     */
    CardView* createCardView(CardModel* cardModel);

    /**
     * @brief 移除卡牌视图
     * @param cardId 要移除的卡牌ID
     */
    void removeCardView(int cardId);

    /**
     * @brief 获取卡牌视图
     * @param cardId 卡牌ID
     * @return 对应ID的卡牌视图，不存在返回nullptr
     */
    CardView* getCardView(int cardId);

    /**
     * @brief 刷新所有卡牌视图状态
     */
    void refreshAllCardViews();

    // ---------------- 事件回调 ----------------
    std::function<void(int cardId)> onCardClicked;  // 卡牌点击事件转发

    //-------------------------------------------
    cocos2d::Node* getReserveLayer() const;
    void removeReserveCardView(int cardId);
    CardView* updateBaseCardView(CardModel* newBaseCard);

private:
    /**
     * @brief 根据卡牌状态添加到对应父节点
     */
    void addCardToLayer(CardView* cardView, CardModel* cardModel);

    // ---------------- 成员变量 ----------------
    cocos2d::Node* _playfieldLayer = nullptr;  // 主牌区层（上层）
    cocos2d::Node* _baseCardLayer = nullptr;   // 底牌堆层（中层）
    cocos2d::Node* _reserveLayer = nullptr;    // 备用牌堆层（下层）
    std::unordered_map<int, CardView*> _cardViewMap;  // 卡牌ID到视图的映射表
};

