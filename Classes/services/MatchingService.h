#pragma once
#include "models/CardModel.h"

/**
 * MatchingService: 负责卡牌匹配的业务逻辑。
 * - 不持有任何数据，仅处理匹配规则等逻辑。
 * - 实现为单例模式。
 */
class MatchingService
{
public:
    /**
     * 获取单例实例。
     * @return MatchingService* 单例实例。
     */
    static MatchingService* getInstance();
    static void destroyInstance();

    /**
     * 检查卡牌是否可以匹配当前基准卡。
     * @param card 要检测的卡牌。
     * @return true 表示匹配成功，false表示匹配失败。
     */
    bool isCardMatchable(CardModel* card) const;

    /**
     * 检查两张卡牌是否连续（例如，如果规则是数值相邻）。
     * @param card1 第一张卡。
     * @param card2 第二张卡。
     * @return true 表示连续，false表示不连续。
     */
    bool isConsecutive(CardModel* card1, CardModel* card2) const;

    /**
     * 更新当前匹配基准卡。
     * @param newBase 新的当前匹配基准卡。
     */
    void updateCurrentBaseCard(CardModel* newBase);

private:
    MatchingService();  // 私有构造函数，禁止外部实例化。
    ~MatchingService() = default;

    MatchingService(const MatchingService&) = delete;
    MatchingService& operator=(const MatchingService&) = delete;

    static MatchingService* _instance;
    CardModel* _currentBaseCard; // 当前匹配的基准卡牌。
};

