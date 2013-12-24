package com.tianxunnet.fmt;

/**
 * 应用层的协议编号。每一个编号都对应一个“通讯双方事先约定好格式的”Fmt对象。
 * 目前因为文档不够详细，没有准确描述每个协议编号对应的Fmt对象内容，这方面需要改进。
 * 
 * @author Liigo
 */
public class ProtocolCmd {
	public static final int PT_LOGINTOUCH = 0x01;
	public static final int PT_TOUCH_SUCCESS = 0x02; //登录被允许 返回一个对象 包含2个成员 分别是 ip(IP地址)和 port(端口)

	// 以下包类型是TCP链接中才有的.
	public static final int PT_LOGIN = 0x03; // 请求登录,提供网吧 或者茶楼信息.
	public static final int PT_LOGINENABLE = 0x04; // 登录被允许。
	public static final int PT_LOGINDISABLE = 0x05;	// 登录被禁止 返回一个对象 包含code(错误代码),message(错误消息)

	// 客户端提交过来的心跳包.服务器返回 PONG
	public static final int PT_PING = 0x06; // PING 客户端发送的心跳包
	public static final int PT_PONG = 0x07; // PING 服务器回应的心跳包

	public static final int PT_SERVER_IS_BUSY = 0x08; // 服务器正忙,无法处理您的提交请求.

	public static final int PT_RECIVER_OK = 0x09; // 服务器已经成功接收到数据.

	// 升级通道,平台向服务器发送升级查询,带上平台已有的包信息.
	// 服务器返回 需要升级的包信息 以及 需要删除的包信息.
	public static final int PT_MODULE_INFO = 0x0A; // 客户端发送模块信息
	public static final int PT_MODULE_UPDATE = 0x0B; // 服务器检查得到需要升级的包，并通过这个指令返回
	public static final int PT_MODULE_REMOVE = 0x0C; // 服务器返回需要删除的软件包

	// 安装向导向服务器查询场所信息
	public static final int PT_QUERY_CORPINFO = 0x0D;
	// 服务器返回安装信息给向导
	public static final int PT_CORP_INFO = 0x0E;
	// 中间服务器向客户端下发网吧唯一码.
	public static final int PT_CORPCODE = 0x0F;
	// 用户必须登录后才能提交数据
	public static final int PT_NEED_LOGIN = 0x10;

	// 设置主机root用户密码
	public static final int PT_SET_ROOT_PASSWORD = 0x11;

	// 服务器构建升级包失败.
	public static final int PT_MODULE_UPDATE_ERR = 0x12;

	// 挂接数据转发. 内部业务数据转发使用.
	public static final int PT_ADD_MONITOR = 0X13;

	// 重启路由器
	public static final int PT_REBOOT_ROUTER = 0X14;

	// 服务器回应给客户端的命令. 数据包有错误的情况下返回

	// 注销. 本命令不建议客户端发出。服务器在检测到客户端断开后自动产生事件.
	public static final int _PT_LOGOUT = 0xEF;

	public static final int PT_ERROR_DATA = 0x01FF;

	// 未知命令
	public static final int PT_UNKNOW_DATA = 0xFF;

	// ==QQ聊天记录包类型.
	// 提交QQ聊天记录，类型是一个“对象”，包含内容自定 至少包含 当前QQ，对方QQ号，消息内容。是否为群消息。
	// 如果是群消息;则还需要包含群号码，对方QQ号为群内消息发送者的QQ号。
	// 一条消息为一个对象，可以是数组，包含多条消息(多个对象)
	public static final int PT_COMMIT_QQMSG = MAKE_DATA_CMDID(0x01);
	// ==提交页面访问记录
	// 包含 URL地址 提交方法,如果是 POST 则需要包含POST信息（POST以1条文本存储）
	public static final int PT_COMMIT_URL = MAKE_DATA_CMDID(0x02);
	// 客户端上机数据.
	public static final int PT_COMMIT_ONLINE = MAKE_DATA_CMDID(0x03);
	// 客户端下机数据.
	public static final int PT_COMMIT_OFFLINE = MAKE_DATA_CMDID(0x04);

	// 顾客 入住/退房 数据,
	public static final int PT_COMMIT_CHECKINOUT = MAKE_DATA_CMDID(0x05);

	// 场所运行状态
	public static final int PT_COMMIT_STATUS = MAKE_DATA_CMDID(0x06);

	// 发帖回帖
	public static final int PT_COMMIT_POST = MAKE_DATA_CMDID(0x07);

	// 虚拟帐号上下线（其中PT_COMMIT_VID_UPDATE仅在客户端内部使用）
	public static final int PT_COMMIT_VID_UPDATE = MAKE_DATA_CMDID(0x0A);
	public static final int PT_COMMIT_VID_ONLINE = MAKE_DATA_CMDID(0x0B);
	public static final int PT_COMMIT_VID_OFFLINE = MAKE_DATA_CMDID(0x0C);

	// 生成纯中转数据包的协议ID，这类数据包经过路由器中转直达中心服务器，中间过程不需要解析数据包。
	private static int MAKE_DATA_CMDID(int n) {
		final int mask = (int) 1 << 15;
		return mask | n;
	}
}
