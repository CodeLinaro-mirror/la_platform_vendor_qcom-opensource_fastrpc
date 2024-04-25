// Copyright (c) 2024, Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef __FASTRPC_CAP_H__
#define __FASTRPC_CAP_H__

// Status notification version 2 capability value
#define STATUS_NOTIF_V2 3

int fastrpc_get_cap(uint32_t domain, uint32_t attributeID, uint32_t *capability);
uint32_t get_dsp_dma_reverse_rpc_map_capability(int domain);
int check_error_code_change_present(void);
int is_status_notif_version2_supported(int domain);
int is_userspace_allocation_supported(void);
int is_proc_sharedbuf_supported_dsp(int domain);
int check_error_code_change_present();

#endif /*__FASTRPC_CAP_H__*/
