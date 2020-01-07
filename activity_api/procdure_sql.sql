
    CREATE PROCEDURE [dbo].[up_web_Admin_QueryUserLastLoginTime]
            @ID	as nvarchar(20)
        AS
        BEGIN
            SELECT  LastLoginDate
            FROM  [dbo].[member] 
            WHERE  ID = @ID 
        END




CREATE PROCEDURE [dbo].[up_web_Admin_QueryUserTodayUseCash]
	@ID	as nvarchar(20)
AS
BEGIN
	SELECT sum(f_amount) 
	FROM [dbo].[tbl_CashLog] as C , [dbo].[member] as M
	WHERE f_date BETWEEN CONVERT(varchar(100), GETDATE(), 23) +' 00:00:00' AND CONVERT(varchar(100), GETDATE(), 23) +' 23:59:59.999' 
				and f_amount < 0			
				and M.ID = @ID and M.MemberKey = C.MemberKey
END



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

    exec [dbo].[up_web_Admin_SumAccTodayPlayTime] 'gsx'




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


-- 按照角色统计 每个角色的在线时间
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


CREATE PROCEDURE [dbo].[up_web_Admin_QueryAccRoleInfoList]
	@ID	as nvarchar(20)
AS
BEGIN
	  SELECT B.Name,C.Lv
	  FROM [DR2_Member].[dbo].[Member] as A ,
	  DR2_User.dbo.TB_CharacterBasic as B,
	  DR2_User.dbo.TB_CharacterSub as C	  
	  where A.MemberKey = B.MemberID and B.CharacterID = C.CharacterID	  
	  and A.ID = @ID
END
