/*****************************************************************//**
 * @file    GameController.h
 * @author  59641
 * @date    January 2026
 *********************************************************************/
#pragma once

#include "models/GameModel.h"
#include "managers/UndoManager.h"
#include "managers/CardManager.h"
#include <vector>
#include "cocos2d.h"

class CardManager;
/**
 * @brief 游戏控制器类（遵循MVC架构的controllers层规范）
 * 负责游戏逻辑的整体控制，协调GameModel和GameView/CardView的交互
 * 1. 处理卡牌移动和匹配逻辑，如selectCardFromPlayefieldAndMatch方法，验证卡牌是否可以移动
 * 2. 处理Stack卡牌的点击，通过UndoManager记录操作状态
 * 3. 实现撤销功能，undo操作根据UndoModel记录的状态，恢复到指定位置的状态
 * 4. 协调视图更新，通过CardView的接口执行卡牌移动动画
 */
class GameController {
public:
    /**
     * @brief 构造函数
     * @param gameModel 游戏模型
     * @param playfieldOffset 主牌区偏移，默认Vec2::ZERO
     * @param stackOffset 备用牌堆偏移，默认Vec2::ZERO
     */
    GameController(GameModel gameModel,
        const cocos2d::Vec2& playfieldOffset = cocos2d::Vec2::ZERO,
        const cocos2d::Vec2& stackOffset = cocos2d::Vec2::ZERO);

    /**
     * @brief 析构函数
     */
    ~GameController();

    /**
     * @brief 选择主牌区卡牌并匹配，处理卡牌移动和匹配逻辑
     * @param selectedCard 选中的卡牌
     * @return bool 是否成功
     */
    bool selectCardFromPlayefieldAndMatch(CardModel& selectedCard);

    /**
     * @brief 点击备用牌堆卡牌，记录状态到UndoModel
     * @param card 点击的卡牌
     */
    void clickStackCard(CardModel& card);

    /**
     * @brief 处理卡牌点击事件
     * @param card 点击的卡牌
     */
    void handleCardClicked(CardModel& card);

    /**
     * @brief 撤销操作
     * @return bool 是否成功撤销
     */
    bool undo();

    /**
     * @brief 处理标签点击
     */
    void handleLabelClick();

private:
    GameModel _gameModel;
    UndoManager _undoManager;

    /**
     * @brief 获取底部的卡牌
     * @return CardModel 底部的卡牌
     */
    CardModel getBottomCard();

    /**
     * @brief 检查两张卡牌是否匹配
     * @param card1 第一张卡牌
     * @param card2 第二张卡牌
     * @return bool 是否匹配
     */
    bool isCardMatch(const CardModel& card1, const CardModel& card2);

    /**
     * @brief 将卡牌移动到原始位置
     * @param state 撤销卡牌状态
     */
    void moveCardToOriginalPosition(const UndoCardState& state);

    /**
     * @brief 根据CardModel获取CardManager
     * @param card 卡牌模型
     * @return CardManager* 卡牌管理器指针
     */
    CardManager* getCardManager(const CardModel& card);

    cocos2d::Vec2 _playfieldOffset;
    cocos2d::Vec2 _stackOffset;
};

