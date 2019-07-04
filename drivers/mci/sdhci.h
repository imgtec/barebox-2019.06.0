#ifndef __MCI_SDHCI_H
#define __MCI_SDHCI_H

#define SDHCI_DMA_ADDRESS					0x00
#define SDHCI_BLOCK_SIZE__BLOCK_COUNT				0x04
#define SDHCI_BLOCK_SIZE					0x04
#define  SDHCI_DMA_BOUNDARY_512K		SDHCI_DMA_BOUNDARY(7)
#define  SDHCI_DMA_BOUNDARY_256K		SDHCI_DMA_BOUNDARY(6)
#define  SDHCI_DMA_BOUNDARY_128K		SDHCI_DMA_BOUNDARY(5)
#define  SDHCI_DMA_BOUNDARY_64K			SDHCI_DMA_BOUNDARY(4)
#define  SDHCI_DMA_BOUNDARY_32K			SDHCI_DMA_BOUNDARY(3)
#define  SDHCI_DMA_BOUNDARY_16K			SDHCI_DMA_BOUNDARY(2)
#define  SDHCI_DMA_BOUNDARY_8K			SDHCI_DMA_BOUNDARY(1)
#define  SDHCI_DMA_BOUNDARY_4K			SDHCI_DMA_BOUNDARY(0)
#define  SDHCI_DMA_BOUNDARY(x)			(((x) & 0x7) << 12)
#define  SDHCI_TRANSFER_BLOCK_SIZE(x)		((x) & 0xfff)
#define SDHCI_BLOCK_COUNT					0x06
#define SDHCI_ARGUMENT						0x08
#define SDHCI_TRANSFER_MODE__COMMAND				0x0c
#define SDHCI_TRANSFER_MODE					0x0c
#define  SDHCI_MULTIPLE_BLOCKS			BIT(5)
#define  SDHCI_DATA_TO_HOST			BIT(4)
#define  SDHCI_BLOCK_COUNT_EN			BIT(1)
#define  SDHCI_DMA_EN				BIT(0)
#define SDHCI_COMMAND						0x0e
#define  SDHCI_CMD_INDEX(c)			(((c) & 0x3f) << 8)
#define  SDHCI_DATA_PRESENT			BIT(5)
#define  SDHCI_CMD_INDEX_CHECK_EN		BIT(4)
#define  SDHCI_CMD_CRC_CHECK_EN			BIT(3)
#define  SDHCI_RESP_TYPE_48_BUSY		3
#define  SDHCI_RESP_TYPE_48			2
#define  SDHCI_RESP_TYPE_136			1
#define  SDHCI_RESP_NONE			0
#define SDHCI_RESPONSE_0					0x10
#define SDHCI_RESPONSE_1					0x14
#define SDHCI_RESPONSE_2					0x18
#define SDHCI_RESPONSE_3					0x1c
#define SDHCI_BUFFER						0x20
#define SDHCI_PRESENT_STATE					0x24
#define  SDHCI_CMD_INHIBIT_DATA			BIT(1)
#define  SDHCI_CMD_INHIBIT_CMD			BIT(0)
#define SDHCI_PRESENT_STATE1					0x26
#define SDHCI_HOST_CONTROL__POWER_CONTROL__BLOCK_GAP_CONTROL	0x28
#define SDHCI_HOST_CONTROL					0x28
#define  SDHCI_DATA_WIDTH_8BIT			BIT(5)
#define  SDHCI_HIGHSPEED_EN			BIT(2)
#define  SDHCI_DATA_WIDTH_4BIT			BIT(1)
#define SDHCI_POWER_CONTROL					0x29
#define  SDHCI_BUS_VOLTAGE_330			SDHCI_BUS_VOLTAGE(7)
#define  SDHCI_BUS_VOLTAGE(v)			((v) << 1)
#define  SDHCI_BUS_POWER_EN			BIT(0)
#define SDHCI_CLOCK_CONTROL__TIMEOUT_CONTROL__SOFTWARE_RESET	0x2c
#define SDHCI_CLOCK_CONTROL					0x2c
#define  SDHCI_FREQ_SEL(x)			(((x) & 0xff) << 8)
#define  SDHCI_SDCLOCK_EN			BIT(2)
#define  SDHCI_INTCLOCK_STABLE			BIT(1)
#define  SDHCI_INTCLOCK_EN			BIT(0)
#define SDHCI_TIMEOUT_CONTROL					0x2e
#define SDHCI_SOFTWARE_RESET					0x2f
#define  SDHCI_RESET_ALL			BIT(0)
#define SDHCI_INT_STATUS					0x30
#define SDHCI_INT_NORMAL_STATUS					0x30
#define  SDHCI_INT_ERROR			BIT(15)
#define  SDHCI_INT_DMA				BIT(3)
#define  SDHCI_INT_XFER_COMPLETE		BIT(1)
#define  SDHCI_INT_CMD_COMPLETE			BIT(0)
#define SDHCI_INT_ERROR_STATUS					0x32
#define SDHCI_INT_ENABLE					0x34
#define SDHCI_SIGNAL_ENABLE					0x38
#define SDHCI_ACMD12_ERR__HOST_CONTROL2				0x3C
#define SDHCI_CAPABILITIES					0x40
#define SDHCI_CAPABILITIES_1					0x42
#define  SDHCI_HOSTCAP_VOLTAGE_180		BIT(10)
#define  SDHCI_HOSTCAP_VOLTAGE_300		BIT(9)
#define  SDHCI_HOSTCAP_VOLTAGE_330		BIT(8)
#define  SDHCI_HOSTCAP_HIGHSPEED		BIT(5)
#define  SDHCI_HOSTCAP_8BIT			BIT(2)

#define SDHCI_SPEC_200_MAX_CLK_DIVIDER	256
#define SDHCI_MMC_BOOT						0xC4

#define COMMAND_CMD(x)		((x & 0x3f) << 24)
#define COMMAND_CMDTYP_NORMAL	0x0
#define COMMAND_CMDTYP_SUSPEND	0x00400000
#define COMMAND_CMDTYP_RESUME	0x00800000
#define COMMAND_CMDTYP_ABORT	0x00c00000
#define COMMAND_DPSEL		0x00200000
#define COMMAND_CICEN		0x00100000
#define COMMAND_CCCEN		0x00080000
#define COMMAND_RSPTYP_NONE	0
#define COMMAND_RSPTYP_136	0x00010000
#define COMMAND_RSPTYP_48	0x00020000
#define COMMAND_RSPTYP_48_BUSY	0x00030000
#define TRANSFER_MODE_MSBSEL	0x00000020
#define TRANSFER_MODE_DTDSEL	0x00000010
#define TRANSFER_MODE_AC12EN	0x00000004
#define TRANSFER_MODE_BCEN	0x00000002
#define TRANSFER_MODE_DMAEN	0x00000001

#define IRQSTAT_TE		0x04000000
#define IRQSTAT_DMAE		0x02000000
#define IRQSTAT_AC12E		0x01000000
#define IRQSTAT_CLE		0x00800000
#define IRQSTAT_DEBE		0x00400000
#define IRQSTAT_DCE		0x00200000
#define IRQSTAT_DTOE		0x00100000
#define IRQSTAT_CIE		0x00080000
#define IRQSTAT_CEBE		0x00040000
#define IRQSTAT_CCE		0x00020000
#define IRQSTAT_CTOE		0x00010000
#define IRQSTAT_CINT		0x00000100
#define IRQSTAT_CRM		0x00000080
#define IRQSTAT_CINS		0x00000040
#define IRQSTAT_BRR		0x00000020
#define IRQSTAT_BWR		0x00000010
#define IRQSTAT_DINT		0x00000008
#define IRQSTAT_BGE		0x00000004
#define IRQSTAT_TC		0x00000002
#define IRQSTAT_CC		0x00000001

#define IRQSTATEN_DMAE		0x10000000
#define IRQSTATEN_AC12E		0x01000000
#define IRQSTATEN_DEBE		0x00400000
#define IRQSTATEN_DCE		0x00200000
#define IRQSTATEN_DTOE		0x00100000
#define IRQSTATEN_CIE		0x00080000
#define IRQSTATEN_CEBE		0x00040000
#define IRQSTATEN_CCE		0x00020000
#define IRQSTATEN_CTOE		0x00010000
#define IRQSTATEN_CINT		0x00000100
#define IRQSTATEN_CRM		0x00000080
#define IRQSTATEN_CINS		0x00000040
#define IRQSTATEN_BRR		0x00000020
#define IRQSTATEN_BWR		0x00000010
#define IRQSTATEN_DINT		0x00000008
#define IRQSTATEN_BGE		0x00000004
#define IRQSTATEN_TC		0x00000002
#define IRQSTATEN_CC		0x00000001

#define PRSSTAT_DAT0		0x01000000
#define PRSSTAT_CLSL		0x00800000
#define PRSSTAT_WPSPL		0x00080000
#define PRSSTAT_CDPL		0x00040000
#define PRSSTAT_CINS		0x00010000
#define PRSSTAT_BREN		0x00000800
#define PRSSTAT_BWEN		0x00000400
#define PRSSTAT_SDSTB		0x00000008
#define PRSSTAT_DLA		0x00000004
#define PRSSTAT_CIDHB		0x00000002
#define PRSSTAT_CICHB		0x00000001

#endif /* __MCI_SDHCI_H */