package com.tianxunnet.fmt;

/**
 * FmtParser解析器对象的解析回调接口。被 FmtParser.pustData() 使用。
 * 使用方法参见 FmtParser.pustData() 的文档，已及 README.md 内的使用示例代码。
 * @see FmtParser#pushData(byte[], OnFmtParsed)
 * @author Liigo, 2013-12.
 */
public interface OnFmtParsed {
	/**
	 * 每当 FmtParser 解析出一个Fmt对象（及其对应的协议编号cmd），此方法就被调用一次。
	 * @param fmt 是当前被解析出的Fmt对象，此对象在 onFmtParsed() 方法返回后被自动销毁。可能为null，与cmd有关。
	 * @param cmd 是对应的协议编号，通常它能够确定参数 fmt 对象的类型（具体取决于协议双方的约定）。 
	 * @see FmtParser#pushData(byte[], OnFmtParsed)
	 */
	public void onFmtParsed(Fmt fmt, int cmd);
}
