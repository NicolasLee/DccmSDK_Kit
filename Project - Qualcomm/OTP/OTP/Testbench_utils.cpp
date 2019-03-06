#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PDAFCalibrationTools_Dll.h"
#include "PDAFCalibrationTools_Context.h"

#define ABS(x) ((x<0)?-(x):(x))

inline uint8_t upperbyte( uint16_t twobytes ) 
{
  return twobytes >> 8;
}

inline uint8_t lowerbyte( uint16_t twobytes ) 
{
  return twobytes & 0x00FF;
}

inline uint16_t big_to_little( uint16_t big_en )
{
  return (lowerbyte(big_en) << 8) + upperbyte(big_en);
}

void print_return_code(int32_t rc)
{
  // error codes from gain map calibration

  // 0x0001: Memory allocation error
  // 0x0002: Invalid sensor configuration or sensor gain
  // 0x0004: Invalid pd pixel pattern within block
  // 0x0008: Gain Map: pixel values too high for calibration
  // 0x0010: Gain Map: pixel values too low for calibration
  // 0x0020: Gain Map: gain map shows maximum value that is too high
  // 0x0040: DCC: phase disparity values out of bound. invalid input image 
  // 0x0080: DCC: phase disparity values out of order. LEFT and RIGHT pixels may be swapped 
  // 0x0100: DCC: insufficient number of lens sweep prior to dcc calculation
  // 0x0200: DCC: insufficient match between zero phase disparity focus peak
  // 0x0400: DCC: insufficient range of pd values observed from INF to MACRO
  // 0x1000: DCC: Please check line chart condition.
  // 0x2000: DCC: object distance close to fit boundary.

  if (rc & 0x0001)
    printf("\nError code 0x0001: Memory allocation error.");
  if (rc & 0x0002)
    printf("\nError code 0x0002: Invalid sensor configuration or sensor gain.");
  if (rc & 0x0004)
    printf("\nError code 0x0004: Invalid pd pixel pattern within block.");
  if (rc & 0x0008)
    printf("\nError code 0x0008: Gain Map: pd pixel values too high for calibration.");
  if (rc & 0x0010)
    printf("\nError code 0x0010: Gain Map: pd pixel values too low for calibration.");
  if (rc & 0x0020)
    printf("\nError code 0x0020: Gain Map: gain map shows maximum value that is too high.");
  if (rc & 0x0040)
    printf("\nError code 0x0040: DCC: phase disparity values out of bound. invalid input image.");
  if (rc & 0x0080)
    printf("\nError code 0x0080: DCC: phase disparity values out of order. LEFT and RIGHT pixels may be swapped.");
  if (rc & 0x0100)
    printf("\nError code 0x0100: DCC: insufficient number of lens sweep prior to dcc calculation.");
  if (rc & 0x0200)
    printf("\nError code 0x0200: DCC: insufficient match between zero phase disparity focus peak.");
  if (rc & 0x0400)
    printf("\nError code 0x0400: DCC: insufficient range of pd values from INF to MACRO.");
  if (rc & 0x1000)
    printf("\nError code 0x1000: DCC: Please check line chart condition.");
  if (rc & 0x2000)
    printf("\nError code 0x2000: DCC: object distance is too close to stack fit boundary.");
}


// image flip left right
int fliplr( uint16_t *img_in, int width, int height ) 
{
  uint16_t i, j, *img_tmp;

  img_tmp = (uint16_t *) malloc( width * height * sizeof(uint16_t));
  if( img_tmp == NULL ) {
    return -1;
  }
  
  for(j=0; j < width; j++){
    for(i=0; i < height; i++){
      img_tmp[i*width + (width-1-j)] = img_in[i*width+j];
    }  
  }  

  memcpy(img_in, img_tmp, width * height * sizeof(uint16_t));
  free( img_tmp );
  
  return 0;
}

// image flip up down
int flipud( uint16_t *img_in, int width, int height ) 
{
  uint16_t i, j, *img_tmp;

  img_tmp = (uint16_t *) malloc( width * height * sizeof(uint16_t));
  if( img_tmp == NULL ) {
    return -1;
  }
 
  for(i=0; i < height; i++){
    for(j=0; j < width; j++){
      img_tmp[ (height-1-i)*width + j] = img_in[i*width+j];
    }  
  }  

  memcpy(img_in, img_tmp, width * height * sizeof(uint16_t));
  free( img_tmp );
  
  return 0;
}


int read_raw_file(char *filename, uint16_t *img, int width, int height)
{
	int i, j;
	FILE *fp;
	uint16_t pix;

	fp = fopen(filename, "rb");
	if (fp == NULL) {
		return -1;
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			fread(&pix, sizeof(uint16_t), 1, fp);
			img[i*width + j] = uint16_t(pix);
		}
	}
	fclose(fp);
	return 0;
}

int read_bin_file(char *filename, uint16_t *img, uint16_t *width, uint16_t *height)
{
  int i, j;
  FILE *fp;
  uint16_t pix, img_width, img_height;

  fp = fopen(filename, "rb");
  if (fp == NULL) {
  printf("Cannot open file for read: %s\n", filename);
  	return -1;
  }

  // read file dim
  fread(&pix, sizeof(uint16_t), 1, fp);
  img_width = pix;
  fread(&pix, sizeof(uint16_t), 1, fp);
  img_height = pix;

  // first two lines are dummy lines
  for (i = 0; i < 2; i++) {
    for (j = 0; j < img_width; j++) {
  		fread(&pix, sizeof(uint16_t), 1, fp);
  	}
  }

  //img_height = img_height - 3;  // UNCOMMENT TO USE WITH IMX298
  img_height = img_height - 2; // USE FOR OTHER IMX SENSORS

  // IMX298 .bin files contains 2 dummy row at start
  // and 1 dummy row at the end

  // read image
  for (i = 0; i < img_height; i++)	{
    for (j = 0; j < img_width; j++) {
  		fread(&pix, sizeof(uint16_t), 1, fp);
      img[i*img_width + j] = uint16_t(pix);
  	}
  }
  fclose(fp);

  // return raw image width and height
  *width = img_width;
  *height = img_height;

	return 0;
}

int read_2pd_raw_file(char *filename, uint16_t *img_l, uint16_t *img_r, 
  uint16_t width, uint16_t height, cfa_t cfa)
{
  int i, j, k, l;
  FILE *fp;
  uint16_t *img;
  uint16_t pix;

  fp = fopen(filename, "rb");
  if (fp == NULL) {
  printf("Cannot open file for read: %s\n", filename);
  	return -1;
  }

  // allocate temp buffer
  img = (uint16_t *) malloc( width * height * sizeof(uint16_t));
  if( img == NULL ) {
    return -1;
  }

  // read file content
  for (i = 0; i < height; i++)	{
  	for (j = 0; j < width; j++) {
  		fread(&pix, sizeof(uint16_t), 1, fp);
  		img[i*width + j] = uint16_t(pix);
  	}
  }
  fclose(fp);


  // parse left and right pd images with 2x2 green binning to mimic tail mode buffer
  // if tail mode buffer is directly from sensor (e.g. IMX362 mode 3/4) bypass this step
  if (cfa == GRBG || cfa == GBRG ) {
    for (i = 0; i < (height>>2); i++)	{
      for (j = 0; j < (width>>2); j++) {
        k = i<<2;
        l = j<<2;
        
        img_l[ i*(width>>2) + j] = 
          ( img[ (k+0)*width + (l+0)] + 
            img[ (k+1)*width + (l+2)] + 
            img[ (k+2)*width + (l+0)] + 
            img[ (k+3)*width + (l+2)] )>>2;

        img_r[i*(width>>2) + j] =
          ( img[ (k+0)*width + (l+1)] + 
            img[ (k+1)*width + (l+3)] + 
            img[ (k+2)*width + (l+1)] + 
            img[ (k+3)*width + (l+3)] )>>2;      
      }
    }
  } else {
    for (i = 0; i < (height>>2); i++)	{
      for (j = 0; j < (width>>2); j++) {
        k = i<<2;
        l = j<<2;
        
        img_l[ i*(width>>2) + j] = 
          ( img[ (k+0)*width + (l+2)] + 
            img[ (k+1)*width + (l+0)] + 
            img[ (k+2)*width + (l+2)] + 
            img[ (k+3)*width + (l+0)] )>>2;

        img_r[i*(width>>2) + j] =
          ( img[ (k+0)*width + (l+3)] + 
            img[ (k+1)*width + (l+1)] + 
            img[ (k+2)*width + (l+3)] + 
            img[ (k+3)*width + (l+1)] )>>2;      
      }
    }
  }

  // free temp buffer
  free(img);

  return 0;
}

int read_2pd_bin_file(char *filename, uint16_t *img_l, uint16_t *img_r, 
  uint16_t *out_width, uint16_t *out_height, cfa_t cfa)
{
  int i, j, k, l;
  FILE *fp;
  uint16_t *img;
  uint16_t pix, width, height;

  fp = fopen(filename, "rb");
  if (fp == NULL) {
  printf("Cannot open file for read: %s\n", filename);
  	return -1;
  }

  // read file dim
  fread(&pix, sizeof(uint16_t), 1, fp);
  width = pix;
  fread(&pix, sizeof(uint16_t), 1, fp);
  height = pix;

  // allocate temp buffer
  img = (uint16_t *) malloc( width * height * sizeof(uint16_t));
  if( img == NULL ) {
    return -1;
  }

  // read file content
  for (i = 0; i < height; i++)	{
  	for (j = 0; j < width; j++) {
  		fread(&pix, sizeof(uint16_t), 1, fp);
  		img[i*width + j] = uint16_t(pix);
  	}
  }
  fclose(fp);

  // parse left and right pd images with 2x2 green binning to mimic tail mode buffer
  // if tail mode buffer is directly from sensor (e.g. IMX362 mode 3/4) bypass this step
  if (cfa == GRBG || cfa == GBRG ) {
    for (i = 0; i < (height>>2); i++)	{
    	for (j = 0; j < (width>>2); j++) {
        k = i<<2;
        l = j<<2;
        
    		img_l[ i*(width>>2) + j] = 
        ( img[ (k+0)*width + (l+0)] + 
          img[ (k+1)*width + (l+2)] + 
          img[ (k+2)*width + (l+0)] + 
          img[ (k+3)*width + (l+2)] )>>2;

        img_r[i*(width>>2) + j] =
        ( img[ (k+0)*width + (l+1)] + 
          img[ (k+1)*width + (l+3)] + 
          img[ (k+2)*width + (l+1)] + 
          img[ (k+3)*width + (l+3)] )>>2;      
    	}
    }
  }else {
    for (i = 0; i < (height>>2); i++)	{
    	for (j = 0; j < (width>>2); j++) {
        k = i<<2;
        l = j<<2;
        
    		img_l[ i*(width>>2) + j] = 
        ( img[ (k+0)*width + (l+2)] + 
          img[ (k+1)*width + (l+0)] + 
          img[ (k+2)*width + (l+2)] + 
          img[ (k+3)*width + (l+0)] )>>2;

        img_r[i*(width>>2) + j] =
        ( img[ (k+0)*width + (l+3)] + 
          img[ (k+1)*width + (l+1)] + 
          img[ (k+2)*width + (l+3)] + 
          img[ (k+3)*width + (l+1)] )>>2;      
    	}
    }

  }


  // free temp buffer
  free(img);

  // return pd image width and height
  *out_width = width>>2;
  *out_height = height>>2;

	return 0;
}

int read_gainmap_file(char *filename, gainmap_t *gain_map)
{
  FILE *fp;
  int i;

	fp = fopen(filename,"r");
	if (fp == NULL) {
    printf("Error: cannot open file for read: %s\n", filename);
		return -1;
  }

  fscanf(fp, "%d", &gain_map->cal_version);

  fscanf(fp, "%hu", &gain_map->width);
  fscanf(fp, "%hu", &gain_map->height);

  for (i = 0; i < gain_map->height*gain_map->width; i++)
  	fscanf(fp, "%hu", &(gain_map->gm_l[i]));
  for (i = 0; i < gain_map->height*gain_map->width; i++)
  	fscanf(fp, "%hu", &(gain_map->gm_r[i]));


  fclose(fp);
  return 0;
}

int write_gainmap_file(char *filename, gainmap_t *gain_map) 
{
  int i;
	FILE *fp = fopen(filename,"w");
  if( fp == NULL ) {
    printf("Cannot open file for write: %s\n", filename);
    return -1;
  }  

  fprintf(fp, "%d\n", gain_map->cal_version);

  fprintf(fp, "%d\n", gain_map->width);     // from PDAF CAL version K and up 
  fprintf(fp, "%d\n", gain_map->height);    // gain map size is fixed to 17x13

  for (i = 0; i < gain_map->height*gain_map->width; i++)
  	fprintf(fp, "%d\n",gain_map->gm_l[i]);
  for (i = 0; i < gain_map->height*gain_map->width; i++)
  	fprintf(fp, "%d\n", gain_map->gm_r[i]);
  fclose(fp);

  return 0;
}


int write_cal_buf_txt_file(char *filename, pdaf_cal_t *pdaf_cal) 
{
  int i, j, cal_version, gm_height, gm_width, dm_height, dm_width, dm_q_format;
  int gm, dm;
  uint8_t checksum;

	FILE *fp = fopen(filename,"w");
  if( fp == NULL ) {
    printf("Cannot open file for write: %s\n", filename);
    return -1;
  }  

  // convert big-endian to little endian for windows
  cal_version = big_to_little(pdaf_cal->cal_version);
  gm_width    = big_to_little(pdaf_cal->gm_width); 
  gm_height   = big_to_little(pdaf_cal->gm_height);  
  
  // caliration version
  fprintf(fp, "cal version: %d\n", cal_version);

  // gain map
  fprintf(fp, "gm_width: %d\n", gm_width);
  fprintf(fp, "gm_height: %d\n", gm_height);
  fprintf(fp, "gm_left:\n");   
  for(i=0; i < gm_height; i++) {
    for(j=0; j < gm_width; j++ ) {
      gm = big_to_little(pdaf_cal->gm_l[i*gm_width + j]);
      fprintf(fp, "%d ", gm);       
    }
    fprintf(fp, "\n");       
  }
  fprintf(fp, "gm_right:\n");
  for (i = 0; i < gm_height; i++) {
    for (j = 0; j < gm_width; j++) {
      gm = big_to_little(pdaf_cal->gm_r[i*gm_width + j]);
      fprintf(fp, "%d ", gm);
    }
    fprintf(fp, "\n");
  }

  // dcc map q format
  dm_q_format = big_to_little(pdaf_cal->dm_q_format); 
  fprintf(fp, "dm_q_format: %d\n", dm_q_format);
 
  // dcc map
  dm_width  = big_to_little(pdaf_cal->dm_width);  
  dm_height = big_to_little(pdaf_cal->dm_height);  
  fprintf(fp, "dm_width: %d\n", dm_width);
  fprintf(fp, "dm_height: %d\n", dm_height);
  fprintf(fp, "dm:\n");
  for (i = 0; i < dm_height; i++) {
    for (j = 0; j < dm_width; j++) {
      dm = big_to_little(pdaf_cal->dm[i*dm_width + j]); 
      fprintf(fp, "%d ", dm);
    }
    fprintf(fp, "\n");
  }
  
  // checksum
  checksum = pdaf_cal->checksum;
  fprintf(fp, "checksum: %d\n", checksum);

  fclose(fp);

  return 0;
}

int write_cal_buf_bin_file(char *filename, char *buf, uint16_t size) 
{
  int i;
	FILE *fp = fopen(filename,"w");
  if( fp == NULL ) {
    printf("Cannot open file for write: %s\n", filename);
    return -1;
  }  

  for (i = 0; i < size; i++)
  	fprintf(fp, "checksum: %d\n",buf[i]);

  fclose(fp);

  return 0;
}


void get_dll_cfg(dll_config_t *dll_cfg )
{
  int8_t dcc_fir[]  = DCC_FIR;

  dll_cfg->cal_version         = CAL_VERSION;
  dll_cfg->chart_type          = LINECHART;
  dll_cfg->verify_type         = NORMALIZE;
  dll_cfg->gain_map_height     = GAIN_MAP_HEIGHT;
  dll_cfg->gain_map_width      = GAIN_MAP_WIDTH;
  dll_cfg->gain_map_q_fmt      = GAIN_MAP_Q_FMT;	
  dll_cfg->dcc_map_height      = DCC_MAP_HEIGHT;
  dll_cfg->dcc_map_width       = DCC_MAP_WIDTH;
  dll_cfg->dcc_map_q_fmt       = DCC_MAP_Q_FMT;
  memcpy(dll_cfg->dcc_fir, dcc_fir, sizeof(int8_t)*DCC_FIR_SZ);
  dll_cfg->dcc_fir_sz          = DCC_FIR_SZ;
  dll_cfg->dcc_stack_sz        = DCC_STACK_SZ;
  dll_cfg->dcc_search_range    = DCC_SEARCH_RANGE;
  dll_cfg->dcc_fit_exclusion   = DCC_FIT_EXCLUSION;
  dll_cfg->dcc_min_pd_range    = DCC_MIN_PD_RANGE;
  dll_cfg->dcc_map_overlap     = DCC_MAP_OVERLAP;
  dll_cfg->dpd_pixel_unit      = DPD_PIXEL_UNIT;
  dll_cfg->dpd_min_sensor_gain = DPD_MIN_SENSOR_GAIN;
  dll_cfg->dpd_max_sensor_gain = DPD_MAX_SENSOR_GAIN;
}

errno_t cal_dump_to_txt(char* cal_filepath, char* txt_filepath) {
  context_t context;
  FILE* fp = NULL;
  errno_t err = 0;
  float max_err = 0.0;
  int i, j;

  /***** read dump info from .cal file *****/
  err = fopen_s(&fp, cal_filepath, "rb");
  if (err != 0 || fp == NULL) {
    printf("Cannot open file for read: %s\n", cal_filepath);
    return err;
  }
  fread_s( &context, sizeof(context_t), sizeof(context_t), 1, fp);
  fclose(fp);
  fp = NULL;
  err = 0;

  /***** checking context information *****/
  err = fopen_s(&fp, txt_filepath, "w");
  if (err != 0 || fp == NULL) {
    printf("Cannot open file for write: %s\n", txt_filepath);
    return err;
  }

  /***** Calibration Tool Version and basic information *****/
  fprintf_s(fp, "----- DCC basic information: -----\n");
  fprintf_s(fp, "\tcontext version: %d\n", context.context_version);
  fprintf_s(fp, "\tchart_type: %s\n", context.chart_type ? "Diamond chart":"Line chart");
  fprintf_s(fp, "\tdcc_stack_cnt: %d\n", context.dcc_stack_cnt);
  fprintf_s(fp, "\tdcc_fit_cnt: %d\n", context.dcc_fit_cnt);
  fprintf_s(fp, "\tdcc_search_range: %d\n", context.dcc_search_range);
  fprintf_s(fp, "\tdcc_map_overlap: %G\n", context.dcc_map_overlap);
  fprintf_s(fp, "\tfv_measure_channel: %d\n", context.fv_measure_channel);
  for(j=0; j<context.dcc_map.height; j++) {
	  fprintf_s(fp, "\tdcc_val_tol(:, %d):",j);
	  for(i=0; i<context.dcc_map.width; i++) {
		fprintf_s(fp, "%8.4G", context.dcc_val_tol[j*context.dcc_map.width+i]);
	  }
	  fprintf_s(fp, "\n");
  }
  fprintf_s(fp, "\tDCC return value: 0x%x\n\n\n", context.dcc_map_rc);

  /***** Gain map information *****/
  fprintf_s(fp, "----- Gain map information: -----\n");
  fprintf_s(fp, "\tgain map size (%d, %d)\n", context.gain_map.width, context.gain_map.height);
  fprintf_s(fp, "\tLeft image gain map values:\n");
  for ( j=0 ; j<context.gain_map.height ; j++ ) {
	fprintf_s(fp, "\t\t");
    for ( i=0 ; i<context.gain_map.width ; i++ ) {
		int gain_map_idx = j*context.gain_map.width+i;
		fprintf_s(fp, "%d\t", context.gain_map.gm_l[gain_map_idx]);
	}
	fprintf_s(fp, "\n");
  }
  fprintf_s(fp, "\tRight image gain map values:\n");
  for ( j=0 ; j<context.gain_map.height ; j++ ) {
    fprintf_s(fp, "\t\t");
    for ( i=0 ; i<context.gain_map.width ; i++ ) {
      int gain_map_idx = j*context.gain_map.width+i;
      fprintf_s(fp, "%d\t", context.gain_map.gm_r[gain_map_idx]);
	}
	fprintf_s(fp, "\n");
  }
  fprintf_s(fp, "\n\n");

  /***** DCC map information *****/
  fprintf_s(fp, "----- DCC map information: -----\n");
  fprintf(fp, "\tDCC Q factor: %d\n", context.dcc_map.q_format);
  fprintf_s(fp, "\tDCC map size (%d, %d)\n", context.dcc_map.width, context.dcc_map.height);
  fprintf_s(fp, "\tDCC map values:\n");
  for ( j=0 ; j<context.dcc_map.height ; j++ ) {
    fprintf_s(fp, "\t\t");
	for ( i=0 ; i<context.dcc_map.width ; i++ ) {
		int dcc_idx = j*context.dcc_map.width+i;
		fprintf_s(fp, "%d\t", context.dcc_map.dm[dcc_idx]);
	}
	fprintf_s(fp, "\n");
  }
  fprintf_s(fp, "\n\n");

  /***** Lens Position Map Information *****/
  fprintf_s(fp, "----- Lens Position Map Information: -----\n");
  for(j=0; j<context.dcc_map.height; j++) {
    fprintf_s(fp, "\t                                        grid index: ", j);
	for(i=0; i<context.dcc_map.width; i++) {
      fprintf_s(fp, "   (%1d,%1d)", i, j);
    }
    fprintf_s(fp, "\n");

	fprintf_s(fp, "\t                    estimated focus position by FV: ");
    for(i=j*context.dcc_map.width; i<(j+1)*context.dcc_map.width; i++) {
      fprintf_s(fp, "%8d", context.lp_map.fv[i]);
    }
    fprintf_s(fp, "\n");

    fprintf_s(fp, "\t                    estimated focus position by PD: ");
    for(i=j*context.dcc_map.width; i<(j+1)*context.dcc_map.width; i++) {
      fprintf_s(fp, "%8d", context.lp_map.pd[i]);
    }
    fprintf_s(fp, "\n");

    fprintf_s(fp, "\t                       between estimated FV and PD: ");
    for(i=j*context.dcc_map.width; i<(j+1)*context.dcc_map.width; i++) {
      fprintf_s(fp, "%8d", context.lp_map.delta[i]);
    }
    fprintf_s(fp, "\n");

    fprintf_s(fp, "\tNormalized: difference between estimated FV and PD: ");
    for(i=j*context.dcc_map.width; i<(j+1)*context.dcc_map.width; i++) {
      fprintf_s(fp, "%8.3f", context.lp_map.ndelta[i]);
	  if (max_err < ABS(context.lp_map.ndelta[i]))
        max_err = ABS(context.lp_map.ndelta[i]);
    }
    fprintf_s(fp, "\n\n\n");
  }
  fprintf_s(fp, "\t**** MAX: Normalized: difference between estimated FV and PD: %8.3f\n\n\n", max_err);

  /*****  *****/
  int map_index = 0;
  int stack_index = 0;
  fprintf_s(fp, "----- Stack Map Information: -----\n");	
  for(map_index=0; map_index<context.dcc_map.width*context.dcc_map.height; map_index++) {
    fprintf_s(fp, "DCC grid cell index: (%d, %d)\n", map_index%context.dcc_map.width, map_index/context.dcc_map.width);

	fprintf_s(fp, "RAW lens position: \t");
	for(stack_index=0; stack_index<context.dcc_stack_cnt; stack_index++) {
      fprintf_s(fp, "%9d\t", context.stack_map[stack_index].lens_pos);
	}
	fprintf_s(fp, "\n");

	fprintf_s(fp, "Phrase disparity: \t");
	for(stack_index=0; stack_index<context.dcc_stack_cnt; stack_index++) {
      fprintf_s(fp, "%9.3f\t", context.stack_map[stack_index].pd_map[map_index]);
    }
	fprintf_s(fp, "\n");

    fprintf_s(fp, "      Focus value: \t");
    for(stack_index=0; stack_index<context.dcc_stack_cnt; stack_index++) {
      fprintf_s(fp, "%9.0f\t", context.stack_map[stack_index].fv_map[map_index]);
    }
	fprintf_s(fp, "\n");

	fprintf_s(fp, "    PD confidence: \t");
    for(stack_index=0; stack_index<context.dcc_stack_cnt; stack_index++) {
      fprintf_s(fp, "%9d\t", context.stack_map[stack_index].cf_map[map_index]);
    }
	fprintf_s(fp, "\n");

	fprintf_s(fp, "     Return error: \t");
    for(stack_index=0; stack_index<context.dcc_stack_cnt; stack_index++) {
      fprintf_s(fp, "%9d\t", context.stack_map[stack_index].rc_map[map_index]);
    }
	fprintf_s(fp, "\n\n\n");
  }
  fclose(fp);

  return err;
}