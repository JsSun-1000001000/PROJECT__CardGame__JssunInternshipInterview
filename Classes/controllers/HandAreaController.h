#pragma once

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"

class HandAreaController : public cocos2d::Node
{
public:
    static HandAreaController* create(GameModel* model, GameView* view);
    virtual bool init(GameModel* model, GameView* view);

    // 处理手牌点击
    void handleCardClick(int cardId);

private:
    GameModel* _gameModel;
    GameView* _gameView;
};

