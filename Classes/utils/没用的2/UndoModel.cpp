/*****************************************************************//**
 * \file   UndoModel.cpp
 * \brief  
 * 
 * \author 59641
 * \date   January 2026
 *********************************************************************/
#include "UndoModel.h"

UndoModel* UndoModel::create()
{
    auto model = new (std::nothrow) UndoModel();
    if (model && model->init())
    {
        model->autorelease();  // 加入自动释放池
        return model;
    }
    CC_SAFE_DELETE(model);
    return nullptr;
}

bool UndoModel::init()
{
    preBaseCard = CardModel::create();
    // 初始化逻辑
    return true;
}



//bool UndoModel::init()
//{
//    // 必须调用基类的init()
//    if (!Ref::init())
//    {
//        return false;
//    }
//    preBaseCard = CardModel::create();
//    return true;
//}