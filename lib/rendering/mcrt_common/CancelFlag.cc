// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "CancelFlag.h"

namespace moonray {
namespace mcrt_common {

CancelFlag gCancelFlag;

} // namespace mcrt_common
} // namespace moonray

// Can be called from C++ and ISPC
extern "C" bool isRenderCanceled()
{
    return moonray::mcrt_common::gCancelFlag.isCanceled();
}
