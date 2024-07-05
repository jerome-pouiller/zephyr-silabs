# Copyright (c) 2024 Silicon Laboratories Inc.
# SPDX-License-Identifier: Apache-2.0

zephyr_library_sources_ifdef(CONFIG_DT_HAS_SILABS_SIWX917_PSRAM_ENABLED memc_silabs_siwx917_psram.c)
