#include "Netvars.hh"

#include "../Helpers/Helpers.hh"

#include "../SDK/ClientClass.hh"
#undef NDEBUG
#include <assert.h>

Netvars_t::Netvars_t(SDK::ClientClass* pClassHead) {
    assert(pClassHead);

    for (SDK::ClientClass* pCopy = pClassHead; pClassHead; pClassHead = pClassHead->m_pNext) {
        if (pClassHead->m_pRecvTable) {
            Dump(pClassHead->m_szNetworkName, pClassHead->m_pRecvTable);
        }
    }
}

void Netvars_t::Dump(const char* szNetworkName, SDK::RecvTable* pTable, ptrdiff_t ptrdiffOffset) {
    for (int i = 0; i < pTable->m_nProps; ++i) {
        SDK::RecvProp& prop = pTable->m_pProps[i];

        if (std::isdigit(prop.m_szVarName[0]) || (prop.m_szVarName[0] == 'b' && prop.m_szVarName[4] == 'c')) {
            continue;
        }

        if (prop.m_nRecvType == 6 && prop.m_pDataTable && prop.m_pDataTable->m_szNetworkTableName[0] == 'D') {
            Dump(szNetworkName, prop.m_pDataTable, prop.m_Offset + ptrdiffOffset);
        }

        this->m_umPtrdiffs[RT_HASH(szNetworkName)][RT_HASH(prop.m_szVarName)] = prop.m_Offset;
    }
}

ptrdiff_t Netvars_t::Get(Hash_t hClass, Hash_t hProperty) const {
    ptrdiff_t ptrdiffResult = this->m_umPtrdiffs.at(hClass).at(hProperty);
    assert(ptrdiffResult);
    return ptrdiffResult;
}