/******************************************************************************
	ModuleName :   Prony Analysis 
	FileName   :   PronyAnalysis.h
	Description:   Directly estimating the frequency, damping, strength, and 
	               phase of modal components present in a given signal. 
	HISTORY    :
	   Date        Action        Author                Description 
	----------------------------------------------------------------------
	2004.06.08     Creation        lsf         动态开内存无限制
	2004.07.06     Modify          lsf         高次多项式的根求取采用解其伴随矩阵特征根的方法 
	2004.08.06     Modify          lsf         修改超定方程组求解方法为SVD分解法 
    2007.07.18     Modify          lsf         增加由模式特征量拟合曲线// 增加svd计算阶数
	2012/04/19     v1.0.0         zhw          添加版本号,增加日志输出
	2012/04/23     v1.0.1         zhw          修改参数结构体,增加能量过滤
	2012/05/07     v1.0.2         zhw          增加最大点数越限判断
	2012/09/05     v1.0.3         zhw          增加信噪比门槛值
	2012/10/30     v1.0.4         zhw          根据输入数据进行计算，不再增加数组
	1)	计算原始波形能量时去掉直流分量。
	2)	若模式能量值大于原始波形能量的2倍，则变换阶数重新计算。
	若达到最大阶数变换次数后仍不满足能量判据，则给出计算异常标志，置模式数为-3。
******************************************************************************/
#ifndef _PRONY_ANALYSIS_H_
#define _PRONY_ANALYSIS_H_
#ifdef _WINDOWS32
#include <winsock2.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#ifdef _WINDOWS32
	#include <windows.h>                     //  Sleep()    
#else
	#include <unistd.h>                     //  sleep() usleep()
#endif

// calculation control parameter
typedef struct type_PRONY_CALC_PARA             
{
	int      if_set_rank;       //阶数确定  ( =1 人为设定; !=1 自动设定)
	int      rank;              //设定的阶数
  int      queue_type;        //排序      ( =0 阻尼比排序; =-1 幅值排序; =-2 能量排序)
  int      delDC;             //去直流    ( =0 否;       =1  是) 
	float    freq_min;          //频率下限
	float    freq_max;          //频率上限
	float    minAbsAmpl;        //保存振荡模式幅值最小限值(绝对值)
  float    stor_lim_ampl;     //保存振荡模式幅值限值(与最大幅值的比值)
	float    stor_lim_damp;     //保存振荡模式阻尼比限值(-100~100)
	float    snr_min;           //最小信噪比限值
	int      show_log;          //日志级别;1,输出调试信息;0,不输出调试信息
	float    eng_time;          //最大能量倍数;>1.0
	int      max_iter;          //最大迭代次数;>1
	int		 is_filter;			//是否采用一阶惯性滤波
}PRONY_CALC_PARA;

// calculation control parameter
typedef struct type_PRONY_MODE_CHARA             
{
	double   damp;              //阻尼比
	double   freq;              //频率
  double   ampl;              //幅值
  double   phas;              //初相(弧度)
	double   real;              //实部
	double   imag;              //虚部
	double   zphs;              //z相角
  double   zamp;              //z幅值
	double   brel;              //b实部
	double   bimg;              //b虚部
	double   engy;              //能量
}PRONY_MODE_CHARA;

////  Macro define
//#define  PRONY_DATA_MAX                   1050                // max point
#define  PRONY_PI                       3.14159265358979    // pi
#define  PRONY_EPS                      0.0000001           // zero //0.000000001
#define  PRONY_EPS_Freq                 0.0001              // min frequency
#define  PRONY_MAX_MODE                 50                 // max rank
#define  PRONY_DATA_MAX                 2000                 // max data num
#define MODE_NUM_MAX        PRONY_MAX_MODE
#ifndef  RADBASE
#define  RADBASE 0.017453292  // pi/180
#endif
//#define _ProMode_                                           // debug switch (for output mode result)

class CPronyAnalysis
{
public:
    CPronyAnalysis();
    ~CPronyAnalysis();
public:
	//     注：输入曲线必须等间隔；最大输出模式个数100；输出角度单位弧度；
	//         返回modenum：>0 正常; =0 无符合要求模式; <0 无可行解;
    void   PronyAnalysis(PRONY_CALC_PARA calcpara, int oldnum, double dataxp[], double datatp[], int &oModeNum, PRONY_MODE_CHARA *oMmode, double &snr,int &actrank);

    //     mode[][7]：   模式特征量(阻尼比、频率、幅值、初相、实部、虚部、engy)
    void   PronyAnalysis7(PRONY_CALC_PARA calcpara, int oldnum, double data_x[], double data_t[], int &modenum, double mode[][7], double &snr,int &actrank);

    //     mode[][8]：   模式特征量(阻尼比、频率、幅值、初相、实部、虚部、z相角和z幅值)
    void   PronyAnalysis8(PRONY_CALC_PARA calcpara, int oldnum, double data_x[], double data_t[], int &modenum, double mode[][8], double &snr,int &actrank);

    //     mode[][11]：  模式特征量(阻尼比、频率、幅值、初相、实部、虚部、z相角、z幅值、bReal、bImag、engy)
    void   PronyAnalysis11(PRONY_CALC_PARA calcpara, int oldnum, double data_x[], double data_t[], int &modenum, double mode[][11], double &snr,int &actrank);

	void   CalcSinglModeCurve( int nDataLen, float * oModeCurve, PRONY_MODE_CHARA iMode );
	void   CalcMultiModeCurve( int nDataLen, float * oModeCurve, PRONY_MODE_CHARA *iMode, int nModeNum );

	virtual void MessShow(const char *log);
private:
	char m_LogStr[512];
	double prony_fabs(double x);
    float  prony_fabs(float x);
	int    prony_sign(double x);
	int    prony_sign(float x);
	void   DelayMinSec( int msec );
	int    CheckPara(PRONY_CALC_PARA &calcpara);
private:
    int    DataProcess(int oldnum,double x[],double t[],double fmax,double xp[],double tp[],double *dt);
	double InsertData(double y[],double x0,double h,int n,double t);
	int    RealEqution(double aa[],double ab[],int m,int n,double ax[]);
	void   Lucomp(double a[],int n,int indx[],double d);
    void   Luback(double a[],int n,int indx[],double b[]);
	bool   Polynomial(double a[], int m, double rtr[], double rti[]);
	void   ReduceMatrix(double a[], int n);
	bool   MatrixEigenvalue(double a[], int n, double wr[], double wi[]);
	int    FormMatrix(double pa[],double zzr[],double zzi[],double px[],int m,int n,double xr[],double xi[]);
	int    EqutionSvd(double a[],double b[],int m,int n,double x[]);
	bool   Svdcomp(double a[],int m,int n,double w[],double v[]);
	double Calculate(double a, double b);
	void   Svdback(double u[], double w[], double v[], int m, int n, double b[], double x[]);
};
#endif
