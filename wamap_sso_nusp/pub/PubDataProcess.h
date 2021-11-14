/******************************************************************************
  ModuleName :   Public Data Process
  FileName   :   PubDataProcess.h
  Description:   Data Process definition
  HISTORY :    	
      Date        Version       Author                Activities      
   ----------------------------------------------------------------------
   03/13/2007       1.0          lsf                   created
******************************************************************************/
#ifndef _PUB_DATA_PROCESS_H_
#define _PUB_DATA_PROCESS_H_
#include <stdio.h>
#include <stdlib.h>                                         //  calloc(), free()
#include <string.h>                                         //  memcpy(), memset()
#include <math.h>

//sequence base information
typedef struct  type_SEQ_BASE_INFO
{
	float             ave_val1;                             //  均值1(未使用)
	float             ave_val;                              //  均值
	float             max_val;                              //  最大值
	int               max_val_pnt;                          //  最大值点号
	float             min_val;                              //  最小值
	int               min_val_pnt;                          //  最大值点号
	float             max_ampl;                             //  最大波动
	float             max_ampl_rat;                         //  最大波动率(波动幅值/均值)
}SEQ_BASE_INFO;

////  Base function
////  Macro define
#define DP_PI                           3.14159265358979324 //  圆周率
//#define DP_RAD                          0.01745329251994330 //  角度转弧度的系数
#define DP_ABS(x)                       ((x)>0?(x):-(x))    //  取绝对值
#define DP_MAX(x,y)                     ((x)>(y)?(x):(y))   //  取最大值
#define DP_MIN(x,y)                     ((x)<(y)?(x):(y))   //  取最小值

#ifndef DP_RAD
#ifdef _D5000
#define DP_RAD                          1.0  // 华东D5000角度均改用弧度制 
#else
#define DP_RAD                          0.01745329251994330 //  角度转弧度的系数
#endif
#endif

#ifndef VF_VOLTBASE
#ifdef _D5000
#define VF_VOLTBASE                     1000.0  //华东D5000电压均改用伏特（V）为单位
#else
#define VF_VOLTBASE                     1.0
#endif
#endif

double  DP_Cos(float fAng);                                 //  计算角度的余弦值 输入：度数
double  DP_Sin(float fAng);                                 //  计算角度的正弦值 输入：度数

////  Calculate sequence base information
float   DP_MidAveVal(const float * iSrcData, int nDataLen);                                                         // 计算中均值
void    DP_CalcDataSeqBaseInfo(const float *iSrcData, int nDataLen, SEQ_BASE_INFO &DataBaseInfo);                   // 计算序列基本信息
int     DP_NumBySample(int Num, short sample);

////  Filter function
void    DP_LimAmpFilter(const float * iSrcData, float * oFltData, int nDataLen, float fMaxErr);                     // 限幅滤波
void    DP_MidValFilter(const float * iSrcData, float * oFltData, int nDataLen, int nFiltLen);                      // 中值滤波
void    DP_MidAveFilter(const float * iSrcData, float * oFltData, int nDataLen, int nFiltLen);                      // 中值平均滤波
void    DP_AveValFilter(const float * iSrcData, float * oFltData, int nDataLen, int nFiltLen);                      // 算术平均滤波
void    DP_CoeAveFilter(const float * iSrcData, float * oFltData, int nDataLen, const float * vfCoe, int nFiltLen); // 加权平均滤波
void    DP_SimLowPasFilter(const float * iSrcData, float * oFltData, int nDataLen, float fCoe);                     // 一阶滞后滤波

////  Butterworth Low-pass Filter 
void    DP_BtwthLowPasFilter(const float * iSrcData, float * oFltData, int nDataLen, float fSmpFreq, int rank=2);     

////  Insert function 
float DP_InsertData(float y[],float x0,float h,int n,float t);

#define DP_NBSAVX      	  6000	                            //  数据最大点数 
#define DP_MAXPOINT		  100                               //  极值点个数
#define DP_NBCHNPOINT     20								//  曲线中突变点的个数
#define DP_NUMCHNPOINT    200								//  曲线中突变大小
#define DP_EPS            0.00001

////  get cntur 
// 数据                    
typedef struct  type_ANGLE_RAW_DATA
{
	float    time[DP_NBSAVX];         //时间轴
	float    data1[DP_NBSAVX];        //原始曲线数据
	float    data2[DP_NBSAVX];        //差值曲线数据
}ANGLE_RAW_DATA;

// 数据                    
typedef struct  type_ANGLE_HDL_DATA
{
	float    time[DP_NBSAVX];         //时间轴
	float    data[DP_NBSAVX];         //插值后曲线数据
}ANGLE_HDL_DATA;

int     DP_RawAngleHdlbyCntur( int nRawPnt, ANGLE_RAW_DATA *pRawData, int iMaxFramePerSec, float fFreq, int &nMaxPnt, ANGLE_HDL_DATA *pMaxPnt, int &nMinPnt, ANGLE_HDL_DATA *pMinPnt );
int     DP_CalcAngleDiff( int nPnt, float * pAgl1, float * pAgl2, float * pAgl );
int     DP_JumpPointHdl(int nPnt, float *pOutAngl, float *pInAngl, float MaxAnglDiff);  //  解决曲线跳变  // 2008-08-14 by zhw
#endif

