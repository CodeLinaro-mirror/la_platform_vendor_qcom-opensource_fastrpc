// Copyright (c) 2024, Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
int adspmsgd_init(remote_handle64 handle, int filter);

typedef void* (*reader_thread)();

typedef struct {
  volatile int threadStop;// variable to stop the msgd HLOS thread
  bool thread_running; // to check whether logger thread was launched
  unsigned int bufferSize; //size of msgd shared buffer
  unsigned int readIndex; //the index from which msgd thread starts reading
  unsigned int* currentIndex; //if currentIndex is same as readIndex then msgd thread waits for messages from DSP
  char* headPtr; //head pointer to the msgd shared buffer
  char* message; //scratch buffer used to print messages
  pthread_t msgreader_thread;
  FILE *log_file_fd; // file descriptor to save runtime farf logs
} msgd;

void adspmsgd_log_message(char *format, char *msg);
void adspmsgd_stop(int);

