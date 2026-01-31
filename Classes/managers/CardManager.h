#pragma once
#include "models/CardModel.h"
#include "views/CardView.h"
#include "cocos2d.h"


class CardView;

class CardManager {
public:
    CardManager(const CardModel& model);

    ~CardManager();

    void setCard(const CardModel& model, CardView* view);

    void setupTouchEvents();

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);


    // 设置点击回调函数
    void setCardClickedCallback(const std::function<void(CardModel&)>& callback);

    CardView* getView() { 
        return _view; 
    }

    CardModel getModel() {
        return _model; 
    }


private:
    CardModel _model;

    CardView* _view;

    bool _isSelected;

    std::function<void(CardModel&)> _cardClickedCallback;
};
