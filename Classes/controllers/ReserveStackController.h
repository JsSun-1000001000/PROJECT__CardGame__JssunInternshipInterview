#pragma once
#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "services/MatchingService.h"

/**
 * ReserveStackController: 备用牌堆控制器
 * - 负责备用牌堆的交互逻辑：点击抽牌、无牌提示
 * - 处理无匹配牌时的抽牌流程
 */
class ReserveStackController : public cocos2d::Node
{
public:
    static ReserveStackController* create(GameModel* gameModel, GameView* gameView);
    virtual bool init(GameModel* gameModel, GameView* gameView);

    /**
     * 处理备用牌点击事件
     */
    void handleReserveCardClick();

    /**
     * 检查是否有可匹配的牌（桌面牌/手牌区）
     * @return true:存在可匹配牌；false:无匹配牌
     */
    bool hasMatchingCard() const;

private:
    GameModel* _gameModel;
    GameView* _gameView;
    MatchingService* _matchingService;

    /**
     * 从备用牌堆抽取新的底牌
     */
    void drawNewBaseCard();

    /**
     * 显示无牌可抽提示
     */
    void showNoCardTip() const;
};
