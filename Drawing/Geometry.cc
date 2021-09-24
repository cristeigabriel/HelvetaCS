#include "Geometry.hh"

#include "../Vendor/ImGui/imgui.h"
#include "Drawing.hh"

/**
 * @brief Utility Macros
 * 
 */
//	========================================================================================================================================
#define IMVEC4_COLOR_RGBA(x, y, z, w) ImGui::GetColorU32(ImGui::ColorConvertFloat4ToU32(ImVec4(x / 255.F, y / 255.F, z / 255.F, w / 255.F)))
#define IMVEC4_COLOR(x)               IMVEC4_COLOR_RGBA(x.m_u8R, x.m_u8G, x.m_u8B, x.m_u8A)
#define IMVEC2(x, y)                  ImVec2((float)(x), (float)(y))
#define POSITION(x, y, w, h)          IMVEC2(x, y), IMVEC2(x + w, y + h)
//	========================================================================================================================================

void Rectangle_t::Draw(ImDrawList* pDraw) const {
    pDraw->AddRectFilled(POSITION(this->m_iX, this->m_iY, this->m_iW, this->m_iH), IMVEC4_COLOR(this->m_Color), this->m_flRounding);
}

void RectangleOutline_t::Draw(ImDrawList* pDraw) const {
    pDraw->AddRect(POSITION(this->m_iX, this->m_iY, this->m_iW, this->m_iH), IMVEC4_COLOR(this->m_Color), this->m_flRounding, 0, this->m_flThickness);
}

void RectangleGradient_t::Draw(ImDrawList* pDraw) const {
    if (this->m_bHorizontal)
        pDraw->AddRectFilledMultiColor(POSITION(this->m_iX, this->m_iY, this->m_iW, this->m_iH), IMVEC4_COLOR(this->m_Color), IMVEC4_COLOR(this->m_SecondColor), IMVEC4_COLOR(this->m_SecondColor), IMVEC4_COLOR(this->m_Color));
    else
        pDraw->AddRectFilledMultiColor(POSITION(this->m_iX, this->m_iY, this->m_iW, this->m_iH), IMVEC4_COLOR(this->m_Color), IMVEC4_COLOR(this->m_Color), IMVEC4_COLOR(this->m_SecondColor), IMVEC4_COLOR(this->m_SecondColor));
}

void Line_t::Draw(ImDrawList* pDraw) const {
    pDraw->AddLine(IMVEC2(this->m_iX, this->m_iY), IMVEC2(this->m_iX2, this->m_iY2), IMVEC4_COLOR(this->m_Color), this->m_flThickness);
}

void Circle_t::Draw(ImDrawList* pDraw) const {
    pDraw->AddCircleFilled(IMVEC2(this->m_iCenterX, this->m_iCenterY), this->m_flRadius, IMVEC4_COLOR(this->m_Color));
}

void CircleOutline_t::Draw(ImDrawList* pDraw) const {
    pDraw->AddCircle(IMVEC2(this->m_iCenterX, this->m_iCenterY), this->m_flRadius, IMVEC4_COLOR(this->m_Color), 0, this->m_flThickness);
}

Text_t::Text_t(int iX, int iY, std::string strText, const ImFont* pFont, float flSize, const Color_t& color) {
    this->m_iX      = iX;
    this->m_iY      = iY;
    this->m_strText = std::move(strText);
    this->m_pFont   = pFont;
    this->m_flSize  = flSize;

    const ImVec2& vecSize = this->m_pFont->CalcTextSizeA(this->m_flSize, FLT_MAX, 0.F, this->m_strText.data(), nullptr);
    this->m_iW            = (int)vecSize[0];
    this->m_iH            = (int)vecSize[1];

    this->m_Color = color;
}

void Text_t::Draw(ImDrawList* pDraw) const {
    pDraw->AddText(this->m_pFont, this->m_flSize, IMVEC2(this->m_iX, this->m_iY), IMVEC4_COLOR(this->m_Color), this->m_strText.c_str(), nullptr);
}