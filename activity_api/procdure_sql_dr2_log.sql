
-- 角色上线时间
drop PROCEDURE if exists  [dbo].[up_web_Admin_SumAccTodayPlayTime]
CREATE PROCEDURE [dbo].[up_web_Admin_SumAccTodayPlayTime]
	@ID	as nvarchar(20)
AS
BEGIN
    declare @queryBody nvarchar(512)    
    declare @tbName  nvarchar(30)    
    declare @subTbName  nvarchar(20)    

    set @queryBody = N''  
    set @tbName = 'TB_'+convert(nvarchar(8),GetDate() , 112)     + ' as A '
    set @subTbName = 'TB_'+convert(nvarchar(8),GetDate() , 112)+'_Sub'  + ' as B ' 

     set @queryBody =N'select sum(i64Value1) from '
     +  @tbName
     + ','
     +  @subTbName
    + ' where A.logguid = B.logguid 
        and logtype = 1103 
        and ID = '''
    + @ID + ''''
     
     exec sp_executesql @queryBody
 END


-- 按照角色统计 每个角色的在线时间
CREATE PROCEDURE [dbo].[up_web_Admin_SummaryRoleTodayPlayTime]
    @ID as nvarchar(20)
AS
BEGIN
    declare @queryBody nvarchar(512)    
    declare @tbName  nvarchar(30)    
    declare @subTbName  nvarchar(20)    

    set @queryBody = N''  
    set @tbName = 'TB_'+convert(nvarchar(8),GetDate() , 112)     + ' as A '
    set @subTbName = 'TB_'+convert(nvarchar(8),GetDate() , 112)+'_Sub'  + ' as B ' 

     set @queryBody =N'select name,sum(i64Value1) from '
     +  @tbName
     + ','
     +  @subTbName
    + ' where A.logguid = B.logguid 
        and logtype = 1103         
        and ID = '''
    + @ID + '''
    
      group by name 
    '
     
     exec sp_executesql @queryBody
 END


-- 按照角色统计 每个角色的强化次数
CREATE PROCEDURE [dbo].[up_web_Admin_CountPlayerTodayEnchentTime]
    @ID as nvarchar(20)
AS
BEGIN
    declare @queryBody nvarchar(512)    
    declare @tbName  nvarchar(30)    
    declare @subTbName  nvarchar(20)    

    set @queryBody = N''  
    set @tbName = 'TB_'+convert(nvarchar(8),GetDate() , 112)     + ' as A '
    set @subTbName = 'TB_'+convert(nvarchar(8),GetDate() , 112)+'_Sub'  + ' as B ' 

     set @queryBody =N'select name,count(*) from '
     +  @tbName
     + ','
     +  @subTbName
    + ' where A.logguid = B.logguid 
        and logtype = 3002 and sublogtype = 302 and ordertype = 1301        
        and ID = '''
    + @ID + '''
    
      group by name 
    '
     
     exec sp_executesql @queryBody
 END

