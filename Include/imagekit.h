#ifndef __IMAGEKIT_H__
#define __IMAGEKIT_H__


/**************************************************************************************** 
*
* Imagekit series of various hardware version definitions
*
****************************************************************************************/

#define VER_HS100   0x1000
#define VER_HS200   0x1020

#define VER_HS120   0x1020 
#define VER_HS128	0x1200
#define VER_HS230   0x1230

#define VER_HS280	0x1280

#define VER_HS130	0x1300
#define VER_HS300   0x1330
#define VER_HS320   0x1332

#define VER_HV810   0x1380
#define VER_HV820   0x1382
#define VER_HV910   0x1390
#define VER_HV920   0x1392

/**************************************************************************************** 
*
* Initialization of SENSOR and Control Related
*
****************************************************************************************/

typedef struct _SensorTab
{
	/// @brief SENSOR width
	USHORT width;          /// < SENSOR width
	/// @brief SENSOR height
	USHORT height;         /// < SENSOR height
	/// @brief SENSOR data type
	BYTE type;             ///<SENSOR data type
	/// @brief SENSOR RESET and PWDN pin setting
	BYTE pin;              ///<SENSOR RESET and PWDN pin setting
	/// @brief SENSOR device address
	BYTE SlaveID;         ///< the device address of the SENSOR
	/// @brief SENSOR's I2C mode
	BYTE mode;            ///<SENSOR's I2C mode
	/// @brief SENSOR marker register 1.
	USHORT FlagReg;        ///<SENSOR flag register 1.
	/// @brief SENSOR flag register 1
	USHORT FlagData;       ///< the value of SENSOR flag register 1
	/// @brief SENSOR flag register 1 mask value
	USHORT FlagMask;	   ///< mask value of SENSOR flag register 1
	/// @brief SENSOR marker register 2.
	USHORT FlagReg1;	   ///<SENSOR flag register 2.
	/// @brief SENSOR flag register 2
	USHORT FlagData1;	   ///< the value of SENSOR flag register 2
	/// @brief SENSOR flag register 2 mask value
	USHORT FlagMask1;	   ///< mask value of SENSOR flag register 2
	/// @ name of brief SENSOR
	char name [64];       /// < name of the SENSOR

	/// @brief initializes the SENSOR data table
	USHORT * ParaList;		///< initialize SENSOR data table
	/// @brief initialize SENSOR data table size, unit byte
	USHORT ParaListSize;	///< initialize SENSOR data table size, unit byte
	/// @brief SENSOR enters the parameter table of Sleep mode
	USHORT * SleepParaList;	  ///< the SENSOR enters the parameter table of Sleep mode
	/// @brief SENSOR enters the parameter table size of Sleep mode, unit byte
	USHORT SleepParaListSize;  ///<SENSOR into Sleep mode parameter table size, unit byte

	/// @brief SENSOR output data format, YUV//0:YCbYCr;/ / 1: YCrYCb;/ / 2: CbYCrY;/ / 3: CrYCbY.
	BYTE outformat;		///<SENSOR output data format, YUV//0:YCbYCr;/ / 1: YCrYCb;/ / 2: CbYCrY;/ / 3: CrYCbY.
	/// @brief SENSOR's input clock MCLK, 0:12M;M unto them;Chief of M.
	int mclk;	///<SENSOR's input clock MCLK, 0:12M;M unto them;Chief of M.
	/// @brief SENSOR's AVDD voltage value
	BYTE avdd;		///< the SENSOR's AVDD voltage value
	/// @brief SENSOR's DOVDD voltage value
	BYTE dovdd;		///<SENSOR's DOVDD voltage value
	/// @ DVDD voltage of brief SENSOR
	BYTE dvdd;		///< the SENSOR's DVDD voltage value
	/// @brief SENSOR data interface type
	BYTE port;		///< data interface type of SENSOR
	USHORT Ext0;
	USHORT Ext1;
	USHORT Ext2; 

	/// @brief AF initialization parameter list
	USHORT * AF_InitParaList;///<AF initialization parameter table
	/// @brief AF initialization parameter table size, unit byte
	USHORT AF_InitParaListSize;///<AF initialization parameter table size, unit byte
	/// @brief AF_AUTO parameter table
	USHORT * AF_AutoParaList;/// < AF_AUTO parameter table
	/// @brief AF_AUTO parameter table size, unit byte
	USHORT AF_AutoParaListSize;///<AF_AUTO parameter table size, unit byte
	/// @brief AF_FAR parameter table
	USHORT * AF_FarParaList;/// < AF_FAR parameter table
	/// @brief AF_FAR parameter table size, unit bytes
	USHORT AF_FarParaListSize;///<AF_FAR parameter table size, unit bytes

	/// @brief AF_NEAR parameter table
	USHORT * AF_NearParaList;/// < AF_NEAR parameter table
	/// @brief AF_NEAR parameter table size, unit byte
	USHORT AF_NearParaListSize;///<AF_NEAR parameter table size, unit byte
	/// @brief exposure parameter table
	USHORT * Exposure_ParaList;///< exposure parameter table
	/// @brief exposure parameter table size, unit byte
	USHORT Exposure_ParaListSize;///< exposure parameter table size, unit byte
	/// @brief gain parameters table
	USHORT * Gain_ParaList;///< gain parameter table
	/// @brief gain parameter table size, unit byte
	USHORT Gain_ParaListSize;///< gain parameter table size, unit byte

	_SensorTab()
	{
		width=0;
		height=0;
		type=0;
		pin=0;
		SlaveID=0;
		mode=0;
		FlagReg=0;
		FlagData=0;
		FlagMask=0;
		FlagReg1=0;
		FlagData1=0;
		FlagMask1=0;
		memset(name,0,sizeof(name));

		ParaList=NULL;
		ParaListSize=0;
		SleepParaList=NULL;
		SleepParaListSize=0;

		outformat=0;
		mclk=0;               //0:12M; 1:24M; 2:48M.
		avdd=0;               // 
		dovdd=0;              //
		dvdd=0;

		port=0; 	
		Ext0=0;
		Ext1=0;
		Ext2=0; 

		AF_InitParaList=NULL;        //AF_InitParaList
		AF_InitParaListSize=0;

		AF_AutoParaList=NULL;
		AF_AutoParaListSize=0;

		AF_FarParaList=NULL;
		AF_FarParaListSize=0;

		AF_NearParaList=NULL;
		AF_NearParaListSize=0;

		Exposure_ParaList=NULL;      // exposure
		Exposure_ParaListSize=0;

		Gain_ParaList=NULL;          // gain
		Gain_ParaListSize=0;
	}
}SensorTab, *pSensorTab;

// following SensorTab, add the SensorTab2 definition
///////////////////////////////////////////////////////////
typedef struct _SensorTab2
{
	/// @brief SENSOR width
	UINT width;		/// < SENSOR width
	/// @brief SENSOR height
	UINT height;	/// < SENSOR height
	UINT Quick_w;	/// < the Quick View width
	UINT Quick_h;	/// < the Quick View level
	/// @brief SENSOR data type
	UINT type;	///<SENSOR data type
	/// @brief SENSOR RESET and PWDN pin setting
	UINT pin;	///<SENSOR RESET and PWDN pin setting
	/// @brief SENSOR device address
	UINT SlaveID;	///< the device address of the SENSOR
	/// @brief SENSOR's I2C mode
	UINT mode;		///<SENSOR's I2C mode
	/// @brief SENSOR marker register 1.
	UINT FlagReg;		///<SENSOR flag register 1.
	/// @brief SENSOR flag register 1
	UINT FlagData;		///< the value of SENSOR flag register 1
	/// @brief SENSOR flag register 1 mask value
	UINT FlagMask;		///< mask value of SENSOR flag register 1
	/// @brief SENSOR marker register 2.
	UINT FlagReg1;		///<SENSOR flag register 2.
	/// @brief SENSOR flag register 2
	UINT FlagData1;		///< the value of SENSOR flag register 2
	/// @brief SENSOR flag register 2 mask value
	UINT FlagMask1;		///< mask value of SENSOR flag register 2
	/// @ name of brief SENSOR
	char name [64];		/// < name of the SENSOR

	/// @brief initializes the SENSOR data table
	UINT * ParaList;	///< initialize SENSOR data table
	/// @brief initialize SENSOR data table size, unit byte
	UINT ParaListSize;	///< initialize SENSOR data table size, unit byte
	/// @brief SENSOR enters the parameter table of Sleep mode
	UINT * SleepParaList;	///< the SENSOR enters the parameter table of Sleep mode
	/// @brief SENSOR enters the parameter table size of Sleep mode, unit byte
	UINT SleepParaListSize;	///<SENSOR into Sleep mode parameter table size, unit byte

	/// @brief SENSOR enters the parameter table of Quick View mode
	UINT * QuickParaList;	///<SENSOR enters the parameter table of the Quick View mode
	/// @brief SENSOR enters the parameter table size of Quick View mode, unit byte
	UINT QuickParaListSize;	///<SENSOR into Quick View mode parameter table size, unit bytes

	/// @brief SENSOR output data format, YUV//0:YCbYCr;/ / 1: YCrYCb;/ / 2: CbYCrY;/ / 3: CrYCbY.
	UINT outformat;		///<SENSOR output data format, YUV//0:YCbYCr;/ / 1: YCrYCb;/ / 2: CbYCrY;/ / 3: CrYCbY.
	/// @brief SENSOR's input clock MCLK, 0:12M;M unto them;Chief of M.
	UINT mclk;		///<SENSOR's input clock MCLK, 0:12M;M unto them;Chief of M.
	/// @brief SENSOR's AVDD voltage value
	UINT avdd;	///< the SENSOR's AVDD voltage value
	/// @brief SENSOR's DOVDD voltage value
	UINT dovdd;	///<SENSOR's DOVDD voltage value
	/// @ DVDD voltage of brief SENSOR
	UINT dvdd;	///< the SENSOR's DVDD voltage value
	/// @brief SENSOR data interface type
	UINT port;	///< data interface type of SENSOR
    UINT Ext0;
	UINT Ext1;
	UINT Ext2; 

	/// @brief AF initialization parameter list
	UINT * AF_InitParaList;	///<AF initialization parameter table
	/// @brief AF initialization parameter table size, unit byte
	UINT AF_InitParaListSize;	///<AF initialization parameter table size, unit byte
	/// @brief AF_AUTO parameter table
	UINT * AF_AutoParaList;	/// < AF_AUTO parameter table
	/// @brief AF_AUTO parameter table size, unit byte
	UINT AF_AutoParaListSize;	///<AF_AUTO parameter table size, unit byte
	/// @brief AF_FAR parameter table
	UINT * AF_FarParaList;	/// < AF_FAR parameter table
	/// @brief AF_FAR parameter table size, unit bytes
	UINT AF_FarParaListSize;	///<AF_FAR parameter table size, unit bytes

	/// @brief AF_NEAR parameter table
	UINT * AF_NearParaList;	/// < AF_NEAR parameter table
	/// @brief AF_NEAR parameter table size, unit byte
	UINT AF_NearParaListSize;	///<AF_NEAR parameter table size, unit byte
	/// @brief exposure parameter table
	UINT * Exposure_ParaList;	///< exposure parameter table
	/// @brief exposure parameter table size, unit byte
	UINT Exposure_ParaListSize;	///< exposure parameter table size, unit byte
	/// @brief gain parameters table
	UINT * Gain_ParaList;	///< gain parameter table
	/// @brief gain parameter table size, unit byte
	UINT Gain_ParaListSize;	///< gain parameter table size, unit byte

	_SensorTab2()
	{
		width=0;
		height=0;
		Quick_w = 0; //20141031
		Quick_h = 0; //20141031
		type=0;
		pin=0;
		SlaveID=0;
		mode=0;
		FlagReg=0;
		FlagData=0;
		FlagMask=0;
		FlagReg1=0;
		FlagData1=0;
		FlagMask1=0;
		memset(name,0,sizeof(name));

		ParaList=NULL;
		ParaListSize=0;
		SleepParaList=NULL;
		SleepParaListSize=0;

		QuickParaList = NULL; //20141031
		QuickParaListSize = 0; //20141031

		outformat = 0;
		mclk= 0;               //0:12M; 1:24M; 2:48M.
		avdd= 0;               // 
		dovdd = 0;             //
		dvdd = 0;

		port=0; 	
		Ext0=0;
		Ext1=0;
		Ext2=0; 

		AF_InitParaList=NULL;     //AF_InitParaList
		AF_InitParaListSize=0;

		AF_AutoParaList=NULL;
		AF_AutoParaListSize=0;

		AF_FarParaList=NULL;
		AF_FarParaListSize=0;

		AF_NearParaList=NULL;
		AF_NearParaListSize=0;

		Exposure_ParaList=NULL;    // exposure
		Exposure_ParaListSize=0;

		Gain_ParaList=NULL;        //gain
		Gain_ParaListSize=0;
	}
}SensorTab2, *pSensorTab2;



/** @defgroup group2 ISP related
@{

*/
/** @name SENSOR output image type definition (value definition of SensorTab::type)
@{

*/
/* SENSOR output image type definition (value definition of SensorTab::type) */
#define D_RAW10				0x00 
#define D_RAW8				0x01
#define D_MIPI_RAW8			0x01
#define D_YUV				0x02 
#define D_RAW16				0x03
#define D_MIPI_RAW16		0x03
#define D_RGB565			0x04
#define D_YUV_SPI			0x05
#define D_MIPI_RAW10		0x06    ///<5bytes = 4 pixel...
#define D_MIPI_RAW12		0x07    ///<3bytes = 2 pixel...
#define D_RAW12				0x07
#define D_YUV_MTK_S			0x08    //MTK output...
#define D_YUV_10			0x09
#define D_YUV_12			0x0a

#define D_MIPI_RAW14		0x0b    ///<7bytes = 4 pixel...

#define D_BGR24             0x20    ///< order: B, G, R, each 8bit
#define D_BGR32             0x21    ///<order: B, G, R, 0, each 8bit
#define D_P10               0x24    ///<a pixel takes up two bytes, LSB, 0 ~ 1023, generally used for MIPI_RAW10 conversion
#define D_P12               0x25    ///<a pixel takes up two bytes, LSB, 0 ~ 4095, commonly used for MIPI_RAW12 conversion

#define D_G8                0x28
#define D_G10               0x29
#define D_GRAY8				0x2a
//#define D_RGB24             0x0b
//#define D_HISPI_SP			0x09    //aptina hispi packet sp.
/** @} */

/** @name RAW to RGB algorithm definition
@ {

* /
/* RAW to RGB algorithm definition */
#define RAW2RGB_NORMAL			0
#define RAW2RGB_SMOOTH			1
#define RAW2RGB_SHARP			2
/* *@} * /
/** @name YUV image 4 output format definitions
@ {

* /
/* RAW, YUV images 4 output format definitions (SensorTab::outformat value definitions) */
/// YUV image 4 output format definitions.
enum OUTFORMAT_YUV
{
	OUTFORMAT_YCbYCr = 0,///<YCbYCr output format
	OUTFORMAT_YCrYCb,///<YCrYCb output format
	OUTFORMAT_CbYCrY,///<CbYCrY output format
	OUTFORMAT_CrYCbY,///<CrYCbY output format
};
/** @} */

/** @name RAW image 4 output format definitions
@ {

* /
/// RAW image 4 output format definitions.
enum OUTFORMAT_RGB
{
	OUTFORMAT_RGGB = 0,///<RGGB output format
	OUTFORMAT_GRBG,///<GRBG output format
	OUTFORMAT_GBRG,///<GBRG output format
	OUTFORMAT_BGGR,///<BGGR output format
};
/** @} */

/* RAW and YUV formats supported by this system are defined */
/** @name supports RAW format definitions
@ {

*/
/// supported RAW format.
enum RAW_FORMAT
{
	RAW_RGGB = 0,	    ///<RAW arrange by RGGB
	RAW_GRBG,			///<RAW arrange by GRBG
	RAW_GBRG,			///<RAW arrange by GBRG
	RAW_BGGR,			///<RAW arrange by BGGR
};
/** @} */

/** @name YUV format definition supported by
@{

*/
/// supported YUV format.
enum YUV_FORMAT
{
	YUV_YCBYCR = 0,  ///<YUV format by YCBYCR
	YUV_YCRYCB,		 ///<YUV format by YCRYCB
	YUV_CBYCRY,		 ///<YUV format by CBYCRY
	YUV_CRYCBY,		 ///<YUV format by CRYCBY
};
/** @} */

/** @name image processing mode selection
@{

*/
///image processing mode selection
enum ISP_MODE
{
	NORMAL = 0, /// NORMAL processing mode. The raw data is obtained by grabbing the frame
	S2DFAST,    ///S2DFAST mode, the frame capture is RGB data
	S2DFAST_GPU,///S2DFAST_GPU mode, GPU for image processing, RGB data is obtained by grabbing frames
};

/** @} */

enum IMAGE_FORMAT
{
    FORMAT_RAW10 = 0x00, 
    FORMAT_RAW8 = 0x01,
    FORMAT_YUV = 0x02, 
    FORMAT_RAW16 = 0x03,
    FORMAT_RGB565 = 0x04,
    FORMAT_YUV_SPI	= 0x05,
    FORMAT_MIPI_RAW10 = 0x06,           // 5bytes = 4 pixel...
    FORMAT_MIPI_RAW12 = 0x07,           // 3bytes = 2 pixel...
    FORMAT_YUV_MTK_S = 0x08,            // MTK output...
    FORMAT_YUV_10 = 0x09,
    FORMAT_YUV_12 = 0x0a,

    FORMAT_BGR24 = 0x20,                // the order is B, G, R, 8 bits each
    FORMAT_BGR32 = 0x21,                // order B, G, R, 0, 8 bits each
    FORMAT_P10 = 0x24,                  // a pixel takes up two bytes, LSB, 0 ~ 1023, commonly used for MIPI_RAW10 conversion
    FORMAT_P12 = 0x25,                  // a pixel takes up two bytes, LSB, 0 ~ 4095, commonly used for MIPI_RAW12 conversion
    FORMAT_G8 = 0x28,                   //G value only, 8 bit
    FORMAT_G10 = 0x29,                  // G value, only 16 bit
	FORMAT_GRAY8 = 0x2a,
};


typedef struct DtRoi_s
{
    UINT            x;		    // ROI starting point X coordinate value
    UINT            y;	        // ROI starting point Y coordinate value
    UINT            w;		    // ROI width
    UINT            h;		    // ROI height
}DtRoi_t;

typedef struct DtImage_s 
{
    IMAGE_FORMAT    format;      // image format
    RAW_FORMAT      rawFmt;      // RAW format details
    YUV_FORMAT      yuvFmt;      // YUV format details
    UINT            width;       // image size
    UINT            height;	     // image size
    BYTE            *data;       // image data
    unsigned int    dataSize;    // buffer space size
    UINT            resv[8];     // save 32 bytes
}DtImage_t;


/** @} */ // end of group2


/** @defgroup group3 SENSOR related


* @{

*/

/** @name the control word definition included in the SENSOR register parameter table
@{

*/
/* Control word definitions included in the SENSOR register parameter table */
#define DTDELAY				0xffff
#define DTMODE				0xfffe	
#define DTOR				0xfffd
#define DTAND				0xfffc
#define DTPOLLT				0xfffb
#define DTPOLL1				0xfffa
#define DTPOLL0				0xfff9
#define DTI2CADDR			0xfff8
#define DTI2CREG			0xfff7
#define DTAFTYPE			0xfff6  //20121223 added... modify the AF Device type
#define DTAFADDR			0xfff5  //20121223 added... modify the AF Device Address..	
#define DTSPIMTKCTRL		0xfff4
#define DTEND				0xfeff	
#define DTMACRO_ON			0xfef0
#define DTMACRO_OFF			0xfef1 
#define DTSPIMTKCTRL		0xfff4

// XHS
// XVS
// V_START
// V_END
// H_START
// H_END
/** @} */

/** @name SENSOR power supply voltage selection definition (the definition used by DTCCM)
@{

*/
/* SENSOR power supply voltage select definition */
#define AVDD_28				0x00
#define AVDD_25				0x01
#define AVDD_18				0x02
#define AVDD_DEFAULT		0x03

#define DOVDD_28			0x00
#define DOVDD_25			0x01
#define DOVDD_18			0x02
#define DOVDD_DEFAULT		0x03

#define DVDD_18				0x00
#define DVDD_15				0x01
#define DVDD_12				0x02
#define DVDD_DEFAULT		0x03

#define AFVCC_33			0x00
#define AFVCC_28			0x01
#define AFVCC_18			0x02
#define AFVCC_DEFAULT		0x03
/** @} */

/** @name SENSOR input clock select definition
@{

*/
/* SENSOR input clock select definition */
//you can use these enum type ,or use MHZ or hundred KHZ directly 
enum MCLKOUT
{
	MCLK_6M = 0,
	MCLK_8M,
	MCLK_10M,
	MCLK_11M4,
	MCLK_12M,
	MCLK_12M5,
	MCLK_13M5,
	MCLK_15M,
	MCLK_18M,
	MCLK_24M,
	MCLK_25M,
	MCLK_27M,
	MCLK_30M,
	MCLK_32M,
	MCLK_36M,
	MCLK_40M,
	MCLK_45M,
	MCLK_48M,
	MCLK_50M,
	MCLK_60M,
	MCLK_DEFAULT,
};
/** @} */

/** @name multi-sensor module channel definition (currently only CHANNEL_B exists in DTLC2)
@{

*/
/* multi-sensor module channel definition (currently only CHANNEL_B exists in DTLC2/UH920) **/
#define CHANNEL_A					0x01	// only use A channel
#define CHANNEL_B					0x02	// just use the B channel
#define CHANNEL_AB					0x03	// AB channel is used simultaneously
/** @} */ 

/** @name In the SensorEnable function, RESET/PWDN level status definition when enable SENSOR
@{

*/
/* In SensorEnable function, RESET/PWDN pin level status definition  when enabling SENSOR */
#define RESET_H						0x02
#define RESET_L						0x00
#define PWDN_H						0x01
#define PWDN_L						0x00
#define PWDN2_H						0x04
#define PWDN2_L						0x00
/** @} */

/** @name supported SENSOR data interface definition
@{

*/
/// define the supported SENSOR data interface
typedef enum
{
	SENSOR_PORT_MIPI = 0,	///<MIPI interface
	SENSOR_PORT_PARA,		///< parallel synchronization interface
	SENSOR_PORT_MTK_SERIAL, ///< serial interface of MTK company
	SENSOR_PORT_SPI,		/// < the SPI interface
	SENSOR_PORT_SIM,		///< simulated image for testing
	SENSOR_PORT_HISPI,		///<Aptina's HISPI interface supports packet sp format
	SENSOR_PORT_ZX_SERIAL,	///< spread-over serial interface

	SENSOR_PORT_SONY_LVDS = 0x81,		 // SONY LVDS
	SENSOR_PORT_SMARTSENS_LVDS = 0x82,	 // xway LVDS
	SENSOR_PORT_PANASONIC_LVDS = 0x85, 	 // panasonic LVDS
	SENSOR_PORT_SUPERPIX_LVDS = 0x86,	 // sybiko LVDS
}SENSOR_PORT;
/** @} */

/** @name Macro definition of earlier version
@{

*/
/* Macro definition used in earlier version */
#define PORT_MIPI			0   //MIPI output
#define PORT_PARALLEL		1   //Parallel output
#define PORT_MTK			2   //MTK output
#define PORT_SPI			3   //SPI output
#define PORT_TEST			4   //TEST ouput. FPGA output the image...
#define PORT_HISPI			5   //aptina HISPI packet sp...
#define PORT_ZX2_4			6   //zhanxun 2bit/4bit packet sp...
#define PORT_MAX			7   //maxium... can't support >=PORT_MAX
/** @} */

#define PORT_SONY_LVDS		0x81 
#define PORT_PANASONIC		0x85  
/** @name defines various pin functions in the flexible interface
@{

*/
/* define various pin functions in flexible interfaces */
typedef enum
{
	PIN_D0 = 0,
	PIN_D1,
	PIN_D2,
	PIN_D3,
	PIN_D4,
	PIN_D5,
	PIN_D6,
	PIN_D7,
	PIN_D8,
	PIN_D9,
	PIN_PCLK,
	PIN_HSYNC,
	PIN_VSYNC,
	PIN_MCLK,
	PIN_RESET,
	PIN_PWDN,
	PIN_PWDN2,
	PIN_GPIO1,
	PIN_SDA,
	PIN_SCL,
	PIN_NC,
	PIN_GPIO2,
	PIN_GPIO3,
	PIN_GPIO4,
	PIN_NC1,
	PIN_NC2,
	PIN_D10,
	PIN_D11,
	PIN_SPI_SCK,	// flexible interface    new SPI interface
	PIN_SPI_CS,
	PIN_SPI_SDI,
	PIN_SPI_SDO,
	PIN_SPI_SDA,
	PIN_CLK_ADJ_200K,
	PIN_CLK_ADJ_18M,
	PIN_GPIO5,
	PIN_GPIO6,
	PIN_GPIO7,
	PIN_GPIO8,
    PIN_FRAME_UPDATE,
    PIN_D12,
    PIN_D13,
    PIN_D14,
    PIN_D15,
}PIN_FUNC;
/** @} */

/** @Name definition flexible interface pin name (number)
@{

*/
/* Name definition flexible interface pin name (number) */
typedef enum
{
	PIN_IO1 = 0,
	PIN_IO2,
	PIN_IO3,
	PIN_IO4,
	PIN_IO5,
	PIN_IO6,
	PIN_IO7,
	PIN_IO8,
	PIN_IO9,
	PIN_IO10,
	PIN_IO11,
	PIN_IO12,
	PIN_IO13,
	PIN_IO14,
	PIN_IO15,
	PIN_IO16,
	PIN_IO17,
	PIN_IO18,
	PIN_IO19,
	PIN_IO20,
	PIN_IO21,
	PIN_IO22,
	PIN_IO23,
	PIN_IO24,
	PIN_IO25,
	PIN_IO26,
}SOFT_PIN;
/** @} */


/* MIPI CTRL extension structure */
typedef struct MipiCtrlEx_s
{
	BYTE			byPhyType;						  /// mipi phy set (d-phy_1.5G/d-phy_2.5G,c-phy_2.0G

	BYTE			byLaneCnt;						  ///lane number setting, 1/2/4lane 

	DWORD			dwCtrl;							  /// MIPI ctrl

	UINT            uVc;								///sets the received image channel number, 0/1/2/3

	BOOL            bVCFilterEn;					  /// enables filtering of other virtual channels

	UINT            uPackID;						 ///the ID number that enables the output

	int             bPackIDEn;						  ///enables the ID number currently set to output

	BYTE			resv[62];

}MipiCtrlEx_t;

/** @name MIPI controller's bitwise definition
@ {

* /
/* bit definition of MIPI controller features */
#define MIPI_CTRL_LP_EN				1	    ///<allows to enter the LP state
#define MIPI_CTRL_AUTO_START		(1<<1)	///<automatically starts after the differential signal appears for OS testing
#define MIPI_CTRL_NON_CONT			(1<<2)	///<use the discontinuous clock
#define MIPI_CTRL_FULL_CAP			(1<<3)	///< complete packet acquisition, including header and CRC16 validation, results in an increase of 6 bytes per line of image data
#define MIPI_CTRL_CLK_LP_CHK		(1<<4)	///<the LP state of CLK Lane is tested. It is mandatory that CLK Lane at MIPI TX terminal must enter the LP state once
/** @} */

/** @name MIPI Phy type selection
@ {

* /
/* MIPI Phy type select */
#define	MIPI_DPHY_1_5G				0	    ///< DPHY selection without deskew calibration below 1.5g
#define MIPI_DPHY_2_5G				1		///< DPHY selection with deskew calibration above 1.5g
#define MIPI_CPHY					2		///< CPHY select

/** @name synchronizes the bit definition of the parallel interface feature
@{

*/
/* bit definition of synchronous parallel interface features */
#define PARA_PCLK_RVS				(1<<3)	///< PCLK is reversed
#define PARA_VSYNC_RVS				(1<<4)	///< VSYNC inverts
#define PARA_HSYNC_RVS				(1<<5)	///<  HSYNC inverts
#define PARA_AUTO_POL				(1<<6)	///<  VSYNC,HSYNC polarity automatic recognition
/** @} */

/** @name synchronous parallel interface,3bit used to select bit width
@{

*/
/* 3bit is used to select bit width */
#define PARA_BW_8BIT				0		
#define PARA_BW_10BIT				1
#define PARA_BW_12BIT				2
#define PARA_BW_16BIT				3
/** @} */

/** @name HiSPI interface feature bit definition
@{

*/
/* bit definition of HiSPI interface features */
// 2 bits are used to select bit width
#define	HISPI_WW_10BIT				0
#define	HISPI_WW_12BIT				1
#define	HISPI_WW_14BIT				2
#define	HISPI_WW_16BIT				3
/** @} */

/** @name bitwise definition of simulated image module feature
@{

*/
/* bit definition of analog image module features */
// 2 bits are used to select the style of the simulated image
#define	SIM_STYLE1					0	 ///< output fixed color
#define	SIM_STYLE2					1	 ///<  horizontal gradient
#define	SIM_STYLE3					2	 ///<  vertical gradient
#define	SIM_STYLE4					3	 ///<  per frame gradient
/** @} */

/**************************************************************************************** 
*
* I2C bus related
*
****************************************************************************************/
/*
* Common register read-write mode
* I2C mode definiton
* when read or write by I2c ,should use this definiton...
* Normal Mode:8 bit address,8 bit data,
* Samsung Mode:8 bit address,8 bit data,but has a stop between slave ID and addr...
* Micron:8 bit address,16bit data...
* Stmicro:16bit addr ,8bit data,such as eeprom and stmicro sensor...
*/

/** @name I2C schema definition
@{

*/
///I2C schema definition.
enum I2CMODE
{
	I2CMODE_NORMAL=0,		///< 8 bit addr,8 bit value 
	I2CMODE_SAMSUNG,		///< 8 bit addr,8 bit value,Stopen
	I2CMODE_MICRON,			///< 8 bit addr,16 bit value
	I2CMODE_STMICRO,		///< 16 bit addr,8 bit value, (eeprom also)
	I2CMODE_MICRON2,		///< 16 bit addr,16 bit value
};
/// the sensor i2c pull-up resistor is selected
enum I2CPULLUPRESISTOR
{
	PULLUP_RESISTOR_1_5K=0,		///< 1.5K pull up resistor
	PULLUP_RESISTOR_4_7K,		///< 4.7K pull up resistor
	PULLUP_RESISTOR_0_88K,		///< 0.88K pull up resistor
};

/** @} */

/** @name SPI schema definition
@{

*/
///SPI schema definition
enum SPIMODE
{
	SPIMODE_SONY_A1_D1=0x81,	///< 8 bit addr,8 bit value 
	SPIMODE_SONY_A1_D2,			///< 8 bit addr,16 bit value
	SPIMODE_SONY_A2_D1,			///< 16 bit addr,8 bit value
	SPIMODE_SONY_A2_D2,			///< 16 bit addr,16 bit value

	SPIMODE_40KFPS_A1_D1=0x88,

	/* panasonic lsb */
	SPIMODE_PANASONIC_A1_D1=0x91,		///< 8 bit addr,8 bit value 
	SPIMODE_PANASONIC_A1_D2,			///< 8 bit addr,16 bit value
	SPIMODE_PANASONIC_A2_D1,			///< 16 bit addr,8 bit value
	SPIMODE_PANASONIC_A2_D2,			///< 16 bit addr,16 bit value
	
	/* smartsens msb */
	SPIMODE_SMARTSENS_A2_D1=0xcb,		///< 16 bit addr,8 bit value  bit15-bit0
};
/** @} */



#define	MASTER_CTRL_DATA_SHIFT	(1<<0)	///< data shift mode 0: MSB out first, 1: LSB out first
#define	MASTER_CTRL_CPOL		(1<<1)	///< Clock polarity (Clock polarity) -spi when idle, the level state of Clock signal [1: high level in idle, 0: low level in idle]

/* Clock phase - SPI sampling at the edge of SCLK
CPHA=0, which represents the first edge
When CPOL=0 and idle is low, the first edge is going from low to high, so it's going up;
For CPOL=1, idle is high, and the first edge is going from high to low, so it's going down;
CPHA=1 represents the second edge:
CPOL=0, idle is low, and the second edge is going from high to low, so it's going down;
CPOL=1, idle is high, and the first edge is going from low to high, so it's going up;
*/
#define	MASTER_CTRL_CPHA		(1<<2)	
#define MASTER_CTRL_DELAY		(1<<3)	///< 0: there is no delay in register interval;1: delay is required after the register is written, and the delay value is given in data

/* SPI configures the structure */
typedef struct MasterSpiConfig_s
{
	double fMhz;      ///< configure SPI's clock
	BYTE byWordLen;   ///< Word length in bits. 0:8bit;1:16 bit, default is 0
	BYTE byCtrl;      ///< supported bit control code: MASTER_CTRL_DATA_SHIFT/MASTER_CTRL_CPOL/ MASTER_CTRL_CPHA/MASTER_CTRL_DELAY
	BYTE uRsv[64];    ///< keep* /
}MasterSpiConfig_t;

/* fingerprint module configuration */
typedef struct FpmConfig_s
{
    BYTE    byMode;
    UINT    uIrqCnt;            ///< sets several behaviors for an interrupt line, such as width of 320 and uIrqCnt=4, then 320*4 data update interrupt Numbers are read at one time
    BYTE    uRsv[64];           ///< keep * /
}FpmConfig_t;

/** @} */ // end of group3

/**************************************************************************************** 
*
* Image data acquisition is related
*
****************************************************************************************/


/** @defgroup group4 Image data acquisition is related
@{

*/

/** @name FrameBuffer mode configuration
@{

*/
///< FrameBuffer mode configuration

#define	BUF_MODE_NORMAL		0			 ///< the caching effect is equivalent to FIFO;When cache exceeds;\n
										// When the cache upper limit is set, the new frame will not be written to the cache.

#define BUF_MODE_SKIP		1			 ///<skip mode, there will be no "queuing" in the cache.

#define	BUF_MODE_NEWEST		2			 ///< nestmodel is only valid for the model with pci-e interface at present;\n
										// GrabFrame will get the latest cached frames;For other models\n
										// will be equivalent to SKIP mode
/** @} */ 

/* used to configure FrameBuffer */
typedef struct _FrameBufferConfig
{
	ULONG	    uMode; 			///< frame buffer mode
	ULONG		uBufferSize;	///<  the size of the frame cache in the device (bytes)
	ULONG		uUpLimit;		///<  cache upper limit (bytes). When the cache exceeds this upper limit, new frames are discarded
	ULONG		resv[16];		///< keep and fill in 0
}FrameBufferConfig;

/*The frame correlation information, corresponding to the frame data, is described by */
typedef struct sFrameInfo
{
	BYTE byChannel;		///< image channel identification, supported only by UH920/DTLC2
	USHORT uWidth;		///< image width, unit byte
	USHORT uHeight;		///< height of image, unit byte
	UINT uDataSize;		///< data size, unit byte
	UINT64 uiTimeStamp;	///< frame start timestamp value, unit us
}FrameInfo;

// the expanded frame information structure
typedef struct sFrameInfoEx
{
	BYTE	byChannel;		///< image channel identification, supported only by UH920/DTLC2
	BYTE    resvl[3];		///< keep 3 bytes and fill in 0
	BYTE    byImgFormat;	///< image format, D_RAW8, D_RAW10...
	USHORT	uWidth;			///< image width, unit byte
	USHORT	uHeight;		///< height of image, unit byte
	UINT	uDataSize;	    ///< data size, unit byte
	UINT	uFrameTag;		///< function upgrade identification
	double  fFSTimeStamp;	///< the timestamp at the start of the frame
	double  fFETimeStamp;	///< the timestamp at the end of the frame
	UINT	uEccErrorCnt;	///< the ECC error count per frame is only valid for the MIPI interface
	UINT	uCrcErrorCnt;	///< the CRC error count per frame is only valid for the MIPI interface
	UINT	uFrameID;		///< frame count
	UINT	resv[60];		///< keep and fill in 0
}FrameInfoEx;

/** @name FrameInfoEx::uFrameTag bit definition interpretation
@ {

* /
/* FrameInfoEx: : uFrameTag * /
/* the collection has started */
#define FRM_INFO_TAG_STARTED        1

/* frame has been collected and completed */
#define FRM_INFO_TAG_GRAB_OK        (1<<1)

/* frames have been processed by */
#define FRM_INFO_TAG_PROC_OK        (1<<2)

/* frames are broken or invalid, half of them may be collected and processed, and no further processing or submission is required */
#define FRM_INFO_TAG_BAD            (1<<4)

/* some errors may have occurred, but the data volume is complete */
#define FRM_INFO_TAG_ERR            (1<<5)

/* first frame after Restart */
#define FRM_INFO_TAG_FIRST          (1<<6)

/* TestWindow enables the flag */
#define FRM_INFO_TAG_TW             (1<<7)
/** @} */

/** @name preview window definition
@ {

* /
The preview window defines */
#define PREVIEW_ROI_B0    0x00
#define PREVIEW_ROI_B1    0x01
#define PREVIEW_ROI_B2    0x02
#define PREVIEW_ROI_B3    0x03
#define PREVIEW_ROI_B4    0x04
#define PREVIEW_ROI_GRID  0x05
#define PREVIEW_QUICK	  0x06 
#define PREVIEW_FULL      0x07
#define PREVIEW_NOTHING   0x08
/** @} */

/** @} */ // end of group4


/**************************************************************************************** 
*
* power management is relevant
*
****************************************************************************************/

/** @defgroup group5 power management unit related
@ {
* /
The frame correlation information, corresponding to the frame data, is described by */
/** @name power type definition required for SENSOR
@ {

* /
/* define the type of power required for the SENSOR */
/// define the type of power required for the SENSOR.
typedef enum
{

	/* A channel, or only one channel when */
	POWER_AVDD = 0,///<A channel AVDD
	POWER_DOVDD, ///<A channel DOVDD
	POWER_DVDD, ///<A channel DVDD
	POWER_AFVCC, ///<A channel AFVCC
	POWER_VPP, ///<A channel VPP

	/* B channel */
	POWER_AVDD_B, ///<B channel AVDD
	POWER_DOVDD_B, ///<B channel DOVDD
	POWER_DVDD_B, ///<B channel DVDD
	POWER_AFVCC_B, ///<B channel AFVCC
	POWER_VPP_B, ///<B channel VPP

	/* newly added power channel definition */
	POWER_OISVDD,
	POWER_AVDD2,
	POWER_AUX1,
	POWER_AUX2,
}SENSOR_POWER;


/** @} */

/** @name SENSOR system power type definition
@ {

* /
/* define system power type */
/// define the system power type.
typedef enum
{
	POWER_SYS_12V = 0,///<12V system power supply
	POWER_SYS_5V, ///<5V system power
	POWER_SYS_3_3V ///< 3.3v system power supply
}SYS_POWER;
/** @} */

/** @name SENSOR power mode definition
@ {

* /
/* define the power mode */
/// define the power mode.
typedef enum
{
	POWER_MODE_WORK = 0,///<SENSOR power supply for working mode
	POWER_MODE_STANDBY, ///<SENSOR power supply for standby mode
	POWER_MODE_PWDN ///<SENSOR power supply is power off mode
}POWER_MODE;
/** @} */

/** @name current test range definition
@ {

* /
/* define current test range */
/// define the current test range.
typedef enum
{
	CURRENT_RANGE_MA = 0,///< current test range is mA
	CURRENT_RANGE_UA, ///< current test range is uA
	CURRENT_RANGE_NA ///< current test range is nA
}CURRENT_RANGE;
/** @} */
/** @} */ // end of group5

/**************************************************************************************** 
*
* Definition of button function (definition used in dtTest. Exe)
*
****************************************************************************************/
/** @defgroup group6 initialization control is relevant
@{

*/
/** @name button function definition
@{

*/ 
#define KEY_ROI_B0			0x100
#define KEY_ROI_B1			0x80
#define KEY_ROI_B2			0x40
#define KEY_ROI_B3			0x10
#define KEY_ROI_B4			0x20
#define KEY_ROI_GRID		0x04
#define KEY_FULL			0x08
#define KEY_PLAY			0x01
#define KEY_CAM				0x02
#define KEY_NOTHING			0x00 
/** @} */
/** @} */ // end of group6

/**************************************************************************************** 
*
* definition of AF device model supported by the system
*
****************************************************************************************/
/** @defgroup group7 AF related
@{

*/
/** @name supported AF device model definition
@{

*/
#define AF_DRV_AD5820		0
#define AF_DRV_DW9710		0
#define AF_DRV_DW9714		0

#define AF_DRV_AD5823		1
#define AF_DRV_FP5512		2
#define AF_DRV_DW9718		3
#define AF_DRV_BU64241		4
#define AF_DRV_LV8498		5
#define AF_DRV_BU64291		6
#define AF_DRV_AD1457		7

#define AF_DRV_DW9761		8
#define AF_DRV_AD5816		8

#define AF_DRV_AK7345		9
#define AF_DRV_DW9800		10

#define AF_DRV_ZC533		11
#define AF_DRV_BU64295		12
#define AF_DRV_DW9719		13
//#define AF_DRV_SC9714		14
#define AF_DRV_FP5518		14

#define AF_DRV_AK7374		15

#define AF_DRV_LC898219		16

#define AF_DRV_MAX			30

/** @} */
/** @} */ // end of group7


/**************************************************************************************** 
*
* OS/LC related
*
****************************************************************************************/
/** @defgroup group8 OS/LC related
@{

*/
/** @name OS/LC test configuration definition
@{

*/
/* OS/LC test configuration definition USES in the LC_OS_CommandConfig function */
#define OS_CFG_CHANNEL_A					(1<<5)
#define OS_CFG_CHANNEL_B					(1<<4)	
#define OS_CFG_TEST_ENA						(1<<7)
#define LC_CFG_TEST_ENA						(1<<6)
#define OS_CFG_HIGH							(1<<3)
#define OS_CFG_LOW							(1<<2)
#define LC_CFG_HIGH							(1<<1)
#define LC_CFG_LOW							(1<<0)
/** @} */
/** @name OS/LC test result definition, results returned by the OS_Read function
@{

*/
/* OS/LC test results definition, results returned by the OS_Read function */
#define	OS_TEST_RESULT_PASS					0	  // pass the test
#define OS_TEST_RESULT_NG					1	  // failed the test
#define OS_TEST_RESULT_FAILED				0xfe  // the test failed
#define OS_TEST_RESULT_INVALID				0xff  // test invalid
/** @} */

/** @} */ // end of group8

/************************************************************************
*
* external extended IO definition
*
/************************************************************************/
/** @defgroup group9 extends IO
@{

*/
/** @name external extension IO pin definition
@{

*/
typedef enum
{
	GPIO0=0,	///<GPIO0
	GPIO1,		///<GPIO1
	GPIO2,		///<GPIO2
	GPIO3,		///<GPIO3
	GPIO4,		///<GPIO4
	GPIO5,		///<GPIO5
	GPIO6,		///<GPIO6
	GPIO7,		///<GPIO7
	GPIO8,		///<GPIO8
	GPIO9,		///<GPIO9
	GPIO10,		///<GPIO10
	GPIO11,		///<GPIO11
}EXT_GPIO;
/** @} */

/** @name external extension IO schema definition
@{

*/
typedef enum
{
	GPIO_INPUT=0, ///< input mode
	GPIO_OUTPUT, ///< constant level output mode
	GPIO_OUTPUT_PP, ///< high and low level interactive output

}EXT_GPIO_MODE;
/** @} */
/** @} */ // end of group9
/**************************************************************************************** 
*
* macrodefinitions used in some SDK interface functions(definitions used by DTCCM)
*
****************************************************************************************/
//PMU range....
#define PMU1_1						0x11
#define PMU1_2						0x10
#define PMU1_3						0x12
#define PMU2_1						0x21
#define PMU2_2						0x20
#define PMU2_3						0x22
#define PMU3_1						0x31
#define PMU3_2						0x30
#define PMU3_3						0x32
#define PMU4_1						0x41
#define PMU4_2						0x40
#define PMU4_3						0X42
#define PMU5_1						0x51
#define PMU5_2						0x50
#define PMU5_3						0X52

#define I2C_400K					1
#define I2C_100K					0

#define I_MAX_100mA					1
#define I_MAX_300mA					0

#define PMU_ON						0
#define PMU_OFF						1

#define POWER_ON					1
#define POWER_OFF					0

#define CLK_ON						1
#define CLK_OFF						0

#define IO_PULLUP					1
#define IO_NOPULL					0

#define MULTICAM_NORMAL				0x00
#define MULTICAM_PWDN_NOT			0x01
#define MULTICAM_RESET_PWDN_OVERLAP	0x02
#define MULTICAM_SPECIAL			0x03

// the control code given to the user is used by the EXCTRL function
#define EXCTRL_PMU_LDO_CTRL											1000	/* ExCtrl function parameter dwParam, BIT0 control AVDD,BIT1 control AVDD2.1 is LDO on and 0 is LDO off*/
#define EXCTRL_PMU_CAP_CTRL											1001	/* low 2-bit control CAP of the ExCtrl function parameter dwParam, BIT0 control AVDD,BIT1 control AVDD2.1 is open CAP, 0 is close CAP */

/**************************************************************************************** 
*
* common error code
*
****************************************************************************************/
/// @brief no action
#define DT_ERROR_NO_ACTION						8		///< no action
/// @brief operation is ignored and no action is required
#define DT_ERROR_IGNORED						7		 ///< operation is ignored and no action is required
/// @brief requires other data and operations
#define DT_ERROR_NEED_OTHER					    6		 ///< requires additional data and actions	
/// @brief has to proceed to the next stage, and only part of the action has been completed
#define DT_ERROR_NEXT_STAGE						5		///< the next stage is needed and only part of the action is completed     
/// @brief is busy (the last operation is still in progress), this operation cannot be carried out
#define DT_ERROR_BUSY					   		4		 ///< busy (the last operation is still in progress), this operation cannot be done
/// @brief needs to wait (the condition for operation is not true) and can be tried again
#define DT_ERROR_WAIT                	   		3		///< need to wait (the condition for the operation is not true), you can try again
/// @brief is under way and has been operated
#define DT_ERROR_IN_PROCESS               		2		///<is under way and has been operated
/// @brief operation was successful
#define DT_ERROR_OK								1		///<operation was successful
/// @brief operation failed
#define DT_ERROR_FAILED							0		///<operation failed
/// @brief internal error
#define DT_ERROR_INTERNAL_ERROR					-1		///< internal error
/// @brief unknown error
#define DT_ERROR_UNKNOW							-1		///<unknown error
/// @brief does not support this feature
#define DT_ERROR_NOT_SUPPORTED					-2		///<does not support this feature
/// @brief initialization is not completed
#define DT_ERROR_NOT_INITIALIZED         		-3		///<initialization is not completed
/// @brief parameter is invalid
#define DT_ERROR_PARAMETER_INVALID       		-4		///<parameter is invalid
/// @brief parameter crosses the line
#define DT_ERROR_PARAMETER_OUT_OF_BOUND  		-5		 ///< parameter crossing
/// @brief failed
#define DT_ERROR_UNENABLED  					-6		///< does not enable
/// @brief is not connected to the equipment
#define DT_ERROR_UNCONNECTED					-7		///< not connected to the device
/// @brief function is invalid
#define DT_ERROR_NOT_VALID						-8		///<function is invalid
/// @brief device is not opened
#define DT_ERROR_UNPLAY							-9		///< device is not on
/// @brief did not start
#define DT_ERROR_NOT_STARTED					-10		 ///< not started
/// @brief did not stop
#define DT_ERROR_NOT_STOPPED					-11		///< not stopped
/// @brief is not prepared
#define DT_ERROR_NOT_READY						-12		///< not ready
/// @brief error description
#define DT_ERROR_DESCR_FAULT					-20		///< description of the error
/// @brief wrong name
#define DT_ERROR_NAME_FAULT						-21		///< name of the error
/// @brief error assignment
#define DT_ERROR_VALUE_FAULT					-22		///< assignment for the error
/// @brief is restricted
#define DT_ERROR_LIMITED						-28		///<restricted
/// @brief function is invalid
#define DT_ERROR_FUNCTION_INVALID				-29		/// < function is invalid
/// @brief is in automatic operation, manual method is invalid
#define DT_ERROR_IN_AUTO						-30		///< in the automatic and manual mode is invalid
/// @brief operation was rejected
#define DT_ERROR_DENIED							-31     ///< operation is rejected
/// @brief offset or address not aligned
#define DT_ERROR_BAD_ALIGNMENT					-40     ///< offset or address did not align it
/// @brief address is invalid
#define DT_ERROR_ADDRESS_INVALID				-41     ///<address is invalid
/// @brief data block size is invalid
#define DT_ERROR_SIZE_INVALID					-42     /// < data block size is invalid
/// @brief data volume overload
#define DT_ERROR_OVER_LOAD						-43     ///< data volume overload
/// @brief data volume overload
#define DT_ERROR_UNDER_LOAD						-44     ///< data quantity is not enough
/// @breif Buffer space is too small
#define DT_ERROR_BUFFER_SMALL					-52		///< Buffer space is too small
/// @brief inspection and verification failed
#define DT_ERROR_CHECKED_FAILED					-50     ///< check, check failed
/// / @brief is not available
#define DT_ERROR_UNUSABLE						-51		///< unavailable
/// @brief business ID is invalid or does not match
#define DT_ERROR_BID_INVALID					-52   	///< business ID is invalid or does not match


/* IO, storage, device-related */
/// @brief timeout error
#define DT_ERROR_TIME_OUT             	   		-1000	/// < timeout error
/// @brief hardware IO error
#define DT_ERROR_IO_ERROR                 		-1001
/// @brief communication error
#define DT_ERROR_COMM_ERROR						-1002	 ///< communication error
/// / @brief bus error
#define DT_ERROR_BUS_ERROR				   		-1003	///< bus error
/// / @brief format error
#define DT_ERROR_FORMAT_INVALID					-1004	/// < format error
/// @brief content is invalid
#define DT_ERROR_CONTENT_INVALID				-1005	///<Invalid content
/// @brief data verification error
#define DT_ERROR_BAD_CHECKSUM					-1006   ///< data validation error
/// @brief I2C bus error
#define DT_ERROR_I2C_FAULT						-1010	 ///<I2C bus error
/// @brief I2C waits for response timeout
#define DT_ERROR_I2C_ACK_TIMEOUT				-1011	 ///<I2C wait for reply timeout
/// @brief I2C waits for the bus action to timeout, for example, SCL is pulled to low level by external device
#define DT_ERROR_I2C_BUS_TIMEOUT				-1012	///<I2C wait for bus action timeout, for example SCL is pulled to low level by external device
/// @brief SPI bus error
#define DT_ERROR_SPI_FAULT						-1020	///<SPI bus error
/// @brief UART bus error
#define DT_ERROR_UART_FAULT						-1030	///<UART bus error
/// @brief GPIO bus error
#define DT_ERROR_GPIO_FAULT						-1040	///<GPIO bus error
/// / @brief USB bus error
#define DT_ERROR_USB_FAULT						-1050	///<USB bus error
/// @brief PCI bus error
#define DT_ERROR_PCI_FAULT						-1060	///<PCI bus error
/// @brief physical layer error
#define DT_ERROR_PHY_FAULT						-1070	///<physical layer error
/// @brief link layer error
#define DT_ERROR_LINK_FAULT						-1080	///< link layer error
/// @brief transmission layer error
#define DT_ERROR_TRANS_FAULT					-1090	///< transport layer error

/// @brief no equipment was found
#define DT_ERROR_NO_DEVICE_FOUND				-1100	///< no equipment found /// @brief no logical equipment found
/// @brief did not find the logical equipment
#define DT_ERROR_NO_LOGIC_DEVICE_FOUND   		-1101	///<No logical device was found
/// @brief equipment has been opened
#define DT_ERROR_DEVICE_IS_OPENED				-1102	///<equipment has been opened
/// @brief equipment has been closed
#define DT_ERROR_DEVICE_IS_CLOSED				-1103	///<equipment has been closed
/// @brief equipment has been disconnected
#define DT_ERROR_DEVICE_IS_DISCONNECTED    		-1104  	///The device has been disconnected
/// @brief equipment has been opened by other hosts
#define DT_ERROR_DEVICE_IS_OPENED_BY_ANOTHER	-1105	/// the device has been opened by other hosts
/// @brief kernel drivers have problems
#define DT_ERROR_KERNEL_DRIVER_PROBLEM			-1106	///There was a problem with kernel driver
/// @brief network sockets has problems
#define DT_ERROR_SOCKET_PROBLEM					-1107	///There was a problem with network socket

/// @brief does not have enough system memory
#define DT_ERROR_NO_MEMORY  	   		   		-1200	/// < not enough system memory
/// @brief memory read and write error or unable to read and write normally
#define DT_ERROR_MEM_FAULT						-1201	///< memory read and write error or unable to read and write normally
/// @brief write protection, not write
#define DT_ERROR_WRITE_PROTECTED          		-1202  	 ///< write protected, not writable

/// @brief failed to create document
#define DT_ERROR_FILE_CREATE_FAILED				-1300	///< create file failed
/// @brief file format is invalid
#define DT_ERROR_FILE_INVALID             		-1301	/// < file format is invalid
/// @brief read file failed
#define DT_ERROR_FILE_READ_FAILED				-1302  	///< read file failed
/// @brief writing to file failed
#define DT_ERROR_FILE_WRITE_FAILED				-1303  	///< write to file failed
/// @brief failed to open the file
#define DT_ERROR_FILE_OPEN_FAILED				-1304	///< open file failed
/// @brief read data comparison failed
#define DT_ERROR_FILE_CHECKSUM_FAILED			-1305  	 ///< reading data comparison failed

/// @brief data collection failed
#define DT_ERROR_GRAB_FAILED           	   		-1600	///<data acquisition fail
/// @brief data is lost and incomplete
#define DT_ERROR_LOST_DATA                		-1601	///<data is missing, incomplete
/// @brief does not receive the frame terminator
#define DT_ERROR_EOF_ERROR           	   		-1602	///<did not receive the end of the frame
/// @brief data collection function has been opened
#define DT_ERROR_GRAB_IS_OPENED         		-1603  	 ///< data acquisition function has been turned on
/// @brief data collection function has been closed
#define DT_ERROR_GRAB_IS_CLOSED         		-1604  	///<The data acquisition function is closed
/// @brief data collection has been started
#define DT_ERROR_GRAB_IS_STARTED         		-1605  	///<The data collection has been started
/// @brief data collection has been stopped
#define DT_ERROR_GRAB_IS_STOPPED         		-1606  	///<The data collection is stopped
/// @brief data collection is being restarted
#define DT_ERROR_GRAB_IS_RESTARTING				-1607	///<data collection is restarted
/// @brief data collection has been suspended
#define DT_ERROR_GRAB_IS_HOLD                   -1608   ///<Data collection has been suspended
/// @ the frames collected by brief are out of date
#define DT_ERROR_FRAME_IS_OUTDATED              -1609   ///< the acquired frames are out of date
/// @brief sets an invalid ROI parameter
#define DT_ERROR_ROI_PARAM_INVALID				-1610   ///<The ROI parameter set by  is invalid
/// @brief ROI function is not supported
#define DT_ERROR_ROI_NOT_SUPPORTED				-1611   ///<ROI function is not supported
/// @brief discards unwanted frames after video stream restart
#define DT_ERROR_GRAB_IS_DROPPED				-1613	///<discards unwanted frames after video stream restart
/// @brief calibration failed
#define DT_ERROR_CALIBRATE_FAILED               -1614  ///< calibration failure
/** @} */ // end of group10


/****************************************************************************************
*
* Used to make it easy to write a program and return the error code immediately when an error is encountered
*
****************************************************************************************/
#define CHECK_RETURN(_FUN_) \
{\
	int iChkRet = _FUN_; \
	if (iChkRet != DT_ERROR_OK) \
	return iChkRet; \
	}



#endif // __IMAGEKIT_H__
