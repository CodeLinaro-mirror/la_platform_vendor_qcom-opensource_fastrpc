// Copyright (c) 2024, Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef FASTRPC_INTERNAL_COMMON_H
#define FASTRPC_INTERNAL_COMMON_H

/* File only compiled  when support to downstream kernel is required*/

#include <sys/ioctl.h>

/**
 * FastRPC IOCTL functions
 **/
#define FASTRPC_IOCTL_INVOKE              _IOWR('R', 1, struct fastrpc_ioctl_invoke)
#define FASTRPC_IOCTL_MMAP                _IOWR('R', 2, struct fastrpc_ioctl_mmap)
#define FASTRPC_IOCTL_MUNMAP              _IOWR('R', 3, struct fastrpc_ioctl_munmap)
#define FASTRPC_IOCTL_INVOKE_FD           _IOWR('R', 4, struct fastrpc_ioctl_invoke_fd)
#define FASTRPC_IOCTL_SETMODE             _IOWR('R', 5, uint32)
#define FASTRPC_IOCTL_INIT                _IOWR('R', 6, struct fastrpc_ioctl_init)
#define FASTRPC_IOCTL_INVOKE_ATTRS        _IOWR('R', 7, struct fastrpc_ioctl_invoke_attrs)
#define FASTRPC_IOCTL_GETINFO             _IOWR('R', 8, uint32)
#define FASTRPC_IOCTL_GETPERF             _IOWR('R', 9, struct fastrpc_ioctl_perf)
#define FASTRPC_IOCTL_INIT_ATTRS          _IOWR('R', 10, struct fastrpc_ioctl_init_attrs)
#define FASTRPC_IOCTL_INVOKE_CRC          _IOWR('R', 11, struct fastrpc_ioctl_invoke_crc)
#define FASTRPC_IOCTL_CONTROL             _IOWR('R', 12, struct fastrpc_ioctl_control)
#define FASTRPC_IOCTL_MUNMAP_FD           _IOWR('R', 13, struct fastrpc_ioctl_munmap_fd)
#define FASTRPC_IOCTL_MMAP_64             _IOWR('R', 14, struct fastrpc_ioctl_mmap)
#define FASTRPC_IOCTL_MUNMAP_64           _IOWR('R', 15, struct fastrpc_ioctl_munmap)
#define FASTRPC_IOCTL_GET_DSP_INFO        _IOWR('R', 16, struct fastrpc_ioctl_dsp_capabilities)
#define FASTRPC_IOCTL_GET_DSP_ATTR        _IOWR('R', 17, struct remote_dsp_capability)
#define FASTRPC_IOCTL_INVOKE2             _IOWR('R', 18, struct fastrpc_ioctl_invoke2)
#define FASTRPC_IOCTL_MEM_MAP             _IOWR('R', 19, struct fastrpc_ioctl_mem_map)
#define FASTRPC_IOCTL_MEM_UNMAP           _IOWR('R', 20, struct fastrpc_ioctl_mem_unmap)
#define FASTRPC_IOCTL_INVOKE_PERF         _IOWR('R', 21, struct fastrpc_ioctl_invoke_perf)
#define FASTRPC_IOCTL_DSPSIGNAL_CREATE    _IOWR('R', 23, struct fastrpc_ioctl_dspsignal_create)
#define FASTRPC_IOCTL_DSPSIGNAL_DESTROY   _IOWR('R', 24, struct fastrpc_ioctl_dspsignal_destroy)
#define FASTRPC_IOCTL_DSPSIGNAL_SIGNAL    _IOWR('R', 25, struct fastrpc_ioctl_dspsignal_signal)
#define FASTRPC_IOCTL_DSPSIGNAL_WAIT      _IOWR('R', 26, struct fastrpc_ioctl_dspsignal_wait)
#define FASTRPC_IOCTL_DSPSIGNAL_CANCEL_WAIT _IOWR('R', 27, struct fastrpc_ioctl_dspsignal_cancel_wait)

#define DEVICE_NAME "adsprpc-smd"

#define ADSPRPC_DEVICE "/dev/adsprpc-smd"
#define SDSPRPC_DEVICE "/dev/sdsprpc-smd"
#define MDSPRPC_DEVICE "/dev/mdsprpc-smd"
#define CDSPRPC_DEVICE "/dev/cdsprpc-smd"

/* Secure and default device nodes */
#define SECURE_DEVICE "/dev/adsprpc-smd-secure"
#define DEFAULT_DEVICE "/dev/adsprpc-smd"

#define FASTRPC_ATTR_NOVA (1)

#define INITIALIZE_REMOTE_ARGS(total)	struct remote_arg* args = NULL; \
					int fds[32], *pfds = fds;	\
					unsigned attrs[32], *pattrs = attrs; \
					args = pra; \
					if (total > STD_ARRAY_SIZE(fds)) {	\
						 VERIFYC(NULL != (pfds = malloc(total * sizeof(*pfds))), AEE_ENOMEMORY);	\
					}	\
					if (total > STD_ARRAY_SIZE(attrs)) {	\
						VERIFYC(NULL != (pattrs = malloc(total * sizeof(*pattrs))), AEE_ENOMEMORY);	\
					}	\

#define DESTROY_REMOTE_ARGS()		if(pfds!=fds) {	\
						free(pfds);	\
						pfds = NULL;	\
					}	\
					if(pattrs!=attrs) {	\
						free(pattrs);	\
						pattrs = NULL;	\
					}

#define set_args(i, ptr, len, fd, attr)	\
                    pra[i].buf.pv = ptr; \
					pra[i].buf.nLen = len; \
                    pfds[i] = fd;  \
					pattrs[i] = attr;

#define set_args_ptr(i, ptr)		0
#define set_args_len(i, len)		0
#define set_args_attr(i, attr)		pattrs[i] = attr
#define set_args_fd(i, fd)		pfds[i] = fd
#define get_args_ptr(i)			pra[i].buf.pv
#define get_args_len(i)			pra[i].buf.nLen
#define get_args_attr(i)		pattrs[i]
#define get_args_fd(i)			pfds[i]
#define append_args_attr(i, attr)	pattrs[i] |= attr
#define get_args()			pra
#define is_upstream()			0

/* Map and unmap IOCTL methods reserved memory size for future extensions */
#define MAP_RESERVED_NUM (14)
#define UNMAP_RESERVED_NUM (10)

#ifndef FASTRPC_MAX_DSP_ATTRIBUTES_FALLBACK
#define FASTRPC_MAX_DSP_ATTRIBUTES_FALLBACK (ARCH_VER + 1) // number of attributes for older kernel (fallback)
#endif

struct fastrpc_ioctl_invoke {
   remote_handle handle;
   uint32_t sc;
   remote_arg* pra;
};

struct fastrpc_ioctl_invoke_fd {
   struct fastrpc_ioctl_invoke inv;
   int *fds;
};

struct fastrpc_ioctl_invoke_attrs {
   struct fastrpc_ioctl_invoke inv;
   int *fds;
   unsigned *attrs;
};

struct fastrpc_ioctl_invoke_crc {
   struct fastrpc_ioctl_invoke inv;
   int *fds;
   unsigned *attrs;
   unsigned int *crc;
};

struct fastrpc_ioctl_invoke_perf {
   struct fastrpc_ioctl_invoke inv;
   int *fds;
   unsigned *attrs;
   unsigned int *crc;
   uint64_t *perf_kernel;
   uint64_t *perf_dsp;
};

enum fastrpc_invoke2_type {
	FASTRPC_INVOKE2_ASYNC		= 1,
	FASTRPC_INVOKE2_ASYNC_RESPONSE		= 2,

	/*
	 * Same IOCTL call can be used to enable any future
	 * kernel optimizations as well.
	 */
	FASTRPC_INVOKE2_KERNEL_OPTIMIZATIONS,

	/* IOCTL call for notification response from DSP */
	FASTRPC_INVOKE2_STATUS_NOTIF,
	/* IOCTL call for sharing buffer to dsp */
	 FASTRPC_INVOKE2_PROC_SHAREDBUF_INFO,
	  /* IOCTL call to set session info of remote sub system */
	FASTRPC_INVOKE2_SESS_INFO,

};

// Async job ioctl invocation structure
struct fastrpc_ioctl_invoke_async {
        struct fastrpc_ioctl_invoke inv;
        int *fds;
        unsigned *attrs;
        unsigned int *crc;
        uint64_t *perf_kernel;
        uint64_t *perf_dsp;
        struct fastrpc_async_job *job;
};

struct fastrpc_ioctl_async_response {
	fastrpc_async_jobid jobid; // Job id sent during Async call
	int result;                // Result from DSP
	uint64_t *perf_kernel;     // Kernel perf data pointer
	uint64_t *perf_dsp;        // DSP perf data pointer
	uint32_t handle;
	uint32_t sc;
};

struct fastrpc_ioctl_invoke2 {
	uint32_t req;       /* type of invocation request */
	uintptr_t invparam; /* invocation request param */
	uint32_t size;      /* size of invocation param */
	int err;            /* reserved */
};

struct fastrpc_ioctl_init {
   uint32_t flags;
   byte* file;
   int filelen;
   int filefd;
   void* mem;
   int memlen;
   int memfd;
};

struct fastrpc_ioctl_init_attrs {
   struct fastrpc_ioctl_init init;
   int attrs;
   int siglen;
};

struct fastrpc_ioctl_munmap {
   uint64_t vaddrout; /* optional virtual address, if non zero, dsp will use vaaddrin */
   ssize_t size;       /* size */
};

struct fastrpc_ioctl_mmap {
   int fd;             /* ion handle */
   uint32 flags;       /* flags to map with */
   uint64_t vaddrin;  /* virtual address */
   ssize_t size;       /* size */
   uint64_t vaddrout; /* dsps virtual address */
};

struct fastrpc_ioctl_munmap_fd {
   int     fd;		/* fd */
   uint32_t  flags;	/* control flags */
   uintptr_t va;	/* va */
   ssize_t  len;	/* length */
};

struct fastrpc_ioctl_perf { /* kernel performance data */
	uintptr_t data;
	uint32_t numkeys;
	uintptr_t keys;
};

struct fastrpc_ioctl_control {
	uint32_t req;
	union {
		struct fastrpc_ctrl_latency lp;
		struct fastrpc_ctrl_smmu smmu;
		struct fastrpc_ctrl_kalloc kalloc;
		struct fastrpc_ctrl_wakelock wl;
		struct fastrpc_ctrl_pm pm;
	};
};

struct fastrpc_ioctl_dspsignal_create {
    uint32_t signal_id; /* Signal ID */
    uint32_t flags;     /* Flags, currently unused */
};

struct fastrpc_ioctl_dspsignal_destroy {
    uint32_t signal_id; /* Signal ID */
};

struct fastrpc_ioctl_dspsignal_signal {
    uint32_t signal_id; /* Signal ID */
};

struct fastrpc_ioctl_dspsignal_wait {
    uint32_t signal_id;    /* Signal ID */
    uint32_t timeout_usec; /* Timeout in microseconds. UINT32_MAX for an infinite wait */
};

struct fastrpc_ioctl_dspsignal_cancel_wait {
    uint32_t signal_id; /* Signal ID */
};

struct fastrpc_ioctl_mem_map {
	int version;
	union {
		struct fastrpc_mem_map m;
		int reserved[MAP_RESERVED_NUM];
	};
};

/* unmap memory to DSP device */
struct fastrpc_ioctl_mem_unmap {
	int version;		/* Initial version 0 */
	union {
		struct fastrpc_mem_unmap um;
		int reserved[UNMAP_RESERVED_NUM];
	};
};

struct fastrpc_ioctl_dsp_capabilities {
    uint32_t domain;                         //! DSP domain to query capabilities
    uint32_t dsp_attributes[FASTRPC_MAX_DSP_ATTRIBUTES_FALLBACK];
};

// Notification worker thread response structure
struct fastrpc_ioctl_notif_response {
	int domain;						// Domain of User PD
	int session;					// Session ID of User PD
	uint32_t status;				// Status of DSP User PD
};

//Utility macros for reading the ioctl structure
#define NOTIF_GETDOMAIN(response)	((struct fastrpc_ioctl_notif_response*) response)->domain;
#define NOTIF_GETSESSION(response)	((struct fastrpc_ioctl_notif_response*) response)->session;
#define NOTIF_GETSTATUS(response)	((struct fastrpc_ioctl_notif_response*) response)->status;

#endif /* FASTRPC_INTERNAL_COMMON_H */
