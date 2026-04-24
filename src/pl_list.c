// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
// SPDX-License-Identifier: BSD-3-Clause-Clear 
#include "platform_libs.h"

PL_DEP(gpls)
PL_DEP(listener_android)

struct platform_lib *(*pl_list[])(void) = {PL_ENTRY(gpls),
                                           PL_ENTRY(listener_android), 0};
