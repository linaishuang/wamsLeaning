/******************************************************************************
  ModuleName :   WAMS LFO Analysis frame
  FileName   :   WamLfoAnaObj.cpp
  Description:   WAMS LFO Analysis frame Implemention
  HISTORY :    	
      Date        Version       Author                Activities      
   ----------------------------------------------------------------------
   09/11/2007       1.0         lsf                    created
******************************************************************************/
//VC6.0设置：Project-->Settings...-->C/C++ -->General -->Preprocessors definitions: _WINDOWS32
//			 Project-->Settings...-->C/C++ -->Code Generation -->Use run-time library: Debug Multithreaded DLL
//	         Project-->Settings...-->Link  -->General -->Object/library modules --> Ws2_32.lib

//关于平台：1) TableNet 接口，更新库记录长度有限制，目前发现超过250float出错；
//			2) 多线程程序，创建两个TableNet实体，需现在一个线程中使用后，才可用；
// 公用定义： 
// 宏定义
// 函数定义
//   接口函数(输入、输出接口、告警发送、采样触发、tmd接口)
//   数据组织函数(数据预处理)
//   计算作业控制函数
#ifndef _WAM_LFO_ANA_OBJ_H_
#define _WAM_LFO_ANA_OBJ_H_

#include "pubodbapi.h"
#include "pubfun.h"
#include "pubwamsdata.h"
#include "DataFile.h"
// #include  "PubWamAppFunc.h"               //  WAMS应用公用函数
#include  "WamLfoStruct.h"                                  //  LFO监视分析结构
//#include  "WamRtCurveDisp.h"                                //  曲线显示结构
#include  "WamSsoMonObjItfc.h"                              //  数据库交互结构 WamSsoMonObjItfc.h
#include  "LFOAnaByCurve.h"                          //  LFO分析算法函数
#include "PubDataProcess.h"

#ifdef _PODB_SAMPLE_
#include "db_def/sample_def_wams.h"
#endif

// #define _LFO_ASST_DECI_                                  //  生成辅助策略
#ifdef _LFO_ASST_DECI_
#include "WamLfoAsstObj.h"
#endif
#include <vector>
#include <string>
//  宏定义
/******************************************************************************/
//#define     g_appNo                  AP_WAMAP             // 

// case state
#define L_No_PROC        0         // 未处理
#define L_PROC           1         // 正在处理中
#define L_FINISH         2         // 处理完毕

// estimate state
#define L_EST_FIRST      1          // 初次
#define L_EST_SECOND     2          // 非初次

// data state
#define L_VALID_DATA     1          // 有效数据 
#define L_BAD_DATA       2          // 坏数据

// memory state
#define NOALLOCMEM       0          // 不重新分配
#define REALLOCMEM       1          // 重新分配

// Form-Resulte state//(0 不整理 1初次启动 2整理 3统计最后结果)
#define L_NOT_FORM_RESULTE       0          // 不整理
#define L_FST_FORM_RESULTE       1          // 初次启动
#define L_SnD_FORM_RESULTE       2          // 整理
#define L_END_FORM_RESULTE       3          // 统计最后结果

// 保存数据类型  2008-06-11 by zhw
#define DATAFILEMAXLENGTH  (1024*1024*10)   //  数据文件最大长度

#define DATA_TYPE_FAC_VOLT       0          //  厂站线电压
#define DATA_TYPE_FAC_ANG        1          //  厂站相角
#define DATA_TYPE_GEN_POW        2          //  发电机有功
#define DATA_TYPE_GEN_ANG        3          //  发电机功角
#define DATA_TYPE_ACLN_POW       4          //  线路有功

//  召唤100帧文件
#define LFO_TMD_DYN_GET_NO       0          //  召唤100帧文件
#define LFO_TMD_DYN_GET_YES      1

#define  FILE_TYPE_SIM    0
#define  FILE_TYPE_DYN    1

#define DB_PIC_NO_DIS                 0                     //  画面不显示数值

#define	MAIN_ROLE			0
#define	VICE_ROLE			1
#define CALC_ROLE			2

#define    NBSAVX	          400                           // point

/*母线类别*/
#ifndef BS_TYPE_DM
#define	BS_TYPE_DM	0	/* 单母 */
#define	BS_TYPE_ZM	1	/* 正母 */
#define	BS_TYPE_FM	2	/* 副母 */
#define	BS_TYPE_PM	3	/* 旁母 */
#endif

#ifndef WAMS_SSO_WARN_TYPE
#define WAMS_SSO_WARN_TYPE 210
#endif

#ifndef AP_WAMS_SSO
#define AP_WAMS_SSO 703200
#endif

#define CONNECT_FES 1 //连接到前置
#define CONNECT_PMU 2 //连接到PMU
#define IN_TRANS    3 //传输中
#define SUCCED      4 //召唤成功
#define FAILED      5 //召唤失败

#define     MULTI_MENU(n)    ((1<<(n-1))) // 多选菜单

/******************************************************************************/
//  数据预处理参数（将来）
//  频率预处理
const float FHZ_MAX_VAL              =  53.0f;               //  频率最高数值
const float FHZ_MIN_VAL              =  47.0f;               //  频率最低数值
//  电压预处理
const float VOL_UP_LMT_COE           =  1.3f;                //  非正常电压上限系数（电压类型基值）
const float VOL_LW_LMT_COE           =  0.7f;                //  非正常电压下限系数
//  功率预处理
const float PWR_UP_LMT_COE           =  2.0f;                //  非正常功率上限系数（额定值）
const float PWR_LW_LMT_COE           = -1.0f;                //  非正常功率下限系数

//  突变判断
const short BAS_FRAME_PERSEC         =  100;                 //  最小每秒帧数
const float FHZ_MAX_CHG              =  2.0f;                //  频率最大突变
const float G_ANG_MAX_CHG            =  3.0f;                //  0.01秒功角最大变化
const float G_PWR_MAX_CHG            =  0.05f;               //  0.01秒功率最大突变（额定值）
const float L_PWR_MAX_CHG            =  30.0f;               //  0.01秒有功最大变化

//  宏定义
#define ZEROVALUE  0.0001

/******************************************************************************/

// 静态信息
//////////////////////////////////////////////////////////////////////////
#define PMU_SET_INF_FCNO_NO   0                             //  无文件号列号
#define PMU_SET_INF_FCNO_OK   1                             //  正常
#define PMU_SET_INF_FCNO_ER   3                             //  文件号列号错误


// 遥测信息定义
/******************************************************************************/
// 厂站遥测信息
typedef struct tag_LFO_FC_PMU
{
    INT_REC         fac_id;                                 //  厂站ID号
    INT_REC         m_bs_id;                                //  母线ID号
	VEC_V_FCNO      m_bs_vecV;                              //  三相相量 (UA UB UC)
    INT_REC         v_bs_id;                                //  母线ID号
	VEC_V_FCNO      v_bs_vecV;                              //  三相相量 (UA UB UC)
    VEC_YC_FCNO     vecFHZ;                                 //  厂站频率
    unsigned char   vcurSts;                                //  电压当前状态
    unsigned char   vhisSts;                                //  电压历史状态
    unsigned char   fcurSts;                                //  频率当前状态
    unsigned char   fhisSts;                                //  频率历史状态
	unsigned char   is_m_bs;                                //  正母优先
} LFO_FC_PMU;

// 机组遥测信息
typedef struct tag_SSO_GN_PMU
{
	INT_REC         gn_id;      //  机组ID号
	char            gn_name[64];   //  机组名称 
	VEC_YC_FCNO     vecAmpl;   //  振荡幅值量测
	VEC_YC_FCNO     vecFreq;    //  振荡频率量测
	VEC_YC_FCNO     vecSSO;    //  振荡状态量测
	VEC_YC_FCNO     vecPwr;    //  振荡状态量测
    unsigned char   curSts;                                 //  当前状态
    unsigned char   hisSts;                                 //  历史状态
}SSO_GN_PMU;

// 交流线路遥测信息
typedef struct tag_SSO_LN_PMU
{
	INT_REC         ln_id;      //  线路ID号
	char            ln_name[64];   //  线路名称 
	INT_REC         dot_id_1;   //  首端端点ID号
	VEC_YC_FCNO     vecAmpl1;   //  首端振荡幅值量测
	VEC_YC_FCNO     vecFreq1;    //  首端振荡频率量测
	VEC_YC_FCNO     vecSSO1;    //  首端振荡状态量测
	VEC_YC_FCNO     vecPwr1;    //  首端有功量测
	INT_REC         dot_id_2;   //  末端端点ID号
	VEC_YC_FCNO     vecAmpl2;   //  末首端振荡幅值量测
	VEC_YC_FCNO     vecFreq2;    //  末端振荡频率量测
	VEC_YC_FCNO     vecSSO2;    //  末端振荡状态量测
	VEC_YC_FCNO     vecPwr2;    //  末端有功量测
    unsigned char   curSts;                                 //  当前状态
    unsigned char   hisSts;                                 //  历史状态
	unsigned char   is_end;                                 //  末端优先
}SSO_LN_PMU;

// 变压器遥测信息
typedef struct tag_SSO_TR_PMU
{
	INT_REC         tr_id;      //  变压器ID号
	char            tr_name[64];   //  变压器名称 
	INT_REC         high_wind_id;   //  变压器高压侧ID号
	VEC_YC_FCNO     vecAmpl_high;   //  高压侧振荡幅值量测
	VEC_YC_FCNO     vecFreq_high;    //  高压侧振荡频率量测
	VEC_YC_FCNO     vecSSO_high;    //  高压侧振荡状态量测
	VEC_YC_FCNO     vecPwr_high;    //  高压侧振荡状态量测
	INT_REC         mid_wind_id;   //  变压器中压侧ID号
	VEC_YC_FCNO     vecAmpl_mid;   //  中压侧振荡幅值量测
	VEC_YC_FCNO     vecFreq_mid;    //  中压侧振荡频率量测
	VEC_YC_FCNO     vecSSO_mid;    //  中压侧振荡状态量测
	VEC_YC_FCNO     vecPwr_mid;    //  高压侧振荡状态量测
	INT_REC         low_wind_id;   //  变压器低压侧ID号
	VEC_YC_FCNO     vecAmpl_low;   //  低压侧振荡幅值量测
	VEC_YC_FCNO     vecFreq_low;    //  低压侧振荡频率量测
	VEC_YC_FCNO     vecSSO_low;    //  低压侧振荡状态量测
	VEC_YC_FCNO     vecPwr_low;    //  高压侧振荡状态量测
    unsigned char   curSts;                                 //  当前状态
    unsigned char   hisSts;                                 //  历史状态
    INT_REC         slct_wind_id;                           //  选择绕组ID
    unsigned char   hwd_yc_ok;                              //  高压侧测点OK
    unsigned char   mwd_yc_ok;                              //  中压侧测点OK
    unsigned char   lwd_yc_ok;                              //  低压侧测点OK
}SSO_TR_PMU;

//长录波文件信息
typedef struct tag_HD_File_Struct
{
	INT_REC fac_id;//厂站名称
	char cfg_name[64];//配置文件名
	char dat_name[64];//数据文件名
}HD_File_Struct;
//启动SSO分析消息文件结构
typedef struct tag_SSO_MSG_FILE_STRUCT
{
	char version[10];  // 版本号
	char dir[64];  // 录波文件存储目录
	RTDB_TIME start_time;  // 振荡起始时间
	int file_num;//录波文件数目
	vector<HD_File_Struct> vfile;//长录波文件
} SSO_MSG_FILE_STRUCT;
//召唤长录波文件的厂站信息
typedef struct tag_SSO_HD_FILE_FAC
{
	INT_REC fac_id;
	int stat;
	char file_name[40];
} SSO_HD_FILE_FAC;

/******************************************************************************/

//  case并行计算宏定义
/******************************************************************************/
#define CALC_THRD_NUM                   2                    //  并行计算线程数
#define THRD_CASE_NUM                   20//10                   //  计算线程Case数
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
////  并行计算case
typedef struct  tag_CALC_THRD_CASE
{	
#ifdef _WINDOWS32
	HANDLE          ThrdCaseMutex;                          //  线程互斥体
#else
	pthread_mutex_t ThrdCaseMutex;                          //  线程互斥体
#endif
	int             thrd_id;                                //  线程id
	int             isStartUp;                              //  是否启动计算（主进程置位 0 否     1 启动）
	int             isFinish;                               //  是否完成    （线程置位   0 未完成 1 完成）
	int             nCases;                                 //  线程case数
	CASE_TYPE       pcases[THRD_CASE_NUM];                  //  线程case
}CALC_THRD_CASE;
/******************************************************************************/

//  外部宏定义
/******************************************************************************/
//////////////////////////////////////////////////////////////////////////
extern  bool                   g_ProcessExitFlag ;                  //  进程退出标志
extern  int                    g_nCalcThrd       ;                  //  线程个数
extern  CALC_THRD_CASE *       g_pCalcThrdCase   ;                  //  线程case
////////////////////////////////////////////////////////////////////////////////
////  thread访问互斥体锁定和解锁函数
#ifdef _WINDOWS32
	extern "C" void    THRD_LockThreadMutex(    HANDLE  mutex ); 
	extern "C" void    THRD_UnLockThreadMutex(  HANDLE  mutex ); 
	extern "C" void    THRD_InitThreadMutex(    HANDLE  &mutex, char * mutexName ); 
	extern "C" void    THRD_ReleaseThreadMutex( HANDLE  mutex ); 

    extern "C" int     ClacCtlMainProcess( int nCase,  CASE_TYPE * pCase );

	extern "C" unsigned long WINAPI CalcThreadMain(void *param);
	extern unsigned __stdcall CallDynFileTMain(void *para);

	extern "C" unsigned long WINAPI ThreadFunc_RealtimeCurveProcess (void *lparm);
	extern "C" int     Thred_NetTableGetRecNum(int iAppNo, int iTableNo );
	extern "C" void    Thred_UpdateLfoCurve( TAB_LFO_RT_CURVE RtCurve  ) ;

	extern "C" unsigned long WINAPI ThreadFunc_MessageProcess(void *lparm);
#else
	extern "C" void    mypthread_mutex_unlock(void* mymutex);  //  AIX下需重新封装互斥体解锁函数
	extern "C" void    THRD_LockThreadMutex(    pthread_mutex_t  mutex, int oldtype ); 
	extern "C" void    THRD_UnLockThreadMutex(    pthread_mutex_t  mutex, int oldtype );
	//extern "C" void    THRD_UnLockThreadMutex(  pthread_mutex_t  mutex, int oldtype ); 
	extern "C" void    THRD_InitThreadMutex(    pthread_mutex_t  mutex, char * mutexName ); 
	extern "C" void    THRD_ReleaseThreadMutex( pthread_mutex_t  mutex ); 

    extern "C" int     ClacCtlMainProcess( int nCase,  CASE_TYPE * pCase );
	extern "C" void    CalcThreadMain(void *param);
	extern void *CallDynFileTMain(void *);

	extern "C" void    ThreadFunc_RealtimeCurveProcess (void *lparm);
	extern "C" int     Thred_NetTableGetRecNum(int iAppNo, int iTableNo );
	extern "C" void    Thred_UpdateLfoCurve( TAB_LFO_RT_CURVE RtCurve  ) ;

	extern "C" void    ThreadFunc_MessageProcess(void *lparm);
#endif
////////////////////////////////////////////////////////////////////////////////



class CLfoAnaObj : public CPubFun, public CPubOdbApi, public CPubWamsData
{

public:
	////  配置参数如下：
    int                  para_nFcMaxMem;                    //  开内存厂站最大数
    int                  para_nGnMaxMem;                    //  开内存机组最大数
    int                  para_nLnMaxMem;                    //  开内存线路最大数
    int                  para_nTrMaxMem;                    //  开内存变压器最大数
	int                  para_nAnaMaxFcNum;                 //  分析厂站最大数
    int                  para_nItfcMaxMem;                  //  开内存断面最大数
	int                  para_nItfcMaxDev;                  //  开内存断面最大线路数
	int                  para_nShwMaxFcNum;                 //  每群显示厂站最大数
	// 增加百分比控制
	int                  para_nCaseMaxMem;                  //  算例个数 (para_nAnaMaxFcNum+para_nGnMaxMem+para_nLnMaxMem+para_nTrMaxMem)
	int                  para_LfoAnaByPwr;                  //  数据选项
	int                  para_LfoAnaByEEAC;                 //  方式选项
    int                  para_nSysMaxMode;                  //  开内sysmode数
    int                  para_nObjMaxMode;                  //  开内objmode数(3*para_nCaseMaxMem)
	// 显示曲线控制
    int                  para_nDisplyCurve;                 //  开内存曲线数
	// 主动滞后秒数（监视和分析均5s）
    int                  para_nDelaySec;                    //  取数据主动延后秒数
	int                  para_CalcPQVByUI;                  //  是否计算PQV选项
    int                  para_CalcAngByEA;                  //  是否计算功角  2008-05-22 by zhw
    // 2008-04-06 added by zhw at yn
    int                  para_SaveAllFac;                   //  tmd是否保存所有厂站数据
    int                  para_CallAllFacDyn;                //  是否召唤所有厂站文件
    time_t               para_nGpsTime;                     //  数据案例测试时Gps时刻
    int                  para_SaveDataToFile;                 //  数据保存开关 2008-06-10 by zhw
    int                  para_DataSaveNum;                    //  数据文件保存个数
    int                  para_nAsstDeciMax;                   //  辅助决策最大数  2008-09-09
    int                  para_SaveXmlFile;                    //  是否生成XML文件
    char                 para_XmlFileDir[MAXBUF];          //  山西应急系统接口文件目录
    vector<string>       m_XmlFileName;                         //  保存文件名，ftp发送时使用

	// 福建E语言格式接口文件
	int                  para_SaveResFile;                    // 1,保存文件
	char                 para_FtpServer[MAXBUF];                      // FTP服务器IP
	char                 para_FtpUser[MAXBUF];           // FTP服务器用户名
	char                 para_FtpPsw[MAXBUF];			  // 密码
	char                 para_ResFileName[MAXBUF];       // 结果文件名
	char                 para_FtpDir[MAXBUF];            // FTP目录
	char                 para_FtpCmd[MAXBUF];            // 命令行
    int                  para_DebugInfo;                      //  调试信息开关  // 2008-07-16
    float                para_Ampl_aver;                      //  解决斜直线问题，主导模式幅值占波形均值比  // 2008-08-12
    float                para_FcVoltPer;                      //  厂站电压预判百分比  // 2008-08-13
    float                para_VoltAmplPer;                    //  厂站电压幅值百分比  // 2008-08-14
    float                para_MaxAnglDiff;                    //  曲线突变角度差
    int                  para_IsCallDynFile;                  //  是否召唤100帧文件
    int                  para_IsCallComtrFile;                //  是否召唤录波文件
    int                  para_CallDynDelaySec;                //  召唤100帧文件延迟时间
    int                  para_IsShowAnaRes;                   //  TMD是否显示参与机组,振荡中心信息
    int                  para_IsShowAsstDeci;                 //  TMD是否显示辅助决策
    int                  para_IsShowAnaData;                  //  TMD是否显示设备测点，频率，阻尼比信息
    int                  para_IsFactByClust;                  //  在分群结果中选择幅值最大的设备,做为参与因子的基准
    int                  para_UpAppVariable;                  //  1,WAMS应用模块告警信息表;0,不更新
    int                  para_FormAsstDeci;                   //  形成辅助策略(合并)
	int                  para_UpDevLfoStat;                   //  更新公共设备表LFO状态
	int                  para_IsDataCycle;                    //  1,循环读取数据;0,不循环读取数据
public:
    int                  para_FilePmuInfo;                   //  DevicePmuInfo.txt 设备测点信息
    // 0不创建文件; 1创建文件后追加内容; 2覆盖创建文件
    int                  para_FileModeTxt;                   //  AllObjMode.txt ModeShape.txt LfoCentor.txt
    // 0不输出; 1输出计算简要信息; 2输出模式信息; 3输出数据
    int                  para_DebugProny;                    //  PRONY调试选项
    int                  para_IsFltSim;                      // 是否过滤模拟量;1,过滤;0,不过滤;默认值为0
    int                  para_IsWarnIA;                     // 是否向综合智能告警发送消息(科东);1,发送;0,不发送
    int                  para_ShowFacName;                   // 是否显示厂站名称;1,显示;0,不显示;默认为1
    int                  para_WarnAppNo;                     // d5000发送告警的应用;700000
    int                  para_IsDataValid;                     // 是否判断数据状态;1,判断;0,不判断;默认值为0
public:
	// 数据处理控制
    int                  m_nSmpFre;                         //  采样频率
    int                  m_nSmpStp;                         //  采样间隔
	time_t               m_tNewSec;                         //  取数GPS时刻(m_CurGpsTime-m_nMovSec) 
    int                  m_nMovSec;                         //  取数移动秒数
    int                  m_iOffPos;                         //  取数偏移位置
	time_t               m_CalcStartTime;                   //  计算起始GPS时刻(m_CurGpsTime-time_leng) 
    int                  m_iCalcOffPos;                     //  计算偏移位置
	time_t               m_CurGpsTime;                      //  当前GPS时刻(截止GPS时刻)
	time_t               m_OldGpsTime;                      //  上次GPS时刻(截止GPS时刻)
public:
	float                m_nOrigDlt;					    //  原始采样点间隔
	int                  m_nOrigSam;					    //  原始采样点个数
	float                m_Matchtime[DATA_NUM_MAX];         //  匹配时刻数据
	int                  m_nSamples ;					    //  计算用采样点个数
	int                  m_nWarnWdsnum ;					//  振荡窗口数
	float				 m_fAverFreq;						//	平均频率
	float                m_ProMatchtime[DATA_NUM_MAX];      //  计算用匹配时刻
public:
	int                  m_setParaFlag;                     //  输出参数文件
	int                  m_reAllocmem;                      //  更新内存
	int                  m_LfoAnaByPwr;                     //  分析开关（1 有功 0 相对相角）

public:
	int                  m_DataFileAskPnt;                  //  连续数据文件要求读取数据数
	int                  m_FcDataFilePos;                   //  数据文件读取位置
	int                  m_FcDataFileOffset;                //  相对数据文件读取位置的偏移量
	int                  m_FcDataFileAskPnt;                //  连续数据文件要求读取数据数
	int                  m_GnDataFilePos;                   //  数据文件读取位置
	int                  m_GnDataFileOffset;                //  相对数据文件读取位置的偏移量
	int                  m_GnDataFileAskPnt;                //  连续数据文件要求读取数据数
	int                  m_LnDataFilePos;                   //  数据文件读取位置
	int                  m_LnDataFileOffset;                //  相对数据文件读取位置的偏移量
	int                  m_LnDataFileAskPnt;                //  连续数据文件要求读取数据数
    int                  m_TrDataFilePos;                   //  数据文件读取位置
    int                  m_TrDataFileOffset;                //  相对数据文件读取位置的偏移量
	int                  m_TrDataFileAskPnt;                //  连续数据文件要求读取数据数
    int                  m_FcVolFilePos;                    //  厂站电压读取位置  //fac_volt.txt
    int                  m_FcAngFilePos;                    //  厂站相角读取位置  //fac_angle.txt
    int                  m_FcVolFileOffset;                 //  厂站电压读取位置的偏移量
    int                  m_FcAngFileOffset;                 //  厂站相角读取位置的偏移量
    int                  m_GnPowFilePos;                    //  发电机有功读取位置  //gn_power.txt
    int                  m_GnAngFilePos;                    //  发电机功角读取位置  //gn_angle.txt
    int                  m_GnPowFileOffset;                 //  发电机有功读取位置的偏移量
    int                  m_GnAngFileOffset;                 //  发电机功角读取位置的偏移量
    int                  m_CallHDFile;                      //  是否正在召唤长录波文件标志
	int                  m_HDFileOk;						//  长录波文件召唤完成标志

public:
    PARA_LFO_ANA         m_LfoPara;                         //  参数信息
	float                para_AnaWnapPer;                 //  启动分析限值
    PARA_DISP_CTL        m_DispCtl;                         //  输出控制
    PARA_PROC_CTL        m_ProcCtl;                         //  进程信息
public:
    int                  m_nFcNum;                          //  LFO厂站个数
    SSO_FC_INF *         m_pFcInf;                          //  LFO厂站静态信息
    SSO_FC_CTL *         m_pFcCtl;                          //  LFO厂站控制信息
    LFO_FC_PMU *         m_pFcPmu;                          //  LFO厂站PMU信息
    LFO_FC_DAT *         m_pFcDat;                          //  LFO厂站动态数据
    SSO_FC_MON *         m_pFcMon;                          //  LFO厂站监视信息
    SSO_FC_MON *         m_pFcLfo;                          //  LFO厂站振荡信息
	int                  m_nFcProDat;                       //  需处理的厂站
	LFO_FC_DAT *         m_pFcProDat;                       //  需处理的厂站数据((预处理后)
    LFO_FC_ANA *         m_pFcAna;                          //  LFO厂站分析信息
	int					 m_nFcWarn;                         //  窗口告警厂站数
    SSO_FC_MON *         m_pFcWarn;                         //  窗口告警厂站信息
	int					 m_nEveFcWarn;                      //  事件告警厂站数
    SSO_FC_MON *         m_pEveFcWarn;                      //  事件告警厂站信息
	int					 m_nFcHDFile;						//  召唤长录波文件厂站个数
	SSO_HD_FILE_FAC	*	 m_pFcHDFile;						//  召唤长录波文件厂站信息
	vector<FES_COMM_TAB_CRE> m_vCOMMFc;						//  前置通讯厂站表
public:
    int                  m_nGnNum;                          //  LFO机组台数
    SSO_GN_INF *         m_pGnInf;                          //  LFO机组静态信息
    SSO_GN_CTL *         m_pGnCtl;                          //  LFO机组控制信息
    SSO_GN_PMU *         m_pGnPmu;                          //  LFO机组PMU信息
    SSO_GN_DAT *         m_pGnDat;                          //  LFO机组动态数据
    SSO_GN_MON *         m_pGnMon;                          //  LFO机组监视信息
	OSCILLAT_FLAG *      m_pGnOsc;                          //  LFO机组振荡标志
    OBJ_ACT_FACT *       m_pGnFact;                        //  LFO机组参与因子  2008-04-04 added by zhw at yn
	int                  m_nGnProDat;                       //  需处理的机组
	SSO_GN_DAT *         m_pGnProDat;                       //  需处理的机组数据(预处理后)
	int                  m_nGnWarn;
	int					 sum_WarnGen;
	SSO_GN_MON *         m_pGnWarn;
 	LFO_OBJ_RES_SAVE *   m_pGnResSave;                      //  监视设备结果
	SSO_GN_MON *         m_DevGnWarn;							//  告警设备结果
	SSO_GN_MON *         sum_DevGnWarn;
    SSO_GN_DAT *         sum_pGnDat;                          //  LFO机组动态数据
    SSO_GN_INF *         sum_pGnInf;      
	TYPE_GN_WARN_TIME *  gn_warn_time;
	multimap<INT_REC, int>				g_map_gn_fac;//发电机厂站ID

public:
    int                  m_nLnNum;                          //  LFO线路数
    SSO_LN_INF *         m_pLnInf;                          //  LFO线路静态信息
    SSO_LN_CTL *         m_pLnCtl;                          //  LFO线路控制信息
    SSO_LN_PMU *         m_pLnPmu;                          //  LFO线路PMU信息
	SSO_LN_DAT *         m_pLnDat;                          //  LFO线路动态数据
    SSO_LN_MON *         m_pLnMon;                          //  LFO线路监视信息
	OSCILLAT_FLAG *      m_pLnOsc;                          //  LFO线路振荡标志
    OBJ_ACT_FACT *       m_pLnFact;                        //  LFO线路参与因子  2008-05-21 added by zhw at nj
	int                  m_nLnProDat;                       //  需处理的线路
	SSO_LN_DAT *         m_pLnProDat;                       //  需处理的线路数据(预处理后)
	int                  m_nLnWarn;
	int					 sum_WarnLn;
	SSO_LN_MON *         m_pLnWarn;
 	LFO_OBJ_RES_SAVE *   m_pLnResSave;                      //  监视设备结果
	SSO_LN_MON *         m_DevLnWarn;							//  告警设备结果
	SSO_LN_MON *		 sum_DevLnWarn;
    SSO_LN_DAT *         sum_pLnDat;      
    SSO_LN_INF *         sum_pLnInf;    
	TYPE_LN_WARN_TIME *  ln_warn_time;
	multimap<INT_REC, INT_REC>				g_map_ac_dot;//交流线段端点id
	map<INT_REC, int>				g_map_acln_dot;//交流线段端点id
	vector<INT_REC>		 m_vAcDot_id; // 案例演示中需要置为的端点

public:
    int                  m_nTrNum;                          //  LFO变压器数
    SSO_TR_INF *         m_pTrInf;                          //  LFO变压器静态信息
    SSO_TR_CTL *         m_pTrCtl;                          //  LFO变压器控制信息
    SSO_TR_PMU *         m_pTrPmu;                          //  LFO变压器PMU信息
    SSO_TR_DAT *         m_pTrDat;                          //  LFO变压器动态数据
    SSO_TR_MON *         m_pTrMon;                          //  LFO变压器监视信息
    OSCILLAT_FLAG *      m_pTrOsc;                          //  LFO变压器振荡标志
    int                  m_nTrProDat;                       //  需处理的变压器
    SSO_TR_DAT *         m_pTrProDat;                       //  需处理的变压器数据(预处理后)
    int                  m_nTrWarn;
	int					 sum_WarnTr;
    SSO_TR_MON *         m_pTrWarn;
 	LFO_OBJ_RES_SAVE *   m_pTrResSave;                      //  监视变压器结果
	SSO_TR_MON *         m_DevTrWarn;							//  告警设备结果
	SSO_TR_MON *		 sum_DevTrWarn;
    SSO_TR_DAT *         sum_pTrDat; 
    SSO_TR_INF *         sum_pTrInf;   
	TYPE_TR_WARN_TIME *  tr_warn_time;
	multimap<INT_REC, int>				g_map_tr_fac;//变压器厂站ID

public:
	bool                 m_LfoAmplOverLmt;                  //  LFO时，振幅越限标志
    SSO_SYS_MON          m_SysMon;                          //  LFO系统监视信息
    SSO_SYS_MON          m_SysMon_Old;                      //  LFO系统前一轮监视信息
    LFO_SYS_LFO          m_SysLfo;                          //  LFO系统振荡信息
    LFO_SYS_ANA          m_SysAna;                          //  LFO系统分析信息
	int					 m_nDevWarn;						//  窗口振荡设备总数
	SSO_DEV_MON	*		 m_pDevWarn;						//	窗口振荡设备信息
	int					 m_nDevEveWarn;						//  事件振荡设备总数
	SSO_DEV_MON	*		 m_pDevEveWarn;						//	事件振荡设备信息
	vector<FAD_TRANS_REPORT_STRUCT> m_vTranReport;			//  召唤录波文件返回标志
public:
	//////////////////////////////////////////////////////////////////////////
	int                  m_nCases;                          //  算例个数
	CASE_TYPE *          m_pCases;                          //  算例信息
	//int                  m_nCalcThrd;                       //  线程个数
	//CALC_THRD_CASE *     m_pCalcThrdCase;                   //  线程case
	int                  m_wRoleID;                         //  节点角色
	//////////////////////////////////////////////////////////////////////////
public:
	int                  m_nMonMode;	                    //  mon 模式数
	PRONY_ANAL_RSLT *    m_pMonMode;	                    //  mon 模式
	///////////////////////////////曲线显示///////////////////////////////
public:
	// 用于显示LFO实测振荡曲线
	#ifdef _WINDOWS32
		HANDLE            m_RtCurvemutex;                   //  实时曲线互斥体
		HANDLE            m_DynFilemutex;                   //  召唤100帧文件互斥体
	#else
		pthread_mutex_t   m_RtCurvemutex;                   //  实时曲线互斥体
		pthread_mutex_t   m_DynFilemutex;
	#endif
	SSO_GN_PMU            m_LfoCurveGenDev ;                //  实时曲线对应发电机
	SSO_LN_PMU            m_LfoCurveLineDev;                //  实时曲线对应联络线
    SSO_TR_PMU            m_LfoCurveTrDev;                  //  实时曲线对应变压器
// 	RTIME_CUR_DEV         m_LfoCurveGenDev ;                //  实时曲线对应发电机
// 	RTIME_CUR_DEV         m_LfoCurveLineDev;                //  实时曲线对应联络线
	TAB_LFO_RT_CURVE      m_GenLfoCurve  ;                  //  发电机对应实时曲线(功角)
	TAB_LFO_RT_CURVE      m_AclnLfoCurve ;                  //  联络线对应实时曲线(有功)
	TAB_LFO_RT_CURVE      m_TimeLfoCurve ;                  //  实时曲线对应时间
    TAB_LFO_RT_CURVE      m_GenPowCurve  ;                  //  发电机对应实时曲线(有功)  2008-06-11 by zhw
    TAB_LFO_RT_CURVE      m_TrLfoCurve   ;                  //  变压器实时曲线(有功)
	// 用于显示LFO实测振荡模式曲线
	PERIOD_LEAD_MODE      m_pLfoModeCurve[LFO_CURVE_MAX_PNT];//  画面模式曲线
	TAB_SSO_RT_CURVE2	  m_SsoCurve2[LFO_CURVE_MAX_PNT];  // 画面SSO实时曲线横向表
public:
	#ifdef _WINDOWS32
		HANDLE           m_Itfcmutex;                       //  断面互斥体
	#else
		pthread_mutex_t  m_Itfcmutex;                       //  断面互斥体
	#endif
    //int                  m_nItfcNum;                        //  LFO考察断面数
    //LFO_INTERFACE *      m_pItfcDat;                        //  LFO断面监视信息
    //int                  m_nItfcDevNum;                     //  LFO断面线路数
    //LFO_INTERFACE_DEV *  m_pItfcDevInf;                     //  LFO断面线路信息
    //SSO_LN_PMU *         m_pItfcDevPmu;                     //  LFO断面线路PMU信息
	//LFO_LN_DAT *         m_pItfcDevDat;                     //  LFO断面线路动态数据(预处理后)
////////////////////////////////分析//////////////////////////////////////
public:
	//LFO_REF_ANG_DAT
	// 相角参考点选取
    INT_REC              m_AngRefID;                        //  参考相角id
	float                m_AngRefDat[DATA_NUM_MAX];         //  参考相角数据
	float                m_AngRefDat2[DATA_NUM_MAX];        //  参考相角数据
public:
	int                  m_nAllObjMode;                     //  单曲线模式总数(剔除相同模式)
	MODE_PART_OBJ *      m_pAllObjMode;                     //  单曲线模式
	int                  m_nAllMode;                        //  未筛选模式总数
	MODE_CHRCT *         m_pAllMode;                        //  未筛选模式
	int                  m_nSltMode;                        //  系统模式总数
	MODE_CHRCT *         m_pSltMode;                        //  系统模式(分群细化)
	MODE_CHRCT           m_DispMode;
	//GEN_LEAD_CLUS *      m_GnLeadClus;
	//不在 m_pSltMode 中的机组则不参与该振荡模式
	int                  m_nLfoMode;                        //  LFO期间模式总数
	MODE_CHRCT *         m_pLfoMode;                        //  LFO期间模式
    LFO_CALC_WDS *       m_pLfoWds;                         //  LFO期间计算窗口
// 振荡中心区域 
public:
	//////////////////////////////////////////////////////////////////////////
	int                 m_nCntrFc;                         //  振荡中心厂站数
	SSO_FC_MON *        m_pCntrFc;                         //  振荡中心厂站信息
	int                 m_nCntrLn;                         //  振荡中心厂站数
    LFO_CNTR_LN *       m_pCntrLn;                         //  LFO振荡中心线路
public:
// 辅助控制措施信息(依据振荡中心和参与因子)
#ifdef _LFO_ASST_DECI_
    int                 m_nAsstDeci;                       //  辅助决策数
    TAB_LFO_ASST_DECI * m_pAsstDeci;
#endif
	// 振荡性质（区内/区外；信息不够）
	// 说明振荡双方
	// 说明调整建议
//////////////////////////////////////////////////////////////////////////
	
//////////////////////////////////////////////////////////////////////////
#ifdef _WINDOWS32
	HANDLE          m_logmutex;                             //  日志访问互斥体
#else
	pthread_mutex_t m_logmutex;                             //  日志访问互斥体
#endif
	char    m_backslash[2];
	char    m_ProcName[20];                         //  运行信息目录
	char    m_RunInfDir[MAXBUF];                         //  运行信息目录
	char    m_CasefDir[MAXBUF];                         //  交互文件目录
	char    m_LogString[MAXBUF];                         //  日志文件字符串
	void    MessShow( const char * logstr ,const int logtype = LTINFO);
	//void    WriteColIDtoLogFile() ;
public:
    // 2008-06-10 by zhw at nj
    char   m_SaveFileDir[MAXBUF];
	char   m_DataDir[MAXBUF];
    char   m_DataFileDir[MAXBUF];
    char   m_HDFileDir[MAXBUF];//长录波文件目录 $D5000_HOME/var/pmu_offline_file/comtr_file
    string   m_LnPowFileName;
    string   m_FacVoltFileName;
    string   m_FacAngFileName;
    string   m_GnPowFileName;
    string   m_GnAngFileName;
    string   m_TrPowFileName;
    string   m_LfoResFileName;
	// 案例文件目录名 2008-09-26
	char   m_DataFile[MAXBUF];
	char   m_DataFile1[MAXBUF];
	char   m_DataFile2[MAXBUF];
	char   m_DataFile3[MAXBUF];
	char   m_DataFile4[MAXBUF];
    void   WriteDataToFile( int nType, int realNum, float t_start, float t_end, float t_delta, int nOffSec );  // 数据保存
	int    CheckDataFile(const char* fileName);    //数据文件管理函数	
	char*  TrimLeft(char* str);                               //  去掉字符串左端的空格
public:
	 CLfoAnaObj();
	~CLfoAnaObj();
public:
	int     ReadDefaultFile();                              //  参数配置信息
	bool    AllocMemforPointer();                           //  分配内存空间
    void    ReleaseMem();                                   //  释放内存
public:
    bool    InitPrject();                                         //  获取基本信息
    bool    InitFc(vector<FAC_TAB_CRE> &VecFacTab,vector<AREA_TAB_CRE> &VecAreaTab,vector<FES_YC_TAB_CRE> &VecFesYcTab);  //  厂站信息
    bool    InitLn(vector<FAC_TAB_CRE> &VecFacTab,vector<AREA_TAB_CRE> &VecAreaTab,vector<FES_YX_YC> &VecFesYcTab);  //  线路信息
    bool    InitGn(vector<FAC_TAB_CRE> &VecFacTab,vector<AREA_TAB_CRE> &VecAreaTab,vector<FES_YX_YC> &VecFesYcTab);  //  机组信息
    bool    InitItfc(vector<FAC_TAB_CRE> &VecFacTab,vector<AREA_TAB_CRE> &VecAreaTab,vector<FES_YC_TAB_CRE> &VecFesYcTab);  //  断面信息
    bool    InitTr(vector<FAC_TAB_CRE> &VecFacTab,vector<AREA_TAB_CRE> &VecAreaTab,vector<FES_YX_YC> &VecFesYcTab);  //  变压器信息
//     int     GetFacInfo(vector<FAC_TAB_CRE> &VecFacTab);
// 	int     GetAreaInfo(vector<AREA_TAB_CRE> &VecAreaTab);
// 	int     GetVLTypeInfo(vector<VL_TYPE_TAB_CRE> &VecVLTypeTab);
// 	int     GetFesYcInfo(vector<FES_YC_TAB_CRE> &VecFesYcTab);
// 	int     GetBusInfo(vector<BUS_TAB_CRE> &VecBusTab);
// 	int     GetAclnInfo(vector<ACLINE_TAB_CRE> &VecAclnTab);
// 	int     GetAclnDotInfo(vector<ACLNDOT_TAB_CRE> &VecAclnDotTab);
// 	int     GetGenInfo(vector<GEN_TAB_CRE> &VecGenTab);
// 	int     GetTrInfo(vector<TR_TAB_CRE> &VecTrTab);
// 	int     GetTrwdInfo(vector<TRWD_TAB_CRE> &VecTrwdTab);
	int     GetSsoFac(vector<SSO_FC_TAB_CRE> &VecLfoFcTab);
	int     GetSsoAcln(vector<SSO_LN_TAB_CRE> &VecLfoAclnTab);
	int     GetSsoGen(vector<SSO_GN_TAB_CRE> &VecLfoGnTab);
	int     GetSsoTr(vector<SSO_TR_TAB_CRE> &VecLfoTrTab);
	int     WritePmuInfo(const int devtableno);
	int     GetLfoRTCurveId(vector<TAB_SSO_RT_CURVE_ID> &VecLfoRt);
public:
	void    InitialLFOEstimate();                           //  每轮计算初始化参数
    int     CheckCalcCtlPara( ) ;                           //  参数检查
public:
	int     HandlingPmuSampleData();                        //  获取数据（缓冲区和文件）
	int     GetFcData( float &t_start, float &t_end, float &t_delta, int dsource = 0 );//  获取厂站数据
    int     GetLnData( float &t_start, float &t_end, float &t_delta, int dsource = 0 );//  获取线路数据
    int     GetGnData( float &t_start, float &t_end, float &t_delta, int dsource = 0 );//  获取机组数据
    int     GetTrData( float &t_start, float &t_end, float &t_delta, int dsource = 0 );//  获取变压器数据
    int     para_FileType;                                  //  文件类型 0:SIM 1:DYN
public:
	int     FormFcProData(  );                              //  厂站数据处理
    int     FormLnProData(  );                              //  线路数据处理
    int     FormGnProData(  );                              //  机组数据处理
public:
	void    MainProcess();
	int     FormMonProData();                               //  mon数据处理
	void    FormMonCaseData();
	void    MonClacProcess();                               //  LFO判断流程
	int     EstimateObjOscillate();							//  设备次同步振荡判断
	int     EstimateSysOscillate();							//  系统次同步振荡判断
	void	PushCallFac();									//	形成召唤厂站队列
	void	PushEveFac();									//	统计振荡事件中的振荡厂站
	bool    IsWarnDev( INT_REC dev_id , int &ii);									//	判断振荡设备在本次振荡事件中是否已经统计过
	bool    IsWarnFac( INT_REC fac_id , int &ii);									//	判断振荡设备的厂站是否统计过
	int     SelectRefAngle( ); 
	int     SelectRefAngle2( ); 
	int     FormAnaProData( );                              //  ana数据处理
	void    FormAnaCaseData( );
	void    AnaCalcProcess();                               //  LFO分析流程
	int     GetVarNsquare(int var, int n);
public:
	//  eeac
	void    WAMAPDataHandling();
    void    EEACIdentifyCluster();
public:
	//  prony
//	int     PronyAnalysisProcess( CASE_TYPE * case_pointer );
//	void    ClacCtlMainProcess( int nCases,  CASE_TYPE * pCase ); // 计算作业控制()
public:
	//  mon
	void    ModeResultHandling( float damp_lim );           //  模式结果信息
	void    ExtractLeadMode();                              //
//	void    EstimateObjOscillate();                         //  判断监视对象(临界群)振荡情况
//	void    EstimateSysOscillate();                         //  判断系统振荡情况(刷新系统状态，并发告警)
    void    EstimateSingleObjState( OSCILLAT_FLAG  *osciFlag, LFO_LEAD_MODE *leadMode, int times, float SameFreqDifVal );
	void    UpdateMonLfoToWAMAP();                          // 在线监视信息
public:
	//  ana
	void    LfoClctMainMode(); // case result   -> m_pAllObjMode	
    void    LfoAnaEEACIdentifyCluster();
	void    LfoAnaPartGenPreHandle( int nGnNum, SSO_GN_INF *pGnInf, int nPartGn, MODE_PART_OBJ *pPartGn);
	void    LfoAnaSetPartFact0(   int nPartGn, MODE_PART_OBJ *pPartGn );
	//      2008-09-24 lsf add 统一参与因子基准
	void    LfoAnaSetLnClustbyFc( int nPartLn, MODE_PART_OBJ *pPartLn, int nPartFc, MODE_PART_OBJ *pPartFc );
	void    LfoAnaSetTrClustbyFc( int nPartTr, MODE_PART_OBJ *pPartTr, int nPartFc, MODE_PART_OBJ *pPartFc );
	void    LfoAnaSetGnAmpl(      int nPartGn, MODE_PART_OBJ *pPartGn );
	void    LfoAnaResetFact(      MODE_CHRCT *pSltMode );
	//////////////////////////////////////////////////////////////////////////
	void    CalcGenClustorPcoi();
	void    CalcGenOpstPcoiCurv();
	void    UpdateAnaInfToWAMAP();                          // 在线分析信息
public:
	void    DecideLfoCurveID() ;                            //  确定实时曲线对应设备
	void    FormMonModeCurce();                             //  形成模式曲线
	void    UpdateMonInfToWAMAP();                          //  实时监视信息	
	void    FormItfcCurce();                                //  形成断面曲线
	void	UpdateLfoWarnDev( );	//	振荡设备曲线
public:
	THIS_LFO_INFO        m_ThisLfoInfo;                     //  本次低频振荡信息(LFO消失后统计) 
	int                  m_FormResSaveStat;                 //  整合报表状态
	int                  m_WarnSendFlag;                    //  告警发送
	LFO_SYS_RES_SAVE     m_SysResSave ;                     //  系统结果
    int                  m_CallDynFile;                     //  召唤100帧文件标志
	//  FormLfoSaveResult
	void    FormLfoResultProcess( );
    void    FormAsstDeci();                                 // 形成辅助策略
    void    InitLfoResultSaveStruct( ) ;
	void    FormLfoMonSaveResult( ) ;
	void    FormLfoAnaSaveResult( );
	void    SaveResultFileTxt( ) ; 
    void    WriteMonResult( );                            //  山西应急系统接口文件  // 2008-06-24 by zhw
    void    CreateHisFtpFile(vector<string> &fileName);
	void    SendLfoResFile();                             // 福建E语言格式接口文件
    int     CallHDFile();               //  召唤长录波文件  //  2008-08-19
	bool	HDFileOk();					//	判断是否可以开始召唤长录波文件
	bool	HDFileBack();				//	判断召唤的长录波文件是否已经返回
	void	StartSSOAna();				//	启动sso精细化分析
public:
	//  FAT
    int     m_acdot_id;
    int     m_isForceSet;
    short   m_filNo;
	short   m_colNo;
    //bool    GetPmuFilColNobyKeyId( KEY_ID_STRU ikeyid, short &filNo, short &colNo );// 根据KeyId和域号获取PMU数据文件号列号
    void SetPmuFilColNoForTest( int ac_dot_id, int COL_ID, int nLnNum, SSO_LN_PMU * pLnPmu, int GN_COL_ID, int nGnNum, SSO_GN_PMU * pGnPmu,short ifilNo, short icolNo, int isForceSet );
	/////////////////////////////////接口部分/////////////////////////////////
public:
	//  实时库
	 // 根据KeyId和域号获取PMU数据文件号列号
	//int     GetFileColumNoByKeyID( int fileno, int colno, MY_KEY_ID Mykeyid );
   //  所有读表函数返回：(0 正常, -1 取数失败, 1 记录数不等 )
    int     GetFcCtl( int num, SSO_FC_CTL *pFcCtl ) ;  //  LFO厂站表
    int     GetLnCtl( int num, SSO_LN_CTL *pLnCtl ) ;  //  LFO交流线路表
    int     GetGnCtl( int num, SSO_GN_CTL *pGnCtl ) ;  //  LFO发电机表
    int     GetTrCtl( int num, SSO_TR_CTL *pTrCtl ) ;  //  LFO变压器表
    int     GetLfoPara( PARA_LFO_ANA  &LfoPara ) ;  //  LFO参数表（第一条记录）
    int     GetDispCtl( PARA_DISP_CTL &DispCtl ) ;  //  LFO参数表（第一条记录）
    int     GetProcCtl( PARA_PROC_CTL &ProcCtl ) ;  //  LFO参数表（第一条记录）
	void    UpdateMonFcTable( int num, SSO_FC_INF *pfc_inf, int num_gn, SSO_GN_MON *pgn_mon,int num_tr, SSO_TR_MON *ptr_mon) ;   //  LFO厂站表
	void    UpdateMonFcTableStat( int num, SSO_FC_INF *pfc_inf, int num_gn, SSO_GN_MON *pgn_mon,int num_tr, SSO_TR_MON *ptr_mon) ;   //  LFO厂站表
	void    UpdateMonGnTable( int num, SSO_GN_INF *pgn_inf, SSO_GN_MON *pgn_mon ) ;   //  LFO发电机表
	void    UpdateMonGnTableStat( int num, SSO_GN_INF *pgn_inf, SSO_GN_MON *pgn_mon ) ;   //  LFO发电机表状态
	void    UpdateonGnTableStat( int num, SSO_GN_INF *pgn_inf, SSO_GN_MON *pgn_mon ) ;   //  LFO发电机表
	void    UpdateMonLnTable( int num, SSO_LN_INF *pln_inf, SSO_LN_MON *pln_mon ) ;   //  LFO交流线路表状态
	void    UpdateMonLnTableStat( int num, SSO_LN_INF *pln_inf, SSO_LN_MON *pln_mon ) ;   //  LFO交流线路表状态
    void    UpdateMonTrTable( int num, SSO_TR_INF *ptr_inf, SSO_TR_MON *ptr_mon ) ;   //  LFO变压器表
    void    UpdateMonTrTableStat( int num, SSO_TR_INF *ptr_inf, SSO_TR_MON *ptr_mon ) ;   //  LFO变压器表
	void    UpdateMonMdTable( int num, MODE_PART_OBJ *pobj_mode );                    //  实时设备模式全表
	void    UpdateMonSysTable( SSO_SYS_MON sys_mon ) ;                                //  LFO参数表（第一条记录）
	void    UpdateMonSysTableStat( SSO_SYS_MON sys_mon ) ;                                //  LFO参数表（第一条记录）
    int     UpdateAppVariable(SSO_SYS_MON sys_mon) ;                                  // 更新WAMS应用模块告警信息表
	void    UpdateFcInfTableLfoStat(  int num, SSO_FC_MON *pfc_mon  ) ;               //  厂站信息表LFO状态
	void    UpdateGnDevTableLfoStat(  MODE_CHRCT &DispMode) ;                                             //  发电机表LFO状态
	void    UpdateLnDevTableLfoStat( int num, SSO_LN_MON *pln_mon );                  //  交流线段表LFO状态
	void    UpdateLfoFcTable( int num, SSO_FC_INF *pfc_inf, SSO_FC_MON *pfc_lfo ) ;   //  lfo结果
	void    UpdateLfoGnTable( int num, SSO_GN_INF *pgn_inf, LFO_OBJ_RES_SAVE *pgn_lfo ) ;
	void    UpdateLfoLnTable( int num, SSO_LN_INF *pln_inf, LFO_OBJ_RES_SAVE *pln_lfo ) ;
    void    UpdateLfoTrTable( int num, SSO_TR_INF *ptr_inf, LFO_OBJ_RES_SAVE *ptr_lfo ) ;
	void    UpdateLfoMdTable( int num, LFO_DEV_LEAD_MODE *pobj_mode );                //  实时设备主导模式表
	void    UpdateLfoSysTable( ) ;                                //  LFO参数表（第一条记录）
    void    UpdateLfoHisTime( LFO_HIS_TIME his_time );
	// ana
	//void    UpdateAnaFcTable( int num, SSO_FC_INF *fc_inf, SSO_FC_MON *fc_stat ) ;  //  ana结果
	//void    UpdateAnaGnTable( int num, SSO_GN_INF *gn_inf, LFO_GN_LFO *gn_stat ) ;
	//void    UpdateAnaLnTable( int num, SSO_LN_INF *ln_inf, LFO_LN_LFO *ln_stat ) ;
	void    UpdateAnaSysTable( LFO_SYS_ANA sys_ana ) ;                                //  LFO参数表（第一条记录）
	void    UpdateAnaCntrTable( int num, SSO_FC_INF *pfc_inf, SSO_FC_MON *pcntr_fc ); //  LFO厂站表（部分厂站）
	void    UpdateAnaShapTable( int num, MODE_CHRCT *pModeShap ) ;                    //  LFO实时模态表
    void    UpdateGnFact(int num, MODE_CHRCT *pModeShap );                                     //  LFO发电机表参与因子 2008-04-03 added by zhw at yn
    void    UpdateLnFact(int num, MODE_CHRCT *pModeShap );                                     //  LFO线路参与因子 2008-05-21 added by zhw at nj
    void    UpdateAverFact( int num, MODE_CHRCT *pModeShap );                                  //  参与因子平均值入库 2008-06-05 by zhw
	void    UpdateCasWndsTable( int num, LFO_CALC_WDS CalcWds, int is_clear = 0) ;    //  LFO实时case窗口表
	void    UpdateCasAllShapeTable( int num, MODE_CHRCT *pModeShap ) ;                //  LFO实时case模态全表
	//  画面曲线显示
	void    UpdateLfoCurve( TAB_LFO_RT_CURVE RtCurve  ) ;                            //  LFO实时曲线表（记录1、2、3）
	void    UpdateMonModeCurce(  PERIOD_LEAD_MODE * pLfoModeCurve ) ;                //  LFO实时曲线表（记录4、5、6）
	//void    UpdateItfcCurce(    int num, LFO_INTERFACE *pItfcCurve ) ;               //  LFO断面定义表

	//  详细结果浏览画面展示告警设备的振荡曲线20141008
	void	UpdateWarnDevCurve( TAB_WARN_DEV_CURVE WarnCurve);
	void    RealtimeCurveProcess();
	//  缓冲区
	//bool    GetLastTimeFromWAMAP(time_t *recordtime);                                //  获取PMU数据表中最后一次刷新时间
	bool    GetFcDataFromWAMAP( LFO_FC_PMU iFcPmu, time_t tBgnSec, int nVecSec, LFO_FC_DAT &oFcDat, int nOffSec );       //获取厂站PMU量测数据
	bool    GetGnDataFromWAMAP( SSO_GN_PMU iGnPmu, time_t tBgnSec, int nVecSec, SSO_GN_DAT &oGnDat, int nOffSec );       //获取机组PMU量测数据
	bool    GetLnDataFromWAMAP( SSO_LN_PMU iLnPmu, time_t tBgnSec, int nVecSec, SSO_LN_DAT &oLnDat, int nOffSec );       //获取线路PMU量测数据
    bool    GetTrDataFromWAMAP( SSO_TR_PMU iTrPmu, time_t tBgnSec, int nVecSec, SSO_TR_DAT &oTrDat, int nOffSec );       //获取线路PMU量测数据
    bool    IWM_PmuDataCheck( PUB_WT_VEC_DATA *iPmudata, time_t tBgnSec, float * oVecData, VEC_DATA_STS &vecSts);
	//  消息
	//int     IsOnDuty( int app_id, short ctx_no = AC_REALTIME_NO );
	//int     GetAppHostName( int app_id, char * host_name, short ctx_no = AC_REALTIME_NO );
    //  获取辅助策略
    bool    GetAsstDeci(); // 2008-09-09 by zhw
    int     LFOSendWarnMessage(const SSO_SYS_MON status_info, int warn_time, INT_REC factory_id) ;
	int     LFOTriggerSample();
	int     SampleSsoPara();
	int     SampleHisDev();
	int     SampleHisCase();
	int     SampleHisShape();
	bool    SaveLfoCaseByTmd(  );        //发送保存 LFO case信息，启动TMD记录PMU数据
    int     LFOTriEmsSingle( SSO_SYS_MON sys_mon );         // 向EMS转发LFO信息  2008-05-31 by zhw at fj
    int     CallDynFile();               //  召唤100帧文件  //  2008-08-19
    int     IsDynFileExist( int fac_id, int bgn_t, int end_t );
    int     IsTrFileExist( int fac_id, int bgn_t, int end_t );
    int     FormDynFile(int fac_id,int beg_sec,int end_sec,vector<string> &dynVec);
    void    GetPmuType(string idcode,short &pmuType);
    void    GetDynTimeStr(int beg_sec,int end_sec,vector<string> &dynTimeVec);
    int     CallTrFile();
    bool    SendHisLfoMess();      //  给离线程序发消息，包含最近一次振荡时间
    int     UpdateLfoEvent();  // 更新LFO振荡事件表
    int     UpdateHisCase();  // 更新LFO实时CASE窗口表
    int     UpdateHisShape();  // LFO实时CASE模态全表
    int     SaveCaseFile();
	int     ReadNewData(const string tmpfilename, int &realNum, float &t_delta, int &nDataTime);
	int     SendWarnToIA(const int mode);  // 向综合智能告警(科东)发送消息
	/////////////////////////////////数据处理/////////////////////////////////
public:
    bool    IsFcStsValid(const VEC_DATA_STS &vecSts)
    {
        return ( vecSts.nBadNum <= MAX_FRA_PER_SEC / 5 );
    }
	
    bool    IsLnStsValid(const VEC_DATA_STS &vecSts)
    {
        return ( vecSts.nBadNum <= MAX_FRA_PER_SEC / 5 );
    }
	
    bool    IsGnStsValid(const VEC_DATA_STS &vecSts)
    {
        return ( vecSts.nBadNum <= MAX_FRA_PER_SEC / 5 );
    }
	
    bool    IsTrStsValid(const VEC_DATA_STS &vecSts)
    {
        return ( vecSts.nBadNum <= MAX_FRA_PER_SEC / 5 );
    }

    int     IsFcDatValid( int no );
    int     IsLnDatValid( int no, int ChgTimCoe, float ChgMinVal );
    int     IsGnDatValid( int no, int ChgTimCoe, float ChgMinVal );
	int     IsAglDatValid( int nAgl, float * pAgl, float MaxChg );
    int     IsTrDatValid( int no, int ChgTimCoe, float ChgMinVal );
	
	//  判断相邻数据点是否有突变。无突变返回-1, 方法：1)相邻两点绝对差 > (前十点绝对差之和)；2) 相邻两点绝对差 > 给定的允许值
    float   JudgeTwoValDelta(float *data, int num, float max_change_lim, float delta_lim_val, int algorithm);
	int     CurveLfoPreEstimate( SEQ_BASE_INFO dataBaseInfo, float ampl_sec_lim, int check_aver, float aver_lim  );
	//////////////////////////////// 主站缓冲数据分析 /////////////////////////////////
public:
	int                  Date7_RunStatus;     //  运行状态 0等待  1分析 
	int                  m_RunCtrCom;         //  离线分析运行方式控制
    char                 m_szLfoMsg[256];     //  运行信息
	int                  Date7_beginTime ;    //  起始时刻
	int                  Date7_timeLeng ;     //  要求分析时间长度
	int                  Date7_acln_id;       //  要求分析的单条线路
    int  Date7GetTimePara( int &begintime, int &endtime, int &MemMaxDate);
    int  Date7TimeParaCheck( int begintime, int endtime, int MemMaxDate);
    bool Date7LfoMsgCtxToWAMAP(char * Msg_Ctx, int if_firt = 0);
    int  Data7HandlingPmuSampleData();
    void Data7MainProcess( );	
	void Date7RealtimeCurveProcess( time_t endtime );//刷新实测曲线
	/////////////////////////////////测试输出/////////////////////////////////
public:
	//  ana
// 	void    WriteGnLeadClusToFile(  );
// 	void    WriteModeShapeToFile( );
//     void    WriteCurveBaseInfoToFile( );
    //void    WriteAllObjModeToFile( const int inObjMode, MODE_PART_OBJ *ipObjMode );
	void    WriteModeShapeToFile( const int inMode, MODE_CHRCT *ipMode, unsigned char isShowFac = 1  );
	void	WriteLfoCenterToFile( LFO_SYS_ANA SysAna, int nFcProInf, LFO_FC_ANA *pFcProInf);
private:
#ifdef _LFO_ASST_DECI_
    PARA_LFO_ASST        m_LfoAsstPara;                     //  辅助策略参数
    LFO_ASST_PREFIX      m_LfoAsstfix;                      //  辅助决策前后缀
    LFO_KEY_DEV          m_AsstDeciStat;
	LFO_KEY_DEV          m_AsstDeciStatold;                 // 2008-09-26
    int                  m_nAsstDeciInf;                    //  LFO辅助策略个数
    LFO_ASST_DECI *      m_pAsstDeciInf;                    //  LFO辅助策略信息
    int                  m_nAsstDeciInfOld;                 //  LFO辅助策略个数
    LFO_ASST_DECI *      m_pAsstDeciInfOld;                 //  LFO辅助策略历史信息
    int             m_nCtlDevNum;
    LFO_CTL_DEV*    m_pCtlDevId;
    int             m_nCtlDevNumOld;
    LFO_CTL_DEV*    m_pCtlDevIdOld;
    int                  m_nDeciIndxInf;                    //  LFO策略索引个数
    LFO_DECI_INDEX *     m_pDeciIndxInf;                    //  LFO策略索引信息
    
    int                  m_nCtlDeciInf;                     //  LFO策略个数
    LFO_CTL_DECI   *     m_pCtlDeciInf;                     //  LFO策略信息
    
    int                  m_nDeciActInf;                     //  LFO控制对象个数
    LFO_CTL_ACT *        m_pDeciActInf;                     //  LFO控制对象信息

    int  InitAsstDeci();
    int  ReleaseAsst();
    int  AllocAsstMem();
    int  ReadAsstIni();
    int  EstimatLfoDeciRank();
    int  SechLfoDeciKeyDev();
    int  LfoDeciFromAnaRelt();
    int  StringCat(char* strSrc, const char* strAdd, int stringlen);
    void MemCpyDeciInfStru( LFO_ASST_DECI iDeciInf, LFO_ASST_DECI &oDeciInf );
    int  IsNewAsstDeciInfo( int nDeciInf, LFO_ASST_DECI *pDeciInf, LFO_ASST_DECI tempDeciInf );

    void UpdateAsstDeciTable( int DeciNum, LFO_ASST_DECI *DeciInf, int is_clear = 0 );
    void UpdateDevTable( int nDevNum, LFO_CTL_DEV * pDevId );
    int  LfoDeciTriggerSample( int DeciNum, LFO_ASST_DECI *DeciInf );
    void WriteAsstDeciToFile();
#endif
};

#endif

