/******************************************************************************
	ModuleName :   Prony Analysis 
	FileName   :   PronyAnalysis.cpp
	Description:   Directly estimating the frequency, damping, strength, and 
	               phase of modal components present in a given signal. 
	HISTORY    :
	   Date        Action        Author                Description 
	----------------------------------------------------------------------

******************************************************************************/

#include "PronyAnalysis.h"

char  g_PronyVersion[] = "v1.0.1";

extern void g_PubLogMessShow(const char *log, const int logtype = 1);

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 排序比较函数 damp由小到大
////////////////////////////////////////////////////////////////////////////////
extern "C" int g_PronyModeCompareByDamp(const void * p1, const void * p2)
{
    if ( ((PRONY_MODE_CHARA *)p1)->damp != ((PRONY_MODE_CHARA *)p2)->damp )
    {
		return ((PRONY_MODE_CHARA *)p1)->damp > ((PRONY_MODE_CHARA *)p2)->damp ? 1 : -1;
    }
	
    return ((PRONY_MODE_CHARA *)p1)->ampl < ((PRONY_MODE_CHARA *)p2)->ampl ? 1 : -1;
}
////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 排序比较函数 ampl由大到小
////////////////////////////////////////////////////////////////////////////////
extern "C" int g_PronyModeCompareByAmpl(const void * p1, const void * p2)
{
    if ( ((PRONY_MODE_CHARA *)p1)->ampl != ((PRONY_MODE_CHARA *)p2)->ampl )
    {
		return ((PRONY_MODE_CHARA *)p1)->ampl < ((PRONY_MODE_CHARA *)p2)->ampl ? 1 : -1;
    }
	
    return ((PRONY_MODE_CHARA *)p1)->damp > ((PRONY_MODE_CHARA *)p2)->damp ? 1 : -1;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 排序比较函数 engy由大到小
////////////////////////////////////////////////////////////////////////////////
extern "C" int g_PronyModeCompareByEngy(const void * p1, const void * p2)
{
    if ( ((PRONY_MODE_CHARA *)p1)->engy != ((PRONY_MODE_CHARA *)p2)->engy )
    {
		return ((PRONY_MODE_CHARA *)p1)->engy < ((PRONY_MODE_CHARA *)p2)->engy ? 1 : -1;
    }
	
    return ((PRONY_MODE_CHARA *)p1)->damp > ((PRONY_MODE_CHARA *)p2)->damp ? 1 : -1;
}
//////////////////////////////////////////////////////////////////////////
CPronyAnalysis::CPronyAnalysis()
{

}

CPronyAnalysis::~CPronyAnalysis()
{
   
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 计算单一模式曲线
//  处理过程 : 
//  备    注 :
//  返 回 值 :
//  参数说明 : nDataLen                 数据长度
//             oModeCurve               模式曲线[OUT]
//             iMode                    模式特征
////////////////////////////////////////////////////////////////////////////////
void CPronyAnalysis::CalcSinglModeCurve( int nDataLen, float * oModeCurve, PRONY_MODE_CHARA iMode )
{
    if( !oModeCurve || nDataLen <= 0 )
	{
		return;
	}
	
	int ii;
	for( ii=0; ii<nDataLen; ii++)
	{
		oModeCurve[ii] = (float)(iMode.ampl*cos((iMode.phas + ii*iMode.zphs)) * pow(iMode.zamp,(double)ii));
	}
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 计算多模式拟合曲线
//  处理过程 : 
//  备    注 :
//  返 回 值 :
//  参数说明 : nDataLen                 数据长度
//             oModeCurve               模式曲线[OUT]
//             iMode                    模式特征
//             nModeNum                 模式个数
////////////////////////////////////////////////////////////////////////////////
void CPronyAnalysis::CalcMultiModeCurve( int nDataLen, float * oModeCurve, PRONY_MODE_CHARA *iMode, int nModeNum )
{
    if( !oModeCurve || nDataLen <= 0 ) return;
	if( !iMode      || nModeNum <= 0 ) return;

	int ii, jj;
	for( ii=0; ii<nDataLen; ii++)
	{
		oModeCurve[ii] = 0;
		for( jj=0; jj<nModeNum; jj++)
		{
			oModeCurve[ii] += (float)(iMode[jj].ampl*cos((iMode[jj].phas + ii*iMode[jj].zphs)) * pow(iMode[jj].zamp,(double)ii));
		}
	}	
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : Prony分析
//  处理过程 : 
//  备    注 :
//  返 回 值 :
//  参数说明 : mode[][7]：   模式特征量(阻尼比、频率、幅值、初相、实部、虚部、engy)
////////////////////////////////////////////////////////////////////////////////
void CPronyAnalysis::PronyAnalysis7(PRONY_CALC_PARA calcpara, int oldnum, double data_x[], double data_t[], int &modenum, double mode[][7], double &snr,int &actrank)
{
	if ( !data_x || !data_t || !mode ) return ;
	
	int    ii;
	int    oModeNum = 0;
	PRONY_MODE_CHARA oMmode[PRONY_MAX_MODE];
	memset((void*)oMmode,0,PRONY_MAX_MODE*sizeof(PRONY_MODE_CHARA));
	
	PronyAnalysis( calcpara, oldnum, data_x, data_t, oModeNum, oMmode, snr, actrank);
	
	for(ii=0; ii<oModeNum; ii++)
	{
		mode[ii][0] = oMmode[ii].damp;                    // 阻尼比百分数
		mode[ii][1] = oMmode[ii].freq;                    // 频率
		mode[ii][2] = oMmode[ii].ampl;                    // 振幅
		mode[ii][3] = oMmode[ii].phas;                    // 初相
		mode[ii][4] = oMmode[ii].real;                    // 实部(衰减因子)
		mode[ii][5] = oMmode[ii].imag;                    // 虚部
		mode[ii][6] = oMmode[ii].engy;                    // engy
	}
	modenum = oModeNum;
	
	return ;
}


////////////////////////////////////////////////////////////////////////////////
//  函数功能 : Prony分析
//  处理过程 : 
//  备    注 :
//  返 回 值 :
//  参数说明 : mode[][8]：   模式特征量(阻尼比、频率、幅值、初相、实部、虚部、z相角和z幅值)
////////////////////////////////////////////////////////////////////////////////
void CPronyAnalysis::PronyAnalysis8(PRONY_CALC_PARA calcpara, int oldnum, double data_x[], double data_t[], int &modenum, double mode[][8], double &snr,int &actrank)
{
	if ( !data_x || !data_t || !mode ) return ;

	int    ii;
	int    oModeNum = 0;
	PRONY_MODE_CHARA oMmode[PRONY_MAX_MODE];
	memset((void*)oMmode,0,PRONY_MAX_MODE*sizeof(PRONY_MODE_CHARA));

	PronyAnalysis( calcpara, oldnum, data_x, data_t, oModeNum, oMmode, snr, actrank);

	for(ii=0; ii<oModeNum; ii++)
	{
		mode[ii][0] = oMmode[ii].damp;                    // 阻尼比百分数
		mode[ii][1] = oMmode[ii].freq;                    // 频率
		mode[ii][2] = oMmode[ii].ampl;                    // 振幅
		mode[ii][3] = oMmode[ii].phas;                    // 初相
		mode[ii][4] = oMmode[ii].real;                    // 实部(衰减因子)
		mode[ii][5] = oMmode[ii].imag;                    // 虚部
		mode[ii][6] = oMmode[ii].zphs;                    // zphs
		mode[ii][7] = oMmode[ii].zamp;                    // zmag
	}
	modenum = oModeNum;
	
	return ;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : Prony分析
//  处理过程 : 
//  备    注 :
//  返 回 值 :
//  参数说明 : mode[][11]：  模式特征量(阻尼比、频率、幅值、初相、实部、虚部、z相角、z幅值、bReal、bImag、engy)
////////////////////////////////////////////////////////////////////////////////
void CPronyAnalysis::PronyAnalysis11(PRONY_CALC_PARA calcpara, int oldnum, double data_x[], double data_t[], int &modenum, double mode[][11], double &snr,int &actrank)
{
	if ( !data_x || !data_t || !mode ) return ;
	
	int    ii;
	int    oModeNum = 0;
	int    datanum  = 0;
	PRONY_MODE_CHARA oMmode[PRONY_MAX_MODE];
	memset((void*)oMmode,0,PRONY_MAX_MODE*sizeof(PRONY_MODE_CHARA));
	
	datanum = oldnum;
	PronyAnalysis( calcpara, datanum, data_x, data_t, oModeNum, oMmode, snr, actrank);
	
	for(ii=0; ii<oModeNum; ii++)
	{
		mode[ii][0]  = oMmode[ii].damp;                    // 阻尼比百分数
		mode[ii][1]  = oMmode[ii].freq;                    // 频率
		mode[ii][2]  = oMmode[ii].ampl;                    // 振幅
		mode[ii][3]  = oMmode[ii].phas;                    // 初相
		mode[ii][4]  = oMmode[ii].real;                    // 实部(衰减因子)
		mode[ii][5]  = oMmode[ii].imag;                    // 虚部
		mode[ii][6]  = oMmode[ii].zphs;                    // zphs
		mode[ii][7]  = oMmode[ii].zamp;                    // zmag
		mode[ii][8]  = oMmode[ii].brel;                    // bReal
		mode[ii][9]  = oMmode[ii].bimg;                    // bImag
		mode[ii][10] = oMmode[ii].engy;                    // engy
	}
	modenum = oModeNum;
	
	return ;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : Prony分析
//  处理过程 : 
//  备    注 :
//  返 回 值 :
//  参数说明 : nDataLen                 数据长度
//             oModeCurve               模式曲线[OUT]
//             iMode                    模式特征
////////////////////////////////////////////////////////////////////////////////
//void CDSAPronyAna::PronyAnalysis(PRONY_CALC_PARA calcpara,int *oldnum,double data_x[],double data_t[],int *modenum,double mode[][8],int *datanum, double *modedata, double *snr,int *actrank)
//void CDSAPronyAna::PronyAnalysis(CALCULATE_PARA calcpara,double data_x[],double data_t[],int *modenum,double mode[][8],double *snr,int *actrank)
//void CDSAPronyAna::PronyAnalysis(PRONY_CALC_PARA calcpara,int *oldnum,double data_x[],double data_t[],int *modenum,double mode[][11], double *snr,int *actrank)
void   CPronyAnalysis::PronyAnalysis(PRONY_CALC_PARA calcpara, int oldnum, double data_x[], double data_t[], int &modenum, PRONY_MODE_CHARA *mode, double &snr,int &actrank)
{
	modenum = 0;
	if (CheckPara(calcpara) < 0)
	{
		return;
	}

    int    nn,np,mod_num,i,j,k,k1,k2,intn,m;                     // nn：预处理后等间距数据点数；np：振荡模式阶数；mod_num：符合要求的模式数；
	double zrr,zir,tt,damping,frequency,amplitude; //,phase;     // tt：数据点的间距；damping、frequency、amplitude：振荡模式中的衰减、频率、振幅、初相； 
	double r,dlt,snr1,dampratio;     //r1,                       // snr1：本次计算的信噪比； 
    int    *modp = NULL;
	double dataxp[PRONY_DATA_MAX]={0.0},datatp[PRONY_DATA_MAX]={0.0};                // data_x[],data_t[]：原始数据；dataxp[],datatp：预处理后数据；ppx：Prony分析后拟合数据
	double *a=NULL,*br=NULL,*bi=NULL,*zr=NULL,*zi=NULL,*ppx=NULL;// a：AR参数；br、bi：指数函数系数的实部与虚部；zr、zi：指数函数基数z的实部与虚部
	double *ra=NULL,*rb=NULL;
	
	// 2006-10-30 增加变量初始化
	//////////////////////////////////////////////////////////////////////////
	nn=np=mod_num=0;
	i=j=k=k1=k2=intn=m=0;
	zrr=zir=tt=damping=frequency=amplitude=0;
	r=dlt=snr1=dampratio=0;
	memset((void*)dataxp, 0 , PRONY_DATA_MAX*sizeof(double));
	memset((void*)datatp, 0 , PRONY_DATA_MAX*sizeof(double));
	
	//////////////////////////////////////////////////////////////////////////
    // 数据预处理
	///*nn=DataProcess(oldnum,data_x,data_t,calcpara.freq_max,dataxp,datatp,&tt);
	//if(nn <= 0)                                                            
	//{
	//	*modenum = -2;
	//	return ;
	//}
	
	if (oldnum > PRONY_DATA_MAX)
	{
		sprintf(m_LogStr,"数据点数越限[%d]>[%d],取前[%d]个",oldnum,PRONY_DATA_MAX,PRONY_DATA_MAX);
		MessShow(m_LogStr);
		oldnum = PRONY_DATA_MAX;
	}

	for (k=0; k<oldnum; k++)
	{
		dataxp[k]=data_x[k];
		datatp[k]=data_t[k];
	}
	nn = oldnum;

	if(nn >= 5)
	{
		tt = data_t[4]- data_t[3];
	}
	else
	{
		modenum = -2;
		return ;
	}
	//*/
	// amplation limited 
	//float ampl_lim = calcpara.minAbsAmpl;
	//if(ampl_lim < 0.00001)  ampl_lim = (float)0.00001;
	
	double datamax = dataxp[0];
	double datamin = dataxp[0];
	double amplmax = 0.0;
	for (k=1;k<nn;k++)
	{
		if(datamax < dataxp[k])
		{
			datamax = dataxp[k];
		}
		if(datamin > dataxp[k])
		{
			datamin = dataxp[k];
		}
	} 	
	amplmax = (datamax-datamin)/2;
	amplmax = 8 * amplmax;
	
	int prony_max_rank = 2*PRONY_MAX_MODE ;
	if(calcpara.if_set_rank == 1)
	{
		np = (int)(nn/2);
		if ( np > prony_max_rank ) np = prony_max_rank;
		
		if ((calcpara.rank >0) && (calcpara.rank <= np))
		{
			np=calcpara.rank;
		}
		else
		{
			modenum = -2;
			sprintf(m_LogStr,"阶数设定错误,应满足:0<Rank<%d",np);
			MessShow(m_LogStr);
			return ;
		}
		
	}
	else
	{
		if(nn <=110)
		{
			np=(int)(nn/2)-1;      // 自动设定的模式阶数//np=(int)(nn/2)-1;
		}
		else
		{
			np=(int)(nn/2)-2;      // np=(int)(nn/2)-2;  
		}
	}
	if ( np > prony_max_rank )
	{
		np = prony_max_rank;
	}
	//if (np>100) np=100;            // np最大不能大于200  
	//if (np>50) np=50;
	m=np;
	
    // 进行Prony计算
	ppx=(double *)calloc(nn,sizeof(double));
	memset((void *)ppx,  0, nn * sizeof(double));
	
	intn=1;
    do
	{
		m=np;
		a= (double *)calloc(np,sizeof(double));
		ra=(double *)calloc(m*np,sizeof(double));
		rb=(double *)calloc(m,sizeof(double));
		
		memset((void *)a,  0, np * sizeof(double));
		memset((void *)ra, 0, m*np * sizeof(double));
		memset((void *)rb, 0, m * sizeof(double));
		
		// 构造样本矩阵
		for (i=0;i<=m-1;i++)
		{
			for (k1=m;k1<=nn-1;k1++)
			{ 
				rb[i]-=(dataxp[k1]*dataxp[k1-i-1]);
			}
			for (j=0;j<=np-1;j++)
			{
				for (k2=m;k2<=nn-1;k2++)
				{ 
					k=i*np+(j);
					ra[k]+=(dataxp[k2-j-1]*dataxp[k2-i-1]);
				}
			}
		}
		
		// 求解得到的实数方程组的解,即参数 a
		int temp = EqutionSvd(ra,rb,m,np,a);
		if(temp == 0)
		{
			if(a!=NULL) free(a); a =NULL;
			if(ra!=NULL) free(ra);ra=NULL;
			if(rb!=NULL) free(rb);rb=NULL;
			modenum = -1;
			return ;
		}
		if(ra!=NULL) free(ra);ra=NULL;
		if(rb!=NULL) free(rb);rb=NULL;
		
		zr=(double *)calloc(np,sizeof(double)); 
		zi=(double *)calloc(np,sizeof(double));
		memset((void *)zr,  0, np * sizeof(double));
		memset((void *)zi,  0, np * sizeof(double));	
		// 高次多项式求根
		if( !Polynomial(a,np,zr,zi) )
		{
			if(a!=NULL) free(a);a=NULL;
			if(zr!=NULL) free(zr);zr=NULL;
			if(zi!=NULL) free(zi);zi=NULL;
			
			modenum = -1;
			return ;
		}
		
		br=(double *)calloc(np,sizeof(double));
		bi=(double *)calloc(np,sizeof(double));
		memset((void *)br,  0, np * sizeof(double));
		memset((void *)bi,  0, np * sizeof(double));	
		//形成超定复系数线性方程组，求解最小二乘解
		if( FormMatrix(a,zr,zi,dataxp,nn,np,br,bi) == 0)
		{
			if(a!=NULL) free(a);a=NULL;
			if(zr!=NULL) free(zr);zr=NULL;
			if(zi!=NULL) free(zi);zi=NULL;
			if(br!=NULL) free(br);br=NULL;
			if(bi!=NULL) free(bi);bi=NULL;
			
			modenum = -1;
			return ;
		}
		if(a!=NULL) free(a);a=NULL;
		
		// Prony分析拟合结果曲线
		for (k=0;k<=nn-1;k++)
		{  
			ppx[k]=0.0;
			for (i=0;i<=np-1;i++)
			{
				r=sqrt(zr[i]*zr[i]+zi[i]*zi[i]);
				dlt=atan2(zi[i],zr[i]);
				r=k*log(r); 
				r=exp(r);
				zrr=r*cos(k*dlt);                                           
				zir=r*sin(k*dlt);
				
				ppx[k]=ppx[k]+br[i]*zrr-bi[i]*zir;
			}
		}
		zrr=0.0;
		zir=0.0;
		for (k=0;k<=nn-1;k++)
		{  
			zrr+=dataxp[k]*dataxp[k];
			zir+=(dataxp[k]-ppx[k])*(dataxp[k]-ppx[k]);
		}
		snr1=0.0;
		snr1=20*log10(sqrt(zrr/nn)/sqrt(zir/nn));
		
		if(snr1 < calcpara.snr_min)
		{
			np=np-1; 
			if(zr!=NULL) free(zr);zr=NULL;
			if(zi!=NULL) free(zi);zi=NULL;
			if(br!=NULL) free(br);br=NULL;
			if(bi!=NULL) free(bi);bi=NULL;
		}
		
		intn=intn+1;
	}while ((snr1 < calcpara.snr_min) && (intn < 4));
	
	if (snr1 < calcpara.snr_min)
	{
		if( ppx != NULL) { free(ppx);	ppx=NULL; }
		if( ra != NULL)  { free(ra);	ra=NULL; }
		if( rb != NULL)  { free(rb);	rb=NULL; }
		if( a  != NULL)  { free(a);	    a=NULL;  }
		if( br != NULL)  { free(br);	br=NULL; }
		if( bi != NULL)  { free(bi);	bi=NULL; }
		if( zr != NULL)  { free(zr);	zr=NULL; }
		if( zi != NULL)  { free(zi);	zi=NULL; }
		
		modenum = 0;
		return;
	}
	
	//拟合曲线
	//    for (k=0;k<=nn-1;k++) modedata[k]=ppx[k];
	
    actrank=np;
    snr=snr1;
	
// 	if (calcpara.show_log == 1)
// 	{
// 		for (i=0;i<=nn-1;i++)
// 		{
// 			sprintf(m_LogStr,"%.4f",ppx[i]);
// 			MessShow(m_LogStr);
// 		}
// 		for (i=0;i<np-1;i++)
// 		{
// 			if (i ==0 )
// 			{
// 				sprintf(m_LogStr,"zi zr br bi");
// 				MessShow(m_LogStr);
// 			}
// 			sprintf(m_LogStr,"%.3f %.3f %.3f %.3f",zi[i],zr[i],br[i],bi[i]);
// 			MessShow(m_LogStr);
// 		}
// 	}

	if( ppx != NULL) { free(ppx);	ppx=NULL; }
    // 对分析结果进行处理，忽略幅值非常小的分量
	r=0.0;
	for (i=0;i<=np-1;i++)
	{  
		frequency=atan2(zi[i],zr[i])/(2.0*PRONY_PI*tt);
		amplitude=sqrt(br[i]*br[i]+bi[i]*bi[i]);
		if (frequency > 0.0) 
		{
			amplitude=2.0*amplitude;
			r=r > amplitude ? r: amplitude;
		}
		/*else if (frequency==0.0)
		{ 
		r=r > amplitude ? r : amplitude; 
	   }*/
	}
	
	r=calcpara.stor_lim_ampl*r; 
	if( r< calcpara.minAbsAmpl)
	{
		r = calcpara.minAbsAmpl;
	}
	if( calcpara.freq_min< 0)
	{
		calcpara.freq_min = 0.0;
	}
	
	// 2012/04/26 梯形法计算原始波形能量
	double engorg = 0.0f;  // 原始波形能量
	for (i =1; i<nn; i++)
	{
		engorg += fabs(dataxp[i-1]+dataxp[i]) * tt / 2.0f;
	}

	mod_num=0;
	modp=(int *)malloc(np*sizeof(int));                          // 模式指针
	memset((void *)modp,0, np*sizeof(int));	
	
	//double temptime = 0.0;
	double tempdata = 0.0;
	double tempzphs = 0.0;
	double tempzamp = 0.0;
	double singlemodedata    = 0;
	double singlemodedataold = 0;
	//temptime = (nn-1)*tt/3;///5.0;
	for (i=0;i<=np-1;i++)
	{  
		tempzphs  = atan2(zi[i],zr[i]) ;
		frequency = tempzphs/(2.0*PRONY_PI*tt);//  frequency = atan2(zi[i],zr[i])/(2.0*PRONY_PI*tt);
		amplitude = 2.0*sqrt(br[i]*br[i]+bi[i]*bi[i]);
		
		if(calcpara.delDC == 1)
		{
			if ((frequency >= calcpara.freq_min) && (frequency <= calcpara.freq_max) && (prony_fabs(frequency)>=PRONY_EPS_Freq) && (amplitude>=r)  && (amplitude < amplmax) )
			{
				tempzamp  = sqrt(zr[i]*zr[i]+zi[i]*zi[i]);
				damping   = log(tempzamp)*pow(tt,-1.0);// damping   = log(sqrt(zr[i]*zr[i]+zi[i]*zi[i]))*pow(tt,-1.0);
				dampratio = -damping/sqrt(damping*damping+4.0*PRONY_PI*PRONY_PI*frequency*frequency)*100.0;
				
				if (dampratio<=calcpara.stor_lim_damp )
				{
					mode[mod_num].damp = dampratio;                        // 阻尼比百分数
					mode[mod_num].freq = frequency;                        // 频率
					mode[mod_num].ampl = amplitude;                        // 振幅
					mode[mod_num].phas = atan2(bi[i],br[i]);               // 初相 //atan2(bi[i],br[i])*180.0/PRONY_PI;
					mode[mod_num].real = damping;                          // 实部(衰减因子)
					mode[mod_num].imag = 2.0*PRONY_PI*frequency;           // 虚部 amplitude*exp(damping*temptime);// 预测幅值 
					mode[mod_num].zphs = tempzphs;                         // zphs // atan2(zi[i],zr[i])*180.0/PRONY_PI;
					mode[mod_num].zamp = tempzamp;                         // zmag // sqrt(zr[i]*zr[i]+zi[i]*zi[i]);
					mode[mod_num].brel = 2.0*br[i];                            // bReal
					mode[mod_num].bimg = 2.0*bi[i];                            // bImag
					modp[i]=mod_num;
					// 2012/04/26 将能量计算提前
					// calculate mode energy
					tempdata = 0;
					singlemodedata = 0;
					singlemodedataold = mode[mod_num].ampl*cos(mode[mod_num].phas);
					// 梯形计算
					for(j=1;j<nn;j++) 
					{	
						singlemodedata     = mode[mod_num].ampl*cos((mode[mod_num].phas + j*mode[mod_num].zphs)) * pow(mode[mod_num].zamp,(double)j);     
						tempdata          += prony_fabs( singlemodedataold + singlemodedata );
						singlemodedataold = singlemodedata;
					}
					mode[mod_num].engy = tempdata * tt / 2.0;

					// 增加能量过滤条件
					if (mode[mod_num].engy < 2*engorg)
					{
						mod_num=mod_num+1;
					}
				}
				else
				{
					modp[i]=-1;
				}
			}
			else
			{
				modp[i]=-1;
			}
		}
		else
		{
			if ((frequency >= calcpara.freq_min) && (frequency <= calcpara.freq_max) && (prony_fabs(frequency)>=PRONY_EPS_Freq) && (amplitude>=r)  && (amplitude < amplmax) )
			{
				tempzamp  = sqrt(zr[i]*zr[i]+zi[i]*zi[i]);
				damping   = log(tempzamp)*pow(tt,-1.0);// damping   = log(sqrt(zr[i]*zr[i]+zi[i]*zi[i]))*pow(tt,-1.0);
				dampratio = -damping/sqrt(damping*damping+4.0*PRONY_PI*PRONY_PI*frequency*frequency)*100.0;
				
				if (dampratio<=calcpara.stor_lim_damp )
				{
					mode[mod_num].damp = dampratio;                        // 阻尼比百分数
					mode[mod_num].freq = frequency;                        // 频率
					mode[mod_num].ampl = amplitude;                        // 振幅
					mode[mod_num].phas = atan2(bi[i],br[i]);               // 初相 //atan2(bi[i],br[i])*180.0/PRONY_PI;
					mode[mod_num].real = damping;                          // 实部(衰减因子)
					mode[mod_num].imag = 2.0*PRONY_PI*frequency;           // 虚部 amplitude*exp(damping*temptime);// 预测幅值 
					mode[mod_num].zphs = tempzphs;                         // zphs // atan2(zi[i],zr[i])*180.0/PRONY_PI;
					mode[mod_num].zamp = tempzamp;                         // zmag // sqrt(zr[i]*zr[i]+zi[i]*zi[i]);
					mode[mod_num].brel = 2.0*br[i];                            // bReal
					mode[mod_num].bimg = 2.0*bi[i];                            // bImag
					
					modp[i]=mod_num;
					// 2012/04/26 将能量计算提前
					// calculate mode energy
					tempdata = 0;
					singlemodedata = 0;
					singlemodedataold = mode[mod_num].ampl*cos(mode[mod_num].phas);
					// 梯形计算
					for(j=1;j<nn;j++) 
					{	
						singlemodedata     = mode[mod_num].ampl*cos((mode[mod_num].phas + j*mode[mod_num].zphs)) * pow(mode[mod_num].zamp,(double)j);     
						tempdata          += prony_fabs( singlemodedataold + singlemodedata );
						singlemodedataold = singlemodedata;
					}
					mode[mod_num].engy = tempdata * tt / 2.0;
					
					// 增加能量过滤条件
					if (mode[mod_num].engy < 2*engorg)
					{
						mod_num=mod_num+1;
					}
				}
				else
				{
					modp[i]=-1;
				}
			}
			else if ((prony_fabs(frequency) < PRONY_EPS_Freq) && (amplitude >= r))  //2*r))
			{
				tempzamp  = sqrt(zr[i]*zr[i]+zi[i]*zi[i]);
				damping   = log(tempzamp)*pow(tt,-1.0);// damping   = log(sqrt(zr[i]*zr[i]+zi[i]*zi[i]))*pow(tt,-1.0);
				dampratio = -prony_sign(damping)*100.0;  
				
				if ( dampratio<=calcpara.stor_lim_damp)
				{
					mode[mod_num].damp = dampratio;                        // 阻尼比百分数
					mode[mod_num].freq = frequency;                        // 频率
					mode[mod_num].ampl = amplitude/2.0;                    // 振幅
					mode[mod_num].phas = atan2(bi[i],br[i]);               // 初相 //atan2(bi[i],br[i])*180.0/PRONY_PI;
					mode[mod_num].real = damping;                          // 实部(衰减因子)
					mode[mod_num].imag = 2.0*PRONY_PI*frequency;           // 虚部 amplitude*exp(damping*temptime);// 预测幅值 
					mode[mod_num].zphs = tempzphs;                         // zphs // atan2(zi[i],zr[i])*180.0/PRONY_PI;
					mode[mod_num].zamp = tempzamp;                         // zmag // sqrt(zr[i]*zr[i]+zi[i]*zi[i]);
					mode[mod_num].brel = br[i];                            // bReal
					mode[mod_num].bimg = bi[i];                            // bImag
					modp[i]=mod_num;
					// 2012/04/26 将能量计算提前
					// calculate mode energy
					tempdata = 0;
					singlemodedata = 0;
					singlemodedataold = mode[mod_num].ampl*cos(mode[mod_num].phas);
					// 梯形计算
					for(j=1;j<nn;j++) 
					{	
						singlemodedata     = mode[mod_num].ampl*cos((mode[mod_num].phas + j*mode[mod_num].zphs)) * pow(mode[mod_num].zamp,(double)j);     
						tempdata          += prony_fabs( singlemodedataold + singlemodedata );
						singlemodedataold = singlemodedata;
					}
					mode[mod_num].engy = tempdata * tt / 2.0;
					// 增加能量过滤条件
					if (mode[mod_num].engy < 2*engorg)
					{
						mod_num=mod_num+1;
					}
				}
				else
				{
					modp[i]=-1;
				}
			}
			else
			{
				modp[i]=-1;
			}
		}	   
	}
	
	////////////////////////////////////////////////////////////////////////////////
	// mode sort 
	if(mod_num > 1 )
	{
		if (calcpara.queue_type == 0)
		{
			// mode sort by damp and ampl
			qsort((void *)mode, mod_num, sizeof(PRONY_MODE_CHARA), g_PronyModeCompareByDamp);
		}
		else if (calcpara.queue_type == -2)
		{
			// mode sort by engy and damp
			qsort((void *)mode, mod_num, sizeof(PRONY_MODE_CHARA), g_PronyModeCompareByEngy);
		}
		else
		{
			// mode sort by ampl and damp
			qsort((void *)mode, mod_num, sizeof(PRONY_MODE_CHARA), g_PronyModeCompareByAmpl);
		}
		
		if( modp != NULL) { free(modp);	modp=NULL; }
		if( br != NULL)  { free(br);	br=NULL; }
		if( bi != NULL)  { free(bi);	bi=NULL; }
		if( zr != NULL)  { free(zr);	zr=NULL; }
		if( zi != NULL)  { free(zi);	zi=NULL; }
		
		modenum = mod_num;
	}

	if (calcpara.show_log == 1)
	{
		sprintf(m_LogStr,"********PronyAnalysis Ver %s build at %s %s********",g_PronyVersion, __DATE__, __TIME__);
		MessShow(m_LogStr);
		sprintf(m_LogStr,"set_rank=%d,rank=%d,queue_type=%d,delDC=%d,snr_min=%f",
			calcpara.if_set_rank,calcpara.rank,calcpara.queue_type,calcpara.delDC,calcpara.snr_min);
		MessShow(m_LogStr);
		sprintf(m_LogStr,"freq_min=%f,freq_max=%f,minAbsAmpl=%f,stor_lim_ampl=%f,stor_lim_damp=%f",
			calcpara.freq_min,calcpara.freq_max,calcpara.minAbsAmpl,calcpara.stor_lim_ampl,calcpara.stor_lim_damp);
		MessShow(m_LogStr);
		for (int ii =0; ii<nn; ii++)
		{
			sprintf(m_LogStr,"%.2f    %.4f",datatp[ii],dataxp[ii]);
			MessShow(m_LogStr);
		}
		sprintf(m_LogStr,"模式数:%d,信噪比:%f",mod_num, snr);
		MessShow(m_LogStr);
		sprintf(m_LogStr,"  幅值      频率     阻尼比     初相      实部      虚部     能量");
		MessShow(m_LogStr);
		for(i=0; i<mod_num; i++)
		{
			sprintf(m_LogStr,"%6.3f    %6.3f    %6.3f    %6.3f    %6.3f    %6.3f    %6.3f",
				mode[i].ampl,
				mode[i].freq,
				mode[i].damp,
				mode[i].phas,
				mode[i].real,
				mode[i].imag,
				mode[i].engy);
			MessShow(m_LogStr);
		}
		// 输出模式曲线
// 		float *curve=NULL;
// 		curve = new float[nn];
// 		for (i=0;i<mod_num; i++)
// 		{
// 			memset(curve,0,nn*sizeof(float));
// 			CalcSinglModeCurve(nn,curve,mode[i]);
// 			sprintf(m_LogStr,"第%d个模式:%6.3f    %6.3f    %6.3f    %6.3f",i,mode[i].ampl,mode[i].freq,mode[i].damp,mode[i].phas);
// 			MessShow(m_LogStr);
// 			for (j =0; j<nn; j++)
// 			{
// 				sprintf(m_LogStr,"%.3f",curve[j]);
// 				MessShow(m_LogStr);
// 			}
// 		}
// 		delete []curve;
	}
	
	return;
}

/* 数据预处理 */
int CPronyAnalysis::DataProcess(int oldnum,double x[],double t[],double fmax,double xp[],double tp[],double *dt)
{ 	  
	int i,j,kn,n=0,num=0;
	double lkn,h=0.0,t1,dtinitial;
    // double *dxp,*dtp;
	// int knmax=0;

	for (int k=0;k<oldnum;k++)
	{
		xp[k]=x[k];
		tp[k]=t[k];
	} 
	
	n=oldnum;
	
	if ((t[n-1]<0.0) || (t[n-1] < t[0]))
	{
		sprintf(m_LogStr,"时间轴错误");
		MessShow(m_LogStr);
		return(-1);
	}
	
	if (n < 8 )  
	{
		//	printf("观测数据点数太少，不能进行分析！");
		//	return (-1);
	}
	
	if ((n>=8)&&(n<100))
	{
		*dt=(tp[3]-tp[2]);
		//*dt=(tp[6]+tp[4]-tp[5]-tp[3])/2;
		return (n);		
	}		
	else if (n<8)
	{
		kn=(int)(40/n);
		h=(t[n-1]-t[0])/(n-1);
		lkn=h/kn;
		
		num=(n-1)*kn+1;
		
		xp[0]=x[0];  tp[0]=t[0];
		for(i=1;i<=num-1;i++)
		{
			if (i%kn<=PRONY_EPS)
			{
				j=i/kn;
				xp[i]=x[j];
				tp[i]=t[j];
			}
			else
			{
				t1=t[0]+i*lkn;
				
				xp[i]=InsertData(x,t[0],h,n,t1);
				tp[i]=t1;
			}
		}
	}
	else
	{
		if(n>=150 && n<=200)
		{
			kn=2;
		}
		else if(n>200 && n<=360)
		{
			kn=3;
		}
		else if(n>360 && n<500)
		{
			kn=4;
		}
		else
		{
			kn=5;
		}
		
		//		kn=(int)(n/150)+1;
		
		dtinitial=(tp[6]+tp[4]-tp[5]-tp[3])/2;
		
		/*2006-03-14
		knmax=(int)(1.0/(2.0*fmax)/dtinitial);
		if (kn>knmax)  kn=knmax;
		//*/
		
		num=(int)((n-1)/(kn));
		
		xp[0]=x[0];  tp[0]=t[0];
		for(i=1;i<=num-1;i++)
		{
			j=i*(kn);
			xp[i]=x[j];
			tp[i]=t[j];
		}
	}
	
	*dt=(tp[6]+tp[4]-tp[5]-tp[3])/2;
	
	return (num);
}


  /*  数据预处理－－插值  */
double CPronyAnalysis::InsertData(double y[],double x0,double h,int n,double t)
{ 
	int i,j,k,m,l;
    double z,xx[15],yy[15];

    z=0.0;
    if (n<1)
	{
		return(z);
	}

    if (n==1) 
	{
		z=y[0]; 
		return(z);
	}

    m=15;
    if (m>n)
	{
		m=n;
	}
    if (t<=x0)
	{
		k=1;
	}
    else if (t>=x0+(n-1)*h)
	{
		k=n;
	}
    else
    { 
		k=1; j=n;
        while ((k-j!=1)&&(k-j!=-1))
        { 
			l=(k+j)/2;
            if (t<x0+(l-1)*h)
			{
				j=l;
			}
            else
			{
				k=l;
			}
        }
        if (prony_fabs(t-(x0+(l-1)*h))>prony_fabs(t-(x0+(j-1)*h)))
		{
			k=j;
		}
    }
    j=1; l=0;
    for (i=1;i<=m;i++)
    { 
		k=k+j*l;
        if ((k<1)||(k>n))
        {
			l=l+1;
			j=-j;
			k=k+j*l;
		}
        xx[i-1]=x0+(k-1)*h; yy[i-1]=y[k-1];
        l=l+1; j=-j;
    }

    i=0;
    do
    { 
		i=i+1;
		z=yy[i];
        for (j=0;j<=i-1;j++)
		{
           z=yy[j]+(t-xx[j])*(yy[j]-z)/(xx[j]-xx[i]);
		}
        yy[i]=z;
    }while ((i!=m-1)&&(prony_fabs(yy[i]-yy[i-1])>PRONY_EPS));

    return(z);
}

  /* 求解实系数线性方程组 */
int CPronyAnalysis::RealEqution(double aa[],double ab[],int m,int n,double ax[])
{ 
    int i,*index=NULL;
	double d1=0.0;

	index=(int *)calloc(n,sizeof(int));

    for (i=0;i<=n-1;i++)
	{
	  ax[i]=ab[i];
	}

    Lucomp(aa,n,index,d1);
    Luback(aa,n,index,ax);

	free(index);
	index=NULL;

    return(1);
}

void CPronyAnalysis::Lucomp(double a[],int n,int indx[],double d)
{
	int i,imax,j,k,ij,ik,kj,jk,imaxk;
	double big,dum,sum,temp=0.0,TINY=1.0e-20;
    double *vv=NULL;

	vv=(double *)calloc(n,sizeof(double));

	d=1.0;
	for (i=0;i<=n-1;i++) 
	{
	  big=0.0;
	  for (j=0;j<=n-1;j++)
	  {
		  ij=i*n+j;
		  temp=prony_fabs(a[ij]);
		  if (temp > big)
		  {
			  big=temp;		
		  }
	  }
	  if (big == 0.0) 
	  {
		  sprintf(m_LogStr,"Singular matrix in routine LUDCMP");
		  MessShow(m_LogStr);
		  
          free(vv);
          vv=NULL;
		  return;		
	  }
	  vv[i]=1.0/big;
	}

	for (j=0;j<=n-1;j++) 
	{
	  for (i=0;i<=j-1;i++)
	  {
		  ij=i*n+j;
		  sum=a[ij];
		  for (k=0;k<=i-1;k++)
		  {	
			  ik=i*n+k;
			  kj=k*n+j;
			  sum=sum-a[ik]*a[kj];
		  }
		  a[ij]=sum;
	  }

	  big=0.0;
	  for (i=j;i<=n-1;i++)
	  {
		  ij=i*n+j;
		  sum=a[ij];
		  for (k=0;k<=j-1;k++)
		  {
			  ik=i*n+k;
			  kj=k*n+j;
			  sum=sum-a[ik]*a[kj];
		  }				
		  a[ij]=sum;
		  dum=vv[i]*prony_fabs(sum);
		  if ( dum >= big) 
		  {
		  	imax=i;
 		    big=dum;				
		  }
	  }

	  if (j != imax) 
	  {
		  for (k=0;k<=n-1;k++) 
		  {
			  imaxk=imax*n+k;
			  jk=j*n+k;
			  dum=a[imaxk];
			  a[imaxk]=a[jk];
			  a[jk]=dum;
		  }
		  d = -d;
		  vv[imax]=vv[j];
	  }

	  indx[j]=imax;
	  if (a[j*n+j] == 0.0) a[j*n+j]=TINY;
	  if (j != n) 
	  {
		  dum=1.0/(a[j*n+j]);
		  for (i=j+1;i<=n-1;i++)
		  {
		  	ij=i*n+j;
			a[ij]=a[ij]*dum;
		  }
	  }
	}

    free(vv);
    vv=NULL;

    return;
}


void CPronyAnalysis::Luback(double a[],int n,int indx[],double b[])
{
	int i,ii=-1,j,ll,ij;
	double sum;

	for (i=0;i<=n-1;i++)
	{
		ll=indx[i];
		sum=b[ll];
		b[ll]=b[i];
		if (ii != -1)
		{
			for (j=ii;j<=i-1;j++) 
			{
				ij=i*n+j;
				sum=sum-a[ij]*b[j];
			}
		}
		else if (sum != 0)
		{
			ii=i;
		}
		b[i]=sum;
	}

	for (i=n-1;i>=0;i--) 
	{
		sum=b[i];
		for (j=i+1;j<=n-1;j++)
		{
			ij=i*n+j;
			sum=sum-a[ij]*b[j];
		}
		b[i]=sum/a[i*n+i];
	}

	return;
}


/* 高次多项式求根
*  
*  通过计算多项式伴随矩阵特征值的方法求根。先形成多项式的伴随矩阵，再化
*  简伴随矩阵为Hessenberg Matrix，计算得到的Hessenberg Matrix的特征值。
*/
bool CPronyAnalysis::Polynomial(double a[], int m, double rtr[], double rti[])
{
	int i,j,ji;  
	double *hess; 
	hess=(double *)calloc(m*m,sizeof(double));
	memset((void*)hess, 0, m*m*sizeof(double));

//	if (a[m]==0.0) printf("Bad array a in Polynomial");//MsgSend
	for (i=0;i<=m-1;i++) 
	{
		hess[i]=-a[i];
//		hess[i]=-a[m-1-i]/a[m];
		for (j=1;j<=m-1;j++)
		{
			ji=j*m+i;
			hess[ji]=0.0;
		}
		if (i!=m-1)
		{
			hess[(i+1)*m+i]=1.0;
		}
	}

	ReduceMatrix(hess,m);

	if( !MatrixEigenvalue(hess,m,rtr,rti) )
	{
		if(hess != NULL)
		{
			free(hess);
			hess=NULL;
		}

		return false;
	}

	if(hess != NULL)
	{
		free(hess);
		hess=NULL;
	}

	return true;
}

#define RADIX 2.0
void CPronyAnalysis::ReduceMatrix(double a[], int n)
{
	int last,i,j,ij,ji;
	double s,r,g,f,c,sqrdx;
	s=r=g=f=c=sqrdx=0.0;

	sqrdx=RADIX*RADIX;
	last=0;

	while (last == 0) 
	{
		last=1;
		for (i=0;i<=n-1;i++) 
		{
			r=c=0.0;
			for (j=0;j<=n-1;j++)
			{
				if (j != i)
				{
					ji=j*n+i;
					ij=i*n+j;
					c=c+prony_fabs(a[ji]);
					r=r+prony_fabs(a[ij]);
				}
			}
			if ((c!=0.0) && (r!=0.0))
			{
				g=r/RADIX;
				f=1.0;
				s=c+r;
				while (c<g) 
				{
					f=f*RADIX;
					c=c*sqrdx;
				}
				g=r*RADIX;
				while (c>g)
				{
					f=f/RADIX;
					c=c/sqrdx;
				}
				if ((c+r)/f < 0.95*s) 
				{
					last=0;
					g=1.0/f;
					for (j=0;j<=n-1;j++)
					{
						ij=i*n+j;
						a[ij]=a[ij]*g;
					}
					for (j=0;j<=n-1;j++)
					{
						ji=j*n+i;
						a[ji]=a[ji]*f;
					}
				}
			}
		}
	}
	return;
}
#undef RADIX
bool CPronyAnalysis::MatrixEigenvalue(double a[], int n, double wr[], double wi[])
{
	int counter = 0; 
	int i,j,l,k,ij,nn,m,its,mmin;
	double z,y,x,w,v,u,t,s,r,q,p,anorm;

	z=y=x=w=v=u=t=s=r=q=p=0;
	anorm=0.0;
	for (i=0;i<=n-1;i++)
	{
		k=(i-1) >= 0 ? (i-1) : 0;
		for (j=k;j<=n-1;j++)
		{
			ij=i*n+j;
			anorm=anorm+prony_fabs(a[ij]);
		}
	}
	nn=n-1;
	t=0.0;
	while (nn >= 0)
	{
		its=0;
		do 
		{
			for (l=nn;l>=1;l--)
			{
				s=prony_fabs(a[(l-1)*n+(l-1)])+prony_fabs(a[l*n+l]);

				if (s == 0.0)
				{
					s=anorm;
				}
				if ((prony_fabs(a[l*n+(l-1)])+s) == s)
				{
					break;
				}
			}
			x=a[nn*n+nn];
			if (l == nn)
			{
				wr[nn]=x+t;
				wi[nn]=0.0;
				nn=nn-1;
			}
			else
			{
				y=a[(nn-1)*n+(nn-1)];
				w=a[nn*n+(nn-1)]*a[(nn-1)*n+nn];
				if (l == (nn-1)) 
				{
					p=0.5*(y-x);
					q=p*p+w;
					z=sqrt(prony_fabs(q));
					x=x+t;
					if (q >= 0.0)
					{
						z=p+prony_sign(p)*prony_fabs(z);
						wr[nn]=x+z;
						wr[nn-1]=wr[nn];
						if (z!=0.0)
						{
							wr[nn]=x-w/z;
						}
						wi[nn]=0.0;
						wi[nn-1]=0.0;
					}
					else 
					{
						wr[nn]=x+p;
						wr[nn-1]=wr[nn];
						wi[nn]=z;
						wi[nn-1]=-z;
					}
					nn=nn-2;
				}
				else 
				{
					if (its == 30)
					{
						sprintf(m_LogStr,"Too many iterations in MatrixEigenvalue");
						MessShow(m_LogStr);
						return false;
					}
					if ((its == 10) || (its == 20)) 
					{
						t=t+x;
						for (i=0;i<=nn;i++)
						{
							a[i*n+i]=a[i*n+i]-x;
						}

						s=prony_fabs(a[nn*n+(nn-1)])+prony_fabs(a[(nn-1)*n+(nn-2)]);
						x=0.75*s;
						y=x;
						w=-0.4375*s*s;
					}
					its=its+1;
					for (m=(nn-2);m>=l;m--)
					{
						z=a[m*n+m];
						r=x-z;
						s=y-z;
						p=(r*s-w)/a[(m+1)*n+m]+a[m*n+(m+1)];
						q=a[(m+1)*n+(m+1)]-z-r-s;
						r=a[(m+2)*n+(m+1)];
						s=prony_fabs(p)+prony_fabs(q)+prony_fabs(r);
						p=p/s;
						q=q/s;
						r=r/s;
						if (m == l)
						{
							break;
						}

						u=prony_fabs(a[m*n+(m-1)])*(prony_fabs(q)+prony_fabs(r));
						v=prony_fabs(p)*(prony_fabs(a[(m-1)*n+(m-1)])+prony_fabs(z)+prony_fabs(a[(m+1)*n+(m+1)]));

						if ((u+v) == v)
						{
							break;
						}
					}
					for (i=m+2;i<=nn;i++) 
					{
						a[i*n+(i-2)]=0.0;

						if (i != (m+2))
						{
							a[i*n+(i-3)]=0.0;
						}
					}
					for (k=m;k<=nn-1;k++)
					{
						if (k != m) 
						{
							p=a[k*n+(k-1)];
							q=a[(k+1)*n+(k-1)];
							r=0.0;

							if (k != (nn-1))   r=a[(k+2)*n+(k-1)];

							x=prony_fabs(p)+prony_fabs(q)+prony_fabs(r);

							if (x != 0.0)
							{
								p = p/x;
								q = q/x;
								r = r/x;
							}
						}
						s=prony_sign(p)*sqrt(p*p+q*q+r*r);
						if (s != 0.0) 
						{
							if (k == m)
							{
								if (l != m)
								{
									a[k*n+(k-1)] = -a[k*n+(k-1)];
								}

							}
							else
							{
								a[k*n+(k-1)] = -s*x;
							}

							p=p+s;
							x=p/s;
							y=q/s;
							z=r/s;
							q=q/p;
							r=r/p;
							for (j=k;j<=nn;j++)
							{
								p=a[k*n+j]+q*a[(k+1)*n+j];
								if (k != (nn-1)) 
								{
									p=p+r*a[(k+2)*n+j];
									a[(k+2)*n+j]=a[(k+2)*n+j]-p*z;
								}
								a[(k+1)*n+j]=a[(k+1)*n+j] - p*y;
								a[k*n+j]=a[k*n+j] - p*x;
							}
							mmin=nn < k+3 ? nn : k+3;
							for (i=l;i<=mmin;i++) 
							{
								p=x*a[i*n+k]+y*a[i*n+(k+1)];
								if (k != (nn-1)) 
								{
									p=p+z*a[i*n+(k+2)];
									a[i*n+(k+2)]=a[i*n+(k+2)] - p*r;
								}
								a[i*n+(k+1)]=a[i*n+(k+1)] - p*q;
								a[i*n+k]=a[i*n+k] - p;
							}
						}
					}
				}
			}

			counter += 1;
			if(counter >= 15)
			{
				counter = 0; 
				DelayMinSec(1);   // 降低CPU负荷率  2005-11-29
			}
		}while (l < nn-1);
	}

	return true;
}

/*      
  * 超定复系数线性方程组求解
  * 形成超定复系数线性方程组，将复数运算转化成实数运算，
  * 求解得到的实数方程组的最小二乘解
*/
int CPronyAnalysis::FormMatrix(double pa[],double zzr[],double zzi[],double px[],int m,int n,double xr[],double xi[])
{
	int i,j,l1,l2,l3,l4;
	double r=0.0,dlt=0.0,ar,ai;
	double *ca=NULL,*cb=NULL,*xx=NULL;

	ca=(double *)calloc((m+1)*(2*n),sizeof(double));
	cb=(double *)calloc(m+1,sizeof(double));
	xx=(double *)calloc(2*n,sizeof(double));
	memset((void *)ca, 0, (m+1)*(2*n)*sizeof(double));
	memset((void *)cb, 0, (m+1)*sizeof(double));
	memset((void *)xx, 0, (2*n)*sizeof(double));

	for (i=0;i<=m-1;i++)
	{
		cb[i]=px[i];
	}
	/* 递推求y(k) (p<=n<=N)   
	for (i=n;i<=m-1;i++)
	{
		for (j=0;j<=n-1;j++)
		{
			cb[i]=cb[i]-pa[j]*cb[i-j-1];
		}				
	}*/
	for (i=0;i<=m-1;i++)  
	{   
		for (j=0;j<=n-1;j++)
		{  
		   r=sqrt(zzr[j]*zzr[j]+zzi[j]*zzi[j]);
		   dlt=atan2(zzi[j],zzr[j]);
		   if ((r+1.0)!=1.0)
		   {
			   r=i*log(r);
			   r=exp(r);
		   }
		   ar=(r*cos(i*dlt));
		   ai=(r*sin(i*dlt));
		   l1=i*2*n+j;         ca[l1]=ar;
		   l2=i*2*n+(j+n);     ca[l2]=-ai;
/*		   l3=(i+m)*2*n+j;     ca[l3]=ai;
		   l4=(i+m)*2*n+(j+n); ca[l4]=ar;
*/		}
	}

	for (j=0;j<=n-1;j++)
	{
	   l3=(m)*2*n+j;
	   ca[l3]=ca[j+n];
	   l4=(m)*2*n+(j+n);
	   ca[l4]=ca[j];
	}

    if( EqutionSvd(ca,cb,m+1,2*n,xx) == 0)
	{
		if( ca!=NULL )
		{
			free(ca);
			ca=NULL;
		}
	    if( cb!=NULL )
		{
			free(cb);
			cb=NULL;
		}
	    if( xx!=NULL )
		{
			free(xx);
			xx=NULL;
		}

		return (0);
	}
	
	for (i=0;i<=n-1;i++)
	{
	   xr[i]=xx[i];
	   xi[i]=xx[n+i];
	}
	if( ca!=NULL )
	{
		free(ca);
		ca=NULL;
	}
	if( cb!=NULL )
	{
		free(cb);
		cb=NULL;
	}
	if( xx!=NULL )
	{
		free(xx);
		xx=NULL;
	}
    return (1);
}

  
int CPronyAnalysis::EqutionSvd(double a[],double b[],int m,int n,double x[])
{
    int k;
    double wmax,wmin;
    double *w=NULL,*v=NULL;

    w=(double *)calloc(n,sizeof(double));
    v=(double *)calloc(n*n,sizeof(double));
	memset((void *)w, 0, n*sizeof(double));
	memset((void *)v, 0, (n*n)*sizeof(double));
 
    /*Do SVD decompose for matrix a */       
    if( !Svdcomp(a,m,n,w,v) )
	{
		if( w!=NULL )
		{
			free(w);
			w=NULL;
		}
		if( v!=NULL )
		{
			free(v);
			v=NULL;
		}

		return 0;
	}
 
    wmax=0;    
    for (k=0;k<=n-1;k++)
  		if (w[k] > wmax) wmax=w[k];

    /* Define "small"           */     
    //wmin=1.0e-4;  //wmax*(1.0e-7);   1.0e-5;
	wmin=1.0e-4;

    /* zero the "small" singular values */
    for (k=0;k<=n-1;k++)
  		if (w[k] < wmin) w[k]=0.0f;
 
    /*Back substitute for the right-hand side vector*/
    Svdback(a,w,v,m,n,b,x);

	if( w!=NULL )
	{
		free(w);
		w=NULL;
	}
	if( v!=NULL )
	{
		free(v);
		v=NULL;
	}

    return(1);
}

/* 对矩阵进行奇异值分解 */
bool CPronyAnalysis::Svdcomp(double a[],int m,int n,double w[],double v[])
{
	int    flag,i,its,j,k,l,nm,ki,kj,ik,jk,il,ij,ji,jnm;
	double anorm,anorm1,c=0,f=0,g,h=0.0,s,scale,x=0,y=0,z=0;
	double *rv1 = NULL;

	rv1=(double *)calloc(n,sizeof(double));
	memset((void *)rv1, 0, n*sizeof(double));
	
	g=0.0;
	scale=0.0;
	anorm=0.0;
	for (i=0;i<=n-1;i++)
	{
		l=i+1;
		rv1[i]=scale*g;
		g=0.0;
		s=0.0;
		scale=0.0;
		if (i <= (m-1)) 
		{
			for (k=i;k<=m-1;k++) 
			{
				ki=k*n+i;
				scale=scale+prony_fabs(a[ki]);
			}
			if (scale!=0.0) 
			{
				for (k=i;k<=m-1;k++)
				{
					ki=k*n+i;
					a[ki]=a[ki]/scale;
					s=s+a[ki]*a[ki];
				}
				f=a[i*n+i];
				g=-(prony_sign(f))*sqrt(s);
				h=f*g-s;
				a[i*n+i]=f-g;
				for (j=l;j<=n-1;j++)
				{
					s=0.0;
					for (k=i;k<=m-1;k++)
					{
						ki=k*n+i;
						kj=k*n+j;
						s=s+a[ki]*a[kj];
					}
					f=s/h;
					for (k=i;k<=m-1;k++) 
					{
						ki=k*n+i;
						kj=k*n+j;
						a[kj]=a[kj]+f*a[ki];
					}
				}
				for (k=i;k<=m-1;k++) 
				{
					ki=k*n+i;
					a[ki]=a[ki]*scale;
				}
			}
		}
		w[i]=scale*g;
		g=0.0;
		s=0.0;
		scale=0.0;
		if ((i<=(m-1)) && (i!=(n-1))) 
		{
			for (k=l;k<=n-1;k++) 
			{
				ik=i*n+k;
				scale=scale+prony_fabs(a[ik]);
			}
			if (scale!=0.0) 
			{
				for (k=l;k<=n-1;k++)
				{
					ik=i*n+k;
					a[ik]=a[ik]/scale;
					s=s+a[ik]*a[ik];
				}
				il=i*n+l;
				f=a[il];
				g=-(prony_sign(f))*sqrt(s);
				h=f*g-s;
				a[il]=f-g;
				for (k=l;k<=n-1;k++)
				{
					ik=i*n+k;
					rv1[k]=a[ik]/h;
				}					
				for (j=l;j<=m-1;j++) 
				{
					s=0.0;
					for (k=l;k<=n-1;k++)
					{
						ik=i*n+k;
						jk=j*n+k;
						s=s+a[jk]*a[ik];
					}
					for (k=l;k<=n-1;k++)
					{
						jk=j*n+k;
						a[jk]=a[jk]+s*rv1[k];
					}
				}
				for (k=l;k<=n-1;k++)
				{
					ik=i*n+k;
					a[ik]=a[ik]*scale;
				}
			}
		}
		anorm1=prony_fabs(w[i])+prony_fabs(rv1[i]);
		if (anorm < anorm1)
		{
			anorm=anorm1;
		}
	}
	for (i=n-1;i>=0;i--)
	{
		if (i < n-1) 
		{
			if (g != 0.0)
			{
				for (j=l;j<=n-1;j++)
				{
					ji=j*n+i;
					ij=i*n+j;
					il=i*n+l;
					v[ji]=(a[ij]/a[il])/g;
				}
				for (j=l;j<=n-1;j++) 
				{
					s=0.0;
					for (k=l;k<=n-1;k++)
					{
						ik=i*n+k;
						kj=k*n+j;
						s=s+a[ik]*v[kj];
					}
					for (k=l;k<=n-1;k++)
					{
						kj=k*n+j;
						ki=k*n+i;
						v[kj]=v[kj]+s*v[ki];
					}
				}
			}
			for (j=l;j<=n-1;j++) 
			{
				ij=i*n+j;
				ji=j*n+i;
				v[ij]=0.0;v[ji]=0.0;
			}
		}
		v[i*n+i]=1.0;
		g=rv1[i];
		l=i;
	}
	for (i=n-1;i>=0;i--) 
	{
		l=i+1;
		g=w[i];
		for (j=l;j<=n-1;j++) 
		{
			ij=i*n+j;
			a[ij]=0.0;
		}
		if (g != 0.0) 
		{
			g=1.0/g;
			for (j=l;j<=n-1;j++) 
			{
				s=0.0;
				for (k=l;k<=m-1;k++)
				{
					ki=k*n+i;
					kj=k*n+j;
					s=s+a[ki]*a[kj];
				}
				f=(s/a[i*n+i])*g;
				for (k=i;k<=m-1;k++) 
				{	
					ki=k*n+i;
					kj=k*n+j;
					a[kj]=a[kj]+f*a[ki];
				}
			}
			for (j=i;j<=m-1;j++) 
			{
				ji=j*n+i;
				a[ji]=a[ji]*g;
			}
		}
		else
		{
			for (j=i;j<=m-1;j++)
			{
				ji=j*n+i;
				a[ji]=0.0;
			}
		}
		a[i*n+i]=a[i*n+i]+1.0;
	}
	for (k=n-1;k>=0;k--)
	{
		for (its=0;its<30;its++) 
		{
			flag=1;			
			for (l=k;l>=0;l--)
			{
				nm=l-1;
				if ((prony_fabs(rv1[l])+anorm) == anorm)
				{
					flag=0;
					break;
				}
				if ((prony_fabs(w[nm])+anorm) == anorm)
				{
					break;			
				}
			}
			if (flag==1)
			{
				c=0.0;
				s=1.0;
				for (i=l;i<=k;i++) 
				{
					f=s*rv1[i];
					rv1[i]=c*rv1[i];
					if ((prony_fabs(f)+anorm) == anorm)
					{
					//	flag=0;
						break;
					}
					g=w[i];
					h=Calculate(f,g);
					w[i]=h;
					h=1.0/h;
					c=g*h;
					s=-(f*h);
					for (j=0;j<=m-1;j++) 
					{
						jnm=j*n+nm;
						ji=j*n+i;
						y=a[jnm];
						z=a[ji];
						a[jnm]=y*c+z*s;
						a[ji]=z*c-y*s;
					}
				}
			}
			z=w[k];
			if (l == k) 
			{
				if (z < 0.0)
				{
					w[k] = -z;
					for (j=0;j<=n-1;j++)
					{
						jk=j*n+k;
						v[jk] = -v[jk];
					}
				}
				break;
			}
			if (its == 30) 
			{
				sprintf(m_LogStr,"no convergence in 30 svdcmp iterations");
				MessShow(m_LogStr);
				if(rv1!=NULL)
				{
					free(rv1);
					rv1=NULL;
				}
				return false;
			}
			x=w[l];
			nm=k-1;
			y=w[nm];
			g=rv1[nm];
			h=rv1[k];
			f=((y-z)*(y+z)+(g-h)*(g+h))/(2.0*h*y);
			g=Calculate(f,1.0);
			f=((x-z)*(x+z)+h*((y/(f+prony_sign(f)*prony_fabs(g)))-h))/x;
			c=1.0;
			s=1.0;
			for (j=l;j<=nm;j++) 
			{
				g=rv1[j+1];
				y=w[j+1];
				h=s*g;
				g=c*g;
				z=Calculate(f,h);
				rv1[j]=z;
				c=f/z;
				s=h/z;
				f=x*c+g*s;
				g = g*c-x*s;
				h=y*s;
				y=y*c;
				for (i=0;i<=n-1;i++)
				{
					ij=i*n+j;
					x=v[ij];
					z=v[ij+1];
					v[ij]=x*c+z*s;
					v[ij+1]=-x*s+z*c;
				}
				z=Calculate(f,h);
				w[j]=z;
				if (z != 0.0) 
				{
					z=1.0/z;
					c=f*z;
					s=h*z;
				}
				f=c*g+s*y;
				x=c*y-s*g;
				for (i=0;i<=m-1;i++)
				{
					ij=i*n+j;
					y=a[ij];
					z=a[ij+1];
					a[ij]=y*c+z*s;
					a[ij+1]=z*c-y*s;
				}
			}
			rv1[l]=0.0;
			rv1[k]=f;
			w[k]=x;
		}
	}
	if(rv1!=NULL)
	{
		free(rv1);
		rv1=NULL;
	}
	return true;
}

double CPronyAnalysis::Calculate(double a, double b)
{
	double absa,absb,num1,num2;

	absa=prony_fabs(a);
	absb=prony_fabs(b);
	num1=absb/absa;
	num2=absa/absb;

	if (absa > absb)
	{
		return(absa*sqrt(1.0+num1*num1));
	}
	else 
	{
		return (absb==0.0 ? 0.0 : absb*sqrt(1.0+num2*num2));
	}

}
 
void CPronyAnalysis::Svdback(double u[], double w[], double v[], int m, int n, double b[], double x[])
{
	int k,j,i,ij,jk;
	double s,*tmp;

	tmp=(double* )calloc(n,sizeof(double));
	memset((void*)tmp, 0, n*sizeof(double));

	for (j=0;j<=n-1;j++)
	{
		s=0.0;
		if (w[j]!=0.0)
		{
			for (i=0;i<=m-1;i++) 
			{
				ij=i*n+j;
				s=s+u[ij]*b[i];
			}
			s=s/w[j];
		}
		tmp[j]=s;
	}
	for (j=0;j<=n-1;j++)
	{
		s=0.0;
		for (k=0;k<=n-1;k++)
		{
			jk=j*n+k;
			s=s+v[jk]*tmp[k];
		}
		x[j]=s;
	}
	if(tmp!=NULL)
	{
		free(tmp);
		tmp=NULL;
	}

	return;
}

const double  PRONY_ZERO= 0.0000001;//0.000000001

double CPronyAnalysis::prony_fabs(double x)
{
	/*double y = 100*x;

	if(y>PRONY_ZERO)
	{
		return x;
	}
	else if(y<(-PRONY_ZERO))
	{
		return -x;
	}
	else
	{
		return 0;
	}
	*/
	return fabs(x);
}

float CPronyAnalysis::prony_fabs(float x)
{
	/*
	float y = 100*x;
	
	if(y>PRONY_ZERO)
	{
		return x;
	}
	else if(y<(-PRONY_ZERO))
	{
		return -x;
	}
	else
	{
		return 0;
	}
	*/
	return (float)(fabs(x));
}

int CPronyAnalysis::prony_sign(double x)
{
	double y = 100*x;

	if(y>PRONY_ZERO)
	{
		return 1;
	}
	else if(y<(-PRONY_ZERO))
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int CPronyAnalysis::prony_sign(float x)
{
	float y = 100*x;
	
	if(y>PRONY_ZERO)
	{
		return 1;
	}
	else if(y<(-PRONY_ZERO))
	{
		return -1;
	}
	else
	{
		return 0;
	}
}
void CPronyAnalysis::DelayMinSec( int msec )
{
#ifdef _WINDOWS32
	Sleep( msec ) ;
#endif
	
#ifdef _UNIX
	usleep( msec*1000 ) ;
#endif	
}

void CPronyAnalysis::MessShow( const char *log )
{
	//printf( "%s\n", log );
	g_PubLogMessShow(log);
 	
	return;
}

int CPronyAnalysis::CheckPara( PRONY_CALC_PARA &calcpara )
{
	int nflag = 0;
	if (calcpara.if_set_rank <0 )
	{
		sprintf(m_LogStr,"设定阶数标志错误,自动设定阶数[%d]",calcpara.if_set_rank);
		MessShow(m_LogStr);
	}
	if(calcpara.if_set_rank == 1)
	{
		if (calcpara.rank <=0 )
		{
			nflag --;
			sprintf(m_LogStr,"设定的阶数错误[%d]",calcpara.rank);
			MessShow(m_LogStr);
		}
	}

	if (calcpara.queue_type !=0 && calcpara.queue_type !=-1 && calcpara.queue_type !=-2 )
	{
		sprintf(m_LogStr,"排序参数错误,按幅值排序[%d]",calcpara.queue_type);
		MessShow(m_LogStr);
	}
	if (calcpara.delDC !=0 && calcpara.delDC !=1 )
	{
		sprintf(m_LogStr,"去直流参数错误,不去直流[%d]",calcpara.delDC);
		MessShow(m_LogStr);
	}
	if (calcpara.freq_min <0.0 )
	{
		calcpara.freq_min = 0.0f;
		sprintf(m_LogStr,"频率下限错误,置为0.0[%.3f]",calcpara.freq_min);
		MessShow(m_LogStr);
	}
	if (calcpara.freq_max <0.0 )
	{
		nflag --;
		sprintf(m_LogStr,"频率上限错误[%.3f]",calcpara.freq_max);
		MessShow(m_LogStr);
	}
	if (calcpara.freq_min >= calcpara.freq_max )
	{
		nflag --;
		sprintf(m_LogStr,"频率下限大于上限[%.3f>=%.3f]",calcpara.freq_min,calcpara.freq_max);
		MessShow(m_LogStr);
	}
	if (calcpara.minAbsAmpl <0.0 )
	{
		calcpara.minAbsAmpl = 0.0f;
		sprintf(m_LogStr,"保存振荡模式幅值最小限值(绝对值)错误,置为0.0[%.3f]",calcpara.minAbsAmpl);
		MessShow(m_LogStr);
	}
	if (calcpara.stor_lim_ampl <0.0 || calcpara.stor_lim_ampl > 1.0 )
	{
		calcpara.stor_lim_ampl = 0.0f;
		sprintf(m_LogStr,"保存振荡模式幅值限值(与最大幅值的比值)(<0.0或>1.0),置为0.0[%.3f]",calcpara.stor_lim_ampl);
		MessShow(m_LogStr);
	}
	if (calcpara.stor_lim_damp <-100.0 || calcpara.stor_lim_damp > 100.0 )
	{
		calcpara.stor_lim_damp = -100.0f;
		sprintf(m_LogStr,"保存振荡模式阻尼比限值错误(<-100.0或>100.0),置为-100.0[%.3f]",calcpara.stor_lim_damp);
		MessShow(m_LogStr);
	}
	if (calcpara.snr_min < 0.0)
	{
		calcpara.snr_min = 20;
		sprintf(m_LogStr,"最小信噪比限值错误,置为20.0[%.3f]",calcpara.snr_min);
		MessShow(m_LogStr);
	}
	return nflag;
}
