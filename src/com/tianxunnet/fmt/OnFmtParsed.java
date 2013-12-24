package com.tianxunnet.fmt;

/**
 * Will be called by FmtParser.pustData().
 * 
 * @author Liigo, 2013-12.
 */
public interface OnFmtParsed {
	/**
	 * This method will be called by pushData() on every Fmt is parsed out.
	 * The fmt is the parsed Fmt, the cmd is the parsed protocol cmd, @see ProtocolCmd.
	 * The fmt will be automatically freed after the method call, if no fmt.incRef() has been called.
	 */
	public void onFmtParsed(Fmt fmt, int cmd);
}
