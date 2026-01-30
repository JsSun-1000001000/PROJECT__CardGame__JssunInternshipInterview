/*****************************************************************//**
 * \file   LevelConfigLoader.h
 * \brief  关卡配置加载器，从JSON文件加载关卡数据
 *
 * \author 59641
 * \date   January 2026
 *********************************************************************/
#pragma once

#include "configs/models/LevelConfig.h"

/**
 * @brief 关卡配置加载器
 * 从 Resources/levels/ 目录加载 JSON 格式的关卡配置
 */
class LevelConfigLoader
{
public:
    /**
     * @brief 加载指定关卡的配置
     * @param levelId 关卡ID，对应 level_N.json 文件
     * @return 关卡配置对象，失败返回 nullptr
     */
    static LevelConfig* loadLevel(int levelId);
};
