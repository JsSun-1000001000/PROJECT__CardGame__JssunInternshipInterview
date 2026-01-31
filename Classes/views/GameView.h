#pragma once

#include "cocos2d.h"
#include "models/GameModel.h"
#include "CardView.h"
#include <vector>
#include "managers/UndoManager.h"
#include "controllers/GameController.h"

USING_NS_CC;


class GameView : public Node {
public:
    /**
     * @brief 静态创建 GameView 的方法
     * @param model GameModel 对象，用于初始化 GameView
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
     * @brief 根据 GameModel 生成对应的 CardView 数组
     * @param model GameModel 对象，包含卡片信息
     */
    void generateCardViews(GameModel& model);

private:
    std::vector<CardView*> _playfieldCardViews; // 存储 playfield 对应的 CardView 数组
    std::vector<CardView*> _stackfieldCardViews; // 存储 stackfield 对应的 CardView 数组

    // Label 成员变量
    cocos2d::Label* _statusLabel;


    // GameController 成员变量
    GameController* _gameController;
    // 处理 Label 点击的回调函数
    void onLabelClicked();

    // 注册触摸事件
    void registerTouchEvents();
};

