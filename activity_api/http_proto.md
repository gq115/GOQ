1. 所有都是 http get 请求
    sign 生成规则:
        KEY是 b83786aaac8d4b80 
        sign = md5(xxx1554888931b83786aaac8d4b80)
        所有参数按字符串连接以后在+key的md5,参数要按照实例的顺序排列


2. 查询账号当日花费
queryTodayCashConsume?acc=xxx&time=1554888931&sign=xasdfasd

{
    'success': 1,//失败就是-1
    'use_cash':9998,//花费数量 一个整数值
}


3. 账号是否今日登陆过
 queryAccTodayLogined?acc=xxx&time=1554888931&sign=xasdfasd 

{
    'success': 1,//失败就是-1
    'is_today_logined':1,//  0 就是没有登陆过
}


4. 账号下面今日上线角色所有在线时长,如果今天没有角色上线,online_info 为空表
 queryTodayRoleOnlineInfo?acc=xxx&time=1554888931&sign=xasdfasd 

 {
   'success': 1,//失败就是-1
    online_info:
    [
        {
            name:'xxx1',
            online_sec:123,//角色名称xxx1 在线 123s
        },
        {
            name:'xxx2',
            online_sec:223,//角色名称xxx1 在线 123s
        }
    ]
 }


 5. 账号下面角色等级
 queryAccRoleLevelInfo?acc=xxx&time=1554888931&sign=xasdfasd 
 {
   'success': 1,//失败就是-1
    level_info:
    [
        {
            name:'xxx1',
            level:10,//角色名称xxx1 在线 123s
        },
        {
            name:'xxx2',
            level:20,//角色名称xxx1 在线 123s
        }
    ]
 }



 6. 账号下面所有角色今日强化次数
 queryAccRoleEnchantInfo?acc=xxx&time=1554888931&sign=xasdfasd 
 {
   'success': 1,//失败就是-1
    enchant_info:
    [
        {
            name:'xxx1',
            times:2,//角色名称xxx1 强化次数
        },
        {
            name:'xxx2',
            times:20,//角色名称xxx1 强化次数
        }
    ]
 }

 
 
 7. 账号下面所有角色列表<封禁的账号不会被查出>
 queryAccRoleListInfo?acc=xxx&time=1554888931&sign=xasdfasd 
 {
   'success': 1,//失败就是-1
    'role_list_info':
    [
        {
            name:'xxx1',
            char_id:'xxxx-xxx-xx',//角色唯一id
			level:10,//角色等级
        },
        {
            name:'xxx1',
            char_id:'xxxx-xxx-xx',//角色唯一id
			level:10,//角色等级
        }
    ]
 }
 
 
  
 
 8. 扣除账号的龙币
 subAccCash?acc=xxx&cash=100&sign=xasdfasd 
 {
   'success': 1,// -1 :龙币不够扣除 
   'remain_cash': 123, //剩余龙币 
 }
 
 
 9. 账号拥有龙币数量
 queryAccCash?acc=xxx&time=1554888931&sign=xasdfasd 
 {
   'success': 1,//失败就是-1
    'cash':999,
 }
 
 
 10，网页发送道具
 addItemV2?charid=xxx&itemid=xxx&itemcount=1&title=xxx&contents=xxx&time=xxx
 {
	'success': 1,//失败就是-1
 }

11，查询时间段内商城消费数
queryAccCashConsume?account=xxx&starttime=xxx&endtime&time=xxx
{
	'success': 1,//成功为返回数量，失败就是-1
}
 