#pragma once

/*
	一个通信包包含 包头(1字节)0x03 版本(2字节) 命令类型(2字节) 用户ID(4字节)
*/

#ifdef __cplusplus
	extern "C"	{
#endif

#include "lib/arraylist.h"
#include "lib/linkhash.h"
#include "lib/automem.h"

#include <stddef.h>

#if defined(_M_IX86) || defined(__i386__) || defined(_M_X64) || defined(__x86_64__) || defined(__MIPSEL__) || defined(__ARMEL__)
	//小端平台 分别是 X86 X64 MIPS ARM
	#define FMT_PROTOCOL_VERSION		0x01
	#define LITTLEENDIANNESS
#elif defined(__MIPSEB__) || defined(__ARMEB__)
	//大端平台 分别是 MIPS ARM
	#define FMT_PROTOCOL_VERSION		0x0100
	#define BIGENDIANNESS
#endif

#if defined(_MSC_VER)
	// VC编译器
	typedef unsigned __int64	__UInt64;
#else
	// GCC编译器
	typedef long long __int64;
	typedef unsigned long long		__UInt64;
#endif

// 平台类型
#if defined(_M_IX86) || defined(__i386__)
	#define PLATFORM_TYPE		1
#elif defined(_M_X64) || defined(__x86_64__)
	#define PLATFORM_TYPE		2
#elif defined(__MIPSEL__)
	#define PLATFORM_TYPE		3
#elif __ARMEL__
	#define PLATFORM_TYPE		4
#elif __MIPSEB__
	#define PLATFORM_TYPE		5
#elif __ARMEB__
	#define PLATFORM_TYPE		6
#endif

enum{
	PDT_BYTE     = 0x01,
	PDT_SHORT    = 0x02,
	PDT_USHORT   = 0x03,
	PDT_INTEGER  = 0x04,
	PDT_UINTEGER = 0x05,
	PDT_LONG     = 0x06,
	PDT_ULONG    = 0x07,
	PDT_DOUBLE   = 0x08,
	PDT_STRING   = 0x09,

	//复合数据类型
	PDT_DATETIME = 0x0A, //日期时间
	PDT_ARRAY    = 0x0B,	 //数组
	PDT_OBJECT   = 0x0C,	 //对象

	_PDT_RAW = 0xFE, //原始数据类型
	_PDT_END = 0xFF, //结尾标识符
};


#define fmt_isarray(a)		\
	( a->t == PDT_ARRAY)

#define fmt_isobject(a)		\
	( a->t == PDT_OBJECT)

#define fmt_isstring(a)		\
	( a->t == PDT_STRING)

#define _fmt_israw( a )		\
	(a->t == _PDT_RAW)


#define MASK_DATA_CMDID    (0x01 << 15 | 0x00)
#define MAKE_DATA_CMDID(v) (MASK_DATA_CMDID | v)

/* 通信协议包类型 */
enum
{
	PT_LOGINTOUCH		= 0x01,
	PT_TOUCH_SUCCESS	= 0x02, //登录被允许 返回一个对象 包含2个成员 分别是 ip(IP地址) 和 port(端口)

// 以下包类型是TCP链接中才有的.
	PT_LOGIN		= 0x03,	//请求登录,提供网吧 或者茶楼信息.
	PT_LOGINENABLE	= 0x04,	// 登录被允许。
	PT_LOGINDISABLE = 0x05,	// 登录被禁止 返回一个对象 包含code(错误代码),message(错误消息)

	// 客户端提交过来的心跳包.服务器返回 PONG
	PT_PING = 0x06,		//PING 客户端发送的心跳包
	PT_PONG = 0x07,		//PING 服务器回应的心跳包

	PT_SERVER_IS_BUSY = 0x08,	//服务器正忙,无法处理您的提交请求.

	PT_RECIVER_OK	= 0x09,		//服务器已经成功接收到数据.

	// 升级通道,平台向服务器发送升级查询,带上平台已有的包信息.
	// 服务器返回 需要升级的包信息 以及 需要删除的包信息.
	PT_MODULE_INFO		= 0x0A, //客户端发送模块信息
	PT_MODULE_UPDATE	= 0x0B, // 服务器检查得到需要升级的包，并通过这个指令返回
	PT_MODULE_REMOVE	= 0x0C, // 服务器返回需要删除的软件包


	// 安装向导向服务器查询场所信息
	PT_QUERY_CORPINFO = 0x0D,
	// 服务器返回安装信息给向导
	PT_CORP_INFO = 0x0E,
	//中间服务器向客户端下发网吧唯一码.
	PT_CORPCODE = 0x0F,
	// 用户必须登录后才能提交数据
	PT_NEED_LOGIN = 0x10,

	// 设置主机root用户密码
	PT_SET_ROOT_PASSWORD = 0x11,

	//服务器构建升级包失败.
	PT_MODULE_UPDATE_ERR = 0x12,

	//挂接数据转发. 内部业务数据转发使用.
	PT_ADD_MONITOR = 0x13,
	
	//重启路由器
	PT_REBOOT_ROUTER = 0x14,

	// 中心服务器发给中间服务器的命令，要求后者启动升级流程（即回复PT_MODULE_INFO）
	PT_MODULE_REQUIRE = 0x15,

	// 服务器回应给客户端的命令. 数据包有错误的情况下返回
	
	// 注销. 本命令不建议客户端发出。服务器在检测到客户端断开后自动产生事件.
	_PT_LOGOUT  = 0xEF,

	PT_ERROR_DATA = 0x01FF,


	//未知命令
	PT_UNKNOW_DATA = 0xFF,

	// ==QQ聊天记录包类型.
	// 提交QQ聊天记录，类型是一个“对象”，包含内容自定 至少包含 当前QQ，对方QQ号，消息内容。是否为群消息。
	// 如果是群消息,则还需要包含群号码，对方QQ号为群内消息发送者的QQ号。
	// 一条消息为一个对象，可以是数组，包含多条消息(多个对象)
	PT_COMMIT_QQMSG	= MAKE_DATA_CMDID( 0x01 ),
	// ==提交页面访问记录
	// 包含 URL地址 提交方法,如果是 POST 则需要包含POST信息（POST以1条文本存储）
	PT_COMMIT_URL = MAKE_DATA_CMDID( 0x02 ),
	//客户端上机数据.
	PT_COMMIT_ONLINE = MAKE_DATA_CMDID( 0x03 ),
	//客户端下机数据.
	PT_COMMIT_OFFLINE = MAKE_DATA_CMDID( 0x04 ),
	
	// 顾客 入住/退房 数据,
	PT_COMMIT_CHECKINOUT = MAKE_DATA_CMDID( 0x05 ),

	// 场所运行状态
	PT_COMMIT_STATUS = MAKE_DATA_CMDID( 0x06 ),

	// 发帖回帖
	PT_COMMIT_POST = MAKE_DATA_CMDID( 0x07 ),

	// 虚拟帐号上下线（其中PT_COMMIT_VID_UPDATE仅在客户端内部使用）
	PT_COMMIT_VID_UPDATE  = MAKE_DATA_CMDID( 0x0A ),
	PT_COMMIT_VID_ONLINE  = MAKE_DATA_CMDID( 0x0B ),
	PT_COMMIT_VID_OFFLINE = MAKE_DATA_CMDID( 0x0C ),

};

typedef struct FMT_s FMT;
typedef void (fmt_object_delete_fn)(FMT *o);
typedef int (fmt_object_serialize_fn)(FMT * fmt,automem_t * pmem);

enum{
	WRONG_MAC_CODE,
	WRONG_PLATFORM_CODE,
	WRONG_DB_CONNECTION,
	WRONG_CAFE_ID,
	WRONG_CAFE_ID_INVALID,
};
struct FMT_s
{
	unsigned char t; //数据类型  
	int _ref;		//引用计数  
	union{
		unsigned char m_byte;
		short m_short;
		unsigned short m_ushort;
		int m_int;
		unsigned int m_uint;
		__int64 m_long;
		__UInt64 m_ulong;
		double m_double;
		__int64 m_date;
		struct array_list* m_array;
		struct lh_table * m_object;
		struct {
			unsigned int m_slen;
			unsigned char * m_str;
		};
	};
	fmt_object_delete_fn *_delete;
	fmt_object_serialize_fn * _serialize;

};

struct FMTParserState_s
{
	FMT * result;		//解析结果
	int is_Server;		//是否为服务器数据包
	int t;				//解析状态 仅仅在扫描时使用.
	//
	unsigned short m_ver;	//数据包版本
	unsigned short m_cmd;	//包类型
	unsigned int m_userId;	//用户ID
	unsigned int m_dLen;	//数据长度
	int lite;
};
typedef struct FMTParserState_s FMTParserState;

/*

*/
int fmt_parser_push(FMTParserState* st,unsigned char * buffer,unsigned int size);

//引用计数操作
FMT * fmt_object_get(FMT* fmt);
void fmt_object_put(FMT* fmt);

//创建文本类型
FMT * fmt_new_string(const unsigned char * str,unsigned int len);
//创建字节类型
FMT * fmt_new_byte(unsigned char bt);
//创建短整数类型
FMT * fmt_new_short(short val);
FMT * fmt_new_ushort(unsigned short val);
//创建整数类型
FMT * fmt_new_integer(int val);
FMT * fmt_new_uinteger(unsigned int val);
//创建长整数类型
FMT * fmt_new_long(__int64 val);
FMT * fmt_new_ulong(__UInt64 val);
//创建浮点类型
FMT * fmt_new_double(double val);

//创建日期时间类型 传递Unix时间戳
FMT * fmt_new_datetime(__int64 val);

/* 创建一个原始数据类型对象 */
FMT* _fmt_new_rawdata(void* data,unsigned int length);

//创建数组类型
FMT * fmt_new_array();
unsigned int fmt_array_length(FMT * fmt);
void fmt_array_append(FMT * fmt,FMT * item);
void fmt_array_remove(FMT * fmt,unsigned int idx);
FMT * fmt_array_get_idx(FMT * fmt,unsigned int idx);



//创建对象型
FMT* fmt_new_object();
void fmt_object_add(FMT* fmt, const char* key, FMT* val);
void fmt_object_remove(FMT * fmt, const char * key);
FMT * fmt_object_lookup(FMT * fmt, const char * key);
int fmt_object_total(FMT * fmt);

//本函数用于构建数据包
int fmt_packet_build(automem_t * pmem, FMT * fmt, short cmd, unsigned int userId);
int fmt_packet_build_header(automem_t * pmem, short cmd, unsigned int userId, unsigned int dataSize);

//本函数用于服务器端构建数据包
int fmt_packet_build_server(automem_t * pmem,FMT * fmt,short cmd);



/// 包解析器
/// 创建包解析器
FMTParserState * fmt_parser_new_state(int is_Server );
FMTParserState * fmt_parser_new_state_full(int is_Server );
/// 销毁包解析器
void fmt_parser_close_state(FMTParserState * st);
///将接收到的数据压入包解析器。返回包解析器成功扫描的字节数。
int fmt_parser_push(FMTParserState* st,unsigned char * buffer,unsigned int size);

///判断是否解析了一个完整的数据包
int fmt_parser_complete(FMTParserState * st);

///如果成功解析了1个数据包，则从数据包中获取数据对象.
FMT * fmt_parser_take(FMTParserState * st);


/// 重置包解析器状态,准备进行下一次解析。
void fmt_parser_reset(FMTParserState * st);


/// 快速解析一个数据对象.
FMT * fmt_quick_parse(char * data,int length);
/// 快速 序列化一个数据对象.
int fmt_quick_serial(automem_t * pmem,FMT * fmt);

unsigned int crc32(unsigned int  crc, const void *buf, size_t size);

const char * byte2hex(unsigned char b);
#ifdef __cplusplus
};
#endif
