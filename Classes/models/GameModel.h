/*****************************************************************//**
 * @file    GameModel.h
 * @author  59641
 * @date    January 2026
 *********************************************************************/
#pragma once

#include "cocos2d.h"
#include "CardModel.h"
#include "UndoModel.h"
#include <vector>
#include "configs/loaders/LevelConfigLoader.h"
#include "configs/models/LevelConfig.h"

USING_NS_CC;


/**
 * @brief 游戏模型类，管理游戏状态
 */
class GameModel {
public:
    /**
     * @brief 构造函数
     * @param config 关卡配置
     */
    GameModel(LevelConfig* config) {

        if (config) {
            // 初始化游戏卡牌
            _playfield = config->getPlayfield();
            // 初始化备用卡牌
            _stackfield = config->getStack();

            _undoModel.clearHistory();
        }
    }

    /**
     * @brief 获取主牌区卡牌
     * @return const std::vector<CardModel>& 主牌区卡牌
     */
    const std::vector<CardModel>& getPlayfield() const {
        return _playfield;
    }

    /**
     * @brief 获取备用牌堆卡牌
     * @return const std::vector<CardModel>& 备用牌堆卡牌
     */
    const std::vector<CardModel>& getStackfield() const {
        return _stackfield;
    }

    /**
     * @brief 获取撤销模型
     * @return UndoModel& 撤销模型
     */
    UndoModel& getUndoModel() {
        return _undoModel;
    }

    /**
     * @brief 添加卡牌到主牌区
     * @param card 卡牌
     */
    void addCardToPlayfield(const CardModel& card) {
        _playfield.push_back(card);
    }

    /**
     * @brief 添加卡牌到备用牌堆
     * @param card 卡牌
     */
    void addCardToStackfield(const CardModel& card) {
        _stackfield.push_back(card);
    }

    /**
     * @brief 从主牌区移除卡牌
     * @param id 卡牌ID
     */
    void removeCardFromPlayfield(int id) {
        for (auto it = _playfield.begin(); it != _playfield.end(); ++it) {
            if (it->_id == id) {
                _playfield.erase(it);
                break;
            }
        }
    }

    /**
     * @brief 从备用牌堆移除卡牌
     * @param id 卡牌ID
     */
    void removeCardFromStackfield(int id) {
        for (auto it = _stackfield.begin(); it != _stackfield.end(); ++it) {
            if (it->_id == id) {
                _stackfield.erase(it);
                break;
            }
        }
    }

private:
    std::vector<CardModel> _playfield; ///< 游戏主牌区的卡牌列表
    std::vector<CardModel> _stackfield; ///< 备用牌堆的卡牌列表
    UndoModel _undoModel; ///< 用于记录操作历史的模型
};

