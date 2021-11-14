/******************************************************************************
	ModuleName :   LFO Analysis based on curve 
	FileName   :   LFOAnaByCurve.h
	Description:   
	HISTORY    :
	   Date        Action        Author                Description 
	----------------------------------------------------------------------
	2007.07.08     Creation        lsf           
******************************************************************************/
#include "LFOAnaByCurve.h"
//#include "powoscana/lfo_ana_obj.h"
#include "db_def/sys_menu_info.h"
CLFOAnaByCurve::CLFOAnaByCurve()
{
	// printf("CLFOAnaByCurve���쿪ʼ\n");
}

CLFOAnaByCurve::~CLFOAnaByCurve()
{
	
}
extern void g_PubLogMessShow(const char *log, const int logtype = 1);
void CLFOAnaByCurve::MessShow( const char *log )
{
	//printf( "%s\n", log );
	g_PubLogMessShow(log);
	
	return;
}
////////////////////////////////////////////////////////////////////////////////
//  �������� : ����MODE_PART_OBJ�ṹ
//  ������� : 
//  ��    ע : 
//  �� �� ֵ : 
//  ����˵�� : ipMode                   ����ģʽ
//             opMode                   ���ģʽ
////////////////////////////////////////////////////////////////////////////////
void CLFOAnaByCurve::MemCpyModePartObjStru( MODE_PART_OBJ ipMode, MODE_PART_OBJ &opMode )
{
	//if ( !ipMode || !opMode ) return;
	
	opMode.obj_id = ipMode.obj_id;
	strcpy(opMode.obj_name, ipMode.obj_name);
	opMode.type = ipMode.type;
	opMode.damp = ipMode.damp;
	opMode.freq = ipMode.freq;
	opMode.ampl = ipMode.ampl;
	opMode.phas = ipMode.phas;
	opMode.mgnt = ipMode.mgnt;
	opMode.real = ipMode.real;
	opMode.imag = ipMode.imag;
	opMode.fact = ipMode.fact;
	opMode.engy = ipMode.engy;
	opMode.clus = ipMode.clus;  //2008-09-24 add
	
	return ;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : �������豸�Ĳ���������0
//  ������� : �����������ǰ����Tj����factλ������������Ӽ�����󣬽�factλ��0
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : iGnNum                  ������[in]
//             PartGn[]                ���[in]
////////////////////////////////////////////////////////////////////////////////
void CLFOAnaByCurve::LfoAnaSetPartFact0(  int nPartGn, MODE_PART_OBJ *pPartGn )
{ 
	if ( nPartGn<0 || !pPartGn ) return ;
	
	int  ii;
	for ( ii=0; ii<nPartGn; ii++)
	{
		pPartGn[ii].fact = 0;
	}
	
	return ;
}
////////////////////////////////////////////////////////////////////////////////
//  �������� : ��������LFO�жϣ��ѵ㣺 LFO�оݣ���ֹʱ���оݣ�
//  ������� : 
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : iSrcData                 ԭʼ����
//             oFltData                 �˲�����[OUT]
//             nDataLen                 ���ݳ���
//             rank                     �˲�������(2 - 5)
////////////////////////////////////////////////////////////////////////////////
//int CLFOAnaByCurve::EstimateSglObjLfo()
//{
//
//	return 0;
//}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ʶ����Ҫģʽ
//  ������� : 
//  ��    ע : 
//  �� �� ֵ : 
//  ����˵�� : inMode                   ԭģʽ��
//             ipMode                   ԭģʽ
//             onMode                   ���ģʽ��
//             opMode                   ���ģʽ
////////////////////////////////////////////////////////////////////////////////
void CLFOAnaByCurve::IdtfMainMode( const int inMode, MODE_PART_OBJ *ipMode, int &onMode, MODE_PART_OBJ *opMode )
{
	if ( !ipMode || !opMode || inMode <= 0 )
	{
		onMode = 0;
		return;
	}
	
	// temp  // ��Ҫ�޸ģ�Prony�еĹ��˹����ڴ˺�����ʵ��
	//////////////////////////////////////////////////////////////////////////
	int ii;
	if (inMode < OBJ_MODE_MAX)
	{
		onMode = inMode;
		for (ii=0; ii<onMode; ii++)
		{
			//memcpy( (void*)&opMode[ii].obj_id, (void*)&ipMode[ii].obj_id, dataSize );
			MemCpyModePartObjStru( ipMode[ii], opMode[ii] );
		}
	}
	else
	{
		onMode = OBJ_MODE_MAX;
		for (ii=0; ii<onMode; ii++)
		{
			//memcpy( (void*)&opMode[ii].obj_id, (void*)&ipMode[ii].obj_id, dataSize );
			MemCpyModePartObjStru( ipMode[ii], opMode[ii] );
		}
	}
	//////////////////////////////////////////////////////////////////////////

	return ;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ʶ����Ҫģʽ(�����ͬһ�����е����ģʽ�磺0.202 0.205)
//  ������� : ����ȹ��ˡ�Ƶ�ʷ�Χ���ˡ���������
//  ��    ע : 
//  �� �� ֵ : 
//  ����˵�� : inMode                   ԭģʽ��
//             ipMode                   ԭģʽ
//             onMode                   ���ģʽ��
//             opMode                   ���ģʽ
////////////////////////////////////////////////////////////////////////////////
void CLFOAnaByCurve::IdtfMainMode( const int inMode, MODE_PART_OBJ *ipMode, int &onMode, MODE_PART_OBJ *opMode, float FreqDif, float DampDif )
{
	if ( !ipMode || !opMode || inMode <= 0 )
	{
		onMode = 0;
		return;
	}
	
	// temp  // ��Ҫ�޸ģ�Prony�еĹ��˹����ڴ˺�����ʵ��
	//////////////////////////////////////////////////////////////////////////
	int  ii, jj;
	bool findSamModeFlag = false;
	
	onMode = 0;
	if (inMode < OBJ_MODE_MAX)
	{
		//onMode = inMode;
		for (ii=0; ii<inMode; ii++)
		{
			findSamModeFlag = false;
			if (ii == 0)
			{
				MemCpyModePartObjStru( ipMode[ii], opMode[ii] );
				onMode = 1;
			}
			else
			{
				for (jj=0; jj<onMode; jj++)
				{
					if ( IdtfSameMode( ipMode[ii].freq, ipMode[ii].damp, opMode[jj].freq, opMode[jj].damp, FreqDif, DampDif) == true ) 
					{
						findSamModeFlag = true;
						break;
					}
				}
				if (findSamModeFlag == false)
				{
					MemCpyModePartObjStru( ipMode[ii], opMode[onMode] );
					onMode += 1;					
				}
			}
		}
	}
	else
	{
		//onMode = OBJ_MODE_MAX;
		for (ii=0; ii<OBJ_MODE_MAX; ii++)
		{
			findSamModeFlag = false;
			if (ii == 0)
			{
				MemCpyModePartObjStru( ipMode[ii], opMode[ii] );
				onMode = 1;
			}
			else
			{
				for (jj=0; jj<onMode; jj++)
				{
					if ( IdtfSameMode( ipMode[ii].freq, ipMode[ii].damp, opMode[jj].freq, opMode[jj].damp, FreqDif, DampDif) == true ) 
					{
						findSamModeFlag = true;
						break;
					}
				}
				if (findSamModeFlag == false)
				{
					MemCpyModePartObjStru( ipMode[ii], opMode[onMode] );
					onMode += 1;
				}
			}
			//MemCpyModePartObjStru( ipMode[ii], opMode[ii] );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	
	return ;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ��ͬģʽ�жϣ�����ģʽ������(Real, Imag)/(freq, damp)��
//  ������� : 
//  ��    ע : �����������������ƣ��� //�ο�ģʽѡȡӰ��ϴ�
//  �� �� ֵ : true ��ͬ; false ��ͬ
//  ����˵�� : iFreq1,  iDamp1          ģʽ1����
//             iFreq2,  iDamp2          ģʽ2����
//             FreqDif, DampDif         �����ֵ
////////////////////////////////////////////////////////////////////////////////
bool CLFOAnaByCurve::IdtfSameMode(float iFreq1, float iDamp1, float iFreq2, float iDamp2, float FreqDif, float DampDif)
{
	if(FreqDif < 0 || DampDif < 0) return false;

	if( fabs(iFreq1-iFreq2) < FreqDif )
	{
		if( fabs(iDamp1-iDamp2) < DampDif )
			return true;
		else
			return false;
	}
	else
		return false;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ʶ��������ɹ���·
//  ������� : 
//  ��    ע : (��Ҫ�Ľ���1������֪��ģʽ����������Ϊ��׼��2����Ҫ�޳�ͬһ�豸������ģʽ��)
//  �� �� ֵ : 
//  ����˵�� : nObjMode                 ��ѡģʽ��
//             pObjMode                 ��ѡģʽ
//             nMode                    ģʽ��
//             pMode                    ģʽ
////////////////////////////////////////////////////////////////////////////////
void CLFOAnaByCurve::IdtfPartGnPartLn( int nObjMode, MODE_PART_OBJ *pObjMode, int &nMode, MODE_CHRCT *pMode, float FreqDif, float DampDif )
{
	if( !pObjMode || !pMode || nObjMode <= 0) return ;

	int  ii, jj;
	int  tempModeNum     = 0;
	int  tempGnNun       = 0;
	int  tempLnNun       = 0;
	int  tempFcNun       = 0;
	int  tempTrNun       = 0;
	bool findSamModeFlag = false; 

	if (tempModeNum > SYS_MODE_MAX) return ;
		
	// first mode
	//pMode[tempModeNum].mode_id = tempModeNum + 1;
	pMode[tempModeNum].freq    = pObjMode[0].freq;
	pMode[tempModeNum].damp    = pObjMode[0].damp;
	pMode[tempModeNum].real    = pObjMode[0].real;
	pMode[tempModeNum].imag    = pObjMode[0].imag;
	if (pMode[tempModeNum].freq >= IDTF_TYPE_FREQ) 
		pMode[tempModeNum].mode_id = IDTF_TYPE_LOCL;
	else
		pMode[tempModeNum].mode_id = IDTF_TYPE_INTR;
	
	pMode[tempModeNum].nPartGn = 0;
	pMode[tempModeNum].nPartLn = 0;
	pMode[tempModeNum].nPartFc = 0;
	// judge objecte type
	if ( pObjMode[0].type == OBJ_TYPE_GN )
	{
		tempGnNun       = pMode[tempModeNum].nPartGn;
		MemCpyModePartObjStru( pObjMode[0], pMode[tempModeNum].PartGn[tempGnNun] );
		pMode[tempModeNum].nPartGn = tempGnNun + 1;
	}
	else if ( pObjMode[0].type == OBJ_TYPE_LN ) 
	{
		tempLnNun       = pMode[tempModeNum].nPartLn;
		MemCpyModePartObjStru( pObjMode[0], pMode[tempModeNum].PartLn[tempLnNun] );
		pMode[tempModeNum].nPartLn = tempLnNun + 1;
	}
	// 2007-11-25 lsf add
	else if ( pObjMode[0].type == OBJ_TYPE_FC ) 
	{
		tempFcNun       = pMode[tempModeNum].nPartFc;
		MemCpyModePartObjStru( pObjMode[0], pMode[tempModeNum].PartFc[tempFcNun] );
		pMode[tempModeNum].nPartFc = tempFcNun + 1;
	}
	// 2008-09-24 add
	else if ( pObjMode[0].type == OBJ_TYPE_TR ) 
	{
		tempTrNun       = pMode[tempModeNum].nPartTr;
		MemCpyModePartObjStru( pObjMode[0], pMode[tempModeNum].PartTr[tempTrNun] );
		pMode[tempModeNum].nPartTr = tempTrNun + 1;
	}
	else
	{
		//tempModeNum = 0;    // ������ �������·�����豸
		//return ;
	}
	tempModeNum += 1;
	
	// judge objecte
	for (ii=1; ii<nObjMode; ii++)
	{
		findSamModeFlag = false;
		for (jj=0; jj<tempModeNum; jj++)
		{
			if ( IdtfSameMode( pMode[jj].freq, pMode[jj].damp, pObjMode[ii].freq, pObjMode[ii].damp, FreqDif, DampDif) == true ) 
			{
				if ( pObjMode[ii].type == OBJ_TYPE_GN )
				{
					tempGnNun       = pMode[jj].nPartGn;
					MemCpyModePartObjStru( pObjMode[ii], pMode[jj].PartGn[tempGnNun] );
					pMode[jj].nPartGn = tempGnNun + 1;
				}
				else if ( pObjMode[ii].type == OBJ_TYPE_LN ) 
				{
					tempLnNun       = pMode[jj].nPartLn;
					MemCpyModePartObjStru( pObjMode[ii], pMode[jj].PartLn[tempLnNun] );
					pMode[jj].nPartLn = tempLnNun + 1;
				}
				// 2007-11-25 lsf add
				else if ( pObjMode[ii].type == OBJ_TYPE_FC ) 
				{
					tempFcNun       = pMode[jj].nPartFc;
					MemCpyModePartObjStru( pObjMode[ii], pMode[jj].PartFc[tempFcNun] );
					pMode[jj].nPartFc = tempFcNun + 1;
				}
				// 2007-09-24 add
				else if ( pObjMode[ii].type == OBJ_TYPE_TR ) 
				{
					tempTrNun       = pMode[jj].nPartTr;
					MemCpyModePartObjStru( pObjMode[ii], pMode[jj].PartTr[tempTrNun] );
					pMode[jj].nPartTr = tempTrNun + 1;
				}
				else
				{
					//tempModeNum = 0;  // ������ �������·�����豸
					//return ;
				}

				findSamModeFlag = true;
				break;
			}
		}

		if (findSamModeFlag == false && tempModeNum < SYS_MODE_MAX) // �������ģʽ������ 2007-09-26
		{
			//pMode[tempModeNum].mode_id = tempModeNum + 1;
			pMode[tempModeNum].freq    = pObjMode[ii].freq;
			pMode[tempModeNum].damp    = pObjMode[ii].damp;
			pMode[tempModeNum].real    = pObjMode[ii].real;
			pMode[tempModeNum].imag    = pObjMode[ii].imag;
			if (pMode[tempModeNum].freq >= IDTF_TYPE_FREQ) 
				pMode[tempModeNum].mode_id = IDTF_TYPE_LOCL;
			else
				pMode[tempModeNum].mode_id = IDTF_TYPE_INTR;
			
			pMode[tempModeNum].nPartGn = 0;
			pMode[tempModeNum].nPartLn = 0;
			pMode[tempModeNum].nPartFc = 0;
			pMode[tempModeNum].nPartTr = 0; //2008-09-24 add
			// judge objecte type
			if ( pObjMode[ii].type == OBJ_TYPE_GN )
			{
				tempGnNun       = pMode[tempModeNum].nPartGn;
				MemCpyModePartObjStru( pObjMode[ii], pMode[tempModeNum].PartGn[tempGnNun] );
				pMode[tempModeNum].nPartGn = tempGnNun + 1;
			}
			else if ( pObjMode[ii].type == OBJ_TYPE_LN ) 
			{
				tempLnNun       = pMode[tempModeNum].nPartLn;
				MemCpyModePartObjStru( pObjMode[ii], pMode[tempModeNum].PartLn[tempLnNun] );
				pMode[tempModeNum].nPartLn = tempLnNun + 1;
			}
			else if ( pObjMode[ii].type == OBJ_TYPE_FC ) 
			{
				tempFcNun       = pMode[tempModeNum].nPartFc;
				MemCpyModePartObjStru( pObjMode[ii], pMode[tempModeNum].PartFc[tempFcNun] );
				pMode[tempModeNum].nPartFc = tempFcNun + 1;
			}
			//2008-09-24 add
			else if ( pObjMode[ii].type == OBJ_TYPE_TR ) 
			{
				tempTrNun       = pMode[tempModeNum].nPartTr;
				MemCpyModePartObjStru( pObjMode[ii], pMode[tempModeNum].PartTr[tempTrNun] );
				pMode[tempModeNum].nPartTr = tempTrNun + 1;
			}
			else
			{
				//tempModeNum = 0;  // ������ �������·�����豸
				//return ;
			}
			tempModeNum += 1;
		}
	}
	nMode = tempModeNum;
	for(ii=0; ii<nMode; ii++)
	{
		pMode[ii].mode_id = ii + 1;
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ģʽ����ȽϺ��� ��������engyLn�ɴ�С��������engyGn�ɴ�С,
//             ���迼�ǿɹ۷�ΧӰ��,
////////////////////////////////////////////////////////////////////////////////
extern "C" int ModeCompareByengy(const void * p1, const void * p2)
{
    if ( ((MODE_CHRCT *)p1)->engyLn != ((MODE_CHRCT *)p2)->engyLn )
    {
		return ((MODE_CHRCT *)p1)->engyLn < ((MODE_CHRCT *)p2)->engyLn ? 1 : -1;
    }
	
    return ((MODE_CHRCT *)p1)->engyGn < ((MODE_CHRCT *)p2)->engyGn ? 1 : -1;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : �����������ƽ������,����ɹ���·��ƽ������
//  ������� : engyGn=(nPartGn | ��engy)/nPartGn
//             engyLn=(nPartLn | ��engy)/nPartLn
//             real=(nPartLn+nPartGn | ��real)/(nPartLn+nPartGn)
//             imag=(nPartLn | ��imag)/(nPartLn+nPartGn)
//             freq=imag/(2*pi)
//             damp=-real/sqrt(real*real+imag*imag)
//  �� �� ֵ :
//  ����˵�� : nMode                    ģʽ��
//             pMode                    ģʽ
////////////////////////////////////////////////////////////////////////////////
void CLFOAnaByCurve::CalcModeGnLnEngy( int nMode, MODE_CHRCT *pMode )
{
	if ( nMode <= 0 || !pMode ) return ;
	
	int   ii, jj;
	int   nPart = 0;
	float fEngy = 0;
	float fReal = 0;
	float fImag = 0;
	
	for ( ii=0; ii<nMode; ii++ )
	{
		// calculate engyGn
		nPart = 0;
		fEngy = 0;
		fReal = 0;
		fImag = 0;
		if ( pMode[ii].nPartGn > 0 ) 
		{
			for (jj=0; jj<pMode[ii].nPartGn; jj++) 
			{
				fEngy += pMode[ii].PartGn[jj].engy;

				fReal += pMode[ii].PartGn[jj].real;
				fImag += pMode[ii].PartGn[jj].imag;
			}
			fEngy = fEngy/pMode[ii].nPartGn;
		}
		pMode[ii].engyGn = fEngy;
		
		// calculate engyLn
		fEngy = 0;
		if ( pMode[ii].nPartLn > 0 ) 
		{
			for (jj=0; jj<pMode[ii].nPartLn; jj++) 
			{
				fEngy += pMode[ii].PartLn[jj].engy;
				
				fReal += pMode[ii].PartLn[jj].real;
				fImag += pMode[ii].PartLn[jj].imag;
			}
			fEngy = fEngy/pMode[ii].nPartLn;
		}
		pMode[ii].engyLn = fEngy;

		nPart = pMode[ii].nPartGn+pMode[ii].nPartLn;
		if ( nPart > 0 ) 
		{
			pMode[ii].real = fReal/nPart;
			pMode[ii].imag = fImag/nPart;
			pMode[ii].freq = (float)(pMode[ii].imag/(2*PRONY_PI));
			pMode[ii].damp = (float)(-pMode[ii].real/sqrt(pMode[ii].real*pMode[ii].real+pMode[ii].imag*pMode[ii].imag)*100);
		}
		else
		{
			pMode[ii].real = fReal;
			pMode[ii].imag = fImag;
			pMode[ii].freq = 0;
			pMode[ii].damp = 0;
		}
		
	}
	
	// sort by pMode[ii].engyLn
	qsort((void *)pMode, nMode, sizeof(MODE_CHRCT), ModeCompareByengy);

	for(ii=0; ii<nMode; ii++)
	{
		pMode[ii].mode_id = ii + 1;
	}
	
	return ;
}
////////////////////////////////////////////////////////////////////////////////
//  �������� : ʶ���񵴻��顢����·����ѹ������վ
//  ������� : 
//  ��    ע : (��Ҫ�Ľ���1������֪��ģʽ����������Ϊ��׼)
//  ��    �� ��nObjMode,pObjMode
//  ��    �� ��nMode,pMode
//  �� �� ֵ : 
//  ����˵�� : nObjMode                 ��ѡģʽ��
//             pObjMode                 ��ѡģʽ
//             nMode                    ģʽ��
//             pMode                    ģʽ
////////////////////////////////////////////////////////////////////////////////
void CLFOAnaByCurve::IdtfPartGnLnTr(int nObjMode,MODE_PART_OBJ *pObjMode,int &nMode,MODE_CHRCT *pMode)
{

	if( !pObjMode || !pMode || nObjMode <= 0) return ;

	int  ii, jj;
	int  tempModeNum     = 0;
	int  tempGnNun       = 0;
	int  tempLnNun       = 0;
	int  tempFcNun       = 0;
	int  tempTrNun       = 0;
	bool findSamModeFlag = false;
	char LogString[1024];
	pMode[tempModeNum].nPartGn = 0;
	pMode[tempModeNum].nPartLn = 0;
	pMode[tempModeNum].nPartFc = 0;
	pMode[tempModeNum].nPartTr = 0; 
	for (ii=0; ii<nObjMode; ii++)
	{
//		if (pObjMode[ii].freq >= IDTF_TYPE_FREQ) 
//			pMode[tempModeNum].mode_id = IDTF_TYPE_LOCL;
//		else
//		pMode[tempModeNum].mode_id = IDTF_TYPE_INTR;
// 		sprintf( LogString, "��ʾ:��[%d]���豸[%s]������[%d],nPartGn=%d,nPartLn=%d,nPartFc=%d,nPartTr=%d",ii+1,pObjMode[ii].obj_name,pObjMode[ii].type,pMode[tempModeNum].nPartGn,pMode[tempModeNum].nPartLn,pMode[tempModeNum].nPartFc,pMode[tempModeNum].nPartTr);
// 		MessShow( LogString );
		if ( pObjMode[ii].type == MENU_LFO_DEV_GEN )
		{
			tempGnNun       = pMode[tempModeNum].nPartGn;
			MemCpyModePartObjStru( pObjMode[ii], pMode[tempModeNum].PartGn[tempGnNun] );
			pMode[tempModeNum].nPartGn = tempGnNun + 1;
// 			sprintf( LogString, "��ʾ:1111111111111111111");
// 			MessShow( LogString );
		}
		else if ( pObjMode[ii].type == MENU_LFO_DEV_LINE ) 
		{
			tempLnNun       = pMode[tempModeNum].nPartLn;
			MemCpyModePartObjStru( pObjMode[ii], pMode[tempModeNum].PartLn[tempLnNun] );
			pMode[tempModeNum].nPartLn = tempLnNun + 1;
		}
		else if ( pObjMode[ii].type == MENU_LFO_DEV_FAC ) 
		{
			tempFcNun       = pMode[tempModeNum].nPartFc;
			MemCpyModePartObjStru( pObjMode[ii], pMode[tempModeNum].PartFc[tempFcNun] );
			pMode[tempModeNum].nPartFc = tempFcNun + 1;
		}
		else if ( pObjMode[ii].type == MENU_LFO_DEV_TR ) 
		{
			tempTrNun       = pMode[tempModeNum].nPartTr;
			MemCpyModePartObjStru( pObjMode[ii], pMode[tempModeNum].PartTr[tempTrNun] );
			pMode[tempModeNum].nPartTr = tempTrNun + 1;
		}
		else
		{

		}
	}
	sprintf( LogString, "��ʾ:tempGnNun=%d,tempLnNun=%d,tempTrNun=%d",tempGnNun,tempLnNun,tempTrNun);
	MessShow( LogString );
	nMode = pMode[tempModeNum].nPartGn+pMode[tempModeNum].nPartFc+pMode[tempModeNum].nPartLn+pMode[tempModeNum].nPartTr;
	return;
}
////////////////////////////////////////////////////////////////////////////////
//  �������� : ģ̬�ֲ���Ϣ����ȽϺ��� real�ɴ�С
////////////////////////////////////////////////////////////////////////////////
extern "C" int ModeShapeCompareByReal(const void * p1, const void * p2)
{
    if ( ((MODE_PART_OBJ *)p1)->real != ((MODE_PART_OBJ *)p2)->real )
    {
		return ((MODE_PART_OBJ *)p1)->real < ((MODE_PART_OBJ *)p2)->real ? 1 : -1;
    }
	
    return ((MODE_PART_OBJ *)p1)->fact < ((MODE_PART_OBJ *)p2)->fact ? 1 : -1;
//    if ( ((MODE_PART_OBJ *)p1)->fact != ((MODE_PART_OBJ *)p2)->fact )
//    {
//		return ((MODE_PART_OBJ *)p1)->fact < ((MODE_PART_OBJ *)p2)->fact ? 1 : -1;
//    }
//	
//    return ((MODE_PART_OBJ *)p1)->real < ((MODE_PART_OBJ *)p2)->real ? 1 : -1;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ģ̬�ֲ���Ϣ����ȽϺ��� fact�ɴ�С
////////////////////////////////////////////////////////////////////////////////
extern "C" int ModeShapeCompareByFact(const void * p1, const void * p2)
{
    if ( ((MODE_PART_OBJ *)p1)->fact != ((MODE_PART_OBJ *)p2)->fact )
    {
		return ((MODE_PART_OBJ *)p1)->fact < ((MODE_PART_OBJ *)p2)->fact ? 1 : -1;
    }
	
    return ((MODE_PART_OBJ *)p1)->real < ((MODE_PART_OBJ *)p2)->real ? 1 : -1;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ͨ�����������������,ͨ����������һ��������ʵ���鲿,��������
//  ������� : fact=engy/max( 1:n | engy); mgnt=ampl/max(1:n | ampl)
//             real=real/max(1:n | ampl);  imag=imag/max(1:n | ampl)
//  ��    ע : ������ʽ�Ľ�
//  �� �� ֵ :
//  ����˵�� : iObjNum                  �����豸��
//             ModeShape[].ampl         ���[in]
//             ModeShape[].engy         ����[in]
//             ModeShape[].phas         ����[in]
//             ModeShape[].mgnt         ����    [out]
//             ModeShape[].real         ��һʵ��[out]
//             ModeShape[].imag         ��һ�鲿[out]
//             ModeShape[].fact         ��������[out]
////////////////////////////////////////////////////////////////////////////////
bool CLFOAnaByCurve::CalcPartFactor( int iObjNum, MODE_PART_OBJ *ModeShape )
{
	if ( iObjNum <= 0 || iObjNum > 9999) return false;
	
	int   ii;
	float fMaxEngy = 0;
	float fMaxAmpl = 0;
	char LogString[1024];



	// find max-engy and max-ampl
	for ( ii=0; ii<iObjNum; ii++ )
	{
		if (fMaxEngy < ModeShape[ii].engy) { fMaxEngy = ModeShape[ii].engy; }
		if (fMaxAmpl < ModeShape[ii].ampl) { fMaxAmpl = ModeShape[ii].ampl; }
		//if (fMaxReal < ModeShape[ii].real) { fMaxReal = ModeShape[ii].real; }
	}

	if (fMaxEngy <= 0 || fMaxAmpl <= 0 ) return false;
	
	// calculate
	for (ii=0; ii<iObjNum; ii++) 
	{
		// 2007-10-11 modify use ampl
		//ModeShape[ii].fact = ModeShape[ii].engy / fMaxEngy;
		ModeShape[ii].fact = ModeShape[ii].ampl / fMaxAmpl;

		ModeShape[ii].mgnt = ModeShape[ii].ampl / fMaxAmpl;
		ModeShape[ii].real = ModeShape[ii].mgnt * (float)cos(ModeShape[ii].phas);
		ModeShape[ii].imag = ModeShape[ii].mgnt * (float)sin(ModeShape[ii].phas);

//		sprintf( LogString, "��ʾ1����ȡ��������=%f",ModeShape[ii].fact);
//		MessShow( LogString );
	}

	// sort by ModeShape[ii].fact
	qsort((void *)ModeShape, iObjNum, sizeof(MODE_PART_OBJ), ModeShapeCompareByFact);

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ͨ�����������������(�������ʱ�䳣��),ͨ����������һ��������ʵ���鲿,��������
//  ������� : fact=ampl*Tj/max( 1:n | ampl*Tj); mgnt=ampl/max(1:n | ampl)
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : iObjNum                  �����豸��
//             ModeShape[].ampl         ���[in]
//             ModeShape[].engy         ����[in]
//             ModeShape[].phas         ����[in]
//             ModeShape[].mgnt         ����    [out]
//             ModeShape[].real         ��һʵ��[out]
//             ModeShape[].imag         ��һ�鲿[out]
//             ModeShape[].fact         ��������[out](isTj==1 ��� Tj)
////////////////////////////////////////////////////////////////////////////////
bool CLFOAnaByCurve::CalcPartFactor( int iObjNum, MODE_PART_OBJ *ModeShape, int isTj )
{
	if ( iObjNum <= 0 ) return false;
	
	int   ii;
	float fMaxEngy = 0;
	float fMaxAmpl = 0;
	float fGnTj    = 0;
	char LogString[1024];
	// find max-engy and max-ampl
	for ( ii=0; ii<iObjNum; ii++ )
	{
//		fGnTj    = ModeShape[ii].fact ;

//		if (fMaxEngy < ModeShape[ii].ampl*fGnTj) { fMaxEngy = ModeShape[ii].ampl*fGnTj; }
		if (fMaxEngy < ModeShape[ii].engy) { fMaxEngy = ModeShape[ii].engy; }
		if (fMaxAmpl < ModeShape[ii].ampl) { fMaxAmpl = ModeShape[ii].ampl; }
	}
//	sprintf( LogString, "�����豸��Ŀ=%d,fMaxEngy=%f,fMaxAmpl=%f",iObjNum,fMaxEngy,fMaxAmpl);
//	MessShow( LogString );	
	if (fMaxEngy <= 0 || fMaxAmpl <= 0) return false;

	// calculate
	for (ii=0; ii<iObjNum; ii++) 
	{
		ModeShape[ii].fact = ModeShape[ii].ampl / fMaxAmpl;
		
		ModeShape[ii].mgnt = ModeShape[ii].ampl / fMaxAmpl;
		ModeShape[ii].real = ModeShape[ii].mgnt * (float)cos(ModeShape[ii].phas);
		ModeShape[ii].imag = ModeShape[ii].mgnt * (float)sin(ModeShape[ii].phas);
//		sprintf( LogString, "��ʾ2����ȡ��������=%f",ModeShape[ii].fact);
//		MessShow( LogString );
	}

	// down sort by ModeShape[ii].fact
	qsort((void *)ModeShape, iObjNum, sizeof(MODE_PART_OBJ), ModeShapeCompareByFact);
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : �жϲ������ӷ��ţ������������й��������������й�����
//  ������� : ͬ���񵴣�ͨ����ѹ����жϴ��ڳ����Ͷ˻����ܶ�
//             �첽�񵴣�ͨ��Ƶ���жϴ��ڸ�Ƶ���ͣ��˺͵�Ƶ���ܣ���
//  ��    ע : ����ǰ��ʱ�������Ҳ������������飬�û���Ϊ����������+_90���ھ�Ϊ�����෴Ϊ����
//  �� �� ֵ :
//  ����˵�� : iObjNum                  �����豸��
//             ModeShape[].ampl         ���[in]
//             ModeShape[].engy         ����[in]
//             ModeShape[].phas         ����[in]
//             ModeShape[].mgnt         ����    [out]
//             ModeShape[].real         ��һʵ��[out]
//             ModeShape[].imag         ��һ�鲿[out]
//             ModeShape[].fact         ��������[out]
//             isSyn                    ������(1 ͬ����0 �첽)
////////////////////////////////////////////////////////////////////////////////
bool CLFOAnaByCurve::JudgPartFactorSign( int iObjNum, MODE_PART_OBJ *ModeShape, unsigned char isSyn )
{
	if ( iObjNum <= 0 ) return false;
	
	int   ii ;

	if (isSyn == 1)
	{
		// 2007-09-11 lsf �����������ӷ��� ������������
		//////////////////////////////////////////////////////////////////////////
		// �������������鼰ͬ������Ϊ�������ȺΪ��
/*		int   findIndex = 0;
		float findFact  = 0;
		float findPhas  = 0;
		float iDifValu  = (float)(PRONY_PI / 2.0);

		if (SearchMaxFact( iObjNum, ModeShape, findFact, findPhas, findIndex ) == false) return false;
		
		ModeShape[findIndex].fact = -ModeShape[findIndex].fact;
		for (ii=0; ii<iObjNum; ii++)
		{
			if (ii != findIndex) 
			{
				if ( JudgCohcyObjByMode( findPhas, iDifValu, ModeShape[ii].phas ) == 1 )
				{
					ModeShape[ii].fact = -ModeShape[ii].fact;
				}
			}
		}
*/
		
		//////////////////////////////////////////////////////////////////////////
		// ���ݴ��ڳ������Ͷ˻����ܶˣ���ӷ��� ��������˾��л��飩
		int   findIndex = 0;
		float findFact  = 0;
		float findPhas  = 0;
		float iDifValu  = (float)(PRONY_PI / 2.0);
		
		if (SearchMaxFact( iObjNum, ModeShape, findFact, findPhas, findIndex ) == false) return false;
		
		ModeShape[findIndex].fact = -ModeShape[findIndex].fact;
		for (ii=0; ii<iObjNum; ii++)
		{
			if (ii != findIndex) 
			{
				if ( JudgCohcyObjByMode( findPhas, iDifValu, ModeShape[ii].phas ) == 1 )
				{
					ModeShape[ii].fact = -ModeShape[ii].fact;
				}
			}
		}
		
		//////////////////////////////////////////////////////////////////////////

	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// ���ݴ��ڸ�Ƶ�˻��ǵ�Ƶ�ˣ���ӷ���
		//////////////////////////////////////////////////////////////////////////
		
	}
	
	// down sort by ModeShape[ii].fact
	qsort((void *)ModeShape, iObjNum, sizeof(MODE_PART_OBJ), ModeShapeCompareByFact);
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////
//  �������� : ���ò������ӷ��ţ������������й��������������й�����
//  ������� : 
//  ��    ע : 
//  �� �� ֵ :
//  ����˵�� : iObjNum                  �����豸��
//             ModeShape[].ampl         ���[in]
//             ModeShape[].engy         ����[in]
//             ModeShape[].phas         ����[in]
//             ModeShape[].mgnt         ����    [out]
//             ModeShape[].real         ��һʵ��[out]
//             ModeShape[].imag         ��һ�鲿[out]
//             ModeShape[].fact         ��������[out]
//             isSyn                    ������(1 ͬ����0 �첽)
////////////////////////////////////////////////////////////////////////////////
bool CLFOAnaByCurve::ReSetFactorSign( int iObjNum, MODE_PART_OBJ *ModeShape )
{
	if ( iObjNum <= 0 ) return false;
	
	int   ii ;

	for ( ii=0; ii<iObjNum; ii++)
	{
		ModeShape[ii].fact = -ModeShape[ii].fact;
	}
	
	// down sort by ModeShape[ii].fact
	qsort((void *)ModeShape, iObjNum, sizeof(MODE_PART_OBJ), ModeShapeCompareByFact);
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ���Ƕȷ�Χ��һ�����������䡣�Ƕȣ�-180 �� 180; ���ȣ�-pi �� pi
////////////////////////////////////////////////////////////////////////////////
extern "C" float UnitAngle( float iAnglVar, unsigned char iIsRad )
{
	// Ŀǰ����ȡֵ��Χ��飬�Ƕȣ�-180 �� 180; ���ȣ�-pi �� pi
	
	float  OpstAngl;
//	float  AbsDifValu1;
	
	if (iIsRad == 0)
	{
		OpstAngl = (float)fmod( (double)iAnglVar, 360.0 );

		if (OpstAngl > 180.0) 
			return (float)( -360.0 + OpstAngl);// 2008-04-05
		else
			return OpstAngl;
	}
	else
	{
		OpstAngl = (float)fmod( (double)iAnglVar, (PRONY_PI+PRONY_PI) );
		
		if (OpstAngl > PRONY_PI) 
			return (float)( -PRONY_PI - PRONY_PI + OpstAngl);// 2008-04-05
		else
			return OpstAngl;
	}
	
}

/*////////////////////////////////////////////////////////////////////////////////
//  �������� : ʶ��ͬ��(coherency)Ⱥ�����Ⱥ
//  ������� : ͬ��Ⱥ: ��iRefAngl�ο��ǵ�iDifValu�����ڵĽǶ�
//             ���Ⱥ����iRefAngl�ο��ǵĻ����ǵ�iDifValu�����ڵĽǶ�
//  ��    ע : Ҫ��Ƕȷ�Χ����-180 �� 180 �� -pi �� pi��
//  �� �� ֵ : =1 ͬ��; =0 ��ͬ�������; =-1 ���
//  ����˵�� : iRefAngl                 �ο���
//             iDifValu                 ����ƫ��
//             iAnglVar                 �Ƕȱ���
//             iIsRad                   �Ƿ񻡶�(=1 ����; =0 �Ƕ�)
////////////////////////////////////////////////////////////////////////////////
int CLFOAnaByCurve::JudgCohcyObjByMode( float iRefAngl, float iDifValu, float iAnglVar, unsigned char iIsRad )
{
	// Ŀǰ����ȡֵ��Χ��飬�Ƕȣ�-180 �� 180; ���ȣ�-pi �� pi

	if (iDifValu < 0) iDifValu = -iDifValu;//(float)fabs((double)iDifValu);

	float  OpstAngl;
	float  AbsDifValu;
	
	if (iIsRad == 0)
	{
		if (iRefAngl > iAnglVar)  
			AbsDifValu = iRefAngl - iAnglVar;
		else
			AbsDifValu = iAnglVar - iRefAngl;

		if (AbsDifValu > 180.0) 
			AbsDifValu = (float)(360.0 - AbsDifValu) ;

		// �ж�
		if ( AbsDifValu < iDifValu ) 
			return 1;
		else if (AbsDifValu > (180.0 - iDifValu) )
			return -1;
		else
			return 0;		
	}
	else
	{
		if (iRefAngl > iAnglVar)  
			AbsDifValu = iRefAngl - iAnglVar;
		else
			AbsDifValu = iAnglVar - iRefAngl;
		
		if (AbsDifValu > PRONY_PI) 
			AbsDifValu = (float)(PRONY_PI + PRONY_PI - AbsDifValu) ;

		// �ж�
		if ( AbsDifValu < iDifValu ) 
			return 1;
		else if (AbsDifValu > (PRONY_PI - iDifValu) )
			return -1;
		else
			return 0;		
	}
}
*/ // 2007-11-25 lsf �޸����Ⱥ�жϷ���
 
////////////////////////////////////////////////////////////////////////////////
//  �������� : ʶ��ͬ��(coherency)Ⱥ�����Ⱥ
//  ������� : ͬ��Ⱥ: ��iRefAngl�ο��ǵ�iDifValu�����ڵĽǶ�
//             ���Ⱥ����iRefAngl�ο��ǵĻ����ǵ�iDifValu�����ڵĽǶ�
//  ��    ע : Ҫ��Ƕȷ�Χ����-180 �� 180 �� -pi �� pi��
//  �� �� ֵ : =1 ͬ��; =0 ��ͬ�������; =-1 ���
//  ����˵�� : iRefAngl                 �ο���
//             iDifValu                 ����ƫ��
//             iAnglVar                 �Ƕȱ���
//             iIsRad                   �Ƿ񻡶�(=1 ����; =0 �Ƕ�)
////////////////////////////////////////////////////////////////////////////////
int CLFOAnaByCurve::JudgCohcyObjByMode( float iRefAngl, float iDifValu, float iAnglVar, unsigned char iIsRad )
{
	// Ŀǰ����ȡֵ��Χ��飬�Ƕȣ�-180 �� 180; ���ȣ�-pi �� pi
	
	if (iDifValu < 0) iDifValu = -iDifValu;//(float)fabs((double)iDifValu);
	
//	float  OpstAngl;
	float  AbsDifValu;
	
	if (iIsRad == 0)
	{
		if (iRefAngl > iAnglVar)  
			AbsDifValu = iRefAngl - iAnglVar;
		else
			AbsDifValu = iAnglVar - iRefAngl;
		
		if (AbsDifValu > 180.0) 
			AbsDifValu = (float)(360.0 - AbsDifValu) ;
		
		// �ж�
		if ( AbsDifValu < iDifValu ) 
			return 1;
		else if (AbsDifValu > (180.0 - iDifValu) )
			return -1;
		else
			return 0;		
	}
	else
	{
		if (iRefAngl > iAnglVar)  
			AbsDifValu = iRefAngl - iAnglVar;
		else
			AbsDifValu = iAnglVar - iRefAngl;
		
		if (AbsDifValu > PRONY_PI) 
			AbsDifValu = (float)(PRONY_PI + PRONY_PI - AbsDifValu) ;
		
		// �ж�
		if ( AbsDifValu < iDifValu ) 
			return 1;
		else if (AbsDifValu > (PRONY_PI - iDifValu) )
			return -1;
		else
			return 0;		
	}
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ����������������豸
//  ������� : 
//  ��    ע : 
//  �� �� ֵ : 
//  ����˵�� : nPartObj                 �����豸��
//             iPartObj[]               �����豸��Ϣ
//             oFact                    ����������[out]
//             oPhas                    ����[out]
//             oIndex                   �±�[out]
////////////////////////////////////////////////////////////////////////////////
bool CLFOAnaByCurve::SearchMaxFact( int nPartObj, MODE_PART_OBJ *iPartObj, float &oFact, float &oPhas, int &oIndex )
{
	if ( !iPartObj || nPartObj <= 0 || nPartObj > 9999) return false;
	
	int   ii;
	float ftempdata = 0;
	
	oFact  = 0;
	oIndex = 0;

	for (ii=0; ii<nPartObj; ii++)
	{
		if ( oFact < iPartObj[ii].fact )
		{
			oFact  = iPartObj[ii].fact;
			oPhas  = iPartObj[ii].phas;
			oIndex = ii;

			ftempdata = iPartObj[ii].ampl;
		}
		else if ( oFact == iPartObj[ii].fact )
		{
			if (ftempdata < iPartObj[ii].ampl) 
			{
				oFact  = iPartObj[ii].fact;
				oPhas  = iPartObj[ii].phas;
				oIndex = ii;
				
				ftempdata = iPartObj[ii].ampl;
			}
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ����������������豸�ĶԶ˲�����������豸
//  ������� : 
//  ��    ע : 
//  �� �� ֵ : 
//  ����˵�� : nPartObj                 �����豸��
//             iPartObj[]               �����豸��Ϣ
//             iRefAngl                 �ο���
//             iDifValu                 ����ƫ��
//             oFact                    ����������[out]
//             oPhas                    ����[out]
//             oIndex                   �±�[out]
//             iIsRad                   �Ƿ񻡶�(=1 ����; =0 �Ƕ�)
////////////////////////////////////////////////////////////////////////////////
bool CLFOAnaByCurve::SearchMaxFactOpst( int nPartObj, MODE_PART_OBJ *iPartObj, float iRefAngl, float iDifValu, float &oFact, float &oPhas, int &oIndex, unsigned char iIsRad )
{
	if ( !iPartObj || nPartObj <= 0 ) return false;
	if ( iDifValu < 0 )               iDifValu = -iDifValu;//(float)fabs((double)iDifValu);
	
	iRefAngl  = UnitAngle( iRefAngl, 1 );      //  ת���Ƕ�Ϊ��������
	int   ii;
	float ftempdata = 0;
//	float OpstAngl;
	float AbsDifValu;
	
	oFact  = 0;
	oIndex = 0;
	
	for (ii=0; ii<nPartObj; ii++)
	{
		if (iIsRad == 0)
		{
			if (iRefAngl > iPartObj[ii].phas)  
				AbsDifValu = iRefAngl - iPartObj[ii].phas;
			else
				AbsDifValu = iPartObj[ii].phas - iRefAngl;
			
			if (AbsDifValu > 180.0) 
				AbsDifValu = (float)(360.0 - AbsDifValu) ;
			
			// �ж�
			if ( AbsDifValu > iDifValu )  // opposite angle 120
			{
				if ( oFact < iPartObj[ii].fact )
				{
					oFact  = iPartObj[ii].fact;
					oPhas  = iPartObj[ii].phas;
					oIndex = ii;
					
					ftempdata = iPartObj[ii].ampl;
				}
				else if ( oFact == iPartObj[ii].fact )
				{
					if (ftempdata < iPartObj[ii].ampl) 
					{
						oFact  = iPartObj[ii].fact;
						oPhas  = iPartObj[ii].phas;
						oIndex = ii;
						
						ftempdata = iPartObj[ii].ampl;
					}
				}
				
			}
		}
		else
		{
			if (iRefAngl > iPartObj[ii].phas)  
				AbsDifValu = iRefAngl - iPartObj[ii].phas;
			else
				AbsDifValu = iPartObj[ii].phas - iRefAngl;
			
			if (AbsDifValu > PRONY_PI) 
				AbsDifValu = (float)(PRONY_PI + PRONY_PI - AbsDifValu) ;
			
			// �ж�
			if ( AbsDifValu > iDifValu )  // opposite angle 120
			{
				if ( oFact < iPartObj[ii].fact )
				{
					oFact  = iPartObj[ii].fact;
					oPhas  = iPartObj[ii].phas;
					oIndex = ii;
					
					ftempdata = iPartObj[ii].ampl;
				}
				else if ( oFact == iPartObj[ii].fact )
				{
					if (ftempdata < iPartObj[ii].ampl) 
					{
						oFact  = iPartObj[ii].fact;
						oPhas  = iPartObj[ii].phas;
						oIndex = ii;
						
						ftempdata = iPartObj[ii].ampl;
					}
				}
				
			}
		}
		

	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ��Դ����ʶ��
//  ������� : �Բ�����������������Ϊ�ο��ǣ����ݸ������������໮��ͬ��Ⱥ��
//             ���Ⱥ��ͬ��Ⱥ��Ϊ������Դ
//  ��    ע : ���о���Ҫ��֤
//  �� �� ֵ : 
//  ����˵�� : iRefAngl                 �ο���
//             iDifValu                 ����ƫ��
//             iAnglVar                 �Ƕȱ���
//             iIsRad                   �Ƿ񻡶�(=1 ����; =0 �Ƕ�)
////////////////////////////////////////////////////////////////////////////////
// #define	 LFO_CLUST_RMAIN  0             // ����Ⱥ // 2008-09-24
// #define	 LFO_CLUST_CRTCL  1             // �ٽ�Ⱥ // 2008-09-24
// #define	 LFO_CLUST_OPOST  2             // ���Ⱥ // 2008-09-24
void CLFOAnaByCurve::IdtfCausLfoGn( int &nCausGn, MODE_PART_OBJ *oCausGn, float iDifValu, unsigned char iIsRad )
{
	if ( !oCausGn || nCausGn <= 0 || nCausGn > 9999) return ;
	if ( iDifValu < 0 )             iDifValu = -iDifValu;//(float)fabs((double)iDifValu);
	
	int   ii;
	int   retValue      = 1;
	int   findIndex     = 0;
	float findFact      = 0;
	float findPhas      = 0;
	float OpstfindPhas  = -1;
	char LogString[1024];
	
	if (SearchMaxFact( nCausGn, oCausGn, findFact, findPhas, findIndex ) == false ) return;
// 	sprintf( LogString, "��׼�Ƕ�=%f,��׼����[%s],����������[%f]",findPhas,oCausGn[findIndex].obj_name,findFact);
// 	MessShow( LogString );

    // 2008-09-19
    if( iIsRad == 0 )
	{
        OpstfindPhas = (float)(findPhas - 180);
// 		sprintf( LogString, "��ԽǶ�=%f",OpstfindPhas);
// 		MessShow( LogString );
		if (OpstfindPhas < -180)
		{
			OpstfindPhas = (float)(OpstfindPhas + 360);
// 			sprintf( LogString, "��������ԽǶ�=%f",OpstfindPhas);
// 			MessShow( LogString );
		}
	}
    else
	{
        OpstfindPhas = (float)(findPhas - PRONY_PI);
		if (OpstfindPhas < -PRONY_PI)
		{
			OpstfindPhas = (float)(OpstfindPhas + 2* PRONY_PI);
		}
	}

// 	/*if ( iIsRad == 0 )   
// 		OpstDifValu = (float)(180 - iDifValu);
// 	else
// 		OpstDifValu = (float)(PRONY_PI - iDifValu);
//     if (SearchMaxFactOpst( nCausGn, oCausGn, findPhas, OpstDifValu, OpstfindFact, OpstfindPhas, OpstfindIndex ) == false )
// 		OpstfindIndex = -1;	*/
	//SearchMaxFactOpst( int nPartObj, MODE_PART_OBJ *iPartObj, float iRefAngl, float iDifValu, float &oFact, float &oPhas, int &oIndex, unsigned char iIsRad )
	//if (findFact == 0) return;

	//oCausGn[findIndex].fact = -oCausGn[findIndex].fact; //2007-11-25 
				
	for (ii=0; ii<nCausGn; ii++)
	{
		if (ii != findIndex) 
		{
//			oCausGn[ii].fact = 0;
			oCausGn[ii].clus = LFO_CLUST_RMAIN;
			retValue = JudgCohcyObjByMode( findPhas, iDifValu, oCausGn[ii].phas );
// 			sprintf( LogString, "��׼�Ƕ�=%f,�жϻ���[%s]�Ƕ�=%f,�Ƕ�ƫ��=%f,�жϽ��[%d]",findPhas, oCausGn[ii].obj_name,oCausGn[ii].phas,iDifValu,retValue);
// 			MessShow( LogString );
			if ( retValue == 1 )
			{
				//oCausGn[ii].fact = oCausGn[ii].fact;//-oCausGn[ii].fact;//2007-11-25 
				oCausGn[ii].clus = LFO_CLUST_CRTCL; //2008-09-24 lsf add 
			}
			else
			{
				retValue = JudgCohcyObjByMode( OpstfindPhas, iDifValu, oCausGn[ii].phas ); // opposite
// 				sprintf( LogString, "��ԽǶ�=%f,�жϻ���[%s]�Ƕ�=%f,�Ƕ�ƫ��=%f,�жϽ��[%d]",OpstfindPhas, oCausGn[ii].obj_name,oCausGn[ii].phas,iDifValu,retValue);
// 				MessShow( LogString );
				if ( retValue == 1 )
				{
					//oCausGn[ii].fact = -oCausGn[ii].fact;//-oCausGn[ii].fact;//2007-11-25 
					oCausGn[ii].clus = LFO_CLUST_OPOST; //2008-09-24 lsf add 
				}
			}
		}
		else
		{
			oCausGn[ii].clus = LFO_CLUST_CRTCL;
		}
	}

	// down sort by ModeShape[ii].fact (��->С)
	qsort((void *)oCausGn, nCausGn, sizeof(MODE_PART_OBJ), ModeShapeCompareByFact);

// 	for (ii=nCausGn-1; ii>=0; ii--)
// 	{
// 		//if ( oCausGn[ii].fact == -100 ) 
// 		if ( oCausGn[ii].clus == LFO_CLUST_RMAIN ) 
// 		{
// 			memset((void*)&oCausGn[ii].obj_id, 0, sizeof(MODE_PART_OBJ) );
// 			nCausGn -= 1;
// 		}
// 	}
	
	return ;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ʶ��ͬ��
//  ������� : �Ը��������Ϊ�ο��ǣ����ݸ��������豸���໮��ͬ��Ⱥ��
//             �Ը����Զ˳����Ϊ�ο����ж����Ⱥ
//  ��    ע : ���о���Ҫ��֤
//  �� �� ֵ : 
//  ����˵�� : iRefAngl                 �ο���
//             iDifValu                 ����ƫ��
//             iAnglVar                 �Ƕȱ���
//             iIsRad                   �Ƿ񻡶�(=1 ����; =0 �Ƕ�)
////////////////////////////////////////////////////////////////////////////////
void CLFOAnaByCurve::IdtfLfoGnCohcyFc( int &nCausFc, MODE_PART_OBJ *oCausFc, float iRefAngl, float iOpstAngl, float iDifValu, unsigned char iIsRad )
{
	if ( !oCausFc || nCausFc <= 0 ) return ;
	if ( iDifValu < 0 )             iDifValu = -iDifValu;   //  (float)fabs((double)iDifValu);

	iRefAngl  = UnitAngle( iRefAngl, 1 );      //  ת���Ƕ�Ϊ��������
	iOpstAngl = UnitAngle( iOpstAngl, 1 );      //  ת���Ƕ�Ϊ��������
//if ( m_pSltMode[ii].PartGn.fact * m_pSltMode[m_pSltMode[ii].nPartGn-1].PartGn.fact < 0.0000001 ) 	
	int   ii;
	int   retValue      = 1;
	float findPhas      = 0;
	float OpstDifValu   = 0;
	int   OpstfindIndex = -1;
	float OpstfindFact  = -1;
	float OpstfindPhas  = -1;
	
/*	if (SearchMaxFact( nCausFc, oCausFc, findFact, findPhas, findIndex ) == false ) return;

	if ( iIsRad == 0 )   
		OpstDifValu = (float)(180 - iDifValu);
	else
		OpstDifValu = (float)(PRONY_PI - iDifValu);
    if (SearchMaxFactOpst( nCausFc, oCausFc, findPhas, OpstDifValu, OpstfindFact, OpstfindPhas, OpstfindIndex ) == false )
		OpstfindIndex = 0;	
	//SearchMaxFactOpst( int nPartObj, MODE_PART_OBJ *iPartObj, float iRefAngl, float iDifValu, float &oFact, float &oPhas, int &oIndex, unsigned char iIsRad )
	//if (findFact == 0) return;
	oCausFc[findIndex].fact = oCausFc[findIndex].fact;//-oCausFc[findIndex].fact; //2007-11-25 
*/
	if ( iIsRad == 0 )  
	{
		findPhas = (float)(iRefAngl - iOpstAngl);
		if ( findPhas < 0 ) 
		{
			findPhas = - findPhas;
			if ( findPhas > 180 ) findPhas = 360-findPhas;//findPhas - 180;
			if ( findPhas < iDifValu )   OpstfindIndex = -2 ; // ����û�жԶ�
		}
	}
	else
	{
		findPhas = (float)(iRefAngl - iOpstAngl);
		if ( findPhas < 0 ) 
		{
			findPhas = - findPhas;
			if ( findPhas > PRONY_PI )   findPhas = (float)(PRONY_PI+PRONY_PI-findPhas );//(float)(findPhas - PRONY_PI);
			if ( findPhas < iDifValu )   OpstfindIndex = -2 ; // ����û�жԶ�
		}
	}
	
	if ( OpstfindIndex == -2 )  // ����û�жԶ�
	{
		if (SearchMaxFactOpst( nCausFc, oCausFc, iRefAngl, OpstDifValu, OpstfindFact, OpstfindPhas, OpstfindIndex ) == false )
			OpstfindIndex = -1;	
		
		for (ii=0; ii<nCausFc; ii++)
		{
			retValue = JudgCohcyObjByMode( iRefAngl, iDifValu, oCausFc[ii].phas );
			if ( retValue == 1 )
			{
				//oCausFc[ii].fact = oCausFc[ii].fact;//-oCausFc[ii].fact;//2007-11-25 
				oCausFc[ii].clus = LFO_CLUST_CRTCL; //2008-09-24 lsf add 
			}
			else
			{
				if (OpstfindIndex == -1) 
				{
					oCausFc[ii].fact = -100;
				}
				else
				{
					retValue = JudgCohcyObjByMode( OpstfindPhas, iDifValu, oCausFc[ii].phas ); // opposite
					if ( retValue == 1 )
					{
						oCausFc[ii].fact = -oCausFc[ii].fact;//-oCausFc[ii].fact;//2007-11-25 
					}
					else
					{
						oCausFc[ii].fact = -100;
					}
				}
			}
		}
	}
	else
	{
		OpstfindPhas = iOpstAngl;
		for (ii=0; ii<nCausFc; ii++)
		{
			retValue = JudgCohcyObjByMode( iRefAngl, iDifValu, oCausFc[ii].phas );
			if ( retValue == 1 )
			{
				//oCausFc[ii].fact = oCausFc[ii].fact;//-oCausFc[ii].fact;//2007-11-25 
			}
			else
			{
				retValue = JudgCohcyObjByMode( OpstfindPhas, iDifValu, oCausFc[ii].phas ); // opposite
				if ( retValue == 1 )
				{
					oCausFc[ii].fact = -oCausFc[ii].fact;//-oCausFc[ii].fact;//2007-11-25 
				}
				else
				{
					oCausFc[ii].fact = -100;
				}
			}
		}
	}
				

	// down sort by ModeShape[ii].fact (��->С)
	qsort((void *)oCausFc, nCausFc, sizeof(MODE_PART_OBJ), ModeShapeCompareByFact);

	for (ii=nCausFc-1; ii>=0; ii--)
	{
		if ( oCausFc[ii].fact == -100 ) 
		{
			memset((void*)&oCausFc[ii].obj_id, 0, sizeof(MODE_PART_OBJ) );
			nCausFc -= 1;
		}
	}
	
	return ;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ʶ��ͬ��
//  ������� : �Ը��������Ϊ�ο��ǣ����ݸ��������豸���໮��ͬ��Ⱥ��
//             �Ը����Զ˳����Ϊ�ο����ж����Ⱥ
//  ��    ע : ���о���Ҫ��֤
//  �� �� ֵ : 
//  ����˵�� : iRefAngl                 �ο���
//             iDifValu                 ����ƫ��
//             iAnglVar                 �Ƕȱ���
//             iIsRad                   �Ƿ񻡶�(=1 ����; =0 �Ƕ�)
////////////////////////////////////////////////////////////////////////////////
void CLFOAnaByCurve::IdtfLfoGnCohcyFc( int &nCausFc, MODE_PART_OBJ *oCausFc, MODE_PART_OBJ *CausGn, float iDifValu, unsigned char iIsRad )
{
	if ( !oCausFc || nCausFc <= 0 ) return ;
	if ( iDifValu < 0 )             iDifValu = -iDifValu;   //  (float)fabs((double)iDifValu);

	int   ii;
	int   retValue      = 1;
	int   findIndex     = 0;
	float findPhas      = 0;
	float OpstfindPhas  = -1;

	findPhas = CausGn->phas;
	char LogString[1024];
	
// 	sprintf( LogString, "��վ��Ⱥ��׼�Ƕ�=%f",findPhas);
// 	MessShow( LogString );
    // 2008-09-19
    if( iIsRad == 0 )
	{
        OpstfindPhas = (float)(findPhas  - 180);
		if (OpstfindPhas < -180)
		{
			OpstfindPhas = (float)(OpstfindPhas + 360);
		}
	}
    else
	{
        OpstfindPhas = (float)(findPhas  - PRONY_PI);
		if (OpstfindPhas < -PRONY_PI)
		{
			OpstfindPhas = (float)(OpstfindPhas + 2 * PRONY_PI);
		}
	}
// 	sprintf( LogString, "��վ��Ⱥ��ԽǶ�=%f",OpstfindPhas);
// 	MessShow( LogString );

// 	/*if ( iIsRad == 0 )   
// 		OpstDifValu = (float)(180 - iDifValu);
// 	else
// 		OpstDifValu = (float)(PRONY_PI - iDifValu);
//     if (SearchMaxFactOpst( nCausFc, oCausFc, findPhas, OpstDifValu, OpstfindFact, OpstfindPhas, OpstfindIndex ) == false )
// 		OpstfindIndex = -1;	*/
	for (ii=0; ii<nCausFc; ii++)
	{
// 		if (ii != findIndex) 
		{
			oCausFc[ii].fact = 0;
			oCausFc[ii].clus = LFO_CLUST_RMAIN;
			retValue = JudgCohcyObjByMode(  findPhas, iDifValu, oCausFc[ii].phas );
			if ( retValue == 1 )
			{
				//oCausFc[ii].fact = oCausFc[ii].fact;//-oCausFc[ii].fact;//2007-11-25 
				oCausFc[ii].clus = CausGn->clus; //2008-09-24 lsf add 
			}
			else
			{
				retValue = JudgCohcyObjByMode( OpstfindPhas, iDifValu, oCausFc[ii].phas ); // opposite
				if ( retValue == 1 )
				{
					if ( CausGn->clus == LFO_CLUST_CRTCL )
					{
						oCausFc[ii].clus = LFO_CLUST_OPOST;
					}
					else if ( CausGn->clus == LFO_CLUST_OPOST )
					{
						oCausFc[ii].clus = LFO_CLUST_CRTCL;
					}
					
				}
			}
		}
	}

	// down sort by ModeShape[ii].fact (��->С)
	qsort((void *)oCausFc, nCausFc, sizeof(MODE_PART_OBJ), ModeShapeCompareByFact);

// 	for (ii=nCausFc-1; ii>=0; ii--)
// 	{
// 		//if ( oCausFc[ii].fact == -100 ) 
// 		if ( oCausFc[ii].clus == LFO_CLUST_RMAIN ) 
// 		{
// 			memset((void*)&oCausFc[ii].obj_id, 0, sizeof(MODE_PART_OBJ) );
// 			nCausFc -= 1;
// 		}
// 	}
	
	return ;
}

void CLFOAnaByCurve::filteDevByPartfact(int nMode,MODE_CHRCT *pMode,int &nSltMode1,MODE_CHRCT *pSltMode1, int sh_fact)
{
//	if ( nMode <= 0 || !pMode ) return ;
	
	int   ii, jj;
	int   nPart = 0;
	float fEngy = 0;
	float fReal = 0;
	float fImag = 0;
	ii=0;
	nSltMode1 = 0;
// 	for ( ii=0; ii<nMode; ii++ )
// 	{
	int   		GnPart = 0;
	int   		LnPart = 0;
	int   		TrPart = 0;
	int   		FcPart = 0;
	char LogString[1024];
	pSltMode1[ii].nPartGn = 0;
	pSltMode1[ii].nPartLn = 0;
	pSltMode1[ii].nPartTr = 0;
	pSltMode1[ii].nPartFc = 0;
	
	sprintf( LogString, "�񵴻�����Ŀ=%d,����·��Ŀ=%d,�񵴱�ѹ����Ŀ=%d",pMode[0].nPartGn,pMode[0].nPartLn,pMode[0].nPartTr);
	MessShow( LogString );
	if ( pMode[ii].nPartGn > 0 ) 
	{			
		for (jj=0; jj<pMode[ii].nPartGn; jj++) 
		{
// 			sprintf( LogString, "�����������=%f,�ż�ֵ=%f",pMode[ii].PartGn[jj].fact,sh_fact);
// 				MessShow( LogString );
			if (float(pMode[ii].PartGn[jj].fact - sh_fact)>0.001)
			{
				MemCpyModePartObjStru( pMode[ii].PartGn[jj], pSltMode1[ii].PartGn[GnPart] );
				GnPart += 1;
				pSltMode1[ii].nPartGn = GnPart;
			}
		}
	}
	if ( pMode[ii].nPartLn > 0 ) 
	{			
		for (jj=0; jj<pMode[ii].nPartLn; jj++) 
		{
// 			sprintf( LogString, "��·��������=%f,�ż�ֵ=%f",pMode[ii].PartLn[jj].fact,sh_fact);
// 				MessShow( LogString );
			if (float(pMode[ii].PartLn[jj].fact - sh_fact)>0.001)
			{
				MemCpyModePartObjStru( pMode[ii].PartLn[jj], pSltMode1[ii].PartLn[LnPart] );
				LnPart += 1;
				pSltMode1[ii].nPartLn = LnPart;
			}
		}
	}
	if ( pMode[ii].nPartTr > 0 ) 
	{			
		for (jj=0; jj<pMode[ii].nPartTr; jj++) 
		{
// 			sprintf( LogString, "��ѹ����������=%f,�ż�ֵ=%f",pMode[ii].PartTr[jj].fact,sh_fact);
// 				MessShow( LogString );
			if (float(pMode[ii].PartTr[jj].fact - sh_fact)>0.001)
			{
				MemCpyModePartObjStru( pMode[ii].PartTr[jj], pSltMode1[ii].PartTr[TrPart] );
				TrPart += 1;
				pSltMode1[ii].nPartTr = TrPart;
			}
		}
	}
	if ( pMode[ii].nPartFc > 0 ) 
	{			
		for (jj=0; jj<pMode[ii].nPartFc; jj++) 
		{
			if (float(pMode[ii].PartFc[jj].fact - sh_fact)>0.001)
			{
				MemCpyModePartObjStru( pMode[ii].PartFc[jj], pSltMode1[ii].PartFc[FcPart] );
				FcPart += 1;
				pSltMode1[ii].nPartFc = FcPart;
			}
		}
	}
	nSltMode1 = pSltMode1[ii].nPartGn + pSltMode1[ii].nPartLn + pSltMode1[ii].nPartTr + pSltMode1[ii].nPartFc;		
	return ;

}

/*////////////////////////////////////////////////////////////////////////////////
//  �������� : ��Դ����ʶ��
//  ������� : �Բ�����������������Ϊ�ο��ǣ����ݸ������������໮��ͬ��Ⱥ��
//             ���Ⱥ��ͬ��Ⱥ��Ϊ������Դ
//  ��    ע : ���о���Ҫ��֤
//  �� �� ֵ : 
//  ����˵�� : iRefAngl                 �ο���
//             iDifValu                 ����ƫ��
//             iAnglVar                 �Ƕȱ���
//             iIsRad                   �Ƿ񻡶�(=0 ����; =1 �Ƕ�)
////////////////////////////////////////////////////////////////////////////////
void CLFOAnaByCurve::IdtfCausLfoGn( int nPartGn, MODE_PART_OBJ *iPartGn, int &nCausGn, MODE_PART_OBJ *oCausGn, float iDifValu )
{
	if ( !iPartGn || nPartGn <= 0 || !oCausGn ) return ;
	
	int   ii;
	int   retValue  = 1;
	int   findIndex = 0;
	float findFact  = 0;
	float findPhas  = 0;
	
	if (SearchMaxFact( nPartGn, iPartGn, findFact, findPhas, findIndex ) == false ) return;
	
	if (findFact == 0) return;
	
	int   dataSize = sizeof(MODE_PART_OBJ);
	nCausGn = 0;
	MemCpyModePartObjStru( iPartGn[findIndex], oCausGn[nCausGn] );
	oCausGn[nCausGn].fact = -oCausGn[nCausGn].fact;
	nCausGn += 1;
				
	for (ii=0; ii<nPartGn; ii++)
	{
		if (ii != findIndex) 
		{
			retValue = JudgCohcyObjByMode( findPhas, iDifValu, iPartGn[ii].phas );
			if ( retValue == 1 )
			{
				//memcpy( (void*)&oCausGn[nCausGn].obj_id, (void*)&iPartGn[ii].obj_id, dataSize );
				MemCpyModePartObjStru( iPartGn[ii], oCausGn[nCausGn] );
				oCausGn[ii].fact = -oCausGn[ii].fact;
				nCausGn += 1;
			}
			else if (retValue == -1)
			{
				//memcpy( (void*)&oCausGn[nCausGn].obj_id, (void*)&iPartGn[ii].obj_id, dataSize );
				MemCpyModePartObjStru( iPartGn[ii], oCausGn[nCausGn] );
				//oCausGn[ii].fact = -oCausGn[ii].fact;
				nCausGn += 1;
			}
			else
			{
			}
		}
	}
	
	// down sort by ModeShape[ii].fact
	qsort((void *)oCausGn, nCausGn, sizeof(MODE_PART_OBJ), ModeShapeCompareByFact);
	return ;
}*/


////////////////////////////////////////////////////////////////////////////////
//  �������� : ��������� f_ampl��С����
////////////////////////////////////////////////////////////////////////////////
extern "C" int ModeCenterCompareByAmpl(const void * p1, const void * p2)
{
    if ( ((LFO_CNTR_FC *)p1)->f_ampl != ((LFO_CNTR_FC *)p2)->f_ampl )
    {
        return ((LFO_CNTR_FC *)p1)->f_ampl < ((LFO_CNTR_FC *)p2)->f_ampl ? 1 : -1;
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : �ж������ģ�����Ҫ����ͬ���񵴺��첽�񵴣�
//  ������� : �����ģ���λ90����ѹ�ڶ���󡢽ǶȰڶ�С����������·�е�ѹ�ڶ���󣬹��ɶ�����·ͬ�����������������˷�
//             ���и���棬������ָ�����������飬��
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : 
////////////////////////////////////////////////////////////////////////////////
int CLFOAnaByCurve::IdtfLfoCenter( MODE_CHRCT *pMode, int isSyn )
{
	if ( !pMode )  return 0;

	int ii;

	pMode->nLfoCenter = 0;
	if ( pMode->nPartFc > 0 )
	{
		for ( ii=0; ii<pMode->nPartFc; ii++ )
		{
			if ( pMode->PartFc[ii].clus == LFO_CLUST_RMAIN )
			{
				pMode->LfoCenter[pMode->nLfoCenter].fac_id = pMode->PartFc[ii].obj_id;
				pMode->LfoCenter[pMode->nLfoCenter].f_ampl = pMode->PartFc[ii].ampl;//�ǶȰڶ�
				pMode->LfoCenter[pMode->nLfoCenter].clus   = pMode->PartFc[ii].clus;
				pMode->nLfoCenter += 1;
			}
		}
	}
	if ( pMode->nLfoCenter > 1)
	{
		qsort((void *)pMode->LfoCenter, pMode->nLfoCenter , sizeof(LFO_CNTR_FC), ModeCenterCompareByAmpl);
	}
	
	return pMode->nLfoCenter;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : �������ƴ�ʩ���������ݲ������Ӻ������ģ����߲��Թ��򣬸����������ƴ�ʩ��
//  ������� : 
//             
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : iSrcData                 ԭʼ����
//             oFltData                 �˲�����[OUT]
//             nDataLen                 ���ݳ���
//             rank                     �˲�������(2 - 5)
////////////////////////////////////////////////////////////////////////////////
int CLFOAnaByCurve::LfoCtrlMesrSearch()
{
	
	return 0;
}
