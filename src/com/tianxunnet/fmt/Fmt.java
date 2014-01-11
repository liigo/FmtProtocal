package com.tianxunnet.fmt;

import java.io.UnsupportedEncodingException;

/**
 * <p>类 Fmt 用于描述一个 Fmt 对象，其中可以存储整数、小数、文本，也可以存储Fmt对象或Fmt数组。
 * <p>Fmt对象是本网络协议最核心的组成部分，它是协议数据发送之前和接受之后，用于描述被传输数据的内容。
 * Fmt对象其序列化（{@link Fmt#packet(int, int)}）后的二进制数据流，直接用于网络发送；
 * 通过网络接收到二进制数据流，直接用于解析为Fmt对象（{@link FmtParser#pushData(byte[], OnFmtParsed)}）。
 * <p>
 * <p>Fmt对象的类型：一个Fmt对象只能存储一种类型的值，其类型可以是整数、小数、文本等基础类型，也
 * 可以是数组、对象等复合类型。我们用一个 int 值表示其类型，取值范围限于 FmtType.PDT_* 之一。
 * Fmt.getType()返回该Fmt对象的类型，是一个int值，如上所述。对于不同类型的Fmt对象，我们通常
 * 称之为“整数类型的Fmt或IntFmt”、“数组类型的Fmt或ArrayFmt”、“对象类型的Fmt或ObjectFmt”。
 * 
 * <p>Fmt对象的值：类型不同，读取值的方法也不同。 {@link #getType()}
 * 如果类型是整数（即Fmt.getType() == FmtType.PDT_INT），Fmt.getInt()返回其中的整数值；
 * 如果类型是小数（即Fmt.getType() == FmtType.PDT_Double），Fmt.getDouble()返回其中的小数值；
 * 其他基本类型也以此类推，至于数组或对象类型的Fmt，参见下文描述。
 * 特别的，如果类型是二进制数据流（即Fmt.getType() == FmtType.PDT_STRING），Fmt.getBinary()
 * 返回其中的二进制数据流（byte[]），而文本也被视为二进制流的特殊形式，即此时 Fmt.getString() 返回文本值；
 * 
 * <p>Fmt对象的创建：调用类静态方法 Fmt.newXxxFmt 系列方法分别创建不同类型的Fmt对象，如 Fmt.newObjectFmt() 等。
 * 
 * <p>Fmt对象的销毁：调用 fmt.decRef() 减小引用计数，必要时它会自动销毁。
 * 
 * <p>数组类型的Fmt对象：创建方法：Fmt.newArrayFmt()；添加数组成员：arrayAppend(fmt)；
 * 删除数组成员：arrayRemove(index)；取指定索引处的数组成员值：arrarAt(index)；取数组成员数：arrayLength()。
 * 
 * <p>对象类型的Fmt对象：创建方法：Fmt.newObjectFmt()；添加命名成员：addField(name,fmt), addIntField(name,i), ...；
 * 删除对象成员：delField(name)；取对象成员值：getField(name)；取对象成员数：getFieldCount()。
 * 
 * @author Liigo, 2013-12. 2014-1 补充文档。
 */
public class Fmt {
	static {
		System.loadLibrary("fmt-native");
	}

	private long fmtPtr; // stores Fmt* managed by C, see native source code.

	private Fmt(long fmtPtr) {
		this.fmtPtr = fmtPtr;
	}

// Create Fmt instances
	
	/** 创建一个对象类型的Fmt对象。此后可通过 addField()等方法添加对象成员。 */
	public static native Fmt newObjectFmt();

	/** 创建一个数组类型的Fmt对象。此后可通过 arrayAppend()方法添加数组成员。 */
	public static native Fmt newArrayFmt();

	/** 创建一个byte类型的Fmt对象，并存入一个byte值。此后可通过 getByte()方法读取存储的值 */
	public static native Fmt newByteFmt(byte b);

	/** 创建一个short类型的Fmt对象，并存入一个short值。此后可通过 getShort()方法读取存储的值 */
	public static native Fmt newShortFmt(short s);

	/** 创建一个int类型的Fmt对象，并存入一个int值。此后可通过 getInt()方法读取存储的值 */
	public static native Fmt newIntFmt(int i);

	/** 创建一个long类型的Fmt对象，并存入一个long值。此后可通过 getLong()方法读取存储的值 */
	public static native Fmt newLongFmt(long l);

	/** 创建一个float类型的Fmt对象，并存入一个float值。此后可通过 getFloat()方法读取存储的值 */
	public static native Fmt newFloatFmt(float f);

	/** 创建一个double类型的Fmt对象，并存入一个double值。此后可通过 getDouble()方法读取存储的值 */
	public static native Fmt newDoubleFmt(double d);

	/** 
	 * 创建一个日期时间类型的Fmt对象，并存入一个日期时间值。此后可通过 getDatetime()方法读取存储的日期时间值。
	 * 注意：本方法的参数单位是毫秒(ms)，即据UTC 1970-1-1 00:00:00的毫秒数，{@link System#currentTimeMillis()}。
	 * 但是 getDatetime() 方法返回的的日期时间值是以秒为单位的。（此处接收毫秒单位的参数，主要是为了方便Java调用。）
	 */
	public static native Fmt newDatetimeFmt(long millis);

	/** 创建一个二进制数据流类型的Fmt对象，并存入一个byte[]值。此后可通过 getBinary()方法读取存储的值 */
	public static native Fmt newBinaryFmt(byte[] data);

	/** 
	 * 创建一个文本类型的Fmt对象，并存入一个String值。此后可通过 getString()方法读取存储的值。
	 * 注意，此方法创建的Fmt对象本质上仍然是二进制数据流类型（getType()==FmtType.PDT_STRING）。
	 * @see #getString()
	 */
	public static Fmt newStringFmt(String str) {
		try {
			return newBinaryFmt(str.getBytes("UTF-8"));
		} catch (UnsupportedEncodingException e) {
			byte[] empty = {};
			return newBinaryFmt(empty);
		}
	}


// Manage Object Fmt fields

	/**
	 * 为“对象类型的Fmt对象”添加命名成员。成员名称不能重复，成员值类型不限。
	 * 调用本方法前需保证 getType() == FmtType.PDT_OBJECT。
	 */
	public native void addField(String name, Fmt field);

	/** 添加byte类型的对象成员，参见 addField() */
	public void addByteField(String name, byte b) {
		Fmt byteFmt = Fmt.newByteFmt(b);
		addField(name, byteFmt);
		byteFmt.decRef();
	}

	/** 添加short类型的对象成员，参见 addField() */
	public void addShortField(String name, short value) {
		Fmt shortFmt = Fmt.newShortFmt(value);
		addField(name, shortFmt);
		shortFmt.decRef();
	}

	/** 添加int类型的对象成员，参见 addField() */
	public void addIntField(String name, int value) {
		Fmt intFmt = Fmt.newIntFmt(value);
		addField(name, intFmt);
		intFmt.decRef();
	}

	/** 添加long类型的对象成员，参见 addField() */
	public void addLongField(String name, long value) {
		Fmt longFmt = Fmt.newLongFmt(value);
		addField(name, longFmt);
		longFmt.decRef();
	}

	/** 添加double类型的对象成员，参见 addField() */
	public void addDoubleField(String name, double value) {
		Fmt doubleFmt = Fmt.newDoubleFmt(value);
		addField(name, doubleFmt);
		doubleFmt.decRef();
	}

	/** 添加日期时间类型的对象成员，参见 addField() */
	public void addDatetimeField(String name, long millis) {
		Fmt timeFmt = Fmt.newDatetimeFmt(millis);
		addField(name, timeFmt);
		timeFmt.decRef();
	}

	/** 添加二进制数据流类型的对象成员，参见 addField() */
	public void addBinaryField(String name, byte[] value) {
		Fmt binaryFmt = Fmt.newBinaryFmt(value);
		addField(name, binaryFmt);
		binaryFmt.decRef();
	}

	/** 添加文本类型的对象成员，参见 addField() */
	public void addStringField(String name, String value) {
		Fmt strFmt = Fmt.newStringFmt(value);
		addField(name, strFmt);
		strFmt.decRef();
	}

	/** 删除“对象类型的Fmt对象”指定名称的成员。调用本方法前需保证 getType()==FmtType.PDT_OBJECT。 */
	public native void delField(String name);

	/** 取“对象类型的Fmt对象”所有成员的个数。调用本方法前需保证 getType()==FmtType.PDT_OBJECT。 */
	public native int getFieldCount();

	/** 
	 * 取“对象类型的Fmt对象”指定名称的成员值。调用本方法前需保证 getType()==FmtType.PDT_OBJECT。
	 * 如果指定名称的成员不存在，返回null。
	 */
	public native Fmt getField(String name);

	/** 取对象类型Fmt内指定名称的byte成员值，不存在则返回默认设定值defaultValue。本方法是getField()方法的包装简化。 */
	public byte getByteField(String name, byte defaultValue) {
		Fmt fmt = getField(name);
		byte value = (fmt != null && fmt.getType() == FmtType.PDT_BYTE) ? fmt.getByte() : defaultValue;
		fmt.decRef();
		return value;
	}

	/** 取对象类型Fmt内指定名称的short成员值，不存在则返回默认设定值defaultValue。本方法是getField()方法的包装简化。 */
	public short getShortField(String name, short defaultValue) {
		Fmt fmt = getField(name);
		short value = (fmt != null && fmt.getType() == FmtType.PDT_SHORT) ? fmt.getShort() : defaultValue;
		fmt.decRef();
		return value;
	}

	/** 取对象类型Fmt内指定名称的int成员值，不存在则返回默认设定值defaultValue。本方法是getField()方法的包装简化。 */
	public int getIntField(String name, int defaultValue) {
		Fmt fmt = getField(name);
		return (fmt != null && fmt.getType() == FmtType.PDT_INTEGER) ? fmt.getInt() : defaultValue; 
	}

	/** 取对象类型Fmt内指定名称的long成员值，不存在则返回默认设定值defaultValue。本方法是getField()方法的包装简化。 */
	public long getLongField(String name, long defaultValue) {
		Fmt fmt = getField(name);
		long value = (fmt != null && fmt.getType() == FmtType.PDT_LONG) ? fmt.getLong() : defaultValue;
		fmt.decRef();
		return value;
	}

	/** 取对象类型Fmt内指定名称的short成员值，不存在则返回默认设定值defaultValue（单位是秒）。本方法是getField()方法的包装简化。 */
	public long getDatetimeField(String name, long defaultValue) {
		Fmt fmt = getField(name);
		long value = (fmt != null && fmt.getType() == FmtType.PDT_DATETIME) ? fmt.getDatetime() : defaultValue;
		fmt.decRef();
		return value;
	}

	/** 取对象类型Fmt内指定名称的String成员值，不存在则返回默认设定值defaultValue。本方法是getField()方法的包装简化。 */
	public String getStringField(String name, String defaultValue) {
		Fmt fmt = getField(name);
		String value = (fmt != null && fmt.getType() == FmtType.PDT_STRING) ? fmt.getString() : defaultValue;
		fmt.decRef();
		return value;
	}
	
	/** 取对象类型Fmt内指定名称的String成员值，不存在则返回默认设定值defaultValue。本方法是getField()方法的包装简化。 */
	public byte[] getBinaryField(String name, byte[] defaultValue) {
		Fmt fmt = getField(name);
		byte[] value = (fmt != null && fmt.getType() == FmtType.PDT_STRING) ? fmt.getBinary() : defaultValue;
		fmt.decRef();
		return value;
	}
	
// Manage Array Fmt elements
	
	/** 取“数组类型的Fmt对象”内所有数组成员的个数。调用本方法前需保证 getType()==FmtType.PDT_ARRAY。 */
	public native int arrayLength();

	/** 向“数组类型的Fmt对象”的末尾添加一个成员。调用本方法前需保证 getType()==FmtType.PDT_ARRAY。 */
	public native void arrayAppend(Fmt element);

	/** 删除“数组类型的Fmt对象”指定索引处的一个成员。索引必须是有效值（0开始）。调用本方法前需保证 getType()==FmtType.PDT_ARRAY。 */
	public native void arrayRemove(int index);

	/** 取“数组类型的Fmt对象”指定索引处的一个成员的值。索引必须是有效值（0开始）。调用本方法前需保证 getType()==FmtType.PDT_ARRAY。 */
	public native Fmt arrayAt(int index);


// Manage Fmt reference-count

	/** 增加此 Fmt 对象的引用计数，并返回该对象自身。更多有关Fmt引用计数的细节，请参考 README.md。 */
	public native Fmt incRef();

	/**
	 * 减小此 Fmt 对象的引用计数。当其引用计数为 0 时，该对象将被自动删除，此后不允许继续使用。
	 * 严格来说，当引用计数为 0 时删除的是Fmt对象背后的C语言概念里的Fmt对象，Java概念里的Fmt交给GC处理。
	 */
	public native void decRef();


// Get type and values of the Fmt object

	/**
	 * 返回一个 int 值用于指定当前 Fmt 对象的类型，返回值是  FmtType.PDT_xxx 其中之一， {@link FmtType}。
	 * 必须依据此类型调用相应的方法操作Fmt对象，例如：只有 getType()==FmtType.PDT_BYTE 的情况下才能调用 getByte()。
	 * @see FmtType
	 */
	public native int getType();

	/** 返回“类型为byte的Fmt对象”内存储的byte值。调用本方法前必须保证 getType()==FmtType.PDT_BYTE。 */
	public native byte getByte();

	/** 返回“类型为short的Fmt对象”内存储的short值。调用本方法前必须保证 getType()==FmtType.PDT_SHORT。 */
	public native short getShort();

	/** 返回“类型为int的Fmt对象”内存储的int值。调用本方法前必须保证 getType()==FmtType.PDT_INT。 */
	public native int getInt();

	/** 返回“类型为long的Fmt对象”内存储的long值。调用本方法前必须保证 getType()==FmtType.PDT_LONG。 */
	public native long getLong();

	/** 返回“类型为double的Fmt对象”内存储的float值。调用本方法前必须保证 getType()==FmtType.PDT_DOUBLE。 */
	public native float getFloat();

	/** 返回“类型为double的Fmt对象”内存储的double值。调用本方法前必须保证 getType()==FmtType.PDT_DOUBLE。 */
	public native double getDouble();

	/** 
	 * 返回“类型为日期时间的Fmt对象”内存储的日期时间值。调用本方法前必须保证 getType()==FmtType.PDT_DATETIME。
	 * 返回值是距 UTC 1970-1-1 00:00:00 的秒数（单位是秒）。 
	 */
	public native long getDatetime();

	/** 返回“类型为二进制数据流的Fmt对象”内存储的byte[]值。调用本方法前必须保证 getType()==FmtType.PDT_STRING。 */
	public native byte[] getBinary();

	/** 返回“类型为二进制数据流的Fmt对象”内存储的数据长度（字节数）。调用本方法前必须保证 getType()==FmtType.PDT_STRING。 */
	public native int getBinaryLength();

	/** 返回“类型为文本的Fmt对象”内存储的String值。调用本方法前必须保证 getType()==FmtType.PDT_STRING。 */
	public String getString() {
		try {
			return new String(getBinary(), "UTF-8");
		} catch (UnsupportedEncodingException e) {
			return new String();
		}
	}


// Packet and serialize

	/**
	 * 把此 Fmt 对象，和协议编号cmd一起，打包为一段二进制数据流，用于网络传输（通过Socket）。
	 * 
	 * @param cmd
	 *            可以是 ProtocolCmd.PT_xxx 系列值之一, @see ProtocolCmd。此参数的取值与协议约定有关，但必须小于65536。
	 * @param userid
	 *            必须是0，此参数目前未被使用。
	 * @return 返回打包后的二进制数据流
	 */
	public native byte[] packet(int cmd, int userid);

	/** 功能类似于 packet()，但此方法是针对服务器的版本，即“调用方充当服务器身份”的情况。 */
	public native byte[] serverPacket(int cmd);

	/** 快速序列化 Fmt 对象 */
	public native byte[] quickSerialize();

	/** 快速解析 Fmt 对象。可能返回null，表示未解析出完整的Fmt对象。 */
	public native Fmt quickParse(byte[] data);
	
	/** 功能类似于packet()，但此方法生成的数据包不包含Fmt对象。 */
	public static native byte[] staticPacket(int cmd, int userid);
	
	/** 功能类似于serverPacket()，但此方法生成的数据包不包含Fmt对象。 */
	public static native byte[] staticServerPacket(int cmd);

}
