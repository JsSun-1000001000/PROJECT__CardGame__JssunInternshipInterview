/*****************************************************************//**
 * \file   CardModel.h
 * \brief  
 * 
 * \author 59641
 * \date   January 2026
 *********************************************************************/


#pragma once

#include "cocos2d.h"
#include "../Classes/utils/CardEnum.h"

class CardModel : public cocos2d::Ref
{
public:
    /** 卡牌状态枚举 */
    enum class State
    {
        /** 覆盖状态，不可点击 */
        COVERED,
        /** 翻开状态，可点击 */
        FLIPPED,
        /** 已消除状态，不可见 */
        ELIMINATED
    };

    //CREATE_FUNC(CardModel);
    // 手动实现静态创建函数
    static CardModel* create();
    //自定义init
    bool init() ;

    // 序列化：将对象转为JSON字符串
    std::string serialize() const;
    // 反序列化：从JSON字符串恢复对象
    bool deserialize(const std::string& jsonStr);

    // ---------------- 成员变量 ----------------
    /** 唯一ID，用于区分不同卡牌 */
    int cardId = -1;
    /** 牌面类型 */
    CardFaceType faceType = CardFaceType::CFT_NONE;
    /** 花色类型 */
    CardSuitType suitType = CardSuitType::CST_NONE;
    /** 当前位置（设计分辨率坐标） */
    cocos2d::Vec2 position;
    /** 当前状态 */
    State state = State::COVERED;


};