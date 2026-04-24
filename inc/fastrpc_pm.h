// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#ifndef FASTRPC_PM_H
#define FASTRPC_PM_H

/*
	fastrpc_wake_lock:

	Takes the wake-lock
	Args: None
	Returns: Integer - 0 on success
*/
int fastrpc_wake_lock();

/*
	fastrpc_wake_unlock:

	Releases the wake-lock
	Args: None
	Returns: Integer - 0 on success
*/
int fastrpc_wake_unlock();

/*
	fastrpc_wake_lock_init:

	Initializes the fastrpc wakelock struct
	Args: None
	Returns: Integer - 0 on success
*/
int fastrpc_wake_lock_init();

/*
	fastrpc_wake_lock_deinit:

	De-initializes the fastrpc wakelock struct
	Args: None
	Returns: Integer - 0 on success
*/
int fastrpc_wake_lock_deinit();
#endif //FASTRPC_PM_H
