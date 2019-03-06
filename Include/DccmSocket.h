#pragma once


#ifndef SOCKET_MSG

#pragma pack(push, 1)   // 内存中对齐方式(1字节) 需与测试软件定义一致
typedef struct
{
	int  CmdID;			// 指令 1~99为测试项目ID，100~199启动、关闭模组、调焦命令，200~300获取测试项目信息
	int  Status;		// 回应请求：0执行请求，-1不执行请求
	int  Result;		// 回应结果:0 OK, 1 NG, -1 继续
	char ErrInfo[256];  // 附加信息字符串
	char TestData[256]; // 测试数据
}SockMsg;
#pragma pack(pop)

#define SM_BEGIN					0	// 命令值下限
#define SM_END						400	// 命令值上限

// command
#define SM_CMD_CAM_START			101
#define SM_CMD_CAM_STOP				102
#define SM_CMD_GET_INSP_NUM			200	// 获取测试项数目
#define SM_CMD_CLOSE_EXE			300	// 关闭测试程序
#define SM_CMD_QUERY_STATUS			301	// 查询测试程序状态：
#define SM_CMD_GET_FINAL_RESULT		302	// 获取最终测试结果

// Status
// Status - MC请求
#define SM_STATUS_INSP_START		10	// 请求开始测试
#define SM_STATUS_INSP_STOP			11	// 请求终止测试
#define SM_STATUS_INSP_CONTINUE		12	// 请求继续执行（控制调整OK）
// Status - Tester回应
#define SM_STATUS_EXECUTE			20	// 响应执行:不区分CAM/INSP
#define SM_STATUS_NO_EXECUTE		21	// 不响应执行::不区分CAM/INSP
#define SM_STATUS_INSP_END			22	// 执行完成

// result
#define SM_RESULT_OK				0	
#define SM_RESULT_NG				1	
#define SM_RESULT_CAM_STANDBY		2	// 模组待机：没启动
#define SM_RESULT_CAM_RUNNING		3	// 模组启动：运行中
#define SM_RESULT_INSP_RUNNING		4	// 测试项：正在测试中
#define SM_RESULT_INSP_EXIST		5	// 测试项：存在
#define SM_RESULT_INSP_NO_EXIST		6	// 测试项：不存在
#define SM_RESULT_CAM_PAUSE			7	// 模组待机：没启动
#define SM_RESULT_CAM_OFFLINE		8	// 模组待机：没启动
#define SM_RESULT_CAM_OPENNING		9	// 模组待机：没启动

// common command
#define SM_INVLID					-1	// 无效
#define SM_INVLID_STR				L""	// 无效

#endif	/* SOCKET_MSG */

#ifndef SE_SOCKET
#define SE_OK                 0
#define SE_NOT_INIT          -1		// 未初始化
#define SE_SEND_FAIL         -2		// 发送失败
#define SE_RECV_FAIL         -3		// 接收失败
#define SE_INIT_WINSOCK_FAIL -4		// 初始化失败
#define SE_FAIL_TO_CONNECT   -5		// 连接失败
#define SE_FAIL_TO_ACCEPT    -6		// 接收连接失败
#define SE_ERROR_SOCKET      -7		// 接收失败
#define SE_SOCKET_DISCONNECT -8		// 连接中断，失去连接
#define SE_TIME_OUT          -9		// 连接超时
#define SE_SOCKET_ERROR      -10	// 
#define SE_BUFFER_OVERFLOW   -11	// Buffer溢出
#define SE_UNINITIALIZED	 -12	// 连接初始化失败
#define SE_INIT_SOCKET_FAIL  -13	// 初始化失败
#define SE_INIT_FAIL         -14	// 初始化失败
#define SE_INFORMAL_MSG		 -15	// 收到非协议格式的命令
#endif	/*SE_SOCKET*/