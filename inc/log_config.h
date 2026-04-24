// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#ifndef __LOG_CONFIG_H__
#define __LOG_CONFIG_H__
/**
  * @brief API to initialize logging framework
  * creates a thread and looks for .farf filebuf
  * when found, reads the file for log levels that
  * should be enabled on APSS and DSP.
  **/
int initFileWatcher(int domain);
/**
  * @brief API to de-initialize logging framework
  * sets an exit flag and wait for the thread to join.
  **/
void deinitFileWatcher(int domain);

#endif /*__LOG_CONFIG_H__*/
