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
 */
struct Base_t {
	Base_t() = default;
	inline ~Base_t() {};

	virtual void Draw(ImDrawList*) const = 0;

	int m_iX = 0;
	int m_iY = 0;
	int m_iW = 0;
	int m_iH = 0;

	Color_t m_Color = {};
};

struct BaseDoubleColor_t: public Base_t {
	BaseDoubleColor_t() = default;
	inline ~BaseDoubleColor_t() {};

	Color_t m_SecondColor = {};
};

struct Rectangle_t final: public Base_t {
	Rectangle_t() = default;
	Rectangle_t(int iX, int iY, int iW, int iH, const Color_t& color, float flRounding = 0.F);
	inline ~Rectangle_t() {};

	void Draw(ImDrawList*) const override;

	float m_flRounding = 0.F;
};

struct RectangleOutline_t final: public Base_t {
	RectangleOutline_t() = default;
	RectangleOutline_t(int iX, int iY, int iW, int iH, const Color_t& color, float flRounding = 0.F, float flThickness = 0.F);
	inline ~RectangleOutline_t() {};

	void Draw(ImDrawList*) const override;

	int m_flRounding	= 0.F;
	float m_flThickness = 0.F;
};

struct RectangleGradient_t: public BaseDoubleColor_t {
	RectangleGradient_t() = default;
	RectangleGradient_t(int iX, int iY, int iW, int iH, const Color_t& color, const Color_t& secondColor, bool bHorizontal = false);
	inline ~RectangleGradient_t() {};

	void Draw(ImDrawList*) const override;

	bool m_bHorizontal = false;
};

struct Line_t final: public Base_t {
	Line_t() = default;
	Line_t(int iX, int iY, int iX2, int iY2, const Color_t& color, float flThickness = 1.F);
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
	Circle_t() = default;
	Circle_t(int iCenterX, int iCenterY, float flRadius, const Color_t& color);
	inline ~Circle_t() {};

	void Draw(ImDrawList*) const override;

	int m_iCenterX	 = 0;
	int m_iCenterY	 = 0;
	float m_flRadius = 1.F;
};

struct CircleOutline_t final: public Base_t {
	CircleOutline_t() = default;
	CircleOutline_t(int iCenterX, int iCenterY, float flRadius, const Color_t& color, float flThickness = 1.F);
	inline ~CircleOutline_t() {};

	void Draw(ImDrawList*) const override;

	int m_iCenterX		= 0;
	int m_iCenterY		= 0;
	float m_flRadius	= 1.F;
	float m_flThickness = 1.F;
};

struct Text_t final: public Base_t {
	Text_t() = default;
	Text_t(int iX, int iY, std::string strText, const ImFont* pFont, float flSize, const Color_t& color);
	inline ~Text_t() {};

	void Draw(ImDrawList*) const override;

	std::string m_strText = {};
	const ImFont* m_pFont = nullptr;
	float m_flSize		  = 0.F;
};