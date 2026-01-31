#pragma once
#include <vector>
#include <string>
#include "cocos2d.h"
#include "models/CardModel.h"

struct UndoEntry {
    int movedCardId;
    int oldBaseId;
    bool fromPlayfield;
    cocos2d::Vec2 oldPosition;
};

class RecordService
{
public:
    static RecordService* getInstance();
    static void destroyInstance();

    void addOperation(int cardId, CardAreaType areaType);
    void addUndoRecord(int movedCardId, int oldBaseId, bool fromPlayfield, const cocos2d::Vec2& oldPos);
    bool canUndo() const { return !_undoStack.empty(); }
    UndoEntry popUndoRecord();

    std::vector<std::string> getOperationRecords() const;
    void clearRecords();

private:
    RecordService();  // ??????????????????????
    ~RecordService() = default;

    RecordService(const RecordService&) = delete;
    RecordService& operator=(const RecordService&) = delete;

    static RecordService* _instance;
    std::vector<std::string> _records;
    std::vector<UndoEntry> _undoStack; // ??????¼?????
};
