#include "Dll.hh"

#include "../Vendor/WinApi/WinApi.hh"

Memory::Pointer_t::Pointer_t(uintptr_t uptrAddress) {
	this->m_uptrAddress = uptrAddress;
	assert(this->m_uptrAddress);
}

const Memory::Pointer_t Memory::Pointer_t::FollowUntil(uint8_t u8Opcode, bool bForward) const {
	uintptr_t uptrAddress = this->m_uptrAddress;

	do {
		uptrAddress += bForward ? 1 : -1;
	} while (*(uint8_t*)(uptrAddress) != u8Opcode);

	return Pointer_t(uptrAddress);
}

Memory::Dll_t::Dll_t(const char* szName) {
	this->m_szName = szName;

	this->m_prgBytes = (uint8_t*)(GetModuleA(szName));
	assert(this->m_prgBytes != nullptr);

	this->m_pDosHeader = (PIMAGE_DOS_HEADER)(this->m_prgBytes);

	this->m_pNtHeaders = (PIMAGE_NT_HEADERS)((DWORD_PTR)this->m_prgBytes + this->m_pDosHeader->e_lfanew);
	assert(this->m_pNtHeaders);

	PIMAGE_SECTION_HEADER pSectionList = IMAGE_FIRST_SECTION(this->m_pNtHeaders);
	assert(pSectionList);

	for (auto i = 0u; i < this->m_pNtHeaders->FileHeader.NumberOfSections; ++i) {
		const char* szName					= (const char*)pSectionList->Name;
		this->m_umSections[RT_HASH(szName)] = Section_t {szName, pSectionList->PointerToRawData, pSectionList->SizeOfRawData};
		++pSectionList;
	}
}