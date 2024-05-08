// Copyright (c) 2024, Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef FASTRPC_INTERNAL_H
#define FASTRPC_INTERNAL_H

#include <errno.h>
#include <stdbool.h>
#include <semaphore.h>

#include "HAP_farf.h"
#include "AEEStdErr.h"
#include "remote64.h"
#include "verify.h"
#include "AEEstd.h"
#include "AEEQList.h"
#include "AEEStdErr.h"
#include "fastrpc_latency.h"
#include "fastrpc_common.h"


#define ALIGN_B(p, a)	      (((p) + ((a) - 1)) & ~((a) - 1))

#define FASTRPC_SESSION_ID1  (4)

// URI string to choose Session 1.
#define FASTRPC_SESSION1_URI "&_session=1"

// URI string to choose a particular session. Session ID needs to be appended to Session uri.
#define FASTRPC_SESSION_URI "&_session="

// URI string of Domain. Domain name needs to be appended to domain URI.
#define FASTRPC_DOMAIN_URI "&_dom="

/* Additional URI length required to add domain and session information to URI
 * Two extra characters for session ID > 9 and domain name more than 4 characters.
 */
#define FASTRPC_URI_BUF_LEN (strlen(CDSP_DOMAIN) + strlen(FASTRPC_SESSION1_URI) + 2)

/**
 * Maximum values of enums exposed in remote
 * header file to validate the user-input.
 * Update these values for any additions to
 * the corresponding enums.
 **/
/* Max value of fastrpc_async_notify_type, used to validate the user input */
#define FASTRPC_ASYNC_TYPE_MAX FASTRPC_ASYNC_POLL + 1

/* Max value of remote_dsp_attributes, used to validate the attribute ID*/
#define FASTRPC_MAX_DSP_ATTRIBUTES STATUS_NOTIFICATION_SUPPORT + 1

/* Max value of remote_mem_map_flags, used to validate the input flag */
#define REMOTE_MAP_MAX_FLAG REMOTE_MAP_MEM_STATIC + 1

/* Max value of fastrpc_map_flags, used to validate range of supported flags */
#define FASTRPC_MAP_MAX FASTRPC_MAP_FD_NOMAP + 1

#if !(defined __qdsp6__) && !(defined __hexagon__)
static __inline uint32 Q6_R_cl0_R(uint32 num) {
   int ii;
   for(ii = 31; ii >= 0; --ii) {
      if(num & (1 << ii)) {
         return 31 - ii;
      }
   }
   return 0;
}
#else
#include "hexagon_protos.h"
#include <types.h>
#endif

#define FASTRPC_INFO_SMMU   (1 << 0)

#define GET_SESSION_ID_FROM_DOMAIN_ID(domain_id) ((int)(domain_id / NUM_DOMAINS))

/* From actual domain ID (0-3) and session ID, get effective domain ID */
#define GET_EFFECTIVE_DOMAIN_ID(domain, session) (domain + (NUM_DOMAINS * session))

#define FASTRPC_INIT_ATTACH      0
#define FASTRPC_INIT_CREATE      1
#define FASTRPC_INIT_CREATE_STATIC  2
#define FASTRPC_INIT_ATTACH_SENSORS 3

/*
 * Process types on remote subsystem
 * Always add new PD types at the end, before MAX_PD_TYPE,
 * for maintaining back ward compatibility
 */
#define DEFAULT_UNUSED    0  /* pd type not configured for context banks */
#define ROOT_PD           1  /* Root PD */
#define AUDIO_STATICPD    2  /* ADSP Audio Static PD */
#define SENSORS_STATICPD  3  /* ADSP Sensors Static PD */
#define SECURE_STATICPD   4  /* CDSP Secure Static PD */
#define OIS_STATICPD      5  /* ADSP OIS Static PD */
#define CPZ_USERPD        6  /* CDSP CPZ USER PD */
#define USERPD            7  /* DSP User Dynamic PD */
#define GUEST_OS_SHARED   8  /* Legacy Guest OS Shared */
#define MAX_PD_TYPE       9  /* Max PD type */


#define FASTRPC_ATTR_DEBUG_PROCESS (1)

#define MAX_DSPPD_NAMELEN  30

#ifndef FASTRPC_MAX_DSP_ATTRIBUTES_FALLBACK
#define FASTRPC_MAX_DSP_ATTRIBUTES_FALLBACK  1
#endif

struct fastrpc_thread_params {
	uint32_t thread_priority;
	uint32_t stack_size;
	int reqID;
	int update_requested;
	sem_t r_sem;
};

// Keeping this at the last as the MAX_DSP_ATTRIBUTES is defined in ioctl.
struct fastrpc_dsp_capabilities {
    uint32_t is_cached;                      //! Flag if dsp attributes are cached
    uint32_t dsp_attributes[FASTRPC_MAX_DSP_ATTRIBUTES_FALLBACK];
};

/* structure to hold fd and size of buffer shared with DSP,
* which contains inital debug parameters that needs to be passed
* during process initialization.
*/
struct fastrpc_proc_sharedbuf_info {
   int buf_fd;
   int buf_size;
};

enum {
   FASTRPC_DOMAIN_STATE_CLEAN = 0,
   FASTRPC_DOMAIN_STATE_INIT = 1,
   FASTRPC_DOMAIN_STATE_DEINIT = 2,
};

struct fastrpc_dsp_capabilities;

struct handle_list {
	QList ql;         //Forward domains handles
   QList nql;        //Forward non-domains handles
   QList rql;        //Reverse handles
   pthread_mutex_t lmut;
	pthread_mutex_t mut;
	pthread_mutex_t init;
	uint32_t constCount;
	uint32_t domainsCount;
	uint32_t nondomainsCount;
	uint32_t reverseCount;
	uint32_t state;
	uint32_t ref;
	int dsppd;
	char dsppdname[MAX_DSPPD_NAMELEN];
	char sessionname[MAX_DSPPD_NAMELEN];
	int domainsupport;
	int kmem_support;
	int dev;
	int setmode;
	uint32_t mode;
	uint32_t info;
	void* pdmem;
	remote_handle64 cphandle;
	remote_handle64 msghandle;
	remote_handle64 listenerhandle;
	remote_handle64 remotectlhandle;
	remote_handle64 adspperfhandle;
	int procattrs;
	struct fastrpc_latency qos;
	struct fastrpc_thread_params th_params;
	int unsigned_module;
	bool pd_dump;
	int first_revrpc_done;
	int disable_exit_logs;
	struct fastrpc_dsp_capabilities cap_info;
	int trace_marker_fd;
	uint64_t jobid;
	/* Capability flag to check if mapping DMA handle through reverse RPC is supported */
	int dma_handle_reverse_rpc_map_capability;
	/* Mutex to synchronize ASync init and deinit */
	pthread_mutex_t async_init_deinit_mut;
	uint32_t pd_initmem_size;  /** Initial memory allocated for remote userPD */
	uint32_t open_handle_count;           // Number of open handles
	bool is_session_reserved;   /** Set if session is reserved or used */
	/* buffer shared with DSP containing initial config parameters */
	void *proc_sharedbuf;
	/* current process shared buffer address to pack process params */
	uint32_t *proc_sharedbuf_cur_addr;
};

/* FastRPC ioctl structure to set session related info */
struct fastrpc_proc_sess_info {
   uint32_t domain_id;  /* Set the remote subsystem, Domain ID of the session  */
   uint32_t session_id; /* Unused, Set the Session ID on remote subsystem */
   uint32_t pd_type;    /* Set the process type on remote subsystem */
   uint32_t sharedcb;   /* Unused, Session can share context bank with other sessions */
};

 /*
  * Enum defined for updating non-domain and reverse handle list
  */
 typedef enum {
     NON_DOMAIN_LIST_PREPEND		=	0,
     NON_DOMAIN_LIST_DEQUEUE		=	1,
     REVERSE_HANDLE_LIST_PREPEND	=	2,
     REVERSE_HANDLE_LIST_DEQUEUE	=	3,
     DOMAIN_LIST_PREPEND		=	4,
     DOMAIN_LIST_DEQUEUE		=	5,
 } handle_list_update_id;

enum fastrpc_control_type {
	FASTRPC_CONTROL_LATENCY		=	1,
	FASTRPC_CONTROL_SMMU		=	2,
	FASTRPC_CONTROL_KALLOC		=	3,
	FASTRPC_CONTROL_WAKELOCK	=	4,
	FASTRPC_CONTROL_PM			=	5,
	FASTRPC_CONTROL_RPC_POLL	=	7,
	FASTRPC_CONTROL_ASYNC_WAKE	=	8,
	FASTRPC_CONTROL_NOTIF_WAKE	=	9,
};

struct fastrpc_ctrl_latency {
	uint32_t enable;	//!latency control enable
	uint32_t latency;	//! target latency in us
};

struct fastrpc_ctrl_smmu {
	uint32_t sharedcb;
};

struct fastrpc_ctrl_kalloc {
	uint32_t kalloc_support;
};

struct fastrpc_ctrl_wakelock {
	uint32_t enable;	//!wake lock enable
};

struct fastrpc_ctrl_pm {
	uint32_t timeout;	//!timeout (in ms) for which system should stay awake
};

enum fastrpc_invoke_type {
	INVOKE,
	INVOKE_ATTRS,
	INVOKE_FD,
	INVOKE_CRC,
	INVOKE_PERF,
};

enum fastrpc_map_type {
	MMAP,
	MUNMAP,
	MMAP_64,
	MUNMAP_64,
	MEM_MAP,
	MEM_UNMAP,
	MUNMAP_FD,
};

struct fastrpc_mem_map {
	int fd;			/* ion fd */
	int offset;		/* buffer offset */
	uint32_t flags;		/* flags defined in enum fastrpc_map_flags */
	int attrs;		/* buffer attributes used for SMMU mapping */
	uintptr_t vaddrin;	/* buffer virtual address */
	size_t length;		/* buffer length */
	uint64_t vaddrout;	/* [out] remote virtual address */
};

struct fastrpc_mem_unmap {
	int fd;			/* ion fd */
	uint64_t vaddr;		/* remote process (dsp) virtual address */
	size_t length;		/* buffer size */
};

struct fastrpc_map {
	int version;
	struct fastrpc_mem_map m;
};

#define FASTRPC_SMD_GUID "fastrpcsmd-apps-dsp"

struct smq_null_invoke32 {
   uint32_t ctx;         //! invoke caller context
   remote_handle handle; //! handle to invoke
   uint32_t sc;          //! scalars structure describing the rest of the data
};

struct smq_null_invoke {
   uint64_t ctx;         //! invoke caller context
   remote_handle handle; //! handle to invoke
   uint32_t sc;          //! scalars structure describing the rest of the data
};

typedef uint32_t smq_invoke_buf_phy_addr;

struct smq_phy_page {
   uint64_t addr; //! physical address
   int64_t size;  //! size
};

struct smq_phy_page32 {
   uint32_t addr; //! physical address
   uint32_t size; //! size
};

struct smq_invoke_buf {
   int num;
   int pgidx;
};

struct smq_invoke32 {
   struct smq_null_invoke32 header;
   struct smq_phy_page32 page;   //! remote arg and list of pages address
};

struct smq_invoke {
   struct smq_null_invoke header;
   struct smq_phy_page page;     //! remote arg and list of pages address
};

struct smq_msg32 {
   uint32_t pid;
   uint32_t tid;
   struct smq_invoke32 invoke;
};

struct smq_msg {
   uint32_t pid;
   uint32_t tid;
   struct smq_invoke invoke;
};

struct smq_msg_u {
   union {
      struct smq_msg32 msg32;
      struct smq_msg msg64;
   } msg;
   int size;
};

struct smq_invoke_rsp32 {
   uint32_t ctx;                 //! invoke caller context
   int nRetVal;                  //! invoke return value
};

struct smq_invoke_rsp {
   uint64_t ctx;                 //! invoke caller context
   int nRetVal;                  //! invoke return value
};

struct smq_invoke_rsp_u {
   union {
      struct smq_invoke_rsp32 rsp32;
      struct smq_invoke_rsp rsp64;
   } rsp;
   int size;
};

static __inline void to_smq_msg(uint32 mode, struct smq_msg_u* msg, struct smq_msg* msg64) {
   if(0 == mode) {
      msg64->pid = msg->msg.msg32.pid;
      msg64->tid = msg->msg.msg32.tid;
      msg64->invoke.header.ctx = msg->msg.msg32.invoke.header.ctx;
      msg64->invoke.header.handle = msg->msg.msg32.invoke.header.handle;
      msg64->invoke.header.sc = msg->msg.msg32.invoke.header.sc;
      msg64->invoke.page.addr = msg->msg.msg32.invoke.page.addr;
      msg64->invoke.page.size = msg->msg.msg32.invoke.page.size;
   } else {
      std_memmove(msg64, &msg->msg.msg64, sizeof(*msg64));
   }
}

static __inline void to_smq_invoke_rsp(uint32 mode, uint64 ctx, int nRetVal, struct smq_invoke_rsp_u* rsp) {
   if (0 == mode) {
      rsp->rsp.rsp32.ctx = (uint32)ctx;
      rsp->rsp.rsp32.nRetVal = nRetVal;
      rsp->size = sizeof(rsp->rsp.rsp32);
   } else {
      rsp->rsp.rsp64.ctx = ctx;
      rsp->rsp.rsp64.nRetVal = nRetVal;
      rsp->size = sizeof(rsp->rsp.rsp64);
   }
}

static __inline struct smq_invoke_buf* to_smq_invoke_buf_start(uint32 mode, void* virt, uint32 sc) {
   struct smq_invoke_buf* buf;
   int len = REMOTE_SCALARS_LENGTH(sc);
   if(0 == mode) {
      remote_arg* pra = (remote_arg*)virt;
      buf = (struct smq_invoke_buf*)(&pra[len]);
   } else {
      remote_arg64* pra = (remote_arg64*)virt;
      buf = (struct smq_invoke_buf*)(&pra[len]);
   }
   return buf;
}

static __inline struct smq_invoke_buf* smq_invoke_buf_start(remote_arg64 *pra, uint32 sc) {
   int len = REMOTE_SCALARS_LENGTH(sc);
   return (struct smq_invoke_buf*)(&pra[len]);
}

static __inline struct smq_phy_page* smq_phy_page_start(uint32 sc, struct smq_invoke_buf* buf) {
   int nTotal =  REMOTE_SCALARS_INBUFS(sc) + REMOTE_SCALARS_OUTBUFS(sc);
   return (struct smq_phy_page*)(&buf[nTotal]);
}

//! size of the out of band data
static __inline int smq_data_size(uint32 sc, int nPages) {
   struct smq_invoke_buf* buf = smq_invoke_buf_start(0, sc);
   struct smq_phy_page* page = smq_phy_page_start(sc, buf);
   return (int)(uintptr_t)(&(page[nPages]));
}

static __inline void to_smq_data(uint32 mode, uint32 sc, int nPages, void* pv, remote_arg64* rpra) {
   if(0 == mode) {
      struct smq_phy_page* page;
      struct smq_phy_page32* page32;
      remote_arg *pra = (remote_arg*)pv;
      int ii, len;
      len = REMOTE_SCALARS_LENGTH(sc);
      for(ii = 0; ii < len; ++ii) {
         rpra[ii].buf.pv = (uint64)(uintptr_t)pra[ii].buf.pv;
         rpra[ii].buf.nLen = pra[ii].buf.nLen;
      }
      len = REMOTE_SCALARS_INBUFS(sc) + REMOTE_SCALARS_OUTBUFS(sc);
      std_memmove(&rpra[ii], &pra[ii], len * sizeof(struct smq_invoke_buf));
      page = (struct smq_phy_page*)((struct smq_invoke_buf*)&rpra[ii] + len);
      page32 = (struct smq_phy_page32*)((struct smq_invoke_buf*)&pra[ii] + len);
      for(ii = 0; ii < nPages; ++ii) {
         page[ii].addr = page32[ii].addr;
         page[ii].size = page32[ii].size;
      }
   } else {
      std_memmove(rpra, pv, smq_data_size(sc, nPages));
   }
}

/*
 * API to get the DSP_SEARCH_PATH stored locally as static.
 * @get_path : get_path will be updated with the path stored in DSP_SEARCH_PATH locally.
 */
void get_default_dsp_search_path(char* path);

/*
 * API to map memory to the remote domain
 * @ fd: fd associated with this memory
 * @ flags: flags to be used for the mapping
 * @ vaddrin: input address
 * @ size: size of buffer
 * @ vaddrout: output address
 * returns 0 on success
 *
 */
int remote_mmap64_internal(int fd, uint32_t flags, uint64_t vaddrin, int64_t size, uint64_t* vaddrout);

/**
 *  remote_get_info API to get DSP/Kernel capibility
 *
 * @param[in] domain: DSP domain
 * @param[in] attributeID: One of the DSP/kernel attributes from enum fastrpc_internal_attributes.
 * @param[out] capability: Result of the DSP/kernel capability.
 * @return Integer value. Zero for success and non-zero for failure.
*/
int fastrpc_get_cap(uint32_t domain, uint32_t attributeID, uint32_t *capability);

/*
 * Check whether error is because of fastrpc.
 * Either kernel driver or dsp.
 * @err: error code to be checked.
 * returns 0 on success i.e. rpc error.
 *
 */
int check_rpc_error(int err);

/**
 * Notify the FastRPC QoS logic of activity outside of the invoke code path
 * that should still be considered in QoS timeout calculations.
 * Note that the function will silently fail on errors such as the domain
 * not having a valid QoS mode.
 *
 * @param[in] domain DSP domain
 */
void fastrpc_qos_activity(int domain);

/**
 * Make IOCTL call to exit async thread
 */
int fastrpc_exit_async_thread(int domain);

/**
 * Make IOCTL call to exit notif thread
 */
int fastrpc_exit_notif_thread(int domain);

// Kernel errno start
#define MIN_KERNEL_ERRNO 0
// Kernel errno end
#define MAX_KERNEL_ERRNO 135

/*
 * Convert kernel to user error.
 * @nErr: Error from ioctl
 * @err_no: errno from kernel
 * returns user error
 */

static __inline int convert_kernel_to_user_error(int nErr, int err_no) {
	if (!(nErr == AEE_EUNKNOWN && err_no && (err_no >= MIN_KERNEL_ERRNO && err_no <= MAX_KERNEL_ERRNO))) {
		return nErr;
	}

	switch (err_no) {
	case EIO:  /* EIO 5 I/O error */
	case ETOOMANYREFS: /* ETOOMANYREFS 109 Too many references: cannot splice */
	case EADDRNOTAVAIL: /* EADDRNOTAVAIL 99 Cannot assign requested address */
	case ENOTTY: /* ENOTTY 25 Not a typewriter */
	case EBADRQC: /* EBADRQC 56 Invalid request code */
		nErr = AEE_ERPC;
		break;
	case EFAULT: /* EFAULT 14 Bad address */
	case ECHRNG: /* ECHRNG 44 Channel number out of range */
	case EBADFD: /* EBADFD 77 File descriptor in bad state */
	case EINVAL: /* EINVAL 22 Invalid argument */
	case EBADF: /* EBADF 9 Bad file number */
	case EBADE: /* EBADE 52 Invalid exchange */
	case EBADR: /* EBADR 53 Invalid request descriptor */
	case EOVERFLOW: /* EOVERFLOW 75 Value too large for defined data type */
	case EHOSTDOWN: /* EHOSTDOWN 112 Host is down */
	case EEXIST: /* EEXIST 17 File exists */
	case EBADMSG: /* EBADMSG 74 Not a data message */
		nErr = AEE_EBADPARM;
		break;
	case ENXIO: /* ENXIO 6 No such device or address */
	case ENODEV: /* ENODEV 19 No such device*/
	case ENOKEY: /* ENOKEY 126 Required key not available */
		nErr = AEE_ENOSUCHDEVICE;
		break;
	case ENOBUFS: /* ENOBUFS 105 No buffer space available */
	case ENOMEM: /* ENOMEM 12 Out of memory */
		nErr = AEE_ENOMEMORY;
		break;
	case ENOSR: /* ENOSR 63 Out of streams resources */
	case EDQUOT: /* EDQUOT 122 Quota exceeded */
	case ETIMEDOUT: /* ETIMEDOUT 110 Connection timed out */
	case EUSERS: /* EUSERS 87 Too many users */
	case ESHUTDOWN: /* ESHUTDOWN 108 Cannot send after transport endpoint shutdown */
		nErr = AEE_EEXPIRED;
		break;
	case ENOTCONN:  /* ENOTCONN 107 Transport endpoint is not connected */
	case ECONNREFUSED: /* ECONNREFUSED 111 Connection refused */
		nErr = AEE_ECONNREFUSED;
		break;
	case ECONNRESET: /* ECONNRESET 104 Connection reset by peer */
	case EPIPE: /* EPIPE 32 Broken pipe */
		nErr = AEE_ECONNRESET;
		break;
	case EPROTONOSUPPORT: /* EPROTONOSUPPORT 93 Protocol not supported */
		nErr = AEE_EUNSUPPORTED;
		break;
	case EFBIG: /* EFBIG 27 File too large */
		nErr = AEE_EFILE;
		break;
	case EACCES: /* EACCES 13 Permission denied */
	case EPERM: /* EPERM 1 Operation not permitted */
		nErr = AEE_EBADPERMS;
		break;
	default:
		nErr = AEE_ERPC;
		break;
	}

	return nErr;
}

// Warning message to use domains
#define PRINT_WARN_USE_DOMAINS() VERIFY_WPRINTF("Warning: %s: Non-domain usage of FastRPC will be deprecated, use domains to offload to DSP using FastRPC", __func__)

int get_domain_from_name(const char *uri, uint32_t type);
int get_domain_from_handle(remote_handle64 local, int *domain);
int free_handle(remote_handle64 local);

remote_handle64 get_adsp_current_process1_handle(int domain);
remote_handle64 get_adspmsgd_adsp1_handle(int domain);
remote_handle64 get_adsp_listener1_handle(int domain);
remote_handle64 get_remotectl1_handle(int domain);
remote_handle64 get_adsp_perf1_handle(int domain);
/*
 * API to update non-domain and reverse handles list
 * @handle: handle to prepend or dequeue
 * @req: request id from enum handle_list_update_id
 * @domain: domain id
 * Prepend and dequeue operations can be performed on reverse and non-domain handle list
 * @returns: 0 on success, valid non-zero error code on failure
 *
 */
int fastrpc_update_module_list(uint32_t req, int domain, remote_handle64 handle, remote_handle64 *local);

/*
 * functions to wrap ioctl syscalls for downstream and upstream kernel
 */
int ioctl_init(int dev, uint32_t flags, int attr, byte* shell, int shelllen, int shellfd, char* initmem, int initmemlen, int initmemfd, int tessiglen);
int ioctl_invoke(int dev, int req, remote_handle handle, uint32_t sc, void* pra, int* fds, unsigned int* attrs, void *job, unsigned int* crc, uint64_t* perf_kernel, uint64_t* perf_dsp);
int ioctl_invoke2_response(int dev, fastrpc_async_jobid *jobid, remote_handle *handle, uint32_t *sc, int* result, uint64_t *perf_kernel, uint64_t *perf_dsp);
int ioctl_invoke2_notif(int dev, int *domain, int *session, int *status);
int ioctl_mmap(int dev, int req, uint32_t flags, int attr, int fd, int offset, size_t len, uintptr_t vaddrin, uint64_t* vaddr_out);
int ioctl_munmap(int dev, int req, int attr, void* buf, int fd, int len, uint64_t vaddr);
int ioctl_getperf(int dev, int keys, void *data, int *datalen);
int ioctl_getinfo(int dev, uint32_t *info);
int ioctl_getdspinfo(int dev, int domain, uint32_t attr_id, uint32_t *cap);
int ioctl_setmode(int dev, int mode);
int ioctl_control(int dev, int ioctltype, void* ctrl);
int ioctl_signal_create(int dev, uint32_t signal, uint32_t flags);
int ioctl_signal_destroy(int dev, uint32_t signal);
int ioctl_signal_signal(int dev, uint32_t signal);
int ioctl_signal_wait(int dev, uint32_t signal, uint32_t timeout_usec);
int ioctl_signal_cancel_wait(int dev, uint32_t signal);
int ioctl_sharedbuf(int dev, struct fastrpc_proc_sharedbuf_info *sharedbuf_info);
int ioctl_session_info(int dev, struct fastrpc_proc_sess_info *sess_info);
int ioctl_optimization(int dev, uint32_t max_concurrency);
const char* get_secure_domain_name(int domain_id);
int is_async_fastrpc_supported(void);

#include "fastrpc_ioctl.h"

#endif // FASTRPC_INTERNAL_H
