#include "CardModel.h"
//下面三个是rapidjson库
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"

CardModel* CardModel::create()
{
    auto model = new (std::nothrow) CardModel();
    if (model && model->init())
    {
        model->autorelease();  // 加入自动释放池
        return model;
    }
    CC_SAFE_DELETE(model);
    return nullptr;
}

bool CardModel::init()
{
    // 初始化逻辑
    return true;
}

std::string CardModel::serialize() const
{
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    doc.AddMember("cardId", cardId, allocator);
    doc.AddMember("faceType", static_cast<int>(faceType), allocator);
    doc.AddMember("suitType", static_cast<int>(suitType), allocator);
    doc.AddMember("positionX", position.x, allocator);
    doc.AddMember("positionY", position.y, allocator);
    doc.AddMember("state", static_cast<int>(state), allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    return buffer.GetString();
}

bool CardModel::deserialize(const std::string& jsonStr)
{
    rapidjson::Document doc;
    doc.Parse(jsonStr.c_str());
    if (doc.HasParseError()) return false;

    cardId = doc["cardId"].GetInt();
    faceType = static_cast<CardFaceType>(doc["faceType"].GetInt());
    suitType = static_cast<CardSuitType>(doc["suitType"].GetInt());
    position.x = doc["positionX"].GetFloat();
    position.y = doc["positionY"].GetFloat();
    state = static_cast<State>(doc["state"].GetInt());

    return true;
}