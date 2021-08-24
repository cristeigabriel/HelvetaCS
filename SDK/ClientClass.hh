#pragma once

#include "Forward.hh"
#include "../Helpers/Helpers.hh"

class SDK::DVariant
{
public:
	union
	{
		float m_flVariant;
		long m_lVariant;
		char *m_szVariant;
		Vector_t<float>::V3 m_vecVariant;
	};

	int m_nType;
};

class SDK::CRecvProxyData
{
public:
	const RecvProp *m_pRecvProp;
	DVariant m_Value;
	int m_iElement;
	int m_ObjectId;
};

struct SDK::RecvTable
{
	RecvProp *m_pProps;
	int m_nProps;

	PAD(4);

	char *m_szNetworkTableName;

private:
	bool m_bInitialized;
	bool m_bInMainList;
};

struct SDK::RecvProp
{
	char *m_szVarName;
	int m_nRecvType;
	int m_Flags;
	int m_StringBufferSize;
	bool m_bInsideArray;

	PAD(4);

	RecvProp *m_pArrayProp;
	ArrayLengthRecvProxy m_ArrayLengthProxy;

	RecvVarProxy m_ProxyFn;
	DataTableRecvVarProxy m_DataTableProxyFn;

	RecvTable *m_pDataTable;
	int m_Offset;

	int m_ElementsStride;
	int m_nElements;

	const char *m_szParentArrayPropName;
};

class SDK::ClientClass
{
public:
	CreateClientClass m_CreateFn;
	CreateEvent m_CreateEventFn;

	char *m_szNetworkName;

	RecvTable *m_pRecvTable;
	ClientClass *m_pNext;

	int m_nClassId;
};