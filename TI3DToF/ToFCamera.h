/*
 * TI Voxel Lib component.
 *
 * Copyright (c) 2014 Texas Instruments Inc.
 */

#ifndef VOXEL_TI_TOFCAMERA_H
#define VOXEL_TI_TOFCAMERA_H

#include <ToFCameraBase.h>
#include "ToFFrameGenerator.h"

#include "TI3DToFExports.h"

#define PIX_CNT_MAX_SET_FAILED "pix_cnt_max_set_failed"
#define PIX_CNT_MAX "pix_cnt_max"
#define QUAD_CNT_MAX "quad_cnt_max"
#define SUBFRAME_CNT_MAX "sub_frame_cnt_max"
#define SYS_CLK_FREQ "sys_clk_freq"

#define TG_EN "tg_en"
#define BLK_SIZE "blk_size"
#define BLK_HEADER_EN "blk_header_en"
#define OP_CS_POL "op_cs_pol"
#define FB_READY_EN "fb_ready_en"

#define CONFIDENCE_THRESHOLD "confidence_threshold"

#define ILLUM_EN_POL "illum_en_pol"

#define BIN_ROWS_TO_MERGE "rows_to_merge"
#define BIN_COLS_TO_MERGE "cols_to_merge"
#define BIN_ROW_COUNT "bin_row_count"
#define BIN_COLUMN_COUNT "bin_col_count"
#define BINNING_EN "binning_en"

#define ROW_START "row_start"
#define ROW_END "row_end"
#define COL_START "col_start"
#define COL_END "col_end"

#define DEBUG_EN "debug_en"

#define PIXEL_DATA_SIZE "pixel_data_size"
#define OP_DATA_ARRANGE_MODE "op_data_arrange_mode"
#define HISTOGRAM_EN "histogram_en"

#define INTG_TIME "intg_time"  // Integration time
#define INTG_DUTY_CYCLE "intg_duty_cycle"
#define INTG_DUTY_CYCLE_SET_FAILED "intg_duty_cycle_set_failed"
#define MOD_FREQ1 "mod_freq1" // Modulation frequency for first source (MHz)
#define MOD_FREQ2 "mod_freq2" // Modulation frequency for second source (MHz)
#define VCO_FREQ "vco_freq"
#define MOD_PS1 "mod_ps1"
#define MOD_PS2 "mod_ps2"
#define MOD_M "mod_m"
#define MOD_N "mod_n"
#define MOD_PLL_UPDATE "mod_pll_update"

#define MA "ma"
#define MB "mb"
#define K0 "ka"
#define DEALIAS_EN "dealias_en"
#define DEALIAS_16BIT_OP_ENABLE "dealias_16bit_op_enable"
#define DEALIASED_PHASE_MASK "dealiased_ph_mask"

#define PHASE_CORR_1 "phase_corr_1"
#define PHASE_CORR_2 "phase_corr_2"
#define TILLUM_CALIB "tillum_calib"
#define TSENSOR_CALIB "tsensor_calib"
#define COEFF_ILLUM_1 "coeff_illum_1"
#define COEFF_ILLUM_2 "coeff_illum_2"
#define COEFF_SENSOR_1 "coeff_sensor_1"
#define COEFF_SENSOR_2 "coeff_sensor_2"
#define DISABLE_OFFSET_CORR "disable_offset_corr"
#define DISABLE_TEMP_CORR "disable_temp_corr"
#define CALIB_PREC "calib_prec"

#define ToF_FRAME_TYPE "output_mode"
#define SOFTWARE_RESET "software_reset"

#define SPEED_OF_LIGHT 299792458.0

#define TI_VENDOR_ID 0x0451U

#define UNAMBIGUOUS_RANGE "unambiguous_range"

#define ILLUM_POWER "illum_power"
#define ILLUM_POWER_PERCENTAGE "illum_power_percentage"

namespace Voxel
{
  
namespace TI
{

// TI's ToF specific handling. This is generic base currently for TinTin and Haddock generation
class TI3DTOF_EXPORT ToFCamera: public ToFCameraBase
{
protected:
  bool _init();
  
  virtual bool _processRawFrame(const RawFramePtr &rawFrameInput, RawFramePtr &rawFrameOutput); // here output raw frame will have processed data, like ToF data for ToF cameras
  
  virtual bool _getAmplitudeNormalizingFactor(float &factor);
  virtual bool _getDepthScalingFactor(float& factor);
  
  virtual bool _setFrameRate(const FrameRate &r);
  virtual bool _getFrameRate(FrameRate &r) const;
  
  virtual bool _getFrameSize(FrameSize &s) const;
  virtual bool _setFrameSize(const FrameSize &s);
  virtual bool _setFrameSize(const FrameSize &s, bool resetROI);
  virtual bool _setStreamerFrameSize(const FrameSize &s) = 0;
  virtual bool _getMaximumFrameRate(FrameRate& frameRate, const FrameSize& forFrameSize) const;
  
  virtual bool _getBytesPerPixel(uint &bpp) const;
  virtual bool _setBytesPerPixel(const uint &bpp);

  virtual bool _getOpDataArrangeMode(int &dataArrangeMode) const;
  
  // Return the resultant modulation frequency. This is mod_freq1 for non-dealiased case and 
  // gcd(mod_freq1, mod_freq2) for dealiased case
  virtual bool _getIlluminationFrequency(float &frequency) const = 0;
  
  virtual bool _getBinning(uint &rowsToMerge, uint &columnsToMerge) const;
  virtual bool _setBinning(uint rowsToMerge, uint columnsToMerge, const FrameSize &frameSize);
  
  // Return in MHz
  virtual bool _getSystemClockFrequency(uint &frequency) const = 0;
  virtual bool _isHistogramEnabled() const = 0;
  
  virtual bool _getToFFrameType(ToFFrameType &frameType) const;
  
  virtual bool _is16BitModeEnabled(bool &mode16Bit);
  
  virtual bool _initStartParams();
  
  virtual bool _applyCalibrationParams() = 0;
  
  virtual bool _reset();
  
  Ptr<ToFFrameGenerator> _tofFrameGenerator;
  
public:
  ToFCamera(const String &name, DevicePtr device);
  
  virtual ~ToFCamera() {}
  
  friend class IntegrationTimeParameter;
};

}
}

#endif // VOXEL_TI_TOFCAMERA_H