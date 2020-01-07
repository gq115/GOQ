
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



CREATE PROCEDURE [dbo].[up_web_Admin_QueryAccRoleInfoList]
    @ID as nvarchar(20)
AS
BEGIN
      SELECT B.Name,C.Lv
      FROM [DR2_Member].[dbo].[Member] as A ,
      DR2_User.dbo.TB_CharacterBasic as B,
      DR2_User.dbo.TB_CharacterSub as C   
      where A.MemberKey = B.MemberID and B.CharacterID = C.CharacterID    
      and A.ID = @ID
END