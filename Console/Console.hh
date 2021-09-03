#pragma once

#include <Windows.h>

#include <deque>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "../Animator/Animator.hh"
#include "../Helpers/Types.hh"

struct Color_t;
struct Drawing_t;

/**
 * @todo: Implement custom callbacks with validity state return
 * 
 */
struct Console_t;
using Callback_t = std::function<bool(Console_t*)>;
struct Console_t {
	using Config_t = std::variant<bool, int, Color_t>;

	Console_t() = delete;
	Console_t(std::string&& strConfig);
	inline ~Console_t() {};

	void Think();
	void WndProc(UINT u32Msg, WPARAM wParam);
	void Draw(Drawing_t* pDraw) const;

	void AddCallback(std::string&& strName, Callback_t&& method);
	void AddIdentifier(std::string&& strName, Config_t defaultValue);

	void WriteToBuffer(std::string&& strText);
	void ProcessBuffer();

	bool SetAutoSuggestFocusIfPossible();

	template<typename T>
	inline T& Get(Hash_t hHash) {
		return std::get<T>(this->m_umIdentifiers[hHash]);
	}

	void SaveConfig() const;
	bool LoadConfig();

	bool m_bOpen	   = false;
	bool m_bInputFocus = true;

  private:
	Animator_t m_Animation;
	std::string m_strInputBuffer						 = {};
	std::deque<std::string> m_deqConsoleBuffer			 = {};
	std::unordered_map<Hash_t, Callback_t> m_umCallbacks = {};
	std::unordered_map<Hash_t, Config_t> m_umIdentifiers = {};

  public:
	//	Last pair = {callback, hash}
	std::vector<std::pair<std::pair<std::string, bool>, std::pair<bool, Hash_t>>> m_vecCollection = {};

  private:
	std::string m_strConfig = {};

	//	Hack, but it's ok...
	mutable int m_iH = 0;

	int m_iTextScrollX = 0;

	int m_iMaxH = 0;

	int m_iScrollage = 0;

	bool m_bAbleToSwitchFocus		 = false;
	int m_nAvailableSuggestedOptions = 0;
	int m_nAutoSuggestedSelection	 = 0;
};

inline std::unique_ptr<Console_t> g_pConsole;

/**
 * @brief Utility Macros
 * 
 */
//	========================================================================================================================================
#define STATIC_GET(t, id) g_pConsole->Get<t>(HASH(id))

#define BOOL_GET(x, y) static bool& x = STATIC_GET(bool, y)

#define INT_GET(x, y) static int& x = STATIC_GET(int, y)

#define COLOR_GET(x, y) static Color_t& x = STATIC_GET(Color_t, y)
//	========================================================================================================================================
