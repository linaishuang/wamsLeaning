//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	文件名:	wrdb_client_api.h

	创建人:	严耿 Copyright (c) 2021 北京四方继保自动化股份有限公司

	日  期:	2021-03-18

	描  述:	WAMS时序实时库API接口数据结构、函数定义

	版  本:	1.0
			
	修改人: 

	日  期: 

	描  述: WRDB为 Wams Realtime Database 的缩写
*************************************************************************/
//////////////////////////////////////////////////////////////////////////

#ifndef __WRDB_API_H_
#define __WRDB_API_H_

#ifdef _WIN32
#ifdef WRDB_API_EXPORTS
#define WRDB_DLL_ENTRY __declspec(dllexport)
#else
#define WRDB_DLL_ENTRY __declspec(dllimport)
#endif
#else
#define WRDB_DLL_ENTRY
#endif

#if defined (__x86_64)||(__x86_64__)||(__amd64)||(__x86_64__)||(_M_X64)
	#ifndef __int64
		#define __int64 long
	#endif
#else 
	#ifndef __int64
		#define __int64 long long
	#endif
#endif//#if defined (__x86_64)||(__x86_64__)||(__amd64)||(__x86_64__)||(_M_X64)

#ifdef __cplusplus
extern "C" { 
#endif

#include "stdlib.h"
#include "string.h"

	//数据点配置定义
	typedef struct _WRDB_RecordConfig
	{
		int iRecordID;			// 数据ID(-1为无效),本ID也作为在recordArr的下标。
		int iDatasetID;			// 所属数据集的ID
		int iIndex;				// 在压缩文件中的索引位置
		int iType;				// 数据类型（遵循WAMS实时数据库的数据类型定义)
		__int64 recv_ana_id;	// 对应d5000接收量测表的ID号
		//char codeStr[32];		// WAMS时间序列库测点英文标识
		char szDesc[64];		// 描述定义
		char szTagName[32];		// 子站测点标签名 [11/16/2010 xiexiaodong]
	} WRDB_RecordConfig;


	//数据库访问模式
	enum  ACCESS_MODE_WRDB	 
	{
		ACCESS_MODE_READONLY_WRDB			=	0,		//只读（对于只读模式，可访问全部管理的数据集）
		ACCESS_MODE_WRITEONLY_WRDB			=	1,		//只写（对于只写模式，只能访问指定的数据集）
		ACCESS_MODE_WRITEANDREAD_WRDB		=	2,		//读写（对于读写模式，只能访问指定的数据集）
	};


	//时序实时库的描述符
	struct  WRDB_DESCR
	{
		void*   lpHandle;				//访问句柄
		int		iMode;					//访问标志
		WRDB_DESCR()
		{
			lpHandle = NULL;
			iMode = -1;
		}
	};

	//WAMS时间序列库时间结构定义（与实时库中的WRDB_Time兼容，可做强制转换）
	struct WRDB_Time
	{
		int secs;
		int usecs;
	};

	//WAMS时间序列库数据值点定义（与实时库中的WRDB_Value等价，可做强制转换）
	struct WRDB_Value
	{
		WRDB_Time time;
		float value;
		char status;
	};




/*********************************************************************
功  能：	打开时序实时库接口
参  数：	descr:		时序实时库访问对象的指针
flag:		访问模式，只读，只写，读写
szDomain:	调度区域名称
返回值：	0 : 操作成功
		-1: 操作失败
用  法：	在访问时序实时库之前进行调用
			server_ip:服务端所在IP，访问本机填写NULL即可
			descr	: 访问描述符的指针
			flag	: 查询模式
			szDomain: 访问域名（暂未启用）
					: NULL		表示访问本地域
					: "本系统"	表示本地域
					: "XXXXX"	表示访问域的名字
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_Open(char * server_ip, WRDB_DESCR *descr, int flag, const char* szDomain=0);

/*********************************************************************
功  能：	关闭时序实时库接口
参  数：	pDescr:		时序实时库访问对象的指针	
返回值：	无
用  法：	在结束访问时序实时库后调用
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_Close(WRDB_DESCR *pDescr);


/*********************************************************************
功  能：	检查时序实时库接口的状态
参  数：	pDescr:		时序实时库访问对象的指针	
返回值：	
			0 : 关闭
			1 : 打开
用  法：	在访问时序实时库数据之前进行调用
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_IsOpen(WRDB_DESCR *pDescr);


/*********************************************************************
功  能：	写入数据记录
参  数：	descr:		时序实时库访问对象的指针	
iRecordID:	数据iID
iBeginSOC:	数据的开始时间(秒)
iBeginUSEC:	数据的开始时间(微秒)
iEndSOC:	数据的结束时间(秒)
iEndUSEC:	数据的结束时间(微秒)
pBuffer:	数据存储区域
iLength:	数据长度
pFlag:		数据标志存储区域
iFlagLength:数据标志存储区域长度
szDomain	:	访问域名
			:	NULL		表示访问本地域
			:	"本系统"	表示本地域
			:	"XXXXX"	表示访问域的名字

返回值：	0	:成功
			-1	:失败
用  法：	在打开访问接口后,可以调用.
			时间区间为前闭后开
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_Write(	WRDB_DESCR *descr,
							  int iRecordID,			
							  int iBeginSOC,		
							  int iEndSOC,
							  int iInterval,
							  const char* pBuffer,
							  int iLength,
							  const char* pFlag, 
							  int iFlagLength,
							  const char* szDomain = 0);



/*********************************************************************
功  能：	读取数据记录
参  数：	descr:		时序实时库访问对象的指针	
			iRecordID:	数据iID
			iBeginSOC:	数据的开始时间(秒)
			iEndSOC:	数据的结束时间(秒)
			iInterval:	查询数据的时间间隔（微秒）
			pBuffer:	数据存储区域，数据值（float型）按时间顺序排列
			iLength:	数据存储区域长度，传入-存储区域长度； 传出-实际有效数据的长度；
			pFlag:		数据标志存储区域，数据值（char型）按时间顺序排列
			iFlagLength:数据标志存储区域长度 传入-存储区域长度； 传出-实际有效质量的长度；
			szDomain	:	访问域名
						:	NULL		表示访问本地域
						:	"本系统"	表示本地域
						:	"XXXXX"	表示访问域的名字

返回值：	0	:成功
			-1	:失败（注意，如果传入存储区域长度不足，返回-1）
用  法：	在打开访问接口后,可以调用.
			查询时间区间为前闭后开,
			
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_Read(	WRDB_DESCR *descr,
							 int iRecordID,			//数据iID	 
							 int iBeginSOC,			//开始时间(秒)
							 int iEndSOC,			//结束时间(秒)
							 WRDB_Value* pValue,	//测点数据数组
							 int* iCount,			//测点数据个数
							 int iInterval = 20000,	//查询数据的时间间隔（微秒）
							 const char* szDomain = 0);



/*********************************************************************
功  能：	写入数据记录
参  数：	pDescr:		时序实时库访问对象的指针	
iRecordID:	数据iID
pValue:		写入数据缓冲区
iCount:		写入数据的个数
返回值：	0	:成功
-1	:失败
用  法：	在打开访问接口后,可以调用.
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_WriteValue(	WRDB_DESCR *pDescr,
								   int iRecordID,			
								   const WRDB_Value* pValue,
								   const int& iCount,
								   const char* szDomain = 0);



/*********************************************************************
功  能：	读取数据记录
参  数：	pDescr		:	时序实时库访问对象的指针	
			iRecordID	:	数据iID
			iBeginSOC	:	数据的开始时间(秒)
			iEndSOC		:	数据的结束时间(秒)
			iInterval	:	查询数据的时间间隔（微秒）
			pValue		:	读取数据的缓冲区
			iCount		:	读取数据的个数
			szDomain	:	访问域名
						:	NULL		表示访问本地域
						:	"本系统"	表示本地域
						:	"XXXXX"	表示访问域的名字

返回值：	0	:成功
			-1	:失败（注意，如果传入存储区域长度不足，返回-1）
用  法：	在打开访问接口后,可以调用.
			1）时间区间为前闭后开,
			2）接口会对传入的参数进行检查，如果数据存储区长度不能保存全部查询到的数据，本函数将会返回失败。
			3) 如传入的pValue=NULL，函数接口会按照计算的长度分配缓冲区，并在iCount中返回长度。
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_ReadValue(	WRDB_DESCR *pDescr,
							  int iRecordID,
							  int iBeginSOC,
							  int iEndSOC,
							  int iInterval,
							  WRDB_Value* pValue,
							  int& iCount,
							  const char* szDomain = 0);



/*********************************************************************
功  能：	插入记录
参  数：	pDescr		:	时序实时库访问对象的指针	
			pRecordConfig:	数据记录的指针
返回值：	0 ：成功
			-1：错误
用  法：	插入记录的结构中，必须要设置的字段有：
			1）RecordID		记录ID号，并且在定义的有效范围内取值。
			2）codeStr		测点英文标识，本标识在库中应为唯一描述。
			3）szDesc		中文描述
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_Insert(WRDB_DESCR *pDescr,
							   const WRDB_RecordConfig* pRecordConfig,
							   const char* szDomain = 0);		


/*********************************************************************
功  能：	删除记录
参  数：	pDescr:			时序实时库访问对象的指针	
			iRecordID: 需删除数据记录的ID号
返回值：	0：成功
			-1：错误
用  法：	
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_Delete(WRDB_DESCR *pDescr,
							   int iRecordID,
							   const char* szDomain = 0);		


/*********************************************************************
功  能：	读取最近错误代码
参  数：	pDescr:			时序实时库访问对象的指针	

返回值：	错误代码
用  法：	在读写操作失败后调用
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_Error(WRDB_DESCR *pDescr);		

/*********************************************************************
功  能：	读取最近错误描述
参  数：	pDescr:			时序实时库访问对象的指针	
返回值：	错误描述
用  法：	在读写操作失败后调用
*********************************************************************/
WRDB_DLL_ENTRY const char* WRDB_StrError(WRDB_DESCR *pDescr);		


/*********************************************************************
功  能：	读取服务端的数据集ID
参  数：	pDescr:	 历史库访问对象的指针	
返回值：	本对话服务端的数据集ID号
用  法：	本函数对于只写和读写模式有效，即写操作只能写到指定的数据集中。
			对于只读模式无意义，因在只读模式下，服务端能够访问到全部的数据集，不需知道服务端管理数据集的ID。
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_GetDatasetID(WRDB_DESCR *pDescr);

/*********************************************************************
功  能：	获取记录列表
参  数：	pDescr:		in	历史库访问对象的指针	
			pConfig:	out	测点配置信息数组指针
			iCount:		in  测点配置信息数组维数
						out 读出测点配置的数量
返回值：	0	：成功
			-1	：失败
用  法：	
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_GetRecordList(WRDB_DESCR *pDescr,
									  WRDB_RecordConfig* pConfig,
									  int& iCount,
									  const char* szDomain = 0);



/*********************************************************************
功  能：	获取本系统域名
参  数：	pDescr:		in	时序实时库访问对象的指针	
			szDomain:	out	本地系统域名
返回值：	0	：成功
			-1	：失败
用  法：	
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_LocalDomainName(WRDB_DESCR *pDescr,char* szDomain);


/*********************************************************************
功  能：	获取时序库最新事件
参  数：	pDescr:		in	时序实时库访问对象的指针	
		latest_time:	out	时序库最新时间
返回值：	0	：成功
		-1	：失败
用  法：	
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_ReadLastTime(WRDB_DESCR *pDescr, WRDB_Time & latest_time);


#ifdef __cplusplus
}
#endif

#endif // !defined(AFX_WRDB_H__0B25F7A1_D615_4294_968D_D11C8B475E9B__INCLUDED_)
