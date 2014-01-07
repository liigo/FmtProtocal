Fmt Protocol (for Android/Java)
===========================================

# 概述
Fmt Protocol 是一套简单的自定义网络传输协议。其核心组件是 Fmt 和 cmd 。其中，Fmt是纯粹的数据对象，cmd是应用程序自定义的数值（int）。
当需要在网络中传输时，把 Fmt 和 cmd 打包为一个数据包（Packet），即生成一段二进制数据流，通过 Socket 发送给接收者。接收者通过 Socket 接收
到二进制数据流之后，全部依次提交给 FmtParser 解析，最终解析出配对的（Fmt + cmd）。程序的接收方和发送方，可事先约定好每一个与 cmd 对应
的 Fmt 对象的数据格式，处理时就可以依据 cmd 判断与其关联的 Fmt 的内容。

Fmt Protocol 是自定义协议，其存在有历史原因。Fmt Protocol 采用C语言编写，作者是龚辟愚。
本文作者 Liigo 负责封装为Java库，本文仅涉及其概念和用法(for Java)。[Java APIs](http://htmlpreview.github.com/?https://github.com/liigo/FmtProtocol/blob/master/api/index.html)，[实现细节](#internal-impl)（Android NDK + Java JNI）。

# Fmt 对象
Fmt 是一个数据对象，可序列化和反序列化。其中可以存储各种类型的值（byte/short/double/byte[]/String/对象/数组）。
Fmt对象的类型（以 int 表示）决定了Fmt对象内部存储的值。如果Fmt的类型是“对象”，其中可以存放命名成员（成员的值是别的Fmt）；如果Fmt的类型是“数组”，其中可以依次存放多个别的Fmt。Fmt对象是数据被发送前的表现形式，也是数据被解析后的表现形式：在数据发送前，Fmt被打包，在数据接收后，Fmt被解包。

## Fmt 对象的类型
Fmt 对象的类型，用一个 int 表示，可以是以下数值之一：

	public class FmtType {
		public static final int _PDT_NONE     = 0x00;
		
		public static final int PDT_BYTE     = 0x01; //字节
		public static final int PDT_SHORT    = 0x02; //短整数
		public static final int PDT_USHORT   = 0x03;
		public static final int PDT_INTEGER  = 0x04; //整数
		public static final int PDT_UINTEGER = 0x05;
		public static final int PDT_LONG     = 0x06; //长整数
		public static final int PDT_ULONG    = 0x07;
		public static final int PDT_DOUBLE   = 0x08; //双精度小数
		public static final int PDT_STRING   = 0x09; //二进制数据（或文本）
		public static final int PDT_DATETIME = 0x0A; //日期时间

		public static final int PDT_ARRAY    = 0x0B; //数组
		public static final int PDT_OBJECT   = 0x0C; //对象（可包含其他Fmt成员）

		public static final int _PDT_RAW = 0xFE; //原始数据类型
		public static final int _PDT_END = 0xFF; //结尾标识符
	}

可调用 Fmt.getType() 得到fmt对象实例的类型：int t = fmt.getType();

## Fmt 对象的创建
在 Java 里可以通过 Fmt.newXxxFmt() 系列方法生成 Fmt 对象实例。如：

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
	public static Fmt newStringFmt(String str);

## 读取 Fmt 对象的值
首先需要知道Fmt对象的类型（Fmt.getType()），然后根据类型调用不同的方法（Fmt.getXXX()）读取其中的值。例如：

	if(fmt.getType() == FmtType.PDT_INTEGER) {
		int value = fmt.getInt();
	}

但如果类型不是FmtType.PDT_INTEGER，就不能调用 Fmt.getInt() 。
在实际应用中，通常是首先确定cmd，进而也知道了与其对应的Fmt的对象类型和内容（通过事先约定）。

## 对象类型的 Fmt
生成一个对象类型的Fmt实例：

	Fmt fmt = Fmt.newObjectFmt();
	assert(fmt.getType() == FmtType.PDT_OBJECT);

添加命名成员：

	fmt.addField("name", otherFmt);

取成员值：

	Fmt field = fmt.getField("name");

取成员个数：

	fmt.getFildCount();
	
## 数组类型的 Fmt
生成一个数组类型的Fmt实例：

	Fmt fmt = Fmt.newArrayFmt();
	assert(fmt.getType() == FmtType.PDT_ARRAY);

添加数组成员：

	fmt.arrayAppend(otherFmt);

取数组成员值：

	Fmt element = fmt.arrayAt(index);

取数组成员个数：

	fmt.arrayLength();

## Fmt 对象的打包（Packet）
调用 Fmt.packet(cmd) 方法，将 Fmt 对象跟 cmd 一起打包，生成一段二进制数据流（byte[]）：

	byte[] data = fmt.packet(ProtocolCmd.PT_PING, 0);

这样生成的数据流可以通过Socket发送出去。

## Fmt 对象的引用计数
因为此协议原本身C语言编写的，需要手工处理引用计数，封装到Java之后，也仍然需要人工处理引用计数。

	fmt.incRef(); // 增加引用计数
	fmt.decRef(); // 减少引用计数，当引用计数减小到0时，fmt实例将被释放

新生成的Fmt对象实例，其引用计数是1，当不再使用该对象时，必须调用 fmt.decRef() 减小引用计数，以保证正常释放。
调用对象方法（如Fmt.getField(), Fmt.arrayAt()）返回的Fmt对象，返回前已经增加了引用计数，使用完毕也必须调用 decRef() 确保正常释放。

## Fmt 对象的解析
参见 FmtParser。


# FmtParser：Fmt协议数据解析器
FmtParser负责从二进制数据流解析出配对的 Fmt 和 cmd，这一步可以理解为 Fmt.packet(cmd) 的反操作。
创建 FmtParser 解析器对象实例：

	FmtParser parser = FmtParser.newFmtParser(false, false);

给解析器“喂”数据：

	parser.pushData(data, callback);

通过 Socket 接收到二进制数据流之后，直接按顺序喂给解析器即可，解析器负责拆包解包。
解析器每解析出一个配对的 Fmt 和 cmd，就调用一次callback对象。callback的类型是 OnFmtParsed 接口，有唯一方法 void onFmtParsed(Fmt fmt, int cmd)。
以下是示例代码：

	OnFmtParsed callback = new OnFmtParsed() {
		@Override
		public void onFmtParsed(Fmt fmt, int cmd) {
			if(cmd == 0) {
				// read fmt value
			} else if(cmd == 1) {

			}
		}
	};

	onReceivedDataFromSocket(byte[] data) {
		parser.pushData(data, callback); // 收到来自Socket的数据流直接喂给FmtParser
	}

解析器使用完毕必须手工释放：

	parser.close();

<a name="internal-impl"></a>
# 实现细节（Java JNI + Android NDK）

本项目核心代码在 src 和 jni 两目录内。src目录内主要是 Java 代码，主要由 Fmt.java 和 FmtParser.java 这两个 Java Class 组成（许多方法通过JNI本地库实现），另有几个辅助类和测试代码（测试代码目前写在 MainActivity.java 内）。jni目录内主要是 C 代码，com_tianxunnet_fmt_Fmt.c 和 com_tianxunnet_fmt_FmtParser.c 这两个文件是本项目内最核心的代码，主要用于实现 Fmt 和 FmtParser 这两个Java本地类；com_tianxunnet_fmt_Fmt.h 和 com_tianxunnet_fmt_FmtParser.h 是使用 javah 依据 Fmt.java / FmtParser.java 自动生成的；jin/protocol目录内是Fmt协议本身的源代码。本项目中的JNI本地库需使用 Android NDK 编译。

使用 javah 生成 .h 文件的操作（必要时指定 javah 全路径）：

	cd <project-home>/bin/classes
	javah com.tianxunnet.fmt.Fmt
	javah com.tianxunnet.fmt.FmtParser

以上操作将生成 com_tianxunnet_fmt_Fmt.h 和 com_tianxunnet_fmt_FmtParser.h 两个头文件，然后需要将其手工复制到 jni 目录。

使用 Android NDK 编译本地库的操作：

	cd <project-home>/jni
	/path/to/Android/ndk/ndk-build

如果当前电脑操作系统是 Windows 环境，以上操作需在 Cygwin 终端环境中执行，如：

	cd /cygdrive/c/Users/LIIGO/workspace/FmtProtocal
	/cygdrive/c/Android/ndk/ndk-build

执行成功将编译生成 libs/armeabi/libfmt-native.so，也就是前面提到的本地库。

文件 jni/Android.mk 也是本项目的重要组成部分，它用于配置本地库的编译细节，内容如下：

	LOCAL_PATH := $(call my-dir)
	include $(CLEAR_VARS)
	LOCAL_MODULE    := fmt-native
	LOCAL_SRC_FILES := com_tianxunnet_fmt_Fmt.c com_tianxunnet_fmt_FmtParser.c protocol/protocol.c protocol/buffmtparser.c protocol/lib/automem.c protocol/lib/linkhash.c protocol/lib/arraylist.c
	include $(BUILD_SHARED_LIBRARY)

由于此源码仓库中已经包含了自动生成的 .h 文件，和编译生成的 .so，编译本项目不必依赖 Java SDK, Anroid NDK 以及 Cygwin，只需 ADT (Android Developer Tools) 通过 Eclipse 直接 build 即可，也可在 Eclipse 内调试运行。

生成API文档（必要时指定javadoc完整目录）：

	cd <project-home>
	javadoc -sourcepath src -encoding utf-8 -classpath bin/classes com.tianxunnet.fmt -d api -windowtitle "FmtProtocol APIs"

更多细节待续。
