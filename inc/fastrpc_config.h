// Copyright (c) 2024, Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef __FASTRPC_CONFIG_H__
#define __FASTRPC_CONFIG_H__

#include "AEEstd.h"

#define MAX_PANIC_ERR_CODES 32 /* Maximum number of panic error codes allowed in debug config. */

struct err_codes {
   int err_code[MAX_PANIC_ERR_CODES]; /* Panic err codes read from debug config. */
   int num_err_codes;  /* Number of panic error codes read from debug config. */
};

struct err_codes *fastrpc_config_get_errcodes(void);
int fastrpc_config_get_rpctimeout(void);
// Function to get if PD dump feature is enabled.
boolean fastrpc_config_is_pddump_enabled(void);
// Functions to get if profiling mode is enabled.
boolean fastrpc_config_is_perfkernel_enabled(void);
boolean fastrpc_config_is_perfdsp_enabled(void);
// Function to get the file name to collect runtime farf logs.
char *fastrpc_config_get_runtime_farf_file(void);
// Function to get the file name to collect userspace runtime farf logs.
char *fastrpc_config_get_userspace_runtime_farf_file(void);
// Function to get if iregion logging feature is enabled.
boolean fastrpc_config_is_log_iregion_enabled(void);
// Function to get if debug logging feature is enabled.
boolean fastrpc_config_is_debug_logging_enabled(void);
// Function to get if debug logging feature is enabled for sysmon reserved bit.
boolean fastrpc_config_is_sysmon_reserved_bit_enabled(void);
// Function to get if QTF tracing is enabled.
boolean fastrpc_config_is_qtf_tracing_enabled(void);
// Function to get heap caller level.
int fastrpc_config_get_caller_level(void);
// Function to get if uaf should be enabled in heap
boolean fastrpc_config_is_uaf_enabled(void);
// Function to get if Log packet is enabled.
boolean fastrpc_config_is_logpacket_enabled(void);
// Function to get if leak detect is enabled
int fastrpc_config_get_leak_detect(void);
// Function to return the call stack num
int fastrpc_config_get_caller_stack_num(void);
// Fastrpc config init function
int fastrpc_config_init();

#endif /*__FASTRPC_CONFIG_H__*/
