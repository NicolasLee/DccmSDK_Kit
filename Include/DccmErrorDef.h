#pragma once

// 测试项目结果
#define RESULT_INSPECTION_NULL				100// + itemID = 1~99 漏测
#define RESULT_INSPECTION_OK				200// + itemID = 1~99 OK
#define RESULT_INSPECTION_NG				300// + itemID = 1~99 NG
#define RESULT_INSPECTION_END				400

// 最终结果
#define RESULT_FINAL_NULL					8	// 没结果
#define RESULT_FINAL_PASS					7	// PASS
#define RESULT_FINAL_FAIL					6	// FAIL

// 关闭模组
#define RESULT_STOP_NULL					5	// 没关闭
#define RESULT_STOP_OK						4	// 关闭成功
#define RESULT_STOP_NG						3	// 关闭失败

// 启动模组
#define RESULT_PLAY_NULL					2	// 没启动
#define RESULT_PLAY_OK						1	// 启动成功
#define RESULT_PLAY_NG						0	// 启动失败

// 错误代码
#define ERROR_OK							0
#define ERROR_BOARD_OFFLINE					-1		// 测试盒未连接
#define ERROR_CAMERA_CHECK_ID_FAILED		-2
#define ERROR_CAMERA_PARAMETER_INIT_FAILED	-3
#define ERROR_OPEN_SHORT_NG					-4		// 开短路失败
#define ERROR_OPEN_SHORT_NO_FUNCTION		-5		// 测试盒不支持开短路测试
#define ERROR_STARTPROCESS_FAILED			-6      
#define ERROR_INSPECTION_INIT_FAILED		-7		// 测试项初始化失败（PS：ERROR_DLL_INIT_FAILED系统已有定义）
#define ERROR_DRIVERIC_INIT_FAILED			-8
#define ERROR_PREHANDLE_FAILED				-9		// 
#define ERROR_CHECK_CAMID_FAILED			-10
#define ERROR_MEMORY_ALLOC_FAILED			-11
#define ERROR_CAMERA_INIT_FAILED			-12		// 点亮失败：未定义原因
#define ERROR_READ_EXP_I2C					-13		// 读取exp reg失败
#define ERROR_CAMERA_I2C_FAILED				-14
#define ERROR_NO_LOAD_CONFIG				-15		// 没有加载参数
#define ERROR_GRAB_FRAME					-16		// 采集超时或者内部错误
#define ERROR_FAIL_FRAME_LIMIT_CNT			-17		// 错误帧数量超出限制
#define ERROR_SOFT_PIN_PULL_UP				-18		// 拉高软Pin失败
#define ERROR_SENSOR_ENABLE_FAIL			-19		// 使能Sensor失败
#define ERROR_SOCKET_OPEN					-20		// 治具处于打开状态
#define ERROR_SN_NUMBER						-21		// 设备SN号不正确
#define ERROR_CAMERA_CHECK_MES_FAILED		-22		
#define ERROR_CURRENT_WORKING				-23		// 工作电流 NG
#define ERROR_CURRENT_STANDBY				-24		// 待机电流 NG
#define ERROR_CAM_LOST_CONNECTED			-25		// 模组失去连接
#define ERROR_SENSOR_IS_NOT_ONLINE			-26		// 没有模组 sensor is not on line
#define ERROR_SENSOR_INIT_FAILED			-27
#define ERROR_UNDEFINED						-99

// 每个测试项目预留10个 Error Code 
#define ERROR_PARTICLE_POD					-100	// POD NG
#define ERROR_PARTICLE_POG					-101	// POD NG
#define ERROR_PARTICLE_POD_POG				-102	// POD NG

