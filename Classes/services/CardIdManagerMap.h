#pragma once
#include "managers/CardManager.h"
#include <unordered_map>


class CardIdManagerMap {
public:
    static CardIdManagerMap& getInstance() {
        static CardIdManagerMap instance;
        return instance;
    }

    void addCardManager(int cardId, CardManager* manager) {
        _cardIdManagerMap[cardId] = manager;
    }

    CardManager* getCardManager(int cardId) {

        auto it = _cardIdManagerMap.find(cardId);

        if (it != _cardIdManagerMap.end()) {
            return it->second;
        }
        return nullptr;
    }

private:
    CardIdManagerMap() {}
    ~CardIdManagerMap() {}

    std::unordered_map<int, CardManager*> _cardIdManagerMap;
};