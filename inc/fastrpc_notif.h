// Copyright (c) 2024, Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef FASTRPC_NOTIF_H
#define FASTRPC_NOTIF_H

#include "remote64.h"
#include "fastrpc_internal.h"

/*
 * Internal function to create fastrpc status notification thread
 * @ domain: domain to which notification is initialized
 * returns 0 on success
 */
int fastrpc_notif_domain_init(int domain);

/*
 * Internal function to exit fastrpc status notification thread
 * @ domain: domain to which notification is de-initialized
 * returns 0 on success
 */
void fastrpc_notif_domain_deinit(int domain);

/*
 * Internal function to get notification response from kernel. Waits in kernel until notifications are received from DSP
 * @ domain: domain to which notification needs to be received
 * returns 0 on success
 */
int get_remote_notif_response(int domain);

void fastrpc_notif_init();
void fastrpc_notif_deinit();
void fastrpc_cleanup_notif_list();
int fastrpc_notif_register(int domain, struct remote_rpc_notif_register *notif);

#endif // FASTRPC_NOTIF_H
