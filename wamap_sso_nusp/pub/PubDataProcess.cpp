/******************************************************************************
  ModuleName :   Public Data Process
  FileName   :   PubDataProcess.cpp
  Description:   Data Process Implemention
  HISTORY :    	
      Date        Version       Author                Activities      
   ----------------------------------------------------------------------
   03/13/2007       1.0          lsf                   created
******************************************************************************/
#include "PubDataProcess.h"

////  Base function
////////////////////////////////////////////////////////////////////////////////
//  �������� : ����Ƕȵ�����ֵ
//  ������� : 
//  ��    ע :
//  �� �� ֵ : ����ֵ
//  ����˵�� : fAng                     �Ƕ�
////////////////////////////////////////////////////////////////////////////////
double DP_Cos(float fAng)
{
    return cos(DP_RAD * fAng);
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ����Ƕȵ�����ֵ
//  ������� : 
//  ��    ע :
//  �� �� ֵ : ����ֵ
//  ����˵�� : fAng                     �Ƕ�
////////////////////////////////////////////////////////////////////////////////
double DP_Sin(float fAng)
{
    return sin(DP_RAD * fAng);
}
////////////////////////////////////////////////////////////////////////////////
//  �������� : �������ܵ���
//  ������� : 
//  ��    ע :
//  �� �� ֵ : 
//  ����˵�� : 
////////////////////////////////////////////////////////////////////////////////
int DP_NumBySample(int Num, short sample)
{
	if (sample <= 1)
	{
		return Num;
	}
	else
	{
		return (int)((Num-(sample-1))/sample + 1 );
	}	
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ����ǶȲ�
//  ������� : 
//  ��    ע :
//  �� �� ֵ : 
//  ����˵�� : 
////////////////////////////////////////////////////////////////////////////////
int DP_CalcAngleDiff( int nPnt, float * pAgl1, float * pAgl2, float * pAgl )
{
	if ( nPnt<1 || !pAgl1 || !pAgl2 || !pAgl ) return -1;

	int  kk;
	for ( kk=0; kk< nPnt; kk++ )
	{
		if ( pAgl1[kk] > pAgl2[kk] ) 
		{
			pAgl[kk] = pAgl1[kk] - pAgl2[kk];
		}
		else
		{
			pAgl[kk] = pAgl2[kk] - pAgl1[kk];
		}

		pAgl[kk] = (float)fmod( (double)(pAgl[kk]+360.0), 360.0 );
		// 2008-07-16
		if (pAgl[kk] > 180.0) 	pAgl[kk] = ( -360.0f + pAgl[kk]);
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ������ֵ���л�����Ϣ
//  ������� : �������м�ֵ
//  ��    ע :
//  �� �� ֵ : 
//  ����˵�� : iSrcData                 ��ֵ����
//             nDataLen                 ���ݸ���
//             DataBaseInfo             ������Ϣ
////////////////////////////////////////////////////////////////////////////////
void DP_CalcDataSeqBaseInfo(const float *iSrcData,int nDataLen, SEQ_BASE_INFO &DataBaseInfo)
{
	int   j;	
	memset((void*)&DataBaseInfo, 0, sizeof(SEQ_BASE_INFO));
	
	if( nDataLen < 1)
	{
		return ;
	}
	else
	{
		DataBaseInfo.ave_val = iSrcData[0];
		DataBaseInfo.max_val = iSrcData[0];
		DataBaseInfo.min_val = iSrcData[0];
		for( j=1; j<nDataLen; j++ )
		{
			DataBaseInfo.ave_val += iSrcData[j];
			
			if(DataBaseInfo.max_val < iSrcData[j])
			{
				DataBaseInfo.max_val     = iSrcData[j];
				DataBaseInfo.max_val_pnt = j;
			}
			if(DataBaseInfo.min_val > iSrcData[j])
			{
				DataBaseInfo.min_val     = iSrcData[j];
				DataBaseInfo.min_val_pnt = j;
			}
		}
		
		DataBaseInfo.ave_val = DataBaseInfo.ave_val/nDataLen;
		DataBaseInfo.max_ampl = (float)(DP_ABS(DataBaseInfo.max_val - DataBaseInfo.min_val) / 2.0);
		
		if(DataBaseInfo.ave_val == 0)
		{
			DataBaseInfo.max_ampl_rat = 0;
		}
		else
		{
			DataBaseInfo.max_ampl_rat = (DataBaseInfo.max_ampl/DP_ABS(DataBaseInfo.ave_val));
		}
	}
	return;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : �����о�ֵ
//  ������� : N������, ȥ��һ�����ֵ��һ����Сֵ, ����N-2�����ݵ�ƽ��ֵ
//  ��    ע :
//  �� �� ֵ : �о�ֵ
//  ����˵�� : iSrcData                 ԭʼ����
//             nDataLen                 ���ݳ���
////////////////////////////////////////////////////////////////////////////////
float DP_MidAveVal(const float * iSrcData, int nDataLen)
{
    float   fVal;

    if( !iSrcData || nDataLen < 1 ) return 0.0f;

    if( nDataLen == 1 ) return iSrcData[0];

    if( nDataLen == 2 ) return ( iSrcData[0] + iSrcData[1] ) / 2;

    float * pfData = (float *)calloc(nDataLen, sizeof(float));
    if( !pfData ) return 0.0f;
    memcpy((void *)pfData, (void *)iSrcData, sizeof(float) * nDataLen);

    //  ֱ�Ӳ�������
    int     ii, jj;
    for( ii = 1; ii < nDataLen; ++ii )
    {
        //  ���ڵ���������������ֵ, ����ԭλ, �����������
        if( pfData[ii] >= pfData[ii - 1] ) continue;

        //  ���渱��
        fVal = pfData[ii];

        //  �����������������в��Ҳ���λ��, ���ؼ��ִ�ļ�¼����һ��λ��
        for( jj = ii - 1; jj >= 0 && fVal < pfData[jj]; --jj ) pfData[jj + 1] = pfData[jj];

        //  ���뵽��ȷ��λ����
        pfData[jj + 1] = fVal;
    }

    fVal = 0.0f;
    for( ii = nDataLen - 2; ii > 0; --ii ) fVal += pfData[ii];
    fVal /= nDataLen - 2;

    free(pfData);

    return fVal;
}

////  Filter function
////////////////////////////////////////////////////////////////////////////////
//  �������� : �޷��˲��� ---- �����ж��˲���
//  ������� : ���������ڵĲ���ֵ���, ���������(�Ծ���ֵ��ʾ), Ȼ�������β������������ֵA���бȽ�,
//             ��С�ڻ����A, ��ȡ���β���ֵ; ����ȡ�ϴβ���ֵ��Ϊ���β���ֵ. �����ͺ�
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : iSrcData                 ԭʼ����
//             oFltData                 �˲�����[OUT]
//             nDataLen                 ���ݳ���
//             fMaxErr                  ���ƫ��
////////////////////////////////////////////////////////////////////////////////
void DP_LimAmpFilter(const float * iSrcData, float * oFltData, int nDataLen, float fMaxErr)
{
    if( !iSrcData || !oFltData || nDataLen < 1 || fMaxErr <= 0 ) return;

    oFltData[0] = iSrcData[0];
    for( int idx = 1; idx < nDataLen; ++idx )
    {
        if( iSrcData[idx] - iSrcData[idx - 1] > fMaxErr || iSrcData[idx - 1] - iSrcData[idx] > fMaxErr )
        {
            oFltData[idx] = iSrcData[idx - 1];
        }
        else
        {
            oFltData[idx] = iSrcData[idx];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ��ֵ�˲���
//  ������� : ��ĳһ������������N��(һ��NΪ����), ��N�β���ֵ����, ȡ�м�ֵ��Ϊ���β���ֵ
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : iSrcData                 ԭʼ����
//             oFltData                 �˲�����[OUT]
//             nDataLen                 ���ݳ���
//             nFiltLen                 �˲�����
////////////////////////////////////////////////////////////////////////////////
void DP_MidValFilter(const float * iSrcData, float * oFltData, int nDataLen, int nFiltLen)
{
    int             nHalfLen = ( nFiltLen - 1 ) / 2;        //  �˲������
    float *         pfData   = NULL;                        //  �˲�������
    float           fTmp;
    int             idx, ii, jj;

    nFiltLen = nHalfLen * 2 + 1;
    if( !iSrcData || !oFltData || nHalfLen < 1 || nDataLen < nFiltLen ) return;

    pfData = (float *)calloc(nFiltLen, sizeof(float));
    if( !pfData ) return;

    //  Դ�ź�ǰ�����˰봰�ڿ�����ݱ��ֲ���
    for( idx = 0; idx < nHalfLen; ++idx ) oFltData[idx] = iSrcData[idx];
    for( idx = nDataLen - nHalfLen; idx < nDataLen; ++idx ) oFltData[idx] = iSrcData[idx];

    //  ��ÿһ����Ϊ����
    for( idx = nDataLen - nHalfLen - 1; idx >= nHalfLen; --idx )
    {
        //  ��idxΪ����, ѡȡ2*n+1������data[idx-n],...,data[idx],...,data[idx+n]
        for( ii = 0; ii < nFiltLen; ++ii ) pfData[ii] = iSrcData[idx - nHalfLen + ii];

        //  ֱ�Ӳ�������
        for( ii = 1; ii < nFiltLen; ++ii )
        {
            //  ���ڵ���������������ֵ, ����ԭλ, �����������
            if( pfData[ii] >= pfData[ii - 1] ) continue;

            //  ���渱��
            fTmp = pfData[ii];

            //  �����������������в��Ҳ���λ��, ���ؼ��ִ�ļ�¼����һ��λ��
            for( jj = ii - 1; jj >= 0 && fTmp < pfData[jj]; --jj ) pfData[jj + 1] = pfData[jj];

            //  ���뵽��ȷ��λ����
            pfData[jj + 1] = fTmp;
        }

        oFltData[idx] = pfData[nHalfLen];
    }

    free(pfData);
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ��ֵƽ���˲��� ---- ���������ƽ���˲���
//  ������� : ��������N������, ȥ��һ�����ֵ��һ����Сֵ, ����N-2�����ݵ�ƽ��ֵ, Nȡ: 3~14
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : iSrcData                 ԭʼ����
//             oFltData                 �˲�����[OUT]
//             nDataLen                 ���ݳ���
//             nFiltLen                 �˲�����
////////////////////////////////////////////////////////////////////////////////
void DP_MidAveFilter(const float * iSrcData, float * oFltData, int nDataLen, int nFiltLen)
{
// 	memcpy(oFltData,iSrcData,nDataLen*sizeof(float));
// 	return;
    int             nHalfLen = ( nFiltLen - 1 ) / 2;        //  �˲������
    float *         pfData   = NULL;                        //  �˲�������
    float           fTmp;
    int             nValidLen;
    int             idx, ii, jj;

    nFiltLen  = nHalfLen * 2 + 1;
    nValidLen = nFiltLen - 2;
    if( !iSrcData || !oFltData || nHalfLen < 1 || nDataLen < nFiltLen ) return;

    pfData = (float *)calloc(nFiltLen, sizeof(float));
    if( !pfData ) return;

    //  Դ�ź�ǰ�����˰봰�ڿ�����ݱ��ֲ���
    for( idx = 0; idx < nHalfLen; ++idx ) oFltData[idx] = iSrcData[idx];
    for( idx = nDataLen - nHalfLen; idx < nDataLen; ++idx ) oFltData[idx] = iSrcData[idx];

    //  ��ÿһ����Ϊ����
    for( idx = nDataLen - nHalfLen - 1; idx >= nHalfLen; --idx )
    {
        //  ��idxΪ����, ѡȡ2*n+1������data[idx-n],...,data[idx],...,data[idx+n]
        for( ii = 0; ii < nFiltLen; ++ii ) pfData[ii] = iSrcData[idx - nHalfLen + ii];

        //  ֱ�Ӳ�������
        for( ii = 1; ii < nFiltLen; ++ii )
        {
            //  ���ڵ���������������ֵ, ����ԭλ, �����������
            if( pfData[ii] >= pfData[ii - 1] ) continue;

            //  ���渱��
            fTmp = pfData[ii];

            //  �����������������в��Ҳ���λ��, ���ؼ��ִ�ļ�¼����һ��λ��
            for( jj = ii - 1; jj >= 0 && fTmp < pfData[jj]; --jj ) pfData[jj + 1] = pfData[jj];

            //  ���뵽��ȷ��λ����
            pfData[jj + 1] = fTmp;
        }

        oFltData[idx] = 0.0f;
        for( jj = nValidLen; jj > 0; --jj ) oFltData[idx] += pfData[jj];
        oFltData[idx] /= nValidLen;
    }

    free(pfData);
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ��ֵ�˲��� ---- ����ƽ���˲���
//  ������� : ����ȡ��β���ֵ��������ƽ��
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : iSrcData                 ԭʼ����
//             oFltData                 �˲�����[OUT]
//             nDataLen                 ���ݳ���
//             nFiltLen                 �˲�����
////////////////////////////////////////////////////////////////////////////////
void DP_AveValFilter(const float * iSrcData, float * oFltData, int nDataLen, int nFiltLen)
{
    int             nHalfLen = ( nFiltLen - 1 ) / 2;        //  �˲������
    int             idx, ii, jj;

    nFiltLen  = nHalfLen * 2 + 1;

    if( !iSrcData || !oFltData || nHalfLen < 1 || nDataLen < nFiltLen ) return;

    //  Դ�ź�ǰ�����˰봰�ڿ�����ݱ��ֲ���
    for( idx = 0; idx < nHalfLen; ++idx ) oFltData[idx] = iSrcData[idx];
    for( idx = nDataLen - nHalfLen; idx < nDataLen; ++idx ) oFltData[idx] = iSrcData[idx];

    idx = nDataLen - nHalfLen - 1;
    oFltData[idx] = 0.0f;
    for( ii = 0, jj = idx - nFiltLen; ii < nFiltLen; ++ii, ++jj ) oFltData[idx] += iSrcData[jj];

    for( idx = nDataLen - nHalfLen - 2; idx >= nHalfLen; --idx )
    {
        oFltData[idx] = oFltData[idx + 1] - iSrcData[idx + 1 + nHalfLen] + iSrcData[idx - nHalfLen];
    }

    for( idx = nDataLen - nHalfLen - 1; idx >= nHalfLen; --idx ) oFltData[idx] /= nFiltLen;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ��Ȩƽ���˲���
//  ������� : ������N�β���ֵ, �ֱ���ϲ�ͬ�ļ�Ȩϵ�������
//            ��Ȩϵ��һ����С���, ��ͻ���������ɲ�����Ч��, ��ǿϵͳ�Բ����仯���Ƶı�ʶ
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : iSrcData                 ԭʼ����
//             oFltData                 �˲�����[OUT]
//             nDataLen                 ���ݳ���
//             vfCoe                    ��Ȩϵ��
//             nFiltLen                 �˲�����
////////////////////////////////////////////////////////////////////////////////
void DP_CoeAveFilter(const float * iSrcData, float * oFltData, int nDataLen, const float * vfCoe, int nFiltLen)
{
    int             nHalfLen = ( nFiltLen - 1 ) / 2;        //  �˲������
    float           fCoeSum  = 0.0f;
    int             idx, ii;

    nFiltLen  = nHalfLen * 2 + 1;

    if( !iSrcData || !oFltData || !vfCoe || nHalfLen < 1 || nDataLen < nFiltLen ) return;

    for( ii = 0; ii < nFiltLen; ++ii ) fCoeSum += vfCoe[ii];
    if( fCoeSum == 0 ) return;

    //  Դ�ź�ǰ�����˰봰�ڿ�����ݱ��ֲ���
    for( idx = 0; idx < nHalfLen; ++idx ) oFltData[idx] = iSrcData[idx];
    for( idx = nDataLen - nHalfLen; idx < nDataLen; ++idx ) oFltData[idx] = iSrcData[idx];

    for( idx = nDataLen - nHalfLen - 1; idx >= nHalfLen; --idx )
    {
        oFltData[idx] = 0.0f;
        for( ii = 0; ii < nFiltLen; ++ii ) oFltData[idx] += iSrcData[idx - nHalfLen + ii] * vfCoe[ii];
        oFltData[idx] /= fCoeSum;
    }
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : һ���ͺ��˲���(�򵥵�ͨ�˲���)
//  ������� : �٣�= �ᡤ�أ�+ (1-��)���٣�-1
//             �٣�Ϊ�����˲����ֵ; �أ�Ϊ���β���ֵ; �٣�-1Ϊ�ϴ��˲����ֵ; ��Ϊ�˲�ϵ��, ��ֵͨ��ԶС��1
//             �����˲������ֵ��Ҫȡ�����ϴ��˲������ֵ ( ע�ⲻ���ϴεĲ���ֵ, ��ͼ�Ȩƽ���˲����б�������� )
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : iSrcData                 ԭʼ����
//             oFltData                 �˲�����[OUT]
//             nDataLen                 ���ݳ���
//             fCoe                     �˲�ϵ��
////////////////////////////////////////////////////////////////////////////////
void DP_SimLowPasFilter(const float * iSrcData, float * oFltData, int nDataLen, float fCoe)
{
    float           fTmp = 1.0f - fCoe;

    if( !iSrcData || !oFltData || nDataLen < 1 ) return;

    oFltData[0] = iSrcData[0];
    for( int idx = 1; idx < nDataLen; ++idx ) oFltData[idx] = fCoe * iSrcData[idx] + fTmp * oFltData[idx - 1];
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : Butterworth�˲���(��ֹƵ��3Hz)
//  ������� : 
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : iSrcData                 ԭʼ����
//             oFltData                 �˲�����[OUT]
//             nDataLen                 ���ݳ���
//             rank                     �˲�������(2 - 5)
////////////////////////////////////////////////////////////////////////////////
void DP_BtwthLowPasFilter(const float * iSrcData, float * oFltData, int nDataLen, float fSmpFreq, int rank)
{
	if ( rank > 5 ) rank = 5;
	if ( rank < 2 ) rank = 2;
	
	if( !iSrcData || !oFltData || fSmpFreq <= 0 ||nDataLen < rank ) return ;
	
	///////////////////////////////////////////////////////////
	// 2 �� 5 �װ�����˹�˲���,��ֹƵ��3Hz

	int   i=0;
	float b[6],a[6];//�˲����ķ���b�ͷ�ĸa,���y
	float middle1,middle2;//�洢�м����
	float fp=3.0,Ts=1/fSmpFreq;//ͨ����ֹƵ��3Hz�������ֹƵ��5Hz��ͨ�����˥��3dB����֤C��1���������С˥��8dB

	float wp    = (float)(2*DP_PI*fp*Ts);
	float Const = (float)(1/tan((double)(wp/2)));//���ݺ���z��pֻ����ϵ��һ��ϵ��	

	if(rank==2)//��Ϊ2��
	{
		b[0]=1;b[1]=2;b[2]=1;
		middle1 = (float)(-2*cos(3*DP_PI/4));

		a[0]=Const*Const+middle1*Const+1;
		a[1]=-2*Const*Const+2;
		a[2]=Const*Const-middle1*Const+1;

		oFltData[0]= iSrcData[0];
		oFltData[1]= iSrcData[1];
		for( i=2; i<nDataLen; i++ )
			oFltData[i] =( b[0]*iSrcData[i]+b[1]*iSrcData[i-1]+b[2]*iSrcData[i-2] - a[1]*oFltData[i-1]-a[2]*oFltData[i-2] ) / a[0];
	}

	else if(rank==3)//��Ϊ3��
	{
		b[0]=1;b[1]=3;b[2]=3;b[3]=1;
		middle1=(float)(-2*cos(2*DP_PI/3));
		float temp1=(middle1+1)*Const*(Const+1);
		float temp2=(middle1+1)*Const*(Const-1);

		a[0]=temp1+2;
		a[1]=-temp2;
		a[2]=-temp1+6;
		a[3]=temp2;

		oFltData[0]= iSrcData[0];
		oFltData[1]= iSrcData[1];
		oFltData[2]= iSrcData[2];
		for(i=3; i<nDataLen; i++)
			oFltData[i] =( b[0]*iSrcData[i]+b[1]*iSrcData[i-1]+b[2]*iSrcData[i-2]+b[3]*iSrcData[i-3] - a[1]*oFltData[i-1]-a[2]*oFltData[i-2]-a[3]*oFltData[i-3] ) / a[0];
	}

	else if(rank==4)//��Ϊ4��
	{
		b[0]=1;b[1]=4;b[2]=6;b[3]=4;b[4]=1; 
		middle1=(float)(-2*cos(5*DP_PI/8));
		middle2=(float)(-2*cos(7*DP_PI/8));
		float a1[3]={Const*Const+middle1*Const+1, -2*Const*Const+2, Const*Const-middle1*Const+1};
		float a2[3]={Const*Const+middle2*Const+1, -2*Const*Const+2, Const*Const-middle2*Const+1};

		a[0]=a1[0]*a2[0];
		a[1]=a1[0]*a2[1]+a1[1]*a2[0];
		a[2]=a1[0]*a2[2]+a1[1]*a2[1]+a1[2]*a2[0];
		a[3]=a1[1]*a2[2]+a1[2]*a2[1];
		a[4]=a1[2]*a2[2];

		oFltData[0]= iSrcData[0];
		oFltData[1]= iSrcData[1];
		oFltData[2]= iSrcData[2];
		oFltData[3]= iSrcData[3];
		for(i=4; i<nDataLen; i++)
			oFltData[i] =( b[0]*iSrcData[i]+b[1]*iSrcData[i-1]+b[2]*iSrcData[i-2]+b[3]*iSrcData[i-3]+b[4]*iSrcData[i-4] - a[1]*oFltData[i-1]-a[2]*oFltData[i-2]-a[3]*oFltData[i-3]-a[4]*oFltData[i-4] ) / a[0];
	}

	else if(rank==5)//��Ϊ5��
	{
		b[0]=1;b[1]=5;b[2]=10;b[3]=10;b[4]=5;b[5]=1; 
		middle1=(float)(-2*cos(3*DP_PI/5));
		middle2=(float)(-2*cos(4*DP_PI/5));
		float a1[3]={Const*Const+middle1*Const+1, -2*Const*Const+2, Const*Const-middle1*Const+1};
		float a2[3]={Const*Const+middle2*Const+1, -2*Const*Const+2, Const*Const-middle2*Const+1};
		float a3[5]={a1[0]*a2[0], a1[0]*a2[1]+a1[1]*a2[0], a1[0]*a2[2]+a1[1]*a2[1]+a1[2]*a2[0], a1[1]*a2[2]+a1[2]*a2[1], a1[2]*a2[2]};//��׵�ϵ��

		a[0]=a3[0]*(Const+1);
		a[1]=-a3[0]*(Const-1)+a3[1]*(Const+1);
		a[2]=-a3[1]*(Const-1)+a3[2]*(Const+1);
		a[3]=-a3[2]*(Const-1)+a3[3]*(Const+1);
		a[4]=-a3[3]*(Const-1)+a3[4]*(Const+1);
		a[5]=-a3[4]*(Const-1);

		oFltData[0]= iSrcData[0];
		oFltData[1]= iSrcData[1];
		oFltData[2]= iSrcData[2];
		oFltData[3]= iSrcData[3];
		oFltData[4]= iSrcData[4];
		for(i=5; i<nDataLen; i++)
			oFltData[i] =( b[0]*iSrcData[i]+b[1]*iSrcData[i-1]+b[2]*iSrcData[i-2]+b[3]*iSrcData[i-3]+b[4]*iSrcData[i-4]+b[5]*iSrcData[i-5] - a[1]*oFltData[i-1]-a[2]*oFltData[i-2]-a[3]*oFltData[i-3]-a[4]*oFltData[i-4]-a[5]*oFltData[i-5] ) / a[0];
	}

}
/////////////////////////////////////////////////////////////////
// ��ֵ // 2007-12-08
////////////////////////////////////////////////////////////////////////////////
//  �������� : ����Ԥ��������ֵ
//  ������� : 
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� :                  ԭʼ����
//                              ����[OUT]
//                              ���ݳ���
////////////////////////////////////////////////////////////////////////////////
float DP_InsertData(float y[],float x0,float h,int n,float t)
{ 
	int i,j,k,m,l;
    float z,xx[15],yy[15];
	
    z=0.0;
    if (n<1) return(z);
    if (n==1) 
	{ 	z=y[0]; 
	return(z);
	}
    m=15;
    if (m>n) m=n;
    if (t<=x0) k=1;
    else if (t>=x0+(n-1)*h) k=n;
    else
    { 
		k=1; j=n;
        while ((k-j!=1)&&(k-j!=-1))
        { 
			l=(k+j)/2;
            if (t<x0+(l-1)*h) j=l;
            else k=l;
        }
        if (DP_ABS(t-(x0+(l-1)*h))>DP_ABS(t-(x0+(j-1)*h))) k=j;
    }
    j=1; l=0;
    for (i=1;i<=m;i++)    { 
		k=k+j*l;
        if ((k<1)||(k>n))
        { l=l+1; j=-j; k=k+j*l;}
        xx[i-1]=x0+(k-1)*h; yy[i-1]=y[k-1];
        l=l+1; j=-j;
    }
	
    i=0;
    do
    { 
		i=i+1; z=yy[i];
        for (j=0;j<=i-1;j++)
			z=yy[j]+(t-xx[j])*(yy[j]-z)/(xx[j]-xx[i]);
        yy[i]=z;
    }
    while ((i!=m-1)&&(DP_ABS(yy[i]-yy[i-1])>DP_EPS));
    return(z);
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ����Ԥ��������ֵ
//  ������� : 
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� :                  ԭʼ����
////////////////////////////////////////////////////////////////////////////////
int DP_RawAngleHdlbyCntur( int nRawPnt, ANGLE_RAW_DATA *pRawData, int iMaxFramePerSec, float fFreq, int &nMaxPnt, ANGLE_HDL_DATA *pMaxPnt, int &nMinPnt, ANGLE_HDL_DATA *pMinPnt )
{
	float T=1/fFreq;      //����
	int n_PntperT =(int) (iMaxFramePerSec*T);
	
	float min = pRawData->data2[0];
	float max = pRawData->data2[0];
	int i=0,j=0;
	int a=0,kk=0,nMax=0,nMin=0;
	int t_min,t_max;
	
	float time_min[DP_MAXPOINT],time_max[DP_MAXPOINT];        //����ʱ�伫ֵ
	float value_min[DP_MAXPOINT],value_max[DP_MAXPOINT];		//�������߼�ֵ
	
	float time_min_temp[DP_MAXPOINT],time_max_temp[DP_MAXPOINT];     //�м����ݴ���  ����ʱ�伫��ֵ
	float value_min_temp[DP_MAXPOINT],value_max_temp[DP_MAXPOINT];   //�м����ݴ���  �������߼���ֵ
	
	float min_pNedObj_temp[DP_MAXPOINT],max_pNedObj_temp[DP_MAXPOINT];     ////�м����ݴ���  �����ֵ���߼���ֵ

	for(i=0;i<nRawPnt;)
	{
		min = pRawData->data2[i];
		t_min =i;
		max = pRawData->data2[i];
		t_max =i;
		
		for(j=0;j<n_PntperT;j++)
		{
			a=i+j;
			if(a>nRawPnt-1) break;
			
			if(pRawData->data2[a]<min) 
			{
				min=pRawData->data2[a];
				t_min=a;
			}
			if(pRawData->data2[a]>max) 
			{
				max=pRawData->data2[a];
				t_max=a;
			}
		}
		
		time_min_temp[kk]= pRawData->time[t_min];            //��Сֵ��ʱ���
		value_min_temp[kk]= pRawData->data1[t_min];
		time_max_temp[kk]= pRawData->time[t_max];
		value_max_temp[kk]= pRawData->data1[t_max];
		
		min_pNedObj_temp[kk]= pRawData->data2[t_min];
		max_pNedObj_temp[kk]= pRawData->data2[t_max];
		
		kk++;
		i+=n_PntperT;
	}

	////////////////  ɾ���ظ���  /////////////////////////////
	T/=2;

	for(i=0;i<kk-1;i++)    //ȥ���ظ������
	{
		if((time_max_temp[i+1]-time_max_temp[i])<T)
		{
			if(max_pNedObj_temp[i]>max_pNedObj_temp[i+1]) 
			{
				time_max_temp[i+1]=-1;
				i++;
			}
			else
			{
				time_max_temp[i]=-1;
			}
		}
		
	}
		
	for(i=0;i<kk;i++)
	{
		if((time_max_temp[i]+1)<0.0001) continue;
		time_max[nMax]=time_max_temp[i];
		value_max[nMax]=value_max_temp[i];
		nMax++;
	}

	for(i=0;i<kk-1;i++)    //ȥ���ظ���С��
	{
		if((time_min_temp[i+1]-time_min_temp[i])<T)
		{
			if(min_pNedObj_temp[i]>min_pNedObj_temp[i+1]) 
			{
				time_min_temp[i+1]=-1;
				i++;
			}
			else
			{
				time_min_temp[i]=-1;
				i++;
			}
		}
		
	}
	
	for(i=0;i<kk;i++)
	{
		if((time_min_temp[i]+1)<0.001) continue;
		time_min[nMin]=time_min_temp[i];
		value_min[nMin]=value_min_temp[i];
		nMin++;
	}

			
	////////////////////////////////////////////////////////////////////////////
	
	float chn_time[DP_NBCHNPOINT];     //����ͻ����ʱ����
	float chn_value[DP_NBCHNPOINT];	//����ͻ������������
	
	int nTime=0;					//ÿ����ֵ���ڵ�ͻ������
	int flag[DP_NBSAVX];               //��־
	for(i=0;i<DP_NBSAVX;i++)
	{
		flag[i]=0;
	}
	
	for(i=1;i<=nRawPnt;i++)		//����ͻ���,��������ͻ��㣬������������
	{
		if((pRawData->data1[i]-pRawData->data1[i-1])>DP_NUMCHNPOINT)
		{
			flag[i]=2;				//�ҵ�ͻ��㣬flag=2
		}
		else if((pRawData->data1[i-1]-pRawData->data1[i])>DP_NUMCHNPOINT)
		{
			flag[i]=2;
		}
	}

	///////////////////  ��ֵ����  /////////////////////////////
	float tt=0.01f,t1;
	int biaozhi=0;

	float array[2];
	for (i=0;i<2;i++)
	{
		array[i]=0;
	}
	
	nMaxPnt=0;          //�����ļ���ֵ��ĸ���
	for(i=0;i<nMax-1;i++)
	{
		biaozhi=0;      //biaozhi����
		nTime=0;        //nTime����
		float h=time_max[i+1]-time_max[i];
		
		for(j=0;j<nRawPnt-1;j++)
		{
			if(flag[j]==2)
			{
				if(pRawData->time[j]>time_max[i]&&pRawData->time[j]<time_max[i+1])
				{
					chn_time[nTime]=pRawData->time[j];
					chn_value[nTime]=pRawData->data1[j];
					nTime++;
					biaozhi=1;
				}
			}
		}
		if(nTime!=0) //or biaozhi==1
		{
			chn_time[nTime]=time_max[i+1];
			nTime++;
		}
		
		if(biaozhi==0)
		{	
			array[0]=value_max[i];
			array[1]=value_max[i+1];
		}
		if(biaozhi==1)
		{	
			array[0]=value_max[i];
			array[1]=value_max[i+1];
			if((array[1]-array[0])>DP_NUMCHNPOINT) array[1]-=360;
			else if((array[0]-array[1])>DP_NUMCHNPOINT) array[1]+=360;
		}
		
		for(j=0;;j++)
		{
			t1=time_max[i]+j*tt;
			if(fabs(t1-time_max[i+1])<0.0001) break;
			
			if(fabs(t1-time_max[i])<0.0001)        //�������
			{
				pMaxPnt->time[nMaxPnt]= time_max[i];
				pMaxPnt->data[nMaxPnt]= value_max[i];
			}
			else
			{
				if(biaozhi==0)
				{
					pMaxPnt->time[nMaxPnt]= t1;
					pMaxPnt->data[nMaxPnt]= DP_InsertData(array,time_max[i],h,2,t1);
				}
				if(biaozhi==1)
				{
					pMaxPnt->time[nMaxPnt]= t1;
					
					for(int m=0;m<nTime-1;m++)
					{
						if(t1<chn_time[0]&&(fabs(t1-chn_time[0])>0.0001)) 
						{
							pMaxPnt->data[nMaxPnt]= DP_InsertData(array,time_max[i],h,2,t1);
							break;
						}
						
						if((t1>chn_time[m]&&t1<chn_time[m+1]&&(fabs(t1-chn_time[m+1])>0.0001))
							||fabs(t1-chn_time[m])<0.0001)
						{
							if((chn_value[m]-array[0])>DP_NUMCHNPOINT)
							{
								pMaxPnt->data[nMaxPnt]= DP_InsertData(array,time_max[i],h,2,t1)+360;
								break;
							}
							else if((array[0]-chn_value[m])>DP_NUMCHNPOINT)
							{
								pMaxPnt->data[nMaxPnt]= DP_InsertData(array,time_max[i],h,2,t1)-360;
								break;
							}
							else
							{
								pMaxPnt->data[nMaxPnt]= DP_InsertData(array,time_max[i],h,2,t1);
								break;
							}
						}
					}
				}
			}
			
			nMaxPnt++;
		}
	}

	/////////////// ��Сֵ //////////////////////////////////

	nMinPnt=0;          //�����ļ�Сֵ��ĸ���
	for (i=0;i<2;i++)
	{
		array[i]=0;
	}
	
	for(i=0;i<nMin-1;i++)
	{
		biaozhi=0;      //biaozhi����
		nTime=0;        //nTime����
		float h=time_min[i+1]-time_min[i];
		
		for(j=0;j<nRawPnt-1;j++)
		{
			if(flag[j]==2)
			{
				if(pRawData->time[j]>time_min[i]&&pRawData->time[j]<time_min[i+1])
				{
					chn_time[nTime]=pRawData->time[j];
					chn_value[nTime]=pRawData->data1[j];
					nTime++;
					biaozhi=1;
				}
			}
		}
		if(nTime!=0) //or biaozhi==1
		{
			chn_time[nTime]=time_min[i+1];
			nTime++;
		}
		
		if(biaozhi==0)
		{	
			array[0]=value_min[i];
			array[1]=value_min[i+1];
		}
		if(biaozhi==1)
		{	
			array[0]=value_min[i];
			array[1]=value_min[i+1];
			if((array[1]-array[0])>DP_NUMCHNPOINT) array[1]-=360;
			else if((array[0]-array[1])>DP_NUMCHNPOINT) array[1]+=360;
		}
		
		for(j=0;;j++)
		{
			t1=time_min[i]+j*tt;
		
			if(fabs(t1-time_min[i+1])<0.0001) break;

			if(fabs(t1-time_min[i])<0.0001)        //�������
			{
				pMinPnt->time[nMinPnt]= time_min[i];
				pMinPnt->data[nMinPnt]= value_min[i];
			}
			else
			{
				if(biaozhi==0)
				{
					pMinPnt->time[nMinPnt]= t1;
					pMinPnt->data[nMinPnt]= DP_InsertData(array,time_min[i],h,2,t1);
				}
				if(biaozhi==1)
				{
					pMinPnt->time[nMinPnt]= t1;
					
					for(int m=0;m<nTime-1;m++)
					{
						if(t1<chn_time[0]&&(fabs(t1-chn_time[0])>0.0001)) 
						{
							pMinPnt->data[nMinPnt]= DP_InsertData(array,time_min[i],h,2,t1);
							break;
						}
						

						if((t1>chn_time[m]&&t1<chn_time[m+1]&&(fabs(t1-chn_time[m+1])>0.0001))
							||fabs(t1-chn_time[m])<0.0001)
						{
							if((chn_value[m]-array[0])>DP_NUMCHNPOINT)
							{
								pMinPnt->data[nMinPnt]= DP_InsertData(array,time_min[i],h,2,t1)+360;
								break;
							}
							else if((array[0]-chn_value[m])>DP_NUMCHNPOINT)
							{
								pMinPnt->data[nMinPnt]= DP_InsertData(array,time_min[i],h,2,t1)-360;
								break;
							}
							else
							{
								pMinPnt->data[nMinPnt]= DP_InsertData(array,time_min[i],h,2,t1);
								break;
							}
						}
					}
				}
			}
			
			nMinPnt++;
		}
	}
	
	return 1;         
}

////  �������


////////////////////////////////////////////////////////////////////////////////
//  �������� : ����Ԥ�����������������
//  ������� : 
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� :                  ԭʼ����
////////////////////////////////////////////////////////////////////////////////
int DP_JumpPointHdl( int nPnt, float *pOutAngl, float *pInAngl, float MaxAnglDiff )
{
    int ii=0,jj=0;
    float Diff = 0.0,MultVal=0.0;
    int JumpNum,IdxStart=0,IdxEnd=0;
    int JumpIdx[DP_MAXPOINT];

    memset((void*)&JumpIdx, 0, DP_MAXPOINT*sizeof(int));

    for(ii=0; ii<nPnt; ii++)
    {
        pOutAngl[ii] = pInAngl[ii];
    }
    // �õ�ͻ����±�
    JumpNum = 0;
    for(ii=0; ii<nPnt-1; ii++)
    {
        Diff = pOutAngl[ii] - pOutAngl[ii+1];  // ��������ǶȲ�
        if( fabs(Diff) > MaxAnglDiff )
        {
            JumpIdx[JumpNum] = ii+1;  // ��¼��һ�±�
            JumpNum = JumpNum + 1;
        }
    }

    JumpIdx[JumpNum] = nPnt;

    //  ���ǶȲ����ͻ��
    for(ii=0; ii<JumpNum; ii++)
    {
        IdxStart = JumpIdx[ii];
        IdxEnd   = JumpIdx[ii+1];

        Diff = pOutAngl[IdxStart-1] - pOutAngl[IdxStart];
        if( Diff > MaxAnglDiff )
        {
            MultVal = (float)(((int)(Diff/360.0)+1)*360.0);
        }
        else if( Diff < -MaxAnglDiff )
        {
            MultVal = (float)(((int)(Diff/360.0)-1)*360.0);
        }
        else
        {
            MultVal = 0.0;
        }

        for( jj=IdxStart; jj<IdxEnd; jj++)
        {
            pOutAngl[jj] = pOutAngl[jj]+MultVal;
        }
    }

    return 0;
}

