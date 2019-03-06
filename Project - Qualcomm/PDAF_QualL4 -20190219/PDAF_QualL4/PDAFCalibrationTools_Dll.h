#ifndef _PDAFCAL_REVL_H_
#define _PDAFCAL_REVL_H_

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef PDAFCAL_REVL_EXPORTS
	#define PDAFCAL_REVL_EXPORTS_DECLSPEC    __declspec( dllexport )
#else
	#define PDAFCAL_REVL_EXPORTS_DECLSPEC
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "PDAFCalibrationTools_EEPROM.h"

//////////////////////////////////////////////
// DO NOT CHANGE

#define GAIN_MAP_MAX_SZ 256
#define DCC_MAP_MAX_SZ 144
#define MAX_PD_PIXELS_PER_BLOCK 128

#define GAIN_MAP_HEIGHT 13
#define GAIN_MAP_WIDTH 17
#define GAIN_MAP_Q_FMT 8

#define DCC_MAP_HEIGHT 6
#define DCC_MAP_WIDTH 8
#define DCC_MAP_Q_FMT 4

#define DCC_FIR {-1, -4, -6, -4, 0, 4, 6, 4, 1}
#define DCC_FIR_SZ 9

#define DCC_STACK_SZ 10
#define DCC_SEARCH_RANGE 48
#define DCC_FIT_EXCLUSION 1
#define DCC_MIN_PD_RANGE 1
#define DCC_MAP_OVERLAP 0.5

#define DPD_PIXEL_UNIT 2
#define DPD_MIN_SENSOR_GAIN 1.9
#define DPD_MAX_SENSOR_GAIN 3.1

#define LINECHART 0
#define DIAMONDCHART 1

#define NORMALIZE 0
#define WEIGHTED 1

#define DCC_LINEARITY_LEVEL_THRESHOLD 0.1
//////////////////////////////////////////////

//////////////////////////////////////////////
// DCC VALIDATION TOLERANCE 
#define DCC_VAL_TOL_SPARSE \
  { 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, \
  0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, \
  0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, \
  0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, \
  0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, \
  0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20 }

#define DCC_VAL_TOL_2BY1 \
  { 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, \
  0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, \
  0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, \
  0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, \
  0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, \
  0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15 }

#define DCC_VAL_TOL_DPD \
  {	0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, \
  0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, \
  0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, \
  0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, \
  0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, \
  0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10 }
//////////////////////////////////////////////

typedef struct _dll_config_t {
  int8_t cal_version;
  int8_t chart_type;
  int8_t verify_type;
  int8_t gain_map_height;
  int8_t gain_map_width;
  int8_t gain_map_q_fmt;
  int8_t dcc_map_height;
  int8_t dcc_map_width;
  int8_t dcc_map_q_fmt;
  float dcc_fir[31];
  int8_t dcc_fir_sz;
  int8_t dcc_stack_sz;
  int8_t dcc_search_range;
  int8_t dcc_fit_exclusion;
  int8_t dcc_min_pd_range;
  float  dcc_map_overlap;
  float  dcc_tolerance_table[48];
  float  dcc_linearity_level_theshold;
  int8_t dpd_pixel_unit;
  float  dpd_min_sensor_gain;
  float  dpd_max_sensor_gain;
} dll_config_t;


typedef enum _cfa_pattern_t {
   UNSPECIFIED=0,
   RGGB,
   BGGR,
   GRBG,
   GBRG
} cfa_t; 

typedef struct _sensor_config_t
{
  cfa_t    cfa;
  uint16_t image_width;
  uint16_t image_height;
  uint16_t black_lvl;
  uint16_t bit_depth;
  uint16_t pd_block_width;
  uint16_t pd_block_height;
  uint16_t pd_block_start_x;
  uint16_t pd_block_start_y;
  uint16_t pd_block_n_pd_pairs;
  uint16_t pd_block_l_x[MAX_PD_PIXELS_PER_BLOCK];
  uint16_t pd_block_l_y[MAX_PD_PIXELS_PER_BLOCK];
  uint16_t pd_block_r_x[MAX_PD_PIXELS_PER_BLOCK];
  uint16_t pd_block_r_y[MAX_PD_PIXELS_PER_BLOCK];
  uint16_t fv_measure_channel; //1:left-top, 2:right-top, 3:left_bottom, 4:right_bottom
} sensor_config_t;


typedef struct _gainmap_limits_t
{
  uint16_t gain_max_limit;
  uint16_t pd_min_limit;
  uint16_t pd_max_limit;
} gainmap_limits_t;

typedef struct _gainmap_t
{
  int8_t cal_version;
  uint16_t width;
  uint16_t height;
  uint16_t gm_l[GAIN_MAP_MAX_SZ];
  uint16_t gm_r[GAIN_MAP_MAX_SZ];
} gainmap_t;

typedef struct _dccmap_t
{
  int8_t cal_version;
  uint16_t q_format;
  uint16_t width;
  uint16_t height;
  int16_t dm[DCC_MAP_MAX_SZ];
} dccmap_t;

typedef struct _pdmap_t
{
  uint16_t width;
  uint16_t height;
  float pd[DCC_MAP_MAX_SZ];
} pdmap_t;

/* testbench utility functions */
inline uint8_t upperbyte( uint16_t twobytes );
inline uint8_t lowerbyte(uint16_t twobytes);
inline uint16_t big_to_little( uint16_t big_en );
void get_dll_cfg(dll_config_t *dll_cfg );
void print_return_code(int32_t rc);
int fliplr( uint16_t *img_in, int width, int height );
int flipud( uint16_t *img_in, int width, int height );
int read_raw_file(char *filename, uint16_t *img, int width, int height);
int read_bin_file(char *filename, uint16_t *img, uint16_t *width, uint16_t *height);
int read_2pd_raw_file(char *filename, uint16_t *img_l, uint16_t *img_r, 
  uint16_t width, uint16_t height, cfa_t cfa);
int read_2pd_bin_file(char *filename, uint16_t *img_l, uint16_t *img_r, 
  uint16_t *out_width, uint16_t *out_height, cfa_t cfa);
int read_gainmap_file(char *filename, gainmap_t *gain_map);
int write_gainmap_file(char *filename, gainmap_t *gain_map);
int write_cal_buf_txt_file(char *filename, pdaf_cal_t *pdaf_cal);
int write_cal_buf_bin_file(char *filename, char *buf, uint16_t size);
int read_cal_buf_bin_file(char *filename, char *buf, uint16_t size);
int write_focus_position_to_file(char* filename, int lens_pos, dccmap_t* dccmap, pdmap_t* pdmap);
errno_t cal_dump_to_txt(char* cal_filepath, char* txt_filepath);

/* get sensor configuration */
extern void get_sensor_cfg_3m2(sensor_config_t *sensor_cfg);
extern void get_sensor_cfg_3m3(sensor_config_t *sensor_cfg);
extern void get_sensor_cfg_ar1335pd(sensor_config_t *sensor_cfg);
extern void get_sensor_cfg_imx258_bin(sensor_config_t *sensor_cfg);
extern void get_sensor_cfg_imx258_hdd(sensor_config_t *sensor_cfg);
extern void get_sensor_cfg_imx258_hdr(sensor_config_t *sensor_cfg);
extern void get_sensor_cfg_imx258u_verLplus(sensor_config_t *sensor_cfg );
extern void get_sensor_cfg_imx258q_verLplus(sensor_config_t *sensor_cfg );
extern void get_sensor_cfg_imx362(sensor_config_t *sensor_cfg);
extern void get_sensor_cfg_imx386(sensor_config_t *sensor_cfg );
extern void get_sensor_cfg_imx398(sensor_config_t *sensor_cfg);
extern void get_sensor_cfg_ovt13853(sensor_config_t *sensor_cfg);
extern void get_sensor_cfg_ovt12890(sensor_config_t *sensor_cfg );

/* COMMON API FOR ALL PDAF SENSORS */
PDAFCAL_REVL_EXPORTS_DECLSPEC void* PDAF_Cal_create( dll_config_t *dll_cfg );
PDAFCAL_REVL_EXPORTS_DECLSPEC int32_t PDAF_Cal_destroy( void *p );
PDAFCAL_REVL_EXPORTS_DECLSPEC int32_t PDAF_Cal_validate_sensor_cfg( 
  uint16_t *img, uint16_t width, uint16_t height, sensor_config_t *sensor_cfg,
  uint16_t *img_l, uint16_t *img_r, uint16_t *out_width, uint16_t *out_height);

PDAFCAL_REVL_EXPORTS_DECLSPEC void PDAF_Cal_get_lib_version(char *s);
PDAFCAL_REVL_EXPORTS_DECLSPEC void PDAF_Cal_get_lib_features(char *s);
PDAFCAL_REVL_EXPORTS_DECLSPEC int32_t PDAF_Cal_get_dccmap( void *p, 
  dccmap_t *dcc_map, char* file_name);
PDAFCAL_REVL_EXPORTS_DECLSPEC void PDAF_Cal_get_calibration_block( 
  gainmap_t *gain_map, dccmap_t *dcc_map, pdaf_cal_t *pdaf_cal);
PDAFCAL_REVL_EXPORTS_DECLSPEC int PDAF_Cal_read_calibration_block( 
  gainmap_t *gain_map, dccmap_t *dcc_map, pdaf_cal_t *pdaf_cal);

/* API FOR SPARSE AND 2X1 OCL PDAF SENSORS */
PDAFCAL_REVL_EXPORTS_DECLSPEC int32_t PDAF_Cal_get_gainmap( void *p, 
  uint16_t *img, sensor_config_t *sensor_cfg, gainmap_t *gain_map, 
  gainmap_limits_t *gm_limits );
PDAFCAL_REVL_EXPORTS_DECLSPEC int32_t PDAF_Cal_add_raw( void *p, 
  int16_t stack_id, int16_t lens_pos, uint16_t *img, 
  sensor_config_t *sensor_cfg, gainmap_t *gain_map);
PDAFCAL_REVL_EXPORTS_DECLSPEC int32_t PDAF_Cal_get_raw_pd( void *p, 
  uint16_t *img, sensor_config_t *sensor_cfg, gainmap_t *gain_map,
  pdmap_t* pdmap );

/* API FOR DUAL PHOTODIODE PDAF SENSORS */
PDAFCAL_REVL_EXPORTS_DECLSPEC int32_t PDAF_Cal_get_gainmap_2pd( void *p, 
  uint16_t *img_l, uint16_t *img_r, uint16_t width, uint16_t height, 
  uint16_t black_lvl, gainmap_t *gain_map, gainmap_limits_t *gm_limits, 
  float sensor_gain);
PDAFCAL_REVL_EXPORTS_DECLSPEC int32_t PDAF_Cal_add_raw_2pd( void *p, 
  int16_t stack_id, int16_t lens_pos, uint16_t *img_l, uint16_t *img_r, 
  uint16_t width, uint16_t height, uint16_t black_lvl, gainmap_t *gm,
  float sensor_gain);
PDAFCAL_REVL_EXPORTS_DECLSPEC int32_t PDAF_Cal_get_raw_pd_2pd( void *p, 
  uint16_t *img_l, uint16_t *img_r, uint16_t width, uint16_t height,
  uint16_t black_lvl, gainmap_t *gain_map, pdmap_t* pdmap );




#if defined(__cplusplus)
}
#endif

#endif



