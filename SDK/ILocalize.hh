#pragma once

#include "Forward.hh"

class SDK::ILocalize {
  public:
	const char* Find(const char* szLocalizationToken);
};