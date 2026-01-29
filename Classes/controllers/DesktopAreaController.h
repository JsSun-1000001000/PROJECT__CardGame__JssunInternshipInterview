#pragma once
#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "services/MatchingService.h"

class DesktopAreaController : public cocos2d::Node
{
public:
    static DesktopAreaController* create(GameModel* model, GameView* view);
    virtual bool init(GameModel* model, GameView* view);

    // 处理桌面牌点击
    void handleCardClick(int cardId);

private:
    GameModel* _gameModel;
    GameView* _gameView;
    MatchingService* _matchingService; // 服务层用于判定匹配规则
};