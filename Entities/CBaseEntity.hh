#pragma once

#include "../SDK/Forward.hh"
#include "../Helpers/Helpers.hh"
#include "Forward.hh"

class IClientNetworkable {
    virtual void pad() = 0;

  public:
    virtual void Release()                                = 0;
    virtual SDK::ClientClass* GetAllClasses()             = 0;
    virtual void NotifyShouldTransmit(int iState)         = 0;
    virtual void OnPreDataChanged(int iType)              = 0;
    virtual void OnDataChanged(int iType)                 = 0;
    virtual void PreDataUpdate(int iType)                 = 0;
    virtual void PostDataUpdate(int iType)                = 0;
    virtual void OnDataUnchangedInPvs()                   = 0;
    virtual bool IsDormant()                              = 0;
    virtual int Index() const                             = 0;
    virtual void ReceiveMessage(int iClassId, void* pMsg) = 0;
    virtual void* GetDataTable()                          = 0;
    virtual void SetDestroyedOnRecreateEntities()         = 0;
};

/**
* @todo: Implement Prediction DataMap to replace current MoveType getter
* 
*/
NETWORKED_CLASS(CBaseEntity,
                NETWORKED_VARIABLE_DEDUCE(m_iTeamNum);

                IClientNetworkable * Networkable();
                OFFSET(uint8_t, m_nMoveType, g_Netvars.Get(hHash, HASH("m_nRenderMode")) + 1);

                bool ComputeHitboxSurroundingBox(Vector_t<float>::V3* pVecMins, Vector_t<float>::V3* pVecMaxs);
                Vector_t<float>::V3 & GetAbsOrigin();
                void SetAbsOrigin(const Vector_t<float>::V3& vecOrigin););