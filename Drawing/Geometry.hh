#pragma once

#include <string>

#include "Color.hh"

struct ImDrawList;
struct ImFont;
/**
 * @brief Every superset of base will have to set the following.
 * This allows for interaction with elements who rely on static dispatch.
 * 
 * Example: say you want to neatly outline every element within a range.
 * 
 * Or, say you want to modify the color of the element under your cursor.
 * 
 * 'Draw' will be statically dispatched in cases where that's possible, otherwise
 * the messaging will be done at run-time.
 * 
 * The default constructor and destructor are declared for the sake of polymorphism,
 * say we want to have concurrent rendering queues that get drawn in EndScene but
 * are dispatched in any other place.
 * 
 * For that we'd need a vector of pointers to the objects.
 * In the case of using smart pointers, the initializer and deleter will try to find a ctor/dtor
 * accordingly, but find none.
 * 
 * Constexpr constructors are inlined inside the header. They're not prone to change, so it shouldn't be an issue.
 */
struct Base_t {
	constexpr Base_t() = default;
	inline ~Base_t() {};

	virtual void Draw(ImDrawList*) const = 0;

	int m_iX = 0;
	int m_iY = 0;
	int m_iW = 0;
	int m_iH = 0;

	Color_t m_Color = {};
};

struct BaseDoubleColor_t: public Base_t {
	constexpr BaseDoubleColor_t() = default;
	inline ~BaseDoubleColor_t() {};

	Color_t m_SecondColor = {};
};

struct Rectangle_t final: public Base_t {
	constexpr Rectangle_t() = default;
	constexpr Rectangle_t(int iX, int iY, int iW, int iH, const Color_t& color, float flRounding = 0.F) {
		this->m_iX		   = iX;
		this->m_iY		   = iY;
		this->m_iW		   = iW;
		this->m_iH		   = iH;
		this->m_Color	   = color;
		this->m_flRounding = flRounding;
	}
	inline ~Rectangle_t() {};

	void Draw(ImDrawList*) const override;

	float m_flRounding = 0.F;
};

struct RectangleOutline_t final: public Base_t {
	constexpr RectangleOutline_t() = default;
	constexpr RectangleOutline_t(int iX, int iY, int iW, int iH, const Color_t& color, float flRounding = 0.F, float flThickness = 0.F) {
		this->m_iX			= iX;
		this->m_iY			= iY;
		this->m_iW			= iW;
		this->m_iH			= iH;
		this->m_Color		= color;
		this->m_flRounding	= flRounding;
		this->m_flThickness = flThickness;
	}
	inline ~RectangleOutline_t() {};

	void Draw(ImDrawList*) const override;

	int m_flRounding	= 0.F;
	float m_flThickness = 0.F;
};

struct RectangleGradient_t: public BaseDoubleColor_t {
	constexpr RectangleGradient_t() = default;
	constexpr RectangleGradient_t(int iX, int iY, int iW, int iH, const Color_t& color, const Color_t& secondColor, bool bHorizontal = false) {
		this->m_iX			= iX;
		this->m_iY			= iY;
		this->m_iW			= iW;
		this->m_iH			= iH;
		this->m_Color		= color;
		this->m_SecondColor = secondColor;
		this->m_bHorizontal = bHorizontal;
	}
	inline ~RectangleGradient_t() {};

	void Draw(ImDrawList*) const override;

	bool m_bHorizontal = false;
};

struct Line_t final: public Base_t {
	constexpr Line_t() = default;
	constexpr Line_t(int iX, int iY, int iX2, int iY2, const Color_t& color, float flThickness = 1.F) {
		this->m_iX			= iX;
		this->m_iY			= iY;
		this->m_iX2			= iX2;
		this->m_iY2			= iY2;
		this->m_Color		= color;
		this->m_flThickness = flThickness;

		this->m_iW = this->m_iX2 - this->m_iX;
		this->m_iH = this->m_iY2 - this->m_iY;
	}
	inline ~Line_t() {};

	void Draw(ImDrawList*) const override;

	int m_iX2			= 0;
	int m_iY2			= 0;
	float m_flThickness = 1.F;
};

/**
 * @brief m_iX and m_iY will be the leftmost and uppermost points of the circle.
 * The position it actually will draw at will be the center positions specified.
 * Left/upper-most points will be calculated with X or Y accordingly - radius
 * and m_iW and m_iH will be radius * 2.
 * 
 */
struct Circle_t final: public Base_t {
	constexpr Circle_t() = default;
	constexpr Circle_t(int iCenterX, int iCenterY, float flRadius, const Color_t& color) {
		this->m_iCenterX = iCenterX;
		this->m_iCenterX = iCenterY;
		this->m_flRadius = flRadius;

		this->m_iX = this->m_iCenterX - (int)flRadius;
		this->m_iY = this->m_iCenterY - (int)flRadius;
		this->m_iW = this->m_iH = ((int)flRadius) * 2;

		this->m_Color = color;
	}
	inline ~Circle_t() {};

	void Draw(ImDrawList*) const override;

	int m_iCenterX	 = 0;
	int m_iCenterY	 = 0;
	float m_flRadius = 1.F;
};

struct CircleOutline_t final: public Base_t {
	constexpr CircleOutline_t() = default;
	constexpr CircleOutline_t(int iCenterX, int iCenterY, float flRadius, const Color_t& color, float flThickness = 1.F) {
		this->m_iCenterX = iCenterX;
		this->m_iCenterX = iCenterY;
		this->m_flRadius = flRadius;

		this->m_iX = this->m_iCenterX - (int)flRadius;
		this->m_iY = this->m_iCenterY - (int)flRadius;
		this->m_iW = this->m_iH = ((int)flRadius) * 2;

		this->m_Color		= color;
		this->m_flThickness = flThickness;
	}
	inline ~CircleOutline_t() {};

	void Draw(ImDrawList*) const override;

	int m_iCenterX		= 0;
	int m_iCenterY		= 0;
	float m_flRadius	= 1.F;
	float m_flThickness = 1.F;
};

struct Text_t final: public Base_t {
	constexpr Text_t() = default;
	Text_t(int iX, int iY, std::string strText, const ImFont* pFont, float flSize, const Color_t& color);
	inline ~Text_t() {};

	void Draw(ImDrawList*) const override;

	std::string m_strText = {};
	const ImFont* m_pFont = nullptr;
	float m_flSize		  = 0.F;
};