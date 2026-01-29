#include "GameModel.h"
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"


GameModel* GameModel::create()
{
    auto model = new (std::nothrow) GameModel();
    if (model && model->init())
    {
        model->autorelease();  // 加入自动释放池
        return model;
    }
    CC_SAFE_DELETE(model);
    return nullptr;
}

bool GameModel::init()
{
    // 初始化默认空卡牌
    baseCard = CardModel::create();
    //初始化逻辑
    return true;
}

//bool GameModel::init()
//{
//    // 必须调用基类的init()
//    if (!Ref::init())
//    {
//        return false;
//    }
//    // 初始化默认空卡牌
//    baseCard = CardModel::create();
//    return true;
//}

std::string GameModel::serialize() const
{
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    doc.AddMember("currentLevelId", currentLevelId, allocator);

    // 序列化主牌区卡牌
    rapidjson::Value playfieldArr(rapidjson::kArrayType);
    for (const auto& card : playfieldCards)
    {
        rapidjson::Value cardJson(rapidjson::kStringType);
        cardJson.SetString(card->serialize().c_str(), allocator);
        playfieldArr.PushBack(cardJson, allocator);
    }
    doc.AddMember("playfieldCards", playfieldArr, allocator);

    // 序列化底牌
    rapidjson::Value baseCardJson(rapidjson::kStringType);
    baseCardJson.SetString(baseCard->serialize().c_str(), allocator);
    doc.AddMember("baseCard", baseCardJson, allocator);

    // 序列化备用牌堆
    rapidjson::Value reserveArr(rapidjson::kArrayType);
    for (const auto& card : reserveCards)
    {
        rapidjson::Value cardJson(rapidjson::kStringType);
        cardJson.SetString(card->serialize().c_str(), allocator);
        reserveArr.PushBack(cardJson, allocator);
    }
    doc.AddMember("reserveCards", reserveArr, allocator);

    doc.AddMember("topPlayfieldCardId", topPlayfieldCardId, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    return buffer.GetString();
}

bool GameModel::deserialize(const std::string& jsonStr)
{
    rapidjson::Document doc;
    doc.Parse(jsonStr.c_str());
    if (doc.HasParseError()) return false;

    currentLevelId = doc["currentLevelId"].GetInt();
    topPlayfieldCardId = doc["topPlayfieldCardId"].GetInt();

    // 反序列化主牌区卡牌
    const auto& playfieldArr = doc["playfieldCards"];
    for (rapidjson::SizeType i = 0; i < playfieldArr.Size(); ++i)
    {
        auto card = CardModel::create();
        card->deserialize(playfieldArr[i].GetString());
        playfieldCards.push_back(card);
    }

    // 反序列化底牌
    baseCard->deserialize(doc["baseCard"].GetString());

    // 反序列化备用牌堆
    const auto& reserveArr = doc["reserveCards"];
    for (rapidjson::SizeType i = 0; i < reserveArr.Size(); ++i)
    {
        auto card = CardModel::create();
        card->deserialize(reserveArr[i].GetString());
        reserveCards.push_back(card);
    }

    return true;
}


CardModel* GameModel::getCardById(int id) const
{
    if (baseCard && baseCard->cardId == id) return baseCard;
    for (auto c : playfieldCards) if (c && c->cardId == id) return c;
    for (auto c : reserveCards) if (c && c->cardId == id) return c;
    return nullptr;
}

void GameModel::setBaseCard(CardModel* card)
{
    if (!card) return;
    // 从 reserveCards 中移除（如果存在）
    reserveCards.erase(std::remove(reserveCards.begin(), reserveCards.end(), card), reserveCards.end());
    // 从 playfield 中也尝试移除（保险）
    playfieldCards.erase(std::remove(playfieldCards.begin(), playfieldCards.end(), card), playfieldCards.end());
    // 设置底牌并更新 areaType
    baseCard = card;
    baseCard->areaType = CardAreaType::CAT_BASE_STACK;
}

void GameModel::moveCard(CardModel* card, CardAreaType destArea)
{
    if (!card) return;

    // 先从所有容器中移除
    playfieldCards.erase(std::remove(playfieldCards.begin(), playfieldCards.end(), card), playfieldCards.end());
    reserveCards.erase(std::remove(reserveCards.begin(), reserveCards.end(), card), reserveCards.end());
    if (baseCard == card) baseCard = nullptr;

    // 再根据目标区域放入对应容器 / 更新 baseCard
    card->areaType = destArea;
    switch (destArea)
    {
    case CardAreaType::CAT_BASE_STACK:
        baseCard = card;
        break;
    case CardAreaType::CAT_PLAYFIELD:
        playfieldCards.push_back(card);
        break;
    case CardAreaType::CAT_RESERVE_STACK:
        reserveCards.push_back(card);
        break;
    default:
        break;
    }
}