#pragma once

#include "DccmErrorDef.h"
#include "DccmSocket.h"

//UI 
#define  UI_DLGOPEN              0   //程序启动
#define  UI_LOADCONFIG_SUCESS    1   //加载参数成功
#define  UI_PLAY_SUCESS          2   //播放成功
#define  UI_PLAY_FAILED         -2   //播放失败
#define  UI_STOP_SUCESS          3   //停止成功
#define  UI_AUTOTEST_START       4   //自动测试开始
#define  UI_MANUALTEST_START     5   //自动测试开始
#define  UI_ENTER_OFFLINE        6   //
#define  UI_EXIT_OFFLINE         7
#define  UI_START_STOP           8   //启动停止中

//USER MESSAGE
#define WM_CAMERA_START			WM_USER + 10
#define WM_CAMERA_STOP			WM_USER + 11

//自定义消息
#define WM_LOG_SHOW				WM_USER + 101
#define WM_LOG_CLEAR			WM_USER + 102
#define WM_UPDATE_UI			WM_USER + 103
#define WM_SHOW_STATUS			WM_USER + 104
#define WM_GET_FOCUS			WM_USER + 105
#define WM_SELECT_RADIO_BUTTON  WM_USER + 106
#define WM_CREATE_SETTING_DLG   WM_USER + 108
#define WM_FULL_SCREEN			WM_USER + 109
#define WM_SHOW_TEXT_INFO       WM_USER + 110
#define WM_CAPTURE_SCRREN		WM_USER + 111
#define WM_OPERATOR_DLG			WM_USER + 112
#define WM_FRAME_RECORD_COUNT	WM_USER + 113

#define WM_GETMASKEDEDIT_TEXT   WM_USER + 201

//BUTTON PANEL
#define WM_INSPECTIONBUTTON_SHOWSTATUS   WM_USER + 301
#define WM_INSPECTIONBUTTON_ALIGNWNDPOS  WM_USER + 302

//Main Window To DLL dlg
#define WM_INSPCETIONDLL_KEY_CTRLNUM  WM_USER + 401


//曝光返回值
#define EXPOSURE_ERROR_ZERO                -2
#define EXPOSURE_ERROR_OVERFLOW            -1
#define EXPOSURE_OK                         0
#define EXPOSURE_FRAME_PITCHING             1
#define EXPOSURE_RUNNING                    2


#define EXP_Y 0  //Y通道曝光
#define EXP_G 1  //G通道曝光
#define EXP_WHITE_Y 2 //针对黑白CHART 透光区域曝光 Y
#define EXP_WHITE_G 3 //针对黑白CHART 透光区域曝光 G通道曝光

#define COLOR_DARK_RED				RGB(139,0,0)
#define COLOR_GREEN					RGB(0,255,0)
#define COLOR_DARK_GREEN			RGB(0,100,0)
#define COLOR_LIGHT_GREEN			RGB(144,238,144)
#define COLOR_SKY					RGB(66,162,255)
#define COLOR_LIGHT_BLUE			RGB(173,216,230)
#define COLOR_LIGHT_BLUE1			RGB(191,239,255)
#define COLOR_LIGHT_BLUE2			RGB(178,223,238)
#define COLOR_LIGHT_BLUE3			RGB(154,192,205)
#define COLOR_LIGHT_BLUE4			RGB(104,131,139)
#define COLOR_LIGHT_YELLOW			RGB(255,255,224)
#define COLOR_CYAN2					RGB(0,238,238)	// 蓝绿
#define COLOR_CYAN3					RGB(0,205,205)	// 蓝绿
#define COLOR_CYAN4					RGB(0,139,139)	// 蓝绿
#define COLOR_GREEN_WHITE			RGB(100,240,100)	
#define COLOR_MISTY_ROSE1			RGB(255,228,225)
#define COLOR_MISTY_ROSE2			RGB(238,213,210)
#define COLOR_MISTY_ROSE3			RGB(205,183,181)
#define COLOR_MISTY_ROSE4			RGB(139,125,123)
#define COLOR_SYS					RGB(236,233,215)
#define COLOR_LIMIT					RGB(0,128,128)
#define COLOR_GREENRED				RGB(128,128,0)
#define COLOR_GREENBLUE				RGB(0,128,128)
#define COLOR_GREY					RGB(232,232,232)
#define COLOR_DIM_GREY				RGB(190,190,190)
#define COLOR_PALE_TURQUOISE		RGB(175,238,238)
#define COLOR_DARK_TURQUOISE		RGB(0,206,209)
#define COLOR_MEDIUM_TURQUOISE		RGB(72,209,204)
#define COLOR_DARK_GOLDENROD		RGB(184,134,11)		// Dark Yellow
#define COLOR_LIGHTPINK             RGB(255,182,193)    // 浅粉红  
#define COLOR_PINK                  RGB(255,192,203)    // 粉红  
#define COLOR_CRIMSON               RGB(220,20,60)      // 猩红 (深红)  
#define COLOR_LAVENDERBLUSH         RGB(255,240,245)    // 淡紫红  
#define COLOR_PALEVIOLETRED         RGB(219,112,147)    // 弱紫罗兰红  
#define COLOR_HOTPINK               RGB(255,105,180)    // 热情的粉红  
#define COLOR_DEEPPINK              RGB(255,20,147)     // 深粉红  
#define COLOR_MEDIUMVIOLETRED       RGB(199,21,133)     // 中紫罗兰红  
#define COLOR_ORCHID                RGB(218,112,214)    // 兰花紫  
#define COLOR_THISTLE               RGB(216,191,216)    // 蓟  
#define COLOR_PLUM                  RGB(221,160,221)    // 李子紫  
#define COLOR_VIOLET                RGB(238,130,238)    // 紫罗兰  
#define COLOR_MAGENTA               RGB(255,0,255)      // 洋红 (品红 玫瑰红)  
#define COLOR_FUCHSIA               RGB(255,0,255)      // 灯笼海棠(紫红色)  
#define COLOR_DARKMAGENTA           RGB(139,0,139)      // 深洋红  
#define COLOR_PIGRED                RGB(123,15,31)		// 猪血红
#define COLOR_PURPLE                RGB(128,0,128)      // 紫色  
#define COLOR_MEDIUMORCHID          RGB(186,85,211)     // 中兰花紫  
#define COLOR_DARKVIOLET            RGB(148,0,211)      // 暗紫罗兰  
#define COLOR_DARKORCHID            RGB(153,50,204)     // 暗兰花紫  
#define COLOR_INDIGO                RGB(75,0,130)      // 靛青 (紫兰色)  
#define COLOR_BLUEVIOLET            RGB(138,43,226)     // 蓝紫罗兰  
#define COLOR_MEDIUMPURPLE          RGB(147,112,219)    // 中紫色  
#define COLOR_MEDIUMSLATEBLUE       RGB(123,104,238)    // 中板岩蓝  
#define COLOR_SLATEBLUE             RGB(106,90,205)     // 板岩蓝  
#define COLOR_DARKSLATEBLUE         RGB(72,61,139)     // 暗板岩蓝  
#define COLOR_LAVENDER              RGB(230,230,250)    // 熏衣草淡紫  
#define COLOR_GHOSTWHITE            RGB(248,248,255)    // 幽灵白  
#define COLOR_BLUE                  RGB(0,0,255)		// 纯蓝  
#define COLOR_MEDIUMBLUE            RGB(0,0,205)		// 中蓝色  
#define COLOR_MIDNIGHTBLUE          RGB(25,25,112)		// 午夜蓝  
#define COLOR_DARKBLUE              RGB(0,0,139)		// 暗蓝色  
#define COLOR_NAVY                  RGB(0,0,128)		// 海军蓝  
#define COLOR_ROYALBLUE             RGB(65,105,225)    // 皇家蓝 (宝蓝)  
#define COLOR_CORNFLOWERBLUE        RGB(100,149,237)    // 矢车菊蓝  
#define COLOR_LIGHTSTEELBLUE        RGB(176,196,222)    // 亮钢蓝  
#define COLOR_LIGHTSLATEGRAY        RGB(119,136,153)    // 亮石板灰  
#define COLOR_SLATEGRAY             RGB(112,128,144)    // 石板灰  
#define COLOR_DODGERBLUE            RGB(30,144,255)    // 道奇蓝  
#define COLOR_ALICEBLUE             RGB(240,248,255)    // 爱丽丝蓝  
#define COLOR_STEELBLUE             RGB(70,130,180)    // 钢蓝 (铁青)  
#define COLOR_LIGHTSKYBLUE          RGB(135,206,250)    // 亮天蓝色  
#define COLOR_SKYBLUE               RGB(135,206,235)    // 天蓝色  
#define COLOR_DEEPSKYBLUE           RGB(0,191,255)		// 深天蓝  
#define COLOR_LIGHTBLUE             RGB(173,216,230)    // 亮蓝  
#define COLOR_POWDERBLUE            RGB(176,224,230)    // 火药青  
#define COLOR_CADETBLUE             RGB(95,158,160)    // 军服蓝  
#define COLOR_AZURE                 RGB(240,255,255)    // 蔚蓝色  
#define COLOR_LIGHTCYAN             RGB(224,255,255)    // 淡青色  
#define COLOR_PALETURQUOISE         RGB(175,238,238)    // 弱绿宝石  
#define COLOR_CYAN                  RGB(0,255,255)      // 青色  
#define COLOR_AQUA                  RGB(0,255,255)      // 水色  
#define COLOR_DARKTURQUOISE         RGB(0,206,209)      // 暗绿宝石  
#define COLOR_DARKSLATEGRAY         RGB(47,79,79)      // 暗石板灰  
#define COLOR_DARKCYAN              RGB(0,139,139)      // 暗青色  
#define COLOR_TEAL                  RGB(0,128,128)      // 水鸭色  
#define COLOR_MEDIUMTURQUOISE       RGB(72,209,204)    // 中绿宝石  
#define COLOR_LIGHTSEAGREEN         RGB(32,178,170)    // 浅海洋绿  
#define COLOR_TURQUOISE             RGB(64,224,208)    // 绿宝石  
#define COLOR_AQUAMARINE            RGB(127,255,212)    // 宝石碧绿  
#define COLOR_MEDIUMAQUAMARINE      RGB(102,205,170)    // 中宝石碧绿  
#define COLOR_MEDIUMSPRINGGREEN     RGB(0,250,154)      // 中春绿色  
#define COLOR_MINTCREAM             RGB(245,255,250)    // 薄荷奶油  
#define COLOR_SPRINGGREEN           RGB(0,255,127)      // 春绿色  
#define COLOR_MEDIUMSEAGREEN        RGB(60,179,113)    // 中海洋绿  
#define COLOR_SEAGREEN              RGB(46,139,87)     // 海洋绿  
#define COLOR_HONEYDEW              RGB(240,255,240)    // 蜜瓜色  
#define COLOR_LIGHTGREEN            RGB(144,238,144)    // 淡绿色  
#define COLOR_PALEGREEN             RGB(152,251,152)    // 弱绿色  
#define COLOR_DARKSEAGREEN          RGB(143,188,143)    // 暗海洋绿  
#define COLOR_LIMEGREEN             RGB(50,205,50)     // 闪光深绿  
#define COLOR_LIME                  RGB(0,255,0)		// 闪光绿  
#define COLOR_FORESTGREEN           RGB(34,139,34)     // 森林绿  
#define COLOR_GREEN_HALF            RGB(0,128,0)		// 纯绿  
#define COLOR_DARK_GREEN             RGB(0,100,0)		// 暗绿色  
#define COLOR_CHARTREUSE            RGB(127,255,0)      // 查特酒绿 (黄绿色)  
#define COLOR_LAWNGREEN             RGB(124,252,0)      // 草坪绿  
#define COLOR_GREENYELLOW           RGB(173,255,47)     // 绿黄色  
#define COLOR_DARKOLIVEGREEN        RGB(85,107,47)     // 暗橄榄绿  
#define COLOR_YELLOWGREEN           RGB(154,205,50)     // 黄绿色  
#define COLOR_OLIVEDRAB             RGB(107,142,35)     // 橄榄褐色  
#define COLOR_BEIGE                 RGB(245,245,220)    // 米色(灰棕色)  
#define COLOR_LIGHTGOLDENRODYELLOW  RGB(250,250,210)    // 亮菊黄  
#define COLOR_IVORY                 RGB(255,255,240)    // 象牙  
#define COLOR_LIGHTYELLOW           RGB(255,255,224)    // 浅黄色  
#define COLOR_YELLOW                RGB(255,255,0)      // 纯黄  
#define COLOR_OLIVE                 RGB(128,128,0)      // 橄榄  
#define COLOR_DARKKHAKI             RGB(189,183,107)    // 深卡叽布  
#define COLOR_LEMONCHIFFON          RGB(255,250,205)    // 柠檬绸  
#define COLOR_PALEGOLDENROD         RGB(238,232,170)    // 灰菊黄  
#define COLOR_KHAKI                 RGB(240,230,140)    // 卡叽布  
#define COLOR_GOLD                  RGB(255,215,0)      // 金色  
#define COLOR_CORNSILK              RGB(255,248,220)    // 玉米丝色  
#define COLOR_GOLDENROD             RGB(218,165,32)     // 金菊黄  
#define COLOR_DARKGOLDENROD         RGB(184,134,11)     // 暗金菊黄  
#define COLOR_FLORALWHITE           RGB(255,250,240)    // 花的白色  
#define COLOR_OLDLACE               RGB(253,245,230)    // 旧蕾丝  
#define COLOR_WHEAT                 RGB(245,222,179)    // 小麦色  
#define COLOR_MOCCASIN              RGB(255,228,181)    // 鹿皮靴  
#define COLOR_ORANGE                RGB(255,165,0)      // 橙色  
#define COLOR_PAPAYAWHIP            RGB(255,239,213)    // 番木瓜  
#define COLOR_BLANCHEDALMOND        RGB(255,235,205)    // 发白的杏仁色  
#define COLOR_NAVAJOWHITE           RGB(255,222,173)    // 土著白  
#define COLOR_ANTIQUEWHITE          RGB(250,235,215)    // 古董白  
#define COLOR_TAN                   RGB(210,180,140)    // 茶色  
#define COLOR_BURLYWOOD             RGB(222,184,135)    // 硬木色  
#define COLOR_BISQUE                RGB(255,228,196)    // 陶坯黄  
#define COLOR_DARK_ORANGE           RGB(255,140,0)      // 深橙色  
#define COLOR_LINEN                 RGB(250,240,230)    // 亚麻布  
#define COLOR_PERU                  RGB(205,133,63)     // 秘鲁  
#define COLOR_PEACHPUFF             RGB(255,218,185)    // 桃肉色  
#define COLOR_SANDYBROWN            RGB(244,164,96)     // 沙棕色  
#define COLOR_CHOCOLATE             RGB(210,105,30)     // 巧克力  
#define COLOR_SADDLEBROWN           RGB(139,69,19)      // 马鞍棕色  
#define COLOR_SEASHELL              RGB(255,245,238)    // 海贝壳  
#define COLOR_SIENNA                RGB(160,82,45)      // 黄土赭色  
#define COLOR_LIGHTSALMON           RGB(255,160,122)    // 浅鲑鱼肉色  
#define COLOR_CORAL                 RGB(255,127,80)     // 珊瑚  
#define COLOR_RED_ORANGE            RGB(255,69,0)		// 橙红色  
#define COLOR_DARKSALMON            RGB(233,150,122)    // 深鲜肉(鲑鱼)色  
#define COLOR_TOMATO                RGB(255,99,71)      // 番茄红  
#define COLOR_MISTYROSE             RGB(255,228,225)    // 薄雾玫瑰  
#define COLOR_SALMON                RGB(250,128,114)    // 鲜肉(鲑鱼)色  
#define COLOR_SNOW                  RGB(255,250,250)    // 雪  
#define COLOR_LIGHTCORAL            RGB(240,128,128)    // 淡珊瑚色  
#define COLOR_ROSYBROWN             RGB(188,143,143)    // 玫瑰棕色  
#define COLOR_INDIANRED             RGB(205,92,92)      // 印度红  
#define COLOR_RED                   RGB(255,0,0)		// 纯红  
#define COLOR_BROWN                 RGB(165,42,42)      // 棕色  
#define COLOR_FIREBRICK             RGB(178,34,34)      // 耐火砖  
#define COLOR_DARKRED               RGB(139,0,0)		// 深红色  
#define COLOR_MAROON                RGB(128,0,0)		// 栗色  
#define COLOR_WHITESMOKE            RGB(245,245,245)    // 白烟  
#define COLOR_GAINSBORO             RGB(220,220,220)    // 庚斯博罗灰色  
#define COLOR_LIGHTGREY             RGB(211,211,211)    // 浅灰色  
#define COLOR_SILVER                RGB(192,192,192)    // 银灰色  
#define COLOR_DARKGRAY              RGB(169,169,169)    // 深灰色  
#define COLOR_GRAY                  RGB(186,183,186)    // 灰色(*)  
#define COLOR_DIMGRAY               RGB(105,105,105)    // 暗淡的灰色  
#define COLOR_BLACK                 RGB(0,0,0)			// 纯黑
#define COLOR_WHITE                 RGB(255,255,255)    // 纯白  
#define COLOR_DARK_SLATE_GREY		RGB(150,255,255) 
#define COLOR_GREY_WHITE			RGB(160,160,160)	// 灰白色

#ifndef SAFE_DELETE
    #define SAFE_DELETE(p)			{	if(p) { delete (p);						(p)=NULL; }	}
    #define SAFE_DELETE_ARRAY(p)	{	if(p) { delete [] (p);					(p)=NULL; }	}
    #define SAFE_CLOSE_HANDLE(p)	{	if(p) { ::CloseHandle(p);				(p)=NULL; }	}
    #define SAFE_DELETE_OBJECT(p)	{	if(p) { ::DeleteObject((HGDIOBJ) p);	(p)=NULL; }	}
	#define SAFE_FREE(p)			{	if(p) { ::free(p);						(p)=NULL; } }
	#define SAFE_FREE_LIBRARY(p)	{	if(p) { ::FreeLibrary(p);				(p)=NULL; } }
#endif	/* SAFE_DELETE */

#define POINTER_ASSERT(p) { if(!p) return; }

#define STATUS_OFFLINE  1	// 离线模式
#define STATUS_RUNNING  2	// 运行状态
#define STATUS_STANDBY  3	// 就绪状态
#define STATUS_OPENNING 4	// 打开中状态
#define STATUS_PAUSE    5	// 暂停状态

#define WORKSTATION_NULL   0 
#define WORKSTATION_TJ     1 
#define WORKSTATION_OTP    2
#define WORKSTATION_JC     3 
#define WORKSTATION_EX1    4 
#define WORKSTATION_EX2    5 

#define SET_BIT(x,y) x|=(1<<y)  //将X的第Y个位置置1
#define CLR_BIT(x,y) x&= ~(1<<y) //将X的第Y个位置置0

#define ARROWKEY_LEFT  1
#define ARROWKEY_RIGHT 2
#define ARROWKEY_UP    3
#define ARROWKEY_DOWN  4

#define DATA_DEC        0 //十进制
#define	DATA_HEX_WORD   1 //十六进制
#define	DATA_HEX_BYTE   2 //0xAA
#define	DATA_HEX_DWORD  3 //0xAAAA
#define	DATA_STRING     4

#define THROW_I2C_ERROR 1949 //定义I2C通信错误的抛出的值

#define SAVETOSQL_STEP_INIT 0  //存储的阶段 初始化失败，没有点亮模组
#define SAVETOSQL_STEP_RUN  1 //点亮模组

enum I2CMODE_TRULY
{
	I2CMODE_8_8 = 0,	// I2CMODE_NORMAL 8 bit addr,8 bit value 
	I2CMODE_8_8_S,		// I2CMODE_SAMSUNG 8 bit addr,8 bit value,Stopen 
	I2CMODE_8_16,		// I2CMODE_MICRON  8 bit addr,16 bit value 
	I2CMODE_16_8,		// I2CMODE_STMICRO 16 bit addr,8 bit value, (eeprom also)
	I2CMODE_16_16,	    // I2CMODE_MICRON2 16 bit addr,16 bit value
	I2CMODE_D_8_8,       //I2CMODE_DIRECT_A8V8 no register direct 直接读写
	I2CMODE_D_8_16,      //I2CMODE_DIRECT_A8V16 no register  
};

enum COLOR_GAIN
{
	COLOR_GAIN_R,
	COLOR_GAIN_B,
	COLOR_GAIN_GR,
	COLOR_GAIN_GB,
};

#define OIS_ON_MODE_STILL 0
#define OIS_ON_MODE_MOVIE 1

#define DS_INIT 0 //测试时模组只执行到了初始化环节
#define DS_ALL 1  //测试时模组正常执行完毕
#define DS_BREAK 2 //测试时模组异常中断

// 20160606
#define MainCaption						_T("HTHC")			// 公司名
#define MainBackground					_T("HTHC")			// 公司名作背景
#define DEFAULT_LOCAL_REPORT_PATH		_T("D:\\DccmReport")// 
#define DEFAULT_NETWORK_REPORT_PATH		_T("\\\\111.111.111.100\\DccmReport")
#define DEFAULT_PATH_FILE_LOG			_T("D:\\DccmLog")
#define DEFAULT_NAME_FILE_LOG			_T("DccmLog")
#define FONT_FACE_NAME					_T("Consolas")

#define FONT_SIZE_TINY			80
#define FONT_SIZE_SMALL			120
#define FONT_SIZE_DEFAULT		160
#define FONT_SIZE_BIG			220

enum eSaveImageType
{
	SIT_JPEG = 1,			// 1 JPEG(*.jpg)|*.jpg|
	SIT_BMP,				// 2 BMP(*.bmp)|*.bmp|
	SIT_RawSingleChannel,	// 3 RawSingleChannel(*.bmp)|*.bmp|
	SIT_RawGray,			// 4 RawGray(*.bmp)|*.bmp|
	SIT_Raw,				// 5 Raw(*.raw)|*.raw|
	SIT_AssistWnd,			// 6 AssistWnd(*.jpg)|*.jpg|
	SIT_DisplayWnd,			// 7 DisplayWnd(*.jpg)|*.jpg|
	SIT_BmpGray,			// 8 BmpGray(*.bmp)|*.bmp||
};

enum LOGIN_MODE
{
	LOGIN_MODE_ENGNEER,
	LOGIN_MODE_OPERATOR
};