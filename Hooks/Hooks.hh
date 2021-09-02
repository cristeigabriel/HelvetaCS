#pragma once

#include <Windows.h>

namespace Hooks {
static HWND hwndWindow = nullptr;
extern inline void Bootstrap();
}  // namespace Hooks