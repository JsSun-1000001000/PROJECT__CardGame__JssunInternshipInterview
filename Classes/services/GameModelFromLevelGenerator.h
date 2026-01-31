/*****************************************************************//**
 * @file    GameModelFromLevelGenerator.h
 * @author  59641
 * @date    January 2026
 *********************************************************************/
#pragma once


#include "models/GameModel.h"
#include "views/GameView.h"
#include "configs/models/LevelConfig.h"// 包含卡牌配置数据
#include "configs/loaders/LevelConfigLoader.h"
#include <vector>

USING_NS_CC;

/**
 * @brief 服务层：将LevelConfig转换为运行时GameModel
 * @note 遵循 Service 层规范：无状态、纯函数式接口、可静态调用
 */
class GameModelFromLevelGenerator {

public:
    /**
     * @brief 生成 GameModel 对象
     * @param config 关卡配置数据
     * @return 初始化后的 GameModel 对象
     */
    static GameModel generateGameModel(const std::string levelFile) {
        //auto config = LevelConfigLoader::loadLevelConfig("level_1.json");
        LevelConfig* config = LevelConfigLoader::loadLevelConfig(levelFile.empty() ? "level_1.json" : levelFile);
        if (!config) {
            CCLOG("GameModelFromLevelGenerator: failed to load level config");
            return GameModel(nullptr);
        }
        GameModel gameModel(config);

        delete config;

        return gameModel;
    }

    static void generateGameView(GameModel& gameModel, Node* parent) {
        // 创建 GameView 实例
        auto gameView = GameView::create(gameModel);
        if (gameView) {
            //parent->addChild(gameView, 1); // 将 GameView 添加到场景中
            parent->addChild(gameView, 1);
        }
    }
private:
    /**
     * @brief 私有构造函数：禁止实例化（静态类）
     */
    GameModelFromLevelGenerator() = default;


};

