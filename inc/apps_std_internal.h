// Copyright (c) 2024, Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef __APPS_STD_INTERNAL_H__
#define __APPS_STD_INTERNAL_H__

#include "apps_std.h"

/*
 * Macros defined for environment search paths
 *
 */
#define RETRY_WRITE (3) // number of times to retry write operation

#define ADSP_LIBRARY_PATH "ADSP_LIBRARY_PATH"
#define DSP_LIBRARY_PATH "DSP_LIBRARY_PATH"
#define ADSP_AVS_PATH "ADSP_AVS_CFG_PATH"

#ifndef ENABLE_UPSTREAM_DRIVER_INTERFACE
#ifndef DSP_MOUNT_LOCATION
#define DSP_MOUNT_LOCATION "/dsp/"
#endif
#ifndef DSP_DOM_LOCATION
#define DSP_DOM_LOCATION "/dsp/xdsp"
#endif
#else
#ifndef DSP_MOUNT_LOCATION
#define DSP_MOUNT_LOCATION "/usr/lib/dsp/"
#endif
#ifndef DSP_DOM_LOCATION
#define DSP_DOM_LOCATION "/usr/lib/dsp/xdsp"
#endif
#endif
#ifndef VENDOR_DSP_LOCATION
#define VENDOR_DSP_LOCATION "/vendor/dsp/"
#endif
#ifndef VENDOR_DOM_LOCATION
#define VENDOR_DOM_LOCATION "/vendor/dsp/xdsp/"
#endif

#ifndef DSP_SEARCH_PATH
#define DSP_SEARCH_PATH ";/usr/lib/rfsa/adsp;/dsp;/usr/lib/dsp/;"
#endif
#ifndef ADSP_AVS_CFG_PATH
#define ADSP_AVS_CFG_PATH ";/etc/acdbdata/;"
#endif

#endif /*__APPS_STD_INTERNAL_H__*/
