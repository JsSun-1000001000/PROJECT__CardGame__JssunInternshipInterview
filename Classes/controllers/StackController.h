#pragma once
#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "services/MatchingService.h"

class StackController : public cocos2d::Node
{
public:
    static StackController* create(GameModel* model, GameView* view);
    virtual bool init(GameModel* model, GameView* view);

    void initStacks(); // 初始化底牌堆和备用牌堆
    void handleBaseCardClick(int cardId); // 处理底牌堆点击
    void handleReserveCardClick(int cardId); // 处理备用牌堆点击

private:
    void updateBaseCardView(); // 更新底牌堆显示
    void shuffleReserveCards(); // 洗牌逻辑（可选）

    GameModel* _gameModel;
    GameView* _gameView;
    MatchingService* _matchingService;
};


