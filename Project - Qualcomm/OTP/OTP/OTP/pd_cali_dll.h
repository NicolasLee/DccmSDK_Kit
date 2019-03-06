// pd_cali_dll.h
#ifndef __PD_CALI_DLL_H__
#define __PD_CALI_DLL_H__

////////////////////////////////////////
/// DLL header
////////////////////////////////////////
#ifdef PD_CALI_DLL_EXPORTS
#define PD_CALI_DLL_API __declspec(dllexport) 
#else
#define PD_CALI_DLL_API __declspec(dllimport) 
#endif

typedef enum
{
	PD_ERR_OK = 0,
	PD_ERR_FILE_NOT_FOUND,
	PD_ERR_INIT_FAIL,
	PD_ERR_INVALID_PARAM,
	PD_ERR_OUTPUT_MEM_NOT_SUFFICIENT,
	PD_ERR_NOT_INITIALIZED,
	PD_ERR_INPUT_DATA_VERIFIED_FAIL,
	PD_ERR_OUTPUT_DATA_VERIFIED_FAIL,
	PD_ERR_QUALITY_VERIFIED_FAIL,
	PD_ERR_GENERAL_ERROR
} PD_ERR_CODE;

PD_CALI_DLL_API PD_ERR_CODE pd_cali_init(char *filename);
PD_CALI_DLL_API PD_ERR_CODE pd_cali_get_max_output_size(int& max_size);
PD_CALI_DLL_API PD_ERR_CODE pd_cali_get_capture_num(int& num);
PD_CALI_DLL_API PD_ERR_CODE pd_cali_get_raw_width_height(int& width, int& height);
PD_CALI_DLL_API PD_ERR_CODE pd_cali_get_data_format_version(unsigned char *eeprom_buf, int& version);
PD_CALI_DLL_API PD_ERR_CODE pd_cali_proc1(unsigned short*raw, char *output, int max_output_size, int& output_size);
PD_CALI_DLL_API PD_ERR_CODE pd_cali_proc2(char **raw_seq, int *lens_pos, int num, char* output, int max_output_size, int& output_size);
PD_CALI_DLL_API PD_ERR_CODE pd_cali_verify(unsigned char *eeprom_buf);
PD_CALI_DLL_API PD_ERR_CODE pd_cali_verify_proc1(unsigned char *eeprom_buf_proc1);
PD_CALI_DLL_API PD_ERR_CODE pd_cali_verify_proc2(unsigned char *eeprom_buf_proc2);
PD_CALI_DLL_API PD_ERR_CODE pd_cali_get_DAC_proc2(unsigned char *eeprom_buf_proc2, int& DAC);
PD_CALI_DLL_API PD_ERR_CODE pd_cali_get_linearity(unsigned char *eeprom_buf_proc2, double* out_linearity);
PD_CALI_DLL_API PD_ERR_CODE pd_cali_get_linearity_size(unsigned char *eeprom_buf_proc2, int& sizeX, int& sizeY);
#endif // #define __PDCALIDLL_H__