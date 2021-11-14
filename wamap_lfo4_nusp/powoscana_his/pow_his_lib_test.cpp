#include "lfo_ana_obj.h"

extern CLfoAnaObj  * g_pAnaObj;
extern int WamPowHisAna();
void PowAnaEnd(LFO_CASE_INFO &LfoCase);
void PowAnaDoing(LFO_CASE_INFO &LfoCase);

int main()
{
    WamPowHisAna( );
    return 0;
}

void PowAnaEnd(LFO_CASE_INFO &LfoCase)
{
    return;
}

void PowAnaDoing(LFO_CASE_INFO &LfoCase)
{
    return;
}

void PowMessShow(char * msg)
{
	g_pAnaObj->PLF_MessShow(msg);
	return;
}

