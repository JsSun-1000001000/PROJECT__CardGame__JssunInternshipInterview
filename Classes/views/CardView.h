#pragma once

#include "cocos2d.h"
#include "models/CardModel.h"
class CardView :
    public cocos2d::Sprite
{
public:
    /**
     * @brief 创建卡牌视图
     * @param cardModel 绑定的数据模型
     * @return 自动释放的CardView实例，创建失败返回nullptr
     */
    static CardView* create(CardModel* cardModel);

    /**
     * @brief 初始化卡牌视图
     * @param cardModel 绑定的数据模型
     * @return 初始化成功返回true，失败返回false
     */
    bool init(CardModel* cardModel);

    /**
     * @brief 更新视图显示，同步模型状态
     * @param force 强制刷新（忽略状态是否变化）
     */
    void updateView(bool force = false);

    /**
     * @brief 播放卡牌翻转动画
     * @param callback 动画完成回调（可选）
     */
    void playFlipAnimation(std::function<void()> callback = nullptr);

    void playFailedAnimation();

    /**
     * @brief 播放卡牌消除动画
     * @param callback 动画完成回调（可选）
     */
    void playEliminateAnimation(std::function<void()> callback = nullptr);

    /**
     * @brief 移动卡牌到目标位置
     * @param targetPos 目标位置（设计分辨率坐标）
     * @param duration 动画时长（秒）
     * @param callback 动画完成回调（可选）
     */
    void moveToPosition(const cocos2d::Vec2& targetPos, float duration = 0.3f, std::function<void()> callback = nullptr);

    // ---------------- 事件回调 ----------------
    std::function<void(int cardId)> onCardClicked;  // 卡牌点击事件，参数为卡牌ID

private:
    /**
     * @brief 根据模型状态更新纹理
     */
    void updateTexture();

    /**
     * @brief 启用/禁用点击事件
     */
    void setClickable(bool enable);

    /**
     * @brief 处理卡牌点击事件
     */
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    


    // ---------------- 成员变量 ----------------
    CardModel* _cardModel = nullptr;  // 绑定的数据模型
    CardModel::State _lastState = CardModel::State::COVERED;  // 上一次显示的状态（用于优化刷新）
    cocos2d::Sprite* _frontSprite = nullptr;  // 卡牌正面
    cocos2d::Sprite* _backSprite = nullptr;   // 卡牌背面
    bool _isAnimating = false;  // 是否正在播放动画
};

