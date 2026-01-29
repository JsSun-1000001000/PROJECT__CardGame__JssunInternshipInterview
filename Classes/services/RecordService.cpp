#include "RecordService.h"
#include <sstream>

RecordService* RecordService::_instance = nullptr;

RecordService* RecordService::getInstance()
{
    if (!_instance)
    {
        _instance = new RecordService();
    }
    return _instance;
}

void RecordService::destroyInstance()
{
    CC_SAFE_DELETE(_instance);
}

RecordService::RecordService() {}

void RecordService::addOperation(int cardId, CardAreaType areaType)
{
    // 添加操作记录到日志
    std::ostringstream ss;
    ss << "CardID: " << cardId << " | AreaType: " << static_cast<int>(areaType);
    _records.push_back(ss.str());
}

std::vector<std::string> RecordService::getOperationRecords() const
{
    return _records;
}

void RecordService::clearRecords()
{
    _records.clear();
}