#! /vendor/bin/sh
#Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.  
#SPDX-License-Identifier: BSD-3-Clause-Clear

# SoftSKU check to disable cdsprpcd for Shikra FP1 license
target=`getprop ro.board.platform`
case "$target" in
    "shikra")
        SOFTSKU_PATH="/proc/device-tree/qcom,softsku-id"
        if [ -f "$SOFTSKU_PATH" ]; then
            SKU_ID=$(od -An -t x1 "$SOFTSKU_PATH" | tr -d '[:space:]')
            if [ "$SKU_ID" = "00000001" ]; then
                setprop vendor.fastrpc.disable.cdsprpcd.daemon 1
            fi
        fi
        ;;
esac