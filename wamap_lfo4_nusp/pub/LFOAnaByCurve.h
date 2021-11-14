/******************************************************************************
	ModuleName :   LFO Analysis based on curve 
	FileName   :   LFOAnaByCurve.h
	Description:   
	HISTORY    :
	   Date        Action        Author                Description 
	----------------------------------------------------------------------
	2007.07.08     Creation        lsf           
******************************************************************************/
#ifndef _LFO_ANA_BY_CURVE_H_
#define _LFO_ANA_BY_CURVE_H_
#ifdef _KEDONG
#include "db_api/odb_net_m.h"
#else
#include "db_api/odb_struct.h"
#endif // _KEDONG
#include "PronyAnalysis.h"                     //  PRONY_PI    
#include <time.h>

//////////////////////////////////////////////////////////////////////////
// 2007-7-16 LFO在线分析增加结构
////  Macro define
#define  MODE_MAXGN       500//300           // 模式参与机组最大数
#define  MODE_MAXLN       1000//200           // 模式可观线路最大数
#define  MODE_MAXFC       100//200           // 模式可观线路最大数 // 2007-11-25
#define  MAXLFOCENT       20                 // 振荡中心厂站最大数 // 2008-08-28

#define  OBJ_MODE_MAX    3              // 曲线候选模式最大数
#define  SYS_MODE_MAX    10             // 系统候选模式最大数

#define  IDTF_TYPE_FREQ   0.7           // 
#define  IDTF_TYPE_LOCL   0             // 局部
#define  IDTF_TYPE_INTR   1             // 区域

#define	 LFO_CLUST_RMAIN  0             // 余下群 // 2008-09-24
#define	 LFO_CLUST_CRTCL  1             // 临界群 // 2008-09-24
#define	 LFO_CLUST_OPOST  2             // 相对群 // 2008-09-24

#define  OBJ_TYPE_GN      0             // 机组
#define  OBJ_TYPE_LN      1             // 线路
#define  OBJ_TYPE_FC      2             // 厂站 // 2007-11-25
#define  OBJ_TYPE_TR      3             // 变压器 // 2008-09-24

//======== 数据类型 ======//
#ifndef INT_REC_FMT
#ifdef _D5000
	#ifdef _NUSP
		typedef     RTDB_KEY_STRU					INT_REC;
		#define     INT_REC_FMT						"%s"
		#define		INT_REC_VAL(id)					(id).FormStringID().c_str()
		#define		VOL_INT_REC						int
	#else
		typedef     long							INT_REC;
		#define		VOL_INT_REC						long
		#define     INT_REC_FMT "%ld"
		#define		INT_REC_VAL(id)					(id)
	#endif // _NUSP
#else
	typedef     int									INT_REC;
	#define     VOL_INT_REC                         int
	#define     INT_REC_FMT							"%d"
	#define		INT_REC_VAL(id)						(id)
#endif // _D5000
#endif // INT_REC_FMT

//  mode participation object
typedef struct  lfo_mode_part_obj
{
	INT_REC           obj_id;                               // 设备ID
	char              obj_name[64];                         // 设备名称
	int               type;                                 // 类型(OBJ_TYPE_GN; OBJ_TYPE_LN)
	float             damp;                                 // 阻尼比
	float             freq;                                 // 频率
	float             ampl;                                 // 振幅
	float             phas;                                 // 初相
	float             mgnt;                                 // 量级
	float             real;                                 // 实部  // 
	float             imag;                                 // 虚部
	float             fact;                                 // 参与因子
	float             engy;                                 // 能量
    unsigned char     clus;                                 // 所属群 //2008-09-24 add
	float			  pow;                                  // 耗散功率
	int				  is_source;							// 是否扰动源 1是,0否
}MODE_PART_OBJ;
// 振荡中心厂站
typedef struct tag_LFO_CNTR_FC
{
    INT_REC         fac_id;                                 //  厂站ID
    char            fac_name[64];                           //  厂站名称
    INT_REC         area_id;                                //  区域ID
    unsigned char   fac_type;                               //  厂站类型
	VOL_INT_REC	    vlty_id;                                //  最高电压等级
    float           v_aver;                                 //  电压均值
    float           v_max;                                  //  电压最大值
    float           v_min;                                  //  电压最小值
    float           v_ampl;                                 //  电压摆动
    float           f_aver;                                 //  频率均值
    float           f_max;                                  //  频率最大值
    float           f_min;                                  //  频率最小值
    float           f_ampl;                                 //  频率摆动
    unsigned char   is_cntr;                                //  是否中心
    unsigned char   is_disp;                                //  是否显示
    unsigned char   clus;                                   //  所属群
}LFO_CNTR_FC;

// 振荡中心线路
typedef struct tag_LFO_CNTR_LN
{
    INT_REC         acln_id;                                //  线路ID号
    char            acln_name[64];	                        //  线路名称
    INT_REC         fac_id_s;	                            //  一端厂站ID号
    INT_REC         fac_id_e;	                            //  二端厂站ID号
    unsigned char   is_cntr;                                //  是否中心
    unsigned char   is_disp;                                //  是否显示
} LFO_CNTR_LN;
//  mode characteristics
typedef struct  lfo_mode_chrct
{
#ifdef _NUSP
	int					mode_id;                            //模式ID
#else
	INT_REC           mode_id;                              //模式ID
#endif // _NUSP
	int               type;                                 //类型(=0 local; =1 interarea)

    time_t            time_bgn;                             // 起始时刻  2008-06-04 by zhw at nj
    time_t            time_end;                             // 截止时刻

	float             real;                                 //实部(aver)
	float             imag;                                 //虚部(aver)
	float             freq;                                 //频率(aver)
	float             damp;                                 //阻尼比(aver)

	float             engyGn;                               //机组能量(aver) // 2007-08-10 lsf add
	float             engyLn;                               //线路能量(aver) // 2007-08-10 lsf add
	
	int               nPartGn;                              //参与机组数
	MODE_PART_OBJ     PartGn[MODE_MAXGN];                   //机组模态
	int               nPartLn;                              //可观线路数
	MODE_PART_OBJ     PartLn[MODE_MAXLN];                   //线路模态
	int               nPartTr;                              //可观变压器数 // 2008-09-24
	MODE_PART_OBJ     PartTr[MODE_MAXLN];                   //变压器模态

	int               nPartFc;                              //可观fac
	MODE_PART_OBJ     PartFc[MODE_MAXFC];                   //fac模态

    int               nLfoCenter;                           //振荡中心厂站数
    LFO_CNTR_FC       LfoCenter[MAXLFOCENT];
	
}MODE_CHRCT;

//////////////////////////////////////////////////////////////////////////
// 需要的参数：
// SmModeFreqDif:  0.025
// SmModeDampDif:  
// CohcyAnglDif:   30*PRONY_PI/180 or 30

class CLFOAnaByCurve
{
public:
    CLFOAnaByCurve();
    ~CLFOAnaByCurve();

public:
	//////////////////////////////////////////////////////////////////////////
	//  识别主要模式
	void IdtfMainMode( const int inMode, MODE_PART_OBJ *ipMode, int &onMode, MODE_PART_OBJ *opMode );
	void IdtfMainMode( const int inMode, MODE_PART_OBJ *ipMode, int &onMode, MODE_PART_OBJ *opMode, float FreqDif, float DampDif=100 );
	
	//  相同模式判断（依据模式特征量(Real, Imag)/(freq, damp)）
	bool IdtfSameMode( float iFreq1, float iDamp1, float iFreq2, float iDamp2, float FreqDif, float DampDif=100 );
	//  相同模式判断（依据参与机组，模式特征量作为辅助判据(Real, Imag)/(freq, damp)）
	//bool IdtfSameMode( float iFreq1, float iDamp1, float iFreq2, float iDamp2, float FreqDif, float DampDif=100 );
	
	//  识别参与机组可观线路
	void IdtfPartGnPartLn( int nObjMode, MODE_PART_OBJ *pObjMode, int &nMode, MODE_CHRCT *pMode, float FreqDif, float DampDif=100 );
	void IdtfPartGnLnTr(int nObjMode,MODE_PART_OBJ *pObjMode,int &nMode, MODE_CHRCT *pMode);
	//  计算engyGn和engyLn，排序
	void CalcModeGnLnEngy( int nMode, MODE_CHRCT *pMode );		

	//  计算参与因子
	bool CalcPartFactor( int iObjNum, MODE_PART_OBJ *ModeShape );
	bool CalcPartFactor( int iObjNum, MODE_PART_OBJ *ModeShape, int isTj );
	bool JudgPartFactorSign( int iObjNum, MODE_PART_OBJ *ModeShape, unsigned char isSyn = 1 ); // isSyn = 1 同步振荡
	bool ReSetFactorSign( int iObjNum, MODE_PART_OBJ *ModeShape );
	
	//  判断是否同调
	int  JudgCohcyObjByMode( float iRefAngl, float iDifValu, float iAnglVar, unsigned char iIsRad=0 );// iIsRad = 1 弧度

	//  搜索参与因子最大设备
	bool SearchMaxFact( int nPartObj, MODE_PART_OBJ *iPartObj, float &oFact, float &oPhas, int &oIndex );
	bool SearchMaxFactOpst( int nPartObj, MODE_PART_OBJ *iPartObj, float iRefAngl, float iDifValu, float &oFact, float &oPhas, int &oIndex, unsigned char iIsRad=0 );// iIsRad = 1 弧度
	
	//  振荡源机组识别
	void IdtfCausLfoGn( int &nCausGn, MODE_PART_OBJ *oCausGn, float iDifValu, unsigned char iIsRad=0 );
	void IdtfLfoGnCohcyFc( int &nCausFc, MODE_PART_OBJ *oCausFc, MODE_PART_OBJ *CausGn, float iDifValu, unsigned char iIsRad=0 );
	void IdtfLfoGnCohcyFc( int &nCausFc, MODE_PART_OBJ *oCausFc, float iRefAngl, float iOpstAngl, float iDifValu, unsigned char iIsRad=0 );
	
	//  振荡中心判断
	//int  IdtfAmplMaxObj(  int isSyn = 1);// isSyn = 1 同步振荡
	int  IdtfLfoCenter(  MODE_CHRCT *pMode, int isSyn = 1);// isSyn = 1 同步振荡

	//  辅助控制措施搜索（依据参与因子和振荡中心，离线策略规则，给出辅助控制措施）
	int  LfoCtrlMesrSearch(  );
	
	//  memcpy
	void MemCpyModePartObjStru( MODE_PART_OBJ ipMode, MODE_PART_OBJ &opMode );
	void  LfoAnaSetPartFact0( int nPartGn, MODE_PART_OBJ *pPartGn );
	//////////////////////////////////////////////////////////////////////////
	void	filteDevByPartfact( int nMode, MODE_CHRCT *pMode, int &nSMode, MODE_CHRCT *pSMode, int sh_fact);

	void MessShow(const char *log);
protected:
	//////////////////////////////////////////////////////////////////////////
	
};




#endif
