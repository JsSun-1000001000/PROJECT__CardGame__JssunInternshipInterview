/*****************************************************************//**
 * @file    UndoModel.h
 * @author  59641
 * @date    January 2026
 *********************************************************************/
#pragma once

#include "cocos2d.h"
#include "CardModel.h"  

USING_NS_CC;

/**
 * @brief 撤销卡牌状态结构体
 */
struct UndoCardState {
    int id; // 卡牌ID
    Vec2 position; // 操作前的位置
    CardZone zone; // 操作前的区域
};

/**
 * @brief 撤销模型类，管理撤销历史
 */
class UndoModel {
public:
    /**
     * @brief 默认构造函数
     */
    UndoModel() = default;

    /**
     * @brief 记录撤销状态
     * @param state 撤销卡牌状态
     */
    void record(const UndoCardState& state) {
        _history.push_back(state);  // 添加操作到历史
    }

    /**
     * @brief 执行撤销操作
     * @param outState 输出撤销状态
     * @return bool 是否成功
     */
    bool undo(UndoCardState& outState) {
        if (_history.empty()) {
            return false;  // 历史记录为空
        }
        // 取出最后一个操作状态
        outState = _history.back();
        // 从历史记录中移除该操作
        _history.pop_back();
        return true;
    }

    /**
     * @brief 清空历史记录
     */
    void clearHistory() {
        _history.clear();  // 清空所有记录
    }

    /**
     * @brief 检查是否有可撤销的操作
     * @return bool 是否可以撤销
     */
    bool canUndo() const { 
        return !_history.empty(); 
    }

    /**
     * @brief 获取历史记录大小
     * @return int 历史记录大小
     */
    int getSize() const { 
        return _history.size(); 
    }

private:
    std::vector<UndoCardState> _history;  // 操作历史记录，按时间顺序存储，最新操作在末尾
};



