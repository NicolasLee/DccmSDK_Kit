#pragma once


#ifndef SOCKET_MSG

#pragma pack(push, 1)   // �ڴ��ж��뷽ʽ(1�ֽ�) ��������������һ��
typedef struct
{
	int  CmdID;			// ָ�� 1~99Ϊ������ĿID��100~199�������ر�ģ�顢�������200~300��ȡ������Ŀ��Ϣ
	int  Status;		// ��Ӧ����0ִ������-1��ִ������
	int  Result;		// ��Ӧ���:0 OK, 1 NG, -1 ����
	char ErrInfo[256];  // ������Ϣ�ַ���
	char TestData[256]; // ��������
}SockMsg;
#pragma pack(pop)

#define SM_BEGIN					0	// ����ֵ����
#define SM_END						400	// ����ֵ����

// command
#define SM_CMD_CAM_START			101
#define SM_CMD_CAM_STOP				102
#define SM_CMD_GET_INSP_NUM			200	// ��ȡ��������Ŀ
#define SM_CMD_CLOSE_EXE			300	// �رղ��Գ���
#define SM_CMD_QUERY_STATUS			301	// ��ѯ���Գ���״̬��
#define SM_CMD_GET_FINAL_RESULT		302	// ��ȡ���ղ��Խ��

// Status
// Status - MC����
#define SM_STATUS_INSP_START		10	// ����ʼ����
#define SM_STATUS_INSP_STOP			11	// ������ֹ����
#define SM_STATUS_INSP_CONTINUE		12	// �������ִ�У����Ƶ���OK��
// Status - Tester��Ӧ
#define SM_STATUS_EXECUTE			20	// ��Ӧִ��:������CAM/INSP
#define SM_STATUS_NO_EXECUTE		21	// ����Ӧִ��::������CAM/INSP
#define SM_STATUS_INSP_END			22	// ִ�����

// result
#define SM_RESULT_OK				0	
#define SM_RESULT_NG				1	
#define SM_RESULT_CAM_STANDBY		2	// ģ�������û����
#define SM_RESULT_CAM_RUNNING		3	// ģ��������������
#define SM_RESULT_INSP_RUNNING		4	// ��������ڲ�����
#define SM_RESULT_INSP_EXIST		5	// ���������
#define SM_RESULT_INSP_NO_EXIST		6	// �����������
#define SM_RESULT_CAM_PAUSE			7	// ģ�������û����
#define SM_RESULT_CAM_OFFLINE		8	// ģ�������û����
#define SM_RESULT_CAM_OPENNING		9	// ģ�������û����

// common command
#define SM_INVLID					-1	// ��Ч
#define SM_INVLID_STR				L""	// ��Ч

#endif	/* SOCKET_MSG */

#ifndef SE_SOCKET
#define SE_OK                 0
#define SE_NOT_INIT          -1		// δ��ʼ��
#define SE_SEND_FAIL         -2		// ����ʧ��
#define SE_RECV_FAIL         -3		// ����ʧ��
#define SE_INIT_WINSOCK_FAIL -4		// ��ʼ��ʧ��
#define SE_FAIL_TO_CONNECT   -5		// ����ʧ��
#define SE_FAIL_TO_ACCEPT    -6		// ��������ʧ��
#define SE_ERROR_SOCKET      -7		// ����ʧ��
#define SE_SOCKET_DISCONNECT -8		// �����жϣ�ʧȥ����
#define SE_TIME_OUT          -9		// ���ӳ�ʱ
#define SE_SOCKET_ERROR      -10	// 
#define SE_BUFFER_OVERFLOW   -11	// Buffer���
#define SE_UNINITIALIZED	 -12	// ���ӳ�ʼ��ʧ��
#define SE_INIT_SOCKET_FAIL  -13	// ��ʼ��ʧ��
#define SE_INIT_FAIL         -14	// ��ʼ��ʧ��
#define SE_INFORMAL_MSG		 -15	// �յ���Э���ʽ������
#endif	/*SE_SOCKET*/