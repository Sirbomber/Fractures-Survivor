
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <type_traits>
#include <cstdint>

using uintptr = uintptr_t;


#define CCF2_BUILD   0
#define SWARMZ_BUILD 0
#define C_EXPORT extern "C" __declspec(dllexport)  // Macro for defining C-style exports.


constexpr unsigned   Op2Base     = 0x400000;  // Preferred load address of Outpost2.exe.
inline    HINSTANCE  g_hInst     = nullptr;   // HINSTANCE of this DLL.
inline    HMODULE    g_hOutpost2 =            // HMODULE of Outpost2.exe.
  [] { const auto hMod = GetModuleHandleA(nullptr);  return hMod ? hMod : reinterpret_cast<HMODULE>(Op2Base); }();


template <typename T> constexpr bool ArgIsPointer  = std::is_pointer_v<T> || std::is_array_v<T> || std::is_function_v<T>;

// Functions to reference memory in Outpost2.exe.

// Reference OP2 memory by pointer.
template <typename T = void*, typename R = std::enable_if_t<ArgIsPointer<T>, T>>
T Op2Mem(uintptr address) { return   reinterpret_cast<T>(reinterpret_cast<char*>(g_hOutpost2) + (address - Op2Base));  }
// Reference OP2 memory by reference.
template <typename T,         typename R = std::enable_if_t<std::is_reference_v<T>, T>>
R Op2Mem(uintptr address) { return reinterpret_cast<T>(*(reinterpret_cast<char*>(g_hOutpost2) + (address - Op2Base))); }
// Reference OP2 memory, with built-in init-once.
template <uintptr Address, typename T = void*>  T Op2Mem() { static const T p = Op2Mem<T>(Address);  return p; }
// Call OP2 function, with built-in init-once.
template <uintptr Address, typename Fn, typename... Args>  auto Op2Thunk(Args&&... args)
  { return Op2Mem<Address, std::conditional_t<std::is_function_v<Fn>, Fn*, Fn>>()(std::forward<Args>(args)...); }


template <typename T1, typename T2>
constexpr auto Max(T1 a, T2 b) -> std::remove_reference_t<decltype(true ? a : b)> { return (a > b) ? a : b; }
template <typename T1, typename T2>
constexpr auto Min(T1 a, T2 b) -> std::remove_reference_t<decltype(true ? a : b)> { return (a < b) ? a : b; }


constexpr unsigned MaxMapWidth  = 1024;
constexpr unsigned MaxMapHeight = 512;


#if _MSC_VER
# define strcasecmp _stricmp
#endif

// Printfs a debug message.  You need to include stdio.h and windows.h to use this.
#if _DEBUG || ENABLE_DEBUG_MSG
# define DEBUG_MSG(format, ...)  \
  { char dbgStr[1024]; snprintf(dbgStr, sizeof(dbgStr), format "\n", __VA_ARGS__); OutputDebugStringA(dbgStr); } (void)0
#else
# define DEBUG_MSG(format, ...)
#endif
