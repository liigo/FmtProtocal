#pragma once

/*
	һ��ͨ�Ű����� ��ͷ(1�ֽ�)0x03 �汾(2�ֽ�) ��������(2�ֽ�) �û�ID(4�ֽ�)
*/

#ifdef __cplusplus
	extern "C"	{
#endif

#include "lib/arraylist.h"
#include "lib/linkhash.h"
#include "lib/automem.h"

#include <stddef.h>

#if defined(_M_IX86) || defined(__i386__) || defined(_M_X64) || defined(__x86_64__) || defined(__MIPSEL__) || defined(__ARMEL__)
	//С��ƽ̨ �ֱ��� X86 X64 MIPS ARM
	#define FMT_PROTOCOL_VERSION		0x01
	#define LITTLEENDIANNESS
#elif defined(__MIPSEB__) || defined(__ARMEB__)
	//���ƽ̨ �ֱ��� MIPS ARM
	#define FMT_PROTOCOL_VERSION		0x0100
	#define BIGENDIANNESS
#endif

#if defined(_MSC_VER)
	// VC������
	typedef unsigned __int64	__UInt64;
#else
	// GCC������
	typedef long long __int64;
	typedef unsigned long long		__UInt64;
#endif

// ƽ̨����
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

	//������������
	PDT_DATETIME = 0x0A, //����ʱ��
	PDT_ARRAY    = 0x0B,	 //����
	PDT_OBJECT   = 0x0C,	 //����

	_PDT_RAW = 0xFE, //ԭʼ��������
	_PDT_END = 0xFF, //��β��ʶ��
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

/* ͨ��Э������� */
enum
{
	PT_LOGINTOUCH		= 0x01,
	PT_TOUCH_SUCCESS	= 0x02, //��¼������ ����һ������ ����2����Ա �ֱ��� ip(IP��ַ) �� port(�˿�)

// ���°�������TCP�����в��е�.
	PT_LOGIN		= 0x03,	//�����¼,�ṩ���� ���߲�¥��Ϣ.
	PT_LOGINENABLE	= 0x04,	// ��¼������
	PT_LOGINDISABLE = 0x05,	// ��¼����ֹ ����һ������ ����code(�������),message(������Ϣ)

	// �ͻ����ύ������������.���������� PONG
	PT_PING = 0x06,		//PING �ͻ��˷��͵�������
	PT_PONG = 0x07,		//PING ��������Ӧ��������

	PT_SERVER_IS_BUSY = 0x08,	//��������æ,�޷����������ύ����.

	PT_RECIVER_OK	= 0x09,		//�������Ѿ��ɹ����յ�����.

	// ����ͨ��,ƽ̨�����������������ѯ,����ƽ̨���еİ���Ϣ.
	// ���������� ��Ҫ�����İ���Ϣ �Լ� ��Ҫɾ���İ���Ϣ.
	PT_MODULE_INFO		= 0x0A, //�ͻ��˷���ģ����Ϣ
	PT_MODULE_UPDATE	= 0x0B, // ���������õ���Ҫ�����İ�����ͨ�����ָ���
	PT_MODULE_REMOVE	= 0x0C, // ������������Ҫɾ���������


	// ��װ�����������ѯ������Ϣ
	PT_QUERY_CORPINFO = 0x0D,
	// ���������ذ�װ��Ϣ����
	PT_CORP_INFO = 0x0E,
	//�м��������ͻ����·�����Ψһ��.
	PT_CORPCODE = 0x0F,
	// �û������¼������ύ����
	PT_NEED_LOGIN = 0x10,

	// ��������root�û�����
	PT_SET_ROOT_PASSWORD = 0x11,

	//����������������ʧ��.
	PT_MODULE_UPDATE_ERR = 0x12,

	//�ҽ�����ת��. �ڲ�ҵ������ת��ʹ��.
	PT_ADD_MONITOR = 0x13,
	
	//����·����
	PT_REBOOT_ROUTER = 0x14,

	// ���ķ����������м�����������Ҫ����������������̣����ظ�PT_MODULE_INFO��
	PT_MODULE_REQUIRE = 0x15,

	// ��������Ӧ���ͻ��˵�����. ���ݰ��д��������·���
	
	// ע��. ���������ͻ��˷������������ڼ�⵽�ͻ��˶Ͽ����Զ������¼�.
	_PT_LOGOUT  = 0xEF,

	PT_ERROR_DATA = 0x01FF,


	//δ֪����
	PT_UNKNOW_DATA = 0xFF,

	// ==QQ�����¼������.
	// �ύQQ�����¼��������һ�������󡱣����������Զ� ���ٰ��� ��ǰQQ���Է�QQ�ţ���Ϣ���ݡ��Ƿ�ΪȺ��Ϣ��
	// �����Ⱥ��Ϣ,����Ҫ����Ⱥ���룬�Է�QQ��ΪȺ����Ϣ�����ߵ�QQ�š�
	// һ����ϢΪһ�����󣬿��������飬����������Ϣ(�������)
	PT_COMMIT_QQMSG	= MAKE_DATA_CMDID( 0x01 ),
	// ==�ύҳ����ʼ�¼
	// ���� URL��ַ �ύ����,����� POST ����Ҫ����POST��Ϣ��POST��1���ı��洢��
	PT_COMMIT_URL = MAKE_DATA_CMDID( 0x02 ),
	//�ͻ����ϻ�����.
	PT_COMMIT_ONLINE = MAKE_DATA_CMDID( 0x03 ),
	//�ͻ����»�����.
	PT_COMMIT_OFFLINE = MAKE_DATA_CMDID( 0x04 ),
	
	// �˿� ��ס/�˷� ����,
	PT_COMMIT_CHECKINOUT = MAKE_DATA_CMDID( 0x05 ),

	// ��������״̬
	PT_COMMIT_STATUS = MAKE_DATA_CMDID( 0x06 ),

	// ��������
	PT_COMMIT_POST = MAKE_DATA_CMDID( 0x07 ),

	// �����ʺ������ߣ�����PT_COMMIT_VID_UPDATE���ڿͻ����ڲ�ʹ�ã�
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
	unsigned char t; //��������  
	int _ref;		//���ü���  
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
	FMT * result;		//�������
	int is_Server;		//�Ƿ�Ϊ���������ݰ�
	int t;				//����״̬ ������ɨ��ʱʹ��.
	//
	unsigned short m_ver;	//���ݰ��汾
	unsigned short m_cmd;	//������
	unsigned int m_userId;	//�û�ID
	unsigned int m_dLen;	//���ݳ���
	int lite;
};
typedef struct FMTParserState_s FMTParserState;

/*

*/
int fmt_parser_push(FMTParserState* st,unsigned char * buffer,unsigned int size);

//���ü�������
FMT * fmt_object_get(FMT* fmt);
void fmt_object_put(FMT* fmt);

//�����ı�����
FMT * fmt_new_string(const unsigned char * str,unsigned int len);
//�����ֽ�����
FMT * fmt_new_byte(unsigned char bt);
//��������������
FMT * fmt_new_short(short val);
FMT * fmt_new_ushort(unsigned short val);
//������������
FMT * fmt_new_integer(int val);
FMT * fmt_new_uinteger(unsigned int val);
//��������������
FMT * fmt_new_long(__int64 val);
FMT * fmt_new_ulong(__UInt64 val);
//������������
FMT * fmt_new_double(double val);

//��������ʱ������ ����Unixʱ���
FMT * fmt_new_datetime(__int64 val);

/* ����һ��ԭʼ�������Ͷ��� */
FMT* _fmt_new_rawdata(void* data,unsigned int length);

//������������
FMT * fmt_new_array();
unsigned int fmt_array_length(FMT * fmt);
void fmt_array_append(FMT * fmt,FMT * item);
void fmt_array_remove(FMT * fmt,unsigned int idx);
FMT * fmt_array_get_idx(FMT * fmt,unsigned int idx);



//����������
FMT* fmt_new_object();
void fmt_object_add(FMT* fmt, const char* key, FMT* val);
void fmt_object_remove(FMT * fmt, const char * key);
FMT * fmt_object_lookup(FMT * fmt, const char * key);
int fmt_object_total(FMT * fmt);

//���������ڹ������ݰ�
int fmt_packet_build(automem_t * pmem, FMT * fmt, short cmd, unsigned int userId);
int fmt_packet_build_header(automem_t * pmem, short cmd, unsigned int userId, unsigned int dataSize);

//���������ڷ������˹������ݰ�
int fmt_packet_build_server(automem_t * pmem,FMT * fmt,short cmd);



/// ��������
/// ������������
FMTParserState * fmt_parser_new_state(int is_Server );
FMTParserState * fmt_parser_new_state_full(int is_Server );
/// ���ٰ�������
void fmt_parser_close_state(FMTParserState * st);
///�����յ�������ѹ��������������ذ��������ɹ�ɨ����ֽ�����
int fmt_parser_push(FMTParserState* st,unsigned char * buffer,unsigned int size);

///�ж��Ƿ������һ�����������ݰ�
int fmt_parser_complete(FMTParserState * st);

///����ɹ�������1�����ݰ���������ݰ��л�ȡ���ݶ���.
FMT * fmt_parser_take(FMTParserState * st);


/// ���ð�������״̬,׼��������һ�ν�����
void fmt_parser_reset(FMTParserState * st);


/// ���ٽ���һ�����ݶ���.
FMT * fmt_quick_parse(char * data,int length);
/// ���� ���л�һ�����ݶ���.
int fmt_quick_serial(automem_t * pmem,FMT * fmt);

unsigned int crc32(unsigned int  crc, const void *buf, size_t size);

const char * byte2hex(unsigned char b);
#ifdef __cplusplus
};
#endif
