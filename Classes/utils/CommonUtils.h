/*****************************************************************//**
 * \file   CommonUtils.h
 * \brief  basic settings
 * 
 * \author 59641(jssun)
 * \date   January 2026
 *********************************************************************/

#pragma once

#include "cocos2d.h"
#include "CardEnum.h"


class CommonUtils
{
public:

    /**
     * @brief 将牌面类型转换为数字点数
     * @param face 牌面类型
     * @return 1-13（A为1，K为13），无效返回-1
     */

    static int convertFaceToNumber(CardFaceType face);

    /**
     * @brief 从文件读取JSON配置
     * @param path 配置文件路径
     * @return JSON字符串，读取失败返回空
     */
    static std::string readJsonConfig(const std::string& path);

    /**
     * @brief 获取设计分辨率适配后的坐标
     * @param designPos 设计分辨率下的坐标（1080*2080）
     * @return 适配屏幕后的实际坐标
     */
    static cocos2d::Vec2 getAdaptedPosition(const cocos2d::Vec2& designPos);

};

