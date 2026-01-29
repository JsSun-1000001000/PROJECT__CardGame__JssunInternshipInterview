#pragma once

#include "cocos2d.h"
#include "CardModel.h"

class GameModel : public cocos2d::Ref
{
public:
    // CREATE_FUNC是cocos2dx提供的内存管理宏，自动处理new/autorelease/init
    //CREATE_FUNC(GameModel);

    static GameModel* create();
    bool init();

    // 序列化整个游戏状态
    std::string serialize() const;
    // 反序列化恢复游戏状态
    bool deserialize(const std::string& jsonStr);

    // ---------------- 数据成员 ----------------
    /** 当前关卡ID */
    int currentLevelId = 1;
    /** 主牌区卡牌列表（索引对应位置） */
    std::vector<CardModel*> playfieldCards;
    /** 底牌堆（仅存当前顶部可匹配的卡牌） */
    CardModel* baseCard = nullptr;
    /** 备用牌堆（剩余未抽取的卡牌，栈结构） */
    std::vector<CardModel*> reserveCards;
    /** 主牌区当前顶部可点击的卡牌ID */
    int topPlayfieldCardId = -1;


    // ------------- 新增的访问器 / 操作接口（供 controller 使用） -------------
    const std::vector<CardModel*>& getPlayfieldCards() const { return playfieldCards; }
    const std::vector<CardModel*>& getReserveCards() const { return reserveCards; }
    CardModel* getBaseCard() const { return baseCard; }

    // 根据 id 查找卡牌（在主区、备用堆或底牌中查找）
    CardModel* getCardById(int id) const;

    // 将某张卡设为底牌（会尝试从 reserve / playfield 中移除）
    void setBaseCard(CardModel* card);

    // 将卡牌移动到目标区域（最小实现：更新容器和 areaType）
    void moveCard(CardModel* card, CardAreaType destArea);

};

