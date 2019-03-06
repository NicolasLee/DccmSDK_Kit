#pragma once

// 测试环境类型	TEST_ENV_TYPE
#define TET_NULL					0x0000 // 无定义

// 0xnn01 Sensor初始化
#define TET_INIT					0x0001
#define TET_INIT_AFTER_POWER_ON		0x0101 //用于SENSOR 上电后，Streaming前初始化
#define TET_INIT_AFTER_STREAM		0x0201 //用于SENSOR Streaming后初始化

// 0xnn02 远景
#define TET_INF						0x0002 //远景，适用于只分站位不分距离的情况
#define TET_INF_1M					0x0102
#define TET_INF_5M					0x0202
#define TET_INF_10M					0x0302

// 0xnn03 中景
#define TET_MID						0x0003 //中景，适用于只分站位不分距离的情况
#define TET_MID_15CM				0x0103
#define TET_MID_20CM				0x0203
#define TET_MID_30CM				0x0303
#define TET_MID_50CM				0x0403

// 0xnn04 近景
#define TET_MAC						0x0004 //近景，适用于只分站位不分距离的情况
#define TET_MAC_7CM					0x0104
#define TET_MAC_8CM					0x0204
#define TET_MAC_10CM				0x0304

// 0xnn05 白场
#define TET_WHITE					0x0005 //白场，适用于只分站位不分距离的情况
#define TET_WHITE_1CM				0x0105
#define TET_WHITE_2CM				0x0205

// 0xnn06 黑场
#define TET_BLACK					0x0006 //黑场，适用于只分站位不分距离的情况


// 0xnn07 特殊测试项目
#define TET_FOCUS					0x0007 //黑场，适用于只分站位不分距离的情况


#define TET_ALL						8		// 总共8个大类

