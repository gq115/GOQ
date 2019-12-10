工具功能：
	此工具的功能为：
		1，根据 【用户名/用户账号】 查询 【用户ID + 用户账号/用户名】
		2，利用 【用户ID】 给此用户角色发送某种道具
		3，查询游戏在线人数等信息



使用步骤：
	Type：指示操作类型，如果缺失，则命令不再执行。
		1 == 查询用户账户/角色信息命令
		2 == 装备发送命令
		3 == 查询游戏在线人数等数据
		4 == 充值操作
		5 == 服务器指令
	一：查询用户账户/角色信息命令
		1，配置工作目录下的 【userinfo.txt】 文件
		2，将 【操作类型 + 用户账号/用户名 + 当前时间戳 + 配置文件对应的sign】 组成的字符串
		3，将 【步骤2】 组成的字符串进行 MD5加密 生成md5码
		4，利用组装好的 URL 进行查询信息
		5，根据查询的 【用户ID】 加上相应的其它信息给用户 发送道具
		
		查询:
			1：MD5码=用户名 + 时间戳 + userinfo中对应的userconf
			2；user 账号查询
				127.0.0.1:8080?Type=1&AccountName=账户名&_time=时间戳&sign=MD5码
			2：user 角色名查询
				127.0.0.1:8080?Type=1&chName=角色名&_time=时间戳&sign=MD5码
				
	二：装备发送命令
		发送道具：
			假如userinfo里面inspectEquip里面按照编号有:chaid / sender / title / content / itemid / itemname / itemcount
			再加入我需要发送两个道具
			对应指令为
				127.0.0.1:8080?Type=2&chaid=玩家ID&sender=发送者&title=发送标题&content=发送内容&itemid1=发送道具1的ID&itemname1=发送道具1的名字&itemcount1=发送道具1的数量&itemid2=发送道具2的ID&itemname2=发送道具2的名字&itemcount2=发送道具2的数量
				
	三：查询游戏在线人数等数据
		Moment == 0:    查询当前在线人数
		Moment == 1:	根据开始时间和结束时间的时间段查询在线人数
		Moment == 2:	当天时间。以开始时间的24小时时间
		Moment == 3:	当周时间。以开始时间的7天时间
		Moment == 4:	当月时间
		Link:			线路选择，目前只有1线和2线
		查询当前在线人数：
			127.0.0.1:9090?Type=3&Link=1&Moment=0
		查询历史在线人数：
			127.0.0.1:9090?Type=3&Link=1&Moment=1&StartTime=2019-03-21 00:00:00&EndTime=2019-03-22 11:29:00
	
	四：充值操作
		充值操作的数据为特殊一组，需要独立设置
			Sign = userinfo中除Sign参数列表中的所有参数串联+sign 的MD5
			127.0.0.1:9090?Type=4&UserName=账号名&GameMoney=充值点数&OrderNo=订单号&Sign=MD5码
	
	五：服务器指令工作
		给GM服务器发送指令。使其服务器动态改变。主要工作为刷新 dat 资源文件和
			开双倍之类的功能。功能指令在 GMControl.txt 里面
		127.0.0.1:9090?Type=5&Account=administrator&Passwd=123456&Target=27&Arg=1
		
注意事项：
	1，必须要先进过查询后才能进行发送道具工作
	2，查询工作之后 【5分钟】 之内查询工作才有效
	
	
userinfo.txt文件介绍：
	inspectRole:
		记录查询请求中必须要效验的信息，那么就是必须存在的信息。可根据条件修改
	inspectEquip:
		记录装备查询工作中必须要效验的信息，那么就是必须存在的信息。可根据条件修改
	userIP:
		记录准许的IP，只有在此条款中的用户才能够发送请求
	userConf:
		记录准许的校验码，以便生成有效的MD5码，其中的每条信息与userIP信息一一对应
	GMInfo:
		记录准许的服务器管理员信息