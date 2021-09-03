#include "InfoBar.hh"

#include "../Drawing/Drawing.hh"
#include "../Drawing/Color.hh"

#include "../Helpers/Helpers.hh"

/**
 * @brief Styling is not an issue yet, thus, I'll make stylization details global (at least, for now).
 * 
 */
//	========================================================================================================================================
constexpr static int g_iBlockSize	= 18;
constexpr static float g_flFontSize = 15.F;

static_assert(g_flFontSize <= g_iBlockSize);

constexpr static int g_iContentDelimiterSpace = 4;
constexpr static int g_iContentDelimiterWidth = 2;

static_assert(g_iContentDelimiterWidth < g_iContentDelimiterSpace);

constexpr static Color_t g_backgroundColor		 = Color_t(36, 37, 42, 102);
constexpr static Color_t g_contentDelimiterColor = Color_t(255, 255, 255, 255);
//	========================================================================================================================================

Content_t::Content_t(const std::function<bool()>& stateGetter, const std::function<Result_t(bool)>& whatIs) {
	this->m_fnStateGetter	= stateGetter;
	this->m_fnContentGetter = whatIs;
}

const Content_t::Result_t Content_t::Get() const {
	return this->m_fnContentGetter(this->m_fnStateGetter());
}

void InfoBar_t::Draw(Drawing_t* pDraw) const {
	static const ImFont* pFont = pDraw->GetFont(HASH("Terminus"));

	if (!this->m_bOpen)
		return;

	int iY = pDraw->m_iScreenH - g_iBlockSize;
	pDraw->Draw(Rectangle_t(0, iY, pDraw->m_iScreenW, pDraw->m_iScreenH, g_backgroundColor));

	//	Eventually, move data and thinking to their own function and do some caching.
	bool bEmpty			  = this->m_vecContents.empty();
	size_t iLastDelimiter = this->m_vecContents.size();
	int iCursor			  = 0;

	for (size_t i = 0; i < this->m_vecContents.size(); ++i) {
		const Content_t::Result_t& entry = this->m_vecContents[i].Get();
		Text_t&& text					 = Text_t(iCursor, iY + g_iBlockSize / 2, entry.first, pFont, g_flFontSize, entry.second);
		text.m_iY -= text.m_iH / 2;

		if (i > 0 && i < iLastDelimiter)
			pDraw->Draw(Rectangle_t(iCursor - (int)(g_iContentDelimiterSpace / 2) - (int)(g_iContentDelimiterWidth / 2), iY + 2, g_iContentDelimiterWidth, g_iBlockSize - 4, g_contentDelimiterColor));

		iCursor += text.m_iW + g_iContentDelimiterSpace;
		pDraw->Draw(text);
	}
}