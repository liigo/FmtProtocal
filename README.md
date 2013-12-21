Fmt Protocol (for Java through JNI)
===================================

# ����
Fmt Protocol ��һ�׼򵥵��Զ������紫��Э�顣���������� Fmt �� cmd �����У�Fmt�Ǵ�������ݶ���cmd��Ӧ�ó����Զ������ֵ��int����
����Ҫ�������д���ʱ���� Fmt �� cmd ���Ϊһ�����ݰ���Packet����������һ�ζ�������������ͨ�� Socket ���͸������ߡ�������ͨ�� Socket ����
��������������֮��ȫ�������ύ�� FmtParser ���������ս�������Եģ�Fmt + cmd��������Ľ��շ��ͷ��ͷ���������Լ����ÿһ���� cmd ��Ӧ
�� Fmt ��������ݸ�ʽ������ʱ�Ϳ������� cmd �ж���������� Fmt �����ݡ�

Fmt Protocol ���Զ���Э�飬���������ʷԭ��Fmt Protocol ����C���Ա�д�������ǹ����ޡ�
���ĵ����� Liigo �����װΪJava�⣬���Ľ��漰�������÷�(for Java)��

# Fmt ����
Fmt ��һ�����ݶ��󣬿����л��ͷ����л������п��Դ洢�������͵�ֵ��byte/short/double/byte[]/String/����/���飩��
Fmt��������ͣ��� int ��ʾ��������Fmt�����ڲ��洢��ֵ�����Fmt�������ǡ����󡱣����п��Դ��������Ա����Ա��ֵ�Ǳ��Fmt�������Fmt�������ǡ����顱�����п������δ�Ŷ�����Fmt��Fmt���������ݱ�����ǰ�ı�����ʽ��Ҳ�����ݱ�������ı�����ʽ�������ݷ���ǰ��Fmt������������ݽ��պ�Fmt�������

## Fmt ���������
Fmt ��������ͣ���һ�� int ��ʾ��������������ֵ֮һ��

	public class FmtType {
		public static final int _PDT_NONE     = 0x00;
		
		public static final int PDT_BYTE     = 0x01; //�ֽ�
		public static final int PDT_SHORT    = 0x02; //������
		public static final int PDT_USHORT   = 0x03;
		public static final int PDT_INTEGER  = 0x04; //����
		public static final int PDT_UINTEGER = 0x05;
		public static final int PDT_LONG     = 0x06; //������
		public static final int PDT_ULONG    = 0x07;
		public static final int PDT_DOUBLE   = 0x08; //˫����С��
		public static final int PDT_STRING   = 0x09; //���������ݣ����ı���
		public static final int PDT_DATETIME = 0x0A; //����ʱ��

		public static final int PDT_ARRAY    = 0x0B; //����
		public static final int PDT_OBJECT   = 0x0C; //���󣨿ɰ�������Fmt��Ա��

		public static final int _PDT_RAW = 0xFE; //ԭʼ��������
		public static final int _PDT_END = 0xFF; //��β��ʶ��
	}

�ɵ��� Fmt.getType() �õ�fmt����ʵ�������ͣ�int t = fmt.getType();

## Fmt ����Ĵ���
�� Java �����ͨ�� Fmt.newXxxFmt() ϵ�з������� Fmt ����ʵ�����磺

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

## ��ȡ Fmt �����ֵ
������Ҫ֪��Fmt��������ͣ�Fmt.getType()����Ȼ��������͵��ò�ͬ�ķ�����Fmt.getXXX()����ȡ���е�ֵ�����磺

	if(fmt.getType() == FmtType.PDT_INTEGER) {
		int value = fmt.getInt();
	}

��������Ͳ���FmtType.PDT_INTEGER���Ͳ��ܵ��� Fmt.getInt() ��
��ʵ��Ӧ���У�ͨ��������ȷ��cmd������Ҳ֪���������Ӧ��Fmt�Ķ������ͺ����ݣ�ͨ������Լ������

## �������͵� Fmt
����һ���������͵�Fmtʵ����

	Fmt fmt = Fmt.newObjectFmt();
	assert(fmt.getType() == FmtType.PDT_OBJECT);

���������Ա��

	fmt.addField("name", otherFmt);

ȡ��Աֵ��

	Fmt field = fmt.getField("name");

ȡ��Ա������

	fmt.getFildCount();
	
## �������͵� Fmt
����һ���������͵�Fmtʵ����

	Fmt fmt = Fmt.newArrayFmt();
	assert(fmt.getType() == FmtType.PDT_ARRAY);

��������Ա��

	fmt.arrayAppend(otherFmt);

ȡ�����Աֵ��

	Fmt element = fmt.arrayAt(index);

ȡ�����Ա������

	fmt.arrayLength();

## Fmt ����Ĵ����Packet��
���� Fmt.packet(cmd) �������� Fmt ����� cmd һ����������һ�ζ�������������byte[]����

	byte[] data = fmt.packet(ProtocolCmd.PT_PING, 0);

�������ɵ�����������ͨ��Socket���ͳ�ȥ��

## Fmt ��������ü���
��Ϊ��Э��ԭ����C���Ա�д�ģ���Ҫ�ֹ��������ü�������װ��Java֮��Ҳ��Ȼ��Ҫ�˹��������ü�����

	fmt.incRef(); // �������ü���
	fmt.decRef(); // �������ü����������ü�����С��0ʱ��fmtʵ�������ͷ�

�����ɵ�Fmt����ʵ���������ü�����1��������ʹ�øö���ʱ��������� fmt.decRef() ��С���ü������Ա�֤�����ͷš�
���ö��󷽷�����Fmt.getField(), Fmt.arrayAt()�����ص�Fmt���󣬷���ǰ�Ѿ����������ü�����ʹ�����Ҳ������� decRef() ȷ�������ͷš�

## Fmt ����Ľ���
�μ� FmtParser��


# FmtParser��FmtЭ�����ݽ�����
FmtParser����Ӷ�������������������Ե� Fmt �� cmd����һ���������Ϊ Fmt.packet(cmd) �ķ�������
���� FmtParser ����������ʵ����

	FmtParser parser = FmtParser.newFmtParser(false, false);

����������ι�����ݣ�

	parser.pushData(data, callback);

ͨ�� Socket ���յ�������������֮��ֱ�Ӱ�˳��ι�����������ɣ������������������
������ÿ������һ����Ե� Fmt �� cmd���͵���һ��callback����callback�������� OnFmtParsed �ӿڣ���Ψһ���� void onFmtParsed(Fmt fmt, int cmd)��
������ʾ�����룺

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
		parser.pushData(data, callback); // �յ�����Socket��������ֱ��ι��FmtParser
	}

������ʹ����ϱ����ֹ��ͷţ�

	parser.close();

# ʵ��ϸ�ڣ�Java JNI + Android NDK��
����
