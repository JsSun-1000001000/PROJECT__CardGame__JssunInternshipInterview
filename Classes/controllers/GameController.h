#pragma once

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "services/MatchingService.h"
#include "controllers/PlayFieldController.h"
#include "controllers/StackController.h"

class GameController : public cocos2d::Node
{
public:
    static GameController* create(GameModel* model, GameView* view);
    virtual bool init(GameModel* model, GameView* view);

    // 游戏生命周期
    void startGame();  // 开始游戏
    void pauseGame();  // 暂停游戏
    void resumeGame(); // 恢复游戏
    void endGame();    // 结束游戏

    // 全局事件回调
    void onCardClicked(int cardId); // 响应卡牌点击事件
    void onMatchSuccess(CardModel* movedCard); // 匹配成功回调
    void onMatchFailed(CardModel* movedCard);  // 匹配失败回调

private:
    // 内部状态检查
    bool isGamePaused() const;
    bool isGameOver() const;
    void checkGameCompletion(); // 检查游戏是否完成

    // 服务层和子控制器
    MatchingService* _matchingService;
    PlayFieldController* _playFieldController;
    StackController* _stackController;

    // MVC核心组件
    GameModel* _gameModel;
    GameView* _gameView;

    // 游戏状态
    bool _isPaused;
    bool _isGameOver;
};

