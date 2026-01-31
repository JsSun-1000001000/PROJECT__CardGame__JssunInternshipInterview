/*****************************************************************//**
 * \file   LevelConfigLoader.h
 * \brief  关卡配置加载器，从JSON文件加载关卡数据
 *
 * \author 59641
 * \date   January 2026
 *********************************************************************/
#pragma once

#include "configs/models/LevelConfig.h"
#include "cocos2d.h"
#include <memory>
#include <fstream>
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include "json/filereadstream.h"
#include "models/CardModel.h"

using namespace rapidjson;

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
    static LevelConfig* loadLevelConfig(std::string fileName);

private:
    static int currentId;

    /**
     * @brief 私有构造函数，禁止实例化（静态类设计）
     */
    LevelConfigLoader() = default;

    /**
     * @brief 禁止拷贝构造函数
     */
    LevelConfigLoader(const LevelConfigLoader&) = delete;

    /**
     * @brief 禁止赋值运算符
     */
    LevelConfigLoader& operator=(const LevelConfigLoader&) = delete;

    static bool parseCardModel(const rapidjson::Value& cardNode, std::vector<CardModel>& target, CardZone zone);
};
