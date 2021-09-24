#pragma once

#include "Forward.hh"

class SDK::CGlobalVarsBase {
  public:
    float m_flRealTime;
    int m_nFrameCount;

    float m_flAbsFrameTime;
    float m_flAbsFrameTimeStdDev;

    float m_flCurTime;
    float m_flFrameTime;

    int m_nMaxClients;

    int m_iTickCount;
    float m_flIntervalPerTick;

    PAD(4);

    bool m_bClient;
    bool m_bRemoteClient;
};