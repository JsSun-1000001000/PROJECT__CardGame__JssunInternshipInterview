#pragma once
#include "cocos2d.h"
#include "models/CardModel.h"
#include "utils/CardEnum.h"

/**
 * @brief CardTextureUtils 工具类
 * @details 提供根据卡牌的模型数据生成纹理的静态方法
 */
class CardTextureUtils
{
public:
    /**
     * @brief 创建卡牌正面精灵（纹理）
     * @param cardModel 需要生成纹理的卡牌数据模型
     * @return 包含相应卡牌正面纹理的Sprite
     */
    static cocos2d::Sprite* createCardFrontSprite(CardModel* cardModel, bool isBig = false);

    /**
     * @brief 创建卡牌背面精灵（纹理）
     * @return 包含卡牌背面纹理的Sprite
     */
    static cocos2d::Sprite* createCardBackSprite();
};

