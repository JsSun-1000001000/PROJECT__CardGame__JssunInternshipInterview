#pragma once
#include "models/CardModel.h"
#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "services/MatchingService.h"


class PlayFieldController : public cocos2d::Node
{
public:
    static PlayFieldController* create(GameModel* model, GameView* view);
    virtual bool init(GameModel* model, GameView* view);

    void initPlayField(); // 初始化主牌区
    void handleCardClick(int cardId); // 处理主牌区卡牌点击
    int getPlayFieldCardCount() const; // 获取主牌区剩余卡牌数量

private:
    bool isCardPlayable(int cardId); // 检查卡牌是否可操作
    void revealUnderlyingCards(CardModel* removedCard); // 翻开被覆盖的下层卡牌

    GameModel* _gameModel;
    GameView* _gameView;
    MatchingService* _matchingService;
};

