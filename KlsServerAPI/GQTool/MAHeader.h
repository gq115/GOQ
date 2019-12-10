#pragma once

#include <Windows.h>

#define UID_LENGTH				15
#define PWD_LENGTH				15
const int MAX_MA_ROLLING_LEN = 1280;
const int MAX_BEGINNERGUILD_NOTICE_LEN = 3000;

typedef enum _tagMAHeader
{
	maClose = 1,
	maMent,
	maLogin,
	maChgPwd,
	maLC,
	maVic,
	maListen,
	maNotice,
	maNoticeFile,
	maIni,
	maIniFile,
	maDoubleGold,
	maDoubleExp,
	maDoubleItem,
	maFreeInit,
	maAllowPK,
	maListenCertify,
	maListenFile,
	maListenBackup,
	maReloadBaseItem,
	maReloadOption,
	maReloadExpGoal,
	maReloadZone,
	maReloadSkill,
	maReloadCombine,
	maReloadFixedItem,
	maReloadSpecialItem,
	maReloadClass,
	maReloadGrade,
	maReloadSetItem,
	maReloadNpcProto,
	maReloadGrowth,
	maScenario,
	maScenarioData,
	maCombineEvent,
	maUserLimit,
	maRefreshNPC,
	maReloadVirtue,
	maReloadRefine,
	maGrowthNameEvent,
	maListenMobile,
	maListenLottery,
	maReloadZoneControl,
	maReloadStatics,
	maReloadLottery,
	maReloadMode,
	maReloadScript,
	maSaveAllData,
	maFormat,
	maFixDropGoldrate,
	maRollingNotice,
	maBeginnerGuildNotice,
};


typedef struct _tagMALogin
{
	BYTE header;
	char UID[UID_LENGTH];
	char passwd[PWD_LENGTH];
} SMALogin;

typedef struct _tagMAChgPwd
{
	BYTE header;
	char passwd[PWD_LENGTH];
} SMAChgPwd;

typedef struct _tagMAMent
{
	BYTE header;
	char msg[90];
} SMAMent;

typedef struct _tagMAData
{
	BYTE header;
	DWORD data1;
	DWORD data2;

	double	PercentData;
}SMAData;

typedef struct _tagScenario
{
	BYTE header;
	BYTE strName[128];
	BYTE strArg[128];
}ScenarioPacket;

typedef struct _tagMARollingMSG
{
	BYTE header;
	char szMsg[MAX_MA_ROLLING_LEN];
} SMARollingMSG;


typedef struct _tagMABeginnerGuildNotice
{
	BYTE header;
	char szMsg[MAX_BEGINNERGUILD_NOTICE_LEN];
} SMABeginnerGuildNotice;

struct GMAdmin {
	char GAccount[64];
	char GPasswd[64];
	char GIp[12];
	int	 GPort;
};