/*
 * Copyright (c) 2024 Silicon Laboratories Inc.
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/kernel.h>
#include "zephyr/device.h"
#include "rsi_qspi.h"
#include "rsi_qspi_proto.h"
#include "sl_si91x_psram.h"
#include "si91x_psram.h"

#define QSPI_SIWX917_NODE       DT_NODELABEL(qspi0)
#define PSRAM_SIWX917_NODE      DT_NODELABEL(psram)

sl_psram_info_type_t PSRAM_Device = {
	.devDensity             = DT_PROP(PSRAM_SIWX917_NODE, device_density),
	.deviceName             = DT_PROP(PSRAM_SIWX917_NODE, device_name),
	.normalReadMAXFrequency = DT_PROP(PSRAM_SIWX917_NODE, normal_read_speed),
	.fastReadMAXFrequency   = DT_PROP(PSRAM_SIWX917_NODE, fast_read_speed),
	.rwType                 = DT_PROP(PSRAM_SIWX917_NODE, rw_type),
	.defaultBurstWrapSize   = DT_PROP(PSRAM_SIWX917_NODE, default_burst_wrap_size),
	.toggleBurstWrapSize    = DT_PROP(PSRAM_SIWX917_NODE, toggle_burst_wrap_size),
	.spi_config = {
		.spi_config_1 = {
			.inst_mode         = DT_PROP(QSPI_SIWX917_NODE, spi_config_1_insta_mode),
			.addr_mode         = DT_PROP(QSPI_SIWX917_NODE, spi_config_1_addr_mode),
			.dummy_mode        = DT_PROP(QSPI_SIWX917_NODE, spi_config_1_dummy_mode),
			.data_mode         = DT_PROP(QSPI_SIWX917_NODE, spi_config_1_data_mode),
			.extra_byte_mode   = DT_PROP(QSPI_SIWX917_NODE, spi_config_1_extra_byte_mode),
			.prefetch_en       = DIS_PREFETCH,
			.dummy_W_or_R      = DUMMY_READS,
			.d3d2_data         = 3,
			.continuous        = DIS_CONTINUOUS,
			.read_cmd          = DT_PROP(QSPI_SIWX917_NODE, spi_config_1_read_cmd_code),
			.flash_type        = 0xf,
			.no_of_dummy_bytes = DT_PROP(QSPI_SIWX917_NODE, spi_config_1_no_of_dummy_bytes),
			.extra_byte_en     = 0,
		},
		.spi_config_2 = {
			.auto_mode                   = EN_AUTO_MODE,
			.cs_no                       = PSRAM_CHIP_SELECT,
			.neg_edge_sampling           = NEG_EDGE_SAMPLING,
			.qspi_clk_en                 = QSPI_FULL_TIME_CLK,
			.protection                  = DNT_REM_WR_PROT,
			.dma_mode                    = NO_DMA,
			.swap_en                     = SWAP,
			.full_duplex                 = IGNORE_FULL_DUPLEX,
			.wrap_len_in_bytes           = NO_WRAP,
			.addr_width_valid            = 0,
			.addr_width                  = _24BIT_ADDR,
			.pinset_valid                = 0,
			.dummy_cycles_for_controller = 0,
		},
		.spi_config_3 = {
			.xip_mode          = 0,
			._16bit_cmd_valid  = 0,
			._16bit_rd_cmd_msb = 0,
			.reserved          = 0,
			.wr_cmd            = DT_PROP(QSPI_SIWX917_NODE, spi_config_3_write_cmd),
			.wr_inst_mode      = DT_PROP(QSPI_SIWX917_NODE, spi_config_3_write_inst_mode),
			.wr_addr_mode      = DT_PROP(QSPI_SIWX917_NODE, spi_config_3_write_addr_mode),
			.wr_data_mode      = DT_PROP(QSPI_SIWX917_NODE, spi_config_3_write_data_mode),
			.dummys_4_jump     = 1,
		},
		.spi_config_4 = {
			._16bit_wr_cmd_msb    = 0,
			.dual_flash_mode      = 0,
			.secondary_csn        = 1,
			.polarity_mode        = 0,
			.valid_prot_bits      = 4,
			.no_of_ms_dummy_bytes = 0,
			.continue_fetch_en    = 0,
		},
		.spi_config_5 = {
			.busy_bit_pos         = 0,
			.d7_d4_data           = 0xf,
			.dummy_bytes_for_rdsr = 0x0,
			.reset_type           = 0x0,
		}
	}
};

PSRAMSecureSegmentType PSRAMSecureSegments[MAX_SEC_SEGMENTS] = {
	{ .segmentEnable = 1, .lowerBoundary = 0x00000, .higherBoundary = 0x0ffff },
	{ .segmentEnable = 0, .lowerBoundary = 0x00000, .higherBoundary = 0x00000 },
	{ .segmentEnable = 0, .lowerBoundary = 0xf0001, .higherBoundary = 0xfffff },
	{ .segmentEnable = 0, .lowerBoundary = 0x00000, .higherBoundary = 0x00000 },
};

int silabs_siwg917_psram_init(void)
{
	sl_psram_return_type_t status;
	uint8_t device_id[8] = DT_PROP(PSRAM_SIWX917_NODE, device_id);

	PSRAM_Device.deviceID.MFID = device_id[0];
	PSRAM_Device.deviceID.KGD = device_id[1];
	memcpy(PSRAM_Device.deviceID.EID, device_id + 2, 6);
	status = sl_si91x_psram_uninit();
	__ASSERT(!status, "sl_si91x_psram_uninit()\n");
	status = sl_si91x_psram_init();
	__ASSERT(!status, "sl_si91x_psram_init()\n");
	return 0;
}

