package com.tianxunnet.fmt;

import java.io.UnsupportedEncodingException;

/**
 * An Fmt object.
 * 
 * @author Liigo, 2013-12.
 * 
 */
public class Fmt {
	static {
		System.loadLibrary("fmt-native");
	}

	private long fmtPtr; // stores Fmt* managed by C, see native source code.

	private Fmt(long fmtPtr) {
		this.fmtPtr = fmtPtr;
	}

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
	public static native Fmt newDatetimeFmt(long millis);

	public static native Fmt newBinaryFmt(byte[] data);

	public static Fmt newStringFmt(String str) {
		try {
			return newBinaryFmt(str.getBytes("UTF-8"));
		} catch (UnsupportedEncodingException e) {
			byte[] empty = {};
			return newBinaryFmt(empty);
		}
	}

	// Manage object-Fmt objects' fields. Please ensure
	// getType()==ProtocolCmd.PDT_OBJECT.
	public native void addField(String name, Fmt field);

	public void addByteField(String name, byte b) {
		Fmt byteFmt = Fmt.newByteFmt(b);
		addField(name, byteFmt);
		byteFmt.decRef();
	}

	public void addShortField(String name, short value) {
		Fmt shortFmt = Fmt.newShortFmt(value);
		addField(name, shortFmt);
		shortFmt.decRef();
	}

	public void addIntField(String name, int value) {
		Fmt intFmt = Fmt.newIntFmt(value);
		addField(name, intFmt);
		intFmt.decRef();
	}

	public void addLongField(String name, long value) {
		Fmt longFmt = Fmt.newLongFmt(value);
		addField(name, longFmt);
		longFmt.decRef();
	}

	public void addDoubleField(String name, double value) {
		Fmt doubleFmt = Fmt.newDoubleFmt(value);
		addField(name, doubleFmt);
		doubleFmt.decRef();
	}

	public void addDatetimeField(String name, long millis) {
		Fmt timeFmt = Fmt.newDatetimeFmt(millis);
		addField(name, timeFmt);
		timeFmt.decRef();
	}

	public void addBinaryField(String name, byte[] value) {
		Fmt binaryFmt = Fmt.newBinaryFmt(value);
		addField(name, binaryFmt);
		binaryFmt.decRef();
	}

	public void addStringField(String name, String value) {
		Fmt strFmt = Fmt.newStringFmt(value);
		addField(name, strFmt);
		strFmt.decRef();
	}

	public native void delField(String name);

	public native Fmt getField(String name);

	public native int getFieldCount();

	// Manage array-Fmt objects' elements. Please
	// getType()==ProtocolCmd.PDT_ARRAY.
	public native int arrayLength();

	public native void arrayAppend(Fmt element);

	public native void arrayRemove(int index);

	public native Fmt arrayAt(int index);

	// Manage Fmt reference-count
	/** Increase reference count, and returns itself. */
	public native Fmt incRef();

	/**
	 * Decrease reference count, delete the Fmt object when ref==0. Must be
	 * called if no longer use this object, to make sure it be freed properly.
	 */
	public native void decRef();

	// Get type and values of the Fmt object
	/**
	 * Returns one of FmtType.PDT_xxx, @see FmtType. Please ensure
	 * getType()==FmtType.PDT_BYTE before calling getByte(), the same to others.
	 */
	public native int getType();

	public native byte getByte();

	public native short getShort();

	public native int getInt();

	public native long getLong();

	public native float getFloat();

	public native double getDouble();

	/** Returns seconds (not milliseconds) after midnight, January 1, 1970 UTC. */
	public native long getDatetime();

	public native byte[] getBinary();

	public native int getBinaryLength();

	public String getString() {
		try {
			return new String(getBinary(), "UTF-8");
		} catch (UnsupportedEncodingException e) {
			return new String();
		}
	}

	/**
	 * packet the Fmt and the protocol cmd to a packet that ready to be sent
	 * between sockets.
	 * 
	 * @param cmd
	 *            should be one of ProtocolCmd.PT_xxx, @see ProtocolCmd.
	 * @param userid
	 *            always set to 0 currently.
	 * @return returns packeted data
	 */
	public native byte[] packet(int cmd, int userid);

	public native byte[] serverPacket(int cmd);

	public native byte[] quickSerialize();

	public native Fmt quickParse(byte[] data);

}
