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

 // 卡牌在游戏中的区域（控制器中使用的枚举）
enum class CardAreaType
{
    CAT_NONE = -1,
    CAT_PLAYFIELD,
    CAT_BASE_STACK,
    CAT_RESERVE_STACK
};


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


    CardAreaType areaType = CardAreaType::CAT_NONE;
    bool isRevealed = false;

    // 检查当前卡牌是否被指定卡覆盖（简单占位实现）
    // 实际游戏中应根据布局/坐标/层级实现更精确逻辑
    bool isCoveredBy(const CardModel* other) const
    {
        // 占位：返回 false 表示未被覆盖；可替换为真实逻辑
        // 例如：基于 position 或一些覆盖关系表判断
        (void)other;
        return false;
    }

};

