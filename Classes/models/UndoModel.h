/*****************************************************************//**
 * \file   UndoModel.h
 * \brief  
 * 
 * \author 59641
 * \date   January 2026
 *********************************************************************/
#pragma once

#include "cocos2d.h"
#include "CardModel.h"

class UndoModel : public cocos2d::Ref
{
public:
    /** 回退操作类型 */
    enum class OperationType
    {
        /** 玩家消除了主牌区的卡牌 */
        ELIMINATE_PLAYFIELD_CARD,
        /** 玩家从备用牌堆抽取了卡牌 */
        DRAW_RESERVE_CARD,
        /** 玩家翻牌替换了主牌区顶部 */
        FLIP_PLAYFIELD_CARD
    };

    // CREATE_FUNC是cocos2dx提供的内存管理宏，自动处理new/autorelease/init 放屁
    //CREATE_FUNC(UndoModel);

    // 手动实现静态创建函数
    static UndoModel* create();

    // 自定义init函数，不用override
    bool init();


    // ---------------- 操作记录数据 ----------------
    /** 操作类型 */
    OperationType opType;
    /** 操作时间戳，用于排序，可选 */
    double timestamp = 0.0;
    /** 操作前的底牌状态 */
    CardModel* preBaseCard = nullptr;
    /** 操作前的主牌区卡牌状态快照（仅变化的卡牌） */
    std::vector<CardModel*> changedPlayfieldCards;
    /** 操作前的备用牌堆状态快照（仅栈顶变化的卡牌） */
    std::vector<CardModel*> changedReserveCards;
    /** 操作前的主牌区顶部卡牌ID */
    int preTopPlayfieldCardId = -1;

};

