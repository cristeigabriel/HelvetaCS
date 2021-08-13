#pragma once

#include "Geometry.hh"
#include "Queue.hh"
#include "../Helpers/Helpers.hh"
#include <memory>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <d3d9.h>

struct ImDrawList;
struct ImFont;
struct Drawing_t
{
	Drawing_t() = default;
	Drawing_t(IDirect3DDevice9 *pDevice, int iScreenW, int iScreenH);
	inline ~Drawing_t(){};

	void UpdateIO(int iScreenW, int iScreenH);

	void Draw(Base_t &&renderable) const;
	void Run(const std::function<void(Drawing_t *)> &runFn);

	void PushClip(int iX, int iY, int iW, int iH) const;
	void PopClip() const;

	void AddQueue(Hash_t hKey, bool bAfter);
	void AddQueueLocked(Hash_t hKey, bool bAfter);
	Queue_t &GetQueue(Hash_t hKey);
	void RemoveQueue(Hash_t hKey);

	void AddFont(Hash_t hKey, const void *pContents, uint32_t u32Size, float flSize);
	const ImFont *GetFont(Hash_t hKey) const;
	void RemoveFont(Hash_t hKey);

	int m_iScreenW = 0;
	int m_iScreenH = 0;

private:
	IDirect3DDevice9 *m_pDevice = nullptr;
	ImDrawList *m_pDrawList = nullptr;

	std::shared_mutex m_mutLock = {};

	std::unordered_map<Hash_t, std::pair<bool, Queue_t>> m_umQueues = {};
	std::unordered_map<Hash_t, const ImFont *> m_umFonts = {};
};

inline std::unique_ptr<Drawing_t> g_pDrawing;