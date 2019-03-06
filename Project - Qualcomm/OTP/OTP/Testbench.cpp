#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OTP\PDAFCalibrationTools_Dll.h"

#define space_conc(str1,str2) str1 str2

/***** Liteon IMX362 *****/
#if 1
#define SPARSE_PD 0
/* Input Folder Path */
#define ROOTPATH     "C:\\Users\\htony\\Tony\\Data\\PDAF_CAF_lib\\best\\"
/* Output Folder Path */
#define OUTPUTPATH   ROOTPATH

/* Input File Names */
#define FLATPATH     "imx362_8064_3024_white.raw"
#define DCCLENSPOS   { 359, 424, 489, 553, 618, 682, 747, 812 }
#define DCCRAWPATH   "PDCC_20cm_%d.raw"
#endif

/***** Sunny IMX398 *****/
#if 0
#define SPARSE_PD 1
/* Input Folder Path */
#define ROOTPATH     "C:\\Users\\htony\\Tony\\Data\\PDAF_CAF_lib\\IMX398\\IMX398-08-15\\8\\"
/* Output Folder Path */
#define OUTPUTPATH   ROOTPATH

/* Input File Names */
#define FLATPATH     "2PDGainMap.raw"
#define DCCLENSPOS   { 123, 186, 249, 312, 375, 438, 501, 564 }
#define DCCRAWPATH   "Qualcomm_L1_DCC_%d.raw"
#endif

/***** Semco 3M3 *****/
#if 0
#define SPARSE_PD 1
/* Input Folder Path */
#define ROOTPATH     "C:\\Users\\htony\\Tony\\Data\\PDAF_CAF_lib\\Semco\\3M3\\08-25\\module_1\\"
/* Output Folder Path */
#define OUTPUTPATH   ROOTPATH

/* Input File Names */
#define FLATPATH     "Gainmap.raw"
#define DCCLENSPOS   { 362, 412, 462, 512, 562, 612, 662, 712 }
#define DCCRAWPATH   "qc_pdaf_dcc_%d.raw"
#endif

/* OUTPUT File Names */
#define GAINMAPFN    "pdaf_[module_id]_[date]_[test_station_id]_gainmap.txt"
#define EEPROMBINFN  "pdaf_[module_id]_[date]_[test_station_id]_eeprom.bin"
#define EEPROMTXTFN  "pdaf_[module_id]_[date]_[test_station_id]_eeprom.txt"
#define CAL_DIAG_FN  "pdaf_[module_id]_[date]_[test_station_id]_diagnostic.cal"
#define DUMP_FILE_FN "pdaf_[module_id]_[date]_[test_station_id]_diagnostic.cal"
#define DUMP_TXT_FN  "pdaf_[module_id]_[date]_[test_station_id]_diagnostic.ana"


/////////////////////////////////////////////////////////////////////////
// sample gain map calibration for sparse PD sensor
int32_t example_gm(int argc, char** argv)
{
  void *p; 
  char dll_version_info[1024];
  char dll_feature_info[2048];

  dll_config_t     dll_cfg;
  sensor_config_t  sensor_cfg;
  gainmap_t        gain_map;
  gainmap_limits_t gm_limits;

  uint16_t *img_raw;
  uint16_t raw_width, raw_height;
  int32_t  rc = 0;

  // set test limits
  // calibration DLL will still return gain map even if limits are exceeded
  gm_limits.pd_max_limit   = 950;  // Max PD pixel value after LPF must be below 950
  gm_limits.pd_min_limit   = 100;  // Min PD pixel value after LPF must exceed 100
  gm_limits.gain_max_limit = (uint16_t) (7.999f * (1<<GAIN_MAP_Q_FMT));  // max gain of 8x

  // load dll config parameters
  get_dll_cfg( &dll_cfg );
  
  /////////////////////////////////////////////////////////////////////////
  // INIT RUN ONLY ONCE 

  // allocate scratch buffer for calibration dll
  p = PDAF_Cal_create( &dll_cfg ); 
  if( p == NULL ) {
    return -1;  // dll memory allocation error
  }

  // calibration DLL for version number and version description
  PDAF_Cal_get_lib_version(dll_version_info);  printf("%s", dll_version_info);
  PDAF_Cal_get_lib_features(dll_feature_info); printf("%s", dll_feature_info);

  // obtain new sensor config file from sensor manufacturer
  // add extern declaration to PDAFCalibrationTools_Dll.h and call sensor cfg here
  //get_sensor_cfg_3m2(&sensor_cfg);
  //get_sensor_cfg_ar1335pd(&sensor_cfg);
  //get_sensor_cfg_imx258_bin(&sensor_cfg);
  //get_sensor_cfg_imx258_hdd(&sensor_cfg);
  //get_sensor_cfg_imx258_hdr(&sensor_cfg);
  //get_sensor_cfg_imx398(&sensor_cfg);
  //get_sensor_cfg_3m3(&sensor_cfg);
  //get_sensor_cfg_ovt13853(&sensor_cfg);  

  // specify information about pdaf sensor
  raw_width  = sensor_cfg.image_width;
  raw_height = sensor_cfg.image_height;

  // allocate buffer for reading flat field image
  // incorrect buffer size may terminate DLL with invalid memory access
  img_raw = (uint16_t *)malloc(raw_width * raw_height * sizeof(uint16_t));

  /////////////////////////////////////////////////////////////////////////
  // PER MODULE CALIBRATION
  // RECOMMEND USING SAME FLAT FIELD AS LENS SHADING CALIBRATION   

  // load flat field image
  if (read_raw_file(space_conc(ROOTPATH,FLATPATH), img_raw, raw_width, raw_height) ) {
  //if (read_bin_file(space_conc(ROOTPATH,FLATPATH), img_raw, &raw_width, &raw_height) ) {
    return -2;
  }
  
  // gain map calibration using parsed left and right images
  rc = PDAF_Cal_get_gainmap(p, img_raw, &sensor_cfg, &gain_map, &gm_limits );
  
  // display errors
  print_return_code(rc);

  // do not write NVM if return code indicates error
  if ( rc != 0 ) {
    return -3;
  }

  // store gain map in OTP/NVM
  write_gainmap_file(space_conc(OUTPUTPATH,GAINMAPFN), &gain_map);

  /////////////////////////////////////////////////////////////////////////
  // DE-INIT
  free(img_raw);
  PDAF_Cal_destroy( p ); 
  return rc;
}

/////////////////////////////////////////////////////////////////////////
// sample defocus conversion coefficient calibration for sparse PD sensors including 2x1 OCL
int example_dcc(int argc, char** argv)
{
  void *p;
  char dll_version_info[1024];
  char dll_feature_info[2048];

  char path_dcc_stack[DCC_STACK_SZ][1024];

  dll_config_t    dll_cfg;
  sensor_config_t sensor_cfg;
  gainmap_t       gain_map;
  dccmap_t        dcc_map;
  pdaf_cal_t      pdaf_cal;
  float tolerance_table[48] = DCC_VAL_TOL_SPARSE;

  int16_t  stack_id;
  uint16_t *img_raw;
  uint16_t raw_width, raw_height;
  
  int32_t  rc;

  // number of lens positions to be used for dcc calibration. do not change
  const uint16_t dcc_stack_sz = DCC_STACK_SZ;  

  // read AF calibration
  // move lens from INF/PAN to MACRO in 1/7 equally spaced steps
  // this example assumes INF/PAN focus=240 and MACRO focus=800
   int16_t dcc_stack_lenspos[] = DCCLENSPOS;

  // Debug print
  for( stack_id=0; stack_id < dcc_stack_sz; stack_id++ ) {
	sprintf(path_dcc_stack[stack_id],
		space_conc(ROOTPATH, DCCRAWPATH), dcc_stack_lenspos[stack_id]);
  }
 
  // load dll config parameters
  get_dll_cfg( &dll_cfg );

  // set DCC validation tolerance
  // dll_cfg.chart_type = DIAMONDCHART;
  dll_cfg.chart_type = LINECHART;
  memcpy(&dll_cfg.dcc_tolerance_table, tolerance_table, sizeof(tolerance_table));
  /////////////////////////////////////////////////////////////////////////
  // INIT RUN ONLY ONCE 

  // allocate scratch buffer for calibration dll
  p = PDAF_Cal_create( &dll_cfg ); 
  if( p == NULL ) {
    return -1;  // dll memory allocation error
  }

  // calibration DLL for version number and version description
  PDAF_Cal_get_lib_version(dll_version_info);  printf("%s", dll_version_info);
  PDAF_Cal_get_lib_features(dll_feature_info); printf("%s", dll_feature_info);

  // obtain new sensor config file from sensor manufacturer
  // add extern declaration to PDAFCalibrationTools_Dll.h and call sensor cfg here
  //get_sensor_cfg_3m2(&sensor_cfg);
  //get_sensor_cfg_3m3(&sensor_cfg);
  //get_sensor_cfg_ar1335pd(&sensor_cfg);
  //get_sensor_cfg_imx258_bin(&sensor_cfg);
  //get_sensor_cfg_imx258_hdd(&sensor_cfg);
  //get_sensor_cfg_imx258_hdr(&sensor_cfg);
  //get_sensor_cfg_imx398(&sensor_cfg);
  //get_sensor_cfg_ovt13853(&sensor_cfg);  
  
  // read gain map file
  if (read_gainmap_file(space_conc(OUTPUTPATH,GAINMAPFN), &gain_map)) 
    return -1;

  // specify information about pdaf sensor
  raw_width  = sensor_cfg.image_width;
  raw_height = sensor_cfg.image_height;

  // allocate buffer for reading flat field image
  // incorrect buffer size may terminate DLL with invalid memory access
  img_raw = (uint16_t *)malloc(raw_width * raw_height * sizeof(uint16_t));

  
  /////////////////////////////////////////////////////////////////////////
  // PER MODULE CALIBRATION
  
  // initialize return code
  rc = 0;

  for( stack_id=0; stack_id < dcc_stack_sz; stack_id++ ) {
    printf("run %d\n", stack_id);
    // read dcc calibration test image
    if (read_raw_file(path_dcc_stack[stack_id], img_raw, raw_width, raw_height) ) {
      return -1;
    }

    // move lens to next position here!
    // to save time move lens and read next raw 
    // while running PDAF_CAL_dcc_stack

    // add dcc calibration image to stack
    rc |= PDAF_Cal_add_raw(p, stack_id, dcc_stack_lenspos[stack_id],
      img_raw, &sensor_cfg, &gain_map);

  }

  // proceed to dcc calibration if stacking returned no error
  if( rc == 0 ) {
    // return defocus conversion coeff
    // use PDAF_Cal_dcc_fit(p, &dcc_map, NULL) if diagnostic file not needed
    rc |= PDAF_Cal_get_dccmap(p, &dcc_map, space_conc(OUTPUTPATH,CAL_DIAG_FN));
  }
  
  
  // display errors
  print_return_code(rc);

  // do not write NVM if return code indicates error
  if (rc != 0) {
    return rc;
  }
  
  // get OTP/EEPROM block for write
  PDAF_Cal_get_calibration_block( &gain_map, &dcc_map, &pdaf_cal);

  // program pdaf_cal into EEPROM
  write_cal_buf_bin_file( space_conc(OUTPUTPATH,EEPROMBINFN), (char *) &pdaf_cal, sizeof(pdaf_cal));
  
  // save calibration buffer as text file for viewing
  write_cal_buf_txt_file(space_conc(OUTPUTPATH,EEPROMTXTFN), &pdaf_cal);
  

  /////////////////////////////////////////////////////////////////////////
  // DE-INIT
  free(img_raw); 
  PDAF_Cal_destroy( p ); 

  return 0;

}

/////////////////////////////////////////////////////////////////////////
// sample gain map calibration for dual-photodiode (2pd) sensor
int32_t example_gm_2pd(int argc, char** argv)
{

  void *p; 
  char dll_version_info[1024];
  char dll_feature_info[2048];

  dll_config_t     dll_cfg;
  sensor_config_t  sensor_cfg;
  gainmap_t        gain_map;
  gainmap_limits_t gm_limits;

  uint16_t *img_l, *img_r, black_lvl;
  uint16_t raw_width, raw_height, pd_width, pd_height;
  cfa_t    cfa;
  float    sensor_gain;
  
  int32_t  rc = 0;

  // set test limits
  // calibration DLL will still return gain map even if limits are exceeded
  gm_limits.pd_max_limit   = 950;  // Max PD pixel value after LPF must be below 950
  gm_limits.pd_min_limit   = 100;   // Min PD pixel value after LPF must exceed 300
  gm_limits.gain_max_limit = (uint16_t) (7.999f * (1<<GAIN_MAP_Q_FMT));  // max gain of 6x

  // specify sensor gain = sensor analog gain * sensor digital gain used
  // analog gain of dual-photodiode PD sensor must be between 2.0x and 3.0x 
  // digital gain of dual-photodiode PD sensor must be 1x or disabled
  // this is to prevent pixel from entering nonlinear region  
  sensor_gain = 2.0f; 

  // load dll config parameters
  get_dll_cfg( &dll_cfg );
  
  /////////////////////////////////////////////////////////////////////////
  // INIT RUN ONLY ONCE 

  // allocate scratch buffer for calibration dll
  p = PDAF_Cal_create( &dll_cfg ); 
  if( p == NULL ) {
    return -1;  // dll memory allocation error
  }

  // calibration DLL for version number and version description
  PDAF_Cal_get_lib_version(dll_version_info);  printf("%s", dll_version_info);
  PDAF_Cal_get_lib_features(dll_feature_info); printf("%s", dll_feature_info);

  // obtain new sensor config file from sensor manufacturer
  // add extern declaration to PDAFCalibrationTools_Dll.h and call sensor cfg here
  //get_sensor_cfg_imx362(&sensor_cfg);

  // specify information about pdaf sensor
  raw_width  = sensor_cfg.image_width;
  raw_height = sensor_cfg.image_height;
  black_lvl  = sensor_cfg.black_lvl;
  cfa        = sensor_cfg.cfa;
  pd_width   = raw_width  >> 2; // pd image width is 1/4 raw image width
  pd_height  = raw_height >> 2; // pd image height is 1/4 raw image height


  // allocate buffer for reading flat field image
  // incorrect buffer size may terminate DLL with invalid memory access
  img_l = (uint16_t *)malloc(raw_width * raw_height * sizeof(uint16_t));
  img_r = (uint16_t *)malloc(raw_width * raw_height * sizeof(uint16_t));
  if (img_l == NULL || img_r == NULL) 
    return -1;


  /////////////////////////////////////////////////////////////////////////
  // PER MODULE CALIBRATION
  // RECOMMEND USING SAME FLAT FIELD AS LENS SHADING CALIBRATION 

  // load flat field image
  if (read_2pd_raw_file(space_conc(ROOTPATH,FLATPATH), 
    img_l, img_r, raw_width, raw_height, cfa) ) {
    return -1;
  }

  // gain map calibration using parsed left and right images
  rc = PDAF_Cal_get_gainmap_2pd(p, img_l, img_r, pd_width, pd_height,
    black_lvl, &gain_map, &gm_limits, sensor_gain);

  // display errors
  print_return_code(rc);

  // do not write NVM if return code indicates error
  if ( rc != 0 ) {
    return rc;
  }

  // store gain map in OTP/NVM
  write_gainmap_file(space_conc(OUTPUTPATH,GAINMAPFN), &gain_map);

  /////////////////////////////////////////////////////////////////////////
  // DE-INIT
  free(img_l);
  free(img_r);
  PDAF_Cal_destroy( p );
    
  return rc;
}

/////////////////////////////////////////////////////////////////////////
// sample defocus conversion coefficient calibration for dual-photodiode (2pd) sensor
int example_dcc_2pd(int argc, char** argv)
{
  void *p;
  char dll_version_info[1024];
  char dll_feature_info[2048];

  char path_dcc_stack[DCC_STACK_SZ][1024];

  dll_config_t    dll_cfg;
  sensor_config_t sensor_cfg;
  gainmap_t       gain_map;
  dccmap_t        dcc_map;
  pdaf_cal_t      pdaf_cal;
  float tolerance_table[48] = DCC_VAL_TOL_DPD;

  int16_t  stack_id;
  uint16_t *img_l, *img_r, black_lvl;
  uint16_t raw_width, raw_height, img_width, img_height;
  cfa_t    cfa;
  float    sensor_gain;
  
  int32_t  rc;
  
  // number of lens positions to be used for dcc calibration. do not change
  const uint16_t dcc_stack_sz = DCC_STACK_SZ;  
  
  // read AF calibration
  // move lens from INF/PAN to MACRO in 1/7 equally spaced steps
  // this example assumes INF/PAN focus=100 and MACRO focus=380
  int16_t dcc_stack_lenspos[] = DCCLENSPOS;

  // Debug print
  for( stack_id=0; stack_id < dcc_stack_sz; stack_id++ ) {
	sprintf(path_dcc_stack[stack_id],
	space_conc(ROOTPATH, DCCRAWPATH), dcc_stack_lenspos[stack_id]);
  }

  // load dll config parameters
  get_dll_cfg( &dll_cfg );

  // set DCC validation tolerance
  dll_cfg.chart_type = LINECHART;
  memcpy(&dll_cfg.dcc_tolerance_table, tolerance_table, sizeof(tolerance_table));
  /////////////////////////////////////////////////////////////////////////
  // INIT RUN ONLY ONCE 

  // allocate scratch buffer for calibration dll
  p = PDAF_Cal_create( &dll_cfg ); 
  if( p == NULL ) {
    return -1;  // dll memory allocation error
  } 

  // calibration DLL for version number and version description
  PDAF_Cal_get_lib_version(dll_version_info);  printf("%s", dll_version_info);
  PDAF_Cal_get_lib_features(dll_feature_info); printf("%s", dll_feature_info);

  // obtain new sensor config file from sensor manufacturer
  // add extern declaration to PDAFCalibrationTools_Dll.h and call sensor cfg here
  //get_sensor_cfg_imx362(&sensor_cfg);
  
  // read gain map file
  if (read_gainmap_file(space_conc(OUTPUTPATH,GAINMAPFN), &gain_map)) 
    return -1;

  // specify information about pdaf sensor
  raw_width   = sensor_cfg.image_width;
  raw_height  = sensor_cfg.image_height;
  black_lvl   = sensor_cfg.black_lvl;
  cfa         = sensor_cfg.cfa;
  img_width   = raw_width  >> 2; // tail mode pd image width=1/4 raw width
  img_height  = raw_height >> 2; // tail mode pd image height=1/4 raw height

  // specify sensor gain = sensor analog gain * sensor digital gain used
  // analog gain of dual-photodiode PD sensor must be between 2.0x and 3.0x 
  // digital gain of dual-photodiode PD sensor must be 1x or disabled
  // this is to prevent pixel from entering nonlinear region
  sensor_gain = 2.0f;               

  // allocate buffer for reading flat field image
  // incorrect buffer size may terminate DLL with invalid memory access
  img_l = (uint16_t *)malloc(raw_width * raw_height * sizeof(uint16_t));
  img_r = (uint16_t *)malloc(raw_width * raw_height * sizeof(uint16_t));
  if (img_l == NULL || img_r == NULL) 
    return -1;
  
  /////////////////////////////////////////////////////////////////////////
  // PER MODULE CALIBRATION
  
  // initialize return code
  rc = 0;

  for( stack_id=0; stack_id < dcc_stack_sz; stack_id++ ) {
     printf("run %d\n", stack_id);
     // read dcc calibration test image
     if (read_2pd_raw_file(path_dcc_stack[stack_id], 
       img_l, img_r, raw_width, raw_height, cfa) ) {
       return -1;
     }

     // move lens to next position here!
     // to save time move lens and read next raw 
     // while running PDAF_CAL_dcc_stack
     
     // add dcc calibration image to stack
     rc |= PDAF_Cal_add_raw_2pd(p, stack_id, dcc_stack_lenspos[stack_id],
       img_l, img_r, img_width, img_height, black_lvl, &gain_map, sensor_gain);

  }

  // proceed to dcc calibration if stacking returned no error
  if( rc == 0 ) {
    // return defocus conversion coeff
    // use PDAF_Cal_dcc_fit(p, &dcc_map, 0) 
    // use PDAF_Cal_dcc_fit(p, &dcc_map, NULL) if diagnostic file not needed
    rc |= PDAF_Cal_get_dccmap(p, &dcc_map, space_conc(OUTPUTPATH,CAL_DIAG_FN));
  }
  
  
  // display errors
  print_return_code(rc);

  // do not write NVM if return code indicates error
  if (rc != 0) {
    return rc;
  }

  // get OTP/EEPROM block for write
  PDAF_Cal_get_calibration_block( &gain_map, &dcc_map, &pdaf_cal);

  // program pdaf_cal into EEPROM

  // generate .bin f
  write_cal_buf_bin_file( space_conc(OUTPUTPATH,EEPROMBINFN), (char *) &pdaf_cal, sizeof(pdaf_cal));

  // save calibration buffer as text file for viewing
  write_cal_buf_txt_file(space_conc(OUTPUTPATH,EEPROMTXTFN), &pdaf_cal);


  /////////////////////////////////////////////////////////////////////////
  // DE-INIT
  free(img_l); free(img_r);
  PDAF_Cal_destroy( p );
  return 0;

}

int main(int argc, char** argv)
{

  int rc = 0;
  /////////////////////////////////////////////////////////////////////
  // EXAMPLE CALIBRATIONS
   
#if SPARSE_PD
  // gain map calibration example for sparse pd sensor (e.g. imx258, imx398, 3m2)
  rc = rc | example_gm(argc, argv);

  // dcc map calibration example for sparse pd sensor (e.g. imx258, imx398, 3m2)
  rc = rc | example_dcc(argc, argv);
#else
  // gain map calibration example for 2pd sensor (e.g. imx362, 2l7)
  rc = rc | example_gm_2pd(argc, argv);

  // dcc map calibration example for 2pd sensors (e.g. imx362, 2l7)
  rc = rc | example_dcc_2pd(argc, argv);
#endif

  rc = rc | cal_dump_to_txt(space_conc(OUTPUTPATH,DUMP_FILE_FN), space_conc(OUTPUTPATH,DUMP_TXT_FN));

  return rc;
}
