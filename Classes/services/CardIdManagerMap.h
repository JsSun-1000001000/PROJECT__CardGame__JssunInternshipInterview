/*****************************************************************//**
 * @file    CardIdManagerMap.h
 * @author  59641
 * @date    January 2026
 *********************************************************************/
#pragma once
#include "managers/CardManager.h"
#include <unordered_map>


/**
 * @brief 卡牌ID管理器映射类，单例模式
 */
class CardIdManagerMap {
public:
    /**
     * @brief 获取单例实例
     * @return CardIdManagerMap& 实例引用
     */
    static CardIdManagerMap& getInstance() {
        static CardIdManagerMap instance;
        return instance;
    }

    /**
     * @brief 添加卡牌管理器
     * @param cardId 卡牌ID
     * @param manager 卡牌管理器指针
     */
    void addCardManager(int cardId, CardManager* manager) {
        _cardIdManagerMap[cardId] = manager;
    }

    /**
     * @brief 获取卡牌管理器
     * @param cardId 卡牌ID
     * @return CardManager* 卡牌管理器指针，如果不存在返回nullptr
     */
    CardManager* getCardManager(int cardId) {

        auto it = _cardIdManagerMap.find(cardId);

        if (it != _cardIdManagerMap.end()) {
            return it->second;
        }
        return nullptr;
    }

private:
    /**
     * @brief 私有构造函数
     */
    CardIdManagerMap() {}

    /**
     * @brief 私有析构函数
     */
    ~CardIdManagerMap() {}

    std::unordered_map<int, CardManager*> _cardIdManagerMap; ///< 卡牌ID到管理器的映射
};