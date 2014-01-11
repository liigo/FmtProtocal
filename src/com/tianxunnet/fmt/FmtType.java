package com.tianxunnet.fmt;

/**
 * 描述Fmt对象的类型。Fmt.getType()返回的值必然是本Class中定义的常量值之一。
 * 只有确定Fmt类型（Fmt.getType()）的情况下才能调用相应的方法（Fmt.getXxx()等）读取其内容；
 * 禁止在不知道Fmt类型的前提下读取Fmt的内容。
 * @see Fmt#getType()
 * 
 * @author Liigo
 */
public class FmtType {
	public static final int _PDT_NONE = 0x00;

	/** 表示Fmt对象的类型是 byte */
	public static final int PDT_BYTE = 0x01; // 字节
	
	/** 表示Fmt对象的类型是 short */
	public static final int PDT_SHORT = 0x02; // 短整数
	
	/** 表示Fmt对象的类型是 unsigned short */
	public static final int PDT_USHORT = 0x03;
	
	/** 表示Fmt对象的类型是 int */
	public static final int PDT_INTEGER = 0x04; // 整数
	
	/** 表示Fmt对象的类型是 unsigned int */
	public static final int PDT_UINTEGER = 0x05;
	
	/** 表示Fmt对象的类型是 long */
	public static final int PDT_LONG = 0x06; // 长整数
	
	/** 表示Fmt对象的类型是 unsigned long */
	public static final int PDT_ULONG = 0x07;
	
	/** 表示Fmt对象的类型是 double */
	public static final int PDT_DOUBLE = 0x08; // 双精度小数
	
	/** 表示Fmt对象的类型是 byte[] 或 String */
	public static final int PDT_STRING = 0x09; // 二进制数据（或文本）
	
	/** 表示Fmt对象的类型是日期时间（距离 UTC 1970-1-1 00:00:00 的秒数） */
	public static final int PDT_DATETIME = 0x0A; // 日期时间

	/** 表示Fmt对象的类型是数组（数组成员均是相同类型的Fmt对象） */
	public static final int PDT_ARRAY = 0x0B; // 数组（可包含其他Fmt成员）
	
	/** 表示Fmt对象的类型是对象（有多个命名成员，均可以是各种类型的Fmt对象） */
	public static final int PDT_OBJECT = 0x0C; // 对象（可包含其他Fmt成员）

	public static final int _PDT_RAW = 0xFE; // 原始数据类型
	public static final int _PDT_END = 0xFF; // 结尾标识符
}
