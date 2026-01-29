#include "MatchingService.h"
MatchingService* MatchingService::_instance = nullptr;

MatchingService* MatchingService::getInstance()
{
    if (!_instance)
    {
        _instance = new MatchingService();
    }
    return _instance;
}

void MatchingService::destroyInstance()
{
    CC_SAFE_DELETE(_instance);
}

MatchingService::MatchingService()
    : _currentBaseCard(nullptr)
{
}

bool MatchingService::isCardMatchable(CardModel* card) const
{
    if (!_currentBaseCard || !card) return false;

    // 示例实现：比较卡牌的数值是否相等，或者可以调整逻辑。
    return card->faceType == _currentBaseCard->faceType;
}

bool MatchingService::isConsecutive(CardModel* card1, CardModel* card2) const
{
    if (!card1 || !card2) return false;

    // 示例实现：比较卡牌的数值是否连续。
    return abs(static_cast<int>(card1->faceType) - static_cast<int>(card2->faceType)) == 1;
}

void MatchingService::updateCurrentBaseCard(CardModel* newBase)
{
    _currentBaseCard = newBase;
}