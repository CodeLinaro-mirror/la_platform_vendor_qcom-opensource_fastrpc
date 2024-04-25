// Copyright (c) 2024, Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef __RPCMEM_INTERNAL_H__
#define __RPCMEM_INTERNAL_H__

#include "rpcmem.h"

int rpcmem_to_fd_internal(void *po);
void *rpcmem_alloc_internal(int heapid, uint32 flags, size_t size);
void rpcmem_free_internal(void *po);

#endif /*__RPCMEM_INTERNAL_H__*/
