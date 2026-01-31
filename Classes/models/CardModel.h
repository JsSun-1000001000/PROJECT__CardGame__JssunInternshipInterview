/*****************************************************************//**
 * @file    CardModel.h
 * @author  59641
 * @date    January 2026
 * @license  This file is part of the project and is released under the
 *           GNU General Public License v3.0 (GPLv3).
 *
 *           A copy of the GNU General Public License v3.0 is included in this
 *           project in the file LICENSE-GPLv3.txt, or you may obtain a copy at
 *           <https://www.gnu.org/licenses/gpl-3.0.html>.
 *
 *           This program is free software: you can redistribute it and/or modify
 *           it under the terms of the GNU General Public License as published by
 *           the Free Software Foundation, either version 3 of the License, or
 *           (at your option) any later version.
 *
 *           This program is distributed in the hope that it will be useful,
 *           but WITHOUT ANY WARRANTY; without even the implied warranty of
 *           MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *           GNU General Public License for more details.
 *********************************************************************/

#pragma once

#include "cocos2d.h"
USING_NS_CC;
enum class CardZone {
    Playfield,  // 牌桌/游戏区
    Stack,      // 牌堆/备用区
    Hand,       //手牌区
    Unknown     
};

/**
 * @brief 卡牌花色枚举
 */
enum class CardSuitType {
    CST_NONE = -1,
    CST_CLUBS,      
    CST_DIAMONDS,   
    CST_HEARTS,     
    CST_SPADES,     
    CST_NUM_CARD_SUIT_TYPES
};

/**
 * @brief 卡牌牌面（点数）枚举
 */
enum class CardFaceType {
    CFT_NONE = -1,
    CFT_ACE,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};

/**
 * @class CardModel
 * @brief 卡牌数据模型类（遵循MVC架构的Model层，仅封装数据和基础访问接口）
 * @note 核心成员变量默认值说明：
 * - _face：牌面默认值为CFT_ACE（A牌），取值范围CFT_ACE(1)~CFT_KING(13)
 * - _suit：花色默认值为CST_SPADES（黑桃），包含梅花、方块、红桃、黑桃四种
 * - _zone：区域默认值为CardZone::Unknown（未知区域），包含游戏区、牌堆、手牌区
 * - _position：位置默认值为Vec2::ZERO（坐标原点），表示卡牌在游戏场景中的像素坐标
 * - _id：卡牌唯一标识ID，用于管理器(managers)和控制器(controllers)快速定位实例
 */
class CardModel {
public:
    /**
     * @brief 构造函数（基础版，含牌面、花色、位置核心属性）
     * @param face 卡牌牌面（点数），类型为CardFaceType
     * @param suit 卡牌花色，类型为CardSuitType
     * @param position 卡牌在场景中的坐标，类型为cocos2d::Vec2
     */
    CardModel(CardFaceType face, 
        CardSuitType suit, 
        const cocos2d::Vec2& position) :_face(face), 
        _suit(suit),
        _position(position) {

        //nope~

    }

    /**
     * @brief 构造函数（完整版，含所有属性初始化）
     * @param face 卡牌牌面（点数），类型为CardFaceType
     * @param suit 卡牌花色，类型为CardSuitType
     * @param position 卡牌在场景中的坐标，类型为cocos2d::Vec2
     * @param id 卡牌唯一标识ID，整型
     * @param zone 卡牌所在区域，类型为CardZone
     */
    CardModel(CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position, int id, CardZone zone) :_face(face),
        _suit(suit),
        _position(position),
        _id(id),
        _zone(zone) {

        //nope~

    }

    /**
     * @brief 获取卡牌牌面（点数）
     * @return CardFaceType 卡牌牌面枚举值（const只读，不可修改）
     */
    CardFaceType getFace() const { 
        return _face; 
    }

    /**
     * @brief 获取卡牌花色
     * @return CardSuitType 卡牌花色枚举值（const只读，不可修改）
     */
    CardSuitType getSuit() const { 
        return _suit; 
    }

    /**
     * @brief 获取卡牌场景坐标
     * @return const cocos2d::Vec2& 坐标常量引用（避免拷贝，不可修改）
     */
    const cocos2d::Vec2& getPosition() const { 
        return _position; 
    }

    /**
     * @brief 获取卡牌所在区域
     * @return CardZone 卡牌区域枚举值（const只读，不可修改）
     */
    const CardZone getZone() const {
        return _zone;
    }

    /**
     * @brief 设置卡牌所在区域
     * @param zone 目标区域，类型为CardZone
     */
    void setZone(CardZone zone) { 
        _zone = zone; 
    }

    /**
     * @brief 设置卡牌场景坐标
     * @param position 目标坐标，类型为cocos2d::Vec2
     */
    void setPosition(Vec2 position) { 
        _position = position;
    }

    int _id; //卡牌唯一标识ID（公有属性，供管理器快速访问）

private:
    CardFaceType _face{ 
        CardFaceType::CFT_ACE 
    }; //卡牌牌面（点数），默认A牌
    CardSuitType _suit{ 
        CardSuitType::CST_SPADES 
    }; //卡牌花色，默认黑桃

    CardZone _zone; //卡牌所在区域，默认未知区域

    cocos2d::Vec2 _position{ 
        cocos2d::Vec2::ZERO 
    }; // 卡牌场景坐标，默认原点
};


