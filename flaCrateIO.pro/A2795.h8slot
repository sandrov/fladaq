#ifndef _A2795_H
#define _A2795_H

// ###########################################################################
// A2795 Register Map
// ###########################################################################
#define A_OutputBuffer              0x0000
#define A_FWRevision                0x1000
#define A_ControlReg                0x1004
#define A_ControlReg_Set            0x1008
#define A_ControlReg_Clear          0x100C
#define A_ControlReg2               0x1010
#define A_DebugReg                  0x1014
#define A_StatusReg                 0x1018
#define A_ChDisableMask             0x1024
#define A_Signals                   0x1028
#define A_NevStored                 0x102C
#define A_Timestamp                 0x1030
#define A_DAC                       0x1034
#define A_Temperature               0x1108
#define A_RELE                      0x1200
#define A_MemTest_cmd               0x1300
#define A_MemTest_opc               0x1304
#define A_MemTest_data_l            0x1308
#define A_MemTest_data_h            0x130C

// Mask definition for the Control Register
#define CTRL_BUFFORG                0x0000000F
#define CTRL_ACQRUN                 0x00000010
#define CTRL_TTLINK_MODE            0x00000020
#define CTRL_INHIBIT_TRG            0x00000080
#define CTRL_TEST_PATTERN           0x00000100
#define CTRL_SRAM_TEST              0x00000200
#define CTRL_TEST_MODE              0x10000000
#define CTRL_TEST_EVENT             0x80000000

// Mask definition for the Status Register
#define STAT_SLOT_ID                0x00000007
#define STAT_RUNNING                0x00000008
#define STAT_DRDY                   0x00000010
#define STAT_LBUSY                  0x00000020
#define STAT_GBUSY                  0x00000040

// Mask definition for the Signals Register
#define SIGNALS_SWRESET             0x00000001
#define SIGNALS_SWCLEAR             0x00000002
#define SIGNALS_SWGTRIGGER          0x00000004
#define SIGNALS_TTLINK_SOR          0x00010000
#define SIGNALS_TTLINK_EOR          0x00020000
#define SIGNALS_TTLINK_GTRG         0x00040000
#define SIGNALS_TTLINK_ALIGN        0x00080000
#define SIGNALS_TTLINK_COMMA        0x00100000

// Mask definition for the relé Register (Test Pulses)
#define RELE_TP_DIS                 0x00030009
#define RELE_TP_EXT                 0x00030006
#define RELE_TP_INT                 0x00030005
#define RELE_TP_ODD                 0x00050030
#define RELE_TP_EVEN                0x00060030


#endif

