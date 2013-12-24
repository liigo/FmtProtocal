package com.tianxunnet.fmt;

/**
 * The Fmt parser. See doc of method pushData().
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
	 * Create a new FmtParser. Set isServer to true is you are run at server side.
	 * The arg isFull is not used currently.
	 */
	public static native FmtParser newFmtParser(boolean isServer, boolean isFull);

	/** Close the parser. Must call it when no longer uses the parser. */
	public native void close();

	/** Reset the parser status. */
	public native void reset();

	/**
	 * Push data to parser. The OnFmtParsed callback will be called on every Fmt is parsed out.
	 */
	public native void pushData(byte[] data, OnFmtParsed callback);
}
