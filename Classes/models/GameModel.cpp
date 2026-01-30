#include "GameModel.h"
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"


GameModel* GameModel::create()
{
    auto model = new (std::nothrow) GameModel();
    if (model && model->init())
    {
        model->autorelease();  // ???????????
        return model;
    }
    CC_SAFE_DELETE(model);
    return nullptr;
}

bool GameModel::init()
{
    baseCard = nullptr;
    return true;
}

//bool GameModel::init()
//{
//    // ???????????init()
//    if (!Ref::init())
//    {
//        return false;
//    }
//    // ????????????
//    baseCard = CardModel::create();
//    return true;
//}

std::string GameModel::serialize() const
{
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    doc.AddMember("currentLevelId", currentLevelId, allocator);

    // ???????????????
    rapidjson::Value playfieldArr(rapidjson::kArrayType);
    for (const auto& card : playfieldCards)
    {
        rapidjson::Value cardJson(rapidjson::kStringType);
        cardJson.SetString(card->serialize().c_str(), allocator);
        playfieldArr.PushBack(cardJson, allocator);
    }
    doc.AddMember("playfieldCards", playfieldArr, allocator);

    // ?????????
    if (baseCard) {
        rapidjson::Value baseCardJson(rapidjson::kStringType);
        baseCardJson.SetString(baseCard->serialize().c_str(), allocator);
        doc.AddMember("baseCard", baseCardJson, allocator);
    }

    // ????????????
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

    // ?????????????????
    const auto& playfieldArr = doc["playfieldCards"];
    for (rapidjson::SizeType i = 0; i < playfieldArr.Size(); ++i)
    {
        auto card = CardModel::create();
        card->deserialize(playfieldArr[i].GetString());
        playfieldCards.push_back(card);
    }

    // ???????????
    if (doc.HasMember("baseCard") && baseCard) {
        baseCard->deserialize(doc["baseCard"].GetString());
    }

    // ??????????????
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
    // 仅在 baseCard 为有效指针时访问，避免悬空指针（如 0xDDDDDDDD）
    if (baseCard != nullptr && baseCard->cardId == id) return baseCard;
    for (auto c : playfieldCards) if (c && c->cardId == id) return c;
    for (auto c : reserveCards) if (c && c->cardId == id) return c;
    return nullptr;
}

void GameModel::setBaseCard(CardModel* card)
{
    if (!card) return;
    if (baseCard && baseCard != card) {
        baseCard->areaType = CardAreaType::CAT_RESERVE_STACK;
        reserveCards.push_back(baseCard);
    }
    reserveCards.erase(std::remove(reserveCards.begin(), reserveCards.end(), card), reserveCards.end());
    // ?? playfield ?????????????????
    playfieldCards.erase(std::remove(playfieldCards.begin(), playfieldCards.end(), card), playfieldCards.end());
    // ???????????? areaType
    baseCard = card;
    baseCard->areaType = CardAreaType::CAT_BASE_STACK;
}

void GameModel::moveCard(CardModel* card, CardAreaType destArea)
{
    if (!card) return;

    // ????????????????
    playfieldCards.erase(std::remove(playfieldCards.begin(), playfieldCards.end(), card), playfieldCards.end());
    reserveCards.erase(std::remove(reserveCards.begin(), reserveCards.end(), card), reserveCards.end());
    if (baseCard == card) baseCard = nullptr;

    // ????????????????????? / ???? baseCard
    card->areaType = destArea;
    switch (destArea)
    {
    case CardAreaType::CAT_BASE_STACK:
        if (baseCard && baseCard != card) {
            baseCard->areaType = CardAreaType::CAT_RESERVE_STACK;
            reserveCards.push_back(baseCard);
        }
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