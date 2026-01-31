/*****************************************************************//**
 * \file   UndoModel.h
 * \brief  
 * 
 * \author 59641
 * \date   January 2026
 *********************************************************************/
#pragma once

#include "cocos2d.h"
#include "CardModel.h"  

USING_NS_CC;


struct UndoCardState {
    int id;
    Vec2 position;       // 操作前的位置
    CardZone zone;
};

class UndoModel {
public:
    UndoModel() = default;


    void record(const UndoCardState& state) {
        _history.push_back(state);  // 追加操作快照到历史
    }

    bool undo(UndoCardState& outState) {
        if (_history.empty()) {
            return false;  // 无历史可回退
        }
        // 取出最后一次操作的状态
        outState = _history.back();
        // 从历史中移除（回退）
        _history.pop_back();
        return true;
    }

    void clearHistory() {
        _history.clear();  // 清空所有快照
    }

    /**
     * @brief 检查是否有可回退的操作
     */
    bool canUndo() const { 
        return !_history.empty(); 
    }

    int getSize() const { 
        return _history.size(); 
    }

private:
    std::vector<UndoCardState> _history;  // 操作历史（按时间顺序存储，回退时逆序弹出）
};

