#pragma once

#include "DccmErrorDef.h"
#include "DccmSocket.h"

//UI 
#define  UI_DLGOPEN              0   //��������
#define  UI_LOADCONFIG_SUCESS    1   //���ز����ɹ�
#define  UI_PLAY_SUCESS          2   //���ųɹ�
#define  UI_PLAY_FAILED         -2   //����ʧ��
#define  UI_STOP_SUCESS          3   //ֹͣ�ɹ�
#define  UI_AUTOTEST_START       4   //�Զ����Կ�ʼ
#define  UI_MANUALTEST_START     5   //�Զ����Կ�ʼ
#define  UI_ENTER_OFFLINE        6   //
#define  UI_EXIT_OFFLINE         7
#define  UI_START_STOP           8   //����ֹͣ��

//USER MESSAGE
#define WM_CAMERA_START			WM_USER + 10
#define WM_CAMERA_STOP			WM_USER + 11

//�Զ�����Ϣ
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


//�عⷵ��ֵ
#define EXPOSURE_ERROR_ZERO                -2
#define EXPOSURE_ERROR_OVERFLOW            -1
#define EXPOSURE_OK                         0
#define EXPOSURE_FRAME_PITCHING             1
#define EXPOSURE_RUNNING                    2


#define EXP_Y 0  //Yͨ���ع�
#define EXP_G 1  //Gͨ���ع�
#define EXP_WHITE_Y 2 //��Ժڰ�CHART ͸�������ع� Y
#define EXP_WHITE_G 3 //��Ժڰ�CHART ͸�������ع� Gͨ���ع�

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
#define COLOR_CYAN2					RGB(0,238,238)	// ����
#define COLOR_CYAN3					RGB(0,205,205)	// ����
#define COLOR_CYAN4					RGB(0,139,139)	// ����
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
#define COLOR_LIGHTPINK             RGB(255,182,193)    // ǳ�ۺ�  
#define COLOR_PINK                  RGB(255,192,203)    // �ۺ�  
#define COLOR_CRIMSON               RGB(220,20,60)      // �ɺ� (���)  
#define COLOR_LAVENDERBLUSH         RGB(255,240,245)    // ���Ϻ�  
#define COLOR_PALEVIOLETRED         RGB(219,112,147)    // ����������  
#define COLOR_HOTPINK               RGB(255,105,180)    // ����ķۺ�  
#define COLOR_DEEPPINK              RGB(255,20,147)     // ��ۺ�  
#define COLOR_MEDIUMVIOLETRED       RGB(199,21,133)     // ����������  
#define COLOR_ORCHID                RGB(218,112,214)    // ������  
#define COLOR_THISTLE               RGB(216,191,216)    // ��  
#define COLOR_PLUM                  RGB(221,160,221)    // ������  
#define COLOR_VIOLET                RGB(238,130,238)    // ������  
#define COLOR_MAGENTA               RGB(255,0,255)      // ��� (Ʒ�� õ���)  
#define COLOR_FUCHSIA               RGB(255,0,255)      // ��������(�Ϻ�ɫ)  
#define COLOR_DARKMAGENTA           RGB(139,0,139)      // �����  
#define COLOR_PIGRED                RGB(123,15,31)		// ��Ѫ��
#define COLOR_PURPLE                RGB(128,0,128)      // ��ɫ  
#define COLOR_MEDIUMORCHID          RGB(186,85,211)     // ��������  
#define COLOR_DARKVIOLET            RGB(148,0,211)      // ��������  
#define COLOR_DARKORCHID            RGB(153,50,204)     // ��������  
#define COLOR_INDIGO                RGB(75,0,130)      // ���� (����ɫ)  
#define COLOR_BLUEVIOLET            RGB(138,43,226)     // ��������  
#define COLOR_MEDIUMPURPLE          RGB(147,112,219)    // ����ɫ  
#define COLOR_MEDIUMSLATEBLUE       RGB(123,104,238)    // �а�����  
#define COLOR_SLATEBLUE             RGB(106,90,205)     // ������  
#define COLOR_DARKSLATEBLUE         RGB(72,61,139)     // ��������  
#define COLOR_LAVENDER              RGB(230,230,250)    // Ѭ�²ݵ���  
#define COLOR_GHOSTWHITE            RGB(248,248,255)    // �����  
#define COLOR_BLUE                  RGB(0,0,255)		// ����  
#define COLOR_MEDIUMBLUE            RGB(0,0,205)		// ����ɫ  
#define COLOR_MIDNIGHTBLUE          RGB(25,25,112)		// ��ҹ��  
#define COLOR_DARKBLUE              RGB(0,0,139)		// ����ɫ  
#define COLOR_NAVY                  RGB(0,0,128)		// ������  
#define COLOR_ROYALBLUE             RGB(65,105,225)    // �ʼ��� (����)  
#define COLOR_CORNFLOWERBLUE        RGB(100,149,237)    // ʸ������  
#define COLOR_LIGHTSTEELBLUE        RGB(176,196,222)    // ������  
#define COLOR_LIGHTSLATEGRAY        RGB(119,136,153)    // ��ʯ���  
#define COLOR_SLATEGRAY             RGB(112,128,144)    // ʯ���  
#define COLOR_DODGERBLUE            RGB(30,144,255)    // ������  
#define COLOR_ALICEBLUE             RGB(240,248,255)    // ����˿��  
#define COLOR_STEELBLUE             RGB(70,130,180)    // ���� (����)  
#define COLOR_LIGHTSKYBLUE          RGB(135,206,250)    // ������ɫ  
#define COLOR_SKYBLUE               RGB(135,206,235)    // ����ɫ  
#define COLOR_DEEPSKYBLUE           RGB(0,191,255)		// ������  
#define COLOR_LIGHTBLUE             RGB(173,216,230)    // ����  
#define COLOR_POWDERBLUE            RGB(176,224,230)    // ��ҩ��  
#define COLOR_CADETBLUE             RGB(95,158,160)    // ������  
#define COLOR_AZURE                 RGB(240,255,255)    // ε��ɫ  
#define COLOR_LIGHTCYAN             RGB(224,255,255)    // ����ɫ  
#define COLOR_PALETURQUOISE         RGB(175,238,238)    // ���̱�ʯ  
#define COLOR_CYAN                  RGB(0,255,255)      // ��ɫ  
#define COLOR_AQUA                  RGB(0,255,255)      // ˮɫ  
#define COLOR_DARKTURQUOISE         RGB(0,206,209)      // ���̱�ʯ  
#define COLOR_DARKSLATEGRAY         RGB(47,79,79)      // ��ʯ���  
#define COLOR_DARKCYAN              RGB(0,139,139)      // ����ɫ  
#define COLOR_TEAL                  RGB(0,128,128)      // ˮѼɫ  
#define COLOR_MEDIUMTURQUOISE       RGB(72,209,204)    // ���̱�ʯ  
#define COLOR_LIGHTSEAGREEN         RGB(32,178,170)    // ǳ������  
#define COLOR_TURQUOISE             RGB(64,224,208)    // �̱�ʯ  
#define COLOR_AQUAMARINE            RGB(127,255,212)    // ��ʯ����  
#define COLOR_MEDIUMAQUAMARINE      RGB(102,205,170)    // �б�ʯ����  
#define COLOR_MEDIUMSPRINGGREEN     RGB(0,250,154)      // �д���ɫ  
#define COLOR_MINTCREAM             RGB(245,255,250)    // ��������  
#define COLOR_SPRINGGREEN           RGB(0,255,127)      // ����ɫ  
#define COLOR_MEDIUMSEAGREEN        RGB(60,179,113)    // �к�����  
#define COLOR_SEAGREEN              RGB(46,139,87)     // ������  
#define COLOR_HONEYDEW              RGB(240,255,240)    // �۹�ɫ  
#define COLOR_LIGHTGREEN            RGB(144,238,144)    // ����ɫ  
#define COLOR_PALEGREEN             RGB(152,251,152)    // ����ɫ  
#define COLOR_DARKSEAGREEN          RGB(143,188,143)    // ��������  
#define COLOR_LIMEGREEN             RGB(50,205,50)     // ��������  
#define COLOR_LIME                  RGB(0,255,0)		// ������  
#define COLOR_FORESTGREEN           RGB(34,139,34)     // ɭ����  
#define COLOR_GREEN_HALF            RGB(0,128,0)		// ����  
#define COLOR_DARK_GREEN             RGB(0,100,0)		// ����ɫ  
#define COLOR_CHARTREUSE            RGB(127,255,0)      // ���ؾ��� (����ɫ)  
#define COLOR_LAWNGREEN             RGB(124,252,0)      // ��ƺ��  
#define COLOR_GREENYELLOW           RGB(173,255,47)     // �̻�ɫ  
#define COLOR_DARKOLIVEGREEN        RGB(85,107,47)     // �������  
#define COLOR_YELLOWGREEN           RGB(154,205,50)     // ����ɫ  
#define COLOR_OLIVEDRAB             RGB(107,142,35)     // ��魺�ɫ  
#define COLOR_BEIGE                 RGB(245,245,220)    // ��ɫ(����ɫ)  
#define COLOR_LIGHTGOLDENRODYELLOW  RGB(250,250,210)    // ���ջ�  
#define COLOR_IVORY                 RGB(255,255,240)    // ����  
#define COLOR_LIGHTYELLOW           RGB(255,255,224)    // ǳ��ɫ  
#define COLOR_YELLOW                RGB(255,255,0)      // ����  
#define COLOR_OLIVE                 RGB(128,128,0)      // ���  
#define COLOR_DARKKHAKI             RGB(189,183,107)    // �ߴ��  
#define COLOR_LEMONCHIFFON          RGB(255,250,205)    // ���ʳ�  
#define COLOR_PALEGOLDENROD         RGB(238,232,170)    // �Ҿջ�  
#define COLOR_KHAKI                 RGB(240,230,140)    // ��ߴ��  
#define COLOR_GOLD                  RGB(255,215,0)      // ��ɫ  
#define COLOR_CORNSILK              RGB(255,248,220)    // ����˿ɫ  
#define COLOR_GOLDENROD             RGB(218,165,32)     // ��ջ�  
#define COLOR_DARKGOLDENROD         RGB(184,134,11)     // ����ջ�  
#define COLOR_FLORALWHITE           RGB(255,250,240)    // ���İ�ɫ  
#define COLOR_OLDLACE               RGB(253,245,230)    // ����˿  
#define COLOR_WHEAT                 RGB(245,222,179)    // С��ɫ  
#define COLOR_MOCCASIN              RGB(255,228,181)    // ¹Ƥѥ  
#define COLOR_ORANGE                RGB(255,165,0)      // ��ɫ  
#define COLOR_PAPAYAWHIP            RGB(255,239,213)    // ��ľ��  
#define COLOR_BLANCHEDALMOND        RGB(255,235,205)    // ���׵�����ɫ  
#define COLOR_NAVAJOWHITE           RGB(255,222,173)    // ������  
#define COLOR_ANTIQUEWHITE          RGB(250,235,215)    // �Ŷ���  
#define COLOR_TAN                   RGB(210,180,140)    // ��ɫ  
#define COLOR_BURLYWOOD             RGB(222,184,135)    // Ӳľɫ  
#define COLOR_BISQUE                RGB(255,228,196)    // ������  
#define COLOR_DARK_ORANGE           RGB(255,140,0)      // ���ɫ  
#define COLOR_LINEN                 RGB(250,240,230)    // ���鲼  
#define COLOR_PERU                  RGB(205,133,63)     // ��³  
#define COLOR_PEACHPUFF             RGB(255,218,185)    // ����ɫ  
#define COLOR_SANDYBROWN            RGB(244,164,96)     // ɳ��ɫ  
#define COLOR_CHOCOLATE             RGB(210,105,30)     // �ɿ���  
#define COLOR_SADDLEBROWN           RGB(139,69,19)      // ����ɫ  
#define COLOR_SEASHELL              RGB(255,245,238)    // ������  
#define COLOR_SIENNA                RGB(160,82,45)      // ������ɫ  
#define COLOR_LIGHTSALMON           RGB(255,160,122)    // ǳ������ɫ  
#define COLOR_CORAL                 RGB(255,127,80)     // ɺ��  
#define COLOR_RED_ORANGE            RGB(255,69,0)		// �Ⱥ�ɫ  
#define COLOR_DARKSALMON            RGB(233,150,122)    // ������(����)ɫ  
#define COLOR_TOMATO                RGB(255,99,71)      // ���Ѻ�  
#define COLOR_MISTYROSE             RGB(255,228,225)    // ����õ��  
#define COLOR_SALMON                RGB(250,128,114)    // ����(����)ɫ  
#define COLOR_SNOW                  RGB(255,250,250)    // ѩ  
#define COLOR_LIGHTCORAL            RGB(240,128,128)    // ��ɺ��ɫ  
#define COLOR_ROSYBROWN             RGB(188,143,143)    // õ����ɫ  
#define COLOR_INDIANRED             RGB(205,92,92)      // ӡ�Ⱥ�  
#define COLOR_RED                   RGB(255,0,0)		// ����  
#define COLOR_BROWN                 RGB(165,42,42)      // ��ɫ  
#define COLOR_FIREBRICK             RGB(178,34,34)      // �ͻ�ש  
#define COLOR_DARKRED               RGB(139,0,0)		// ���ɫ  
#define COLOR_MAROON                RGB(128,0,0)		// ��ɫ  
#define COLOR_WHITESMOKE            RGB(245,245,245)    // ����  
#define COLOR_GAINSBORO             RGB(220,220,220)    // ��˹���޻�ɫ  
#define COLOR_LIGHTGREY             RGB(211,211,211)    // ǳ��ɫ  
#define COLOR_SILVER                RGB(192,192,192)    // ����ɫ  
#define COLOR_DARKGRAY              RGB(169,169,169)    // ���ɫ  
#define COLOR_GRAY                  RGB(186,183,186)    // ��ɫ(*)  
#define COLOR_DIMGRAY               RGB(105,105,105)    // �����Ļ�ɫ  
#define COLOR_BLACK                 RGB(0,0,0)			// ����
#define COLOR_WHITE                 RGB(255,255,255)    // ����  
#define COLOR_DARK_SLATE_GREY		RGB(150,255,255) 
#define COLOR_GREY_WHITE			RGB(160,160,160)	// �Ұ�ɫ

#ifndef SAFE_DELETE
    #define SAFE_DELETE(p)			{	if(p) { delete (p);						(p)=NULL; }	}
    #define SAFE_DELETE_ARRAY(p)	{	if(p) { delete [] (p);					(p)=NULL; }	}
    #define SAFE_CLOSE_HANDLE(p)	{	if(p) { ::CloseHandle(p);				(p)=NULL; }	}
    #define SAFE_DELETE_OBJECT(p)	{	if(p) { ::DeleteObject((HGDIOBJ) p);	(p)=NULL; }	}
	#define SAFE_FREE(p)			{	if(p) { ::free(p);						(p)=NULL; } }
	#define SAFE_FREE_LIBRARY(p)	{	if(p) { ::FreeLibrary(p);				(p)=NULL; } }
#endif	/* SAFE_DELETE */

#define POINTER_ASSERT(p) { if(!p) return; }

#define STATUS_OFFLINE  1	// ����ģʽ
#define STATUS_RUNNING  2	// ����״̬
#define STATUS_STANDBY  3	// ����״̬
#define STATUS_OPENNING 4	// ����״̬
#define STATUS_PAUSE    5	// ��ͣ״̬

#define WORKSTATION_NULL   0 
#define WORKSTATION_TJ     1 
#define WORKSTATION_OTP    2
#define WORKSTATION_JC     3 
#define WORKSTATION_EX1    4 
#define WORKSTATION_EX2    5 

#define SET_BIT(x,y) x|=(1<<y)  //��X�ĵ�Y��λ����1
#define CLR_BIT(x,y) x&= ~(1<<y) //��X�ĵ�Y��λ����0

#define ARROWKEY_LEFT  1
#define ARROWKEY_RIGHT 2
#define ARROWKEY_UP    3
#define ARROWKEY_DOWN  4

#define DATA_DEC        0 //ʮ����
#define	DATA_HEX_WORD   1 //ʮ������
#define	DATA_HEX_BYTE   2 //0xAA
#define	DATA_HEX_DWORD  3 //0xAAAA
#define	DATA_STRING     4

#define THROW_I2C_ERROR 1949 //����I2Cͨ�Ŵ�����׳���ֵ

#define SAVETOSQL_STEP_INIT 0  //�洢�Ľ׶� ��ʼ��ʧ�ܣ�û�е���ģ��
#define SAVETOSQL_STEP_RUN  1 //����ģ��

enum I2CMODE_TRULY
{
	I2CMODE_8_8 = 0,	// I2CMODE_NORMAL 8 bit addr,8 bit value 
	I2CMODE_8_8_S,		// I2CMODE_SAMSUNG 8 bit addr,8 bit value,Stopen 
	I2CMODE_8_16,		// I2CMODE_MICRON  8 bit addr,16 bit value 
	I2CMODE_16_8,		// I2CMODE_STMICRO 16 bit addr,8 bit value, (eeprom also)
	I2CMODE_16_16,	    // I2CMODE_MICRON2 16 bit addr,16 bit value
	I2CMODE_D_8_8,       //I2CMODE_DIRECT_A8V8 no register direct ֱ�Ӷ�д
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

#define DS_INIT 0 //����ʱģ��ִֻ�е��˳�ʼ������
#define DS_ALL 1  //����ʱģ������ִ�����
#define DS_BREAK 2 //����ʱģ���쳣�ж�

// 20160606
#define MainCaption						_T("HTHC")			// ��˾��
#define MainBackground					_T("HTHC")			// ��˾��������
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