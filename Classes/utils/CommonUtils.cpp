
/*****************************************************************//**
 * \file   CommonUtils.cpp
 * \brief  
 * 
 * \author 59641
 * \date   January 2026
 *********************************************************************/

#include "CommonUtils.h"
#include "fstream"

/**
 * \brief.
 *
 * \param face
 * \return
 */
int CommonUtils::convertFaceToNumber(CardFaceType face)
{
    switch (face)
    {
    case CardFaceType::CFT_ACE: return 1;
    case CardFaceType::CFT_TWO: return 2;
    case CardFaceType::CFT_THREE: return 3;
    case CardFaceType::CFT_FOUR: return 4;
    case CardFaceType::CFT_FIVE: return 5;
    case CardFaceType::CFT_SIX: return 6;
    case CardFaceType::CFT_SEVEN: return 7;
    case CardFaceType::CFT_EIGHT: return 8;
    case CardFaceType::CFT_NINE: return 9;
    case CardFaceType::CFT_TEN: return 10;
    case CardFaceType::CFT_JACK: return 11;
    case CardFaceType::CFT_QUEEN: return 12;
    case CardFaceType::CFT_KING: return 13;
    default: return -1;
    }
}

/**
 * .
 *
 * \param path
 * \return
 */
std::string CommonUtils::readJsonConfig(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        CCLOG("Fail to open config file: %s", path.c_str());
        return "";
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}

/**
 * .
 *
 * \param designPos
 * \return
 */
cocos2d::Vec2 CommonUtils::getAdaptedPosition(const cocos2d::Vec2& designPos)
{
    auto glView = cocos2d::Director::getInstance()->getOpenGLView();
    if (!glView) return designPos;

    float scaleX = glView->getFrameSize().width / 1080.0f;
    return cocos2d::Vec2(designPos.x * scaleX, designPos.y * scaleX);
}





