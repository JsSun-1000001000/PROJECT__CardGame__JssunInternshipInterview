/*****************************************************************//**
 * @file    UndoManager.h
 * @author  59641
 * @date    January 2026
 *********************************************************************/
#pragma once
#include "models/UndoModel.h"


/**
 * @brief 撤销管理器类，负责管理撤销操作
 */
class UndoManager {
public:
    /**
     * @brief 构造函数
     * @param undoModel 撤销模型
     */
    UndoManager(UndoModel& undoModel) {
        _undoModel = undoModel;
    }

    /**
     * @brief 记录撤销状态
     * @param state 撤销卡牌状态
     */
    void recordUndoState(const UndoCardState& state) {
        _undoModel.record(state);
    }

    /**
     * @brief 执行撤销操作
     * @param outState 输出撤销状态
     * @return bool 是否成功
     */
    bool undo(UndoCardState& outState) {
        return _undoModel.undo(outState);
    }

    /**
     * @brief 检查是否可以撤销
     * @return bool 是否可以撤销
     */
    bool canUndo() const {
        return _undoModel.canUndo();
    }

    /**
     * @brief 清空撤销历史
     */
    void clearUndoHistory() {
        _undoModel.clearHistory();
    }

    /**
     * @brief 获取撤销大小
     * @return int 撤销历史大小
     */
    int getUndoSize() {
        return _undoModel.getSize();
    }

private:
    UndoModel _undoModel; ///< 撤销模型
};