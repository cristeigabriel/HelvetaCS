#pragma once

#include <d3d9.h>

#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>

#include "../Helpers/Types.hh"
#include "Geometry.hh"
#include "Queue.hh"

struct ImGuiIO;
struct ImGuiContext;
struct ImDrawList;
struct ImFont;
struct Drawing_t {
    Drawing_t() = default;
    Drawing_t(IDirect3DDevice9* pDevice, int iScreenW, int iScreenH);
    inline ~Drawing_t() {};

    void UpdateIO(int iScreenW, int iScreenH);

    void Draw(const Base_t& renderable) const;
    void Run(const std::function<void(Drawing_t*)>& runFn);

    void PushClip(int iX, int iY, int iW, int iH) const;
    void PopClip() const;

    void AddQueue(Hash_t hKey, bool bAfter);
    void AddQueueLocked(Hash_t hKey, bool bAfter);
    Queue_t& GetQueue(Hash_t hKey);
    void RemoveQueue(Hash_t hKey);

    void AddFont(Hash_t hKey, const void* pContents, uint32_t u32Size, float flSize);
    const ImFont* GetFont(Hash_t hKey) const;
    void RemoveFont(Hash_t hKey);

    int m_iScreenW = 0;
    int m_iScreenH = 0;

  private:
    IDirect3DDevice9* m_pDevice   = nullptr;
    ImGuiContext* m_pImGuiContext = nullptr;
    ImGuiIO* m_pImGuiIO           = nullptr;
    ImDrawList* m_pDrawList       = nullptr;

    std::shared_mutex m_mutLock = {};

    std::unordered_map<Hash_t, std::pair<bool, Queue_t>> m_umQueues = {};
    std::unordered_map<Hash_t, const ImFont*> m_umFonts             = {};
};

inline std::unique_ptr<Drawing_t> g_pDrawing;