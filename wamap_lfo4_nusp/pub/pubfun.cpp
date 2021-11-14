// pubfun.cpp: implementation of the CPubFun class.
//
//////////////////////////////////////////////////////////////////////

#include "pubfun.h"

//////////////////////////////////////////////////////////////////////////
// CPubTime.cpp: implementation of the CPubTime class.
//
//////////////////////////////////////////////////////////////////////
CPubTime::CPubTime()
{
	// printf("CPubTime构造开始\n");
}

CPubTime::~CPubTime()
{

}

/*
功能: 延时 sec 秒
输入: 秒值
输出: 
*/
void CPubTime::PT_DelaySecond( int sec )
{
#ifdef _WINDOWS32
	Sleep( sec * 1000 ) ;
#else
	sleep( sec ) ;
#endif
}

void CPubTime::PT_DelayMinSec( int msec )
{
#ifdef _WINDOWS32
	Sleep( msec ) ;
#else
	int seconds  = 0;  //2007-05 lsf  hpux usleep()要求参数<1000000
	int mseconds = 0;
	
	seconds  = (int)(msec/1000);
	mseconds = msec - seconds*1000;

	sleep( seconds ) ;
	usleep( mseconds ) ; 
#endif

}

/*
功能: 计算时间t1和t2间隔
输入: 时间t1 t2
输出: 时间间隔（毫秒）
*/
int CPubTime::PT_CalculateTimeInterval(struct timeb t1, struct timeb t2)
{
	int  timeInter = 0;

    timeInter = (t2.time*1000+t2.millitm) - (t1.time*1000+t1.millitm);
	
	if(timeInter >= 0)
	{
		return timeInter;
	}
	else
	{
		return (-timeInter);
	}
}

/*
功能: 计算时间t1和t2间隔
输入: 时间t1 t2
输出: 时间间隔（毫秒）
*/
int CPubTime::PT_CalculateTimeInterval(time_t t1, time_t t2)
{
	int  timeInter = 0;
	
    timeInter = t2 - t1;
	
	if(timeInter >= 0)
	{
		return timeInter;
	}
	else
	{
		return (-timeInter);
	}
}

/*
功能: 获得机器当前秒
输入: 
输出: 
*/
int CPubTime::PT_GetNowSeconds()
{
	time_t now_t ;
	time(&now_t) ;
		
	return now_t ;
}

/*
功能: 获得机器当前毫秒
输入: 
输出: 
*/
struct timeb CPubTime::PT_GetNowMilliSeconds()
{
	struct timeb  now_t ;

	ftime(&now_t) ;
	
	return now_t;
}

/*
功能: 取得指定时间的本月开始时间
输入: tSecond 需转换的时间
输出: (本月1日0时0分0秒的秒值)
*/
time_t CPubTime::PT_GetMonBgnSec(time_t tSecond)
{
    struct tm * ptmTime = localtime(&tSecond);

    ptmTime->tm_mday= 1;
    ptmTime->tm_hour= 0;
    ptmTime->tm_min = 0;
    ptmTime->tm_sec = 0;

    return mktime(ptmTime);
}

/*
功能: 取得指定时间的今日开始时间
输入: tSecond 需转换的时间
输出: (本月今日0时0分0秒的秒值)
*/
time_t CPubTime::PT_GetDayBgnSec(time_t tSecond)
{
    struct tm * ptmTime = localtime(&tSecond);

    ptmTime->tm_hour= 0;
    ptmTime->tm_min = 0;
    ptmTime->tm_sec = 0;

    return mktime(ptmTime);
}

/*
功能: 取得指定时间的今日开始时间
输入: tSecond 需转换的时间
输出: (本月今日本时0分0秒的秒值)
*/
time_t CPubTime::PT_GetHurBgnSec(time_t tSecond)
{
    struct tm * ptmTime = localtime(&tSecond);

    ptmTime->tm_min = 0;
    ptmTime->tm_sec = 0;

    return mktime(ptmTime);
}

/*
功能: 将时间类型转换成秒级的日期时间字符串
输入: 需转换的时间
输出: 转换后的秒级日期时间字符串(YYYY-MM-DD HH:mm:ss)
*/
char * CPubTime::PT_GetTimeStringBySeconds(const time_t tSecond)
{
    static char     szTime[20];
    struct tm *     ptmTime = NULL;
	
	memset(szTime, 0, sizeof(szTime));
	if(((int)tSecond) <= 0)
	{
		sprintf(szTime, "%04d-%02d-%02d %02d:%02d:%02d", 
			0000, 00, 00, 00, 00, 00);
		
		return szTime;
	}
	
    ptmTime = localtime(&tSecond);
    sprintf(szTime, "%04d-%02d-%02d %02d:%02d:%02d",
		ptmTime->tm_year + 1900,
		ptmTime->tm_mon  + 1,
		ptmTime->tm_mday,
		ptmTime->tm_hour,
		ptmTime->tm_min,
		ptmTime->tm_sec);
	
    return  szTime;
}
/*
功能: 将时间类型转换成秒级的日期时间字符串
输入: 需转换的时间
输出: 转换后的秒级日期时间字符串(YYYYMMDDHHmmss)
*/
char * CPubTime::PT_GetNoSpaceMarkTimeStringBySeconds(time_t tSecond)
{
    static char     szTime[20];
    struct tm *     ptmTime = NULL;

	memset(szTime, 0, sizeof(szTime));	
	if(((int)tSecond) <= 0)
	{
		sprintf(szTime, "%04d%02d%02d%02d%02d%02d", 
			0000, 00, 00, 00, 00, 00);
		
		return szTime;
	}
	
    ptmTime = localtime(&tSecond);
    sprintf(szTime, "%04d%02d%02d%02d%02d%02d",
		ptmTime->tm_year + 1900,
		ptmTime->tm_mon  + 1,
		ptmTime->tm_mday,
		ptmTime->tm_hour,
		ptmTime->tm_min,
		ptmTime->tm_sec);
	
    return  szTime;
}
/*
功能: 将世纪秒转换成YYYY年MM月DD日HH时mm分ss秒字符串
输入: 需转换的时间
输出: 转换后的秒级日期时间字符串
*/
char * CPubTime::PT_GetChineseTimeStringBySeconds(time_t tSecond)
{
    static char     szTime[28];
    struct tm *     ptmTime = NULL;

	memset(szTime, 0, sizeof(szTime));
	
	if(((int)tSecond) <= 0)
	{
		sprintf(szTime, "%04d年%02d月%02d日%02d时%02d分%02d秒", 
			0000, 00, 00, 00, 00, 00);
		
		return szTime;
	}
	
    ptmTime = localtime(&tSecond);
    sprintf(szTime, "%04d年%02d月%02d日%02d时%02d分%02d秒",
		ptmTime->tm_year + 1900,
		ptmTime->tm_mon  + 1,
		ptmTime->tm_mday,
		ptmTime->tm_hour,
		ptmTime->tm_min,
		ptmTime->tm_sec);
	
    return  szTime;
}

/*
功能: 将世纪秒转换成字符串（HH:mm:ss）
输入: 需转换的时间
输出: 转换后的时间字符串(HH:mm:ss)
*/
char * CPubTime::PT_GetHMSTimeStringBySeconds(const time_t tSecond)
{
    static char     szTime[10];
    struct tm *     ptmTime = NULL;
	memset(szTime, 0, sizeof(szTime));
	
	if(((int)tSecond) <= 0)
	{
		sprintf(szTime, "%02d:%02d:%02d",	00, 00, 00);
		
		return szTime;
	}
	
    ptmTime = localtime(&tSecond);
    sprintf(szTime, "%02d:%02d:%02d",
		ptmTime->tm_hour,
		ptmTime->tm_min,
		ptmTime->tm_sec);
	
    return  szTime;
}

/*
功能: 将时间间隔（正毫秒）转换成符串（分秒毫秒）
输入: 需转换的时间（正毫秒）
输出: 转换后的字符串（分秒毫秒）
*/
char * CPubTime::PT_GetStringOfTimeInterval(int msec)
{
	static char     szTime[20];
	int             nM, nS, nMs = 0;

	if ( msec<=0 ) 
	{
		sprintf(szTime, "%s", "");
		return  szTime;
	}
	
	nM = (int)(msec/60000);
	nS = (int)((msec-nM*60000)/1000);
	nMs = (int)(msec-nM*60000-nS*1000);
	
	if(nM >0)
	{
		if(nMs>0)
		{
			sprintf(szTime, "%d分%d秒%d毫秒", nM, nS, nMs);
		}
		else
		{
			sprintf(szTime, "%d分%d秒", nM, nS);
		}
	}
	else
	{
		if(nS >0)
		{
			if(nMs>0)
			{
				sprintf(szTime, "%d秒%d毫秒", nS, nMs);
			}
			else
			{
				sprintf(szTime, "%d秒", nS);
			}
		}
		else
		{
			if(nMs>0)
			{
				sprintf(szTime, "%d毫秒", nMs);
			}
		}
	}			
	
    return  szTime;
}
////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 由时间字符串得到世纪秒
//  处理过程 : 将时间字符串YYYY-MM-DD HH:MM:SS转换成世纪秒
//  备    注 : caseinfo.bin文件中格式
//  返 回 值 : 0正常 -1异常
//  参数说明 : timestring         	给定的时间字符串
// 			   nSecond				世纪秒  							   [OUT]
////////////////////////////////////////////////////////////////////////////////
int CPubTime::PT_GetSecondByTimeString(const char * timestring, int &nSecond)
{
    time_t     timeN;
    struct tm  stime;
    
    if ( PT_GetTmTimeByTimeString(timestring,stime) == -1 ) return -1;
    
    nSecond = 0;
    timeN   = mktime(&stime);
    nSecond = timeN;
    
    return 0;
}

int CPubTime::PT_GetSecondByTimeString(const char * timestring)
{
    time_t     timeN;
    struct tm  stime;
    
    if ( PT_GetTmTimeByTimeString(timestring,stime) == -1 ) return -1;
    
    int nSecond = 0;
    timeN   = mktime(&stime);
    nSecond = timeN;
    
    return nSecond;
}
////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 由时间字符串得到时间结构
//  处理过程 : 将时间字符串YYYY-MM-DD HH:MM:SS转换成时间结构体
//  备    注 : 
//  返 回 值 : 
//  参数说明 : timestring         	给定的时间字符串
// 			   TimeSet				时间结构  							   [OUT]
////////////////////////////////////////////////////////////////////////////////
int CPubTime::PT_GetTmTimeByTimeString(const char * timestring, struct tm &sTime)
{
    int iLength = strlen(timestring);
    if( iLength < 19 )  return -1;
    
    char year[5],month[3],day[3],hour[3],mintue[3],second[3];
    int  n_year,n_month,n_day,n_hour,n_mintue,n_second;
    
    memset(year,0,5);
    memset(month,0,3);
    memset(day,0,3);
    memset(hour,0,3);
    memset(mintue,0,3);
    memset(second,0,3);
    
    memcpy(year,   timestring,    4);
    memcpy(month,  timestring+5,  2);
    memcpy(day,    timestring+8,  2);
    memcpy(hour,   timestring+11, 2);
    memcpy(mintue, timestring+14, 2);
    memcpy(second, timestring+17, 2);
    
    n_year	= atoi(year);
    n_month	= atoi(month);
    n_day	= atoi(day);
    n_hour	= atoi(hour);
    n_mintue= atoi(mintue);
    n_second= atoi(second);
    
    // 应增加数值检查 
    sTime.tm_year = n_year-1900 ;
    sTime.tm_mon  = n_month-1;
    sTime.tm_mday = n_day;
    sTime.tm_hour = n_hour;
    sTime.tm_min  = n_mintue;
    sTime.tm_sec  = n_second;
    
    return 0;
}

//////////////////////////////////////////////////////////////////////////
// CPubString.cpp: implementation of the CPubString class.
//
//////////////////////////////////////////////////////////////////////
CPubString::CPubString()
{
	// printf("CPubString构造开始\n");
}

CPubString::~CPubString()
{

}
////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 数据类型的相互转化
//  处理过程 : 
//  备    注 : in_value				输入的数据类型
//			   out_type				转化后的数据类型
//  返 回 值 : 转换后的数
//  参数说明 : t 					待转换的数
////////////////////////////////////////////////////////////////////////////////
template<class out_type, class in_value>
out_type SF_convert(const in_value & t)
{
    stringstream stream;
    
    stream<<t;						//向流中传值
    
    out_type result;				//这里存储转换结果
    
    stream>>result;					//向result中写入值
    
    return result;
}
////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 去掉字符串左端的空格和换行
//  处理过程 : 
//  备    注 : 
//  返 回 值 : 0正常；其余异常
//  参数说明 : str              	类型为string, 需处理的字符串		   [OUT]
////////////////////////////////////////////////////////////////////////////////
int CPubString::PS_StringTrimLeft(string &str)
{
	while(str.find("\n") == 0)
	{
		str.erase(0,1);
	}
	
	while(str.find("\r") == 0)
	{
		str.erase(0,1);
	}
	
	while(str.find(" ") == 0)
	{
		str.erase(0,1);
	}
	
    while(str.find("\t") == 0)
    {
        str.erase(0,1);
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 去掉字符串右端的空格和换行
//  处理过程 : 
//  备    注 : 
//  返 回 值 : 0正常；其余异常
//  参数说明 : str              	类型为string, 需处理的字符串		   [OUT]
////////////////////////////////////////////////////////////////////////////////
int CPubString::PS_StringTrimRight(string &str)
{
	while((int)str.rfind("\n") == (int)str.size()-1)
	{
		int l = str.size();
		if(l == 0)
		{
			return 0;
		}
		str.erase(l-1,1);
	}
	
	while((int)str.rfind("\r") == (int)str.size()-1)
	{
		int l = str.size();
		if(l == 0)
		{
			return 0;
		}
		str.erase(l-1,1);
	}

	while((int)str.rfind(" ") == (int)str.size()-1)
	{
		int l = str.size();
		if(l == 0)
		{
			return 0;
		}
		str.erase(l-1,1);
	}
	
    while((int)str.rfind("\t") == (int)str.size()-1)
    {
        int l = str.size();
        if(l == 0)
        {
            return 0;
        }
        str.erase(l-1,1);
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 去掉字符串两边的空格和换行
//  处理过程 : 
//  备    注 : 
//  返 回 值 : 0正常；其余异常
//  参数说明 : str              	类型为string, 需处理的字符串		   [OUT]
////////////////////////////////////////////////////////////////////////////////
int CPubString::PS_StringTrimAll(string &str)
{
	while(str.find("\n") == 0)
	{
		str.erase(0,1);
	}

	while(str.find("\r") == 0)
	{
		str.erase(0,1);
	}
	
    while(str.find(" ") == 0)
    {
        str.erase(0,1);
	}

    while(str.find("\t") == 0)
    {
        str.erase(0,1);
	}

	while((int)str.rfind("\n") == (int)str.size()-1)
	{
		int l = str.size();
		if(l == 0)
		{
			return 0;
		}
		str.erase(l-1,1);
	}

	while((int)str.rfind("\r") == (int)str.size()-1)
	{
		int l = str.size();
		if(l == 0)
		{
			return 0;
		}
		str.erase(l-1,1);
	}
	
	while((int)str.rfind(" ") == (int)str.size()-1)
	{
		int l = str.size();
		if(l == 0)
		{
			return 0;
		}
		str.erase(l-1,1);
	}
	
    while((int)str.rfind("\t") == (int)str.size()-1)
    {
        int l = str.size();
        if(l == 0)
        {
            return 0;
        }
        str.erase(l-1,1);
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 去掉字符串左端的空格
//  处理过程 : 
//  备    注 : 
//  返 回 值 : 处理后的字符串
//  参数说明 : str              	类型为char, 需处理的字符串
////////////////////////////////////////////////////////////////////////////////
char*  CPubString::PS_CharTrimLeft(char* str)
{
	char* str_begin=str;
	char* str_end=str+strlen(str);
	
	while( str_begin < str_end)
	{
		if( NULL == strchr(" \t",*str_begin) )
			break;
		str_begin++;
	}
	
	memmove(str,str_begin,strlen(str_begin)+1);
	return str;	
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 去掉字符串右端的空格
//  处理过程 : 
//  备    注 : 
//  返 回 值 : 处理后的字符串
//  参数说明 : str              	类型为char, 需处理的字符串
////////////////////////////////////////////////////////////////////////////////
char*  CPubString::PS_CharTrimRight(char* str)
{
	char* newstr=PS_CharTrimLeft(str);
	char* str_Rbegin=newstr+strlen(newstr) - 1;
	
	char* str_begin=newstr;
	
	while( str_Rbegin >= newstr)
	{
		if( NULL == strchr(" \t",*str_Rbegin) )
		{
			break;
		}
		str_Rbegin--;
	}
	str_Rbegin++;
	*str_Rbegin='\0';

	memmove(str,str_begin,(str_Rbegin-str_begin+1));
	return str;	
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 清除字符串中所有的空格
//  处理过程 : 
//  备    注 : 
//  返 回 值 : 0正常；其余异常
//  参数说明 : str              	类型为string, 需处理的字符串		   [OUT]
////////////////////////////////////////////////////////////////////////////////
int CPubString::PS_TrimBlank(string  &str)
{
	int m_State=0;
	int	len_begin = str.length();
	char str_begin[100];
	char str_end[100];
	
	sprintf(str_begin,"%s",str.c_str());
	sprintf(str_end,"%s",str.c_str());
	
	char* pdest;
	pdest = strstr(str_begin," ");
	while(pdest != NULL)
	{
		int pos = pdest - str_begin + 1;
		memcpy(str_begin+pos-1,str_end+pos,len_begin-pos);
		str_begin[len_begin-1] = '\0';
		sprintf(str_end,"%s",str_begin);
		pdest = strstr(str_begin," ");
	}
	
	str = str_begin;
	return m_State;	
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 去除字符串中的字母或者中文
//  处理过程 : 
//  备    注 : mode = 0：去除字符串中的字母
//			   mode = 1：去除字符串中的汉字
//  返 回 值 : 0正常；其余异常
//  参数说明 : str              	类型为string, 需处理的字符串		   [OUT]
//			   mode					模式
////////////////////////////////////////////////////////////////////////////////
int CPubString::PS_TrimABC(string &Name,int mode)
{
	int m_Flag=0;
	string TempName,TempNameEN,TempNameCN;

	TempNameEN.erase();
	TempNameCN.erase();
	TempName.erase();

	int s=0;
	s=Name.length();

	for (int i=0;i<s;) 
	{
		/*
		char * chr;
		chr = Name.substr(i,1).c_str();

		if(chr <= ''&&chr >= '')
		{
			TempNameEN+=Name.substr(i,1);
		}
		*/
// 		if((BYTE)Name[i]<0x80)
// 		{
// 			if (Name[i]!='('&&Name[i]!=')') 
// 			{
// 				TempNameEN+=Name.substr(i,1);
// 			}
// 			i=i+1;
// 		}
// 		else
		{
			TempNameCN+=Name.substr(i,2);
			 i=i+2;
		}
	}
	
	if(mode == 0)  Name = TempNameCN;
	else if(mode == 1)  Name = TempNameEN;
	else m_Flag = -1;

	return m_Flag;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 字符串倒转
//  处理过程 : 
//  备    注 : 
//  返 回 值 : 0正常；其余异常
//  参数说明 : str              	类型为char, 需处理的字符串			   [OUT]
////////////////////////////////////////////////////////////////////////////////
int CPubString::PS_StringStrrev(char* s)
{
	char r[1024]; 
	int i, j, l; 
	for (l = 0; s[l] != '\0'; l++)
		;
	for (i = 0, j = l - 1; i < l; i++, j--)
		r[i] = s[j];
	r[i] = '\0';
	
	
	for(i=0;i<l;i++)
		s[i]=r[i];
	s[i]='\0';
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 填充字符串，使之满NB列，以免发生取数错误
//  处理过程 : 
//  备    注 : NB需要小于160,否则需修改函数
//  返 回 值 : 0正常；其余异常
//  参数说明 : str              	类型为char, 传入及传出的需转化的字符串 [OUT]
//             NB                   指定的结果字符串的长度,工程中定为80
////////////////////////////////////////////////////////////////////////////////
int CPubString::PS_FillLine(string &str,int NB)
{
	int s = 0;
	string str2, str1;
	str2 = "                                                                                                                                                                ";
	s = str.size();
	if(s < NB)
	{
		str1 = str2.substr(0, NB-s);
		str += str1;
	}
	else if(s > NB)
	{
		str1 = str.substr(0, NB);
		str = str1;
	}
	
	str[NB] = '\0';
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 浮点数转化为字符串，便于输出格式控制
//  处理过程 : 
//  备    注 : 
//  返 回 值 : 0表示正常；其余表示异常
//  参数说明 : x 					传入的浮点数
//			   k  					转化的字符串的长度
//			   xstr 				类型为char, 传出的结果字符串		   [OUT]
////////////////////////////////////////////////////////////////////////////////
int CPubString::PS_FloatToChar( float x,int k,char *xstr)
{
	string  emptystr;
	//emptystr="             ";
	emptystr="             ";
	if (fabs(x)<0.0000001)
	{
		sprintf(xstr,"%s",emptystr.substr(0,k).c_str());
		return 0;		
	}

	char Temp[20];
	memset(Temp,0,sizeof(Temp));
	
	string str3;
	sprintf(Temp,"%f",x);
	str3=Temp;
	if (fabs(x)<1)
	{
		if(x>0)
			str3.replace(0,2,".");
		else
			str3.replace(1,2,".");
	}
	int i;
	i=str3.find('.');
	int len;
	len=k-i-1;
	
	switch(len)
	{
	case 0:
		sprintf(Temp,"%0.1f",x);
		break;
	case 1:
		sprintf(Temp,"%0.1f",x);
		break;
	case 2:
		sprintf(Temp,"%0.2f",x);
		break;
	case 3:
		sprintf(Temp,"%0.3f",x);
		break;
	case 4:
		sprintf(Temp,"%0.4f",x);
		break;
	case 5:
		sprintf(Temp,"%0.5f",x);
		break;
	case 6:
		sprintf(Temp,"%0.6f",x);
		break;
	}
	str3=Temp;
	if (fabs(x)<1)
	{
		int i=0;
		i=str3.find("0.");
		if(i>0)
		{
			if(x>0)
				str3.replace(i,2,".");
			else
				str3.replace(i,2,".");
		}
		else if(i==0)
		{
			if(x>0)
				str3.replace(i,2,".");
			else
				str3.replace(i,2,".");
		}
	}
	str3=str3.substr(0,k);
	sprintf(xstr,"%s",str3.c_str());

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 浮点数转化为字符串，便于输出格式控制
//  处理过程 : 
//  备    注 : 
//  返 回 值 : 0表示正常；其余表示异常
//  参数说明 : x 					传入的浮点数
//			   k  					转化的字符串的长度
//			   xstr 				类型为string, 传出的结果字符串		   [OUT]
////////////////////////////////////////////////////////////////////////////////
int CPubString::PS_FloatToStr(float x, int k, string &xstr)
{
	string emptystr;
	stringstream strstream;
	emptystr="0.0000000001";
	if (fabs(x) < 0.0000001)
	{
		xstr = emptystr.substr(0, k);
		return 0;		
	}

	string str3;
	str3 = SF_convert <string> (x);

	if (fabs(x)<1)
		str3.replace(str3.find("0."), 2, ".");
	int i;
	i = str3.find(".");
	int len;
	len = k - i - 1;
	if(len < 0)
	{
		xstr=str3.substr(0, k);         /* 如果遇到较大的数 则这样处理 */
		return -1;
	}
	
	char tran_ptr[20];

	switch(len)
	{
	case 0:
		memset(tran_ptr, 0, sizeof(tran_ptr));
		sprintf(tran_ptr, "%0.1f", x);
		str3 = tran_ptr;
		break;
	case 1:
		memset(tran_ptr, 0, sizeof(tran_ptr));
		sprintf(tran_ptr, "%0.1f", x);
		str3 = tran_ptr;
		break;
	case 2:
		memset(tran_ptr, 0, sizeof(tran_ptr));
		sprintf(tran_ptr, "%0.2f", x);
		str3 = tran_ptr;
		break;
	case 3:
		memset(tran_ptr, 0, sizeof(tran_ptr));
		sprintf(tran_ptr, "%0.3f", x);
		str3 = tran_ptr;
		break;
	case 4:
		memset(tran_ptr, 0, sizeof(tran_ptr));
		sprintf(tran_ptr, "%0.4f", x);
		str3 = tran_ptr;
		break;
	case 5:
		memset(tran_ptr, 0, sizeof(tran_ptr));
		sprintf(tran_ptr, "%0.5f", x);
		str3 = tran_ptr;
		break;
	case 6:
		memset(tran_ptr, 0, sizeof(tran_ptr));
		sprintf(tran_ptr, "%0.6f", x);
		str3 = tran_ptr;
		break;
	}
	if (fabs(x)<1)
		str3.replace(str3.find("0."), 2, ".");

	xstr=str3.substr(0, k);
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 整形10进制数转为二进制的字符串
//  处理过程 : 
//  备    注 : 
//  返 回 值 : 转换后的字符串
//  参数说明 : dec 					传入的整型十进制数
//			   len  				转换后二进制字符串的长度
////////////////////////////////////////////////////////////////////////////////
char*  CPubString::PS_dectobin(int dec,int len)
{
	int i=0;     
	static char buf[256]; 
	memset(buf,0,sizeof(buf));       
	memset(buf,'0',len);   

	while(dec!=0)       
	{              
		buf[i++]=dec%2+48;
		dec=dec/2;       
	}
	
	PS_StringStrrev(buf);
	return buf;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 将字符串转换为大写
//  处理过程 : 
//  备    注 : 
//  返 回 值 : 转换成功为0
//  参数说明 : str 					传入的字符串
////////////////////////////////////////////////////////////////////////////////
int CPubString::PS_StringMakeUpper(string &str)
{
    transform (str.begin(), str.end(),    //source
        str.begin(),                      //destination
        ::toupper);                         //operation
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 将字符串转换为小写
//  处理过程 : 
//  备    注 : 
//  返 回 值 : 转换成功为0
//  参数说明 : str 					传入的字符串
////////////////////////////////////////////////////////////////////////////////
int CPubString::PS_StringMakeLower(string &str)
{
    transform (str.begin(), str.end(),    //source
        str.begin(),                      //destination
        ::tolower);                         //operation
    return 0;
}

int CPubString::PS_Str2Char(const string strSrc, char* CharDes, int Charlen)
{
    char tmpChar[MAXBUF];
    for(int ii=0; ii<Charlen; ii++ )
    {
        tmpChar[ii]=' ';
    }
    sprintf(tmpChar,"%s",strSrc.c_str());
    tmpChar[Charlen] = '\0';
    memcpy(CharDes,tmpChar,Charlen);

    return 0;
}

int CPubString::PS_MemCpy(char *CharDes, const char* CharSrc, const int Charlen )
{
	memcpy(CharDes,CharSrc,Charlen -1 );
	CharDes[Charlen-1] = '\0';
	
	return 0;
}

int CPubString::PS_GetFaultNo(char *fault_desc)
{
	int fault_no =-1;
	string tmpStr = fault_desc;
	string::size_type pStart,pEnd;
	pStart = tmpStr.find_first_of("$");
	pEnd   = tmpStr.find_last_of("$");
	if ( pStart!= string::npos && pEnd!= string::npos )
	{
		fault_no = atoi(tmpStr.substr(pStart+1,pEnd).c_str());
	}
	
	return fault_no;
}

int CPubString::PS_ConvertFtoStr( const double data, const int count, char *strchar )
{
	double source = data;
	PS_PreConvertFtoStr( source, count );
	
	int decimal = 0;
	int sign = 0;
	
	string str = fcvt( source, 4, &decimal, &sign );
	int i = 0;
	
	/* 正数 */
	if ( 0 == sign )
	{
		/* 整数部分超过可表达的范围 */
		if ( decimal > 0 && decimal > count )
		{
			str.erase();
			return 1;
		}
		
		/* 无整数部分且小数部分超过可表达的范围 */
		else if ( decimal < 0 && abs( decimal )+1 >= count )
		{
			str = "0";
		}
		
		/* 可表达的范围内仅可能的精度 */
		else
		{
			if ( decimal < 0 )
			{
				for ( i = 0; i < abs( decimal ); i++ )
				{
					str.insert( 0, "0" );
				}
				decimal = 0;
			}
			str.insert( decimal, "." );
			if ( (int)str.size() > count )
			{
				str.erase( count, str.size() );
			}
		}
	}
	
	/* 负数 */
	else
	{
		if ( decimal > 0 && decimal > count-1 )
		{
			str.erase();
			return -1;
		}
		else if ( decimal < 0 && abs( decimal )+1 >= count-1 )
		{
			str = "0";
		}
		else
		{
			if ( decimal < 0 )
			{
				for ( i = 0; i < abs( decimal ); i++ )
				{
					str.insert( 0, "0" );
				}
				decimal = 0;
			}
			str.insert( decimal, "." );
			str.insert( 0, "-" );
			if ( (int)str.size() > count )
			{
				str.erase( count, str.size() );
			}
		}
	}
	
	sprintf( strchar, "%s", str.c_str() );
	
	return 0;
}

int CPubString::PS_PreConvertFtoStr( double &source, const int count )
{
	int decimal = 0;
	int sign = 0;
	char *buffer;
	
	buffer = fcvt( source, count, &decimal, &sign );

	string str = buffer;
	int i = 0;
	
	/* 正数 */
	if ( 0 == sign )
	{
		/* 整数部分超过可表达的范围 */
		if ( decimal > 0 && decimal > count )
		{
			str.erase();
			return 1;
		}
		
		/* 无整数部分且小数部分超过可表达的范围 */
		else if ( decimal < 0 && abs( decimal ) >= count )
		{
			str.erase();
			return 1;
		}
		
		/* 可表达的范围内仅可能的精度 */
		else
		{
			if ( decimal < 0 )
			{
				for ( i = 0; i < abs( decimal ); i++ )
				{
					str.insert( 0, "0" );
				}
				decimal = 0;
			}
			str.insert( decimal, "." );
			if ( (int)str.size() > count )
			{
				string::size_type k = str.find( "." ), l;
				int n = count;
				if( k == count ) n++;

				int j = atoi(str.substr(n,1).c_str());
				if( j >= 5 )
				{					
					str.erase( count, str.size() );
					source = atof( str.c_str() );
					if( k == string::npos )
					{
						source = source + 1;
					}	
					else
					{
						for( l = 1; l <= count - k - 1; l++ )
							source = source * 10.0;
						source = source + 1;
						for( l = 1; l <= count - k - 1; l++ )
							source = source / 10.0;
					}				
				}
			}
		}
	}
	
	/* 负数 */
	else
	{
		if ( decimal > 0 && decimal > count-1 )
		{
			str.erase();
			return -1;
		}
		else if ( decimal < 0 && abs( decimal ) >= count-1 )
		{
			str.erase();
			return -1;
		}
		else
		{
			if ( decimal < 0 )
			{
				for ( i = 0; i < abs( decimal ); i++ )
				{
					str.insert( 0, "0" );
				}
				decimal = 0;
			}
			str.insert( decimal, "." );
			str.insert( 0, "-" );
			if ( (int)str.size() > count )
			{
				string::size_type k = str.find( "." ), l;
				int n = count;
				if( k == count ) n++;
				
				int j = atoi(str.substr(n,1).c_str());
				if( j >= 5 )
				{					
					str.erase( count, str.size() );
					source = atof( str.c_str() );
					if( k == string::npos )
					{
						source = source - 1;
					}	
					else
					{
						for( l = 1; l <= count - k - 1; l++ )
							source = source * 10.0;
						source = source - 1;
						for( l = 1; l <= count - k - 1; l++ )
							source = source / 10.0;
					}
				}
			}
		}
	}
	
	return 0;
}

int CPubString::PS_GetGenName13( char *name13, const char *name, const float &voltbase, const char *id )
{
	char voltc[5];
	if (0 != PS_ConvertFtoStr(voltbase,4,voltc))
	{
// 		sprintf(m_LogStr,"电压转换失败[%f]",voltbase);
// 		MessShow(m_LogStr,LTERROR);
	}
	
	sprintf(name13,"%s%s%s",name,voltc,id);
	
	return 0;
}

int CPubString::PS_StringParse(string strline,vector<string> &vecline)
{
	stringstream ss;
	ss.str(strline);
	string tmpstr;
	while(ss>>tmpstr)
	{
		PS_StringTrimAll(tmpstr);
		vecline.push_back(tmpstr);
	}
	
	return 0;
}


int CPubString::PS_StringDivde( const string &strSrc, const string & strSpliter, vector<string> &vDivStr )
{
    string strDiv;
	
    strDiv = std::string(strSrc);
	
    vDivStr.clear();
    PS_StringTrimAll(strDiv);
    if (strDiv.empty()) return -1;

    int nPos=strDiv.find(strSpliter);
    while (nPos != string::npos)
    {
        vDivStr.push_back(strDiv.substr(0, nPos));
        strDiv.erase(0, nPos + 1);
        PS_StringTrimLeft(strDiv);
        nPos=strDiv.find(strSpliter);
    }
    vDivStr.push_back(strDiv);
	
    return vDivStr.size();
}

int CPubString::PS_StringCat( char *deschar, char *srcchar, const int deslen, const int srclen, int maxlen )
{
	if ((deslen + srclen) <= maxlen -1)
	{
		strcat(deschar,srcchar);
	}
	else
	{
		strncpy(deschar+deslen,srcchar,maxlen - deslen -1);
		deschar[maxlen -1 ] = '\0';
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
// CPubFile.cpp: implementation of the CPubFile class.
//
//////////////////////////////////////////////////////////////////////
CPubFile::CPubFile()
{
	// printf("CPubFile构造开始\n");
    memset(PF_logstr,0,sizeof(PF_logstr));
#ifdef _WINDOWS32
    PF_ByteOrder = false;
    sprintf(PF_backslash,"\\");
#else
    PF_ByteOrder = true;
    sprintf(PF_backslash,"/");
#endif
    PF_BinFormat = true;
    
    PF_IntSize    = sizeof(int);
    PF_Floatsize  = sizeof(float);
}

CPubFile::~CPubFile()
{

}

int CPubFile::PF_CreateFile(const string filename,const string msg )
{
    string tmpString = filename;
    PF_StringTrimAll(tmpString);

    FILE	*fp;
    if( (fp=fopen(tmpString.c_str(),"w") ) == NULL )
    {
        sprintf(PF_logstr,"文件打开失败[%s]",tmpString.c_str());
        return -1;
    }
    else
    {
        if( !msg.empty() )
        {
            fprintf(fp,"%s",msg.c_str());
        }
        fclose(fp);
        return 0;
    }
}
bool  CPubFile::PF_FindFile(const string filename)
{
    string tmpString = filename;
    PF_StringTrimAll(tmpString);

    fstream f;
    f.open( tmpString.c_str(), ios::in);
    if ( f.fail())
    {
        return false;
    }
    else
    {
		f.close();
        return true;
    }
}

bool  CPubFile::PF_FindDir(const string dirname)
{
    string tmpString = dirname;
    PF_StringTrimAll(tmpString);
    
    if( (access( tmpString.c_str(), 0 )) != -1 )
    {
        return true;
    }
    else
    {
        return false;
    }

}
int  CPubFile::PF_MkDir(const string dirname)
{
    int ret=0;
    if( PF_FindDir(dirname) == false )
    {
#ifdef _WINDOWS32
        ret = mkdir( dirname.c_str() );
#else
        ret = mkdir( dirname.c_str(),0777 );
#endif
    }
    if( ret == -1 )
    {
        printf( "Directory %s was created error\n",dirname.c_str());
        return -1;
    }
    return 0;
}
char* CPubFile::PF_GetCurDir()
{
    static char buffer[MAXBUF];
    
    /* Get the current working directory: */
    if( getcwd( buffer, MAXBUF ) == NULL )
        perror( "_getcwd error" );

    return buffer;
}
int CPubFile::PF_RemoveFile(const string filename )
{
    string tmpString = filename;
    PF_StringTrimAll(tmpString);

    if( PF_FindFile(tmpString) == false )
    {
        return -1;
    }

    if( remove( tmpString.c_str() ) == -1 )
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
int CPubFile::PF_CopyFileM(const string fileSrc,const string fileDes )
{
    string tmpfilesrc,tmpfiledes;
    tmpfilesrc = fileSrc;
    tmpfiledes = fileDes;
    PF_StringTrimAll(tmpfilesrc);
    PF_StringTrimAll(tmpfiledes);

    ifstream isFile(tmpfilesrc.c_str());
    ofstream osFile(tmpfiledes.c_str());

    if( !isFile )
    {
        sprintf(PF_logstr,"文件打开失败：[%s]",tmpfilesrc.c_str());
        return -1;
    }
    
    // copy file
    char c;
    while( isFile.get(c))
    {
        osFile.put(c);
    }

    return 0;
}
int CPubFile::PF_ClearDir(const string dirname)
{
    string tmpFileName;
#ifdef _WINDOWS32
    struct _finddata_t c_file;
    long hFile;
    
    hFile = 0;
    tmpFileName = dirname + "*.*";
    if( (hFile = _findfirst( tmpFileName.c_str(), &c_file )) != -1L )
    {
        while( _findnext( hFile, &c_file ) == 0 )
        {
            if( c_file.name[0] == '.' )
            {
                continue;
            }
            
            tmpFileName = dirname + c_file.name;
            PF_RemoveFile(tmpFileName);
        }
        return 0;
    }
    else
    {
        return -1;
    }

#else
    
    DIR *dir = NULL;
    struct dirent *entryPtr = NULL;
    string tmpType;
    
    /* Open the given directory, if you can. */  
    dir = opendir( dirname.c_str() );
    if( dir == NULL )
    {
        sprintf(PF_logstr, "PF_Error opening %s: %s", dirname.c_str(), strerror( errno ) );
        return -1;
    }
    
    while( (entryPtr = readdir( dir )) != NULL )
    {
        
        if( ( strncmp( entryPtr->d_name, ".", PATH_MAX ) == 0 ) ||
            ( strncmp( entryPtr->d_name, "..", PATH_MAX ) == 0 ) ) 
        {
            continue;
        }
        
        tmpFileName = dirname + entryPtr->d_name;
        PF_RemoveFile(tmpFileName);

    }
    
    closedir( dir );

    return 0;
    
#endif
}

int CPubFile::PF_ClearDir(const string dirname, const vector<string> filetype, const int mode)
{
	// mode =0 ; 保留后缀为"filetype"的文件
	// mode =1 ; 删除后缀为"filetype"的文件
    string tmpFileName;
    int ii = 0, bFlag = 0;
#ifdef _WINDOWS32
    struct _finddata_t c_file;
    long hFile;
    string::size_type idx;
    string tmpFileType;
    
    hFile = 0;
    tmpFileName = dirname + "*.*";
    if( (hFile = _findfirst( tmpFileName.c_str(), &c_file )) != -1L )
    {
        while( _findnext( hFile, &c_file ) == 0 )
        {
            if( c_file.name[0] == '.' )
            {
                continue;
            }
            
            tmpFileName = c_file.name;
            idx = tmpFileName.find_last_of(".");
            if( idx == string::npos )
            {
                continue;
            }
            tmpFileType = tmpFileName.substr(idx+1);
            bFlag = 0;
            for( ii = 0; ii< (int)filetype.size(); ii++ )
            {
                if( tmpFileType == filetype[ii] )
                {
                    bFlag = 1;
                    break;
                }
            }
            if( bFlag == mode )
            {
                tmpFileName = dirname + c_file.name;
                PF_RemoveFile(tmpFileName);
            }
        }
    }
    
#else
    
    DIR *dir = NULL;
    struct dirent *entryPtr = NULL;
    string tmpType;
    
    /* Open the given directory, if you can. */  
    dir = opendir( dirname.c_str() );
    if( dir == NULL )
    {
        sprintf(PF_logstr, "打开目录错误[%s]: %s", dirname.c_str(), strerror( errno ) );
        return false;
    }
    
    while( (entryPtr = readdir( dir )) != NULL )
    {
        
        if( ( strncmp( entryPtr->d_name, ".", PATH_MAX ) == 0 ) ||
            ( strncmp( entryPtr->d_name, "..", PATH_MAX ) == 0 ) ) 
        {
            continue;
        }
        
        tmpType = entryPtr->d_name;
        string::size_type idx = tmpType.find_last_of(".");
        tmpType = tmpType.substr(idx+1);
        
        bFlag = 0;
        for( ii = 0; ii<(int)filetype.size(); ii++ )
        {
            if( tmpType == filetype[ii] )
            {
                bFlag = 1;
                break;
            }
        }
        if( bFlag == mode )
        {
            tmpFileName = dirname + entryPtr->d_name;
            PF_RemoveFile(tmpFileName);
        }
    }
    
    closedir( dir );
    
#endif
    return 0;
}

int CPubFile::PF_CopyDir(const string srcdir,const string desdir)
{
	// 将srcdir目录下所有文件拷贝到desdir目录下
    string SrcFile,DesFile;
#ifdef _WINDOWS32
    struct _finddata_t c_file;
    long hFile;
    
    hFile = 0;
    SrcFile = srcdir + "*.*";
    if( (hFile = _findfirst( SrcFile.c_str(), &c_file )) != -1L )
    {
        while( _findnext( hFile, &c_file ) == 0 )
        {
            if( c_file.name[0] == '.' )
            {
                continue;
            }
            
            SrcFile = srcdir + c_file.name;
			DesFile = desdir + c_file.name;
            PF_CopyFileM(SrcFile,DesFile);
        }
        return 0;
    }
    else
    {
        return -1;
    }
	
#else
    
    DIR *dir = NULL;
    struct dirent *entryPtr = NULL;
    string tmpType;
    
    /* Open the given directory, if you can. */  
    dir = opendir( srcdir.c_str() );
    if( dir == NULL )
    {
        sprintf(PF_logstr, "PF_Error opening %s: %s", srcdir.c_str(), strerror( errno ) );
        return -1;
    }
    
    while( (entryPtr = readdir( dir )) != NULL )
    {
        
        if( ( strncmp( entryPtr->d_name, ".", PATH_MAX ) == 0 ) ||
            ( strncmp( entryPtr->d_name, "..", PATH_MAX ) == 0 ) ) 
        {
            continue;
        }
        
        SrcFile = srcdir + entryPtr->d_name;
		DesFile = desdir + entryPtr->d_name;
        PF_CopyFileM(SrcFile,DesFile);
		
    }
    
    closedir( dir );
	
    return 0;
    
#endif
}

int CPubFile::PF_CopyDir(const string srcdir,const string desdir,const vector<string> filetype)
{
	// 将srcdir目录下所有文件拷贝到desdir目录下
	// 只拷贝指定文件后缀的文件
    string SrcFile,DesFile,tmpType;
	int ii,bFlag;
#ifdef _WINDOWS32
    struct _finddata_t c_file;
    long hFile;
    
    hFile = 0;
    SrcFile = srcdir + "*.*";
    if( (hFile = _findfirst( SrcFile.c_str(), &c_file )) != -1L )
    {
        while( _findnext( hFile, &c_file ) == 0 )
        {
            if( c_file.name[0] == '.' )
            {
                continue;
            }
            
			bFlag = 0;
			for( ii = 0; ii<(int)filetype.size(); ii++ )
			{
				if( tmpType == filetype[ii] )
				{
					bFlag = 1;
					break;
				}
			}
			if( bFlag == 1 )
			{
				SrcFile = srcdir + c_file.name;
				DesFile = desdir + c_file.name;
				PF_CopyFileM(SrcFile,DesFile);
			}
        }
        return 0;
    }
    else
    {
        return -1;
    }
	
#else
    
    DIR *dir = NULL;
    struct dirent *entryPtr = NULL;
    
    /* Open the given directory, if you can. */  
    dir = opendir( srcdir.c_str() );
    if( dir == NULL )
    {
        sprintf(PF_logstr, "PF_Error opening %s: %s", srcdir.c_str(), strerror( errno ) );
        return -1;
    }
    
    while( (entryPtr = readdir( dir )) != NULL )
    {
        
        if( ( strncmp( entryPtr->d_name, ".", PATH_MAX ) == 0 ) ||
            ( strncmp( entryPtr->d_name, "..", PATH_MAX ) == 0 ) ) 
        {
            continue;
        }
        
		bFlag = 0;
		for( ii = 0; ii<(int)filetype.size(); ii++ )
		{
			if( tmpType == filetype[ii] )
			{
				bFlag = 1;
				break;
			}
		}
		if( bFlag == 1 )
		{
			SrcFile = srcdir + entryPtr->d_name;
			DesFile = desdir + entryPtr->d_name;
			PF_CopyFileM(SrcFile,DesFile);
		}
    }
    
    closedir( dir );
	
    return 0;
    
#endif
}

char * CPubFile::PF_Error()
{
    return (PF_logstr);
}

void CPubFile::PF_StringTrimAll(string &str)
{
    while(str.find("\n") == 0)
    {
        str.erase(0,1);
    }
    
    while(str.find("\r") == 0)
    {
        str.erase(0,1);
    }
    
    while(str.find(" ") == 0)
    {
        str.erase(0,1);
    }
    
    while(str.find("\t") == 0)
    {
        str.erase(0,1);
    }
    
    while((int)str.rfind("\n") == (int)str.size()-1)
    {
        int l = str.size();
        if(l == 0)
        {
            return;
        }
        str.erase(l-1,1);
    }
    
    while((int)str.rfind("\r") == (int)str.size()-1)
    {
        int l = str.size();
        if(l == 0)
        {
            return;
        }
        str.erase(l-1,1);
    }
    
    while((int)str.rfind(" ") == (int)str.size()-1)
    {
        int l = str.size();
        if(l == 0)
        {
            return;
        }
        str.erase(l-1,1);
    }
    
    while((int)str.rfind("\t") == (int)str.size()-1)
    {
        int l = str.size();
        if(l == 0)
        {
            return;
        }
        str.erase(l-1,1);
    }
    return;
}

int CPubFile::PF_SetFileFormat(const bool ByteOrder, const bool BinFormat )
{
	PF_ByteOrder = ByteOrder;
	PF_BinFormat = BinFormat;

	return 0;
}
////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 不同操作系统下二进制文件的字节序调整
//  处理过程 : 
//  备    注 : 
//  返 回 值 : 无
//  参数说明 : length				字符串长度
//			   wordP				字符串
////////////////////////////////////////////////////////////////////////////////
void CPubFile::PF_SwapByteOrder(int length, void *wordP)
{
    char  temp ;
    
    for ( int i = 0 ; i < length/2 ; i++ )
    {
        temp = ((char *)wordP)[i] ;
        ((char *)wordP)[i] = ((char *)wordP)[length-i-1] ;
        ((char *)wordP)[length-i-1] = temp ;
    }
}

int CPubFile::PF_ReadBinInt(FILE *pFilePtr )
{
    int iValue = 0;
    unsigned char tempsoc[8];
    memset(tempsoc,0,sizeof(tempsoc));
	
    if( pFilePtr == NULL ) return iValue;
	
    fread(tempsoc,1,sizeof(int),pFilePtr);
    if(PF_ByteOrder) PF_SwapByteOrder(sizeof(int), tempsoc);
    memcpy(&iValue, tempsoc, sizeof(int));
	
    return iValue;
}

float CPubFile::PF_ReadBinFloat( FILE *pFilePtr )
{
    float fValue = 0.0f;
    unsigned char tempsoc[8];
    memset(tempsoc,0,sizeof(tempsoc));
	
    if( pFilePtr == NULL ) return fValue;
	
    fread(tempsoc,1,sizeof(float),pFilePtr);    
    if(PF_ByteOrder) PF_SwapByteOrder(sizeof(float), tempsoc);
    memcpy(&fValue, tempsoc, sizeof(float));
    
    return fValue;
}

double CPubFile::PF_ReadBinDouble( FILE *pFilePtr )
{
    double fValue = 0.0;
    unsigned char tempsoc[8];
    memset(tempsoc,0,sizeof(tempsoc));
	
    if( pFilePtr == NULL ) return fValue;
	
    fread(tempsoc, 1, sizeof(double), pFilePtr);    
    if(PF_ByteOrder) PF_SwapByteOrder(sizeof(double), tempsoc);
    memcpy(&fValue, tempsoc, sizeof(double));
	
    return fValue;
}

char * CPubFile::PF_ReadBinChars(char * pBuf, int nMaxLen, FILE *pFilePtr)
{
    if( pFilePtr == NULL ) return NULL;
	
    if (!pBuf) return NULL;
    
    memset(pBuf, 0, nMaxLen);
    
    fread(pBuf, sizeof(char), nMaxLen - 1, pFilePtr);
    
    pBuf[nMaxLen - 1] = '\0';
    
    return pBuf;
}

long CPubFile::PF_FSeek(FILE *pFilePtr)
{
    if( pFilePtr == NULL ) return 0;
	
    fseek(pFilePtr,2*PF_IntSize,SEEK_CUR);
    long curPos = ftell(pFilePtr);
    return curPos;
}

long CPubFile::PF_FSeek(const long &nPos,FILE *pFilePtr)
{
    if( pFilePtr == NULL ) return 0;
	
    fseek(pFilePtr,nPos,SEEK_CUR);
    long curPos = ftell(pFilePtr);
    return curPos;
}

string CPubFile::PF_GetFileName(const string absfilename)
{
	static string filename;
	string::size_type idx = absfilename.find_last_of(PF_backslash);
	if (idx!=string::npos)
	{
		filename = absfilename.substr(idx+1);
	}

	return filename;
}

int CPubFile::PF_MemoryTest( const string proname, const string filename )
{
#ifdef _MEM_TEST_
	char cmdstring[MAXBUF];
	sprintf(cmdstring,"ps u |grep %s > %s",proname.c_str(),filename.c_str());
	system(cmdstring);
#endif
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// CPubLogFile.cpp: implementation of the CPubLogFile class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPubLogFile::CPubLogFile()
{
	// printf("CPubLogFile构造开始\n");
    PLF_LogName.erase();
    PLF_LogDir.erase();
    PLF_LogIdx.erase();
    
    memset(PLF_logstr,0,sizeof(PLF_logstr));
	PLF_LogDir = PRODEFDIR;
	PLF_LogName = PRODEFLOGNAME;
	PLF_LogIdx  = PRODEFLOGIDX;
	PLF_maxBackupNum = MAXBACKUPNUM;
	PLF_maxFileSize = MAXFILESIZE;
	PLF_logtype = LTDEBUG;
	PLF_IsZip = 1;
#ifdef _WINDOWS32
    PLF_mutex = CreateMutex(NULL,FALSE,MUTEXNAME);
    sprintf(PLF_backslash,"%s","\\");
#else
    pthread_mutex_init(&PLF_mutex, NULL);
    sprintf(PLF_backslash,"%s","/");
#endif
}

CPubLogFile::~CPubLogFile()
{
#ifdef _WINDOWS32
    CloseHandle(PLF_mutex);
    PLF_mutex = NULL;
#else
    pthread_mutex_destroy(&PLF_mutex);
#endif
}

int CPubLogFile::PLF_InitLogFile(const std::string& fileName, size_t maxFileSize /* = 10*1024*1024 */, unsigned int maxBackupNum /* = 5 */)
{
    string tempstr;
    tempstr = fileName;

    PLF_StringTrimAll(tempstr);

    string::size_type idx = tempstr.find_last_of(PLF_backslash);
	if (idx == string::npos)
	{
        sprintf( PLF_logstr, "文件名错误[%s],采用默认值",fileName.c_str());
		PLF_ShowLog(PLF_logstr);
		PLF_LogDir = PRODEFDIR;
		PLF_LogName = PRODEFLOGNAME;
		PLF_LogIdx  = PRODEFLOGIDX;
		PLF_maxBackupNum = MAXBACKUPNUM;
		PLF_maxFileSize = MAXFILESIZE;
		PLF_logtype = LTDEBUG;
		PLF_IsZip = 1;
        return -1;
	}

    PLF_LogDir = tempstr.substr(0,idx+1);
    PLF_LogName= tempstr.substr(idx+1);

    tempstr  = tempstr.substr(idx+1);
    idx = tempstr.find_last_of(".");
    PLF_LogIdx = tempstr.substr(0,idx)+LOGIDXSUFF;

    if( PLF_LogDir.empty() || PLF_LogName.empty() || PLF_LogIdx.empty() )
    {
        sprintf( PLF_logstr, "文件名错误[%s],采用默认值",fileName.c_str());
		PLF_ShowLog(PLF_logstr);
		PLF_LogDir = PRODEFDIR;
		PLF_LogName = PRODEFLOGNAME;
		PLF_LogIdx  = PRODEFLOGIDX;
		PLF_maxBackupNum = MAXBACKUPNUM;
		PLF_maxFileSize = MAXFILESIZE;
		PLF_logtype = LTDEBUG;
		PLF_IsZip = 1;
        return -1;
    }
    
	if (maxFileSize > 0 )
	{
		PLF_maxFileSize  = maxFileSize;
	}

	if (maxBackupNum > 0 )
	{
		PLF_maxBackupNum = maxBackupNum;
	}

    if( (PLF_maxFileSize < MAXFILESIZE/1024) || PLF_maxBackupNum<1 )
    {
        PLF_maxFileSize  = MAXFILESIZE;
        PLF_maxBackupNum = MAXBACKUPNUM;
        sprintf(PLF_logstr,"参数设置过小[%d,%d]，采用默认值[%ld,%d]",
            maxFileSize,PLF_maxBackupNum,PLF_maxFileSize,PLF_maxBackupNum);
		PLF_ShowLog(PLF_logstr);
    }

    if( (access( PLF_LogDir.c_str(), 0 )) == -1 )
    {
#ifdef _WINDOWS32
        if( mkdir( PLF_LogDir.c_str() ) == -1 )
#else
        if( mkdir( PLF_LogDir.c_str(),0777 ) == -1 )
#endif
        {
			sprintf(PLF_logstr,"Directory %s was created error",PLF_LogDir.c_str());
			PLF_ShowLog(PLF_logstr);
        }
        else
        {
			sprintf(PLF_logstr,"Directory %s was created successfully",PLF_LogDir.c_str());
			PLF_ShowLog(PLF_logstr);
        }
    }

    return 0;
}
int CPubLogFile::PLF_MessShow( const string logstr ,const int nlogtype )
{
    if( PLF_logtype > nlogtype ) return 0;

    string tmpString = logstr;
    PLF_StringTrimAll(tmpString);

    time_t nowtime;
    time( &nowtime );
    
    char buffer[MAXBUF];
	if ((tmpString.size() + 30) > MAXBUF)
	{
		sprintf(buffer,"%s >>> %s\n",PLF_GetNowTimeString(),tmpString.substr(0,MAXBUF-30).c_str());
		printf(buffer);
		tmpString = tmpString.substr(MAXBUF-30);
		// 循环打印剩余字符
		int nCol = tmpString.size() / (MAXBUF-1);
		for ( int ii =0; ii<nCol; ii++)
		{
			sprintf(buffer,"%s\n",tmpString.substr(0,MAXBUF-1).c_str());
			printf(buffer);
			tmpString = tmpString.substr(MAXBUF-1);
		}
		sprintf(buffer,"%s\n",tmpString.c_str());
		printf(buffer);
	}
	else
	{
		sprintf(buffer,"%s >>> %s\n",PLF_GetNowTimeString(),tmpString.c_str());
		printf(buffer);
	}

	PLF_WriteLogFile( logstr, nlogtype );
    return 0;
}
int CPubLogFile::PLF_WriteLogFile(const string logstr,const int nlogtype )
{
    if( PLF_logtype > nlogtype ) return 0;

	FILE	*log_fp;
    string  tmpFileName = PLF_LogDir+PLF_LogName;
#ifdef _WINDOWS32
    WaitForSingleObject(PLF_mutex, INFINITE);
#else
    pthread_mutex_lock(&PLF_mutex);
#endif
    if( (log_fp=fopen(tmpFileName.c_str(),"a+")) == NULL )
    {
        sprintf(PLF_logstr,"日志文件[%s]打开失败",tmpFileName.c_str());
		PLF_ShowLog(PLF_logstr);
        return -1;
    }

    string tmpLogStr = logstr;
    PLF_StringTrimAll(tmpLogStr);
    char buffer[MAXBUF];
    string tmpType;
	PLF_GetLogTypeStr(nlogtype,tmpType);
	if ((tmpLogStr.size() + 30) > MAXBUF)
	{
		sprintf(buffer,"%s[%s] %s\n",PLF_GetNowTime(),tmpType.c_str(),tmpLogStr.substr(0,MAXBUF-30).c_str());
		fprintf(log_fp,"%s",buffer);
		tmpLogStr = tmpLogStr.substr(MAXBUF-30);
		// 循环打印剩余字符
		int nCol = tmpLogStr.size() / (MAXBUF-1);
		for ( int ii =0; ii<nCol; ii++)
		{
			sprintf(buffer,"%s\n",tmpLogStr.substr(0,MAXBUF-1).c_str());
			fprintf(log_fp,"%s",buffer);
			tmpLogStr = tmpLogStr.substr(MAXBUF-1);
		}
		sprintf(buffer,"%s\n",tmpLogStr.c_str());
		fprintf(log_fp,"%s",buffer);
	}
	else
	{
		sprintf(buffer,"%s[%s] %s\n",PLF_GetNowTime(),tmpType.c_str(),tmpLogStr.c_str());
		fprintf(log_fp,"%s",buffer);
	}

    fclose(log_fp);

    PLF_CheckLogFile();
#ifdef _WINDOWS32
    ReleaseMutex( PLF_mutex );
#else
    pthread_mutex_unlock(&PLF_mutex);
#endif
    return 0;
}    
int CPubLogFile::PLF_CheckLogFile()
{
    //判断文件大小
    struct stat LogStat;
    string tmpLogFile = PLF_LogDir + PLF_LogName;
    int retcode = stat(tmpLogFile.c_str(),&LogStat);
    if( retcode != 0 )
    {
        sprintf(PLF_logstr,"日志文件[%s]信息获取失败",tmpLogFile.c_str());
		PLF_ShowLog(PLF_logstr);
        return -1;
    }
    
    if( LogStat.st_size < PLF_maxFileSize )
    {
        return 0;
    }

    // 创建新文件名
    string OldFileName = PLF_LogDir + PLF_LogName;
    string::size_type idx = OldFileName.find_last_of(".");
    string NewFileName = OldFileName.substr(0,idx) + PLF_GetNowTime1() + OldFileName.substr(idx);

	// 重命名文件
    retcode = rename( OldFileName.c_str(), NewFileName.c_str() );
    if( retcode != 0 )
    {
        sprintf( PLF_logstr,"重命名文件[%s]失败", OldFileName.c_str() );
		PLF_ShowLog(PLF_logstr);
        return -1;
    }
	
	if (PLF_IsZip == 1)
	{
		// 压缩日志文件
#ifdef _WINDOWS32
		//sprintf(tmpCmd,"7z a %s.gz %s",NewFileName.c_str(),NewFileName.c_str());
#else
		char tmpCmd[MAXBUF];
		sprintf(tmpCmd,"gzip %s",NewFileName.c_str());
		retcode = system(tmpCmd);
		if (retcode !=0 )
		{
			sprintf( PLF_logstr,"压缩文件[%s]失败", tmpCmd );
			PLF_ShowLog(PLF_logstr);
		}
		else
		{
			// 更新为压缩文件名
			NewFileName = NewFileName + ".gz";
		}
#endif
	}

    // 读取已有文件索引
    vector<string> vecFileName;
	if (PLF_LogIdx.empty())
	{
		PLF_LogIdx = PRODEFLOGIDX;
	}
    string IdxFileName = PLF_LogDir + PLF_LogIdx;
    fstream idxfpin;
    string  line;
    idxfpin.open( IdxFileName.c_str(), ios::in);
    if ( !idxfpin.fail() )
    {
        while( getline( idxfpin, line) )
        {
            PLF_StringTrimAll(line);
            vecFileName.push_back(line);
        }
    }
    idxfpin.close();

    // 增加新文件索引
    vecFileName.push_back(NewFileName);
    int nFileNum = vecFileName.size();
    int ii =0;

    vector<string> NewVecFileName;

    // 判断文件个数
    int nDeleteNum = nFileNum - PLF_maxBackupNum;
    for( ii = 0; ii< nFileNum; ii++ )
    {
        if( ii < nDeleteNum )
        {
            retcode = remove(vecFileName[ii].c_str());
            if( retcode == -1 )
            {
                sprintf(PLF_logstr,"删除文件[%s]失败",vecFileName[ii].c_str());
				PLF_ShowLog(PLF_logstr);
            }
        }
        else
        {
            NewVecFileName.push_back(vecFileName[ii]);
        }
    }

    // 重新生成索引文件
    nFileNum = NewVecFileName.size();
    fstream idxfpout;
    idxfpout.open(IdxFileName.c_str(),ios::out );
    if ( idxfpout.fail() )
    {
        sprintf(PLF_logstr,"索引文件生成失败[%s]",IdxFileName.c_str());
		PLF_ShowLog(PLF_logstr);
        return -1;
    }
    for(ii = 0; ii<nFileNum; ii++ )
    {
#ifdef _WINDOWS32
        idxfpout << NewVecFileName[ii] << endl;
#else
        idxfpout << NewVecFileName[ii] << '\r' << endl;
#endif
    }
    idxfpout.close();

    vecFileName.clear();
	vector<string>().swap(vecFileName);
    NewVecFileName.clear();
	vector<string>().swap(NewVecFileName);
	line = "";
	IdxFileName = "";
	NewFileName = "";
	OldFileName = "";
	tmpLogFile = "";

    return 0;
}
void CPubLogFile::PLF_setMaxBackupNum(unsigned int maxBackups)
{
    PLF_maxBackupNum = maxBackups;
    if( PLF_maxBackupNum<1 )
    {
        PLF_maxBackupNum = MAXBACKUPNUM;
    }
}
unsigned int CPubLogFile::PLF_getMaxBackupNum()
{
    return PLF_maxBackupNum;
}
void CPubLogFile::PLF_setMaxFileSize(size_t maxFileSize)
{
    PLF_maxFileSize = maxFileSize;
    if( PLF_maxFileSize< MAXFILESIZE/1024 )
    {
        PLF_maxFileSize = MAXFILESIZE;
    }
}
long int CPubLogFile::PLF_getMaxFileSize()
{
    return PLF_maxFileSize;
}
char * CPubLogFile::PLF_Error()
{
    return PLF_logstr;
}
void CPubLogFile::PLF_setLogType(int nlogtype /* = LINFO */ )
{
    PLF_logtype = nlogtype;
    return;
}
int CPubLogFile::PLF_GetLogTypeStr(const int nlogtype,string &LogTypeStr)
{
    switch(nlogtype)
    {
    case LTDEBUG:
        LogTypeStr = "DBUG";
        break;
    case LTINFO:
        LogTypeStr = "INFO";
        break;
    case LTDEV:
        LogTypeStr = "DEVI";
        break;
    case LTWARN:
        LogTypeStr = "WARN";
        break;
    case LTERROR:
        LogTypeStr = "ERRO";
        break;
    default:
        LogTypeStr = "NONE";
        break;
    }

    return 0;
}
int CPubLogFile::PLF_StringTrimAll(string &str)
{
    while(str.find("\n") == 0)
    {
        str.erase(0,1);
    }
    
    while(str.find("\r") == 0)
    {
        str.erase(0,1);
    }
    
    while(str.find(" ") == 0)
    {
        str.erase(0,1);
    }
    
    while(str.find("\t") == 0)
    {
        str.erase(0,1);
    }
    
    while((int)str.rfind("\n") == (int)str.size()-1)
    {
        int l = str.size();
        if(l == 0)
        {
            return 0;
        }
        str.erase(l-1,1);
    }
    
    while((int)str.rfind("\r") == (int)str.size()-1)
    {
        int l = str.size();
        if(l == 0)
        {
            return 0;
        }
        str.erase(l-1,1);
    }
    
    while((int)str.rfind(" ") == (int)str.size()-1)
    {
        int l = str.size();
        if(l == 0)
        {
            return 0;
        }
        str.erase(l-1,1);
    }
    
    while((int)str.rfind("\t") == (int)str.size()-1)
    {
        int l = str.size();
        if(l == 0)
        {
            return 0;
        }
        str.erase(l-1,1);
    }
    return 0;
}
char* CPubLogFile::PLF_GetNowTime( )
{
    static char szTime[20];
    memset(szTime,0,sizeof(szTime));

	struct timeb  t2 ;
	ftime(&t2) ;
	int ms = t2.millitm;

    struct tm *     ptmTime = NULL;
    time_t          nowTime;
    memset(szTime, 0, sizeof(szTime));

    time(&nowTime);
    ptmTime = localtime(&nowTime);
    sprintf(szTime, "%02d-%02d %02d:%02d:%02d[%d]",
        //ptmTime->tm_year + 1900,
        ptmTime->tm_mon  + 1,
        ptmTime->tm_mday,
        ptmTime->tm_hour,
        ptmTime->tm_min,
		ptmTime->tm_sec,ms);

    return szTime;
}
char* CPubLogFile::PLF_GetNowTime1()
{
    static char szTime[20];
    memset(szTime,0,sizeof(szTime));

    struct tm *     ptmTime = NULL;
    time_t          nowTime;
    memset(szTime, 0, sizeof(szTime));
    
    time(&nowTime);
    ptmTime = localtime(&nowTime);
    sprintf(szTime, "_%04d%02d%02d%02d%02d%02d",
        ptmTime->tm_year + 1900,
        ptmTime->tm_mon  + 1,
        ptmTime->tm_mday,
        ptmTime->tm_hour,
        ptmTime->tm_min,
        ptmTime->tm_sec);
    
    return szTime;
}
/*
功能: 将世纪秒转换成字符串（HH:mm:ss）
输入: 需转换的时间
输出: 转换后的时间字符串(HH:mm:ss)
*/
char * CPubLogFile::PLF_GetNowTimeString()
{
	time_t nowtime;
    time( &nowtime );

    static char     szTime[10];
    struct tm *     ptmTime = NULL;
	memset(szTime, 0, sizeof(szTime));
	
	if(((int)nowtime) <= 0)
	{
		sprintf(szTime, "%02d:%02d:%02d",	00, 00, 00);
		
		return szTime;
	}
	
    ptmTime = localtime(&nowtime);
    sprintf(szTime, "%02d:%02d:%02d",
		ptmTime->tm_hour,
		ptmTime->tm_min,
		ptmTime->tm_sec);
	
    return  szTime;
}

int CPubLogFile::PLF_ShowLog( const string logstr )
{
	cout<<logstr<<endl;
	return 0;
}


//////////////////////////////////////////////////////////////////////////
// IniFile.cpp:  Implementation of the CIniFile class.
// Written by:   Adam Clauss
// Email: cabadam@houston.rr.com
// You may use this class/code as you wish in your programs.  Feel free to distribute it, and
// email suggested changes to me.
//
// Rewritten by: Shane Hill
// Date:         21/08/2001
// Email:        Shane.Hill@dsto.defence.gov.au
// Reason:       Remove dependancy on MFC. Code should compile on any
//               platform.
//
// INI file's format: 
// [Section01] 
//  # this is a comment
//      Key01 = value01 ;
//      Key02 = value02 ;
//////////////////////////////////////////////////////////////////////
CIniFile::CIniFile( string const iniPath)
{
  INI_Path( iniPath);
  caseInsensitive = true;
}

bool CIniFile::INI_ReadFile( string const newPath )
{
    INI_Path(newPath);
    return INI_ReadFile();
}

bool CIniFile::INI_ReadFile()
{
  // Normally you would use ifstream, but the SGI CC compiler has
  // a few bugs with ifstream. So ... fstream used.
  fstream f;
  string   line;
  string   keyname, valuename, svalue, svalcmt;
  string::size_type pLeft, pRight, pEnd, pCmt;

  f.open( path.c_str(), ios::in);
  if ( f.fail())
    return false;
  
  while( getline( f, line)) {
      valuename.erase();
      svalue.erase();
      svalcmt.erase();
      pLeft  = string::npos;
      pRight = string::npos;
      pEnd   = string::npos;
      pCmt   = string::npos;

    // To be compatible with Win32, check for existence of '\r'.
    // Win32 files have the '\r' and Unix files don't at the end of a line.
    // Note that the '\r' will be written to INI files from
    // Unix so that the created INI file can be read under Win32
    // without change.
//     if ( line[line.length() - 1] == '\r')
//       line = line.substr( 0, line.length() - 1);
    INI_TrimAll(line);
    
    if ( line.length()) {
      // Check that the user hasn't openned a binary file by checking the first
      // character of each line!
//       if ( !isprint( line[0])) {
// 	printf( "Failing on char %d\n", line[0]);
// 	f.close();
// 	return false;
//       }
      if (( pLeft = line.find_first_of("#[=")) != string::npos) {
	switch ( line[pLeft]) {
	case '[':
	  if ((pRight = line.find_last_of("]")) != string::npos &&
	      pRight > pLeft) {
	    keyname = line.substr( pLeft + 1, pRight - pLeft - 1);
	    INI_AddKeyName( keyname);
	  }
	  break;
	  
	case '=':
	  valuename = line.substr( 0, pLeft);
      pEnd = line.find_first_of(";");
      if( pEnd == string::npos ) break;

      svalue = line.substr( pLeft + 1, pEnd - pLeft -1 );
      pCmt = line.find_first_of("/");
      if( pCmt != string::npos )
      {
          svalcmt = line.substr( pCmt + 1);
      }

      INI_TrimAll(valuename);
      INI_TrimAll(svalue);
      INI_TrimAll(svalcmt);
	  INI_SetValue( keyname, valuename, svalue, svalcmt);
	  break;
	  
	//case ';':
	case '#':
	  if ( !names.size())
	    INI_HeaderComment( line.substr( pLeft + 1));
	  else
	    INI_KeyComment( keyname, line.substr( pLeft + 1));
	  break;
	}
      }
    }
  }

  f.close();
  if ( names.size())
    return true;
  return false;
}

bool CIniFile::INI_ReadTskFile(string const newPath)
{
    // Normally you would use ifstream, but the SGI CC compiler has
    // a few bugs with ifstream. So ... fstream used.
    fstream f;
    string   line;
    string   keyname, valuename, svalue, svalcmt;
    string::size_type pLeft, pRight, pEnd, pCmt;
    
    f.open( newPath.c_str(), ios::in);
    if ( f.fail())
        return false;
    
    while( getline( f, line)) {
        valuename.erase();
        svalue.erase();
        svalcmt.erase();
        pLeft  = string::npos;
        pRight = string::npos;
        pEnd   = string::npos;
        pCmt   = string::npos;
        
        // To be compatible with Win32, check for existence of '\r'.
        // Win32 files have the '\r' and Unix files don't at the end of a line.
        // Note that the '\r' will be written to INI files from
        // Unix so that the created INI file can be read under Win32
        // without change.
        //if ( line[line.length() - 1] == '\r')
        //    line = line.substr( 0, line.length() - 1);
        INI_TrimAll(line);
        
        if ( line.length()) {
            // Check that the user hasn't openned a binary file by checking the first
            // character of each line!
			//             if ( !isprint( line[0])) {
			//                 printf( "Failing on char %d\n", line[0]);
			//                 f.close();
			//                 return false;
			//             }
            if (( pLeft = line.find_first_of("#[=<")) != string::npos) {
                switch ( line[pLeft]) {
				case '<':
					if ((pRight = line.find_last_of(">")) != string::npos &&
                        pRight > pLeft) {
                        keyname = line;
                        INI_AddKeyName( keyname);
                    }
					break;
                case '[':
                    if ((pRight = line.find_last_of("]")) != string::npos &&
                        pRight > pLeft) {
                        keyname = line;
                        INI_AddKeyName( keyname);
                    }
                    break;
                    
                case '=':
                    valuename = line.substr( 0, pLeft);
                    //pEnd = line.find_first_of(";");
                    //if( pEnd == string::npos ) break;
                    
                    svalue = line.substr( pLeft + 1 );
                    //pCmt = line.find_first_of("/");
                    //if( pCmt != string::npos )
                    //{
                    //    svalcmt = line.substr( pCmt + 1);
                    //}
                    
                    INI_TrimAll(valuename);
                    INI_TrimAll(svalue);
                    INI_SetValue( keyname, valuename, svalue);
                    break;
                    
                    //case ';':
                case '#':
					//case '.':
                    if ( !names.size())
                        INI_HeaderComment( line.substr( pLeft + 1));
                    else
                        INI_KeyComment( keyname, line.substr( pLeft + 1));
                    break;
                }
            }
        }
    }
    
    f.close();
    if ( names.size())
        return true;
    return false;
}

bool CIniFile::INI_WriteFile( string const newPath )
{
    INI_Path(newPath);
    return INI_WriteFile();
}

bool CIniFile::INI_WriteFile()
{
  unsigned commentID, keyID, valueID;
  // Normally you would use ofstream, but the SGI CC compiler has
  // a few bugs with ofstream. So ... fstream used.
  fstream f;

  f.open( path.c_str(), ios::out);
  if ( f.fail())
    return false;

  // Write header comments.
  for ( commentID = 0; commentID < comments.size(); ++commentID)
    f << '#' << comments[commentID] << iniEOL;
  if ( comments.size())
    f << iniEOL;

  // Write keys and values.
  for ( keyID = 0; keyID < keys.size(); ++keyID) {
    f << '[' << names[keyID] << ']' << iniEOL;
    // Comments.
    for ( commentID = 0; commentID < keys[keyID].comments.size(); ++commentID)
      f << '#' << keys[keyID].comments[commentID] << iniEOL;
    // Values.
    for ( valueID = 0; valueID < keys[keyID].names.size(); ++valueID)
    {
      f << keys[keyID].names[valueID] << '=' << keys[keyID].values[valueID] << ';';
//       if( !keys[keyID].valcmts[valueID].empty() )
//       {
//          f << "\t\t\/" << keys[keyID].valcmts[valueID] ;
//       }
      f << iniEOL;
    }
    f << iniEOL;
  }
  f.close();
  
  return true;
}

bool CIniFile::INI_WriteTskFile(string const newPath)
{
	unsigned commentID, keyID, valueID;
	// Normally you would use ofstream, but the SGI CC compiler has
	// a few bugs with ofstream. So ... fstream used.
	fstream f;
	
	f.open( newPath.c_str(), ios::out);
	if ( f.fail())
		return false;
	
	// Write header comments.
	for ( commentID = 0; commentID < comments.size(); ++commentID)
		f << '#' << comments[commentID] << iniEOL;
	if ( comments.size())
		f << iniEOL;
	
	// Write keys and values.
	for ( keyID = 0; keyID < keys.size(); ++keyID) {
		f << names[keyID] << iniEOL;
		// Comments.
		for ( commentID = 0; commentID < keys[keyID].comments.size(); ++commentID)
			f << '#' << keys[keyID].comments[commentID] << iniEOL;
		// Values.
		for ( valueID = 0; valueID < keys[keyID].names.size(); ++valueID)
		{
			f << keys[keyID].names[valueID] << " = " << keys[keyID].values[valueID];
			//       if( !keys[keyID].valcmts[valueID].empty() )
			//       {
			//          f << "\t\t\/" << keys[keyID].valcmts[valueID] ;
			//       }
			f << iniEOL;
		}
	}
	
	f << "(END)" << iniEOL;
	f.close();
	
	return true;
}

long CIniFile::INI_FindKey( string const keyname) const
{
  for ( unsigned keyID = 0; keyID < names.size(); ++keyID)
    if ( INI_CheckCase( names[keyID]) == INI_CheckCase( keyname))
      return long(keyID);
  return noID;
}

long CIniFile::INI_FindValue( unsigned const keyID, string const valuename) const
{
  if ( !keys.size() || keyID >= keys.size())
    return noID;

  for ( unsigned valueID = 0; valueID < keys[keyID].names.size(); ++valueID)
    if ( INI_CheckCase( keys[keyID].names[valueID]) == INI_CheckCase( valuename))
      return long(valueID);
  return noID;
}

unsigned CIniFile::INI_AddKeyName( string const keyname)
{
  names.resize( names.size() + 1, keyname);
  keys.resize( keys.size() + 1);
  return names.size() - 1;
}

string CIniFile::INI_KeyName( unsigned const keyID) const
{
  if ( keyID < names.size())
    return names[keyID];
  else
    return "";
}

unsigned CIniFile::INI_NumValues( unsigned const keyID)
{
  if ( keyID < keys.size())
    return keys[keyID].names.size();
  return 0;
}

unsigned CIniFile::INI_NumValues( string const keyname)
{
  long keyID = INI_FindKey( keyname);
  if ( keyID == noID)
    return 0;
  return keys[keyID].names.size();
}

string CIniFile::INI_ValueName( unsigned const keyID, unsigned const valueID) const
{
  if ( keyID < keys.size() && valueID < keys[keyID].names.size())
    return keys[keyID].names[valueID];
  return "";
}

string CIniFile::INI_ValueName( string const keyname, unsigned const valueID) const
{
  long keyID = INI_FindKey( keyname);
  if ( keyID == noID)
    return "";
  return INI_ValueName( keyID, valueID);
}

bool CIniFile::INI_SetValue( unsigned const keyID, unsigned const valueID, string const value)
{
  if ( keyID < keys.size() && valueID < keys[keyID].names.size())
    keys[keyID].values[valueID] = value;

  return false;
}

bool CIniFile::INI_SetValue( string const keyname, string const valuename, string const value, bool const create)
{
  long keyID = INI_FindKey( keyname);
  if ( keyID == noID) {
    if ( create)
      keyID = long( INI_AddKeyName( keyname));
    else
      return false;
  }

  long valueID = INI_FindValue( unsigned(keyID), valuename);
  if ( valueID == noID) {
    if ( !create)
      return false;
    keys[keyID].names.resize( keys[keyID].names.size() + 1, valuename);
    keys[keyID].values.resize( keys[keyID].values.size() + 1, value);
  } else
    keys[keyID].values[valueID] = value;

  return true;
}

bool CIniFile::INI_SetValue( string const keyname, string const valuename, string const value, string const svalcmt, bool const create)
{
    long keyID = INI_FindKey( keyname);
    if ( keyID == noID) {
        if ( create)
            keyID = long( INI_AddKeyName( keyname));
        else
            return false;
    }
    
    long valueID = INI_FindValue( unsigned(keyID), valuename);
    if ( valueID == noID) {
        if ( !create)
            return false;
        keys[keyID].names.resize( keys[keyID].names.size() + 1, valuename);
        keys[keyID].values.resize( keys[keyID].values.size() + 1, value);
        keys[keyID].valcmts.resize( keys[keyID].valcmts.size() + 1, svalcmt );
    } else
    {
        keys[keyID].values[valueID] = value;
        keys[keyID].valcmts[valueID] = svalcmt;
    }
    
    return true;
}

bool CIniFile::INI_SetValueI( string const keyname, string const valuename, int const value, bool const create)
{
  char svalue[MAX_VALUEDATA];

  sprintf( svalue, "%d", value);
  return INI_SetValue( keyname, valuename, svalue);
}

bool CIniFile::INI_SetValueF( string const keyname, string const valuename, double const value, bool const create)
{
  char svalue[MAX_VALUEDATA];

  sprintf( svalue, "%f", value);
  return INI_SetValue( keyname, valuename, svalue);
}

bool CIniFile::INI_SetValueV( string const keyname, string const valuename, char *format, ...)
{
  va_list args;
  char value[MAX_VALUEDATA];

  va_start( args, format);
  vsprintf( value, format, args);
  va_end( args);
  return INI_SetValue( keyname, valuename, value);
}

string CIniFile::INI_GetValue( unsigned const keyID, unsigned const valueID, string const defValue) const
{
  if ( keyID < keys.size() && valueID < keys[keyID].names.size())
    return keys[keyID].values[valueID];
  return defValue;
}

string CIniFile::INI_GetValue( string const keyname, string const valuename, string const defValue) const
{
  long keyID = INI_FindKey( keyname);
  if ( keyID == noID)
    return defValue;

  long valueID = INI_FindValue( unsigned(keyID), valuename);
  if ( valueID == noID)
    return defValue;

  return keys[keyID].values[valueID];
}

int CIniFile::INI_GetValueI(string const keyname, string const valuename, int const defValue) const
{
  char svalue[MAX_VALUEDATA];

  sprintf( svalue, "%d", defValue);
  return atoi( INI_GetValue( keyname, valuename, svalue).c_str()); 
}

int CIniFile::INI_GetValueBI(string const keyname, string const valuename, int const defValue) const
{
	char svalue[MAX_VALUEDATA];
	
	sprintf( svalue, "%d", defValue);
	string tmpvalue = INI_GetValue( keyname, valuename, svalue);
	if ( tmpvalue == "true" || tmpvalue == "TRUE" )
	{
		return 1;
	} 
	else if (tmpvalue == "false" || tmpvalue == "FALSE")
	{
		return 0;
	}
	else
	{
		return defValue;
	}
}

bool CIniFile::INI_GetValueBB(string const keyname, string const valuename, bool const defValue) const
{
	string tmpvalue = INI_GetValue( keyname, valuename);
	if ( tmpvalue == "true" || tmpvalue == "TRUE" )
	{
		return 1;
	} 
	else if (tmpvalue == "false" || tmpvalue == "FALSE")
	{
		return 0;
	}
	else
	{
		return defValue;
	}
}

double CIniFile::INI_GetValueF(string const keyname, string const valuename, double const defValue) const
{
  char svalue[MAX_VALUEDATA];

  sprintf( svalue, "%f", defValue);
  return atof( INI_GetValue( keyname, valuename, svalue).c_str()); 
}

float CIniFile::INI_GetValueFF(string const keyname, string const valuename, double const defValue) const
{
	char svalue[MAX_VALUEDATA];
	
	sprintf( svalue, "%f", defValue);
	return (float)atof( INI_GetValue( keyname, valuename, svalue).c_str()); 
}

// 16 variables may be a bit of over kill, but hey, it's only code.
unsigned CIniFile::INI_GetValueV( string const keyname, string const valuename, char *format,
			      void *v1, void *v2, void *v3, void *v4,
  			      void *v5, void *v6, void *v7, void *v8,
  			      void *v9, void *v10, void *v11, void *v12,
  			      void *v13, void *v14, void *v15, void *v16)
{
  string   value;
  // va_list  args;
  unsigned nVals;


  value = INI_GetValue( keyname, valuename);
  if ( !value.length())
    return false;
  // Why is there not vsscanf() function. Linux man pages say that there is
  // but no compiler I've seen has it defined. Bummer!
  //
  // va_start( args, format);
  // nVals = vsscanf( value.c_str(), format, args);
  // va_end( args);

  nVals = sscanf( value.c_str(), format,
		  v1, v2, v3, v4, v5, v6, v7, v8,
		  v9, v10, v11, v12, v13, v14, v15, v16);

  return nVals;
}

bool CIniFile::INI_DeleteValue( string const keyname, string const valuename)
{
  long keyID = INI_FindKey( keyname);
  if ( keyID == noID)
    return false;

  long valueID = INI_FindValue( unsigned(keyID), valuename);
  if ( valueID == noID)
    return false;

  // This looks strange, but is neccessary.
  vector<string>::iterator npos = keys[keyID].names.begin() + valueID;
  vector<string>::iterator vpos = keys[keyID].values.begin() + valueID;
  keys[keyID].names.erase( npos, npos + 1);
  keys[keyID].values.erase( vpos, vpos + 1);

  return true;
}

bool CIniFile::INI_DeleteKey( string const keyname)
{
  long keyID = INI_FindKey( keyname);
  if ( keyID == noID)
    return false;

  // Now hopefully this destroys the vector lists within keys.
  // Looking at <vector> source, this should be the case using the destructor.
  // If not, I may have to do it explicitly. Memory leak check should tell.
  // memleak_test.cpp shows that the following not required.
  //keys[keyID].names.clear();
  //keys[keyID].values.clear();

  vector<string>::iterator npos = names.begin() + keyID;
  vector<key>::iterator    kpos = keys.begin() + keyID;
  names.erase( npos, npos + 1);
  keys.erase( kpos, kpos + 1);

  return true;
}

void CIniFile::INI_Erase()
{
  // This loop not needed. The vector<> destructor seems to do
  // all the work itself. memleak_test.cpp shows this.
  //for ( unsigned i = 0; i < keys.size(); ++i) {
  //  keys[i].names.clear();
  //  keys[i].values.clear();
  //}
  names.clear();
  keys.clear();
  comments.clear();
}

void CIniFile::INI_HeaderComment( string const comment)
{
  comments.resize( comments.size() + 1, comment);
}

string CIniFile::INI_HeaderComment( unsigned const commentID) const
{
  if ( commentID < comments.size())
    return comments[commentID];
  return "";
}

bool CIniFile::INI_DeleteHeaderComment( unsigned commentID)
{
  if ( commentID < comments.size()) {
    vector<string>::iterator cpos = comments.begin() + commentID;
    comments.erase( cpos, cpos + 1);
    return true;
  }
  return false;
}

unsigned CIniFile::INI_NumKeyComments( unsigned const keyID) const
{
  if ( keyID < keys.size())
    return keys[keyID].comments.size();
  return 0;
}

unsigned CIniFile::INI_NumKeyComments( string const keyname) const
{
  long keyID = INI_FindKey( keyname);
  if ( keyID == noID)
    return 0;
  return keys[keyID].comments.size();
}

bool CIniFile::INI_KeyComment( unsigned const keyID, string const comment)
{
  if ( keyID < keys.size()) {
    keys[keyID].comments.resize( keys[keyID].comments.size() + 1, comment);
    return true;
  }
  return false;
}

bool CIniFile::INI_KeyComment( string const keyname, string const comment)
{
  long keyID = INI_FindKey( keyname);
  if ( keyID == noID)
    return false;
  return INI_KeyComment( unsigned(keyID), comment);
}

string CIniFile::INI_KeyComment( unsigned const keyID, unsigned const commentID) const
{
  if ( keyID < keys.size() && commentID < keys[keyID].comments.size())
    return keys[keyID].comments[commentID];
  return "";
}

string CIniFile::INI_KeyComment( string const keyname, unsigned const commentID) const
{
  long keyID = INI_FindKey( keyname);
  if ( keyID == noID)
    return "";
  return INI_KeyComment( unsigned(keyID), commentID);
}

bool CIniFile::INI_DeleteKeyComment( unsigned const keyID, unsigned const commentID)
{
  if ( keyID < keys.size() && commentID < keys[keyID].comments.size()) {
    vector<string>::iterator cpos = keys[keyID].comments.begin() + commentID;
    keys[keyID].comments.erase( cpos, cpos + 1);
    return true;
  }
  return false;
}

bool CIniFile::INI_DeleteKeyComment( string const keyname, unsigned const commentID)
{
  long keyID = INI_FindKey( keyname);
  if ( keyID == noID)
    return false;
  return INI_DeleteKeyComment( unsigned(keyID), commentID);
}

bool CIniFile::INI_DeleteKeyComments( unsigned const keyID)
{
  if ( keyID < keys.size()) {
    keys[keyID].comments.clear();
    return true;
  }
  return false;
}

bool CIniFile::INI_DeleteKeyComments( string const keyname)
{
  long keyID = INI_FindKey( keyname);
  if ( keyID == noID)
    return false;
  return INI_DeleteKeyComments( unsigned(keyID));
}

string CIniFile::INI_CheckCase( string s) const
{
  if ( caseInsensitive)
    for ( string::size_type i = 0; i < s.length(); ++i)
      s[i] = tolower(s[i]);
  return s;
}
// Helper functions:
// trim left
void CIniFile::INI_LTrim(string& strSrc) 
{
    if(strSrc.empty()) 
        return ; 
    while(strSrc.find("\n") == 0)
    {
        strSrc.erase(0,1);
    }
    
    while(strSrc.find("\r") == 0)
    {
        strSrc.erase(0,1);
    }
    
    while(strSrc.find(" ") == 0)
    {
        strSrc.erase(0,1);
    }
    return ;
}
// trim right
void CIniFile::INI_RTrim(string& strSrc) 
{
    if(strSrc.empty()) 
        return ; 
    while((int)strSrc.rfind("\n") == (int)strSrc.size()-1)
    {
        int l = strSrc.size();
        if(l == 0)
        {
            return ;
        }
        strSrc.erase(l-1,1);
    }
    
    while((int)strSrc.rfind("\r") == (int)strSrc.size()-1)
    {
        int l = strSrc.size();
        if(l == 0)
        {
            return ;
        }
        strSrc.erase(l-1,1);
    }
    
    while((int)strSrc.rfind(" ") == (int)strSrc.size()-1)
    {
        int l = strSrc.size();
        if(l == 0)
        {
            return ;
        }
        strSrc.erase(l-1,1);
    }
    return ;
}
// Trim all
void CIniFile::INI_TrimAll(string &strSrc)
{
    INI_LTrim(strSrc);
    INI_RTrim(strSrc);

	return;
}

//////////////////////////////////////////////////////////////////////////
/*
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2006 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/


FILE* TiXmlFOpen( const char* filename, const char* mode );

bool TiXmlBase::condenseWhiteSpace = true;

// Microsoft compiler security
FILE* TiXmlFOpen( const char* filename, const char* mode )
{
	#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
		FILE* fp = 0;
		errno_t err = fopen_s( &fp, filename, mode );
		if ( !err && fp )
			return fp;
		return 0;
	#else
		return fopen( filename, mode );
	#endif
}

void TiXmlBase::EncodeString( const TIXML_STRING& str, TIXML_STRING* outString )
{
	int i=0;

	while( i<(int)str.length() )
	{
		unsigned char c = (unsigned char) str[i];

		if (    c == '&' 
		     && i < ( (int)str.length() - 2 )
			 && str[i+1] == '#'
			 && str[i+2] == 'x' )
		{
			// Hexadecimal character reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			//
			// The -1 is a bug fix from Rob Laveaux. It keeps
			// an overflow from happening if there is no ';'.
			// There are actually 2 ways to exit this loop -
			// while fails (error case) and break (semicolon found).
			// However, there is no mechanism (currently) for
			// this function to return an error.
			while ( i<(int)str.length()-1 )
			{
				outString->append( str.c_str() + i, 1 );
				++i;
				if ( str[i] == ';' )
					break;
			}
		}
		else if ( c == '&' )
		{
			outString->append( entity[0].str, entity[0].strLength );
			++i;
		}
		else if ( c == '<' )
		{
			outString->append( entity[1].str, entity[1].strLength );
			++i;
		}
		else if ( c == '>' )
		{
			outString->append( entity[2].str, entity[2].strLength );
			++i;
		}
		else if ( c == '\"' )
		{
			outString->append( entity[3].str, entity[3].strLength );
			++i;
		}
		else if ( c == '\'' )
		{
			outString->append( entity[4].str, entity[4].strLength );
			++i;
		}
		else if ( c < 32 )
		{
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			char buf[ 32 ];
			
			#if defined(TIXML_SNPRINTF)		
				TIXML_SNPRINTF( buf, sizeof(buf), "&#x%02X;", (unsigned) ( c & 0xff ) );
			#else
				sprintf( buf, "&#x%02X;", (unsigned) ( c & 0xff ) );
			#endif		

			//*ME:	warning C4267: convert 'size_t' to 'int'
			//*ME:	Int-Cast to make compiler happy ...
			outString->append( buf, (int)strlen( buf ) );
			++i;
		}
		else
		{
			//char realc = (char) c;
			//outString->append( &realc, 1 );
			*outString += (char) c;	// somewhat more efficient function call.
			++i;
		}
	}
}


TiXmlNode::TiXmlNode( NodeType _type ) : TiXmlBase()
{
	parent = 0;
	type = _type;
	firstChild = 0;
	lastChild = 0;
	prev = 0;
	next = 0;
}


TiXmlNode::~TiXmlNode()
{
	TiXmlNode* node = firstChild;
	TiXmlNode* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}	
}


void TiXmlNode::CopyTo( TiXmlNode* target ) const
{
	target->SetValue (value.c_str() );
	target->userData = userData; 
	target->location = location;
}


void TiXmlNode::Clear()
{
	TiXmlNode* node = firstChild;
	TiXmlNode* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}	

	firstChild = 0;
	lastChild = 0;
}


TiXmlNode* TiXmlNode::LinkEndChild( TiXmlNode* node )
{
	assert( node->parent == 0 || node->parent == this );
	assert( node->GetDocument() == 0 || node->GetDocument() == this->GetDocument() );

	if ( node->Type() == TiXmlNode::TINYXML_DOCUMENT )
	{
		delete node;
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return 0;
	}

	node->parent = this;

	node->prev = lastChild;
	node->next = 0;

	if ( lastChild )
		lastChild->next = node;
	else
		firstChild = node;			// it was an empty list.

	lastChild = node;
	return node;
}


TiXmlNode* TiXmlNode::InsertEndChild( const TiXmlNode& addThis )
{
	if ( addThis.Type() == TiXmlNode::TINYXML_DOCUMENT )
	{
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return 0;
	}
	TiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;

	return LinkEndChild( node );
}


TiXmlNode* TiXmlNode::InsertBeforeChild( TiXmlNode* beforeThis, const TiXmlNode& addThis )
{	
	if ( !beforeThis || beforeThis->parent != this ) {
		return 0;
	}
	if ( addThis.Type() == TiXmlNode::TINYXML_DOCUMENT )
	{
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return 0;
	}

	TiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;
	node->parent = this;

	node->next = beforeThis;
	node->prev = beforeThis->prev;
	if ( beforeThis->prev )
	{
		beforeThis->prev->next = node;
	}
	else
	{
		assert( firstChild == beforeThis );
		firstChild = node;
	}
	beforeThis->prev = node;
	return node;
}


TiXmlNode* TiXmlNode::InsertAfterChild( TiXmlNode* afterThis, const TiXmlNode& addThis )
{
	if ( !afterThis || afterThis->parent != this ) {
		return 0;
	}
	if ( addThis.Type() == TiXmlNode::TINYXML_DOCUMENT )
	{
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return 0;
	}

	TiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;
	node->parent = this;

	node->prev = afterThis;
	node->next = afterThis->next;
	if ( afterThis->next )
	{
		afterThis->next->prev = node;
	}
	else
	{
		assert( lastChild == afterThis );
		lastChild = node;
	}
	afterThis->next = node;
	return node;
}


TiXmlNode* TiXmlNode::ReplaceChild( TiXmlNode* replaceThis, const TiXmlNode& withThis )
{
	if ( !replaceThis )
		return 0;

	if ( replaceThis->parent != this )
		return 0;

	if ( withThis.ToDocument() ) {
		// A document can never be a child.	Thanks to Noam.
		TiXmlDocument* document = GetDocument();
		if ( document ) 
			document->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return 0;
	}

	TiXmlNode* node = withThis.Clone();
	if ( !node )
		return 0;

	node->next = replaceThis->next;
	node->prev = replaceThis->prev;

	if ( replaceThis->next )
		replaceThis->next->prev = node;
	else
		lastChild = node;

	if ( replaceThis->prev )
		replaceThis->prev->next = node;
	else
		firstChild = node;

	delete replaceThis;
	node->parent = this;
	return node;
}


bool TiXmlNode::RemoveChild( TiXmlNode* removeThis )
{
	if ( !removeThis ) {
		return false;
	}

	if ( removeThis->parent != this )
	{	
		assert( 0 );
		return false;
	}

	if ( removeThis->next )
		removeThis->next->prev = removeThis->prev;
	else
		lastChild = removeThis->prev;

	if ( removeThis->prev )
		removeThis->prev->next = removeThis->next;
	else
		firstChild = removeThis->next;

	delete removeThis;
	return true;
}

const TiXmlNode* TiXmlNode::FirstChild( const char * _value ) const
{
	const TiXmlNode* node;
	for ( node = firstChild; node; node = node->next )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const TiXmlNode* TiXmlNode::LastChild( const char * _value ) const
{
	const TiXmlNode* node;
	for ( node = lastChild; node; node = node->prev )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const TiXmlNode* TiXmlNode::IterateChildren( const TiXmlNode* previous ) const
{
	if ( !previous )
	{
		return FirstChild();
	}
	else
	{
		assert( previous->parent == this );
		return previous->NextSibling();
	}
}


const TiXmlNode* TiXmlNode::IterateChildren( const char * val, const TiXmlNode* previous ) const
{
	if ( !previous )
	{
		return FirstChild( val );
	}
	else
	{
		assert( previous->parent == this );
		return previous->NextSibling( val );
	}
}


const TiXmlNode* TiXmlNode::NextSibling( const char * _value ) const 
{
	const TiXmlNode* node;
	for ( node = next; node; node = node->next )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const TiXmlNode* TiXmlNode::PreviousSibling( const char * _value ) const
{
	const TiXmlNode* node;
	for ( node = prev; node; node = node->prev )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


void TiXmlElement::RemoveAttribute( const char * name )
{
    #ifdef TIXML_USE_STL
	TIXML_STRING str( name );
	TiXmlAttribute* node = attributeSet.Find( str );
	#else
	TiXmlAttribute* node = attributeSet.Find( name );
	#endif
	if ( node )
	{
		attributeSet.Remove( node );
		delete node;
	}
}

const TiXmlElement* TiXmlNode::FirstChildElement() const
{
	const TiXmlNode* node;

	for (	node = FirstChild();
			node;
			node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const TiXmlElement* TiXmlNode::FirstChildElement( const char * _value ) const
{
	const TiXmlNode* node;

	for (	node = FirstChild( _value );
			node;
			node = node->NextSibling( _value ) )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const TiXmlElement* TiXmlNode::NextSiblingElement() const
{
	const TiXmlNode* node;

	for (	node = NextSibling();
			node;
			node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const TiXmlElement* TiXmlNode::NextSiblingElement( const char * _value ) const
{
	const TiXmlNode* node;

	for (	node = NextSibling( _value );
			node;
			node = node->NextSibling( _value ) )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const TiXmlDocument* TiXmlNode::GetDocument() const
{
	const TiXmlNode* node;

	for( node = this; node; node = node->parent )
	{
		if ( node->ToDocument() )
			return node->ToDocument();
	}
	return 0;
}


TiXmlElement::TiXmlElement (const char * _value)
	: TiXmlNode( TiXmlNode::TINYXML_ELEMENT )
{
	firstChild = lastChild = 0;
	value = _value;
}


#ifdef TIXML_USE_STL
TiXmlElement::TiXmlElement( const std::string& _value ) 
	: TiXmlNode( TiXmlNode::TINYXML_ELEMENT )
{
	firstChild = lastChild = 0;
	value = _value;
}
#endif


TiXmlElement::TiXmlElement( const TiXmlElement& copy)
	: TiXmlNode( TiXmlNode::TINYXML_ELEMENT )
{
	firstChild = lastChild = 0;
	copy.CopyTo( this );	
}


void TiXmlElement::operator=( const TiXmlElement& base )
{
	ClearThis();
	base.CopyTo( this );
}


TiXmlElement::~TiXmlElement()
{
	ClearThis();
}


void TiXmlElement::ClearThis()
{
	Clear();
	while( attributeSet.First() )
	{
		TiXmlAttribute* node = attributeSet.First();
		attributeSet.Remove( node );
		delete node;
	}
}


const char* TiXmlElement::Attribute( const char* name ) const
{
	const TiXmlAttribute* node = attributeSet.Find( name );
	if ( node )
		return node->Value();
	return 0;
}


#ifdef TIXML_USE_STL
const std::string* TiXmlElement::Attribute( const std::string& name ) const
{
	const TiXmlAttribute* attrib = attributeSet.Find( name );
	if ( attrib )
		return &attrib->ValueStr();
	return 0;
}
#endif


const char* TiXmlElement::Attribute( const char* name, int* i ) const
{
	const TiXmlAttribute* attrib = attributeSet.Find( name );
	const char* result = 0;

	if ( attrib ) {
		result = attrib->Value();
		if ( i ) {
			attrib->QueryIntValue( i );
		}
	}
	return result;
}


#ifdef TIXML_USE_STL
const std::string* TiXmlElement::Attribute( const std::string& name, int* i ) const
{
	const TiXmlAttribute* attrib = attributeSet.Find( name );
	const std::string* result = 0;

	if ( attrib ) {
		result = &attrib->ValueStr();
		if ( i ) {
			attrib->QueryIntValue( i );
		}
	}
	return result;
}
#endif


const char* TiXmlElement::Attribute( const char* name, double* d ) const
{
	const TiXmlAttribute* attrib = attributeSet.Find( name );
	const char* result = 0;

	if ( attrib ) {
		result = attrib->Value();
		if ( d ) {
			attrib->QueryDoubleValue( d );
		}
	}
	return result;
}


#ifdef TIXML_USE_STL
const std::string* TiXmlElement::Attribute( const std::string& name, double* d ) const
{
	const TiXmlAttribute* attrib = attributeSet.Find( name );
	const std::string* result = 0;

	if ( attrib ) {
		result = &attrib->ValueStr();
		if ( d ) {
			attrib->QueryDoubleValue( d );
		}
	}
	return result;
}
#endif


int TiXmlElement::QueryIntAttribute( const char* name, int* ival ) const
{
	const TiXmlAttribute* attrib = attributeSet.Find( name );
	if ( !attrib )
		return TIXML_NO_ATTRIBUTE;
	return attrib->QueryIntValue( ival );
}


#ifdef TIXML_USE_STL
int TiXmlElement::QueryIntAttribute( const std::string& name, int* ival ) const
{
	const TiXmlAttribute* attrib = attributeSet.Find( name );
	if ( !attrib )
		return TIXML_NO_ATTRIBUTE;
	return attrib->QueryIntValue( ival );
}
#endif


int TiXmlElement::QueryDoubleAttribute( const char* name, double* dval ) const
{
	const TiXmlAttribute* attrib = attributeSet.Find( name );
	if ( !attrib )
		return TIXML_NO_ATTRIBUTE;
	return attrib->QueryDoubleValue( dval );
}


#ifdef TIXML_USE_STL
int TiXmlElement::QueryDoubleAttribute( const std::string& name, double* dval ) const
{
	const TiXmlAttribute* attrib = attributeSet.Find( name );
	if ( !attrib )
		return TIXML_NO_ATTRIBUTE;
	return attrib->QueryDoubleValue( dval );
}
#endif


void TiXmlElement::SetAttribute( const char * name, int val )
{	
	TiXmlAttribute* attrib = attributeSet.FindOrCreate( name );
	if ( attrib ) {
		attrib->SetIntValue( val );
	}
}


#ifdef TIXML_USE_STL
void TiXmlElement::SetAttribute( const std::string& name, int val )
{	
	TiXmlAttribute* attrib = attributeSet.FindOrCreate( name );
	if ( attrib ) {
		attrib->SetIntValue( val );
	}
}
#endif

void TiXmlElement::SetAttribute( const char * name, long val )
{	
	TiXmlAttribute* attrib = attributeSet.FindOrCreate( name );
	if ( attrib ) {
		attrib->SetLongValue( val );
	}
}


#ifdef TIXML_USE_STL
void TiXmlElement::SetAttribute( const std::string& name, long val )
{	
	TiXmlAttribute* attrib = attributeSet.FindOrCreate( name );
	if ( attrib ) {
		attrib->SetLongValue( val );
	}
}
#endif

void TiXmlElement::SetAttribute( const char * name, double val )
{	
	TiXmlAttribute* attrib = attributeSet.FindOrCreate( name );
	if ( attrib ) {
		attrib->SetDoubleValue( val );
	}
}


#ifdef TIXML_USE_STL
void TiXmlElement::SetAttribute( const std::string& name, double val )
{	
	TiXmlAttribute* attrib = attributeSet.FindOrCreate( name );
	if ( attrib ) {
		attrib->SetDoubleValue( val );
	}
}
#endif

void TiXmlElement::SetDoubleAttribute( const char * name, double val )
{	
	TiXmlAttribute* attrib = attributeSet.FindOrCreate( name );
	if ( attrib ) {
		attrib->SetDoubleValue( val );
	}
}


#ifdef TIXML_USE_STL
void TiXmlElement::SetDoubleAttribute( const std::string& name, double val )
{	
	TiXmlAttribute* attrib = attributeSet.FindOrCreate( name );
	if ( attrib ) {
		attrib->SetDoubleValue( val );
	}
}
#endif 


void TiXmlElement::SetAttribute( const char * cname, const char * cvalue )
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate( cname );
	if ( attrib ) {
		attrib->SetValue( cvalue );
	}
}


#ifdef TIXML_USE_STL
void TiXmlElement::SetAttribute( const std::string& _name, const std::string& _value )
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate( _name );
	if ( attrib ) {
		attrib->SetValue( _value );
	}
}
#endif


void TiXmlElement::Print( FILE* cfile, int depth ) const
{
	int i;
	assert( cfile );
	for ( i=0; i<depth; i++ ) {
		fprintf( cfile, "    " );
	}

	fprintf( cfile, "<%s", value.c_str() );

	const TiXmlAttribute* attrib;
	for ( attrib = attributeSet.First(); attrib; attrib = attrib->Next() )
	{
		fprintf( cfile, " " );
		attrib->Print( cfile, depth );
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	TiXmlNode* node;
	if ( !firstChild )
	{
		fprintf( cfile, " />" );
	}
	else if ( firstChild == lastChild && firstChild->ToText() )
	{
		fprintf( cfile, ">" );
		firstChild->Print( cfile, depth + 1 );
		fprintf( cfile, "</%s>", value.c_str() );
	}
	else
	{
		fprintf( cfile, ">" );

		for ( node = firstChild; node; node=node->NextSibling() )
		{
			if ( !node->ToText() )
			{
				fprintf( cfile, "\n" );
			}
			node->Print( cfile, depth+1 );
		}
		fprintf( cfile, "\n" );
		for( i=0; i<depth; ++i ) {
			fprintf( cfile, "    " );
		}
		fprintf( cfile, "</%s>", value.c_str() );
	}
}


void TiXmlElement::CopyTo( TiXmlElement* target ) const
{
	// superclass:
	TiXmlNode::CopyTo( target );

	// Element class: 
	// Clone the attributes, then clone the children.
	const TiXmlAttribute* attribute = 0;
	for(	attribute = attributeSet.First();
	attribute;
	attribute = attribute->Next() )
	{
		target->SetAttribute( attribute->Name(), attribute->Value() );
	}

	TiXmlNode* node = 0;
	for ( node = firstChild; node; node = node->NextSibling() )
	{
		target->LinkEndChild( node->Clone() );
	}
}

bool TiXmlElement::Accept( TiXmlVisitor* visitor ) const
{
	if ( visitor->VisitEnter( *this, attributeSet.First() ) ) 
	{
		for ( const TiXmlNode* node=FirstChild(); node; node=node->NextSibling() )
		{
			if ( !node->Accept( visitor ) )
				break;
		}
	}
	return visitor->VisitExit( *this );
}


TiXmlNode* TiXmlElement::Clone() const
{
	TiXmlElement* clone = new TiXmlElement( Value() );
	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


const char* TiXmlElement::GetText() const
{
	const TiXmlNode* child = this->FirstChild();
	if ( child ) {
		const TiXmlText* childText = child->ToText();
		if ( childText ) {
			return childText->Value();
		}
	}
	return 0;
}


TiXmlDocument::TiXmlDocument() : TiXmlNode( TiXmlNode::TINYXML_DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
	ClearError();
}

TiXmlDocument::TiXmlDocument( const char * documentName ) : TiXmlNode( TiXmlNode::TINYXML_DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
	value = documentName;
	ClearError();
}


#ifdef TIXML_USE_STL
TiXmlDocument::TiXmlDocument( const std::string& documentName ) : TiXmlNode( TiXmlNode::TINYXML_DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
    value = documentName;
	ClearError();
}
#endif


TiXmlDocument::TiXmlDocument( const TiXmlDocument& copy ) : TiXmlNode( TiXmlNode::TINYXML_DOCUMENT )
{
	copy.CopyTo( this );
}


void TiXmlDocument::operator=( const TiXmlDocument& copy )
{
	Clear();
	copy.CopyTo( this );
}


bool TiXmlDocument::LoadFile( TiXmlEncoding encoding )
{
	return LoadFile( Value(), encoding );
}

bool TiXmlDocument::SaveFileAppend() const
{
	// The old c stuff lives on...
	FILE* fp = TiXmlFOpen( Value(), "a+" );
	if ( fp )
	{
		bool result = SaveFile( fp );
		fclose( fp );
		return result;
	}
	return false;
}

bool TiXmlDocument::SaveFile() const
{
	return SaveFile( Value() );
}

bool TiXmlDocument::LoadFile( const char* _filename, TiXmlEncoding encoding )
{
	TIXML_STRING filename( _filename );
	value = filename;

	// reading in binary mode so that tinyxml can normalize the EOL
	FILE* file = TiXmlFOpen( value.c_str (), "rb" );	

	if ( file )
	{
		bool result = LoadFile( file, encoding );
		fclose( file );
		return result;
	}
	else
	{
		SetError( TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN );
		return false;
	}
}

bool TiXmlDocument::LoadFile( FILE* file, TiXmlEncoding encoding )
{
	if ( !file ) 
	{
		SetError( TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN );
		return false;
	}

	// Delete the existing data:
	Clear();
	location.Clear();

	// Get the file size, so we can pre-allocate the string. HUGE speed impact.
	long length = 0;
	fseek( file, 0, SEEK_END );
	length = ftell( file );
	fseek( file, 0, SEEK_SET );

	// Strange case, but good to handle up front.
	if ( length <= 0 )
	{
		SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return false;
	}

	// Subtle bug here. TinyXml did use fgets. But from the XML spec:
	// 2.11 End-of-Line Handling
	// <snip>
	// <quote>
	// ...the XML processor MUST behave as if it normalized all line breaks in external 
	// parsed entities (including the document entity) on input, before parsing, by translating 
	// both the two-character sequence #xD #xA and any #xD that is not followed by #xA to 
	// a single #xA character.
	// </quote>
	//
	// It is not clear fgets does that, and certainly isn't clear it works cross platform. 
	// Generally, you expect fgets to translate from the convention of the OS to the c/unix
	// convention, and not work generally.

	/*
	while( fgets( buf, sizeof(buf), file ) )
	{
		data += buf;
	}
	*/

	char* buf = new char[ length+1 ];
	buf[0] = 0;

	if ( fread( buf, length, 1, file ) != 1 ) {
		delete [] buf;
		SetError( TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN );
		return false;
	}

	// Process the buffer in place to normalize new lines. (See comment above.)
	// Copies from the 'p' to 'q' pointer, where p can advance faster if
	// a newline-carriage return is hit.
	//
	// Wikipedia:
	// Systems based on ASCII or a compatible character set use either LF  (Line feed, '\n', 0x0A, 10 in decimal) or 
	// CR (Carriage return, '\r', 0x0D, 13 in decimal) individually, or CR followed by LF (CR+LF, 0x0D 0x0A)...
	//		* LF:    Multics, Unix and Unix-like systems (GNU/Linux, AIX, Xenix, Mac OS X, FreeBSD, etc.), BeOS, Amiga, RISC OS, and others
    //		* CR+LF: DEC RT-11 and most other early non-Unix, non-IBM OSes, CP/M, MP/M, DOS, OS/2, Microsoft Windows, Symbian OS
    //		* CR:    Commodore 8-bit machines, Apple II family, Mac OS up to version 9 and OS-9

	const char* p = buf;	// the read head
	char* q = buf;			// the write head
	const char CR = 0x0d;
	const char LF = 0x0a;

	buf[length] = 0;
	while( *p ) {
		assert( p < (buf+length) );
		assert( q <= (buf+length) );
		assert( q <= p );

		if ( *p == CR ) {
			*q++ = LF;
			p++;
			if ( *p == LF ) {		// check for CR+LF (and skip LF)
				p++;
			}
		}
		else {
			*q++ = *p++;
		}
	}
	assert( q <= (buf+length) );
	*q = 0;

	Parse( buf, 0, encoding );

	delete [] buf;
	return !Error();
}


bool TiXmlDocument::SaveFile( const char * filename ) const
{
	// The old c stuff lives on...
	FILE* fp = TiXmlFOpen( filename, "w" );
	if ( fp )
	{
		bool result = SaveFile( fp );
		fclose( fp );
		return result;
	}
	return false;
}


bool TiXmlDocument::SaveFile( FILE* fp ) const
{
	if ( useMicrosoftBOM ) 
	{
		const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
		const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
		const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;

		fputc( TIXML_UTF_LEAD_0, fp );
		fputc( TIXML_UTF_LEAD_1, fp );
		fputc( TIXML_UTF_LEAD_2, fp );
	}
	Print( fp, 0 );
	return (ferror(fp) == 0);
}


void TiXmlDocument::CopyTo( TiXmlDocument* target ) const
{
	TiXmlNode::CopyTo( target );

	target->error = error;
	target->errorId = errorId;
	target->errorDesc = errorDesc;
	target->tabsize = tabsize;
	target->errorLocation = errorLocation;
	target->useMicrosoftBOM = useMicrosoftBOM;

	TiXmlNode* node = 0;
	for ( node = firstChild; node; node = node->NextSibling() )
	{
		target->LinkEndChild( node->Clone() );
	}	
}


TiXmlNode* TiXmlDocument::Clone() const
{
	TiXmlDocument* clone = new TiXmlDocument();
	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void TiXmlDocument::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	for ( const TiXmlNode* node=FirstChild(); node; node=node->NextSibling() )
	{
		node->Print( cfile, depth );
		fprintf( cfile, "\n" );
	}
}


bool TiXmlDocument::Accept( TiXmlVisitor* visitor ) const
{
	if ( visitor->VisitEnter( *this ) )
	{
		for ( const TiXmlNode* node=FirstChild(); node; node=node->NextSibling() )
		{
			if ( !node->Accept( visitor ) )
				break;
		}
	}
	return visitor->VisitExit( *this );
}


const TiXmlAttribute* TiXmlAttribute::Next() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}

/*
TiXmlAttribute* TiXmlAttribute::Next()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}
*/

const TiXmlAttribute* TiXmlAttribute::Previous() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}

/*
TiXmlAttribute* TiXmlAttribute::Previous()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}
*/

void TiXmlAttribute::Print( FILE* cfile, int /*depth*/, TIXML_STRING* str ) const
{
	TIXML_STRING n, v;

	EncodeString( name, &n );
	EncodeString( value, &v );

	if (value.find ('\"') == TIXML_STRING::npos) {
		if ( cfile ) {
		fprintf (cfile, "%s=\"%s\"", n.c_str(), v.c_str() );
		}
		if ( str ) {
			(*str) += n; (*str) += "=\""; (*str) += v; (*str) += "\"";
		}
	}
	else {
		if ( cfile ) {
		fprintf (cfile, "%s='%s'", n.c_str(), v.c_str() );
		}
		if ( str ) {
			(*str) += n; (*str) += "='"; (*str) += v; (*str) += "'";
		}
	}
}


int TiXmlAttribute::QueryIntValue( int* ival ) const
{
	if ( TIXML_SSCANF( value.c_str(), "%d", ival ) == 1 )
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

int TiXmlAttribute::QueryDoubleValue( double* dval ) const
{
	if ( TIXML_SSCANF( value.c_str(), "%lf", dval ) == 1 )
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

void TiXmlAttribute::SetIntValue( int _value )
{
	char buf [64];
	#if defined(TIXML_SNPRINTF)		
		TIXML_SNPRINTF(buf, sizeof(buf), "%d", _value);
	#else
		sprintf (buf, "%d", _value);
	#endif
	SetValue (buf);
}

void TiXmlAttribute::SetLongValue( long _value )
{
	char buf [64];
#if defined(TIXML_SNPRINTF)		
	TIXML_SNPRINTF(buf, sizeof(buf), "%ld", _value);
#else
	sprintf (buf, "%ld", _value);
#endif
	SetValue (buf);
}

void TiXmlAttribute::SetDoubleValue( double _value )
{
	char buf [256];
	#if defined(TIXML_SNPRINTF)		
		TIXML_SNPRINTF( buf, sizeof(buf), "%g", _value);
	#else
		sprintf (buf, "%g", _value);
	#endif
	SetValue (buf);
}

int TiXmlAttribute::IntValue() const
{
	return atoi (value.c_str ());
}

double  TiXmlAttribute::DoubleValue() const
{
	return atof (value.c_str ());
}


TiXmlComment::TiXmlComment( const TiXmlComment& copy ) : TiXmlNode( TiXmlNode::TINYXML_COMMENT )
{
	copy.CopyTo( this );
}


void TiXmlComment::operator=( const TiXmlComment& base )
{
	Clear();
	base.CopyTo( this );
}


void TiXmlComment::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	for ( int i=0; i<depth; i++ )
	{
		fprintf( cfile,  "    " );
	}
	fprintf( cfile, "<!--%s-->", value.c_str() );
}


void TiXmlComment::CopyTo( TiXmlComment* target ) const
{
	TiXmlNode::CopyTo( target );
}


bool TiXmlComment::Accept( TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}


TiXmlNode* TiXmlComment::Clone() const
{
	TiXmlComment* clone = new TiXmlComment();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void TiXmlText::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	if ( cdata )
	{
		int i;
		fprintf( cfile, "\n" );
		for ( i=0; i<depth; i++ ) {
			fprintf( cfile, "    " );
		}
		fprintf( cfile, "<![CDATA[%s]]>\n", value.c_str() );	// unformatted output
	}
	else
	{
		TIXML_STRING buffer;
		EncodeString( value, &buffer );
		fprintf( cfile, "%s", buffer.c_str() );
	}
}


void TiXmlText::CopyTo( TiXmlText* target ) const
{
	TiXmlNode::CopyTo( target );
	target->cdata = cdata;
}


bool TiXmlText::Accept( TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}


TiXmlNode* TiXmlText::Clone() const
{	
	TiXmlText* clone = 0;
	clone = new TiXmlText( "" );

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


TiXmlDeclaration::TiXmlDeclaration( const char * _version,
									const char * _encoding,
									const char * _standalone )
	: TiXmlNode( TiXmlNode::TINYXML_DECLARATION )
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}


#ifdef TIXML_USE_STL
TiXmlDeclaration::TiXmlDeclaration(	const std::string& _version,
									const std::string& _encoding,
									const std::string& _standalone )
	: TiXmlNode( TiXmlNode::TINYXML_DECLARATION )
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}
#endif


TiXmlDeclaration::TiXmlDeclaration( const TiXmlDeclaration& copy )
	: TiXmlNode( TiXmlNode::TINYXML_DECLARATION )
{
	copy.CopyTo( this );	
}


void TiXmlDeclaration::operator=( const TiXmlDeclaration& copy )
{
	Clear();
	copy.CopyTo( this );
}


void TiXmlDeclaration::Print( FILE* cfile, int /*depth*/, TIXML_STRING* str ) const
{
	if ( cfile ) fprintf( cfile, "<?xml " );
	if ( str )	 (*str) += "<?xml ";

	if ( !version.empty() ) {
		if ( cfile ) fprintf (cfile, "version=\"%s\" ", version.c_str ());
		if ( str ) { (*str) += "version=\""; (*str) += version; (*str) += "\" "; }
	}
	if ( !encoding.empty() ) {
		if ( cfile ) fprintf (cfile, "encoding=\"%s\" ", encoding.c_str ());
		if ( str ) { (*str) += "encoding=\""; (*str) += encoding; (*str) += "\" "; }
	}
	if ( !standalone.empty() ) {
		if ( cfile ) fprintf (cfile, "standalone=\"%s\" ", standalone.c_str ());
		if ( str ) { (*str) += "standalone=\""; (*str) += standalone; (*str) += "\" "; }
	}
	if ( cfile ) fprintf( cfile, "?>" );
	if ( str )	 (*str) += "?>";
}


void TiXmlDeclaration::CopyTo( TiXmlDeclaration* target ) const
{
	TiXmlNode::CopyTo( target );

	target->version = version;
	target->encoding = encoding;
	target->standalone = standalone;
}


bool TiXmlDeclaration::Accept( TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}


TiXmlNode* TiXmlDeclaration::Clone() const
{	
	TiXmlDeclaration* clone = new TiXmlDeclaration();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void TiXmlUnknown::Print( FILE* cfile, int depth ) const
{
	for ( int i=0; i<depth; i++ )
		fprintf( cfile, "    " );
	fprintf( cfile, "<%s>", value.c_str() );
}


void TiXmlUnknown::CopyTo( TiXmlUnknown* target ) const
{
	TiXmlNode::CopyTo( target );
}


bool TiXmlUnknown::Accept( TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}


TiXmlNode* TiXmlUnknown::Clone() const
{
	TiXmlUnknown* clone = new TiXmlUnknown();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


TiXmlAttributeSet::TiXmlAttributeSet()
{
	sentinel.next = &sentinel;
	sentinel.prev = &sentinel;
}


TiXmlAttributeSet::~TiXmlAttributeSet()
{
	assert( sentinel.next == &sentinel );
	assert( sentinel.prev == &sentinel );
}


void TiXmlAttributeSet::Add( TiXmlAttribute* addMe )
{
    #ifdef TIXML_USE_STL
	assert( !Find( TIXML_STRING( addMe->Name() ) ) );	// Shouldn't be multiply adding to the set.
	#else
	assert( !Find( addMe->Name() ) );	// Shouldn't be multiply adding to the set.
	#endif

	addMe->next = &sentinel;
	addMe->prev = sentinel.prev;

	sentinel.prev->next = addMe;
	sentinel.prev      = addMe;
}

void TiXmlAttributeSet::Remove( TiXmlAttribute* removeMe )
{
	TiXmlAttribute* node;

	for( node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node == removeMe )
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = 0;
			node->prev = 0;
			return;
		}
	}
	assert( 0 );		// we tried to remove a non-linked attribute.
}


#ifdef TIXML_USE_STL
TiXmlAttribute* TiXmlAttributeSet::Find( const std::string& name ) const
{
	for( TiXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node->name == name )
			return node;
	}
	return 0;
}

TiXmlAttribute* TiXmlAttributeSet::FindOrCreate( const std::string& _name )
{
	TiXmlAttribute* attrib = Find( _name );
	if ( !attrib ) {
		attrib = new TiXmlAttribute();
		Add( attrib );
		attrib->SetName( _name );
	}
	return attrib;
}
#endif


TiXmlAttribute* TiXmlAttributeSet::Find( const char* name ) const
{
	for( TiXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( strcmp( node->name.c_str(), name ) == 0 )
			return node;
	}
	return 0;
}


TiXmlAttribute* TiXmlAttributeSet::FindOrCreate( const char* _name )
{
	TiXmlAttribute* attrib = Find( _name );
	if ( !attrib ) {
		attrib = new TiXmlAttribute();
		Add( attrib );
		attrib->SetName( _name );
	}
	return attrib;
}


#ifdef TIXML_USE_STL	
std::istream& operator>> (std::istream & in, TiXmlNode & base)
{
	TIXML_STRING tag;
	tag.reserve( 8 * 1000 );
	base.StreamIn( &in, &tag );

	base.Parse( tag.c_str(), 0, TIXML_DEFAULT_ENCODING );
	return in;
}
#endif


#ifdef TIXML_USE_STL	
std::ostream& operator<< (std::ostream & out, const TiXmlNode & base)
{
	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	base.Accept( &printer );
	out << printer.Str();

	return out;
}


std::string& operator<< (std::string& out, const TiXmlNode& base )
{
	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	base.Accept( &printer );
	out.append( printer.Str() );

	return out;
}
#endif


TiXmlHandle TiXmlHandle::FirstChild() const
{
	if ( node )
	{
		TiXmlNode* child = node->FirstChild();
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}


TiXmlHandle TiXmlHandle::FirstChild( const char * value ) const
{
	if ( node )
	{
		TiXmlNode* child = node->FirstChild( value );
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}


TiXmlHandle TiXmlHandle::FirstChildElement() const
{
	if ( node )
	{
		TiXmlElement* child = node->FirstChildElement();
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}


TiXmlHandle TiXmlHandle::FirstChildElement( const char * value ) const
{
	if ( node )
	{
		TiXmlElement* child = node->FirstChildElement( value );
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}


TiXmlHandle TiXmlHandle::Child( int count ) const
{
	if ( node )
	{
		int i;
		TiXmlNode* child = node->FirstChild();
		for (	i=0;
				child && i<count;
				child = child->NextSibling(), ++i )
		{
			// nothing
		}
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}


TiXmlHandle TiXmlHandle::Child( const char* value, int count ) const
{
	if ( node )
	{
		int i;
		TiXmlNode* child = node->FirstChild( value );
		for (	i=0;
				child && i<count;
				child = child->NextSibling( value ), ++i )
		{
			// nothing
		}
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}


TiXmlHandle TiXmlHandle::ChildElement( int count ) const
{
	if ( node )
	{
		int i;
		TiXmlElement* child = node->FirstChildElement();
		for (	i=0;
				child && i<count;
				child = child->NextSiblingElement(), ++i )
		{
			// nothing
		}
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}


TiXmlHandle TiXmlHandle::ChildElement( const char* value, int count ) const
{
	if ( node )
	{
		int i;
		TiXmlElement* child = node->FirstChildElement( value );
		for (	i=0;
				child && i<count;
				child = child->NextSiblingElement( value ), ++i )
		{
			// nothing
		}
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}


bool TiXmlPrinter::VisitEnter( const TiXmlDocument& )
{
	return true;
}

bool TiXmlPrinter::VisitExit( const TiXmlDocument& )
{
	return true;
}

bool TiXmlPrinter::VisitEnter( const TiXmlElement& element, const TiXmlAttribute* firstAttribute )
{
	DoIndent();
	buffer += "<";
	buffer += element.Value();

	for( const TiXmlAttribute* attrib = firstAttribute; attrib; attrib = attrib->Next() )
	{
		buffer += " ";
		attrib->Print( 0, 0, &buffer );
	}

	if ( !element.FirstChild() ) 
	{
		buffer += " />";
		DoLineBreak();
	}
	else 
	{
		buffer += ">";
		if (    element.FirstChild()->ToText()
			  && element.LastChild() == element.FirstChild()
			  && element.FirstChild()->ToText()->CDATA() == false )
		{
			simpleTextPrint = true;
			// no DoLineBreak()!
		}
		else
		{
			DoLineBreak();
		}
	}
	++depth;	
	return true;
}


bool TiXmlPrinter::VisitExit( const TiXmlElement& element )
{
	--depth;
	if ( !element.FirstChild() ) 
	{
		// nothing.
	}
	else 
	{
		if ( simpleTextPrint )
		{
			simpleTextPrint = false;
		}
		else
		{
			DoIndent();
		}
		buffer += "</";
		buffer += element.Value();
		buffer += ">";
		DoLineBreak();
	}
	return true;
}


bool TiXmlPrinter::Visit( const TiXmlText& text )
{
	if ( text.CDATA() )
	{
		DoIndent();
		buffer += "<![CDATA[";
		buffer += text.Value();
		buffer += "]]>";
		DoLineBreak();
	}
	else if ( simpleTextPrint )
	{
		TIXML_STRING str;
		TiXmlBase::EncodeString( text.ValueTStr(), &str );
		buffer += str;
	}
	else
	{
		DoIndent();
		TIXML_STRING str;
		TiXmlBase::EncodeString( text.ValueTStr(), &str );
		buffer += str;
		DoLineBreak();
	}
	return true;
}


bool TiXmlPrinter::Visit( const TiXmlDeclaration& declaration )
{
	DoIndent();
	declaration.Print( 0, 0, &buffer );
	DoLineBreak();
	return true;
}


bool TiXmlPrinter::Visit( const TiXmlComment& comment )
{
	DoIndent();
	buffer += "<!--";
	buffer += comment.Value();
	buffer += "-->";
	DoLineBreak();
	return true;
}


bool TiXmlPrinter::Visit( const TiXmlUnknown& unknown )
{
	DoIndent();
	buffer += "<";
	buffer += unknown.Value();
	buffer += ">";
	DoLineBreak();
	return true;
}
//////////////////////////////////////////////////////////////////////////
const char* TiXmlBase::errorString[ TIXML_ERROR_STRING_COUNT ] =
{
	"No error",
		"Error",
		"Failed to open file",
		"Error parsing Element.",
		"Failed to read Element name",
		"Error reading Element value.",
		"Error reading Attributes.",
		"Error: empty tag.",
		"Error reading end tag.",
		"Error parsing Unknown.",
		"Error parsing Comment.",
		"Error parsing Declaration.",
		"Error document empty.",
		"Error null (0) or unexpected EOF found in input stream.",
		"Error parsing CDATA.",
		"Error when TiXmlDocument added to document, because TiXmlDocument can only be at the root.",
};

//////////////////////////////////////////////////////////////////////////
// Note tha "PutString" hardcodes the same list. This
// is less flexible than it appears. Changing the entries
// or order will break putstring.	
TiXmlBase::Entity TiXmlBase::entity[ NUM_ENTITY ] = 
{
	{ "&amp;",  5, '&' },
	{ "&lt;",   4, '<' },
	{ "&gt;",   4, '>' },
	{ "&quot;", 6, '\"' },
	{ "&apos;", 6, '\'' }
};

// Bunch of unicode info at:
//		http://www.unicode.org/faq/utf_bom.html
// Including the basic of this table, which determines the #bytes in the
// sequence from the lead byte. 1 placed for invalid sequences --
// although the result will be junk, pass it through as much as possible.
// Beware of the non-characters in UTF-8:	
//				ef bb bf (Microsoft "lead bytes")
//				ef bf be
//				ef bf bf 

const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;

const int TiXmlBase::utf8ByteTable[256] = 
{
	//	0	1	2	3	4	5	6	7	8	9	a	b	c	d	e	f
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x00
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x10
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x20
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x30
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x40
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x50
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x60
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x70	End of ASCII range
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x80 0x80 to 0xc1 invalid
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x90 
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0xa0 
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0xb0 
		1,	1,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	// 0xc0 0xc2 to 0xdf 2 byte
		2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	// 0xd0
		3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	// 0xe0 0xe0 to 0xef 3 byte
		4,	4,	4,	4,	4,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1	// 0xf0 0xf0 to 0xf4 4 byte, 0xf5 and higher invalid
};


void TiXmlBase::ConvertUTF32ToUTF8( unsigned long input, char* output, int* length )
{
	const unsigned long BYTE_MASK = 0xBF;
	const unsigned long BYTE_MARK = 0x80;
	const unsigned long FIRST_BYTE_MARK[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

	if (input < 0x80) 
		*length = 1;
	else if ( input < 0x800 )
		*length = 2;
	else if ( input < 0x10000 )
		*length = 3;
	else if ( input < 0x200000 )
		*length = 4;
	else
		{ *length = 0; return; }	// This code won't covert this correctly anyway.

	output += *length;

	// Scary scary fall throughs.
	switch (*length) 
	{
		case 4:
			--output; 
			*output = (char)((input | BYTE_MARK) & BYTE_MASK); 
			input >>= 6;
		case 3:
			--output; 
			*output = (char)((input | BYTE_MARK) & BYTE_MASK); 
			input >>= 6;
		case 2:
			--output; 
			*output = (char)((input | BYTE_MARK) & BYTE_MASK); 
			input >>= 6;
		case 1:
			--output; 
			*output = (char)(input | FIRST_BYTE_MARK[*length]);
	}
}


/*static*/ int TiXmlBase::IsAlpha( unsigned char anyByte, TiXmlEncoding /*encoding*/ )
{
	// This will only work for low-ascii, everything else is assumed to be a valid
	// letter. I'm not sure this is the best approach, but it is quite tricky trying
	// to figure out alhabetical vs. not across encoding. So take a very 
	// conservative approach.

//	if ( encoding == TIXML_ENCODING_UTF8 )
//	{
		if ( anyByte < 127 )
			return isalpha( anyByte );
		else
			return 1;	// What else to do? The unicode set is huge...get the english ones right.
//	}
//	else
//	{
//		return isalpha( anyByte );
//	}
}


/*static*/ int TiXmlBase::IsAlphaNum( unsigned char anyByte, TiXmlEncoding /*encoding*/ )
{
	// This will only work for low-ascii, everything else is assumed to be a valid
	// letter. I'm not sure this is the best approach, but it is quite tricky trying
	// to figure out alhabetical vs. not across encoding. So take a very 
	// conservative approach.

//	if ( encoding == TIXML_ENCODING_UTF8 )
//	{
		if ( anyByte < 127 )
			return isalnum( anyByte );
		else
			return 1;	// What else to do? The unicode set is huge...get the english ones right.
//	}
//	else
//	{
//		return isalnum( anyByte );
//	}
}


class TiXmlParsingData
{
	friend class TiXmlDocument;
  public:
	void Stamp( const char* now, TiXmlEncoding encoding );

	const TiXmlCursor& Cursor()	{ return cursor; }

  private:
	// Only used by the document!
	TiXmlParsingData( const char* start, int _tabsize, int row, int col )
	{
		assert( start );
		stamp = start;
		tabsize = _tabsize;
		cursor.row = row;
		cursor.col = col;
	}

	TiXmlCursor		cursor;
	const char*		stamp;
	int				tabsize;
};


void TiXmlParsingData::Stamp( const char* now, TiXmlEncoding encoding )
{
	assert( now );

	// Do nothing if the tabsize is 0.
	if ( tabsize < 1 )
	{
		return;
	}

	// Get the current row, column.
	int row = cursor.row;
	int col = cursor.col;
	const char* p = stamp;
	assert( p );

	while ( p < now )
	{
		// Treat p as unsigned, so we have a happy compiler.
		const unsigned char* pU = (const unsigned char*)p;

		// Code contributed by Fletcher Dunn: (modified by lee)
		switch (*pU) {
			case 0:
				// We *should* never get here, but in case we do, don't
				// advance past the terminating null character, ever
				return;

			case '\r':
				// bump down to the next line
				++row;
				col = 0;				
				// Eat the character
				++p;

				// Check for \r\n sequence, and treat this as a single character
				if (*p == '\n') {
					++p;
				}
				break;

			case '\n':
				// bump down to the next line
				++row;
				col = 0;

				// Eat the character
				++p;

				// Check for \n\r sequence, and treat this as a single
				// character.  (Yes, this bizarre thing does occur still
				// on some arcane platforms...)
				if (*p == '\r') {
					++p;
				}
				break;

			case '\t':
				// Eat the character
				++p;

				// Skip to next tab stop
				col = (col / tabsize + 1) * tabsize;
				break;

			case TIXML_UTF_LEAD_0:
				if ( encoding == TIXML_ENCODING_UTF8 )
				{
					if ( *(p+1) && *(p+2) )
					{
						// In these cases, don't advance the column. These are
						// 0-width spaces.
						if ( *(pU+1)==TIXML_UTF_LEAD_1 && *(pU+2)==TIXML_UTF_LEAD_2 )
							p += 3;	
						else if ( *(pU+1)==0xbfU && *(pU+2)==0xbeU )
							p += 3;	
						else if ( *(pU+1)==0xbfU && *(pU+2)==0xbfU )
							p += 3;	
						else
							{ p +=3; ++col; }	// A normal character.
					}
				}
				else
				{
					++p;
					++col;
				}
				break;

			default:
				if ( encoding == TIXML_ENCODING_UTF8 )
				{
					// Eat the 1 to 4 byte utf8 character.
					int step = TiXmlBase::utf8ByteTable[*((const unsigned char*)p)];
					if ( step == 0 )
						step = 1;		// Error case from bad encoding, but handle gracefully.
					p += step;

					// Just advance one column, of course.
					++col;
				}
				else
				{
					++p;
					++col;
				}
				break;
		}
	}
	cursor.row = row;
	cursor.col = col;
	assert( cursor.row >= -1 );
	assert( cursor.col >= -1 );
	stamp = p;
	assert( stamp );
}


const char* TiXmlBase::SkipWhiteSpace( const char* p, TiXmlEncoding encoding )
{
	if ( !p || !*p )
	{
		return 0;
	}
	if ( encoding == TIXML_ENCODING_UTF8 )
	{
		while ( *p )
		{
			const unsigned char* pU = (const unsigned char*)p;
			
			// Skip the stupid Microsoft UTF-8 Byte order marks
			if (	*(pU+0)==TIXML_UTF_LEAD_0
				 && *(pU+1)==TIXML_UTF_LEAD_1 
				 && *(pU+2)==TIXML_UTF_LEAD_2 )
			{
				p += 3;
				continue;
			}
			else if(*(pU+0)==TIXML_UTF_LEAD_0
				 && *(pU+1)==0xbfU
				 && *(pU+2)==0xbeU )
			{
				p += 3;
				continue;
			}
			else if(*(pU+0)==TIXML_UTF_LEAD_0
				 && *(pU+1)==0xbfU
				 && *(pU+2)==0xbfU )
			{
				p += 3;
				continue;
			}

			if ( IsWhiteSpace( *p ) )		// Still using old rules for white space.
				++p;
			else
				break;
		}
	}
	else
	{
		while ( *p && IsWhiteSpace( *p ) )
			++p;
	}

	return p;
}

#ifdef TIXML_USE_STL
/*static*/ bool TiXmlBase::StreamWhiteSpace( std::istream * in, TIXML_STRING * tag )
{
	for( ;; )
	{
		if ( !in->good() ) return false;

		int c = in->peek();
		// At this scope, we can't get to a document. So fail silently.
		if ( !IsWhiteSpace( c ) || c <= 0 )
			return true;

		*tag += (char) in->get();
	}
}

/*static*/ bool TiXmlBase::StreamTo( std::istream * in, int character, TIXML_STRING * tag )
{
	//assert( character > 0 && character < 128 );	// else it won't work in utf-8
	while ( in->good() )
	{
		int c = in->peek();
		if ( c == character )
			return true;
		if ( c <= 0 )		// Silent failure: can't get document at this scope
			return false;

		in->get();
		*tag += (char) c;
	}
	return false;
}
#endif

// One of TinyXML's more performance demanding functions. Try to keep the memory overhead down. The
// "assign" optimization removes over 10% of the execution time.
//
const char* TiXmlBase::ReadName( const char* p, TIXML_STRING * name, TiXmlEncoding encoding )
{
	// Oddly, not supported on some comilers,
	//name->clear();
	// So use this:
	*name = "";
	assert( p );

	// Names start with letters or underscores.
	// Of course, in unicode, tinyxml has no idea what a letter *is*. The
	// algorithm is generous.
	//
	// After that, they can be letters, underscores, numbers,
	// hyphens, or colons. (Colons are valid ony for namespaces,
	// but tinyxml can't tell namespaces from names.)
	if (    p && *p 
		 && ( IsAlpha( (unsigned char) *p, encoding ) || *p == '_' ) )
	{
		const char* start = p;
		while(		p && *p
				&&	(		IsAlphaNum( (unsigned char ) *p, encoding ) 
						 || *p == '_'
						 || *p == '-'
						 || *p == '.'
						 || *p == ':' ) )
		{
			//(*name) += *p; // expensive
			++p;
		}
		if ( p-start > 0 ) {
			name->assign( start, p-start );
		}
		return p;
	}
	return 0;
}

const char* TiXmlBase::GetEntity( const char* p, char* value, int* length, TiXmlEncoding encoding )
{
	// Presume an entity, and pull it out.
    TIXML_STRING ent;
	int i;
	*length = 0;

	if ( *(p+1) && *(p+1) == '#' && *(p+2) )
	{
		unsigned long ucs = 0;
		ptrdiff_t delta = 0;
		unsigned mult = 1;

		if ( *(p+2) == 'x' )
		{
			// Hexadecimal.
			if ( !*(p+3) ) return 0;

			const char* q = p+3;
			q = strchr( q, ';' );

			if ( !q || !*q ) return 0;

			delta = q-p;
			--q;

			while ( *q != 'x' )
			{
				if ( *q >= '0' && *q <= '9' )
					ucs += mult * (*q - '0');
				else if ( *q >= 'a' && *q <= 'f' )
					ucs += mult * (*q - 'a' + 10);
				else if ( *q >= 'A' && *q <= 'F' )
					ucs += mult * (*q - 'A' + 10 );
				else 
					return 0;
				mult *= 16;
				--q;
			}
		}
		else
		{
			// Decimal.
			if ( !*(p+2) ) return 0;

			const char* q = p+2;
			q = strchr( q, ';' );

			if ( !q || !*q ) return 0;

			delta = q-p;
			--q;

			while ( *q != '#' )
			{
				if ( *q >= '0' && *q <= '9' )
					ucs += mult * (*q - '0');
				else 
					return 0;
				mult *= 10;
				--q;
			}
		}
		if ( encoding == TIXML_ENCODING_UTF8 )
		{
			// convert the UCS to UTF-8
			ConvertUTF32ToUTF8( ucs, value, length );
		}
		else
		{
			*value = (char)ucs;
			*length = 1;
		}
		return p + delta + 1;
	}

	// Now try to match it.
	for( i=0; i<NUM_ENTITY; ++i )
	{
		if ( strncmp( entity[i].str, p, entity[i].strLength ) == 0 )
		{
			assert( strlen( entity[i].str ) == entity[i].strLength );
			*value = entity[i].chr;
			*length = 1;
			return ( p + entity[i].strLength );
		}
	}

	// So it wasn't an entity, its unrecognized, or something like that.
	*value = *p;	// Don't put back the last one, since we return it!
	//*length = 1;	// Leave unrecognized entities - this doesn't really work.
					// Just writes strange XML.
	return p+1;
}


bool TiXmlBase::StringEqual( const char* p,
							 const char* tag,
							 bool ignoreCase,
							 TiXmlEncoding encoding )
{
	assert( p );
	assert( tag );
	if ( !p || !*p )
	{
		assert( 0 );
		return false;
	}

	const char* q = p;

	if ( ignoreCase )
	{
		while ( *q && *tag && ToLower( *q, encoding ) == ToLower( *tag, encoding ) )
		{
			++q;
			++tag;
		}

		if ( *tag == 0 )
			return true;
	}
	else
	{
		while ( *q && *tag && *q == *tag )
		{
			++q;
			++tag;
		}

		if ( *tag == 0 )		// Have we found the end of the tag, and everything equal?
			return true;
	}
	return false;
}

const char* TiXmlBase::ReadText(	const char* p, 
									TIXML_STRING * text, 
									bool trimWhiteSpace, 
									const char* endTag, 
									bool caseInsensitive,
									TiXmlEncoding encoding )
{
    *text = "";
	if (    !trimWhiteSpace			// certain tags always keep whitespace
		 || !condenseWhiteSpace )	// if true, whitespace is always kept
	{
		// Keep all the white space.
		while (	   p && *p
				&& !StringEqual( p, endTag, caseInsensitive, encoding )
			  )
		{
			int len;
			char cArr[4] = { 0, 0, 0, 0 };
			p = GetChar( p, cArr, &len, encoding );
			text->append( cArr, len );
		}
	}
	else
	{
		bool whitespace = false;

		// Remove leading white space:
		p = SkipWhiteSpace( p, encoding );
		while (	   p && *p
				&& !StringEqual( p, endTag, caseInsensitive, encoding ) )
		{
			if ( *p == '\r' || *p == '\n' )
			{
				whitespace = true;
				++p;
			}
			else if ( IsWhiteSpace( *p ) )
			{
				whitespace = true;
				++p;
			}
			else
			{
				// If we've found whitespace, add it before the
				// new character. Any whitespace just becomes a space.
				if ( whitespace )
				{
					(*text) += ' ';
					whitespace = false;
				}
				int len;
				char cArr[4] = { 0, 0, 0, 0 };
				p = GetChar( p, cArr, &len, encoding );
				if ( len == 1 )
					(*text) += cArr[0];	// more efficient
				else
					text->append( cArr, len );
			}
		}
	}
	if ( p && *p ) 
		p += strlen( endTag );
	return p;
}

#ifdef TIXML_USE_STL

void TiXmlDocument::StreamIn( std::istream * in, TIXML_STRING * tag )
{
	// The basic issue with a document is that we don't know what we're
	// streaming. Read something presumed to be a tag (and hope), then
	// identify it, and call the appropriate stream method on the tag.
	//
	// This "pre-streaming" will never read the closing ">" so the
	// sub-tag can orient itself.

	if ( !StreamTo( in, '<', tag ) ) 
	{
		SetError( TIXML_ERROR_PARSING_EMPTY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return;
	}

	while ( in->good() )
	{
		int tagIndex = (int) tag->length();
		while ( in->good() && in->peek() != '>' )
		{
			int c = in->get();
			if ( c <= 0 )
			{
				SetError( TIXML_ERROR_EMBEDDED_NULL, 0, 0, TIXML_ENCODING_UNKNOWN );
				break;
			}
			(*tag) += (char) c;
		}

		if ( in->good() )
		{
			// We now have something we presume to be a node of 
			// some sort. Identify it, and call the node to
			// continue streaming.
			TiXmlNode* node = Identify( tag->c_str() + tagIndex, TIXML_DEFAULT_ENCODING );

			if ( node )
			{
				node->StreamIn( in, tag );
				bool isElement = node->ToElement() != 0;
				delete node;
				node = 0;

				// If this is the root element, we're done. Parsing will be
				// done by the >> operator.
				if ( isElement )
				{
					return;
				}
			}
			else
			{
				SetError( TIXML_ERROR, 0, 0, TIXML_ENCODING_UNKNOWN );
				return;
			}
		}
	}
	// We should have returned sooner.
	SetError( TIXML_ERROR, 0, 0, TIXML_ENCODING_UNKNOWN );
}

#endif

const char* TiXmlDocument::Parse( const char* p, TiXmlParsingData* prevData, TiXmlEncoding encoding )
{
	ClearError();

	// Parse away, at the document level. Since a document
	// contains nothing but other tags, most of what happens
	// here is skipping white space.
	if ( !p || !*p )
	{
		SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return 0;
	}

	// Note that, for a document, this needs to come
	// before the while space skip, so that parsing
	// starts from the pointer we are given.
	location.Clear();
	if ( prevData )
	{
		location.row = prevData->cursor.row;
		location.col = prevData->cursor.col;
	}
	else
	{
		location.row = 0;
		location.col = 0;
	}
	TiXmlParsingData data( p, TabSize(), location.row, location.col );
	location = data.Cursor();

	if ( encoding == TIXML_ENCODING_UNKNOWN )
	{
		// Check for the Microsoft UTF-8 lead bytes.
		const unsigned char* pU = (const unsigned char*)p;
		if (	*(pU+0) && *(pU+0) == TIXML_UTF_LEAD_0
			 && *(pU+1) && *(pU+1) == TIXML_UTF_LEAD_1
			 && *(pU+2) && *(pU+2) == TIXML_UTF_LEAD_2 )
		{
			encoding = TIXML_ENCODING_UTF8;
			useMicrosoftBOM = true;
		}
	}

    p = SkipWhiteSpace( p, encoding );
	if ( !p )
	{
		SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return 0;
	}

	while ( p && *p )
	{
		TiXmlNode* node = Identify( p, encoding );
		if ( node )
		{
			p = node->Parse( p, &data, encoding );
			LinkEndChild( node );
		}
		else
		{
			break;
		}

		// Did we get encoding info?
		if (    encoding == TIXML_ENCODING_UNKNOWN
			 && node->ToDeclaration() )
		{
			TiXmlDeclaration* dec = node->ToDeclaration();
			const char* enc = dec->Encoding();
			assert( enc );

			if ( *enc == 0 )
				encoding = TIXML_ENCODING_UTF8;
			else if ( StringEqual( enc, "UTF-8", true, TIXML_ENCODING_UNKNOWN ) )
				encoding = TIXML_ENCODING_UTF8;
			else if ( StringEqual( enc, "UTF8", true, TIXML_ENCODING_UNKNOWN ) )
				encoding = TIXML_ENCODING_UTF8;	// incorrect, but be nice
			else 
				encoding = TIXML_ENCODING_LEGACY;
		}

		p = SkipWhiteSpace( p, encoding );
	}

	// Was this empty?
	if ( !firstChild ) {
		SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, encoding );
		return 0;
	}

	// All is well.
	return p;
}

void TiXmlDocument::SetError( int err, const char* pError, TiXmlParsingData* data, TiXmlEncoding encoding )
{	
	// The first error in a chain is more accurate - don't set again!
	if ( error )
		return;

	assert( err > 0 && err < TIXML_ERROR_STRING_COUNT );
	error   = true;
	errorId = err;
	errorDesc = errorString[ errorId ];

	errorLocation.Clear();
	if ( pError && data )
	{
		data->Stamp( pError, encoding );
		errorLocation = data->Cursor();
	}
}


TiXmlNode* TiXmlNode::Identify( const char* p, TiXmlEncoding encoding )
{
	TiXmlNode* returnNode = 0;

	p = SkipWhiteSpace( p, encoding );
	if( !p || !*p || *p != '<' )
	{
		return 0;
	}

	p = SkipWhiteSpace( p, encoding );

	if ( !p || !*p )
	{
		return 0;
	}

	// What is this thing? 
	// - Elements start with a letter or underscore, but xml is reserved.
	// - Comments: <!--
	// - Decleration: <?xml
	// - Everthing else is unknown to tinyxml.
	//

	const char* xmlHeader = { "<?xml" };
	const char* commentHeader = { "<!--" };
	const char* dtdHeader = { "<!" };
	const char* cdataHeader = { "<![CDATA[" };

	if ( StringEqual( p, xmlHeader, true, encoding ) )
	{
		#ifdef DEBUG_PARSER
			TIXML_LOG( "XML parsing Declaration\n" );
		#endif
		returnNode = new TiXmlDeclaration();
	}
	else if ( StringEqual( p, commentHeader, false, encoding ) )
	{
		#ifdef DEBUG_PARSER
			TIXML_LOG( "XML parsing Comment\n" );
		#endif
		returnNode = new TiXmlComment();
	}
	else if ( StringEqual( p, cdataHeader, false, encoding ) )
	{
		#ifdef DEBUG_PARSER
			TIXML_LOG( "XML parsing CDATA\n" );
		#endif
		TiXmlText* text = new TiXmlText( "" );
		text->SetCDATA( true );
		returnNode = text;
	}
	else if ( StringEqual( p, dtdHeader, false, encoding ) )
	{
		#ifdef DEBUG_PARSER
			TIXML_LOG( "XML parsing Unknown(1)\n" );
		#endif
		returnNode = new TiXmlUnknown();
	}
	else if (    IsAlpha( *(p+1), encoding )
			  || *(p+1) == '_' )
	{
		#ifdef DEBUG_PARSER
			TIXML_LOG( "XML parsing Element\n" );
		#endif
		returnNode = new TiXmlElement( "" );
	}
	else
	{
		#ifdef DEBUG_PARSER
			TIXML_LOG( "XML parsing Unknown(2)\n" );
		#endif
		returnNode = new TiXmlUnknown();
	}

	if ( returnNode )
	{
		// Set the parent, so it can report errors
		returnNode->parent = this;
	}
	return returnNode;
}

#ifdef TIXML_USE_STL

void TiXmlElement::StreamIn (std::istream * in, TIXML_STRING * tag)
{
	// We're called with some amount of pre-parsing. That is, some of "this"
	// element is in "tag". Go ahead and stream to the closing ">"
	while( in->good() )
	{
		int c = in->get();
		if ( c <= 0 )
		{
			TiXmlDocument* document = GetDocument();
			if ( document )
				document->SetError( TIXML_ERROR_EMBEDDED_NULL, 0, 0, TIXML_ENCODING_UNKNOWN );
			return;
		}
		(*tag) += (char) c ;
		
		if ( c == '>' )
			break;
	}

	if ( tag->length() < 3 ) return;

	// Okay...if we are a "/>" tag, then we're done. We've read a complete tag.
	// If not, identify and stream.

	if (    tag->at( tag->length() - 1 ) == '>' 
		 && tag->at( tag->length() - 2 ) == '/' )
	{
		// All good!
		return;
	}
	else if ( tag->at( tag->length() - 1 ) == '>' )
	{
		// There is more. Could be:
		//		text
		//		cdata text (which looks like another node)
		//		closing tag
		//		another node.
		for ( ;; )
		{
			StreamWhiteSpace( in, tag );

			// Do we have text?
			if ( in->good() && in->peek() != '<' ) 
			{
				// Yep, text.
				TiXmlText text( "" );
				text.StreamIn( in, tag );

				// What follows text is a closing tag or another node.
				// Go around again and figure it out.
				continue;
			}

			// We now have either a closing tag...or another node.
			// We should be at a "<", regardless.
			if ( !in->good() ) return;
			assert( in->peek() == '<' );
			int tagIndex = (int) tag->length();

			bool closingTag = false;
			bool firstCharFound = false;

			for( ;; )
			{
				if ( !in->good() )
					return;

				int c = in->peek();
				if ( c <= 0 )
				{
					TiXmlDocument* document = GetDocument();
					if ( document )
						document->SetError( TIXML_ERROR_EMBEDDED_NULL, 0, 0, TIXML_ENCODING_UNKNOWN );
					return;
				}
				
				if ( c == '>' )
					break;

				*tag += (char) c;
				in->get();

				// Early out if we find the CDATA id.
				if ( c == '[' && tag->size() >= 9 )
				{
					size_t len = tag->size();
					const char* start = tag->c_str() + len - 9;
					if ( strcmp( start, "<![CDATA[" ) == 0 ) {
						assert( !closingTag );
						break;
					}
				}

				if ( !firstCharFound && c != '<' && !IsWhiteSpace( c ) )
				{
					firstCharFound = true;
					if ( c == '/' )
						closingTag = true;
				}
			}
			// If it was a closing tag, then read in the closing '>' to clean up the input stream.
			// If it was not, the streaming will be done by the tag.
			if ( closingTag )
			{
				if ( !in->good() )
					return;

				int c = in->get();
				if ( c <= 0 )
				{
					TiXmlDocument* document = GetDocument();
					if ( document )
						document->SetError( TIXML_ERROR_EMBEDDED_NULL, 0, 0, TIXML_ENCODING_UNKNOWN );
					return;
				}
				assert( c == '>' );
				*tag += (char) c;

				// We are done, once we've found our closing tag.
				return;
			}
			else
			{
				// If not a closing tag, id it, and stream.
				const char* tagloc = tag->c_str() + tagIndex;
				TiXmlNode* node = Identify( tagloc, TIXML_DEFAULT_ENCODING );
				if ( !node )
					return;
				node->StreamIn( in, tag );
				delete node;
				node = 0;

				// No return: go around from the beginning: text, closing tag, or node.
			}
		}
	}
}
#endif

const char* TiXmlElement::Parse( const char* p, TiXmlParsingData* data, TiXmlEncoding encoding )
{
	p = SkipWhiteSpace( p, encoding );
	TiXmlDocument* document = GetDocument();

	if ( !p || !*p )
	{
		if ( document ) document->SetError( TIXML_ERROR_PARSING_ELEMENT, 0, 0, encoding );
		return 0;
	}

	if ( data )
	{
		data->Stamp( p, encoding );
		location = data->Cursor();
	}

	if ( *p != '<' )
	{
		if ( document ) document->SetError( TIXML_ERROR_PARSING_ELEMENT, p, data, encoding );
		return 0;
	}

	p = SkipWhiteSpace( p+1, encoding );

	// Read the name.
	const char* pErr = p;

    p = ReadName( p, &value, encoding );
	if ( !p || !*p )
	{
		if ( document )	document->SetError( TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME, pErr, data, encoding );
		return 0;
	}

    TIXML_STRING endTag ("</");
	endTag += value;

	// Check for and read attributes. Also look for an empty
	// tag or an end tag.
	while ( p && *p )
	{
		pErr = p;
		p = SkipWhiteSpace( p, encoding );
		if ( !p || !*p )
		{
			if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, pErr, data, encoding );
			return 0;
		}
		if ( *p == '/' )
		{
			++p;
			// Empty tag.
			if ( *p  != '>' )
			{
				if ( document ) document->SetError( TIXML_ERROR_PARSING_EMPTY, p, data, encoding );		
				return 0;
			}
			return (p+1);
		}
		else if ( *p == '>' )
		{
			// Done with attributes (if there were any.)
			// Read the value -- which can include other
			// elements -- read the end tag, and return.
			++p;
			p = ReadValue( p, data, encoding );		// Note this is an Element method, and will set the error if one happens.
			if ( !p || !*p ) {
				// We were looking for the end tag, but found nothing.
				// Fix for [ 1663758 ] Failure to report error on bad XML
				if ( document ) document->SetError( TIXML_ERROR_READING_END_TAG, p, data, encoding );
				return 0;
			}

			// We should find the end tag now
			// note that:
			// </foo > and
			// </foo> 
			// are both valid end tags.
			if ( StringEqual( p, endTag.c_str(), false, encoding ) )
			{
				p += endTag.length();
				p = SkipWhiteSpace( p, encoding );
				if ( p && *p && *p == '>' ) {
					++p;
					return p;
				}
				if ( document ) document->SetError( TIXML_ERROR_READING_END_TAG, p, data, encoding );
				return 0;
			}
			else
			{
				if ( document ) document->SetError( TIXML_ERROR_READING_END_TAG, p, data, encoding );
				return 0;
			}
		}
		else
		{
			// Try to read an attribute:
			TiXmlAttribute* attrib = new TiXmlAttribute();
			if ( !attrib )
			{
				return 0;
			}

			attrib->SetDocument( document );
			pErr = p;
			p = attrib->Parse( p, data, encoding );

			if ( !p || !*p )
			{
				if ( document ) document->SetError( TIXML_ERROR_PARSING_ELEMENT, pErr, data, encoding );
				delete attrib;
				return 0;
			}

			// Handle the strange case of double attributes:
			#ifdef TIXML_USE_STL
			TiXmlAttribute* node = attributeSet.Find( attrib->NameTStr() );
			#else
			TiXmlAttribute* node = attributeSet.Find( attrib->Name() );
			#endif
			if ( node )
			{
				if ( document ) document->SetError( TIXML_ERROR_PARSING_ELEMENT, pErr, data, encoding );
				delete attrib;
				return 0;
			}

			attributeSet.Add( attrib );
		}
	}
	return p;
}


const char* TiXmlElement::ReadValue( const char* p, TiXmlParsingData* data, TiXmlEncoding encoding )
{
	TiXmlDocument* document = GetDocument();

	// Read in text and elements in any order.
	const char* pWithWhiteSpace = p;
	p = SkipWhiteSpace( p, encoding );

	while ( p && *p )
	{
		if ( *p != '<' )
		{
			// Take what we have, make a text element.
			TiXmlText* textNode = new TiXmlText( "" );

			if ( !textNode )
			{
			    return 0;
			}

			if ( TiXmlBase::IsWhiteSpaceCondensed() )
			{
				p = textNode->Parse( p, data, encoding );
			}
			else
			{
				// Special case: we want to keep the white space
				// so that leading spaces aren't removed.
				p = textNode->Parse( pWithWhiteSpace, data, encoding );
			}

			if ( !textNode->Blank() )
				LinkEndChild( textNode );
			else
				delete textNode;
		} 
		else 
		{
			// We hit a '<'
			// Have we hit a new element or an end tag? This could also be
			// a TiXmlText in the "CDATA" style.
			if ( StringEqual( p, "</", false, encoding ) )
			{
				return p;
			}
			else
			{
				TiXmlNode* node = Identify( p, encoding );
				if ( node )
				{
					p = node->Parse( p, data, encoding );
					LinkEndChild( node );
				}				
				else
				{
					return 0;
				}
			}
		}
		pWithWhiteSpace = p;
		p = SkipWhiteSpace( p, encoding );
	}

	if ( !p )
	{
		if ( document ) document->SetError( TIXML_ERROR_READING_ELEMENT_VALUE, 0, 0, encoding );
	}	
	return p;
}


#ifdef TIXML_USE_STL
void TiXmlUnknown::StreamIn( std::istream * in, TIXML_STRING * tag )
{
	while ( in->good() )
	{
		int c = in->get();	
		if ( c <= 0 )
		{
			TiXmlDocument* document = GetDocument();
			if ( document )
				document->SetError( TIXML_ERROR_EMBEDDED_NULL, 0, 0, TIXML_ENCODING_UNKNOWN );
			return;
		}
		(*tag) += (char) c;

		if ( c == '>' )
		{
			// All is well.
			return;		
		}
	}
}
#endif


const char* TiXmlUnknown::Parse( const char* p, TiXmlParsingData* data, TiXmlEncoding encoding )
{
	TiXmlDocument* document = GetDocument();
	p = SkipWhiteSpace( p, encoding );

	if ( data )
	{
		data->Stamp( p, encoding );
		location = data->Cursor();
	}
	if ( !p || !*p || *p != '<' )
	{
		if ( document ) document->SetError( TIXML_ERROR_PARSING_UNKNOWN, p, data, encoding );
		return 0;
	}
	++p;
    value = "";

	while ( p && *p && *p != '>' )
	{
		value += *p;
		++p;
	}

	if ( !p )
	{
		if ( document )	document->SetError( TIXML_ERROR_PARSING_UNKNOWN, 0, 0, encoding );
	}
	if ( *p == '>' )
		return p+1;
	return p;
}

#ifdef TIXML_USE_STL
void TiXmlComment::StreamIn( std::istream * in, TIXML_STRING * tag )
{
	while ( in->good() )
	{
		int c = in->get();	
		if ( c <= 0 )
		{
			TiXmlDocument* document = GetDocument();
			if ( document )
				document->SetError( TIXML_ERROR_EMBEDDED_NULL, 0, 0, TIXML_ENCODING_UNKNOWN );
			return;
		}

		(*tag) += (char) c;

		if ( c == '>' 
			 && tag->at( tag->length() - 2 ) == '-'
			 && tag->at( tag->length() - 3 ) == '-' )
		{
			// All is well.
			return;		
		}
	}
}
#endif


const char* TiXmlComment::Parse( const char* p, TiXmlParsingData* data, TiXmlEncoding encoding )
{
	TiXmlDocument* document = GetDocument();
	value = "";

	p = SkipWhiteSpace( p, encoding );

	if ( data )
	{
		data->Stamp( p, encoding );
		location = data->Cursor();
	}
	const char* startTag = "<!--";
	const char* endTag   = "-->";

	if ( !StringEqual( p, startTag, false, encoding ) )
	{
		document->SetError( TIXML_ERROR_PARSING_COMMENT, p, data, encoding );
		return 0;
	}
	p += strlen( startTag );

	// [ 1475201 ] TinyXML parses entities in comments
	// Oops - ReadText doesn't work, because we don't want to parse the entities.
	// p = ReadText( p, &value, false, endTag, false, encoding );
	//
	// from the XML spec:
	/*
	 [Definition: Comments may appear anywhere in a document outside other markup; in addition, 
	              they may appear within the document type declaration at places allowed by the grammar. 
				  They are not part of the document's character data; an XML processor MAY, but need not, 
				  make it possible for an application to retrieve the text of comments. For compatibility, 
				  the string "--" (double-hyphen) MUST NOT occur within comments.] Parameter entity 
				  references MUST NOT be recognized within comments.

				  An example of a comment:

				  <!-- declarations for <head> & <body> -->
	*/

    value = "";
	// Keep all the white space.
	while (	p && *p && !StringEqual( p, endTag, false, encoding ) )
	{
		value.append( p, 1 );
		++p;
	}
	if ( p && *p ) 
		p += strlen( endTag );

	return p;
}


const char* TiXmlAttribute::Parse( const char* p, TiXmlParsingData* data, TiXmlEncoding encoding )
{
	p = SkipWhiteSpace( p, encoding );
	if ( !p || !*p ) return 0;

	if ( data )
	{
		data->Stamp( p, encoding );
		location = data->Cursor();
	}
	// Read the name, the '=' and the value.
	const char* pErr = p;
	p = ReadName( p, &name, encoding );
	if ( !p || !*p )
	{
		if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, pErr, data, encoding );
		return 0;
	}
	p = SkipWhiteSpace( p, encoding );
	if ( !p || !*p || *p != '=' )
	{
		if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, p, data, encoding );
		return 0;
	}

	++p;	// skip '='
	p = SkipWhiteSpace( p, encoding );
	if ( !p || !*p )
	{
		if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, p, data, encoding );
		return 0;
	}
	
	const char* end;
	const char SINGLE_QUOTE = '\'';
	const char DOUBLE_QUOTE = '\"';

	if ( *p == SINGLE_QUOTE )
	{
		++p;
		end = "\'";		// single quote in string
		p = ReadText( p, &value, false, end, false, encoding );
	}
	else if ( *p == DOUBLE_QUOTE )
	{
		++p;
		end = "\"";		// double quote in string
		p = ReadText( p, &value, false, end, false, encoding );
	}
	else
	{
		// All attribute values should be in single or double quotes.
		// But this is such a common error that the parser will try
		// its best, even without them.
		value = "";
		while (    p && *p											// existence
				&& !IsWhiteSpace( *p )								// whitespace
				&& *p != '/' && *p != '>' )							// tag end
		{
			if ( *p == SINGLE_QUOTE || *p == DOUBLE_QUOTE ) {
				// [ 1451649 ] Attribute values with trailing quotes not handled correctly
				// We did not have an opening quote but seem to have a 
				// closing one. Give up and throw an error.
				if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, p, data, encoding );
				return 0;
			}
			value += *p;
			++p;
		}
	}
	return p;
}

#ifdef TIXML_USE_STL
void TiXmlText::StreamIn( std::istream * in, TIXML_STRING * tag )
{
	while ( in->good() )
	{
		int c = in->peek();	
		if ( !cdata && (c == '<' ) ) 
		{
			return;
		}
		if ( c <= 0 )
		{
			TiXmlDocument* document = GetDocument();
			if ( document )
				document->SetError( TIXML_ERROR_EMBEDDED_NULL, 0, 0, TIXML_ENCODING_UNKNOWN );
			return;
		}

		(*tag) += (char) c;
		in->get();	// "commits" the peek made above

		if ( cdata && c == '>' && tag->size() >= 3 ) {
			size_t len = tag->size();
			if ( (*tag)[len-2] == ']' && (*tag)[len-3] == ']' ) {
				// terminator of cdata.
				return;
			}
		}    
	}
}
#endif

const char* TiXmlText::Parse( const char* p, TiXmlParsingData* data, TiXmlEncoding encoding )
{
	value = "";
	TiXmlDocument* document = GetDocument();

	if ( data )
	{
		data->Stamp( p, encoding );
		location = data->Cursor();
	}

	const char* const startTag = "<![CDATA[";
	const char* const endTag   = "]]>";

	if ( cdata || StringEqual( p, startTag, false, encoding ) )
	{
		cdata = true;

		if ( !StringEqual( p, startTag, false, encoding ) )
		{
			document->SetError( TIXML_ERROR_PARSING_CDATA, p, data, encoding );
			return 0;
		}
		p += strlen( startTag );

		// Keep all the white space, ignore the encoding, etc.
		while (	   p && *p
				&& !StringEqual( p, endTag, false, encoding )
			  )
		{
			value += *p;
			++p;
		}

		TIXML_STRING dummy; 
		p = ReadText( p, &dummy, false, endTag, false, encoding );
		return p;
	}
	else
	{
		bool ignoreWhite = true;

		const char* end = "<";
		p = ReadText( p, &value, ignoreWhite, end, false, encoding );
		if ( p )
			return p-1;	// don't truncate the '<'
		return 0;
	}
}

#ifdef TIXML_USE_STL
void TiXmlDeclaration::StreamIn( std::istream * in, TIXML_STRING * tag )
{
	while ( in->good() )
	{
		int c = in->get();
		if ( c <= 0 )
		{
			TiXmlDocument* document = GetDocument();
			if ( document )
				document->SetError( TIXML_ERROR_EMBEDDED_NULL, 0, 0, TIXML_ENCODING_UNKNOWN );
			return;
		}
		(*tag) += (char) c;

		if ( c == '>' )
		{
			// All is well.
			return;
		}
	}
}
#endif

const char* TiXmlDeclaration::Parse( const char* p, TiXmlParsingData* data, TiXmlEncoding _encoding )
{
	p = SkipWhiteSpace( p, _encoding );
	// Find the beginning, find the end, and look for
	// the stuff in-between.
	TiXmlDocument* document = GetDocument();
	if ( !p || !*p || !StringEqual( p, "<?xml", true, _encoding ) )
	{
		if ( document ) document->SetError( TIXML_ERROR_PARSING_DECLARATION, 0, 0, _encoding );
		return 0;
	}
	if ( data )
	{
		data->Stamp( p, _encoding );
		location = data->Cursor();
	}
	p += 5;

	version = "";
	encoding = "";
	standalone = "";

	while ( p && *p )
	{
		if ( *p == '>' )
		{
			++p;
			return p;
		}

		p = SkipWhiteSpace( p, _encoding );
		if ( StringEqual( p, "version", true, _encoding ) )
		{
			TiXmlAttribute attrib;
			p = attrib.Parse( p, data, _encoding );		
			version = attrib.Value();
		}
		else if ( StringEqual( p, "encoding", true, _encoding ) )
		{
			TiXmlAttribute attrib;
			p = attrib.Parse( p, data, _encoding );		
			encoding = attrib.Value();
		}
		else if ( StringEqual( p, "standalone", true, _encoding ) )
		{
			TiXmlAttribute attrib;
			p = attrib.Parse( p, data, _encoding );		
			standalone = attrib.Value();
		}
		else
		{
			// Read over whatever it is.
			while( p && *p && *p != '>' && !IsWhiteSpace( *p ) )
				++p;
		}
	}
	return 0;
}

bool TiXmlText::Blank() const
{
	for ( unsigned i=0; i<value.length(); i++ )
		if ( !IsWhiteSpace( value[i] ) )
			return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
#ifndef TIXML_USE_STL
// Error value for find primitive
const TiXmlString::size_type TiXmlString::npos = static_cast< TiXmlString::size_type >(-1);


// Null rep.
TiXmlString::Rep TiXmlString::nullrep_ = { 0, 0, { '\0' } };


void TiXmlString::reserve (size_type cap)
{
	if (cap > capacity())
	{
		TiXmlString tmp;
		tmp.init(length(), cap);
		memcpy(tmp.start(), data(), length());
		swap(tmp);
	}
}


TiXmlString& TiXmlString::assign(const char* str, size_type len)
{
	size_type cap = capacity();
	if (len > cap || cap > 3*(len + 8))
	{
		TiXmlString tmp;
		tmp.init(len);
		memcpy(tmp.start(), str, len);
		swap(tmp);
	}
	else
	{
		memmove(start(), str, len);
		set_size(len);
	}
	return *this;
}


TiXmlString& TiXmlString::append(const char* str, size_type len)
{
	size_type newsize = length() + len;
	if (newsize > capacity())
	{
		reserve (newsize + capacity());
	}
	memmove(finish(), str, len);
	set_size(newsize);
	return *this;
}


TiXmlString operator + (const TiXmlString & a, const TiXmlString & b)
{
	TiXmlString tmp;
	tmp.reserve(a.length() + b.length());
	tmp += a;
	tmp += b;
	return tmp;
}

TiXmlString operator + (const TiXmlString & a, const char* b)
{
	TiXmlString tmp;
	TiXmlString::size_type b_len = static_cast<TiXmlString::size_type>( strlen(b) );
	tmp.reserve(a.length() + b_len);
	tmp += a;
	tmp.append(b, b_len);
	return tmp;
}

TiXmlString operator + (const char* a, const TiXmlString & b)
{
	TiXmlString tmp;
	TiXmlString::size_type a_len = static_cast<TiXmlString::size_type>( strlen(a) );
	tmp.reserve(a_len + b.length());
	tmp.append(a, a_len);
	tmp += b;
	return tmp;
}
#endif	// TIXML_USE_STL

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPubFun::CPubFun()
{
	printf("CPubFun构造结束\n");
	
}

CPubFun::~CPubFun()
{
	
}



//////////////////////////////////////////////////////////////////////////
//
// Thread.cpp: implementation file
//
// Copyright (C) Walter E. Capers.  All rights reserved
//
// This source is free to use as you like.  If you make
// any changes please keep me in the loop.  Email your changes
// to walt.capers@comcast.net.
//
// PURPOSE:
//
//  To implement threading as a C++ object
//
// NOTES:
//  This object supports two types of thread models, event driven and
//  interval driven.  Under the event driven model, a thread waits
//  in a paused state until the member function Event is called.  When
//  the Event function is called the thread wakes up and calls OnTask.
//  Under the interval driven model, the thread wakes up every
//  m_dwIdle milli-seconds and calls OnTask.
//
//  You can switch between the two models from within the same object.
//
// COMPILER NOTES:
// On Unix you must use -lpthread a -lrt
// On Windows you must specify threaded under C++ code generation
//
// REVISIONS
// =======================================================
// Date: 10.24.07        
// Name: Walter E. Capers
// Description: File creation
//
// Date: 10.24.07 11:49 am
// Name: Walter E. Capers
// Description: Added SetIdle function to allow the idle time to be altered
//              independent of the SetThreadType member function.
//              Added sleep interval to Stop function.
//
// Date: 10.25.07
// Name: Walter E. Capers
// Description: Added support for other non-windows platforms.
//
//              Added static functions: ThreadIdsEqual and ThreadId.
//
//              Added que for handling multiple events.
//
//              Created the CEventClass and CMutexClass classes to facilitate
//              platform independence.
//
// Date: 10.26.07
// Name: Walter E. Capers
// Description: Made object production ready...
//              Added more comments
//
//              Addressed various issues with threads on UNIX systems.
//                -- there was a defect in the Sleep function
//                -- there was a defect in the main thread function THKERNEL
//                   , when transitioning between thread models the CEvent::Reset
//                   function was not being called when it was necessary resulting
//                   in a lock up.
//              
//				 Transition between thread types also failed on WINDOWS since the Event
//               member function was being called from within SetThreadType.  This
//               resulted in an Event usage error.  To correct the problem m_event.Set
//               is called instead.  Also, eliminated unecessary logic.
//
//               Got rid of OnStart, OnStop, OnDestroy... Could not override with a derived
//				 class, not sure why I will come back to in a later release.
//
//				 Changed default behavior of thread.  If OnTask is not redefined in the derived
//               class the default version now expects a CTask object.  The Class for CTask 
//               is defined in thread.h.  A class must be derived from CTask to use it in
//               the default version of OnTask(LPVOID).
//
// Date: 11.01.07
// Name: Walter E. Capers
// Description: I introduced more logic and ASSERTIONS to insure the integrity of CThread objects.  
//              Both the Homogeneous and Specialized thread types can be physically set using the 
//              SetThreadType member function.  If the thread type is not set, the thread will
//              determine its type based on calls to member functions; however, this does not
//              apply to interval-based threads.  Interval-based threads must be implicitly
//              identified using the SetThreadType member function.  The new integrity tests
//              are implemented to insure usage consistency with a CThread object.   
//
//              New member functions AtCapacity and PercentCapacity were added to determine
//              if a thread is truly busy.  AtCapacity will return TRUE under one of two 
//              conditions: the thread is processing an event and its stack is full, the thread
//              is not running.  These new functions allow thread objects to be placed in arrays
//              and tasked based on their workloads.
//
//              The Event member function has been modified to verify that a thread is running
//              before posting an event.  This resolved a problem on SunOS were threads did not
//              start right away; there was a small delay of a few milliseconds.    
//
//              Error flags are automatically reset when certain member functions are called this
//              isolates error occurrences to specific call sequences.
//
//
// Date: 11.01.07
// Name: Walter E. Capers
// Description: In THKernel, changed how events are released.  Events are now released right after
//              They are recieved.

// #include "pubthread.h"
// #ifdef USE_BEGIN_THREAD
// #include <process.h>
// #endif


#ifndef _WINDOWS32
extern "C"
{
 int	usleep(useconds_t useconds);
#ifdef NANO_SECOND_SLEEP
 int 	nanosleep(const struct timespec *rqtp, struct timespec *rmtp);
#endif
}

void Sleep( unsigned int milli )
{
#ifdef NANO_SECOND_SLEEP
	struct timespec interval, remainder;
	milli = milli * 1000000;
	interval.tv_sec= 0;
	interval.tv_nsec=milli;
	nanosleep(&interval,&remainder);
#else
	usleep(milli*1000);
#endif	
}
#endif

// #include <iostream>
// using namespace std;

/**
 * 
 * _THKERNEL
 * thread callback function used by CreateThread
 *
 *
 **/
#ifdef _WINDOWS32
#ifdef USE_BEGIN_THREAD
unsigned __stdcall
#else
DWORD WINAPI
#endif
#else
LPVOID
#endif
_THKERNEL( LPVOID lpvData /* CThread Object */ 
		  )
{
	CThread *pThread = (CThread *)lpvData;
	ThreadType_t lastType;
	/*
	 *
	 * initialization
	 *
	 */


    pThread->m_mutex.Lock();
		pThread->m_state = ThreadStateWaiting;
		pThread->m_bRunning = TRUE;
#ifndef _WINDOWS32
		pThread->m_dwId = CThread::ThreadId();
#endif
	pThread->m_mutex.Unlock();
	
	while( TRUE )
	{
		lastType = pThread->m_type;

		if( lastType == ThreadTypeHomogeneous ||
			lastType == ThreadTypeSpecialized ||
			lastType == ThreadTypeNotDefined )
		{
			if( ! pThread->m_event.Wait()  )  // wait for a message
					break;
			pThread->m_event.Reset(); // message recieved
		}
		cout<<"pThread->KernelProcess()"<<endl;	
		if( ! pThread->KernelProcess() ) 
				break;

		cout<<"after KernelProcess()"<<endl;	

		/*if( lastType == ThreadTypeHomogeneous ||
			lastType == ThreadTypeSpecialized ||
			lastType == ThreadTypeNotDefined )
		{
			pThread->m_event.Reset();
		} */

		if( pThread->m_type == ThreadTypeIntervalDriven )
			Sleep(pThread->m_dwIdle);

	}


	pThread->m_mutex.Lock();
		pThread->m_state = ThreadStateDown;
		pThread->m_bRunning = FALSE;
	pThread->m_mutex.Unlock();


#ifdef _WINDOWS32
	return 0;
#else
	return (LPVOID)0;
#endif
}

/**
 *
 * FromSameThread
 * determines if the calling thread is the same
 * as the thread assoicated with the object
 *
 **/
BOOL
CThread::FromSameThread()
{
	ThreadId_t id = ThreadId();
	if( ThreadIdsEqual(&id,&m_dwId) ) return TRUE;
	return FALSE;
}

/**
 *
 * OnTask
 * called when a thread is tasked using the Event
 * member function
 *
 **/
BOOL 
CThread::OnTask( LPVOID lpvData /*data passed from thread*/ 
					   )
{
    ASSERT(lpvData && m_type == ThreadTypeHomogeneous);

	cout<<"ASSERT(lpvData && m_type == ThreadTypeHomogeneous);"<<endl;

	if( m_type != ThreadTypeHomogeneous )
	{
		cerr << "Warning CThread::OnTask:\n\tOnTask(LPVOID) called for a non-homogeneous thread!\n";
		return FALSE;
	}
	cout<<"m_type == ThreadTypeHomogeneous"<<endl;

	((CTask *)lpvData)->SetTaskStatus(TaskStatusBeingProcessed);
	BOOL bReturn = ((CTask *)lpvData)->Task();
	((CTask *)lpvData)->SetTaskStatus(TaskStatusCompleted);
	cout<<"bReturn="<<bReturn<<endl;

	return bReturn; 
} 



/**
 *
 * OnTask
 * overloaded implementation of OnTask that
 * takes no arguments
 *
 **/
BOOL
CThread::OnTask()
{
	cout<<"CThread::OnTask()"<<endl;
	ASSERT(m_type == ThreadTypeIntervalDriven);
	cout<<"ASSERT(m_type == ThreadTypeIntervalDriven)-CThread::OnTask()"<<endl;
	if( m_type != ThreadTypeIntervalDriven )
	{
		cerr << "Warning CThread::OnTask:\n\tOnTask() called for a non-event driven thread!\n";
		return FALSE;
	}

	printf("\nthread is alive\n");

	return TRUE;
}

/**
 *
 * CEvent
 * used to place tasks on the threads event queue
 * wakes up thread.
 *
 **/
BOOL
CThread::Event(CTask *pvTask /* data to be processed by thread */
			   )
{
	m_mutex.Lock();

	ASSERT(m_type == ThreadTypeHomogeneous ||
	       m_type == ThreadTypeNotDefined );

	try 
	{
		if( FromSameThread() )
		{
			throw "\n\tit is illegal for a thread to place an event on its own event stack!\n";
		}


		// make sure that the thread is running 
		if( !m_bRunning && m_dwObjectCondition == NO_ERRORS )
		{
			m_mutex.Unlock();
			PingThread(m_dwIdle*2); // wait two idle cycles for it to start
			m_mutex.Lock();
		}
		if( !m_bRunning ) // if it is not running return FALSE;
		{
			m_mutex.Unlock();
			return FALSE;
		}


		if( m_dwObjectCondition & ILLEGAL_USE_OF_EVENT )
			m_dwObjectCondition = m_dwObjectCondition ^ ILLEGAL_USE_OF_EVENT;
		if( m_dwObjectCondition & EVENT_AND_TYPE_DONT_MATCH)
			m_dwObjectCondition = m_dwObjectCondition ^ EVENT_AND_TYPE_DONT_MATCH;

		if( m_type != ThreadTypeHomogeneous &&
			m_type != ThreadTypeNotDefined    )
		{
			m_mutex.Unlock();
			m_dwObjectCondition |= ILLEGAL_USE_OF_EVENT;
			m_dwObjectCondition |= EVENT_AND_TYPE_DONT_MATCH;
			m_state = ThreadStateFault;
			cerr << "Warning: invalid call to CEvent::Event(CTask *), thread type is not specialized\n";

			return FALSE;
		}

		m_type = ThreadTypeHomogeneous;
		m_mutex.Unlock();

		pvTask->SetId(&m_dwId);
		if( ! Push((LPVOID)pvTask) )
			return FALSE;

		pvTask->SetTaskStatus(TaskStatusWaitingOnQueue);
		m_event.Set();

	}
	catch (char *psz)
	{
#ifdef _WINDOWS32
		//MessageBoxA(NULL,&psz[2],"Fatal exception CThread::CEvent",MB_ICONHAND);
		cerr << "Fatal exception CThread::CEvent\n";
		exit(-1);
#else
		cerr << "Fatal exception CThread::CEvent(CTask *pvTask):" << psz;
#endif

	}
	return TRUE;
}

/**
 *
 * Event
 * used to place tasks on the threads event queue
 * wakes up thread.
 *
 **/
BOOL
CThread::Event(LPVOID lpvData /* data to be processed by thread */
			   )
{
	cout<<"Event"<<endl;
	m_mutex.Lock();
	ASSERT( m_type == ThreadTypeSpecialized ||
		    m_type == ThreadTypeNotDefined );
	try 
	{
		if( FromSameThread() )
		{
			throw "\n\tit is illegal for a thread to place an event on its own event stack!\n";
		}
	}
	catch (char *psz)
	{
#ifdef _WINDOWS32
		//MessageBoxA(NULL,&psz[2],"Fatal exception CThread::CEvent",MB_ICONHAND);
		cerr << "Fatal exception CThread::CEvent\n";
		exit(-1);
#else
		cerr << "Fatal exception CThread::CEvent(LPVOID lpvData):" << psz;
#endif

	}

	// make sure that the thread is running 
	if( !m_bRunning && m_dwObjectCondition == NO_ERRORS )
	{
		m_mutex.Unlock();
		  PingThread(m_dwIdle*2); // wait two idle cycles for it to start
		m_mutex.Lock();
	}
	if( !m_bRunning ) // if it is not running return FALSE;
	{
		m_mutex.Unlock();
		return FALSE;
	}

	if( m_dwObjectCondition & ILLEGAL_USE_OF_EVENT )
		m_dwObjectCondition = m_dwObjectCondition ^ ILLEGAL_USE_OF_EVENT;
	if( m_dwObjectCondition & EVENT_AND_TYPE_DONT_MATCH)
		m_dwObjectCondition = m_dwObjectCondition ^ EVENT_AND_TYPE_DONT_MATCH;

	if( m_type != ThreadTypeSpecialized && 
		m_type != ThreadTypeNotDefined )
	{
		m_dwObjectCondition |= ILLEGAL_USE_OF_EVENT;
		m_dwObjectCondition |= EVENT_AND_TYPE_DONT_MATCH;
		cerr << "Warning: invalid call to CEvent::Event(LPVOID), thread type is not specialized\n";
		m_mutex.Unlock();
		return FALSE;
	}
	m_type = ThreadTypeSpecialized;

	m_mutex.Unlock();
	if( ! Push(lpvData) )
	{
		return FALSE;
	}

	m_event.Set();
	cout<<"Event-end"<<endl;

	return TRUE;
}


/**
 *
 * SetPriority
 * sets a threads run priority, see SetThreadPriority
 * Note: only works for Windows family of operating systems
 *
 *
 **/
void
CThread::SetPriority(DWORD dwPriority)
{

#ifdef _WINDOWS32
	SetThreadPriority(m_thread,dwPriority);
#endif
}

	  
/**
 *
 * KernelProcess
 * routes thread activity
 *
 **/
BOOL
CThread::KernelProcess()
{

	m_mutex.Lock();
	m_state = ThreadStateBusy;
	if( !m_bRunning )
	{
		m_state = ThreadStateShuttingDown;
		m_mutex.Unlock();
		return FALSE;
	}
	m_mutex.Unlock();

	if( !Empty() )
	{
		while( !Empty() )
		{
			Pop();
			cout<<"OnTask(m_lpvProcessor)-after Pop();"<<endl;
			if( !OnTask(m_lpvProcessor) )  //lns 虚函数，多态，子类重写方法
			{
				m_mutex.Lock();
				m_lpvProcessor = NULL;
				m_state = ThreadStateShuttingDown;
				m_mutex.Unlock();
				return FALSE;
			}
		}
		m_mutex.Lock();
		m_lpvProcessor = NULL;
		m_state = ThreadStateWaiting;
	}
	else {
		cout<<"OnTask()-without  Pop();"<<endl;
		if( !OnTask() )  //lns 虚函数，多态，子类重写方法
		{
			m_mutex.Lock();
			m_state = ThreadStateShuttingDown;
			m_mutex.Unlock();
			return FALSE;
		}
		m_mutex.Lock();
		m_state = ThreadStateWaiting;
	}

	m_mutex.Unlock();

	return TRUE;
}


/**
 * 
 * GetEventsPending
 * returns the total number of vents waiting
 * in the event que
 * 
 **/
unsigned int
CThread::GetEventsPending()
{
	unsigned int chEventsWaiting;

	m_mutex.Lock();
	  chEventsWaiting = m_queuePos;
    m_mutex.Unlock();

	return chEventsWaiting;
}


/**
 *
 * CThread
 * instanciates thread object and
 * starts thread.
 *
 **/
CThread::CThread(void)
:m_bRunning(FALSE)
#ifdef _WINDOWS32
,m_thread(NULL)
#endif
,m_dwId(0L)
,m_state(ThreadStateDown)
,m_dwIdle(100)
,m_lppvQueue(NULL)
,m_lpvProcessor(NULL)
,m_chQueue(QUEUE_SIZE)
,m_type(ThreadTypeNotDefined)
,m_stackSize(DEFAULT_STACK_SIZE)
,m_queuePos(0)
,m_StopTimeout(30)
{
	// printf("CThread构造开始\n");
	m_dwObjectCondition = NO_ERRORS;

	m_lppvQueue = new LPVOID [QUEUE_SIZE];

	if( !m_lppvQueue ) 
	{
		m_dwObjectCondition |= MEMORY_FAULT;
		m_state = ThreadStateFault;
		return;
	}

	if( !m_mutex.m_bCreated )
	{
		perror("mutex creation failed");
		m_dwObjectCondition |= MUTEX_CREATION;
		m_state = ThreadStateFault;
		return;
	}


	if( !m_event.m_bCreated )
	{
		perror("event creation failed");
		m_dwObjectCondition |= EVENT_CREATION;
		m_state = ThreadStateFault;
		return;
	}

    // 初始声明时不启动线程
	// Start();
}


/**
 *
 * PercentCapacity
 * returns a floating point value identifying
 * the current workload of the thread
 *
 **/
float
CThread::PercentCapacity()
{
	float fValue = 0;
	m_mutex.Lock();
		fValue = (float)m_queuePos/m_chQueue;
	m_mutex.Unlock();
	return fValue;
}

/**
 *
 * SetQueueSize
 * changes the threads queue size
 *
 **/
BOOL
CThread::SetQueueSize( unsigned int ch )
{
	LPVOID * newQueue = NULL;

	m_mutex.Lock();
	    ASSERT(ch > m_queuePos);

	    if( ch <= m_queuePos )
		{
			cerr << "Warning CThread::SetQueueSize:\n\tthe new queue size is less than the number of tasks on a non-empty queue! Request ignored.\n";
			m_mutex.Unlock();
			return FALSE;
		}

		newQueue = new LPVOID [ch];
		if(  !newQueue )
		{
			cerr << "Warning CThread::SetQueueSize:\n\ta low memory, could not reallocate queue!\n";
			m_mutex.Unlock();
			return FALSE;
		}

		for( unsigned int i=0;i<m_queuePos; i++ )
		{
			newQueue[i] = m_lppvQueue[i];
		}

		delete [] m_lppvQueue;

		m_chQueue = ch;
		m_lppvQueue = newQueue;

	m_mutex.Unlock();

	return TRUE;
}



/**
 *
 * Empty
 * returns a value of TRUE if there are no items on the threads que
 * otherwise a value of FALSE is returned.
 *
 **/
BOOL
CThread::Empty()
{
	m_mutex.Lock();
	if( m_queuePos <= 0 )
	{
		m_mutex.Unlock();
		return TRUE;
	}
	m_mutex.Unlock();
	return FALSE;
}



/**
 *
 * Push
 * place a data object in the threads que
 *
 **/
BOOL
CThread::Push( LPVOID lpv )
{
	if( !lpv ) return TRUE;

	m_mutex.Lock();

	if( m_queuePos+1 >= m_chQueue ) {
		m_dwObjectCondition |= STACK_OVERFLOW;
		m_mutex.Unlock();
		return FALSE;
	}
	if( m_dwObjectCondition & STACK_EMPTY    )
		m_dwObjectCondition = m_dwObjectCondition ^ STACK_EMPTY;

	if( m_dwObjectCondition & STACK_OVERFLOW ) 
		m_dwObjectCondition = m_dwObjectCondition ^ STACK_OVERFLOW;

	m_lppvQueue[m_queuePos++] = lpv;
	if( m_queuePos+1 >= m_chQueue )
		m_dwObjectCondition |= STACK_FULL;

	m_mutex.Unlock();
	return TRUE;
}


/**
 *
 * Pop
 * move an object from the input que to the processor
 *
 **/
BOOL
CThread::Pop()
{

	m_mutex.Lock();
	if( m_queuePos-1 < 0 )
	{
		m_queuePos = 0;
		m_dwObjectCondition |= STACK_EMPTY;
		m_mutex.Unlock();
		return FALSE;
	}
	if( m_dwObjectCondition & STACK_EMPTY )
		m_dwObjectCondition = m_dwObjectCondition ^ STACK_EMPTY;
	if( m_dwObjectCondition & STACK_OVERFLOW )
		m_dwObjectCondition = m_dwObjectCondition ^ STACK_OVERFLOW;
	if( m_dwObjectCondition & STACK_FULL )
		m_dwObjectCondition = m_dwObjectCondition ^ STACK_FULL;

	m_queuePos--;
	m_lpvProcessor = m_lppvQueue[m_queuePos];
	m_mutex.Unlock();
	return TRUE;
}


/**
 *
 * SetThreadType
 * specifies the type of threading that is to be performed.
 *
 * ThreadTypeEventDriven (default): an event must be physically sent
 *									to the thread using the Event member
 *									function.
 *
 * ThreadTypeIntervalDriven       : an event occurs automatically every 
 *                                  dwIdle milli seconds.
 *
 **/
void
CThread::SetThreadType(ThreadType_t typ,
              DWORD dwIdle)
{

	try 
	{
		if( FromSameThread() )
		{
			throw "\n\tit is illegal for a thread to change its own type!\n";
		}


		m_mutex.Lock();
		m_dwIdle = dwIdle;


		if( m_type == typ ) {
			m_mutex.Unlock();
			return;
		}
		if( m_dwObjectCondition & ILLEGAL_USE_OF_EVENT )
			m_dwObjectCondition = m_dwObjectCondition ^ ILLEGAL_USE_OF_EVENT;
		if( m_dwObjectCondition & EVENT_AND_TYPE_DONT_MATCH )
			m_dwObjectCondition = m_dwObjectCondition ^ EVENT_AND_TYPE_DONT_MATCH;

		m_type = typ;


		m_mutex.Unlock();
		m_event.Set();
	}
	catch (char *psz)
	{
#ifdef _WINDOWS32
		//MessageBoxA(NULL,&psz[2],"Fatal exception CThread::SetThreadType",MB_ICONHAND);
		cerr << "Fatal exception CThread::SetThreadType\n";
		exit(-1);
#else
		cerr << "Fatal exception CThread::SetThreadType(ThreadType_t typ):" << psz;
#endif

	}
}


/**
 *
 * Stop
 * stop thread 
 *
 **/
BOOL
CThread::Stop()
{
	try 
	{
		if( FromSameThread() )
		{
			throw "\n\tit is illegal for a thread to attempt to signal itself to stop!\n";
		}

		m_mutex.Lock();
		m_bRunning = FALSE;
		m_mutex.Unlock();
		m_event.Set();

		int ticks = (m_StopTimeout*1000)/100;

		for( int i=0; i<ticks; i++ )
		{
			Sleep(100);

			m_mutex.Lock();
			if( m_state == ThreadStateDown )
			{
				m_mutex.Unlock();
				return TRUE;
			}
			m_mutex.Unlock();

		} 
	}
	catch (char *psz)
	{
#ifdef _WINDOWS32
		//MessageBoxA(NULL,&psz[2],"Fatal exception CThread::Stop",MB_ICONHAND);
		cerr << "Fatal exception CThread::Stop\n";
		exit(-1);
#else
		cerr << "Fatal exception CThread::Stop():" << psz;
#endif

	}
	return FALSE;
}


/**
 *
 * SetIdle
 * changes the threads idle interval
 *
 **/
void
CThread::SetIdle(DWORD dwIdle)
{
	m_mutex.Lock();
		m_dwIdle = dwIdle;
	m_mutex.Unlock();
}

/**
 *
 * Start
 * start thread
 *
 **/
BOOL
CThread::Start()
{
	cout<<"启动线程"<<endl;	
	try 
	{
		if( FromSameThread() )
		{
			cout<<"FromSameThread"<<endl;	
			throw "\n\tit is illegal for a thread to attempt to start itself!\n";
		}


		m_mutex.Lock();
		if( m_bRunning ) 
		{
			m_mutex.Unlock();
			cout<<"m_bRunning is true"<<endl;	
			return TRUE;
		}

		m_mutex.Unlock();


		if( m_dwObjectCondition & THREAD_CREATION )  //lns_unknow
			m_dwObjectCondition = m_dwObjectCondition ^ THREAD_CREATION;

#ifdef _WINDOWS32
		if( m_thread ) CloseHandle(m_thread);
#ifdef USE_BEGIN_THREAD
		cout<<"Start-beginthreadex(NULL,(unsigned int)m_stackSize,_THKERNEL,(LPVOID)this,0,&m_dwId);"<<endl;	
		m_thread = (HANDLE )_beginthreadex(NULL,(unsigned int)m_stackSize,_THKERNEL,(LPVOID)this,0,&m_dwId);
#else
		cout<<"Start-CreateThread(NULL,m_stackSize ,_THKERNEL,(LPVOID)this,0,&m_dwId);"<<endl;	
		m_thread = CreateThread(NULL,m_stackSize ,_THKERNEL,(LPVOID)this,0,&m_dwId);
#endif // USE_BEGIN_THREAD
		if( !m_thread )
		{
			perror("thread creation failed");
			m_dwObjectCondition |= THREAD_CREATION;
			m_state = ThreadStateFault;
			return FALSE;
		}
#else // _WINDOWS32
		pthread_attr_t attr;

		pthread_attr_init(&attr);

#ifdef VMS
		if( m_stackSize == 0 )
			pthread_attr_setstacksize(&attr,PTHREAD_STACK_MIN*10);
#endif // VMS
		if( m_stackSize != 0 )
			pthread_attr_setstacksize(&attr,m_stackSize);

		cout<<"Start-pthread_create(&m_thread,&attr,_THKERNEL,(LPVOID)this);"<<endl;	
		int error = pthread_create(&m_thread,&attr,_THKERNEL,(LPVOID)this);

		if( error != 0 )
		{
			m_dwObjectCondition |= THREAD_CREATION;
			m_state = ThreadStateFault;

#if defined(HPUX) || defined(SUNOS) || defined(LINUX)
			switch(error)/* show the thread error */
			{

			case EINVAL:
				cerr << "error: attr in an invalid thread attributes object\n";
				break;
			case EAGAIN:
				cerr << "error: the necessary resources to create a thread are not\n";
				cerr << "available.\n";
				break;
			case EPERM:
				cerr << "error: the caller does not have the privileges to create\n";
				cerr << "the thread with the specified attr object.\n";
				break;
#if defined(HPUX)
			case ENOSYS:

				cerr << "error: pthread_create not implemented!\n";
				if( __is_threadlib_linked()==0 )
				{
					cerr << "error: threaded library not being used, improper linkage \"-lpthread -lc\"!\n";
				}
				break;
#endif
			default:
				cerr << "error: an unknown error was encountered attempting to create\n";
				cerr << "the requested thread.\n";
				break;
			}
#else
			cerr << "error: could not create thread, pthread_create failed (" << error << ")!\n";
#endif
			return FALSE;	
		}
#endif
	}
	catch (char *psz)
	{
#ifdef _WINDOWS32
		//MessageBoxA(NULL,&psz[2],"Fatal exception CThread::Start",MB_ICONHAND);
		cerr << "Fatal exception CThread::Start\n";
#else
		cerr << "Fatal exception CThread::Start():" << psz;
#endif
		exit(-1);
	}
	return TRUE;
}

/**
 *
 * AtCapacity
 * returns TRUE if the threads queue is full, and the thread
 * is busy processing an event or the thread is not running
 *
 **/
BOOL
CThread::AtCapacity()
{
	m_mutex.Lock();
		if( ((m_dwObjectCondition & STACK_OVERFLOW ||
			  m_dwObjectCondition & STACK_FULL ) &&
			  m_state == ThreadStateBusy) || !m_bRunning)
		{
			m_mutex.Unlock();
			return TRUE;
		}
	m_mutex.Unlock();
	return FALSE;
}

/**
 *
 * ThreadState
 * return the current state of the thread
 *
 **/
ThreadState_t 
CThread::ThreadState()
{
	ThreadState_t currentState;
	m_mutex.Lock();
		currentState = m_state;
	m_mutex.Unlock();
	return currentState;
}

ThreadId_t
CThread::GetId()
{
    ThreadId_t currentThreadId;
    m_mutex.Lock();
    currentThreadId = m_dwId;
    m_mutex.Unlock();
    return currentThreadId;
}

/**
 *
 * ~CThread
 * destructor.  Stop should be called prior to destruction to
 * allow for gracefull thread termination.
 *
 **/
CThread::~CThread(void)
{
	if( m_bRunning ) // gracefull termination
	{
		try 
		{
			if( !Stop() )
			{
				throw "\n\tthread failed to stop in a timely manner!\n";
			}
		}
		catch( char *psz )
		{
#ifdef _WINDOWS32
		    //MessageBoxA(NULL,&psz[2],"Fatal exception CThread::Stop",MB_ICONHAND);
			cerr << "Fatal exception CThread::Stop\n";
		    exit(-1);
#else
			cerr << "Fatal exception CThread::Stop: " << psz;
#endif
		}
	}
#ifdef _WINDOWS32
	CloseHandle(m_thread);
#endif

	delete [] m_lppvQueue;
}


/**
 *
 * PingThread
 * used to determine if a thread is running
 *
 **/
BOOL
CThread::PingThread(DWORD dwTimeout /* timeout in milli-seconds */
				 )
{
    DWORD dwTotal = 0;

	while(TRUE)
	{
		if( dwTotal > dwTimeout && dwTimeout > 0 )
			return FALSE;
		m_mutex.Lock();
			if( m_bRunning )
			{
				m_mutex.Unlock();
				return TRUE;
			}
		dwTotal += m_dwIdle;
		m_mutex.Unlock();
		Sleep(m_dwIdle);
	}

	return FALSE;
}

/**
 *
 * WaitTillExit
 * blocks caller until thread exits
 *
 **/
void
CThread::WaitTillExit()
{

	/*
	 *
	 * prevent users from calling this function from within the same thread
	 * of execution
	 *
	 */
	try 
	{
		if( FromSameThread() )
			throw "\n\tthis function can not be called from within the same thread!\n";




		if( !m_bRunning ) return;


#ifdef _WINDOWS32
		WaitForSingleObject(m_thread,INFINITE);
#else
		LPVOID lpv;

		pthread_join(m_thread,&lpv);
#endif
	}
	catch( char *psz )
	{
#ifdef _WINDOWS32
		//MessageBoxA(NULL,&psz[2],"Fatal exception CThread::WaitTillExit",MB_ICONHAND);
		cerr << "Fatal exception CThread::WaitTillExit\n";
		exit(-1);
#else
		cerr << "Fatal exception CThread::WaitTillExit: " << psz;
#endif

	}
}

BOOL CThread::KillThread()
{
	try 
	{
		if( FromSameThread() )
		{
			throw "\n\tit is illegal for a thread to attempt to signal itself to stop!\n";
		}
		
		m_mutex.Lock();
		m_bRunning = FALSE;
		m_mutex.Unlock();
		m_event.Set();
		
		m_mutex.Lock();
		m_state = ThreadStateDown;
		m_mutex.Unlock();
#ifdef _WINDOWS32
		_endthreadex(0);
#else
		pthread_exit(0);
#endif
		return TRUE;
// 		int ticks = (m_StopTimeout*1000)/100;
// 		
// 		for( int i=0; i<ticks; i++ )
// 		{
// 			Sleep(100);
// 			
// 			m_mutex.Lock();
// 			if( m_state == ThreadStateDown )
// 			{
// 				m_mutex.Unlock();
// 				return TRUE;
// 			}
// 			m_mutex.Unlock();
// 			
// 		} 
	}
	catch (char *psz)
	{
#ifdef _WINDOWS32
		//MessageBoxA(NULL,&psz[2],"Fatal exception CThread::Stop",MB_ICONHAND);
		cerr << "Fatal exception CThread::Stop" << psz;
		exit(-1);
#else
		cerr << "Fatal exception CThread::Stop():" << psz;
#endif
		
	}
	return FALSE;
}



//////////////////////////////////////////////////////////////////////////
CMutexClass::CMutexClass(void)
:m_bCreated(TRUE)
{
#ifdef _WINDOWS32
   m_mutex = CreateMutex(NULL,FALSE,NULL);
   if( !m_mutex ) m_bCreated = FALSE;
#else
   pthread_mutexattr_t mattr;

   pthread_mutexattr_init( &mattr );
   pthread_mutex_init(&m_mutex,&mattr);

#endif
   memset(&m_owner,0,sizeof(ThreadId_t));

}

CMutexClass::~CMutexClass(void)
{
#ifdef _WINDOWS32
	WaitForSingleObject(m_mutex,INFINITE);
	CloseHandle(m_mutex);
#else
	pthread_mutex_lock(&m_mutex);
	pthread_mutex_unlock(&m_mutex); 
	pthread_mutex_destroy(&m_mutex);
#endif
}

/**
 *
 * Lock
 * the same thread can not lock the same mutex
 * more than once
 *
 **/
void
CMutexClass::Lock()
{
	ThreadId_t id = CThread::ThreadId();
	try {
		if(CThread::ThreadIdsEqual(&m_owner,&id) )
		    throw "\n\tthe same thread can not acquire a mutex twice!\n"; // the mutex is already locked by this thread
#ifdef _WINDOWS32
		WaitForSingleObject(m_mutex,INFINITE);
#else
		pthread_mutex_lock(&m_mutex);
#endif
		m_owner = CThread::ThreadId();
	}
	catch( char *psz )
	{
#ifdef _WINDOWS32
		//MessageBoxA(NULL,&psz[2],"Fatal exception CMutexClass::Lock",MB_ICONHAND);
		cerr << "Fatal exception CMutexClass::Lock\n";
		exit(-1);
#else
		cerr << "Fatal exception CMutexClass::Lock : " << psz;
#endif


	}

}

/**
 *
 * Unlock
 * releases a mutex.  only the thread that acquires
 * the mutex can release it.
 *
 **/
void 
CMutexClass::Unlock()
{
	ThreadId_t id = CThread::ThreadId();
	try 
	{
	  if( ! CThread::ThreadIdsEqual(&id,&m_owner) )
		throw "\n\tonly the thread that acquires a mutex can release it!"; 

	   memset(&m_owner,0,sizeof(ThreadId_t));
#ifdef _WINDOWS32
	   ReleaseMutex(m_mutex);
#else
	   pthread_mutex_unlock(&m_mutex);
#endif
	}
	catch ( char *psz)
	{
#ifdef _WINDOWS32
		//MessageBoxA(NULL,&psz[2],"Fatal exception CMutexClass::Unlock",MB_ICONHAND);
		cerr << "Fatal exception CMutexClass::Unlock\n";
		exit(-1);
#else
		cerr << "Fatal exception CMutexClass::Unlock : " << psz;
#endif

	}
}


//////////////////////////////////////////////////////////////////////////
CEventClass::CEventClass(void)
:m_bCreated(TRUE)
{
	memset(&m_owner,0,sizeof(ThreadId_t));
#ifdef _WINDOWS32
	m_event = CreateEvent(NULL,FALSE,FALSE,NULL);
	if( !m_event )
	{
		m_bCreated = FALSE;
	}
#else
	pthread_mutexattr_t mattr;
	
	pthread_mutexattr_init(&mattr);
	pthread_mutex_init(&m_lock,&mattr);
	pthread_cond_init(&m_ready,NULL);

#endif	
}

CEventClass::~CEventClass(void)
{
#ifdef _WINDOWS32
	CloseHandle(m_event);
#else
	pthread_cond_destroy(&m_ready);
	pthread_mutex_destroy(&m_lock);
#endif
}


/**
 *
 * Set
 * set an event to signaled
 *
 **/
void
CEventClass::Set()
{
#ifdef _WINDOWS32
	SetEvent(m_event);
#else
	pthread_cond_signal(&m_ready);
#endif
}

/**
 *
 * Wait
 * wait for an event -- wait for an event object
 * to be set to signaled.  must be paired with a
 * call to reset within the same thread.
 *
 **/
BOOL
CEventClass::Wait()
{

	try
	{
		ThreadId_t id = CThread::ThreadId();
		if( CThread::ThreadIdsEqual(&id,&m_owner) )
		{
			throw "\n\tinvalid Wait call, Wait can not be called more than once"
				"\n\twithout a corresponding call to Reset!\n";
		}
		ThreadId_t zero;
		memset(&zero,0,sizeof(ThreadId_t));

		if( memcmp(&zero,&m_owner,sizeof(ThreadId_t)) != 0 )
		{
			throw "\n\tanother thread is already waiting on this event!\n";
		}

		m_owner = CThread::ThreadId();
#ifdef _WINDOWS32
		if( WaitForSingleObject(m_event,INFINITE) != WAIT_OBJECT_0 )
		{
			return FALSE;
		}
#else
		pthread_mutex_lock(&m_lock);
		pthread_cond_wait(&m_ready,&m_lock);
		return TRUE;
#endif
	}
	catch( char *psz )
	{
#ifdef _WINDOWS32
		//MessageBoxA(NULL,&psz[2],"Fatal exception CEventClass::Wait",MB_ICONHAND);
		cerr << "Fatal exception CEventClass::Wait\n";
		exit(-1);
#else
		cerr << "Fatal exception CEventClass::Wait: " << psz;
#endif

	}
	return TRUE;
}

/**
 *
 * Reset
 * reset an event flag to unsignaled
 * wait must be paired with reset within the same thread.
 *
 **/
void
CEventClass::Reset()
{
	try 
	{
		ThreadId_t id = CThread::ThreadId();
		if( !CThread::ThreadIdsEqual(&id,&m_owner) )
		{
			throw "\n\tunbalanced call to Reset, Reset must be called from\n"
				  "\n\tthe same Wait-Reset pair!\n";
		}

		memset(&m_owner,0,sizeof(ThreadId_t));

#ifndef _WINDOWS32
		pthread_mutex_unlock(&m_lock);
#endif
	}
	catch( char *psz )
	{
#ifdef _WINDOWS32
		//MessageBoxA(NULL,&psz[2],"Fatal exception CEventClass::Reset",MB_ICONHAND);
		cerr << "Fatal exception CEventClass::Reset\n";
		exit(-1);
#else
		cerr << "Fatal exception CEventClass::Reset: " << psz;
#endif

	}
}












