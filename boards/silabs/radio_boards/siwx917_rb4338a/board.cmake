# Copyright (c) 2024 Silicon Laboratories Inc.
# SPDX-License-Identifier: Apache-2.0

board_runner_args(silabs_commander "--device=SiWG917M111GTBA" "--file-type=bin"
    "--file=${PROJECT_BINARY_DIR}/${KERNEL_BIN_NAME}.rps")
include(${ZEPHYR_BASE}/boards/common/silabs_commander.board.cmake)

# It is not possible to load/flash a firmware using JLink, but it is possible to
# debug a firmware with:
#    west attach -r jlink
# Once started, it should be possible to reset the device with "monitor reset"
board_runner_args(jlink "--device=Si917" "--speed=10000")
include(${ZEPHYR_BASE}/boards/common/jlink.board.cmake)

# It is possible debug a firmware with:
#    west attach -r openocd
# (west debug -r openocd --no-load will do almost the same)
#
# This experimental configuration does not implement flash, reset or breakpoints.
# It is possible to dump memory and use hardware breakpoints (hbreak)
include(${ZEPHYR_BASE}/boards/common/openocd.board.cmake)

