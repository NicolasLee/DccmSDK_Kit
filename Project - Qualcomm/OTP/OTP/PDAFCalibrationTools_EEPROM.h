#ifndef PDAFCALIBRATION_TOOL_EEPROM_H
#define PDAFCALIBRATION_TOOL_EEPROM_H

#define CAL_VERSION 4


typedef struct _pdaf_cal_v4_t 
{
  uint16_t cal_version;
  uint16_t gm_width;     // 17
  uint16_t gm_height;    // 13
  uint16_t gm_l[221];    // 13x17
  uint16_t gm_r[221];    // 13x17
  uint16_t dm_q_format;  
  uint16_t dm_width;     // 8
  uint16_t dm_height;    // 6
  uint16_t dm[48];       // 6x8
  uint8_t  checksum; 
} pdaf_cal_v4_t;

typedef pdaf_cal_v4_t pdaf_cal_t;

#endif


