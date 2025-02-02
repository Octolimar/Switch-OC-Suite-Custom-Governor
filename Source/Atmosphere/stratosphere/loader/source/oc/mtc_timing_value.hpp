#pragma once

#include "oc_common.hpp"

namespace ams::ldr::oc {

    #define MAX(A, B)   std::max(A, B)
    #define MIN(A, B)   std::min(A, B)
    #define CEIL(A)     std::ceil(A)
    #define FLOOR(A)    std::floor(A)

    const std::array<u32, 6> tRCD_values = {18, 17, 16, 15, 14, 13};
    const std::array<u32, 6> tRP_values = {18, 17, 16, 15, 14, 13};
    const std::array<u32, 6> tRAS_values = {42, 39, 36, 34, 32, 30};

    const std::array<double, 5> tRRD_values = {10, 7.5, 6, 4, 2};
    const std::array<double, 5> tFAW_values = {40, 30, 24, 16, 8};

    const std::array<u32, 5> tWR_values = {18, 15, 15, 12, 8};
    const std::array<double, 5> tRTP_values = {7.5, 7.5, 6, 6, 4};

    const std::array<u32, 5> tRFC_values = {140, 120, 100, 80, 60};

    const std::array<u32, 5> tWTR_values = {10, 9, 8, 7, 6};

    const std::array<u32, 4> tREFpb_values = {488, 976, 1952, 3256};

    const std::array<u32, 6> tWL_values = {14, 12, 10, 8, 6, 4};

    const u32 TIMING_PRESET_ONE = C.ramTimingPresetOne;
    const u32 TIMING_PRESET_TWO = C.ramTimingPresetTwo;
    const u32 TIMING_PRESET_THREE = C.ramTimingPresetThree;
    const u32 TIMING_PRESET_FOUR = C.ramTimingPresetFour;
    const u32 TIMING_PRESET_FIVE = C.ramTimingPresetFive;
    const u32 TIMING_PRESET_SIX = C.ramTimingPresetSix;
    const u32 TIMING_PRESET_SEVEN = C.ramTimingPresetSeven;

    // tCK_avg (average clock period) in ns
    const double tCK_avg = 1000'000. / C.marikoEmcMaxClock;
 
    // tRPpb (row precharge time per bank) in ns
    const u32 tRPpb = !TIMING_PRESET_ONE ? 18 : tRP_values[TIMING_PRESET_ONE-1];
    // tRPab (row precharge time all banks) in ns
    const u32 tRPab = !TIMING_PRESET_ONE ? 21 : tRPpb + 3;
    // tRAS (row active time) in ns
    const u32 tRAS = !TIMING_PRESET_ONE ? 42 : tRAS_values[TIMING_PRESET_ONE-1];
    // tRC (ACTIVATE-ACTIVATE command period same bank) in ns
    const u32 tRC = tRPpb + tRAS;
    // tRFCpb (refresh cycle time per bank) in ns for 8Gb density
    const u32 tRFCpb = !TIMING_PRESET_FOUR ? 140 : tRFC_values[TIMING_PRESET_FOUR-1];
    // tRFCab (refresh cycle time all banks) in ns for 8Gb density
    const u32 tRFCab = !TIMING_PRESET_FOUR ? 280 : 2*tRFCpb;
    // tRCD (RAS-CAS delay) in ns
    const u32 tRCD = !TIMING_PRESET_ONE ? 18 : tRCD_values[TIMING_PRESET_ONE-1];
    // tRRD (Active bank-A to Active bank-B) in ns
    const double tRRD = !TIMING_PRESET_TWO ? 10. : tRRD_values[TIMING_PRESET_TWO-1];
    // tREFpb (average refresh interval per bank) in ns for 8Gb density
    const u32 tREFpb = !TIMING_PRESET_SIX ? 488 : tREFpb_values[TIMING_PRESET_SIX-1];
    // tREFab (average refresh interval all 8 banks) in ns for 8Gb density
    // const u32 tREFab = tREFpb * 8;
    // #_of_rows per die for 8Gb density
    const u32 numOfRows = 65536;
    // {REFRESH, REFRESH_LO} = max[(tREF/#_of_rows) / (emc_clk_period) - 64, (tREF/#_of_rows) / (emc_clk_period) * 97%]
    // emc_clk_period = dram_clk / 2;
    // 1600 MHz: 5894, but N' set to 6176 (~4.8% margin)
    const u32 REFRESH = u32(std::ceil((double(tREFpb) * C.marikoEmcMaxClock / numOfRows * 1.048 / 2 - 64))) / 4 * 4;
    // tACT2PDEN (timing delay from an activate, MRS or EMRS command to power-down entry) in ns
    // Valid clock and CS requirement after CKE input LOW after MRW command
    const u32 tMRWCKEL = 14;
    // Additional time after t XP hasexpired until the MRR commandmay be issued
    const double tMRRI = tRCD + 3 * tCK_avg;
    // tPDEX2WR, tPDEX2RD (timing delay from exiting powerdown mode to a write/read command) in ns
    //const u32 tPDEX2 = 10;
    // Exit power-down to next valid command delay
    const double tXP = 7.5;
    // tPDEX2MRR (timing delay from exiting powerdown mode to MRR command) in ns
    const double tPDEX2MRR = tXP + tMRRI;
    // [Guessed] tPDEX2MRR (timing delay from exiting powerdown mode to MRR command) in ns
    //const double tPDEX2MRR = 28.75;
    // [Guessed] tCKE2PDEN (timing delay from turning off CKE to power-down entry) in ns
    const double tCKE2PDEN = 8.5;
    // tXSR (SELF REFRESH exit to next valid command delay) in ns
    const double tXSR = tRFCab + 7.5;
    // tCKE (minimum CKE high pulse width) in ns
    const double tCKE = 7.5;
    // Delay from valid command to CKE input LOW in ns
    const double tCMDCKE = MAX(1.75, 3*tCK_avg);
    // Minimum self refresh time (entry to exit)
    const u32 tSR = 15;
    // [Guessed] tPD (minimum CKE low pulse width in power-down mode) in ns
    const double tPD = 7.5;
    // tFAW (Four-bank Activate Window) in ns
    const u32 tFAW = !TIMING_PRESET_TWO ? 40 : tFAW_values[TIMING_PRESET_TWO-1];
    // Internal READ-to-PRE-CHARGE command delay in ns
    const double tRTP = !TIMING_PRESET_THREE ? 7.5 : tRTP_values[TIMING_PRESET_THREE-1];
    const u32 WL = !TIMING_PRESET_SEVEN ? (C.marikoEmcMaxClock <= 2131200 ? 10 : 12) : tWL_values[TIMING_PRESET_SEVEN-1]; //?
    const u32 BL = 16;
    const u32 tWR = !TIMING_PRESET_THREE ? 18 : tWR_values[TIMING_PRESET_THREE-1];
    // write-to-precharge time for commands to the same bank in cycles
    const u32 WTP = WL + BL/2 + 1 + CEIL(tWR/tCK_avg);
    const double tDQSS_max = 1.25;
    const double tDQS2DQ_max = 0.8;
    // Write With Auto Precharge to to Power-Down Entry
    const u32 WTPDEN = WTP + 1 + CEIL(tDQSS_max/tCK_avg) + CEIL(tDQS2DQ_max/tCK_avg) + 6;
    // Valid Clock requirement before CKE Input HIGH in ns
    const double tCKCKEH = MAX(1.75, 3*tCK_avg);
    // Write-to-Read delay
    const u32 tWTR = !TIMING_PRESET_FIVE ? 10 : tWTR_values[TIMING_PRESET_FIVE-1];
    // Delay Time From WRITE-to-READ
    const u32 W2R = WL + BL/2 + 1 + CEIL(tWTR/tCK_avg);
    const u32 RL = !TIMING_PRESET_SEVEN ? (C.marikoEmcMaxClock <= 2131200 ? 20 : 24) : WL*2; //?
    const double tDQSCK_max = 3.5;
    // Write preamble (tCK)
    const double tWPRE = 1.8;
    // Read postamble (tck)
    const double tRPST = 0.4;
    const u32 R2W = CEIL (RL + CEIL(tDQSCK_max/tCK_avg) + BL/2 - WL + tWPRE + FLOOR(tRPST));
}

