#ifndef PDAFCALIBRATION_TOOL_CONTEXT_H
#define PDAFCALIBRATION_TOOL_CONTEXT_H

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////
// DO NOT CHANGE
#define CONTEXT_MAX_GAIN_MAP_SZ 256
#define CONTEXT_MAX_DCC_MAP_SZ 144
#define CONTEXT_MAX_STACK_SZ 12
#define CONTEXT_MAX_SAD_SZ 101

///////////////////////////////////////////////////////////////
// GAIN MAP
// see calibration guideline for definition of LEFT/RIGHT pixels
typedef struct _context_v4_gainmap_t
{
  // gain map grid width 
  // same as GAIN_MAP_WIDTH (17)
  uint16_t width; 

  // gain map grid height
  // same as GAIN_MAP_HEIGHT (13)
  uint16_t height;

  // gain map of LEFT PD pixels (17x13)
  uint16_t gm_l[CONTEXT_MAX_GAIN_MAP_SZ];

  // gain map of LEFT PD pixels (17x13)
  uint16_t gm_r[CONTEXT_MAX_GAIN_MAP_SZ];
} context_v4_gainmap_t;

///////////////////////////////////////////////////////////////
// DCC MAP 
// see calibration guideline for definition of dcc
typedef struct _context_v4_dccmap_t
{
  // same as DCC_MAP_Q_FMT
  // dcc_int16 = (dcc_float << q_format) 
  uint16_t q_format;

  // dcc map grid width
  // same as DCC_MAP_WIDTH (8)
  uint16_t width;

  // dcc map grid height
  // same as DCC_MAP_HEIGHT (6)
  uint16_t height;

  // dcc map (8x6)
  int16_t dm[CONTEXT_MAX_DCC_MAP_SZ];
} context_v4_dccmap_t;

//////////////////////////////////////////////
// LENS POSITION MAPS
// see calibration guideline for dcc validation
typedef struct _lens_pos_map_t
{
  // 8x6 map of peak focus lens position from focus value curve [DAC]
  uint16_t fv[CONTEXT_MAX_DCC_MAP_SZ];

  // 8x6 map of peak focus lens position from phase disparity curve [DAC]
  uint16_t pd[CONTEXT_MAX_DCC_MAP_SZ];

  // 8x6 map of delta lens position = peak_lens_pos_fv - peak_lens_pos_pd [DAC]
  int16_t  delta[CONTEXT_MAX_DCC_MAP_SZ];

  // 8x6 map of ndelta = delta / abs(MACRO_DAC - INF_DAC)
  float    ndelta[CONTEXT_MAX_DCC_MAP_SZ];

  // 8x6 map of PD linearity 
  float    linearity_level[CONTEXT_MAX_DCC_MAP_SZ];
} lens_pos_map_t;


//////////////////////////////////////////////
// STACKED MAPS
// each dcc calibration requires 8 frame captures
// 8 frame capture creates 8 stacked maps
// see calibration guideline for dcc calibration
typedef struct _stack_map_t
{
  // 8x6 map of phase disparity (PD) values 
  float    pd_map[CONTEXT_MAX_DCC_MAP_SZ];
  
  // 8x6 map of focus values (FV) 
  float    fv_map[CONTEXT_MAX_DCC_MAP_SZ];
  
  // 8x6 map of confidence level 
  uint16_t cf_map[CONTEXT_MAX_DCC_MAP_SZ];
  
  // 8x6 map of dcc calibration return codes
  int32_t  rc_map[CONTEXT_MAX_DCC_MAP_SZ];
  
  // currently unused 
  float    sad_vector[CONTEXT_MAX_DCC_MAP_SZ][CONTEXT_MAX_SAD_SZ];

  // lens position used for frame capture [DAC]
  int16_t  lens_pos;
} stack_map_t;


typedef struct _context_v4_t
{ 
  // version number of context variable
  // current version number is 4
  uint16_t               context_version;

  // testing chart type: line/diamond
  uint8_t                chart_type;

  context_v4_gainmap_t   gain_map;

  context_v4_dccmap_t    dcc_map;

  lens_pos_map_t         lp_map;

  stack_map_t            stack_map[CONTEXT_MAX_STACK_SZ];

  // size of stacked maps (8) 
  uint16_t               dcc_stack_cnt;

  // currently unused
  uint16_t               dcc_stack_sz;

  // number of frames used in linear regression of dcc calibration (6)
  uint16_t               dcc_fit_cnt;

  // max range of phase disparity (PD)
  // calculated PD value is clipped to [-dcc_search_range, dcc_search_range]
  uint16_t               dcc_search_range;

  // each 8x6 grid is overlapped with adjacent grid by DCC_MAP_OVERLAP
  // 0.5 implies each grid is overlapped by adjacent grid by 50%
  float                  dcc_map_overlap;

  uint8_t                fv_measure_channel;

  // if any entry of 8x6 abs(lp_map.ndelta) > dcc_val_tol then 
  // dcc validation fails and returns error code 0x0200
  // same as DCC_VAL_TOL_SPARSE and DCC_VAL_TOL_DPD
  float                  dcc_val_tol[CONTEXT_MAX_DCC_MAP_SZ];

  // final return code for dcc calibration
  int32_t                dcc_map_rc;
} context_v4_t;

typedef context_v4_t context_t;

#if defined(__cplusplus)
}
#endif

#endif