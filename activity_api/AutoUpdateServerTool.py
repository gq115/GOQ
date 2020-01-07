import configparser
import pyautogui
import win32gui
import win32api
import win32con
import pymssql
import decimal
import shutil
import ctypes
import time
import sys
import os

dbinfo = {
    'host':'127.0.0.1', 
    'user':'sa', 
    'passwd':'123', 
    'datebase':'dr2_def'
}
filepath = {
    'ftpCheckPath' : 'D:\\FTP\\FTPBack\\',
    'ftpEndPath' : 'D:\\FTP\\FTPBack\\BuildFile\\',
    'importCheckPath' : 'D:\\FTP\\import_tool\\',
    'importSavePath' : 'D:\\FTP\\import_tool\\excel\\',
    'importExcPath' : 'D:\\FTP\\import_tool\\start.bat',
    'ContentsXmlPath' : 'D:\\梦幻龙族Server\\Archangel_run\\SMC\\patch\\contents\\xml',
    'ContentsTabPath' : 'D:\\梦幻龙族Server\\Archangel_run\\SMC\\patch\\contents\\table'
}
localRun = '1'

MessageSort = {
    'Expand':-1,
    'Fold':-1,
    'Site_1':-1,
    'AllSel':-1,
    'RunOn':-1,
    'KillSel':-1,
    'killBut':-1,
    'warOk':-1,
    'All':-1,
    'Listview':-1,
    'Alert':-1
}

xmlFileTimeList = []            #记录xml文件名+文件修改时间
dbFileTimeList = []             #记录数据库文件名+文件修改时间
dbList = []



def readIniConfig():
    global filepath
    global dbinfo
    conf=configparser.ConfigParser()
    conf.read("GUIConfig.ini", encoding="utf-8")
    
    if conf.has_option("FTP", "ftpCheckPath") == True:
        filepath['ftpCheckPath'] = conf.get('FTP','ftpCheckPath')
    if conf.has_option("FTP", "ftpEndPath") == True:
        filepath['ftpEndPath'] = conf.get('FTP','ftpEndPath')
    if conf.has_option("Import", "importCheckPath") == True:
        filepath['importCheckPath'] = conf.get('Import','importCheckPath')
    if conf.has_option("Import", "importSavePath") == True:
        filepath['importSavePath'] = conf.get('Import','importSavePath')
    if conf.has_option("Import", "importExcPath") == True:
        filepath['importExcPath'] = conf.get('Import','importExcPath')
    if conf.has_option("Contents", "ContentsXmlPath") == True:
        filepath['ContentsXmlPath'] = conf.get('Contents','ContentsXmlPath')
    if conf.has_option("Contents", "ContentsTabPath") == True:
        filepath['ContentsTabPath'] = conf.get('Contents','ContentsTabPath')

    if conf.has_option("SQL", "host") == True:
        dbinfo['host'] = conf.get('SQL', 'host')
    if conf.has_option("SQL", "user") == True:
        dbinfo['user'] = conf.get('SQL', 'user')
    if conf.has_option("SQL", "pass") == True:
        dbinfo['passwd'] = conf.get('SQL', 'pass')
    if conf.has_option("SQL", "db") == True:
        dbinfo['datebase'] = conf.get('SQL', 'db')
    
    if conf.has_option("otherControl", "localRun") == True:
        localRun = conf.get('otherControl', 'localRun')

    print( '所有文件信息', filepath )
    print( '所有数据库信息', dbinfo )


#在系统中直接找到 ServerControl 这个程序，然后给指定句柄发送消息
def AutoMessageServerControl():
    handler = win32gui.FindWindow(None, '[Tool Version] : 0, 6, 1, 0 / [Product Version] : 8, 10, 21, 1 / [Packet Version S] : 09,10,21,1 / [Packet Version C] : 09,10,21,1 / [Patch Version] : 0, 0, 6')
    if handler <= 0:
        return -1            
    parentHand = handler
    hwndChildList = []     
    win32gui.EnumChildWindows(handler, lambda hwnd, param: param.append(hwnd),  hwndChildList)
    win32gui.ShowWindow(parentHand, win32con.SW_SHOWNORMAL)
    
    #搜寻所有按钮的句柄
    for hd in hwndChildList:
        title = win32gui.GetWindowText(hd)     
        clsname = win32gui.GetClassName(hd)
        if clsname == 'SysTreeView32':
            MessageSort['Site_1']=hd  
        if title == 'Fold':
            MessageSort['Fold']=hd
        if title == 'Expand':
            MessageSort['Expand']=hd
        if clsname == 'SysListView32':
            MessageSort['Listview']=hd
            hwndChildList1 = []
            win32gui.EnumChildWindows(hd, lambda hwnd, param: param.append(hwnd),  hwndChildList1)
            for childchild in hwndChildList1:
                if win32gui.GetClassName(childchild)=='SysHeader32':
                    MessageSort['AllSel']=childchild
        if title == 'On':
            MessageSort['RunOn'] = hd
        if title == 'Kill':
            MessageSort['killBut'] = hd
        if title == 'Terminate':
            MessageSort['KillSel'] = hwndChildList[ hwndChildList.index(hd)+1 ]
        if title == 'All':
            MessageSort['All'] = hd
        if title == 'Alert':
            MessageSort['Alert'] = hd
    #BEGIN RUN
    #结束点击顺序  Expand Fold Site_1 AllSel KillSel killBut warOk
    win32gui.PostMessage(MessageSort['Fold'], win32con.WM_LBUTTONDOWN, 1, 0)
    win32gui.PostMessage(MessageSort['Fold'], win32con.WM_LBUTTONUP, 1, 0)
    time.sleep(0.5)
    win32gui.PostMessage(MessageSort['Site_1'], win32con.WM_LBUTTONDOWN, 1, 0)
    win32gui.PostMessage(MessageSort['Site_1'], win32con.WM_LBUTTONUP, 1, 0)
    time.sleep(0.5)
    win32gui.PostMessage(MessageSort['AllSel'], win32con.WM_LBUTTONDOWN, 1, 0)
    win32gui.PostMessage(MessageSort['AllSel'], win32con.WM_LBUTTONUP, 1, 0)
    time.sleep(0.5)
    win32gui.PostMessage(MessageSort['KillSel'], win32con.WM_LBUTTONDOWN, 1, 0)
    win32gui.PostMessage(MessageSort['KillSel'], win32con.WM_LBUTTONUP, 1, 0)
    time.sleep(0.5)
    win32gui.PostMessage(MessageSort['killBut'], win32con.WM_LBUTTONDOWN, 1, 0)
    win32gui.PostMessage(MessageSort['killBut'], win32con.WM_LBUTTONUP, 1, 0)
    time.sleep(0.5)
    handler = win32gui.FindWindow(None, 'Process Kill warning!!')
    if handler > 0:    
        hwndChildList = []     
        win32gui.EnumChildWindows(handler, lambda hwnd, param: param.append(hwnd),  hwndChildList)
        for hd in hwndChildList:
            title = win32gui.GetWindowText(hd)     
            clsname = win32gui.GetClassName(hd)
            if title == '확인':
                MessageSort['warOk']=hd
                break
        win32gui.PostMessage(MessageSort['warOk'], win32con.WM_LBUTTONDOWN, 1, 0)
        win32gui.PostMessage(MessageSort['warOk'], win32con.WM_LBUTTONUP, 1, 0)
    time.sleep(10)
    print('关闭所有服务成功')

    #开启点击顺序  Expand Fold Site_1 AllSel RunOn warOk
    win32gui.PostMessage(MessageSort['Fold'], win32con.WM_LBUTTONDOWN, 1, 0)
    win32gui.PostMessage(MessageSort['Fold'], win32con.WM_LBUTTONUP, 1, 0)
    time.sleep(0.5)
    win32gui.PostMessage(MessageSort['Site_1'], win32con.WM_LBUTTONDOWN, 1, 0)
    win32gui.PostMessage(MessageSort['Site_1'], win32con.WM_LBUTTONUP, 1, 0)
    time.sleep(0.5)
    win32gui.PostMessage(MessageSort['AllSel'], win32con.WM_LBUTTONDOWN, 1, 0)
    win32gui.PostMessage(MessageSort['AllSel'], win32con.WM_LBUTTONUP, 1, 0)
    time.sleep(0.5)
    win32gui.PostMessage(MessageSort['RunOn'], win32con.WM_LBUTTONDOWN, 1, 0)
    win32gui.PostMessage(MessageSort['RunOn'], win32con.WM_LBUTTONUP, 1, 0)
    time.sleep(0.5)
    handler = win32gui.FindWindow(None, 'Server On warning!!')
    if handler > 0:
        hwndChildList = []     
        win32gui.EnumChildWindows(handler, lambda hwnd, param: param.append(hwnd),  hwndChildList)
        for hd in hwndChildList:
            title = win32gui.GetWindowText(hd)     
            clsname = win32gui.GetClassName(hd)
            if title == '확인':
                MessageSort['warOk']=hd
                break
        win32gui.PostMessage(MessageSort['warOk'], win32con.WM_LBUTTONDOWN, 1, 0)
        win32gui.PostMessage(MessageSort['warOk'], win32con.WM_LBUTTONUP, 1, 0)
    print('启动所有服务成功')
    win32gui.ShowWindow(parentHand, win32con.SW_MINIMIZE)
    return 1

#检测文件的更新程度，如果存在文件更新，则开始工作
#文件绝对路径 "D:\FTP\FTPBack\"
def getFileTimeLog():
    for root, dirs, files in os.walk(filepath['ftpCheckPath']):
        for d in dirs:
            if d in dbList:
                dbFileTimeList.append( [d,os.stat(filepath['ftpCheckPath']+d).st_mtime] )
            elif d == 'dr2_xml':
                xmlFileTimeList.append( [d,os.stat(filepath['ftpCheckPath']+d).st_mtime] )


#检查文件是否更新
#遍历当前文件夹名
#对比对比当前数据库名
#检查当前文件夹下面的更新时间
def checkFileUpdate():
    filereFlush = False
    for root, dirs, files in os.walk(filepath['ftpCheckPath']):
        for d in dirs:# 遍历所有的文件夹
            if d in dbList:
                print(os.stat(filepath['ftpCheckPath']+d).st_mtime)
                fileTimeFlush = False
                for docObj in dbFileTimeList:
                    if d == docObj[0]:
                        if os.stat(filepath['ftpCheckPath']+d).st_mtime != docObj[1] : 
                            filereFlush = True
                            fileTimeFlush = True
                            dbFileTimeList.remove(docObj)
                        break
                if fileTimeFlush == True:
                    dbFileTimeList.append( [d,os.stat(filepath['ftpCheckPath']+d).st_mtime] )
            elif d == 'dr2_xml':
                fileTimeFlush = False
                print(os.stat(filepath['ftpCheckPath']+d).st_mtime)
                for docObj in xmlFileTimeList:
                    if d == docObj[0]:
                        if os.stat(filepath['ftpCheckPath']+d).st_mtime != docObj[1] : 
                            filereFlush = True
                            fileTimeFlush = True
                            xmlFileTimeList.remove(docObj)
                        break
                if fileTimeFlush == True:
                    xmlFileTimeList.append( [d,os.stat(filepath['ftpCheckPath']+d).st_mtime] )
    return filereFlush


#将运营数据复制到指定位置。并执行数据库加载工作
def updateSourceFile():
    for dbdoc in dbFileTimeList:
        tempFilepath = filepath['importSavePath'] + dbdoc[0]
        if os.path.exists(tempFilepath) == True:
            shutil.rmtree(tempFilepath)
        shutil.copytree( filepath['ftpCheckPath']+dbdoc[0], tempFilepath )
    
    cmdstr = filepath['importExcPath']
    os.chdir( filepath['importCheckPath'] )
    os.system(cmdstr)
    os.chdir( os.getcwd() )
    return 1


#将 ContentsServer 生成的 table.bin 文件复制到指定位置
def updateEndFile():
    sourceDocPath = filepath['ftpCheckPath'] + xmlFileTimeList[0][0]
    destDocpath = filepath['ftpEndPath'] 
    for root, dirs, files in os.walk( sourceDocPath ):
        for d in files:# 遍历所有的文件
            endFilePath = destDocpath + '\\' + d
            sourceFilePath = sourceDocPath + '\\' + d
            if os.path.exists( endFilePath ) == True:
                os.remove(endFilePath)
            shutil.copy(sourceFilePath, destDocpath)

    sourceDocPath = filepath['ContentsTabPath']
    for root, dirs, files in os.walk( sourceDocPath ):
        for d in files:# 遍历所有的文件
            endFilePath = destDocpath + '\\' + d
            sourceFilePath = sourceDocPath + '\\' + d
            if os.path.exists( endFilePath ) == True:
                os.remove(endFilePath)
            shutil.copy(sourceFilePath, destDocpath)

    #复制log文件
    sourcelogFilePath = filepath['importCheckPath'] + '\\' + 'log.txt'
    destlogFilePath = filepath['ftpEndPath'] + '\\' + 'log.txt'
    if os.path.exists( destlogFilePath ) == True:
        os.remove(destlogFilePath)
    shutil.copy(sourcelogFilePath, filepath['ftpEndPath'] )

    return 1


"""
执行sql命令
:param sql: sql语句
:return: 元祖
"""
def execSql(sql):
    try:
        conn = pymssql.connect(host=dbinfo['host'], user=dbinfo['user'], password=dbinfo['passwd'], database=dbinfo['datebase'])
        cur = conn.cursor()  # 创建游标
        cur.execute(sql)  # 执行sql命令
        res = cur.fetchall()  # 获取执行的返回结果
        cur.close()
        conn.close()
    except Exception as e:
        print(e)
        return False
    return res


"""
获取所有数据库名
:return: list
"""
def get_all_db():
    exclude_list = ["master", "model", "msdb", "tempdb"]            # 排除自带的数据库
    sql = "SELECT Name FROM Master..SysDatabases ORDER BY Name"     # 显示所有数据库
    res = execSql(sql)
    if not res:  # 判断结果非空
        return False

    db_list = []  # 数据库列表
    for i in res:
        db_name = i[0]
        db_name = db_name.lower()
        # 判断不在排除列表时
        if db_name not in exclude_list:
            db_list.append(db_name)

    if not db_list:
        return False

    return db_list


def main():
    readIniConfig()
    global dbList
    dbList = get_all_db()           #记录当前数据库的所有数据库名
    getFileTimeLog()

    while True:
        updateRet = False
        updateRet = checkFileUpdate()
        if updateRet == True:
            pyautogui.FAILSAFE = False          #屏幕位置异常处理
            pyautogui.hotkey('winleft', 'd')
            pyautogui.FAILSAFE = True
            updateSourceFile()
            AutoMessageServerControl()        #重启所有服务
            print('执行成功，等待三分钟获取结果。等待五分钟开始再次服务')
            time.sleep(180)
            updateEndFile()                 #将结果复制到BuildFile里面
            time.sleep(120)
            pyautogui.FAILSAFE = False
            pyautogui.hotkey('winleft', 'd')
            pyautogui.FAILSAFE = True
        else:
            print('等待三分钟继续检查文件是否更新')
            time.sleep(180)


# 增加调用main()函数
if __name__ == '__main__':
    main()