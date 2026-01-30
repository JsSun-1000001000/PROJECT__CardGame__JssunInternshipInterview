/*****************************************************************//**
 * \file   LevelConfig.h
 * \brief  
 * 
 * \author 59641
 * \date   January 2026
 *********************************************************************/
#pragma once

#include "cocos2d.h"
#include "utils/CardEnum.h"

// 单张卡牌的配置结构
struct CardConfig
{
    CardFaceType faceType = CardFaceType::CFT_NONE;
    CardSuitType suitType = CardSuitType::CST_NONE;
    cocos2d::Vec2 position;
};

class LevelConfig : public cocos2d::Ref
{
public:
    CREATE_FUNC(LevelConfig);

    bool init() { return true; }

    // 主牌区卡牌配置列表
    std::vector<CardConfig> playfieldCards;
    // 底牌堆初始卡牌
    CardConfig baseCard;
    // 备用牌堆卡牌列表
    std::vector<CardConfig> reserveCards;
};
