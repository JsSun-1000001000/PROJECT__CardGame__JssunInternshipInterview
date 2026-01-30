#pragma once


#include "models/GameModel.h"
#include "configs/models/LevelConfig.h"

class GameModelFromLevelGenerator
{
public:
    /**
     * @brief 将LevelConfig静态配置转换为GameModel运行时数据
     * @param levelConfig 关卡配置对象
     * @param levelId 当前关卡ID
     * @return 生成的游戏数据模型，失败返回nullptr
     */
    static GameModel* generateGameModel(const LevelConfig* levelConfig, int levelId);

private:
    // 从CardConfig创建CardModel
    static CardModel* createCardModelFromConfig(const CardConfig& config, int cardId);
};

