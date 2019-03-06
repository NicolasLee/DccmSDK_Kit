#pragma once

// ������Ŀ���
#define RESULT_INSPECTION_NULL				100// + itemID = 1~99 ©��
#define RESULT_INSPECTION_OK				200// + itemID = 1~99 OK
#define RESULT_INSPECTION_NG				300// + itemID = 1~99 NG
#define RESULT_INSPECTION_END				400

// ���ս��
#define RESULT_FINAL_NULL					8	// û���
#define RESULT_FINAL_PASS					7	// PASS
#define RESULT_FINAL_FAIL					6	// FAIL

// �ر�ģ��
#define RESULT_STOP_NULL					5	// û�ر�
#define RESULT_STOP_OK						4	// �رճɹ�
#define RESULT_STOP_NG						3	// �ر�ʧ��

// ����ģ��
#define RESULT_PLAY_NULL					2	// û����
#define RESULT_PLAY_OK						1	// �����ɹ�
#define RESULT_PLAY_NG						0	// ����ʧ��

// �������
#define ERROR_OK							0
#define ERROR_BOARD_OFFLINE					-1		// ���Ժ�δ����
#define ERROR_CAMERA_CHECK_ID_FAILED		-2
#define ERROR_CAMERA_PARAMETER_INIT_FAILED	-3
#define ERROR_OPEN_SHORT_NG					-4		// ����·ʧ��
#define ERROR_OPEN_SHORT_NO_FUNCTION		-5		// ���Ժв�֧�ֿ���·����
#define ERROR_STARTPROCESS_FAILED			-6      
#define ERROR_INSPECTION_INIT_FAILED		-7		// �������ʼ��ʧ�ܣ�PS��ERROR_DLL_INIT_FAILEDϵͳ���ж��壩
#define ERROR_DRIVERIC_INIT_FAILED			-8
#define ERROR_PREHANDLE_FAILED				-9		// 
#define ERROR_CHECK_CAMID_FAILED			-10
#define ERROR_MEMORY_ALLOC_FAILED			-11
#define ERROR_CAMERA_INIT_FAILED			-12		// ����ʧ�ܣ�δ����ԭ��
#define ERROR_READ_EXP_I2C					-13		// ��ȡexp regʧ��
#define ERROR_CAMERA_I2C_FAILED				-14
#define ERROR_NO_LOAD_CONFIG				-15		// û�м��ز���
#define ERROR_GRAB_FRAME					-16		// �ɼ���ʱ�����ڲ�����
#define ERROR_FAIL_FRAME_LIMIT_CNT			-17		// ����֡������������
#define ERROR_SOFT_PIN_PULL_UP				-18		// ������Pinʧ��
#define ERROR_SENSOR_ENABLE_FAIL			-19		// ʹ��Sensorʧ��
#define ERROR_SOCKET_OPEN					-20		// �ξߴ��ڴ�״̬
#define ERROR_SN_NUMBER						-21		// �豸SN�Ų���ȷ
#define ERROR_CAMERA_CHECK_MES_FAILED		-22		
#define ERROR_CURRENT_WORKING				-23		// �������� NG
#define ERROR_CURRENT_STANDBY				-24		// �������� NG
#define ERROR_CAM_LOST_CONNECTED			-25		// ģ��ʧȥ����
#define ERROR_SENSOR_IS_NOT_ONLINE			-26		// û��ģ�� sensor is not on line
#define ERROR_SENSOR_INIT_FAILED			-27
#define ERROR_UNDEFINED						-99

// ÿ��������ĿԤ��10�� Error Code 
#define ERROR_PARTICLE_POD					-100	// POD NG
#define ERROR_PARTICLE_POG					-101	// POD NG
#define ERROR_PARTICLE_POD_POG				-102	// POD NG

