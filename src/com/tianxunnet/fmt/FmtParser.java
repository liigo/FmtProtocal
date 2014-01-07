package com.tianxunnet.fmt;

/**
 * Fmt对象协议数据解析器。使用方法参见 FmtParser.pushData() 和 OnFmtParsed 的文档，以及 README.md 中的示例代码。
 * @see FmtParser#pushData(byte[], OnFmtParsed)
 * 
 * @author Liigo, 2013-12.
 */
public class FmtParser {
	static {
		System.loadLibrary("fmt-native");
	}

	private long parserPtr; // stores fmt-parser pointer, managed by C, see native source code.

	private FmtParser(long parserPtr) {
		this.parserPtr = parserPtr;
	}

	/**
	 * 创建  FmtParser 对象实例。
	 * 如果调用者是服务器，设置参数 isServer 为 true。参数 isFull 目前未被使用。
	 */
	public static native FmtParser newFmtParser(boolean isServer, boolean isFull);

	/** 销毁 FmtParser 对象实例。 */
	public native void close();

	/** 重置内部状态 */
	public native void reset();

	/**
	 * 把数据 data 推入解析器，解析器每解析出一个Fmt对象（以及对应的协议编号cmd），就会调用一次callback。
	 * @see OnFmtParsed
	 */
	public native void pushData(byte[] data, OnFmtParsed callback);
}
