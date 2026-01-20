// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

/// @file CancelFlag.h
/// Global render cancellation flag - shared between rendering_pbr and rendering_rndr
/// to break circular dependency on Windows.

#pragma once

#include <atomic>

// Windows DLL export/import declarations
#ifdef _MSC_VER
    #ifdef rendering_mcrt_common_EXPORTS
        #define MCRT_COMMON_API __declspec(dllexport)
    #else
        #define MCRT_COMMON_API __declspec(dllimport)
    #endif
#else
    #define MCRT_COMMON_API
#endif

namespace moonray {
namespace mcrt_common {

// We want to align this on a cache line. False sharing caused a simple render
// to go from 25 seconds to more than a minute and a half.
struct alignas(64) CancelFlag
{
    CancelFlag() noexcept
    : mCanceled(false)
    {
    }

    bool isCanceled() const noexcept
    {
        return mCanceled.load(std::memory_order_relaxed);
    }

    void set(bool v) noexcept
    {
        mCanceled.store(v, std::memory_order_relaxed);
    }

    std::atomic_bool mCanceled;
};

/// Global cancel flag - checked by isRenderCanceled()
MCRT_COMMON_API extern CancelFlag gCancelFlag;

} // namespace mcrt_common
} // namespace moonray

// Can be called from C++ and ISPC to check if render has been canceled
extern "C" MCRT_COMMON_API bool isRenderCanceled();
