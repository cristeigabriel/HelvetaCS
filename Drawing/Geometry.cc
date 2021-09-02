#include "Geometry.hh"

#include "../Vendor/ImGui/imgui.h"
#include "Drawing.hh"

/**
 * @brief Utility Macros
 * 
 */
//	========================================================================================================================================
#define IMVEC4_COLOR_RGBA(x, y, z, w) ImGui::GetColorU32(ImGui::ColorConvertFloat4ToU32(ImVec4(x / 255.F, y / 255.F, z / 255.F, w / 255.F)))
#define IMVEC4_COLOR(x)				  IMVEC4_COLOR_RGBA(x.m_u8R, x.m_u8G, x.m_u8B, x.m_u8A)
#define IMVEC2(x, y)				  ImVec2((float)(x), (float)(y))
#define POSITION(x, y, w, h)		  IMVEC2(x, y), IMVEC2(x + w, y + h)
//	========================================================================================================================================

Rectangle_t::Rectangle_t(int iX, int iY, int iW, int iH, const Color_t& color, float flRounding) {
	this->m_iX		   = iX;
	this->m_iY		   = iY;
	this->m_iW		   = iW;
	this->m_iH		   = iH;
	this->m_Color	   = color;
	this->m_flRounding = flRounding;
}

void Rectangle_t::Draw(ImDrawList* pDraw) const {
	pDraw->AddRectFilled(POSITION(this->m_iX, this->m_iY, this->m_iW, this->m_iH), IMVEC4_COLOR(this->m_Color), this->m_flRounding);
}

RectangleOutline_t::RectangleOutline_t(int iX, int iY, int iW, int iH, const Color_t& color, float flRounding, float flThickness) {
	this->m_iX			= iX;
	this->m_iY			= iY;
	this->m_iW			= iW;
	this->m_iH			= iH;
	this->m_Color		= color;
	this->m_flRounding	= flRounding;
	this->m_flThickness = flThickness;
}

void RectangleOutline_t::Draw(ImDrawList* pDraw) const {
	pDraw->AddRect(POSITION(this->m_iX, this->m_iY, this->m_iW, this->m_iH), IMVEC4_COLOR(this->m_Color), this->m_flRounding, 0, this->m_flThickness);
}

RectangleGradient_t::RectangleGradient_t(int iX, int iY, int iW, int iH, const Color_t& color, const Color_t& secondColor, bool bHorizontal) {
	this->m_iX			= iX;
	this->m_iY			= iY;
	this->m_iW			= iW;
	this->m_iH			= iH;
	this->m_Color		= color;
	this->m_SecondColor = secondColor;
	this->m_bHorizontal = bHorizontal;
}

void RectangleGradient_t::Draw(ImDrawList* pDraw) const {
	if (this->m_bHorizontal)
		pDraw->AddRectFilledMultiColor(POSITION(this->m_iX, this->m_iY, this->m_iW, this->m_iH), IMVEC4_COLOR(this->m_Color), IMVEC4_COLOR(this->m_SecondColor), IMVEC4_COLOR(this->m_SecondColor), IMVEC4_COLOR(this->m_Color));
	else
		pDraw->AddRectFilledMultiColor(POSITION(this->m_iX, this->m_iY, this->m_iW, this->m_iH), IMVEC4_COLOR(this->m_Color), IMVEC4_COLOR(this->m_Color), IMVEC4_COLOR(this->m_SecondColor), IMVEC4_COLOR(this->m_SecondColor));
}

Line_t::Line_t(int iX, int iY, int iX2, int iY2, const Color_t& color, float flThickness) {
	this->m_iX			= iX;
	this->m_iY			= iY;
	this->m_iX2			= iX2;
	this->m_iY2			= iY2;
	this->m_Color		= color;
	this->m_flThickness = flThickness;

	this->m_iW = this->m_iX2 - this->m_iX;
	this->m_iH = this->m_iY2 - this->m_iY;
}

void Line_t::Draw(ImDrawList* pDraw) const {
	pDraw->AddLine(IMVEC2(this->m_iX, this->m_iY), IMVEC2(this->m_iX2, this->m_iY2), IMVEC4_COLOR(this->m_Color), this->m_flThickness);
}

Circle_t::Circle_t(int iCenterX, int iCenterY, float flRadius, const Color_t& color) {
	this->m_iCenterX = iCenterX;
	this->m_iCenterX = iCenterY;
	this->m_flRadius = flRadius;

	this->m_iX = this->m_iCenterX - (int)flRadius;
	this->m_iY = this->m_iCenterY - (int)flRadius;
	this->m_iW = this->m_iH = ((int)flRadius) * 2;

	this->m_Color = color;
}

void Circle_t::Draw(ImDrawList* pDraw) const {
	pDraw->AddCircleFilled(IMVEC2(this->m_iCenterX, this->m_iCenterY), this->m_flRadius, IMVEC4_COLOR(this->m_Color));
}

CircleOutline_t::CircleOutline_t(int iCenterX, int iCenterY, float flRadius, const Color_t& color, float flThickness) {
	this->m_iCenterX = iCenterX;
	this->m_iCenterX = iCenterY;
	this->m_flRadius = flRadius;

	this->m_iX = this->m_iCenterX - (int)flRadius;
	this->m_iY = this->m_iCenterY - (int)flRadius;
	this->m_iW = this->m_iH = ((int)flRadius) * 2;

	this->m_Color		= color;
	this->m_flThickness = flThickness;
}

void CircleOutline_t::Draw(ImDrawList* pDraw) const {
	pDraw->AddCircle(IMVEC2(this->m_iCenterX, this->m_iCenterY), this->m_flRadius, IMVEC4_COLOR(this->m_Color), 0, this->m_flThickness);
}

Text_t::Text_t(int iX, int iY, std::string strText, const ImFont* pFont, float flSize, const Color_t& color) {
	this->m_iX		= iX;
	this->m_iY		= iY;
	this->m_strText = std::move(strText);
	this->m_pFont	= pFont;
	this->m_flSize	= flSize;

	const ImVec2& vecSize = this->m_pFont->CalcTextSizeA(this->m_flSize, FLT_MAX, 0.F, this->m_strText.data(), nullptr);
	this->m_iW			  = (int)vecSize[0];
	this->m_iH			  = (int)vecSize[1];

	this->m_Color = color;
}

void Text_t::Draw(ImDrawList* pDraw) const {
	pDraw->AddText(this->m_pFont, this->m_flSize, IMVEC2(this->m_iX, this->m_iY), IMVEC4_COLOR(this->m_Color), this->m_strText.c_str(), nullptr);
}