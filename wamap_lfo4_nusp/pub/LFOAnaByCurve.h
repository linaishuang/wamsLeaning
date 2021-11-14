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
// 2007-7-16 LFO���߷������ӽṹ
////  Macro define
#define  MODE_MAXGN       500//300           // ģʽ������������
#define  MODE_MAXLN       1000//200           // ģʽ�ɹ���·�����
#define  MODE_MAXFC       100//200           // ģʽ�ɹ���·����� // 2007-11-25
#define  MAXLFOCENT       20                 // �����ĳ�վ����� // 2008-08-28

#define  OBJ_MODE_MAX    3              // ���ߺ�ѡģʽ�����
#define  SYS_MODE_MAX    10             // ϵͳ��ѡģʽ�����

#define  IDTF_TYPE_FREQ   0.7           // 
#define  IDTF_TYPE_LOCL   0             // �ֲ�
#define  IDTF_TYPE_INTR   1             // ����

#define	 LFO_CLUST_RMAIN  0             // ����Ⱥ // 2008-09-24
#define	 LFO_CLUST_CRTCL  1             // �ٽ�Ⱥ // 2008-09-24
#define	 LFO_CLUST_OPOST  2             // ���Ⱥ // 2008-09-24

#define  OBJ_TYPE_GN      0             // ����
#define  OBJ_TYPE_LN      1             // ��·
#define  OBJ_TYPE_FC      2             // ��վ // 2007-11-25
#define  OBJ_TYPE_TR      3             // ��ѹ�� // 2008-09-24

//======== �������� ======//
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
	INT_REC           obj_id;                               // �豸ID
	char              obj_name[64];                         // �豸����
	int               type;                                 // ����(OBJ_TYPE_GN; OBJ_TYPE_LN)
	float             damp;                                 // �����
	float             freq;                                 // Ƶ��
	float             ampl;                                 // ���
	float             phas;                                 // ����
	float             mgnt;                                 // ����
	float             real;                                 // ʵ��  // 
	float             imag;                                 // �鲿
	float             fact;                                 // ��������
	float             engy;                                 // ����
    unsigned char     clus;                                 // ����Ⱥ //2008-09-24 add
	float			  pow;                                  // ��ɢ����
	int				  is_source;							// �Ƿ��Ŷ�Դ 1��,0��
}MODE_PART_OBJ;
// �����ĳ�վ
typedef struct tag_LFO_CNTR_FC
{
    INT_REC         fac_id;                                 //  ��վID
    char            fac_name[64];                           //  ��վ����
    INT_REC         area_id;                                //  ����ID
    unsigned char   fac_type;                               //  ��վ����
	VOL_INT_REC	    vlty_id;                                //  ��ߵ�ѹ�ȼ�
    float           v_aver;                                 //  ��ѹ��ֵ
    float           v_max;                                  //  ��ѹ���ֵ
    float           v_min;                                  //  ��ѹ��Сֵ
    float           v_ampl;                                 //  ��ѹ�ڶ�
    float           f_aver;                                 //  Ƶ�ʾ�ֵ
    float           f_max;                                  //  Ƶ�����ֵ
    float           f_min;                                  //  Ƶ����Сֵ
    float           f_ampl;                                 //  Ƶ�ʰڶ�
    unsigned char   is_cntr;                                //  �Ƿ�����
    unsigned char   is_disp;                                //  �Ƿ���ʾ
    unsigned char   clus;                                   //  ����Ⱥ
}LFO_CNTR_FC;

// ��������·
typedef struct tag_LFO_CNTR_LN
{
    INT_REC         acln_id;                                //  ��·ID��
    char            acln_name[64];	                        //  ��·����
    INT_REC         fac_id_s;	                            //  һ�˳�վID��
    INT_REC         fac_id_e;	                            //  ���˳�վID��
    unsigned char   is_cntr;                                //  �Ƿ�����
    unsigned char   is_disp;                                //  �Ƿ���ʾ
} LFO_CNTR_LN;
//  mode characteristics
typedef struct  lfo_mode_chrct
{
#ifdef _NUSP
	int					mode_id;                            //ģʽID
#else
	INT_REC           mode_id;                              //ģʽID
#endif // _NUSP
	int               type;                                 //����(=0 local; =1 interarea)

    time_t            time_bgn;                             // ��ʼʱ��  2008-06-04 by zhw at nj
    time_t            time_end;                             // ��ֹʱ��

	float             real;                                 //ʵ��(aver)
	float             imag;                                 //�鲿(aver)
	float             freq;                                 //Ƶ��(aver)
	float             damp;                                 //�����(aver)

	float             engyGn;                               //��������(aver) // 2007-08-10 lsf add
	float             engyLn;                               //��·����(aver) // 2007-08-10 lsf add
	
	int               nPartGn;                              //���������
	MODE_PART_OBJ     PartGn[MODE_MAXGN];                   //����ģ̬
	int               nPartLn;                              //�ɹ���·��
	MODE_PART_OBJ     PartLn[MODE_MAXLN];                   //��·ģ̬
	int               nPartTr;                              //�ɹ۱�ѹ���� // 2008-09-24
	MODE_PART_OBJ     PartTr[MODE_MAXLN];                   //��ѹ��ģ̬

	int               nPartFc;                              //�ɹ�fac
	MODE_PART_OBJ     PartFc[MODE_MAXFC];                   //facģ̬

    int               nLfoCenter;                           //�����ĳ�վ��
    LFO_CNTR_FC       LfoCenter[MAXLFOCENT];
	
}MODE_CHRCT;

//////////////////////////////////////////////////////////////////////////
// ��Ҫ�Ĳ�����
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
	//  ʶ����Ҫģʽ
	void IdtfMainMode( const int inMode, MODE_PART_OBJ *ipMode, int &onMode, MODE_PART_OBJ *opMode );
	void IdtfMainMode( const int inMode, MODE_PART_OBJ *ipMode, int &onMode, MODE_PART_OBJ *opMode, float FreqDif, float DampDif=100 );
	
	//  ��ͬģʽ�жϣ�����ģʽ������(Real, Imag)/(freq, damp)��
	bool IdtfSameMode( float iFreq1, float iDamp1, float iFreq2, float iDamp2, float FreqDif, float DampDif=100 );
	//  ��ͬģʽ�жϣ����ݲ�����飬ģʽ��������Ϊ�����о�(Real, Imag)/(freq, damp)��
	//bool IdtfSameMode( float iFreq1, float iDamp1, float iFreq2, float iDamp2, float FreqDif, float DampDif=100 );
	
	//  ʶ��������ɹ���·
	void IdtfPartGnPartLn( int nObjMode, MODE_PART_OBJ *pObjMode, int &nMode, MODE_CHRCT *pMode, float FreqDif, float DampDif=100 );
	void IdtfPartGnLnTr(int nObjMode,MODE_PART_OBJ *pObjMode,int &nMode, MODE_CHRCT *pMode);
	//  ����engyGn��engyLn������
	void CalcModeGnLnEngy( int nMode, MODE_CHRCT *pMode );		

	//  �����������
	bool CalcPartFactor( int iObjNum, MODE_PART_OBJ *ModeShape );
	bool CalcPartFactor( int iObjNum, MODE_PART_OBJ *ModeShape, int isTj );
	bool JudgPartFactorSign( int iObjNum, MODE_PART_OBJ *ModeShape, unsigned char isSyn = 1 ); // isSyn = 1 ͬ����
	bool ReSetFactorSign( int iObjNum, MODE_PART_OBJ *ModeShape );
	
	//  �ж��Ƿ�ͬ��
	int  JudgCohcyObjByMode( float iRefAngl, float iDifValu, float iAnglVar, unsigned char iIsRad=0 );// iIsRad = 1 ����

	//  ����������������豸
	bool SearchMaxFact( int nPartObj, MODE_PART_OBJ *iPartObj, float &oFact, float &oPhas, int &oIndex );
	bool SearchMaxFactOpst( int nPartObj, MODE_PART_OBJ *iPartObj, float iRefAngl, float iDifValu, float &oFact, float &oPhas, int &oIndex, unsigned char iIsRad=0 );// iIsRad = 1 ����
	
	//  ��Դ����ʶ��
	void IdtfCausLfoGn( int &nCausGn, MODE_PART_OBJ *oCausGn, float iDifValu, unsigned char iIsRad=0 );
	void IdtfLfoGnCohcyFc( int &nCausFc, MODE_PART_OBJ *oCausFc, MODE_PART_OBJ *CausGn, float iDifValu, unsigned char iIsRad=0 );
	void IdtfLfoGnCohcyFc( int &nCausFc, MODE_PART_OBJ *oCausFc, float iRefAngl, float iOpstAngl, float iDifValu, unsigned char iIsRad=0 );
	
	//  �������ж�
	//int  IdtfAmplMaxObj(  int isSyn = 1);// isSyn = 1 ͬ����
	int  IdtfLfoCenter(  MODE_CHRCT *pMode, int isSyn = 1);// isSyn = 1 ͬ����

	//  �������ƴ�ʩ���������ݲ������Ӻ������ģ����߲��Թ��򣬸����������ƴ�ʩ��
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
