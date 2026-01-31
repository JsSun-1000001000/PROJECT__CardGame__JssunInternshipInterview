/*****************************************************************//**
 * @file    GameView.h
 * @author  59641
 * @date    January 2026
 *********************************************************************/
#pragma once

#include "cocos2d.h"
#include "models/GameModel.h"
#include "CardView.h"
#include <vector>
#include "managers/UndoManager.h"
#include "controllers/GameController.h"

USING_NS_CC;


/**
 * @brief 游戏视图类，负责游戏界面的显示
 */
class GameView : public Node {
public:
    /**
     * @brief 静态创建 GameView 的方法
     * @param model GameModel 对象，用于初始化 GameView
     * @return GameView* 创建的游戏视图
     */
    static GameView* create(GameModel& model);

protected:
    /**
     * @brief 初始化 GameView 的逻辑
     * @param model GameModel 对象，用于初始化 GameView
     * @return 初始化成功返回 true，否则返回 false
     */
    bool init(GameModel& model);

    /**
     * @brief 根据 GameModel 生成相应的 CardView 列表
     * @param model GameModel 对象，包含卡牌信息
     */
    void generateCardViews(GameModel& model);

private:
    std::vector<CardView*> _playfieldCardViews; ///< 存储 playfield 对应的 CardView 列表
    std::vector<CardView*> _stackfieldCardViews; ///< 存储 stackfield 对应的 CardView 列表

    cocos2d::Label* _statusLabel; ///< 状态标签

    GameController* _gameController; ///< 游戏控制器

    /**
     * @brief 处理 Label 点击事件的回调函数
     */
    void onLabelClicked();

    /**
     * @brief 注册触摸事件
     */
    void registerTouchEvents();
};

