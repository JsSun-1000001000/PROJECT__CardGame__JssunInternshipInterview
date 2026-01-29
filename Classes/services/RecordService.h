#pragma once
#include <vector>
#include <string>
#include "cocos2d.h"
#include "models/CardModel.h"

/**
 * RecordService: 负责记录游戏操作日志。
 * - 实现为单例，用于记录玩家操作历史。
 */
class RecordService
{
public:
    /**
     * 获取单例实例。
     * @return RecordService* 单例实例。
     */
    static RecordService* getInstance();

    /**
     * 销毁单例实例。
     */
    static void destroyInstance();

    /**
     * 添加玩家操作记录。
     * @param cardId 操作的卡牌ID。
     * @param areaType 卡牌所属的区域类型。
     */
    void addOperation(int cardId, CardAreaType areaType);

    /**
     * 获取所有操作记录。
     * @return std::vector<std::string> 包含日志的列表。
     */
    std::vector<std::string> getOperationRecords() const;

    /**
     * 清空操作记录。
     */
    void clearRecords();

private:
    RecordService();  // 私有构造函数，禁止外部实例化。
    ~RecordService() = default;

    RecordService(const RecordService&) = delete;
    RecordService& operator=(const RecordService&) = delete;

    static RecordService* _instance;
    std::vector<std::string> _records; // 操作记录列表。
};
