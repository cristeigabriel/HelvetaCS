#include "Drawing.hh"

#include "../Helpers/Helpers.hh"

#include "../Vendor/ImGui/imgui.h"
#include "../Vendor/ImGui/imgui_impl_dx9.h"
#undef NDEBUG
#include <assert.h>

Drawing_t::Drawing_t(IDirect3DDevice9* pDevice, int iScreenW, int iScreenH) {
    this->m_pDevice = pDevice;
    assert(this->m_pDevice);

    this->m_pImGuiContext = ImGui::CreateContext();
    assert(this->m_pImGuiContext);
    this->m_pImGuiIO = &ImGui::GetIO();
    assert(this->m_pImGuiIO);
    assert(ImGui_ImplDX9_Init(this->m_pDevice));

    UpdateIO(iScreenW, iScreenH);
}

void Drawing_t::UpdateIO(int iScreenW, int iScreenH) {
    this->m_iScreenW = iScreenW;
    this->m_iScreenH = iScreenH;

    this->m_pImGuiIO->DisplaySize.x = (float)this->m_iScreenW;
    this->m_pImGuiIO->DisplaySize.y = (float)this->m_iScreenH;
}

/**
 * @note: This procses leads to static dispatch.
 * 
 */
void Drawing_t::Draw(const Base_t& renderable) const {
    renderable.Draw(this->m_pDrawList);
}

void Drawing_t::Run(const std::function<void(Drawing_t*)>& runFn) {
    ImGui_ImplDX9_NewFrame();
    ImGui::NewFrame();

    this->m_pDrawList = ImGui::GetForegroundDrawList();
    if (!this->m_pDrawList)
        return;

    this->m_pDrawList->_ClipRectStack.clear();

    //	Hack ahead:
    //	To fix a lighting bug we're creating our own state block to have control over color
    //	write or shaders. Please don't crucify me.
    IDirect3DStateBlock9* pStateBlock = nullptr;
    if (this->m_pDevice->CreateStateBlock(D3DSBT_ALL, &pStateBlock) != D3D_OK)
        return;

    if (pStateBlock->Capture() != D3D_OK)
        return;

    //	Store old data
    DWORD dwColorWrite = 0;
    DWORD dwSRGBWrite  = 0;
    this->m_pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &dwColorWrite);
    this->m_pDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &dwSRGBWrite);

    this->m_pDevice->SetVertexShader(nullptr);
    this->m_pDevice->SetPixelShader(nullptr);

    this->m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
    this->m_pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

    std::unique_lock<std::shared_mutex> lock(this->m_mutLock);

    for (const auto& [key, value] : this->m_umQueues) {
        if (!value.first)
            for (const auto& entry : value.second.m_vecRenderables)
                entry->Draw(this->m_pDrawList);
    }

    runFn(this);

    for (const auto& [key, value] : this->m_umQueues) {
        if (value.first)
            for (const auto& entry : value.second.m_vecRenderables)
                entry->Draw(this->m_pDrawList);
    }

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    //	Apply our state block
    pStateBlock->Apply();
    pStateBlock->Release();

    //	Apply our old data
    this->m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, dwColorWrite);
    this->m_pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, dwSRGBWrite);
}

void Drawing_t::PushClip(int iX, int iY, int iW, int iH) const {
    this->m_pDrawList->PushClipRect(ImVec2((float)(iX), (float)(iY)), ImVec2((float)(iX + iW), (float)(iY + iH)), true);
}

void Drawing_t::PopClip() const {
    this->m_pDrawList->PopClipRect();
}

void Drawing_t::AddQueue(Hash_t hKey, bool bAfter) {
    this->m_umQueues[hKey] = std::make_pair(bAfter, Queue_t());
}

void Drawing_t::AddQueueLocked(Hash_t hKey, bool bAfter) {
    this->m_umQueues[hKey] = std::make_pair(bAfter, Queue_t(&this->m_mutLock));
}

Queue_t& Drawing_t::GetQueue(Hash_t hKey) {
    return this->m_umQueues[hKey].second;
}

void Drawing_t::RemoveQueue(Hash_t hKey) {
    if (this->m_umQueues.contains(hKey))
        this->m_umQueues.erase(hKey);
}

void Drawing_t::AddFont(Hash_t hKey, const void* pContents, uint32_t u32Size, float flSize) {
    this->m_umFonts[hKey] = this->m_pImGuiIO->Fonts->AddFontFromMemoryCompressedTTF(pContents, u32Size, flSize);
}

const ImFont* Drawing_t::GetFont(Hash_t hKey) const {
    return this->m_umFonts.at(hKey);
}

void Drawing_t::RemoveFont(Hash_t hKey) {
    if (this->m_umFonts.contains(hKey))
        this->m_umFonts.erase(hKey);
}