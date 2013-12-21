package com.tianxunnet.fmt;

import java.io.UnsupportedEncodingException;

public class FmtManager {
	static {
		System.loadLibrary("fmt-native");
	}

	// Create new fmt objects. All methods return fmt handle, or 0 if fails.
	public native int newObjectFmt();
	public native int newArrayFmt();
	public native int newByteFmt(byte b);
	public native int newShortFmt(short s);
	public native int newIntFmt(int i);
	public native int newLoogFmt(long l);
	public native int newFloatFmt(float f);
	public native int newDoubleFmt(double d);
	public native int newDatetime(long millis); //System.currentTimeMillis()
	public native int newBinaryFmt(byte[] data);
	public int newStringFmt(String str) {
		try {
			return newBinaryFmt(str.getBytes("UTF-8"));
		} catch(UnsupportedEncodingException e) {
			return 0;
		}
	}

	// Manage fmt objects' fields
	public native void fmtAddField(int fmt, String name, int fieldFmt);
	public native void fmtDelField(int fmt, String name);
	public native int  fmtGetField(int fmt, String name);
	public native int  fmtFieldCount(int fmt);
	
	// Manage fmt array objects' elements. Please ensure the operand fmt is a fmt array object.
	public native int  fmtArrayLength(int fmt);
	public native void fmtArrayAppend(int fmt, int elementFmt);
	public native void fmtArrayRemove(int fmt, int index);
	public native int  fmtArrayAt(int fmt, int index); // returns fmt or 0
	
	// Manage fmt reference-count
	public native int  fmtGet(int fmt); // return the same fmt
	public native void fmtPut(int fmt);
	
	// Get type and values of fmt object
	public native int    fmtType(int fmt); // one of FmtType.PDT_xxx, @see FmtType
	public native byte   fmtGetByte(int fmt); // ensure fmtType(fmt) == FmtType.PDT_BYTE
	public native short  fmtGetShort(int fmt);
	public native int    fmtGetInt(int fmt);
	public native long   fmtGetLong(int fmt);
	public native float  fmtGetFloat(int fmt);
	public native double fmtGetDouble(int fmt);
	public native long   fmtGetDatetime(int fmt); // seconds after midnight, January 1, 1970 UTC.
	public native byte[] fmtGetBinary(int fmt);
	public String fmtGetString(int fmt) {
		try {
			return new String(fmtGetBinary(fmt), "UTF-8");
		} catch (UnsupportedEncodingException e) {
			return new String();
		}
	}
	
	// Serialize, quick-parse FMT object
	// cmd maybe one of ProtocolCmd.PT_xxx, @see ProtocolCmd
	public native byte[] fmtSerialize(int fmt, int cmd, int userid); // always set userid=0
	public native byte[] fmtSerializeServer(int fmt, int cmd);
	public native byte[] fmtQuickSerialize(int fmt);
	public native int    fmtQuickParse(byte[] data); // returns fmt

	// Fmt parser: we push data into parser, and take one fmt out of parser when onParsedFmt() is called.
	public native int newFmtParser(boolean isServer, boolean isFull);
	public native void closeFmtParser(int parser);
	public native void fmtParserPush(int parser, byte[] data, Runnable onParsedFmt);
	public native boolean fmtParserComplete(int parser);
	public native int fmtParserFmt(int parser); // get a fmt from parser. please call it in onParsedFmt() callback, and remember call fmtPut().
	public native int fmtParserCmd(int parser); // get the command of the parsed fmt. please call it in onParsedFmt() callback.
	public native void fmtParserReset(int parser);
}
