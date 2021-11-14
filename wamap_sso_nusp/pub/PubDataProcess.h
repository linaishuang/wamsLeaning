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
	float             ave_val1;                             //  ��ֵ1(δʹ��)
	float             ave_val;                              //  ��ֵ
	float             max_val;                              //  ���ֵ
	int               max_val_pnt;                          //  ���ֵ���
	float             min_val;                              //  ��Сֵ
	int               min_val_pnt;                          //  ���ֵ���
	float             max_ampl;                             //  ��󲨶�
	float             max_ampl_rat;                         //  ��󲨶���(������ֵ/��ֵ)
}SEQ_BASE_INFO;

////  Base function
////  Macro define
#define DP_PI                           3.14159265358979324 //  Բ����
//#define DP_RAD                          0.01745329251994330 //  �Ƕ�ת���ȵ�ϵ��
#define DP_ABS(x)                       ((x)>0?(x):-(x))    //  ȡ����ֵ
#define DP_MAX(x,y)                     ((x)>(y)?(x):(y))   //  ȡ���ֵ
#define DP_MIN(x,y)                     ((x)<(y)?(x):(y))   //  ȡ��Сֵ

#ifndef DP_RAD
#ifdef _D5000
#define DP_RAD                          1.0  // ����D5000�ǶȾ����û����� 
#else
#define DP_RAD                          0.01745329251994330 //  �Ƕ�ת���ȵ�ϵ��
#endif
#endif

#ifndef VF_VOLTBASE
#ifdef _D5000
#define VF_VOLTBASE                     1000.0  //����D5000��ѹ�����÷��أ�V��Ϊ��λ
#else
#define VF_VOLTBASE                     1.0
#endif
#endif

double  DP_Cos(float fAng);                                 //  ����Ƕȵ�����ֵ ���룺����
double  DP_Sin(float fAng);                                 //  ����Ƕȵ�����ֵ ���룺����

////  Calculate sequence base information
float   DP_MidAveVal(const float * iSrcData, int nDataLen);                                                         // �����о�ֵ
void    DP_CalcDataSeqBaseInfo(const float *iSrcData, int nDataLen, SEQ_BASE_INFO &DataBaseInfo);                   // �������л�����Ϣ
int     DP_NumBySample(int Num, short sample);

////  Filter function
void    DP_LimAmpFilter(const float * iSrcData, float * oFltData, int nDataLen, float fMaxErr);                     // �޷��˲�
void    DP_MidValFilter(const float * iSrcData, float * oFltData, int nDataLen, int nFiltLen);                      // ��ֵ�˲�
void    DP_MidAveFilter(const float * iSrcData, float * oFltData, int nDataLen, int nFiltLen);                      // ��ֵƽ���˲�
void    DP_AveValFilter(const float * iSrcData, float * oFltData, int nDataLen, int nFiltLen);                      // ����ƽ���˲�
void    DP_CoeAveFilter(const float * iSrcData, float * oFltData, int nDataLen, const float * vfCoe, int nFiltLen); // ��Ȩƽ���˲�
void    DP_SimLowPasFilter(const float * iSrcData, float * oFltData, int nDataLen, float fCoe);                     // һ���ͺ��˲�

////  Butterworth Low-pass Filter 
void    DP_BtwthLowPasFilter(const float * iSrcData, float * oFltData, int nDataLen, float fSmpFreq, int rank=2);     

////  Insert function 
float DP_InsertData(float y[],float x0,float h,int n,float t);

#define DP_NBSAVX      	  6000	                            //  ���������� 
#define DP_MAXPOINT		  100                               //  ��ֵ�����
#define DP_NBCHNPOINT     20								//  ������ͻ���ĸ���
#define DP_NUMCHNPOINT    200								//  ������ͻ���С
#define DP_EPS            0.00001

////  get cntur 
// ����                    
typedef struct  type_ANGLE_RAW_DATA
{
	float    time[DP_NBSAVX];         //ʱ����
	float    data1[DP_NBSAVX];        //ԭʼ��������
	float    data2[DP_NBSAVX];        //��ֵ��������
}ANGLE_RAW_DATA;

// ����                    
typedef struct  type_ANGLE_HDL_DATA
{
	float    time[DP_NBSAVX];         //ʱ����
	float    data[DP_NBSAVX];         //��ֵ����������
}ANGLE_HDL_DATA;

int     DP_RawAngleHdlbyCntur( int nRawPnt, ANGLE_RAW_DATA *pRawData, int iMaxFramePerSec, float fFreq, int &nMaxPnt, ANGLE_HDL_DATA *pMaxPnt, int &nMinPnt, ANGLE_HDL_DATA *pMinPnt );
int     DP_CalcAngleDiff( int nPnt, float * pAgl1, float * pAgl2, float * pAgl );
int     DP_JumpPointHdl(int nPnt, float *pOutAngl, float *pInAngl, float MaxAnglDiff);  //  �����������  // 2008-08-14 by zhw
#endif

