/*****************************************************************//**
 * \file   CardEnum.h
 * \brief  include CardSuitType and CardFaceType
 * 
 * \author 59641(jssun)
 * \version V1.0
 * \date   January 2026
 * \copyright Copyright (c) 2026
 *********************************************************************/

#pragma once

// 卡牌花色类型
enum class CardSuitType
{
    CST_NONE = -1,
    CST_HEART,     // 红桃
    CST_DIAMOND,   // 方块
    CST_CLUB,      // 梅花
    CST_SPADE,     // 黑桃
    CST_NUM_SUITS
};

// 卡牌牌面类型
enum class CardFaceType
{
    CFT_NONE = -1,
    CFT_ACE,       // A
    CFT_TWO,       // 2
    CFT_THREE,     // 3
    CFT_FOUR,      // 4
    CFT_FIVE,      // 5
    CFT_SIX,       // 6
    CFT_SEVEN,     // 7
    CFT_EIGHT,     // 8
    CFT_NINE,      // 9
    CFT_TEN,       // 10
    CFT_JACK,      // J
    CFT_QUEEN,     // Q
    CFT_KING,      // K
    CFT_NUM_FACES
};
