package com.tianxunnet.fmt;

import java.io.UnsupportedEncodingException;

/**
 * An Fmt object.
 * @author Liigo, 2013-12.
 *
 */
public class Fmt {
	static {
		System.loadLibrary("fmt-native");
	}
	
	private long fmtPtr; // stores Fmt* managed by C, see native source code.
	private Fmt(long fmtPtr) { this.fmtPtr = fmtPtr; }

	/** Create new Fmt objects. */
	public static native Fmt newObjectFmt();
	public static native Fmt newArrayFmt();
	public static native Fmt newByteFmt(byte b);
	public static native Fmt newShortFmt(short s);
	public static native Fmt newIntFmt(int i);
	public static native Fmt newLongFmt(long l);
	public static native Fmt newFloatFmt(float f);
	public static native Fmt newDoubleFmt(double d);
	/** the arg may be System.currentTimeMillis() or something other */
	public static native Fmt newDatetime(long millis);
	public static native Fmt newBinaryFmt(byte[] data);
	public static Fmt newStringFmt(String str) {
		try {
			return newBinaryFmt(str.getBytes("UTF-8"));
		} catch(UnsupportedEncodingException e) {
			byte[] empty = {};
			return newBinaryFmt(empty);
		}
	}

	// Manage Fmt objects' fields. Please ensure getType()==ProtocolCmd.PDT_OBJECT.
	public native void addField(String name, Fmt field);
	public void addStrField(String name, String value) {
    	Fmt strFmt = Fmt.newStringFmt(value);
    	addField(name, strFmt);
    	strFmt.decRef();
    }
	public void addIntField(String name, int value) {
    	Fmt intFmt = Fmt.newIntFmt(value);
    	addField(name, intFmt);
    	intFmt.decRef();
    }
	public native void delField(String name);
	public native Fmt  getField(String name);
	public native int  getFieldCount();

	// Manage Fmt array objects' elements. Please getType()==ProtocolCmd.PDT_ARRAY.
	public native int  arrayLength();
	public native void arrayAppend(Fmt element);
	public native void arrayRemove(int index);
	public native Fmt  arrayAt(int index);

	// Manage Fmt reference-count
	/** Increase reference count, and returns itself. */
	public native Fmt  incRef();
	/** Decrease reference count, delete the Fmt object when ref==0.
	 *  Must be called if no longer use this object, to make sure it be freed properly. 
	 */
	public native void decRef();

	// Get type and values of the Fmt object
	/**
	 * Returns one of FmtType.PDT_xxx, @see FmtType.
	 * Please ensure getType()==FmtType.PDT_BYTE before calling getByte(), the same to others.
	 */
	public native int    getType();
	public native byte   getByte();
	public native short  getShort();
	public native int    getInt();
	public native long   getLong();
	public native float  getFloat();
	public native double getDouble();
	/** Returns seconds (not milliseconds) after midnight, January 1, 1970 UTC. */
	public native long   getDatetime();
	public native byte[] getBinary();
	public native int    getBinaryLength();
	public String getString() {
		try {
			return new String(getBinary(), "UTF-8");
		} catch (UnsupportedEncodingException e) {
			return new String();
		}
	}

	/**
	 * packet the Fmt and the protocol cmd to a packet that ready to be sent between sockets.
	 * @param cmd should be one of ProtocolCmd.PT_xxx, @see ProtocolCmd.
	 * @param userid always set to 0 currently. 
	 * @return returns packeted data
	 */
	public native byte[] packet(int cmd, int userid);
	public native byte[] serverPacket(int cmd);
	public native byte[] quickSerialize();
	public native Fmt    quickParse(byte[] data);

}
