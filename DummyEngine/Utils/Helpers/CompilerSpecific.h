#pragma once

#include "DummyEngine/Utils/Types/Types.h"  // IWYU pragma: keep

#ifdef __GNUC__
#define DE_COMPILER_GCC_VERSION_MAJOR __GNUC__
#define DE_COMPILER_GCC_VERSION_MINOR __GNUC_MINOR__
#else
#define DE_COMPILER_GCC_VERSION_MAJOR 0
#define DE_COMPILER_GCC_VERSION_MINOR 0
#endif

#ifdef __clang_major__
#define DE_COMPILER_CLANG_VERSION_MAJOR __clang_major__
#else
#define DE_COMPILER_CLANG_VERSION_MAJOR 0
#endif

#if (DE_COMPILER_GCC_VERSION_MAJOR >= 12 || (DE_COMPILER_GCC_VERSION_MAJOR == 11 && DE_COMPILER_GCC_VERSION_MINOR >= 4)) || \
    DE_COMPILER_CLANG_VERSION_MAJOR >= 12
#define DE_COMPILER_PATH_HASH
#endif

#if DE_PLATFORM_WINDOWS
#define DE_SCRIPT_API extern "C" __declspec(dllexport)
#endif

#if DE_PLATFORM_LINUX
#define DE_SCRIPT_API extern "C"
#endif

#if DE_PLATFORM_MACOS
#define DE_SCRIPT_API extern "C"
#undef DE_COMPILER_PATH_HASH
#endif

namespace std {  // NOLINT
#ifndef DE_COMPILER_PATH_HASH
    template <> struct hash<DummyEngine::Path> {
        std::size_t operator()(const DummyEngine::Path& path) const { return hash_value(path); }
    };
#endif
}  // namespace std
