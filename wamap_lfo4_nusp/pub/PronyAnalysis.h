/******************************************************************************
	ModuleName :   Prony Analysis 
	FileName   :   PronyAnalysis.h
	Description:   Directly estimating the frequency, damping, strength, and 
	               phase of modal components present in a given signal. 
	HISTORY    :
	   Date        Action        Author                Description 
	----------------------------------------------------------------------
	2004.06.08     Creation        lsf         ��̬���ڴ�������
	2004.07.06     Modify          lsf         �ߴζ���ʽ�ĸ���ȡ���ý����������������ķ��� 
	2004.08.06     Modify          lsf         �޸ĳ�����������ⷽ��ΪSVD�ֽⷨ 
    2007.07.18     Modify          lsf         ������ģʽ�������������// ����svd�������
	2012/04/19     v1.0.0         zhw          ��Ӱ汾��,������־���
	2012/04/23     v1.0.1         zhw          �޸Ĳ����ṹ��,������������
	2012/05/07     v1.0.2         zhw          ����������Խ���ж�
	2012/09/05     v1.0.3         zhw          ����������ż�ֵ
	2012/10/30     v1.0.4         zhw          �����������ݽ��м��㣬������������
	1)	����ԭʼ��������ʱȥ��ֱ��������
	2)	��ģʽ����ֵ����ԭʼ����������2������任�������¼��㡣
	���ﵽ�������任�������Բ����������оݣ�����������쳣��־����ģʽ��Ϊ-3��
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
	int      if_set_rank;       //����ȷ��  ( =1 ��Ϊ�趨; !=1 �Զ��趨)
	int      rank;              //�趨�Ľ���
  int      queue_type;        //����      ( =0 ���������; =-1 ��ֵ����; =-2 ��������)
  int      delDC;             //ȥֱ��    ( =0 ��;       =1  ��) 
	float    freq_min;          //Ƶ������
	float    freq_max;          //Ƶ������
	float    minAbsAmpl;        //������ģʽ��ֵ��С��ֵ(����ֵ)
  float    stor_lim_ampl;     //������ģʽ��ֵ��ֵ(������ֵ�ı�ֵ)
	float    stor_lim_damp;     //������ģʽ�������ֵ(-100~100)
	float    snr_min;           //��С�������ֵ
	int      show_log;          //��־����;1,���������Ϣ;0,�����������Ϣ
	float    eng_time;          //�����������;>1.0
	int      max_iter;          //����������;>1
	int		 is_filter;			//�Ƿ����һ�׹����˲�
}PRONY_CALC_PARA;

// calculation control parameter
typedef struct type_PRONY_MODE_CHARA             
{
	double   damp;              //�����
	double   freq;              //Ƶ��
  double   ampl;              //��ֵ
  double   phas;              //����(����)
	double   real;              //ʵ��
	double   imag;              //�鲿
	double   zphs;              //z���
  double   zamp;              //z��ֵ
	double   brel;              //bʵ��
	double   bimg;              //b�鲿
	double   engy;              //����
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
	//     ע���������߱���ȼ����������ģʽ����100������Ƕȵ�λ���ȣ�
	//         ����modenum��>0 ����; =0 �޷���Ҫ��ģʽ; <0 �޿��н�;
    void   PronyAnalysis(PRONY_CALC_PARA calcpara, int oldnum, double dataxp[], double datatp[], int &oModeNum, PRONY_MODE_CHARA *oMmode, double &snr,int &actrank);

    //     mode[][7]��   ģʽ������(����ȡ�Ƶ�ʡ���ֵ�����ࡢʵ�����鲿��engy)
    void   PronyAnalysis7(PRONY_CALC_PARA calcpara, int oldnum, double data_x[], double data_t[], int &modenum, double mode[][7], double &snr,int &actrank);

    //     mode[][8]��   ģʽ������(����ȡ�Ƶ�ʡ���ֵ�����ࡢʵ�����鲿��z��Ǻ�z��ֵ)
    void   PronyAnalysis8(PRONY_CALC_PARA calcpara, int oldnum, double data_x[], double data_t[], int &modenum, double mode[][8], double &snr,int &actrank);

    //     mode[][11]��  ģʽ������(����ȡ�Ƶ�ʡ���ֵ�����ࡢʵ�����鲿��z��ǡ�z��ֵ��bReal��bImag��engy)
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
