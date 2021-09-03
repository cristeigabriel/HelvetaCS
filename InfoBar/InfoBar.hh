/**
 * Not even close to done!
 */

#pragma once

#include <vector>
#include <string>
#include <functional>
#include <utility>

struct Drawing_t;
struct Color_t;
struct Content_t {
	using Result_t = std::pair<std::string, Color_t>;

	inline Content_t() = delete;
	Content_t(const std::function<bool()>& stateGetter, const std::function<Result_t(bool)>& whatIs);
	inline ~Content_t() {};

	const Result_t Get() const;

  private:
	std::function<bool()> m_fnStateGetter			= {};
	std::function<Result_t(bool)> m_fnContentGetter = {};
};

struct InfoBar_t {
	inline InfoBar_t() = default;
	inline ~InfoBar_t() {};

	inline void Push(const Content_t&);

	void Draw(Drawing_t* pDraw) const;

	inline void Toggle();

  private:
	bool m_bOpen						 = true;
	std::vector<Content_t> m_vecContents = {};
};

void InfoBar_t::Push(const Content_t& content) {
	this->m_vecContents.push_back(content);
}

void InfoBar_t::Toggle() {
	this->m_bOpen = !this->m_bOpen;
}

inline InfoBar_t g_InfoBar;