// Copyright (c) 2024, Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause 

/* File only compiled  when support to upstream kernel is required*/

#include "AEEStdErr.h"
#include "HAP_farf.h"
#include "fastrpc_async.h"
#include "fastrpc_internal.h"
#include "fastrpc_notif.h"
#include "remote.h"
#include <sys/ioctl.h>

/* check async support */
int is_async_fastrpc_supported(void) {
  /* async not supported by upstream driver */
  return 0;
}

/* Returns the name of the domain based on the following
 ADSP/SLPI/MDSP/CDSP - Return Secure node
 */
const char *get_secure_domain_name(int domain_id) {
  const char *name;
  int domain = domain_id & DOMAIN_ID_MASK;

  switch (domain) {
  case ADSP_DOMAIN_ID:
    name = ADSPRPC_SECURE_DEVICE;
    break;
  case SDSP_DOMAIN_ID:
    name = SDSPRPC_SECURE_DEVICE;
    break;
  case MDSP_DOMAIN_ID:
    name = MDSPRPC_SECURE_DEVICE;
    break;
  case CDSP_DOMAIN_ID:
    name = CDSPRPC_SECURE_DEVICE;
    break;
  default:
    name = DEFAULT_DEVICE;
    break;
  }
  return name;
}

int ioctl_init(int dev, uint32_t flags, int attr, byte *shell, int shelllen,
               int shellfd, char *mem, int memlen, int memfd, int tessiglen) {
  int ioErr = 0;
  struct fastrpc_ioctl_init_create init = {0};
  struct fastrpc_ioctl_init_create_static init_static = {0};

  switch (flags) {
  case FASTRPC_INIT_ATTACH:
    ioErr = ioctl(dev, FASTRPC_IOCTL_INIT_ATTACH, NULL);
    break;
  case FASTRPC_INIT_ATTACH_SENSORS:
    ioErr = ioctl(dev, FASTRPC_IOCTL_INIT_ATTACH_SNS, NULL);
    break;
  case FASTRPC_INIT_CREATE_STATIC:
    init_static.namelen = shelllen;
    init_static.memlen = memlen;
    init_static.name = (uint64_t)shell;
    ioErr = ioctl(dev, FASTRPC_IOCTL_INIT_CREATE_STATIC,
                  (unsigned long)&init_static);
    break;
  case FASTRPC_INIT_CREATE:
    init.file = (uint64_t)shell;
    init.filelen = shelllen;
    init.filefd = shellfd;
    init.attrs = attr;
    init.siglen = tessiglen;
    ioErr = ioctl(dev, FASTRPC_IOCTL_INIT_CREATE, (unsigned long)&init);
    break;
  default:
    FARF(ERROR, "ERROR: %s Invalid init flags passed %d", __func__, flags);
    ioErr = AEE_EBADPARM;
    break;
  }

  return ioErr;
}

int ioctl_invoke(int dev, int req, remote_handle handle, uint32_t sc, void *pra,
                 int *fds, unsigned int *attrs, void *job, unsigned int *crc,
                 uint64_t *perf_kernel, uint64_t *perf_dsp) {
  int ioErr = AEE_SUCCESS;
  struct fastrpc_ioctl_invoke invoke = {0};
  struct fastrpc_enhanced_invoke invoke2 = {0};
  struct fastrpc_ioctl_multimode_invoke ioctl_inv2 = {0};

  invoke.handle = handle;
  invoke.sc = sc;
  invoke.args = (uint64_t)pra;
  if (req >= INVOKE && req <= INVOKE_FD) {
    ioErr = ioctl(dev, FASTRPC_IOCTL_INVOKE, (unsigned long)&invoke);
  } else {
    invoke2.inv = invoke;
    invoke2.crc = (uint64_t)crc;
    invoke2.perf_kernel = (uint64_t)perf_kernel;
    invoke2.perf_dsp = (uint64_t)perf_dsp;
    ioctl_inv2.req = FASTRPC_INVOKE_ENHANCED_INVOKE;
    ioctl_inv2.invparam = (uintptr_t)&invoke2;
    ioctl_inv2.size = sizeof(struct fastrpc_enhanced_invoke);
    ioErr =
        ioctl(dev, FASTRPC_IOCTL_MULTIMODE_INVOKE, (unsigned long)&ioctl_inv2);
  }
  return ioErr;
}

int ioctl_invoke2_response(int dev, fastrpc_async_jobid *jobid,
                           remote_handle *handle, uint32_t *sc, int *result,
                           uint64_t *perf_kernel, uint64_t *perf_dsp) {
  return AEE_EUNSUPPORTED;
}

int ioctl_invoke2_notif(int dev, int *domain, int *session, int *status) {
  int ioErr = AEE_SUCCESS;
  struct fastrpc_ioctl_multimode_invoke invoke2 = {0};
  struct fastrpc_ioctl_notif_response notif = {0};

  invoke2.req = FASTRPC_INVOKE_NOTIF;
  invoke2.invparam = (uintptr_t)&notif;
  invoke2.size = sizeof(struct fastrpc_ioctl_notif_response);
  ioErr = ioctl(dev, FASTRPC_IOCTL_MULTIMODE_INVOKE, (unsigned long)&invoke2);
  *domain = notif.domain;
  *session = notif.session;
  *status = notif.status;

  return ioErr;
}

int ioctl_mmap(int dev, int req, uint32_t flags, int attr, int fd, int offset,
               size_t len, uintptr_t vaddrin, uint64_t *vaddrout) {
  int ioErr = AEE_SUCCESS;

  switch (req) {
  case MEM_MAP: {
    struct fastrpc_ioctl_mem_map map = {0};
    map.version = 0;
    map.fd = fd;
    map.offset = offset;
    map.flags = flags;
    map.vaddrin = (uint64_t)vaddrin;
    map.length = len;
    map.attrs = attr;
    ioErr = ioctl(dev, FASTRPC_IOCTL_MEM_MAP, (unsigned long)&map);
    *vaddrout = (uint64_t)map.vaddrout;
  } break;
  case MMAP:
  case MMAP_64: {
    struct fastrpc_ioctl_req_mmap map = {0};
    map.fd = fd;
    map.flags = flags;
    map.vaddrin = (uint64_t)vaddrin;
    map.size = len;
    ioErr = ioctl(dev, FASTRPC_IOCTL_MMAP, (unsigned long)&map);
    *vaddrout = (uint64_t)map.vaddrout;
  } break;
  default:
    FARF(ERROR, "ERROR: %s Invalid request passed %d", __func__, req);
    ioErr = AEE_EBADPARM;
    break;
  }
  return ioErr;
}

int ioctl_munmap(int dev, int req, int attr, void *buf, int fd, int len,
                 uint64_t vaddr) {
  int ioErr = AEE_SUCCESS;

  switch (req) {
  case MEM_UNMAP:
  case MUNMAP_FD: {
    struct fastrpc_ioctl_mem_unmap unmap = {0};
    unmap.version = 0;
    unmap.fd = fd;
    unmap.vaddr = vaddr;
    unmap.length = len;
    ioErr = ioctl(dev, FASTRPC_IOCTL_MEM_UNMAP, (unsigned long)&unmap);
  } break;
  case MUNMAP:
  case MUNMAP_64: {
    struct fastrpc_ioctl_req_munmap unmap = {0};
    unmap.vaddrout = vaddr;
    unmap.size = (ssize_t)len;
    ioErr = ioctl(dev, FASTRPC_IOCTL_MUNMAP, (unsigned long)&unmap);
  } break;
  default:
    FARF(ERROR, "ERROR: %s Invalid request passed %d", __func__, req);
    break;
  }

  return ioErr;
}

int ioctl_getinfo(int dev, uint32_t *info) {
  *info = 1;
  return AEE_SUCCESS;
}

int ioctl_getdspinfo(int dev, int domain, uint32_t attr, uint32_t *capability) {
  int ioErr = AEE_SUCCESS;
  static struct fastrpc_ioctl_capability cap = {0};

  if (attr == PERF_V2_DRIVER_SUPPORT) {
    *capability = 2;
    return 0;
  }
  if (attr > PERF_V2_DRIVER_SUPPORT && attr < FASTRPC_MAX_ATTRIBUTES) {
    *capability = 1;
    return 0;
  }

  cap.domain = domain;
  cap.attribute_id = attr;
  cap.capability = 0;
  ioErr = ioctl(dev, FASTRPC_IOCTL_GET_DSP_INFO, &cap);
  *capability = cap.capability;
  return ioErr;
}

int ioctl_setmode(int dev, int mode) {
  struct fastrpc_ioctl_multimode_invoke ioctl_invk2;

  if (mode == FASTRPC_SESSION_ID1) {
    ioctl_invk2.req = FASTRPC_INVOKE_MULTISESSION;
    ioctl_invk2.invparam = (__u64)(uintptr_t)&mode;
    ioctl_invk2.size = sizeof(uint32_t);
    return ioctl(dev, FASTRPC_IOCTL_MULTIMODE_INVOKE, &ioctl_invk2);
  }
  return AEE_EUNSUPPORTED;
}

int ioctl_control(int dev, int req, void *c) {
  int ioErr = AEE_SUCCESS;
  struct fastrpc_ioctl_control ctrl = {0};
  struct fastrpc_ctrl_kalloc *kalloc;
  struct fastrpc_ioctl_multimode_invoke ioctl_invk2;

  switch (req) {
  case DSPRPC_SET_MODE:
    FARF(ERROR, "Deprecated performance mode\n");
    ioErr = AEE_SUCCESS;
    return ioErr;
  case DSPRPC_CONTROL_LATENCY:
    ctrl.lp = *(struct fastrpc_ctrl_latency *)c;
    ctrl.req = FASTRPC_CONTROL_LATENCY;
    break;
  case DSPRPC_SMMU_SUPPORT:
    ctrl.smmu = *(struct fastrpc_ctrl_smmu *)c;
    ctrl.req = FASTRPC_CONTROL_SMMU;
    break;
  case DSPRPC_KALLOC_SUPPORT:
    kalloc = (struct fastrpc_ctrl_kalloc *)c;
    kalloc->kalloc_support = 1;
    return ioErr;
    break;
  case DSPRPC_CONTROL_WAKELOCK:
    ctrl.wl = *(struct fastrpc_ctrl_wakelock *)c;
    ctrl.req = FASTRPC_CONTROL_WAKELOCK;
    break;
  case DSPRPC_PM:
    ctrl.pm = *(struct fastrpc_ctrl_pm *)c;
    ctrl.req = FASTRPC_CONTROL_PM;
    break;
  case DSPRPC_RPC_POLL:
    ctrl.lp = *(struct fastrpc_ctrl_latency *)c;
    ctrl.req = FASTRPC_CONTROL_RPC_POLL;
    break;
  case DSPRPC_ASYNC_WAKE:
  case DSPRPC_NOTIF_WAKE:
    break;
  case DSPRPC_REMOTE_PROCESS_KILL:
    ctrl.req = FASTRPC_REMOTE_PROCESS_KILL;
    break;
  default:
    FARF(ERROR, "ERROR: %s Invalid request passed %d", __func__, req);
    ioErr = AEE_EBADPARM;
    break;
  }
  ioctl_invk2.req = FASTRPC_INVOKE_CONTROL;
  ioctl_invk2.invparam = (__u64)(uintptr_t)&ctrl;
  ioctl_invk2.size = sizeof(struct fastrpc_ioctl_control);
  return ioctl(dev, FASTRPC_IOCTL_MULTIMODE_INVOKE, &ioctl_invk2);
}

int ioctl_getperf(int dev, int key, void *data, int *datalen) {
  return AEE_EUNSUPPORTED;
}

int ioctl_signal_create(int dev, uint32_t signal, uint32_t flags) {

  struct fastrpc_ioctl_multimode_invoke ioctl_invk2;
  struct fastrpc_ioctl_signal fsig;

  fsig.req = FASTRPC_DSPSIGNAL_CREATE;
  fsig.signal_id = signal;
  fsig.flags = flags;

  ioctl_invk2.req = FASTRPC_INVOKE_DSPSIGNAL;
  ioctl_invk2.invparam = (__u64)(uintptr_t)&fsig;
  ioctl_invk2.size = sizeof(struct fastrpc_ioctl_signal);

  return ioctl(dev, FASTRPC_IOCTL_MULTIMODE_INVOKE, &ioctl_invk2);
}

int ioctl_signal_destroy(int dev, uint32_t signal) {

  struct fastrpc_ioctl_multimode_invoke ioctl_invk2;
  struct fastrpc_ioctl_signal fsig;

  fsig.req = FASTRPC_DSPSIGNAL_DESTROY;
  fsig.signal_id = signal;

  ioctl_invk2.req = FASTRPC_INVOKE_DSPSIGNAL;
  ioctl_invk2.invparam = (__u64)(uintptr_t)&fsig;
  ioctl_invk2.size = sizeof(struct fastrpc_ioctl_signal);

  return ioctl(dev, FASTRPC_IOCTL_MULTIMODE_INVOKE, &ioctl_invk2);
}

int ioctl_signal_signal(int dev, uint32_t signal) {

  struct fastrpc_ioctl_multimode_invoke ioctl_invk2;
  struct fastrpc_ioctl_signal fsig;

  fsig.req = FASTRPC_DSPSIGNAL_SIGNAL;
  fsig.signal_id = signal;

  ioctl_invk2.req = FASTRPC_INVOKE_DSPSIGNAL;
  ioctl_invk2.invparam = (__u64)(uintptr_t)&fsig;
  ioctl_invk2.size = sizeof(struct fastrpc_ioctl_signal);

  return ioctl(dev, FASTRPC_IOCTL_MULTIMODE_INVOKE, &ioctl_invk2);
}

int ioctl_signal_wait(int dev, uint32_t signal, uint32_t timeout_usec) {
  struct fastrpc_ioctl_multimode_invoke ioctl_invk2;
  struct fastrpc_ioctl_signal fsig;

  fsig.req = FASTRPC_DSPSIGNAL_WAIT;
  fsig.signal_id = signal;
  fsig.timeout_usec = timeout_usec;

  ioctl_invk2.req = FASTRPC_INVOKE_DSPSIGNAL;
  ioctl_invk2.invparam = (__u64)(uintptr_t)&fsig;
  ioctl_invk2.size = sizeof(struct fastrpc_ioctl_signal);

  return ioctl(dev, FASTRPC_IOCTL_MULTIMODE_INVOKE, &ioctl_invk2);
}

int ioctl_signal_cancel_wait(int dev, uint32_t signal) {

  struct fastrpc_ioctl_multimode_invoke ioctl_invk2;
  struct fastrpc_ioctl_signal fsig;

  fsig.req = FASTRPC_DSPSIGNAL_CANCEL_WAIT;
  fsig.signal_id = signal;

  ioctl_invk2.req = FASTRPC_INVOKE_DSPSIGNAL;
  ioctl_invk2.invparam = (__u64)(uintptr_t)&fsig;
  ioctl_invk2.size = sizeof(struct fastrpc_ioctl_signal);

  return ioctl(dev, FASTRPC_IOCTL_MULTIMODE_INVOKE, &ioctl_invk2);
}

int ioctl_sharedbuf(int dev,
                    struct fastrpc_proc_sharedbuf_info *sharedbuf_info) {
  struct fastrpc_ioctl_multimode_invoke invoke2 = {0};

  invoke2.req = FASTRPC_INVOKE_CONFIG;
  invoke2.invparam = (uintptr_t)sharedbuf_info;
  invoke2.size = sizeof(struct fastrpc_proc_sharedbuf_info);

  return ioctl(dev, FASTRPC_IOCTL_MULTIMODE_INVOKE, (unsigned long)&invoke2);
}

int ioctl_session_info(int dev, struct fastrpc_proc_sess_info *sess_info) {
  struct fastrpc_ioctl_multimode_invoke invoke2 = {0};

  invoke2.req = FASTRPC_INVOKE_SESSIONINFO;
  invoke2.invparam = (uintptr_t)sess_info;
  invoke2.size = sizeof(struct fastrpc_proc_sess_info);

  sess_info->domain_id = 1;
  return ioctl(dev, FASTRPC_IOCTL_MULTIMODE_INVOKE, (unsigned long)&invoke2);
}

int ioctl_optimization(int dev, uint32_t max_concurrency) {
  struct fastrpc_ioctl_multimode_invoke invoke2;

  std_memset(&invoke2, 0, sizeof(invoke2));
  invoke2.req = FASTRPC_INVOKE2_KERNEL_OPTIMIZATIONS;
  invoke2.invparam = (uintptr_t)&max_concurrency;
  invoke2.size = sizeof(uint32_t);
  return ioctl(dev, FASTRPC_IOCTL_MULTIMODE_INVOKE, (unsigned long)&invoke2);
}
