/*************************************************************************
   pubfun.h: interface for the CPubFun class.
   Date        Version       Author                Activities      
   ----------------------------------------------------------------------
   2011/03/06   v1.0.0        zhw                  创建
   2011/08/03   v1.0.1        zhw                  将clear修改为erase
   2011/08/03   v1.0.2        zhw                  添加线程类CThread
   2012/05/17   v1.0.3        zhw                  修正部分警告内容
   2012/06/01   v1.0.4        zhw                  增加浮点数转字符串函数PS_ConvertFtoStr
   2012/07/17   v1.0.4        zhw                  去掉MessageBoxA
   2012/07/20   v1.0.5        zhw                  写日志函数中增加字符串越限判断，若越限循环输出
   2012/07/25   v1.0.6        zhw                  去掉MessageBoxA
   2012/11/05   v1.0.7        zhw                  压缩日志文件,最大日志文件个数默认25个,文件大小20M
   2012/11/14   v1.0.8        zhw                  更新写日志函数,文件大小10M
   2012/12/20   v1.0.9        zhw                  添加PS_StringParse
   2013/01/07   V1.0.10       zhw                  添加PS_StringCat
*************************************************************************/

#if !defined(AFX_PUBFUN_H__0E62F986_5E3A_4882_A324_1EAA3D43EEDD__INCLUDED_)
#define AFX_PUBFUN_H__0E62F986_5E3A_4882_A324_1EAA3D43EEDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WINDOWS32
#pragma warning(disable:4786)
#endif

#ifdef _WINDOWS32
//#include <afx.h>     // CObject
#include <windows.h>   // Sleep()
#include <io.h>
#include <direct.h>
#include <sys\stat.h>   //  stat()
#include <process.h>    // _beginthreadex,_endthreadex
#else
#include <unistd.h>     //  sleep()
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/stat.h>   //  stat()
#include <dirent.h>
#include <errno.h>
#include <pthread.h>    //  pthread_mutex_t

typedef unsigned char BOOL;
#define TRUE 1
#define FALSE 0
typedef long unsigned int DWORD;
typedef void *LPVOID;


#endif
#include <time.h>  //  time_t, tm, localtime(), mktime()
#include <vector>
#include <string>  // string
#include <algorithm>
#include <sstream> // stringstream
#include <fstream>
#include <iostream>
#include <strstream>
#include <math.h>  // fabs
#include <sys/timeb.h>   //  timeb, ftime()
#include <string.h>      //  memset(),strcpy()
#include <stdio.h>       //  sprintf()
#include <stdlib.h>      //  atoi
#include <stddef.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>

using namespace std;

// Help out windows:
#if defined( _DEBUG ) && !defined( DEBUG )
#define DEBUG
#endif

#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif

#ifdef TIXML_USE_STL
#define TIXML_STRING		std::string
#else
/*	The support for explicit isn't that universal, and it isn't really
	required - it is used to check that the TiXmlString class isn't incorrectly
	used. Be nice to old compilers and macro it here:
*/
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
	// Microsoft visual studio, version 6 and higher.
	#define TIXML_EXPLICIT explicit
#elif defined(__GNUC__) && (__GNUC__ >= 3 )
	// GCC version 3 and higher.s
	#define TIXML_EXPLICIT explicit
#else
	#define TIXML_EXPLICIT
#endif
#endif

#ifndef TIXML_SAFE
#define TIXML_SAFE
#endif

#ifdef TIXML_SAFE
#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
// Microsoft visual studio, version 2005 and higher.
#define TIXML_SNPRINTF _snprintf_s
#define TIXML_SSCANF   sscanf_s
#elif defined(_MSC_VER) && (_MSC_VER >= 1200 )
// Microsoft visual studio, version 6 and higher.
//#pragma message( "Using _sn* functions." )
#define TIXML_SNPRINTF _snprintf
#define TIXML_SSCANF   sscanf
#elif defined(__GNUC__) && (__GNUC__ >= 3 )
// GCC version 3 and higher.s
//#warning( "Using sn* functions." )
#define TIXML_SNPRINTF snprintf
#define TIXML_SSCANF   sscanf
#else
#define TIXML_SNPRINTF snprintf
#define TIXML_SSCANF   sscanf
#endif
#endif

//#define DEBUG_PARSER
#if defined( DEBUG_PARSER )
#	if defined( DEBUG ) && defined( _MSC_VER )
#		include <windows.h>
#		define TIXML_LOG OutputDebugString
#	else
#		define TIXML_LOG printf
#	endif
#endif

#if defined(WIN32)
#define iniEOL endl
#else
#define iniEOL '\r' << endl
#endif

#ifndef MAXBUF
#define MAXBUF 512
#endif

#define DP_ABS(x)                       ((x)>0?(x):-(x))    //  取绝对值
#define DP_MAX(x,y)                     ((x)>(y)?(x):(y))   //  取最大值
#define DP_MIN(x,y)                     ((x)<(y)?(x):(y))   //  取最小值

#ifndef MAX_KEYNAME
#define MAX_KEYNAME    128
#define MAX_VALUENAME  128
#define MAX_VALUEDATA 2048
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) {if(x) delete x; x=NULL;} // 安全删除new分配出来的变量空间  
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) {if(x) delete[] x; x=NULL;} // 安全删除new分配出来的数组空间   
#endif

#ifndef MAXBACKUPNUM
#define MAXBACKUPNUM 500
#define MAXFILESIZE  10400000  // 10*1024*1024
#endif

#ifndef LOGIDXSUFF
#define LOGIDXSUFF     ".idx"
#define PRODEFDIR      "."
#define PRODEFLOGNAME  "prodeflog.log"
#define PRODEFLOGIDX   "prodeflog.idx"
#define MUTEXNAME      "logmutex"
#endif

#ifndef LTDEBUG
#define LTDEBUG 0
#define LTINFO  1
#define LTDEV   2
#define LTWARN  3
#define LTERROR 4
#endif

#if defined(AS400) || defined(OS400)
typedef pthread_id_np_t ThreadId_t;
#elif defined(VMS) 
typedef pthread_t ThreadId_t;
#else
#ifdef USE_BEGIN_THREAD
typedef unsigned ThreadId_t;
#else
typedef DWORD ThreadId_t;
#endif
#endif

#define QUEUE_SIZE 100
#define DEFAULT_STACK_SIZE 0
#ifndef _WINDOWS32
void Sleep( unsigned int mseconds);
#endif

#ifndef ASSERT
#if defined(DEBUG) || defined(_DEBUG)
#define ASSERT(test) if( !(test) ) { \
fprintf(stderr,"\nASSERT(%s) FAILS, %s line %d\n",#test,__FILE__, __LINE__); exit(0);}
#else
#define ASSERT(test)
#endif
#endif


typedef enum {
    ThreadStateBusy,               // thread is currently handling a task
    ThreadStateWaiting,            // thread is waiting for something to do
    ThreadStateDown,               // thread is not running
    ThreadStateShuttingDown,       // thread is in the process of shutting down
    ThreadStateFault               // an error has occured and the thread could not
    // be launched
} ThreadState_t;

typedef enum {
    ThreadTypeHomogeneous,
    ThreadTypeSpecialized,
    ThreadTypeIntervalDriven,
    ThreadTypeNotDefined } ThreadType_t;
    
    
typedef enum {
    TaskStatusNotSubmitted,
    TaskStatusWaitingOnQueue,
    TaskStatusBeingProcessed,
    TaskStatusCompleted } TaskStatus_t;

//////////////////////////////////////////////////////////////////////////
class CPubTime  
{
public:
	CPubTime();
	virtual ~CPubTime();
public:
    void	PT_DelaySecond( int sec );	                         // 延时sec秒
    void	PT_DelayMinSec( int msec );	                         // 延时msec毫秒
    int	    PT_CalculateTimeInterval(struct  timeb t1, struct  timeb t2); //t2-t1
	int     PT_CalculateTimeInterval(time_t t1, time_t t2);
    int	    PT_GetNowSeconds();
    struct  timeb   PT_GetNowMilliSeconds();	                 // 输出：毫秒
    time_t  PT_GetMonBgnSec(time_t tSecond);	                 // 输出：世纪秒
    time_t  PT_GetDayBgnSec(time_t tSecond);	                 // 输出：世纪秒
    time_t  PT_GetHurBgnSec(time_t tSecond);	                 // 输出：世纪秒
    char *  PT_GetTimeStringBySeconds(const time_t tSecond);     // 输入：世纪秒；输出：YYYY-MM-DD HH:mm:ss
    char *  PT_GetNoSpaceMarkTimeStringBySeconds(time_t tSecond);// 输入：世纪秒；输出：YYYYMMDDHHmmss
    char *  PT_GetChineseTimeStringBySeconds(time_t tSecond);    // 输入：世纪秒；输出：YYYY年MM月DD日HH时mm分ss秒
    char *  PT_GetHMSTimeStringBySeconds(const time_t tSecond);  // 输入：世纪秒；输出：HH:mm:ss		
    char *  PT_GetStringOfTimeInterval(int msec);                // 输入：毫秒；  输出：x分x秒x毫秒
    int     PT_GetSecondByTimeString(const char * timestring, int &nSecond);      // 将时间字符串YYYY-MM-DD HH:MM:SS转换成世纪秒；    输出：0正常，-1异常
    int     PT_GetSecondByTimeString(const char * timestring);  // 将时间字符串YYYY-MM-DD HH:MM:SS转换成世纪秒
    int     PT_GetTmTimeByTimeString(const char * timestring, struct tm &sTime);  // 将时间字符串YYYY-MM-DD HH:MM:SS转换成时间结构体；输出：0正常，-1异常
};

//////////////////////////////////////////////////////////////////////////
template<class out_type,class in_value>
out_type SF_convert(const in_value & t);    // 待转换的数据

class CPubString  
{
public:
	CPubString();
	virtual ~CPubString();
public:
    //字符串操作类函数声明
    int     PS_StringTrimLeft(string &str);
    int     PS_StringTrimRight(string &str);
    int     PS_StringTrimAll(string &str);
    int     PS_StringMakeUpper(string &str);                // 将字符串转换为大写
    int     PS_StringMakeLower(string &str);                // 将字符串转换为小写
    char*   PS_CharTrimLeft(char* str);
    char*   PS_CharTrimRight(char* str);
    int     PS_TrimBlank(string  &str);
    int 	PS_TrimABC(string &Name,int mode);
    int     PS_StringStrrev(char* s);
    int     PS_FillLine(string &str,int NB);
    int     PS_FloatToChar( float x,int k,char *xstr);
    int     PS_FloatToStr(float x,int k,string &xstr);
    int     PS_StringCpy();
    int     PS_StringCat(char *deschar, char *srcchar, const int deslen, const int srclen, const int maxlen);
    char*   PS_dectobin(int dec,int len);
    int     PS_Str2Char(const string strSrc, char* CharDes, int Charlen);
	int     PS_MemCpy(char *CharDes, const char* CharSrc, const int Charlen );
	int     PS_GetFaultNo(char *fault_desc);
	int     PS_ConvertFtoStr( const double data, const int count, char *strchar );
	int     PS_PreConvertFtoStr( double &source, const int count );
	int     PS_GetGenName13(char *name13, const char *name, const float &voltbase, const char *id);
	int     PS_StringParse(string strline,vector<string> &vecline);
	int		PS_StringDivde(const string &strSrc, const string & strSpliter, vector<string> &vDivStr);
};

//////////////////////////////////////////////////////////////////////////
class CPubFile  
{
public:
	CPubFile();
	virtual ~CPubFile();
public:
    int   PF_CreateFile(const string filename,const string msg = "" );  // 创建文件
    bool  PF_FindFile(const string filename);                            // 判断文件状态
    bool  PF_FindDir(const string dirname);
    int   PF_RemoveFile(const string filename );                         // 删除文件
    int   PF_CopyFileM(const string fileSrc, const string fileDes);       // 复制文件
    int   PF_ClearDir(const string dirname);                             // 清空目录
    int   PF_ClearDir(const string dirname, const vector<string> filetype, const int mode = 0);
    char* PF_Error();                                                    // 错误信息
    int   PF_MkDir(const string dirname);
    char* PF_GetCurDir();
	int   PF_CopyDir(const string srcdir,const string desdir);
	int   PF_CopyDir(const string srcdir,const string desdir, const vector<string> filetype);
	string PF_GetFileName(const string absfilename);
	int   PF_MemoryTest(const string proname, const string filename);
public:
	int    PF_SetFileFormat(const bool ByteOrder, const bool BinFormat );
	void   PF_SwapByteOrder(int length, void *wordP); // 调整字节序
    int    PF_ReadBinInt(FILE *pFilePtr );
    float  PF_ReadBinFloat(FILE *pFilePtr);
    double PF_ReadBinDouble(FILE *pFilePtr);
    char * PF_ReadBinChars(char * pBuf, int nMaxLen,FILE *pFilePtr);
	
    long   PF_FSeek(FILE *pFilePtr);                 // 移动文件指针2*sizeof(int)
    long   PF_FSeek(const long &nPos,FILE *pFilePtr); // 移动文件指针nPos
private:
    char PF_logstr[MAXBUF];
	char PF_backslash[2];
    void PF_StringTrimAll(string &str);
private:
	bool   PF_ByteOrder;  // 是否转换字节序
    bool   PF_BinFormat;  // 是否去头尾整数
	
    size_t PF_IntSize;    // 整型         sizeof(int)
    size_t PF_Floatsize;  // 浮点数       sizeof(float)
};
//////////////////////////////////////////////////////////////////////////
class CPubLogFile  
{
public:
	CPubLogFile();
	virtual ~CPubLogFile();
public:
    int  PLF_InitLogFile(const string& fileName,
        size_t maxFileSize = MAXFILESIZE, 
        unsigned int maxBackupNum = MAXBACKUPNUM); // 初始化日志信息
    
    void PLF_setMaxBackupNum(unsigned int maxBackups); // 设置最大文件个数
    unsigned int PLF_getMaxBackupNum();                // 获取最大文件个数
    void PLF_setMaxFileSize(size_t maxFileSize);       // 设置文件大小
    long int  PLF_getMaxFileSize();                    // 获取文件大小
    char * PLF_Error();                                // 错误信息
    int    PLF_WriteLogFile( const string logstr,const int nlogtype = LTINFO );     // 记录日志
    int    PLF_MessShow( const string logstr ,const int nlogtype = LTINFO);        // 记录并显示日志
    void   PLF_setLogType(int nlogtype = LTINFO );
public:
    char PLF_logstr[MAXBUF];
	char PLF_backslash[2];
private:
	int    PLF_CheckLogFile();                         // 检查日志文件大小
    int    PLF_StringTrimAll( string &str );
    char*  PLF_GetNowTime();
    char*  PLF_GetNowTime1();
    char*  PLF_GetNowTimeString();
    int    PLF_GetLogTypeStr(const int nlogtype,string &LogTypeStr);
	int    PLF_ShowLog(const string logstr);
public:
    string PLF_LogName;
    string PLF_LogDir;
    string PLF_LogIdx;
    int    PLF_logtype;
	int    PLF_IsZip;  // 是否压缩日志文件;1,压缩;0,不压缩
	
    unsigned int PLF_maxBackupNum;
    long int  PLF_maxFileSize;
private:
#ifdef _WINDOWS32
    HANDLE PLF_mutex;
#else
    pthread_mutex_t PLF_mutex;
#endif
};
//////////////////////////////////////////////////////////////////////////
// IniFile.cpp:  Implementation of the CIniFile class.
// Written by:   Adam Clauss
// Email: cabadam@tamu.edu
// You may use this class/code as you wish in your programs.  Feel free to distribute it, and
// email suggested changes to me.
//
// Rewritten by: Shane Hill
// Date:         21/08/2001
// Email:        Shane.Hill@dsto.defence.gov.au
// Reason:       Remove dependancy on MFC. Code should compile on any
//               platform. Tested on Windows/Linux/Irix
//
// INI file's format: 
// [Section01] 
//  # this is a comment
//      Key01 = value01 ;
//      Key02 = value02 ;
//////////////////////////////////////////////////////////////////////

class CIniFile  
{
private:
  bool   caseInsensitive;
  string path;
  struct key {
    vector<string> names;
    vector<string> values; 
    vector<string> comments;
    vector<string> valcmts;
  };
  vector<key>    keys; 
  vector<string> names; 
  vector<string> comments;
  string INI_CheckCase( string s) const;

public:
  enum errors{ noID = -1};
  CIniFile( string const iniPath = "");
  virtual ~CIniFile()                            {}

  // Sets whether or not keynames and valuenames should be case sensitive.
  // The default is case insensitive.
  void INI_CaseSensitive()                           {caseInsensitive = false;}
  void INI_CaseInsensitive()                         {caseInsensitive = true;}

  // Sets path of ini file to read and write from.
  void INI_Path(string const newPath)                {path = newPath;}
  string INI_Path() const                            {return path;}
  void INI_SetPath(string const newPath)             {INI_Path( newPath);}

  // Reads ini file specified using path.
  // Returns true if successful, false otherwise.
  bool INI_ReadFile();
  bool INI_ReadFile( string const newPath );
  bool INI_ReadTskFile();
  bool INI_ReadTskFile( string const newPath );
  
  // Writes data stored in class to ini file.
  bool INI_WriteFile(); 
  bool INI_WriteFile( string const newPaht );

  bool INI_WriteTskFile( string const newPaht );
  
  // Deletes all stored ini data.
  void INI_Erase();
  void INI_Clear()                                   {INI_Erase();}
  void INI_Reset()                                   {INI_Erase();}

  // Returns index of specified key, or noID if not found.
  long INI_FindKey( string const keyname) const;

  // Returns index of specified value, in the specified key, or noID if not found.
  long INI_FindValue( unsigned const keyID, string const valuename) const;

  // Returns number of keys currently in the ini.
  unsigned INI_NumKeys() const                       {return names.size();}
  unsigned INI_GetNumKeys() const                    {return INI_NumKeys();}

  // Add a key name.
  unsigned INI_AddKeyName( string const keyname);

  // Returns key names by index.
  string INI_KeyName( unsigned const keyID) const;
  string INI_GetKeyName( unsigned const keyID) const {return INI_KeyName(keyID);}

  // Returns number of values stored for specified key.
  unsigned INI_NumValues( unsigned const keyID);
  unsigned INI_GetNumValues( unsigned const keyID)   {return INI_NumValues( keyID);}
  unsigned INI_NumValues( string const keyname);
  unsigned INI_GetNumValues( string const keyname)   {return INI_NumValues( keyname);}

  // Returns value name by index for a given keyname or keyID.
  string INI_ValueName( unsigned const keyID, unsigned const valueID) const;
  string INI_GetValueName( unsigned const keyID, unsigned const valueID) const {
    return INI_ValueName( keyID, valueID);
  }
  string INI_ValueName( string const keyname, unsigned const valueID) const;
  string INI_GetValueName( string const keyname, unsigned const valueID) const {
    return INI_ValueName( keyname, valueID);
  }

  // Gets value of [keyname] valuename =.
  // Overloaded to return string, int, and double.
  // Returns defValue if key/value not found.
  string INI_GetValue( unsigned const keyID, unsigned const valueID, string const defValue = "") const;
  string INI_GetValue(string const keyname, string const valuename, string const defValue = "") const; 
  int    INI_GetValueI(string const keyname, string const valuename, int const defValue = 0) const;
  bool   INI_GetValueB(string const keyname, string const valuename, bool const defValue = false) const {
	  int tmpvalue = INI_GetValueI( keyname, valuename, int( defValue));
	  if (tmpvalue==0)
	  {
		  return false;
	  }
	  else
	  {
		  return true;
	  }
  }
  int    INI_GetValueBI(string const keyname, string const valuename, int const defValue = 0) const;
  bool   INI_GetValueBB(string const keyname, string const valuename, bool const defValue = false ) const;
  double   INI_GetValueF(string const keyname, string const valuename, double const defValue = 0.0) const;
  float    INI_GetValueFF(string const keyname, string const valuename, double const defValue = 0.0) const;
  // This is a variable length formatted GetValue routine. All these voids
  // are required because there is no vsscanf() like there is a vsprintf().
  // Only a maximum of 8 variable can be read.
  unsigned INI_GetValueV( string const keyname, string const valuename, char *format,
		      void *v1 = 0, void *v2 = 0, void *v3 = 0, void *v4 = 0,
  		      void *v5 = 0, void *v6 = 0, void *v7 = 0, void *v8 = 0,
  		      void *v9 = 0, void *v10 = 0, void *v11 = 0, void *v12 = 0,
  		      void *v13 = 0, void *v14 = 0, void *v15 = 0, void *v16 = 0);

  // Sets value of [keyname] valuename =.
  // Specify the optional paramter as false (0) if you do not want it to create
  // the key if it doesn't exist. Returns true if data entered, false otherwise.
  // Overloaded to accept string, int, and double.
  bool INI_SetValue( unsigned const keyID, unsigned const valueID, string const value);
  bool INI_SetValue( string const keyname, string const valuename, string const value, bool const create = true);
  bool INI_SetValue( string const keyname, string const valuename, string const value, string const comments, bool const create = true);
  bool INI_SetValueI( string const keyname, string const valuename, int const value, bool const create = true);
  bool INI_SetValueB( string const keyname, string const valuename, bool const value, bool const create = true) {
    return INI_SetValueI( keyname, valuename, int(value), create);
  }
  bool INI_SetValueF( string const keyname, string const valuename, double const value, bool const create = true);
  bool INI_SetValueV( string const keyname, string const valuename, char *format, ...);

  // Deletes specified value.
  // Returns true if value existed and deleted, false otherwise.
  bool INI_DeleteValue( string const keyname, string const valuename);
  
  // Deletes specified key and all values contained within.
  // Returns true if key existed and deleted, false otherwise.
  bool INI_DeleteKey(string keyname);

  // Header comment functions.
  // Header comments are those comments before the first key.
  //
  // Number of header comments.
  unsigned INI_NumHeaderComments()                  {return comments.size();}
  // Add a header comment.
  void     INI_HeaderComment( string const comment);
  // Return a header comment.
  string   INI_HeaderComment( unsigned const commentID) const;
  // Delete a header comment.
  bool     INI_DeleteHeaderComment( unsigned commentID);
  // Delete all header comments.
  void     INI_DeleteHeaderComments()               {comments.clear();}

  // Key comment functions.
  // Key comments are those comments within a key. Any comments
  // defined within value names will be added to this list. Therefore,
  // these comments will be moved to the top of the key definition when
  // the CIniFile::WriteFile() is called.
  //
  // Number of key comments.
  unsigned INI_NumKeyComments( unsigned const keyID) const;
  unsigned INI_NumKeyComments( string const keyname) const;
  // Add a key comment.
  bool     INI_KeyComment( unsigned const keyID, string const comment);
  bool     INI_KeyComment( string const keyname, string const comment);
  // Return a key comment.
  string   INI_KeyComment( unsigned const keyID, unsigned const commentID) const;
  string   INI_KeyComment( string const keyname, unsigned const commentID) const;
  // Delete a key comment.
  bool     INI_DeleteKeyComment( unsigned const keyID, unsigned const commentID);
  bool     INI_DeleteKeyComment( string const keyname, unsigned const commentID);
  // Delete all comments for a key.
  bool     INI_DeleteKeyComments( unsigned const keyID);
  bool     INI_DeleteKeyComments( string const keyname);
private: 
  // Helper functions
  void      INI_LTrim(string &strSrc);
  void      INI_RTrim(string &strSrc);
  void      INI_TrimAll(string &strSrc);
};
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
//////////////////////////////////////////////////////////////////////////
/*
TiXmlString is an emulation of a subset of the std::string template.
Its purpose is to allow compiling TinyXML on compilers with no or poor STL support.
Only the member functions relevant to the TinyXML project have been implemented.
The buffer allocation is made by a simplistic power of 2 like mechanism : if we increase
a string and there's no more room, we allocate a buffer twice as big as we need.
*/
#ifndef TIXML_USE_STL

#ifndef TIXML_STRING_INCLUDED
#define TIXML_STRING_INCLUDED
class TiXmlString
{
public :
	// The size type used
	typedef size_t size_type;
	
	// Error value for find primitive
	static const size_type npos; // = -1;
	
	
	// TiXmlString empty constructor
	TiXmlString () : rep_(&nullrep_)
	{
	}
	
	// TiXmlString copy constructor
	TiXmlString ( const TiXmlString & copy) : rep_(0)
	{
		init(copy.length());
		memcpy(start(), copy.data(), length());
	}
	
	// TiXmlString constructor, based on a string
	TIXML_EXPLICIT TiXmlString ( const char * copy) : rep_(0)
	{
		init( static_cast<size_type>( strlen(copy) ));
		memcpy(start(), copy, length());
	}
	
	// TiXmlString constructor, based on a string
	TIXML_EXPLICIT TiXmlString ( const char * str, size_type len) : rep_(0)
	{
		init(len);
		memcpy(start(), str, len);
	}
	
	// TiXmlString destructor
	~TiXmlString ()
	{
		quit();
	}
	
	// = operator
	TiXmlString& operator = (const char * copy)
	{
		return assign( copy, (size_type)strlen(copy));
	}
	
	// = operator
	TiXmlString& operator = (const TiXmlString & copy)
	{
		return assign(copy.start(), copy.length());
	}
	
	
	// += operator. Maps to append
	TiXmlString& operator += (const char * suffix)
	{
		return append(suffix, static_cast<size_type>( strlen(suffix) ));
	}
	
	// += operator. Maps to append
	TiXmlString& operator += (char single)
	{
		return append(&single, 1);
	}
	
	// += operator. Maps to append
	TiXmlString& operator += (const TiXmlString & suffix)
	{
		return append(suffix.data(), suffix.length());
	}
	
	
	// Convert a TiXmlString into a null-terminated char *
	const char * c_str () const { return rep_->str; }
	
	// Convert a TiXmlString into a char * (need not be null terminated).
	const char * data () const { return rep_->str; }
	
	// Return the length of a TiXmlString
	size_type length () const { return rep_->size; }
	
	// Alias for length()
	size_type size () const { return rep_->size; }
	
	// Checks if a TiXmlString is empty
	bool empty () const { return rep_->size == 0; }
	
	// Return capacity of string
	size_type capacity () const { return rep_->capacity; }
	
	
	// single char extraction
	const char& at (size_type index) const
	{
		assert( index < length() );
		return rep_->str[ index ];
	}
	
	// [] operator
	char& operator [] (size_type index) const
	{
		assert( index < length() );
		return rep_->str[ index ];
	}
	
	// find a char in a string. Return TiXmlString::npos if not found
	size_type find (char lookup) const
	{
		return find(lookup, 0);
	}
	
	// find a char in a string from an offset. Return TiXmlString::npos if not found
	size_type find (char tofind, size_type offset) const
	{
		if (offset >= length()) return npos;
		
		for (const char* p = c_str() + offset; *p != '\0'; ++p)
		{
			if (*p == tofind) return static_cast< size_type >( p - c_str() );
		}
		return npos;
	}
	
	void clear ()
	{
		//Lee:
		//The original was just too strange, though correct:
		//	TiXmlString().swap(*this);
		//Instead use the quit & re-init:
		quit();
		init(0,0);
	}
	
	/*	Function to reserve a big amount of data when we know we'll need it. Be aware that this
	function DOES NOT clear the content of the TiXmlString if any exists.
	*/
	void reserve (size_type cap);
	
	TiXmlString& assign (const char* str, size_type len);
	
	TiXmlString& append (const char* str, size_type len);
	
	void swap (TiXmlString& other)
	{
		Rep* r = rep_;
		rep_ = other.rep_;
		other.rep_ = r;
	}
	
  private:
	  
	  void init(size_type sz) { init(sz, sz); }
	  void set_size(size_type sz) { rep_->str[ rep_->size = sz ] = '\0'; }
	  char* start() const { return rep_->str; }
	  char* finish() const { return rep_->str + rep_->size; }
	  
	  struct Rep
	  {
		  size_type size, capacity;
		  char str[1];
	  };
	  
	  void init(size_type sz, size_type cap)
	  {
		  if (cap)
		  {
			  // Lee: the original form:
			  //	rep_ = static_cast<Rep*>(operator new(sizeof(Rep) + cap));
			  // doesn't work in some cases of new being overloaded. Switching
			  // to the normal allocation, although use an 'int' for systems
			  // that are overly picky about structure alignment.
			  const size_type bytesNeeded = sizeof(Rep) + cap;
			  const size_type intsNeeded = ( bytesNeeded + sizeof(int) - 1 ) / sizeof( int ); 
			  rep_ = reinterpret_cast<Rep*>( new int[ intsNeeded ] );
			  
			  rep_->str[ rep_->size = sz ] = '\0';
			  rep_->capacity = cap;
		  }
		  else
		  {
			  rep_ = &nullrep_;
		  }
	  }
	  
	  void quit()
	  {
		  if (rep_ != &nullrep_)
		  {
			  // The rep_ is really an array of ints. (see the allocator, above).
			  // Cast it back before delete, so the compiler won't incorrectly call destructors.
			  delete [] ( reinterpret_cast<int*>( rep_ ) );
		  }
	  }
	  
	  Rep * rep_;
	  static Rep nullrep_;
	  
} ;


inline bool operator == (const TiXmlString & a, const TiXmlString & b)
{
	return    ( a.length() == b.length() )				// optimization on some platforms
	       && ( strcmp(a.c_str(), b.c_str()) == 0 );	// actual compare
}
inline bool operator < (const TiXmlString & a, const TiXmlString & b)
{
	return strcmp(a.c_str(), b.c_str()) < 0;
}

inline bool operator != (const TiXmlString & a, const TiXmlString & b) { return !(a == b); }
inline bool operator >  (const TiXmlString & a, const TiXmlString & b) { return b < a; }
inline bool operator <= (const TiXmlString & a, const TiXmlString & b) { return !(b < a); }
inline bool operator >= (const TiXmlString & a, const TiXmlString & b) { return !(a < b); }

inline bool operator == (const TiXmlString & a, const char* b) { return strcmp(a.c_str(), b) == 0; }
inline bool operator == (const char* a, const TiXmlString & b) { return b == a; }
inline bool operator != (const TiXmlString & a, const char* b) { return !(a == b); }
inline bool operator != (const char* a, const TiXmlString & b) { return !(b == a); }

TiXmlString operator + (const TiXmlString & a, const TiXmlString & b);
TiXmlString operator + (const TiXmlString & a, const char* b);
TiXmlString operator + (const char* a, const TiXmlString & b);


/*
TiXmlOutStream is an emulation of std::ostream. It is based on TiXmlString.
Only the operators that we need for TinyXML have been developped.
*/
class TiXmlOutStream : public TiXmlString
{
public :
	
	// TiXmlOutStream << operator.
	TiXmlOutStream & operator << (const TiXmlString & in)
	{
		*this += in;
		return *this;
	}
	
	// TiXmlOutStream << operator.
	TiXmlOutStream & operator << (const char * in)
	{
		*this += in;
		return *this;
	}
	
} ;
#endif	// TIXML_STRING_INCLUDED
#endif	// TIXML_USE_STL

// Deprecated library function hell. Compilers want to use the
// new safe versions. This probably doesn't fully address the problem,
// but it gets closer. There are too many compilers for me to fully
// test. If you get compilation troubles, undefine TIXML_SAFE

class TiXmlDocument;
class TiXmlElement;
class TiXmlComment;
class TiXmlUnknown;
class TiXmlAttribute;
class TiXmlText;
class TiXmlDeclaration;
class TiXmlParsingData;

const int TIXML_MAJOR_VERSION = 2;
const int TIXML_MINOR_VERSION = 6;
const int TIXML_PATCH_VERSION = 1;

/*	Internal structure for tracking location of items 
	in the XML file.
*/
struct TiXmlCursor
{
	TiXmlCursor()		{ Clear(); }
	void Clear()		{ row = col = -1; }

	int row;	// 0 based.
	int col;	// 0 based.
};


/**
	Implements the interface to the "Visitor pattern" (see the Accept() method.)
	If you call the Accept() method, it requires being passed a TiXmlVisitor
	class to handle callbacks. For nodes that contain other nodes (Document, Element)
	you will get called with a VisitEnter/VisitExit pair. Nodes that are always leaves
	are simply called with Visit().

	If you return 'true' from a Visit method, recursive parsing will continue. If you return
	false, <b>no children of this node or its sibilings</b> will be Visited.

	All flavors of Visit methods have a default implementation that returns 'true' (continue 
	visiting). You need to only override methods that are interesting to you.

	Generally Accept() is called on the TiXmlDocument, although all nodes suppert Visiting.

	You should never change the document from a callback.

	@sa TiXmlNode::Accept()
*/
class TiXmlVisitor
{
public:
	virtual ~TiXmlVisitor() {}

	/// Visit a document.
	virtual bool VisitEnter( const TiXmlDocument& /*doc*/ )			{ return true; }
	/// Visit a document.
	virtual bool VisitExit( const TiXmlDocument& /*doc*/ )			{ return true; }

	/// Visit an element.
	virtual bool VisitEnter( const TiXmlElement& /*element*/, const TiXmlAttribute* /*firstAttribute*/ )	{ return true; }
	/// Visit an element.
	virtual bool VisitExit( const TiXmlElement& /*element*/ )		{ return true; }

	/// Visit a declaration
	virtual bool Visit( const TiXmlDeclaration& /*declaration*/ )	{ return true; }
	/// Visit a text node
	virtual bool Visit( const TiXmlText& /*text*/ )					{ return true; }
	/// Visit a comment node
	virtual bool Visit( const TiXmlComment& /*comment*/ )			{ return true; }
	/// Visit an unknow node
	virtual bool Visit( const TiXmlUnknown& /*unknown*/ )			{ return true; }
};

// Only used by Attribute::Query functions
enum 
{ 
	TIXML_SUCCESS,
	TIXML_NO_ATTRIBUTE,
	TIXML_WRONG_TYPE
};


// Used by the parsing routines.
enum TiXmlEncoding
{
	TIXML_ENCODING_UNKNOWN,
	TIXML_ENCODING_UTF8,
	TIXML_ENCODING_LEGACY
};

const TiXmlEncoding TIXML_DEFAULT_ENCODING = TIXML_ENCODING_UNKNOWN;

/** TiXmlBase is a base class for every class in TinyXml.
	It does little except to establish that TinyXml classes
	can be printed and provide some utility functions.

	In XML, the document and elements can contain
	other elements and other types of nodes.

	@verbatim
	A Document can contain:	Element	(container or leaf)
							Comment (leaf)
							Unknown (leaf)
							Declaration( leaf )

	An Element can contain:	Element (container or leaf)
							Text	(leaf)
							Attributes (not on tree)
							Comment (leaf)
							Unknown (leaf)

	A Decleration contains: Attributes (not on tree)
	@endverbatim
*/
class TiXmlBase
{
	friend class TiXmlNode;
	friend class TiXmlElement;
	friend class TiXmlDocument;

public:
	TiXmlBase()	:	userData(0)		{}
	virtual ~TiXmlBase()			{}

	/**	All TinyXml classes can print themselves to a filestream
		or the string class (TiXmlString in non-STL mode, std::string
		in STL mode.) Either or both cfile and str can be null.
		
		This is a formatted print, and will insert 
		tabs and newlines.
		
		(For an unformatted stream, use the << operator.)
	*/
	virtual void Print( FILE* cfile, int depth ) const = 0;

	/**	The world does not agree on whether white space should be kept or
		not. In order to make everyone happy, these global, static functions
		are provided to set whether or not TinyXml will condense all white space
		into a single space or not. The default is to condense. Note changing this
		value is not thread safe.
	*/
	static void SetCondenseWhiteSpace( bool condense )		{ condenseWhiteSpace = condense; }

	/// Return the current white space setting.
	static bool IsWhiteSpaceCondensed()						{ return condenseWhiteSpace; }

	/** Return the position, in the original source file, of this node or attribute.
		The row and column are 1-based. (That is the first row and first column is
		1,1). If the returns values are 0 or less, then the parser does not have
		a row and column value.

		Generally, the row and column value will be set when the TiXmlDocument::Load(),
		TiXmlDocument::LoadFile(), or any TiXmlNode::Parse() is called. It will NOT be set
		when the DOM was created from operator>>.

		The values reflect the initial load. Once the DOM is modified programmatically
		(by adding or changing nodes and attributes) the new values will NOT update to
		reflect changes in the document.

		There is a minor performance cost to computing the row and column. Computation
		can be disabled if TiXmlDocument::SetTabSize() is called with 0 as the value.

		@sa TiXmlDocument::SetTabSize()
	*/
	int Row() const			{ return location.row + 1; }
	int Column() const		{ return location.col + 1; }	///< See Row()

	void  SetUserData( void* user )			{ userData = user; }	///< Set a pointer to arbitrary user data.
	void* GetUserData()						{ return userData; }	///< Get a pointer to arbitrary user data.
	const void* GetUserData() const 		{ return userData; }	///< Get a pointer to arbitrary user data.

	// Table that returs, for a given lead byte, the total number of bytes
	// in the UTF-8 sequence.
	static const int utf8ByteTable[256];

	virtual const char* Parse(	const char* p, 
								TiXmlParsingData* data, 
								TiXmlEncoding encoding /*= TIXML_ENCODING_UNKNOWN */ ) = 0;

	/** Expands entities in a string. Note this should not contian the tag's '<', '>', etc, 
		or they will be transformed into entities!
	*/
	static void EncodeString( const TIXML_STRING& str, TIXML_STRING* out );

	enum
	{
		TIXML_NO_ERROR = 0,
		TIXML_ERROR,
		TIXML_ERROR_OPENING_FILE,
		TIXML_ERROR_PARSING_ELEMENT,
		TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME,
		TIXML_ERROR_READING_ELEMENT_VALUE,
		TIXML_ERROR_READING_ATTRIBUTES,
		TIXML_ERROR_PARSING_EMPTY,
		TIXML_ERROR_READING_END_TAG,
		TIXML_ERROR_PARSING_UNKNOWN,
		TIXML_ERROR_PARSING_COMMENT,
		TIXML_ERROR_PARSING_DECLARATION,
		TIXML_ERROR_DOCUMENT_EMPTY,
		TIXML_ERROR_EMBEDDED_NULL,
		TIXML_ERROR_PARSING_CDATA,
		TIXML_ERROR_DOCUMENT_TOP_ONLY,

		TIXML_ERROR_STRING_COUNT
	};

protected:

	static const char* SkipWhiteSpace( const char*, TiXmlEncoding encoding );

	inline static bool IsWhiteSpace( char c )		
	{ 
		return ( isspace( (unsigned char) c ) || c == '\n' || c == '\r' ); 
	}
	inline static bool IsWhiteSpace( int c )
	{
		if ( c < 256 )
			return IsWhiteSpace( (char) c );
		return false;	// Again, only truly correct for English/Latin...but usually works.
	}

	#ifdef TIXML_USE_STL
	static bool	StreamWhiteSpace( std::istream * in, TIXML_STRING * tag );
	static bool StreamTo( std::istream * in, int character, TIXML_STRING * tag );
	#endif

	/*	Reads an XML name into the string provided. Returns
		a pointer just past the last character of the name,
		or 0 if the function has an error.
	*/
	static const char* ReadName( const char* p, TIXML_STRING* name, TiXmlEncoding encoding );

	/*	Reads text. Returns a pointer past the given end tag.
		Wickedly complex options, but it keeps the (sensitive) code in one place.
	*/
	static const char* ReadText(	const char* in,				// where to start
									TIXML_STRING* text,			// the string read
									bool ignoreWhiteSpace,		// whether to keep the white space
									const char* endTag,			// what ends this text
									bool ignoreCase,			// whether to ignore case in the end tag
									TiXmlEncoding encoding );	// the current encoding

	// If an entity has been found, transform it into a character.
	static const char* GetEntity( const char* in, char* value, int* length, TiXmlEncoding encoding );

	// Get a character, while interpreting entities.
	// The length can be from 0 to 4 bytes.
	inline static const char* GetChar( const char* p, char* _value, int* length, TiXmlEncoding encoding )
	{
		assert( p );
		if ( encoding == TIXML_ENCODING_UTF8 )
		{
			*length = utf8ByteTable[ *((const unsigned char*)p) ];
			assert( *length >= 0 && *length < 5 );
		}
		else
		{
			*length = 1;
		}

		if ( *length == 1 )
		{
			if ( *p == '&' )
				return GetEntity( p, _value, length, encoding );
			*_value = *p;
			return p+1;
		}
		else if ( *length )
		{
			//strncpy( _value, p, *length );	// lots of compilers don't like this function (unsafe),
												// and the null terminator isn't needed
			for( int i=0; p[i] && i<*length; ++i ) {
				_value[i] = p[i];
			}
			return p + (*length);
		}
		else
		{
			// Not valid text.
			return 0;
		}
	}

	// Return true if the next characters in the stream are any of the endTag sequences.
	// Ignore case only works for english, and should only be relied on when comparing
	// to English words: StringEqual( p, "version", true ) is fine.
	static bool StringEqual(	const char* p,
								const char* endTag,
								bool ignoreCase,
								TiXmlEncoding encoding );

	static const char* errorString[ TIXML_ERROR_STRING_COUNT ];

	TiXmlCursor location;

    /// Field containing a generic user pointer
	void*			userData;
	
	// None of these methods are reliable for any language except English.
	// Good for approximation, not great for accuracy.
	static int IsAlpha( unsigned char anyByte, TiXmlEncoding encoding );
	static int IsAlphaNum( unsigned char anyByte, TiXmlEncoding encoding );
	inline static int ToLower( int v, TiXmlEncoding encoding )
	{
		if ( encoding == TIXML_ENCODING_UTF8 )
		{
			if ( v < 128 ) return tolower( v );
			return v;
		}
		else
		{
			return tolower( v );
		}
	}
	static void ConvertUTF32ToUTF8( unsigned long input, char* output, int* length );

private:
	TiXmlBase( const TiXmlBase& );				// not implemented.
	void operator=( const TiXmlBase& base );	// not allowed.

	struct Entity
	{
		const char*     str;
		unsigned int	strLength;
		char		    chr;
	};
	enum
	{
		NUM_ENTITY = 5,
		MAX_ENTITY_LENGTH = 6

	};
	static Entity entity[ NUM_ENTITY ];
	static bool condenseWhiteSpace;
};


/** The parent class for everything in the Document Object Model.
	(Except for attributes).
	Nodes have siblings, a parent, and children. A node can be
	in a document, or stand on its own. The type of a TiXmlNode
	can be queried, and it can be cast to its more defined type.
*/
class TiXmlNode : public TiXmlBase
{
	friend class TiXmlDocument;
	friend class TiXmlElement;

public:
	#ifdef TIXML_USE_STL	

	    /** An input stream operator, for every class. Tolerant of newlines and
		    formatting, but doesn't expect them.
	    */
	    friend std::istream& operator >> (std::istream& in, TiXmlNode& base);

	    /** An output stream operator, for every class. Note that this outputs
		    without any newlines or formatting, as opposed to Print(), which
		    includes tabs and new lines.

		    The operator<< and operator>> are not completely symmetric. Writing
		    a node to a stream is very well defined. You'll get a nice stream
		    of output, without any extra whitespace or newlines.
		    
		    But reading is not as well defined. (As it always is.) If you create
		    a TiXmlElement (for example) and read that from an input stream,
		    the text needs to define an element or junk will result. This is
		    true of all input streams, but it's worth keeping in mind.

		    A TiXmlDocument will read nodes until it reads a root element, and
			all the children of that root element.
	    */	
	    friend std::ostream& operator<< (std::ostream& out, const TiXmlNode& base);

		/// Appends the XML node or attribute to a std::string.
		friend std::string& operator<< (std::string& out, const TiXmlNode& base );

	#endif

	/** The types of XML nodes supported by TinyXml. (All the
			unsupported types are picked up by UNKNOWN.)
	*/
	enum NodeType
	{
		TINYXML_DOCUMENT,
		TINYXML_ELEMENT,
		TINYXML_COMMENT,
		TINYXML_UNKNOWN,
		TINYXML_TEXT,
		TINYXML_DECLARATION,
		TINYXML_TYPECOUNT
	};

	virtual ~TiXmlNode();

	/** The meaning of 'value' changes for the specific type of
		TiXmlNode.
		@verbatim
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
		@endverbatim

		The subclasses will wrap this function.
	*/
	const char *Value() const { return value.c_str (); }

    #ifdef TIXML_USE_STL
	/** Return Value() as a std::string. If you only use STL,
	    this is more efficient than calling Value().
		Only available in STL mode.
	*/
	const std::string& ValueStr() const { return value; }
	#endif

	const TIXML_STRING& ValueTStr() const { return value; }

	/** Changes the value of the node. Defined as:
		@verbatim
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
		@endverbatim
	*/
	void SetValue(const char * _value) { value = _value;}

    #ifdef TIXML_USE_STL
	/// STL std::string form.
	void SetValue( const std::string& _value )	{ value = _value; }
	#endif

	/// Delete all the children of this node. Does not affect 'this'.
	void Clear();

	/// One step up the DOM.
	TiXmlNode* Parent()							{ return parent; }
	const TiXmlNode* Parent() const				{ return parent; }

	const TiXmlNode* FirstChild()	const		{ return firstChild; }	///< The first child of this node. Will be null if there are no children.
	TiXmlNode* FirstChild()						{ return firstChild; }
	const TiXmlNode* FirstChild( const char * value ) const;			///< The first child of this node with the matching 'value'. Will be null if none found.
	/// The first child of this node with the matching 'value'. Will be null if none found.
	TiXmlNode* FirstChild( const char * _value ) {
		// Call through to the const version - safe since nothing is changed. Exiting syntax: cast this to a const (always safe)
		// call the method, cast the return back to non-const.
		return const_cast< TiXmlNode* > ((const_cast< const TiXmlNode* >(this))->FirstChild( _value ));
	}
	const TiXmlNode* LastChild() const	{ return lastChild; }		/// The last child of this node. Will be null if there are no children.
	TiXmlNode* LastChild()	{ return lastChild; }
	
	const TiXmlNode* LastChild( const char * value ) const;			/// The last child of this node matching 'value'. Will be null if there are no children.
	TiXmlNode* LastChild( const char * _value ) {
		return const_cast< TiXmlNode* > ((const_cast< const TiXmlNode* >(this))->LastChild( _value ));
	}

    #ifdef TIXML_USE_STL
	const TiXmlNode* FirstChild( const std::string& _value ) const	{	return FirstChild (_value.c_str ());	}	///< STL std::string form.
	TiXmlNode* FirstChild( const std::string& _value )				{	return FirstChild (_value.c_str ());	}	///< STL std::string form.
	const TiXmlNode* LastChild( const std::string& _value ) const	{	return LastChild (_value.c_str ());	}	///< STL std::string form.
	TiXmlNode* LastChild( const std::string& _value )				{	return LastChild (_value.c_str ());	}	///< STL std::string form.
	#endif

	/** An alternate way to walk the children of a node.
		One way to iterate over nodes is:
		@verbatim
			for( child = parent->FirstChild(); child; child = child->NextSibling() )
		@endverbatim

		IterateChildren does the same thing with the syntax:
		@verbatim
			child = 0;
			while( child = parent->IterateChildren( child ) )
		@endverbatim

		IterateChildren takes the previous child as input and finds
		the next one. If the previous child is null, it returns the
		first. IterateChildren will return null when done.
	*/
	const TiXmlNode* IterateChildren( const TiXmlNode* previous ) const;
	TiXmlNode* IterateChildren( const TiXmlNode* previous ) {
		return const_cast< TiXmlNode* >( (const_cast< const TiXmlNode* >(this))->IterateChildren( previous ) );
	}

	/// This flavor of IterateChildren searches for children with a particular 'value'
	const TiXmlNode* IterateChildren( const char * value, const TiXmlNode* previous ) const;
	TiXmlNode* IterateChildren( const char * _value, const TiXmlNode* previous ) {
		return const_cast< TiXmlNode* >( (const_cast< const TiXmlNode* >(this))->IterateChildren( _value, previous ) );
	}

    #ifdef TIXML_USE_STL
	const TiXmlNode* IterateChildren( const std::string& _value, const TiXmlNode* previous ) const	{	return IterateChildren (_value.c_str (), previous);	}	///< STL std::string form.
	TiXmlNode* IterateChildren( const std::string& _value, const TiXmlNode* previous ) {	return IterateChildren (_value.c_str (), previous);	}	///< STL std::string form.
	#endif

	/** Add a new node related to this. Adds a child past the LastChild.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TiXmlNode* InsertEndChild( const TiXmlNode& addThis );


	/** Add a new node related to this. Adds a child past the LastChild.

		NOTE: the node to be added is passed by pointer, and will be
		henceforth owned (and deleted) by tinyXml. This method is efficient
		and avoids an extra copy, but should be used with care as it
		uses a different memory model than the other insert functions.

		@sa InsertEndChild
	*/
	TiXmlNode* LinkEndChild( TiXmlNode* addThis );

	/** Add a new node related to this. Adds a child before the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TiXmlNode* InsertBeforeChild( TiXmlNode* beforeThis, const TiXmlNode& addThis );

	/** Add a new node related to this. Adds a child after the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TiXmlNode* InsertAfterChild(  TiXmlNode* afterThis, const TiXmlNode& addThis );

	/** Replace a child of this node.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TiXmlNode* ReplaceChild( TiXmlNode* replaceThis, const TiXmlNode& withThis );

	/// Delete a child of this node.
	bool RemoveChild( TiXmlNode* removeThis );

	/// Navigate to a sibling node.
	const TiXmlNode* PreviousSibling() const			{ return prev; }
	TiXmlNode* PreviousSibling()						{ return prev; }

	/// Navigate to a sibling node.
	const TiXmlNode* PreviousSibling( const char * ) const;
	TiXmlNode* PreviousSibling( const char *_prev ) {
		return const_cast< TiXmlNode* >( (const_cast< const TiXmlNode* >(this))->PreviousSibling( _prev ) );
	}

    #ifdef TIXML_USE_STL
	const TiXmlNode* PreviousSibling( const std::string& _value ) const	{	return PreviousSibling (_value.c_str ());	}	///< STL std::string form.
	TiXmlNode* PreviousSibling( const std::string& _value ) 			{	return PreviousSibling (_value.c_str ());	}	///< STL std::string form.
	const TiXmlNode* NextSibling( const std::string& _value) const		{	return NextSibling (_value.c_str ());	}	///< STL std::string form.
	TiXmlNode* NextSibling( const std::string& _value) 					{	return NextSibling (_value.c_str ());	}	///< STL std::string form.
	#endif

	/// Navigate to a sibling node.
	const TiXmlNode* NextSibling() const				{ return next; }
	TiXmlNode* NextSibling()							{ return next; }

	/// Navigate to a sibling node with the given 'value'.
	const TiXmlNode* NextSibling( const char * ) const;
	TiXmlNode* NextSibling( const char* _next ) {
		return const_cast< TiXmlNode* >( (const_cast< const TiXmlNode* >(this))->NextSibling( _next ) );
	}

	/** Convenience function to get through elements.
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	const TiXmlElement* NextSiblingElement() const;
	TiXmlElement* NextSiblingElement() {
		return const_cast< TiXmlElement* >( (const_cast< const TiXmlNode* >(this))->NextSiblingElement() );
	}

	/** Convenience function to get through elements.
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	const TiXmlElement* NextSiblingElement( const char * ) const;
	TiXmlElement* NextSiblingElement( const char *_next ) {
		return const_cast< TiXmlElement* >( (const_cast< const TiXmlNode* >(this))->NextSiblingElement( _next ) );
	}

    #ifdef TIXML_USE_STL
	const TiXmlElement* NextSiblingElement( const std::string& _value) const	{	return NextSiblingElement (_value.c_str ());	}	///< STL std::string form.
	TiXmlElement* NextSiblingElement( const std::string& _value)				{	return NextSiblingElement (_value.c_str ());	}	///< STL std::string form.
	#endif

	/// Convenience function to get through elements.
	const TiXmlElement* FirstChildElement()	const;
	TiXmlElement* FirstChildElement() {
		return const_cast< TiXmlElement* >( (const_cast< const TiXmlNode* >(this))->FirstChildElement() );
	}

	/// Convenience function to get through elements.
	const TiXmlElement* FirstChildElement( const char * _value ) const;
	TiXmlElement* FirstChildElement( const char * _value ) {
		return const_cast< TiXmlElement* >( (const_cast< const TiXmlNode* >(this))->FirstChildElement( _value ) );
	}

    #ifdef TIXML_USE_STL
	const TiXmlElement* FirstChildElement( const std::string& _value ) const	{	return FirstChildElement (_value.c_str ());	}	///< STL std::string form.
	TiXmlElement* FirstChildElement( const std::string& _value )				{	return FirstChildElement (_value.c_str ());	}	///< STL std::string form.
	#endif

	/** Query the type (as an enumerated value, above) of this node.
		The possible types are: DOCUMENT, ELEMENT, COMMENT,
								UNKNOWN, TEXT, and DECLARATION.
	*/
	int Type() const	{ return type; }

	/** Return a pointer to the Document this node lives in.
		Returns null if not in a document.
	*/
	const TiXmlDocument* GetDocument() const;
	TiXmlDocument* GetDocument() {
		return const_cast< TiXmlDocument* >( (const_cast< const TiXmlNode* >(this))->GetDocument() );
	}

	/// Returns true if this node has no children.
	bool NoChildren() const						{ return !firstChild; }

	virtual const TiXmlDocument*    ToDocument()    const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TiXmlElement*     ToElement()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TiXmlComment*     ToComment()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TiXmlUnknown*     ToUnknown()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TiXmlText*        ToText()        const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TiXmlDeclaration* ToDeclaration() const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.

	virtual TiXmlDocument*          ToDocument()    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TiXmlElement*           ToElement()	    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TiXmlComment*           ToComment()     { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TiXmlUnknown*           ToUnknown()	    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TiXmlText*	            ToText()        { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TiXmlDeclaration*       ToDeclaration() { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.

	/** Create an exact duplicate of this node and return it. The memory must be deleted
		by the caller. 
	*/
	virtual TiXmlNode* Clone() const = 0;

	/** Accept a hierchical visit the nodes in the TinyXML DOM. Every node in the 
		XML tree will be conditionally visited and the host will be called back
		via the TiXmlVisitor interface.

		This is essentially a SAX interface for TinyXML. (Note however it doesn't re-parse
		the XML for the callbacks, so the performance of TinyXML is unchanged by using this
		interface versus any other.)

		The interface has been based on ideas from:

		- http://www.saxproject.org/
		- http://c2.com/cgi/wiki?HierarchicalVisitorPattern 

		Which are both good references for "visiting".

		An example of using Accept():
		@verbatim
		TiXmlPrinter printer;
		tinyxmlDoc.Accept( &printer );
		const char* xmlcstr = printer.CStr();
		@endverbatim
	*/
	virtual bool Accept( TiXmlVisitor* visitor ) const = 0;

protected:
	TiXmlNode( NodeType _type );

	// Copy to the allocated object. Shared functionality between Clone, Copy constructor,
	// and the assignment operator.
	void CopyTo( TiXmlNode* target ) const;

	#ifdef TIXML_USE_STL
	    // The real work of the input operator.
	virtual void StreamIn( std::istream* in, TIXML_STRING* tag ) = 0;
	#endif

	// Figure out what is at *p, and parse it. Returns null if it is not an xml node.
	TiXmlNode* Identify( const char* start, TiXmlEncoding encoding );

	TiXmlNode*		parent;
	NodeType		type;

	TiXmlNode*		firstChild;
	TiXmlNode*		lastChild;

	TIXML_STRING	value;

	TiXmlNode*		prev;
	TiXmlNode*		next;

private:
	TiXmlNode( const TiXmlNode& );				// not implemented.
	void operator=( const TiXmlNode& base );	// not allowed.
};


/** An attribute is a name-value pair. Elements have an arbitrary
	number of attributes, each with a unique name.

	@note The attributes are not TiXmlNodes, since they are not
		  part of the tinyXML document object model. There are other
		  suggested ways to look at this problem.
*/
class TiXmlAttribute : public TiXmlBase
{
	friend class TiXmlAttributeSet;

public:
	/// Construct an empty attribute.
	TiXmlAttribute() : TiXmlBase()
	{
		document = 0;
		prev = next = 0;
	}

	#ifdef TIXML_USE_STL
	/// std::string constructor.
	TiXmlAttribute( const std::string& _name, const std::string& _value )
	{
		name = _name;
		value = _value;
		document = 0;
		prev = next = 0;
	}
	#endif

	/// Construct an attribute with a name and value.
	TiXmlAttribute( const char * _name, const char * _value )
	{
		name = _name;
		value = _value;
		document = 0;
		prev = next = 0;
	}

	const char*		Name()  const		{ return name.c_str(); }		///< Return the name of this attribute.
	const char*		Value() const		{ return value.c_str(); }		///< Return the value of this attribute.
	#ifdef TIXML_USE_STL
	const std::string& ValueStr() const	{ return value; }				///< Return the value of this attribute.
	#endif
	int				IntValue() const;									///< Return the value of this attribute, converted to an integer.
	double			DoubleValue() const;								///< Return the value of this attribute, converted to a double.

	// Get the tinyxml string representation
	const TIXML_STRING& NameTStr() const { return name; }

	/** QueryIntValue examines the value string. It is an alternative to the
		IntValue() method with richer error checking.
		If the value is an integer, it is stored in 'value' and 
		the call returns TIXML_SUCCESS. If it is not
		an integer, it returns TIXML_WRONG_TYPE.

		A specialized but useful call. Note that for success it returns 0,
		which is the opposite of almost all other TinyXml calls.
	*/
	int QueryIntValue( int* _value ) const;
	/// QueryDoubleValue examines the value string. See QueryIntValue().
	int QueryDoubleValue( double* _value ) const;

	void SetName( const char* _name )	{ name = _name; }				///< Set the name of this attribute.
	void SetValue( const char* _value )	{ value = _value; }				///< Set the value.

	void SetIntValue( int _value );										///< Set the value from an integer.
	void SetLongValue( long _value );									///< Set the value from an long integer.
	void SetDoubleValue( double _value );								///< Set the value from a double.

    #ifdef TIXML_USE_STL
	/// STL std::string form.
	void SetName( const std::string& _name )	{ name = _name; }	
	/// STL std::string form.	
	void SetValue( const std::string& _value )	{ value = _value; }
	#endif

	/// Get the next sibling attribute in the DOM. Returns null at end.
	const TiXmlAttribute* Next() const;
	TiXmlAttribute* Next() {
		return const_cast< TiXmlAttribute* >( (const_cast< const TiXmlAttribute* >(this))->Next() ); 
	}

	/// Get the previous sibling attribute in the DOM. Returns null at beginning.
	const TiXmlAttribute* Previous() const;
	TiXmlAttribute* Previous() {
		return const_cast< TiXmlAttribute* >( (const_cast< const TiXmlAttribute* >(this))->Previous() ); 
	}

	bool operator==( const TiXmlAttribute& rhs ) const { return rhs.name == name; }
	bool operator<( const TiXmlAttribute& rhs )	 const { return name < rhs.name; }
	bool operator>( const TiXmlAttribute& rhs )  const { return name > rhs.name; }

	/*	Attribute parsing starts: first letter of the name
						 returns: the next char after the value end quote
	*/
	virtual const char* Parse( const char* p, TiXmlParsingData* data, TiXmlEncoding encoding );

	// Prints this Attribute to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const {
		Print( cfile, depth, 0 );
	}
	void Print( FILE* cfile, int depth, TIXML_STRING* str ) const;

	// [internal use]
	// Set the document pointer so the attribute can report errors.
	void SetDocument( TiXmlDocument* doc )	{ document = doc; }

private:
	TiXmlAttribute( const TiXmlAttribute& );				// not implemented.
	void operator=( const TiXmlAttribute& base );	// not allowed.

	TiXmlDocument*	document;	// A pointer back to a document, for error reporting.
	TIXML_STRING name;
	TIXML_STRING value;
	TiXmlAttribute*	prev;
	TiXmlAttribute*	next;
};


/*	A class used to manage a group of attributes.
	It is only used internally, both by the ELEMENT and the DECLARATION.
	
	The set can be changed transparent to the Element and Declaration
	classes that use it, but NOT transparent to the Attribute
	which has to implement a next() and previous() method. Which makes
	it a bit problematic and prevents the use of STL.

	This version is implemented with circular lists because:
		- I like circular lists
		- it demonstrates some independence from the (typical) doubly linked list.
*/
class TiXmlAttributeSet
{
public:
	TiXmlAttributeSet();
	~TiXmlAttributeSet();

	void Add( TiXmlAttribute* attribute );
	void Remove( TiXmlAttribute* attribute );

	const TiXmlAttribute* First()	const	{ return ( sentinel.next == &sentinel ) ? 0 : sentinel.next; }
	TiXmlAttribute* First()					{ return ( sentinel.next == &sentinel ) ? 0 : sentinel.next; }
	const TiXmlAttribute* Last() const		{ return ( sentinel.prev == &sentinel ) ? 0 : sentinel.prev; }
	TiXmlAttribute* Last()					{ return ( sentinel.prev == &sentinel ) ? 0 : sentinel.prev; }

	TiXmlAttribute*	Find( const char* _name ) const;
	TiXmlAttribute* FindOrCreate( const char* _name );

#	ifdef TIXML_USE_STL
	TiXmlAttribute*	Find( const std::string& _name ) const;
	TiXmlAttribute* FindOrCreate( const std::string& _name );
#	endif


private:
	//*ME:	Because of hidden/disabled copy-construktor in TiXmlAttribute (sentinel-element),
	//*ME:	this class must be also use a hidden/disabled copy-constructor !!!
	TiXmlAttributeSet( const TiXmlAttributeSet& );	// not allowed
	void operator=( const TiXmlAttributeSet& );	// not allowed (as TiXmlAttribute)

	TiXmlAttribute sentinel;
};


/** The element is a container class. It has a value, the element name,
	and can contain other elements, text, comments, and unknowns.
	Elements also contain an arbitrary number of attributes.
*/
class TiXmlElement : public TiXmlNode
{
public:
	/// Construct an element.
	TiXmlElement (const char * in_value);

	#ifdef TIXML_USE_STL
	/// std::string constructor.
	TiXmlElement( const std::string& _value );
	#endif

	TiXmlElement( const TiXmlElement& );

	void operator=( const TiXmlElement& base );

	virtual ~TiXmlElement();

	/** Given an attribute name, Attribute() returns the value
		for the attribute of that name, or null if none exists.
	*/
	const char* Attribute( const char* name ) const;

	/** Given an attribute name, Attribute() returns the value
		for the attribute of that name, or null if none exists.
		If the attribute exists and can be converted to an integer,
		the integer value will be put in the return 'i', if 'i'
		is non-null.
	*/
	const char* Attribute( const char* name, int* i ) const;

	/** Given an attribute name, Attribute() returns the value
		for the attribute of that name, or null if none exists.
		If the attribute exists and can be converted to an double,
		the double value will be put in the return 'd', if 'd'
		is non-null.
	*/
	const char* Attribute( const char* name, double* d ) const;

	/** QueryIntAttribute examines the attribute - it is an alternative to the
		Attribute() method with richer error checking.
		If the attribute is an integer, it is stored in 'value' and 
		the call returns TIXML_SUCCESS. If it is not
		an integer, it returns TIXML_WRONG_TYPE. If the attribute
		does not exist, then TIXML_NO_ATTRIBUTE is returned.
	*/	
	int QueryIntAttribute( const char* name, int* _value ) const;
	/// QueryDoubleAttribute examines the attribute - see QueryIntAttribute().
	int QueryDoubleAttribute( const char* name, double* _value ) const;
	/// QueryFloatAttribute examines the attribute - see QueryIntAttribute().
	int QueryFloatAttribute( const char* name, float* _value ) const {
		double d;
		int result = QueryDoubleAttribute( name, &d );
		if ( result == TIXML_SUCCESS ) {
			*_value = (float)d;
		}
		return result;
	}

    #ifdef TIXML_USE_STL
	/// QueryStringAttribute examines the attribute - see QueryIntAttribute().
	int QueryStringAttribute( const char* name, std::string* _value ) const {
		const char* cstr = Attribute( name );
		if ( cstr ) {
			*_value = std::string( cstr );
			return TIXML_SUCCESS;
		}
		return TIXML_NO_ATTRIBUTE;
	}

	/** Template form of the attribute query which will try to read the
		attribute into the specified type. Very easy, very powerful, but
		be careful to make sure to call this with the correct type.
		
		NOTE: This method doesn't work correctly for 'string' types that contain spaces.

		@return TIXML_SUCCESS, TIXML_WRONG_TYPE, or TIXML_NO_ATTRIBUTE
	*/
	template< typename T > int QueryValueAttribute( const std::string& name, T* outValue ) const
	{
		const TiXmlAttribute* node = attributeSet.Find( name );
		if ( !node )
			return TIXML_NO_ATTRIBUTE;

		std::stringstream sstream( node->ValueStr() );
		sstream >> *outValue;
		if ( !sstream.fail() )
			return TIXML_SUCCESS;
		return TIXML_WRONG_TYPE;
	}

	int QueryValueAttribute( const std::string& name, std::string* outValue ) const
	{
		const TiXmlAttribute* node = attributeSet.Find( name );
		if ( !node )
			return TIXML_NO_ATTRIBUTE;
		*outValue = node->ValueStr();
		return TIXML_SUCCESS;
	}
	#endif

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void SetAttribute( const char* name, const char * _value );

    #ifdef TIXML_USE_STL
	const std::string* Attribute( const std::string& name ) const;
	const std::string* Attribute( const std::string& name, int* i ) const;
	const std::string* Attribute( const std::string& name, double* d ) const;
	int QueryIntAttribute( const std::string& name, int* _value ) const;
	int QueryDoubleAttribute( const std::string& name, double* _value ) const;

	/// STL std::string form.
	void SetAttribute( const std::string& name, const std::string& _value );
	///< STL std::string form.
	void SetAttribute( const std::string& name, int _value );
	void SetAttribute( const std::string& name, long _value );
	///< STL std::string form.
	void SetAttribute( const std::string& name, double _value );
	///< STL std::string form.
	void SetDoubleAttribute( const std::string& name, double value );
	#endif

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void SetAttribute( const char * name, int value );
	void SetAttribute( const char * name, long val );

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void SetAttribute( const char * name, double value );

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void SetDoubleAttribute( const char * name, double value );

	/** Deletes an attribute with the given name.
	*/
	void RemoveAttribute( const char * name );
    #ifdef TIXML_USE_STL
	void RemoveAttribute( const std::string& name )	{	RemoveAttribute (name.c_str ());	}	///< STL std::string form.
	#endif

	const TiXmlAttribute* FirstAttribute() const	{ return attributeSet.First(); }		///< Access the first attribute in this element.
	TiXmlAttribute* FirstAttribute() 				{ return attributeSet.First(); }
	const TiXmlAttribute* LastAttribute()	const 	{ return attributeSet.Last(); }		///< Access the last attribute in this element.
	TiXmlAttribute* LastAttribute()					{ return attributeSet.Last(); }

	/** Convenience function for easy access to the text inside an element. Although easy
		and concise, GetText() is limited compared to getting the TiXmlText child
		and accessing it directly.
	
		If the first child of 'this' is a TiXmlText, the GetText()
		returns the character string of the Text node, else null is returned.

		This is a convenient method for getting the text of simple contained text:
		@verbatim
		<foo>This is text</foo>
		const char* str = fooElement->GetText();
		@endverbatim

		'str' will be a pointer to "This is text". 
		
		Note that this function can be misleading. If the element foo was created from
		this XML:
		@verbatim
		<foo><b>This is text</b></foo> 
		@endverbatim

		then the value of str would be null. The first child node isn't a text node, it is
		another element. From this XML:
		@verbatim
		<foo>This is <b>text</b></foo> 
		@endverbatim
		GetText() will return "This is ".

		WARNING: GetText() accesses a child node - don't become confused with the 
				 similarly named TiXmlHandle::Text() and TiXmlNode::ToText() which are 
				 safe type casts on the referenced node.
	*/
	const char* GetText() const;

	/// Creates a new Element and returns it - the returned element is a copy.
	virtual TiXmlNode* Clone() const;
	// Print the Element to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const;

	/*	Attribtue parsing starts: next char past '<'
						 returns: next char past '>'
	*/
	virtual const char* Parse( const char* p, TiXmlParsingData* data, TiXmlEncoding encoding );

	virtual const TiXmlElement*     ToElement()     const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TiXmlElement*           ToElement()	          { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool Accept( TiXmlVisitor* visitor ) const;

protected:

	void CopyTo( TiXmlElement* target ) const;
	void ClearThis();	// like clear, but initializes 'this' object as well

	// Used to be public [internal use]
	#ifdef TIXML_USE_STL
	virtual void StreamIn( std::istream * in, TIXML_STRING * tag );
	#endif
	/*	[internal use]
		Reads the "value" of the element -- another element, or text.
		This should terminate with the current end tag.
	*/
	const char* ReadValue( const char* in, TiXmlParsingData* prevData, TiXmlEncoding encoding );

private:
	TiXmlAttributeSet attributeSet;
};


/**	An XML comment.
*/
class TiXmlComment : public TiXmlNode
{
public:
	/// Constructs an empty comment.
	TiXmlComment() : TiXmlNode( TiXmlNode::TINYXML_COMMENT ) {}
	/// Construct a comment from text.
	TiXmlComment( const char* _value ) : TiXmlNode( TiXmlNode::TINYXML_COMMENT ) {
		SetValue( _value );
	}
	TiXmlComment( const TiXmlComment& );
	void operator=( const TiXmlComment& base );

	virtual ~TiXmlComment()	{}

	/// Returns a copy of this Comment.
	virtual TiXmlNode* Clone() const;
	// Write this Comment to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const;

	/*	Attribtue parsing starts: at the ! of the !--
						 returns: next char past '>'
	*/
	virtual const char* Parse( const char* p, TiXmlParsingData* data, TiXmlEncoding encoding );

	virtual const TiXmlComment*  ToComment() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TiXmlComment*  ToComment() { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool Accept( TiXmlVisitor* visitor ) const;

protected:
	void CopyTo( TiXmlComment* target ) const;

	// used to be public
	#ifdef TIXML_USE_STL
	virtual void StreamIn( std::istream * in, TIXML_STRING * tag );
	#endif
//	virtual void StreamOut( TIXML_OSTREAM * out ) const;

private:

};


/** XML text. A text node can have 2 ways to output the next. "normal" output 
	and CDATA. It will default to the mode it was parsed from the XML file and
	you generally want to leave it alone, but you can change the output mode with 
	SetCDATA() and query it with CDATA().
*/
class TiXmlText : public TiXmlNode
{
	friend class TiXmlElement;
public:
	/** Constructor for text element. By default, it is treated as 
		normal, encoded text. If you want it be output as a CDATA text
		element, set the parameter _cdata to 'true'
	*/
	TiXmlText (const char * initValue ) : TiXmlNode (TiXmlNode::TINYXML_TEXT)
	{
		SetValue( initValue );
		cdata = false;
	}
	virtual ~TiXmlText() {}

	#ifdef TIXML_USE_STL
	/// Constructor.
	TiXmlText( const std::string& initValue ) : TiXmlNode (TiXmlNode::TINYXML_TEXT)
	{
		SetValue( initValue );
		cdata = false;
	}
	#endif

	TiXmlText( const TiXmlText& copy ) : TiXmlNode( TiXmlNode::TINYXML_TEXT )	{ copy.CopyTo( this ); }
	void operator=( const TiXmlText& base )							 	{ base.CopyTo( this ); }

	// Write this text object to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const;

	/// Queries whether this represents text using a CDATA section.
	bool CDATA() const				{ return cdata; }
	/// Turns on or off a CDATA representation of text.
	void SetCDATA( bool _cdata )	{ cdata = _cdata; }

	virtual const char* Parse( const char* p, TiXmlParsingData* data, TiXmlEncoding encoding );

	virtual const TiXmlText* ToText() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TiXmlText*       ToText()       { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool Accept( TiXmlVisitor* content ) const;

protected :
	///  [internal use] Creates a new Element and returns it.
	virtual TiXmlNode* Clone() const;
	void CopyTo( TiXmlText* target ) const;

	bool Blank() const;	// returns true if all white space and new lines
	// [internal use]
	#ifdef TIXML_USE_STL
	virtual void StreamIn( std::istream * in, TIXML_STRING * tag );
	#endif

private:
	bool cdata;			// true if this should be input and output as a CDATA style text element
};


/** In correct XML the declaration is the first entry in the file.
	@verbatim
		<?xml version="1.0" standalone="yes"?>
	@endverbatim

	TinyXml will happily read or write files without a declaration,
	however. There are 3 possible attributes to the declaration:
	version, encoding, and standalone.

	Note: In this version of the code, the attributes are
	handled as special cases, not generic attributes, simply
	because there can only be at most 3 and they are always the same.
*/
class TiXmlDeclaration : public TiXmlNode
{
public:
	/// Construct an empty declaration.
	TiXmlDeclaration()   : TiXmlNode( TiXmlNode::TINYXML_DECLARATION ) {}

#ifdef TIXML_USE_STL
	/// Constructor.
	TiXmlDeclaration(	const std::string& _version,
						const std::string& _encoding,
						const std::string& _standalone );
#endif

	/// Construct.
	TiXmlDeclaration(	const char* _version,
						const char* _encoding,
						const char* _standalone );

	TiXmlDeclaration( const TiXmlDeclaration& copy );
	void operator=( const TiXmlDeclaration& copy );

	virtual ~TiXmlDeclaration()	{}

	/// Version. Will return an empty string if none was found.
	const char *Version() const			{ return version.c_str (); }
	/// Encoding. Will return an empty string if none was found.
	const char *Encoding() const		{ return encoding.c_str (); }
	/// Is this a standalone document?
	const char *Standalone() const		{ return standalone.c_str (); }

	/// Creates a copy of this Declaration and returns it.
	virtual TiXmlNode* Clone() const;
	// Print this declaration to a FILE stream.
	virtual void Print( FILE* cfile, int depth, TIXML_STRING* str ) const;
	virtual void Print( FILE* cfile, int depth ) const {
		Print( cfile, depth, 0 );
	}

	virtual const char* Parse( const char* p, TiXmlParsingData* data, TiXmlEncoding encoding );

	virtual const TiXmlDeclaration* ToDeclaration() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TiXmlDeclaration*       ToDeclaration()       { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool Accept( TiXmlVisitor* visitor ) const;

protected:
	void CopyTo( TiXmlDeclaration* target ) const;
	// used to be public
	#ifdef TIXML_USE_STL
	virtual void StreamIn( std::istream * in, TIXML_STRING * tag );
	#endif

private:

	TIXML_STRING version;
	TIXML_STRING encoding;
	TIXML_STRING standalone;
};


/** Any tag that tinyXml doesn't recognize is saved as an
	unknown. It is a tag of text, but should not be modified.
	It will be written back to the XML, unchanged, when the file
	is saved.

	DTD tags get thrown into TiXmlUnknowns.
*/
class TiXmlUnknown : public TiXmlNode
{
public:
	TiXmlUnknown() : TiXmlNode( TiXmlNode::TINYXML_UNKNOWN )	{}
	virtual ~TiXmlUnknown() {}

	TiXmlUnknown( const TiXmlUnknown& copy ) : TiXmlNode( TiXmlNode::TINYXML_UNKNOWN )		{ copy.CopyTo( this ); }
	void operator=( const TiXmlUnknown& copy )										{ copy.CopyTo( this ); }

	/// Creates a copy of this Unknown and returns it.
	virtual TiXmlNode* Clone() const;
	// Print this Unknown to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const;

	virtual const char* Parse( const char* p, TiXmlParsingData* data, TiXmlEncoding encoding );

	virtual const TiXmlUnknown*     ToUnknown()     const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TiXmlUnknown*           ToUnknown()	    { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool Accept( TiXmlVisitor* content ) const;

protected:
	void CopyTo( TiXmlUnknown* target ) const;

	#ifdef TIXML_USE_STL
	virtual void StreamIn( std::istream * in, TIXML_STRING * tag );
	#endif

private:

};


/** Always the top level node. A document binds together all the
	XML pieces. It can be saved, loaded, and printed to the screen.
	The 'value' of a document node is the xml file name.
*/
class TiXmlDocument : public TiXmlNode
{
public:
	/// Create an empty document, that has no name.
	TiXmlDocument();
	/// Create a document with a name. The name of the document is also the filename of the xml.
	TiXmlDocument( const char * documentName );

	#ifdef TIXML_USE_STL
	/// Constructor.
	TiXmlDocument( const std::string& documentName );
	#endif

	TiXmlDocument( const TiXmlDocument& copy );
	void operator=( const TiXmlDocument& copy );

	virtual ~TiXmlDocument() {}

	/** Load a file using the current document value.
		Returns true if successful. Will delete any existing
		document data before loading.
	*/
	bool LoadFile( TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING );
	/// Save a file using the current document value. Returns true if successful.
	bool SaveFileAppend() const;
	bool SaveFile() const;
	/// Load a file using the given filename. Returns true if successful.
	bool LoadFile( const char * filename, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING );
	/// Save a file using the given filename. Returns true if successful.
	bool SaveFile( const char * filename ) const;
	/** Load a file using the given FILE*. Returns true if successful. Note that this method
		doesn't stream - the entire object pointed at by the FILE*
		will be interpreted as an XML file. TinyXML doesn't stream in XML from the current
		file location. Streaming may be added in the future.
	*/
	bool LoadFile( FILE*, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING );
	/// Save a file using the given FILE*. Returns true if successful.
	bool SaveFile( FILE* ) const;

	#ifdef TIXML_USE_STL
	bool LoadFile( const std::string& filename, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING )			///< STL std::string version.
	{
		return LoadFile( filename.c_str(), encoding );
	}
	bool SaveFile( const std::string& filename ) const		///< STL std::string version.
	{
		return SaveFile( filename.c_str() );
	}
	#endif

	/** Parse the given null terminated block of xml data. Passing in an encoding to this
		method (either TIXML_ENCODING_LEGACY or TIXML_ENCODING_UTF8 will force TinyXml
		to use that encoding, regardless of what TinyXml might otherwise try to detect.
	*/
	virtual const char* Parse( const char* p, TiXmlParsingData* data = 0, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING );

	/** Get the root element -- the only top level element -- of the document.
		In well formed XML, there should only be one. TinyXml is tolerant of
		multiple elements at the document level.
	*/
	const TiXmlElement* RootElement() const		{ return FirstChildElement(); }
	TiXmlElement* RootElement()					{ return FirstChildElement(); }

	/** If an error occurs, Error will be set to true. Also,
		- The ErrorId() will contain the integer identifier of the error (not generally useful)
		- The ErrorDesc() method will return the name of the error. (very useful)
		- The ErrorRow() and ErrorCol() will return the location of the error (if known)
	*/	
	bool Error() const						{ return error; }

	/// Contains a textual (english) description of the error if one occurs.
	const char * ErrorDesc() const	{ return errorDesc.c_str (); }

	/** Generally, you probably want the error string ( ErrorDesc() ). But if you
		prefer the ErrorId, this function will fetch it.
	*/
	int ErrorId()	const				{ return errorId; }

	/** Returns the location (if known) of the error. The first column is column 1, 
		and the first row is row 1. A value of 0 means the row and column wasn't applicable
		(memory errors, for example, have no row/column) or the parser lost the error. (An
		error in the error reporting, in that case.)

		@sa SetTabSize, Row, Column
	*/
	int ErrorRow() const	{ return errorLocation.row+1; }
	int ErrorCol() const	{ return errorLocation.col+1; }	///< The column where the error occured. See ErrorRow()

	/** SetTabSize() allows the error reporting functions (ErrorRow() and ErrorCol())
		to report the correct values for row and column. It does not change the output
		or input in any way.
		
		By calling this method, with a tab size
		greater than 0, the row and column of each node and attribute is stored
		when the file is loaded. Very useful for tracking the DOM back in to
		the source file.

		The tab size is required for calculating the location of nodes. If not
		set, the default of 4 is used. The tabsize is set per document. Setting
		the tabsize to 0 disables row/column tracking.

		Note that row and column tracking is not supported when using operator>>.

		The tab size needs to be enabled before the parse or load. Correct usage:
		@verbatim
		TiXmlDocument doc;
		doc.SetTabSize( 8 );
		doc.Load( "myfile.xml" );
		@endverbatim

		@sa Row, Column
	*/
	void SetTabSize( int _tabsize )		{ tabsize = _tabsize; }

	int TabSize() const	{ return tabsize; }

	/** If you have handled the error, it can be reset with this call. The error
		state is automatically cleared if you Parse a new XML block.
	*/
	void ClearError()						{	error = false; 
												errorId = 0; 
												errorDesc = ""; 
												errorLocation.row = errorLocation.col = 0; 
												//errorLocation.last = 0; 
											}

	/** Write the document to standard out using formatted printing ("pretty print"). */
	void Print() const						{ Print( stdout, 0 ); }

	/* Write the document to a string using formatted printing ("pretty print"). This
		will allocate a character array (new char[]) and return it as a pointer. The
		calling code pust call delete[] on the return char* to avoid a memory leak.
	*/
	//char* PrintToMemory() const; 

	/// Print this Document to a FILE stream.
	virtual void Print( FILE* cfile, int depth = 0 ) const;
	// [internal use]
	void SetError( int err, const char* errorLocation, TiXmlParsingData* prevData, TiXmlEncoding encoding );

	virtual const TiXmlDocument*    ToDocument()    const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TiXmlDocument*          ToDocument()          { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool Accept( TiXmlVisitor* content ) const;

protected :
	// [internal use]
	virtual TiXmlNode* Clone() const;
	#ifdef TIXML_USE_STL
	virtual void StreamIn( std::istream * in, TIXML_STRING * tag );
	#endif

private:
	void CopyTo( TiXmlDocument* target ) const;

	bool error;
	int  errorId;
	TIXML_STRING errorDesc;
	int tabsize;
	TiXmlCursor errorLocation;
	bool useMicrosoftBOM;		// the UTF-8 BOM were found when read. Note this, and try to write.
};


/**
	A TiXmlHandle is a class that wraps a node pointer with null checks; this is
	an incredibly useful thing. Note that TiXmlHandle is not part of the TinyXml
	DOM structure. It is a separate utility class.

	Take an example:
	@verbatim
	<Document>
		<Element attributeA = "valueA">
			<Child attributeB = "value1" />
			<Child attributeB = "value2" />
		</Element>
	<Document>
	@endverbatim

	Assuming you want the value of "attributeB" in the 2nd "Child" element, it's very 
	easy to write a *lot* of code that looks like:

	@verbatim
	TiXmlElement* root = document.FirstChildElement( "Document" );
	if ( root )
	{
		TiXmlElement* element = root->FirstChildElement( "Element" );
		if ( element )
		{
			TiXmlElement* child = element->FirstChildElement( "Child" );
			if ( child )
			{
				TiXmlElement* child2 = child->NextSiblingElement( "Child" );
				if ( child2 )
				{
					// Finally do something useful.
	@endverbatim

	And that doesn't even cover "else" cases. TiXmlHandle addresses the verbosity
	of such code. A TiXmlHandle checks for null	pointers so it is perfectly safe 
	and correct to use:

	@verbatim
	TiXmlHandle docHandle( &document );
	TiXmlElement* child2 = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", 1 ).ToElement();
	if ( child2 )
	{
		// do something useful
	@endverbatim

	Which is MUCH more concise and useful.

	It is also safe to copy handles - internally they are nothing more than node pointers.
	@verbatim
	TiXmlHandle handleCopy = handle;
	@endverbatim

	What they should not be used for is iteration:

	@verbatim
	int i=0; 
	while ( true )
	{
		TiXmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", i ).ToElement();
		if ( !child )
			break;
		// do something
		++i;
	}
	@endverbatim

	It seems reasonable, but it is in fact two embedded while loops. The Child method is 
	a linear walk to find the element, so this code would iterate much more than it needs 
	to. Instead, prefer:

	@verbatim
	TiXmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).FirstChild( "Child" ).ToElement();

	for( child; child; child=child->NextSiblingElement() )
	{
		// do something
	}
	@endverbatim
*/
class TiXmlHandle
{
public:
	/// Create a handle from any node (at any depth of the tree.) This can be a null pointer.
	TiXmlHandle( TiXmlNode* _node )					{ this->node = _node; }
	/// Copy constructor
	TiXmlHandle( const TiXmlHandle& ref )			{ this->node = ref.node; }
	TiXmlHandle operator=( const TiXmlHandle& ref ) { this->node = ref.node; return *this; }

	/// Return a handle to the first child node.
	TiXmlHandle FirstChild() const;
	/// Return a handle to the first child node with the given name.
	TiXmlHandle FirstChild( const char * value ) const;
	/// Return a handle to the first child element.
	TiXmlHandle FirstChildElement() const;
	/// Return a handle to the first child element with the given name.
	TiXmlHandle FirstChildElement( const char * value ) const;

	/** Return a handle to the "index" child with the given name. 
		The first child is 0, the second 1, etc.
	*/
	TiXmlHandle Child( const char* value, int index ) const;
	/** Return a handle to the "index" child. 
		The first child is 0, the second 1, etc.
	*/
	TiXmlHandle Child( int index ) const;
	/** Return a handle to the "index" child element with the given name. 
		The first child element is 0, the second 1, etc. Note that only TiXmlElements
		are indexed: other types are not counted.
	*/
	TiXmlHandle ChildElement( const char* value, int index ) const;
	/** Return a handle to the "index" child element. 
		The first child element is 0, the second 1, etc. Note that only TiXmlElements
		are indexed: other types are not counted.
	*/
	TiXmlHandle ChildElement( int index ) const;

	#ifdef TIXML_USE_STL
	TiXmlHandle FirstChild( const std::string& _value ) const				{ return FirstChild( _value.c_str() ); }
	TiXmlHandle FirstChildElement( const std::string& _value ) const		{ return FirstChildElement( _value.c_str() ); }

	TiXmlHandle Child( const std::string& _value, int index ) const			{ return Child( _value.c_str(), index ); }
	TiXmlHandle ChildElement( const std::string& _value, int index ) const	{ return ChildElement( _value.c_str(), index ); }
	#endif

	/** Return the handle as a TiXmlNode. This may return null.
	*/
	TiXmlNode* ToNode() const			{ return node; } 
	/** Return the handle as a TiXmlElement. This may return null.
	*/
	TiXmlElement* ToElement() const		{ return ( ( node && node->ToElement() ) ? node->ToElement() : 0 ); }
	/**	Return the handle as a TiXmlText. This may return null.
	*/
	TiXmlText* ToText() const			{ return ( ( node && node->ToText() ) ? node->ToText() : 0 ); }
	/** Return the handle as a TiXmlUnknown. This may return null.
	*/
	TiXmlUnknown* ToUnknown() const		{ return ( ( node && node->ToUnknown() ) ? node->ToUnknown() : 0 ); }

	/** @deprecated use ToNode. 
		Return the handle as a TiXmlNode. This may return null.
	*/
	TiXmlNode* Node() const			{ return ToNode(); } 
	/** @deprecated use ToElement. 
		Return the handle as a TiXmlElement. This may return null.
	*/
	TiXmlElement* Element() const	{ return ToElement(); }
	/**	@deprecated use ToText()
		Return the handle as a TiXmlText. This may return null.
	*/
	TiXmlText* Text() const			{ return ToText(); }
	/** @deprecated use ToUnknown()
		Return the handle as a TiXmlUnknown. This may return null.
	*/
	TiXmlUnknown* Unknown() const	{ return ToUnknown(); }

private:
	TiXmlNode* node;
};


/** Print to memory functionality. The TiXmlPrinter is useful when you need to:

	-# Print to memory (especially in non-STL mode)
	-# Control formatting (line endings, etc.)

	When constructed, the TiXmlPrinter is in its default "pretty printing" mode.
	Before calling Accept() you can call methods to control the printing
	of the XML document. After TiXmlNode::Accept() is called, the printed document can
	be accessed via the CStr(), Str(), and Size() methods.

	TiXmlPrinter uses the Visitor API.
	@verbatim
	TiXmlPrinter printer;
	printer.SetIndent( "\t" );

	doc.Accept( &printer );
	fprintf( stdout, "%s", printer.CStr() );
	@endverbatim
*/
class TiXmlPrinter : public TiXmlVisitor
{
public:
	TiXmlPrinter() : depth( 0 ), simpleTextPrint( false ),
					 buffer(), indent( "    " ), lineBreak( "\n" ) {}

	virtual bool VisitEnter( const TiXmlDocument& doc );
	virtual bool VisitExit( const TiXmlDocument& doc );

	virtual bool VisitEnter( const TiXmlElement& element, const TiXmlAttribute* firstAttribute );
	virtual bool VisitExit( const TiXmlElement& element );

	virtual bool Visit( const TiXmlDeclaration& declaration );
	virtual bool Visit( const TiXmlText& text );
	virtual bool Visit( const TiXmlComment& comment );
	virtual bool Visit( const TiXmlUnknown& unknown );

	/** Set the indent characters for printing. By default 4 spaces
		but tab (\t) is also useful, or null/empty string for no indentation.
	*/
	void SetIndent( const char* _indent )			{ indent = _indent ? _indent : "" ; }
	/// Query the indention string.
	const char* Indent()							{ return indent.c_str(); }
	/** Set the line breaking string. By default set to newline (\n). 
		Some operating systems prefer other characters, or can be
		set to the null/empty string for no indenation.
	*/
	void SetLineBreak( const char* _lineBreak )		{ lineBreak = _lineBreak ? _lineBreak : ""; }
	/// Query the current line breaking string.
	const char* LineBreak()							{ return lineBreak.c_str(); }

	/** Switch over to "stream printing" which is the most dense formatting without 
		linebreaks. Common when the XML is needed for network transmission.
	*/
	void SetStreamPrinting()						{ indent = "";
													  lineBreak = "";
													}	
	/// Return the result.
	const char* CStr()								{ return buffer.c_str(); }
	/// Return the length of the result string.
	size_t Size()									{ return buffer.size(); }

	#ifdef TIXML_USE_STL
	/// Return the result.
	const std::string& Str()						{ return buffer; }
	#endif

private:
	void DoIndent()	{
		for( int i=0; i<depth; ++i )
			buffer += indent;
	}
	void DoLineBreak() {
		buffer += lineBreak;
	}

	int depth;
	bool simpleTextPrint;
	TIXML_STRING buffer;
	TIXML_STRING indent;
	TIXML_STRING lineBreak;
};


//////////////////////////////////////////////////////////////////////////
//
// Thread.h: header file
//
// Copyright (C) Walter E. Capers.  All rights reserved
//
// This source is free to use as you like.  If you make
// any changes please keep me in the loop.  Email them to
// walt.capers@comcast.net.
//
// PURPOSE:
//
//  To implement Win32 threading as a C++ object
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
// Date: 10.31.07
// Name: Walter E. Capers
// Description: Added support for beginthreadex
//              To use compile with -DUSE_BEGIN_THREAD
//
//

// #ifndef THREAD_CLASS
// #define THREAD_CLASS

class CMutexClass
{
private:
#ifdef _WINDOWS32
    HANDLE m_mutex;
#else
    pthread_mutex_t m_mutex;
#endif
    ThreadId_t m_owner;
    public:
        BOOL m_bCreated;
        void Lock();
        void Unlock();
        CMutexClass(void);
        ~CMutexClass(void);
};

class CEventClass
{
private:
    ThreadId_t m_owner;
#ifdef _WINDOWS32
    HANDLE m_event;
#else
    pthread_cond_t m_ready;
    pthread_mutex_t m_lock;
#endif
public:
    BOOL m_bCreated;
    void Set();
    BOOL Wait();
    void Reset();
    CEventClass(void);
    ~CEventClass(void);
};

class CTask
{
private:
	TaskStatus_t m_state;
	ThreadId_t m_dwThread;
public:
	CMutexClass m_mutex;

	void SetTaskStatus(TaskStatus_t state) 
	{
		m_mutex.Lock();
			m_state=state;
		m_mutex.Unlock();
	}

	void SetId(ThreadId_t *pid)
	{
		memcpy(&m_dwThread,pid,sizeof(ThreadId_t));
	}

	/**
	 *
	 * Wait
	 * waits for upto timeoutSeconds for a task
	 * to complete
	 *
	 **/
	BOOL Wait(int timeoutSeconds)
	{
        timeoutSeconds = timeoutSeconds * 1000;
		if( Status() != TaskStatusCompleted &&
			timeoutSeconds > 0 )
		{
			Sleep(100);
			timeoutSeconds = timeoutSeconds - 100;
		}
		if( Status() == TaskStatusCompleted ) return TRUE;
		return FALSE;
	}

	/**
	 *
	 * Status
	 * returns current state of a task
	 *
	 **/
	TaskStatus_t Status()
	{
		TaskStatus_t state ;

		m_mutex.Lock();
		  state = m_state;
		m_mutex.Unlock();
	    return state;
	}

	void Thread(ThreadId_t *pId)
	{
		memcpy(pId,&m_dwThread,sizeof(ThreadId_t));
	}

	CTask(){m_state=TaskStatusNotSubmitted; memset(&m_dwThread,0,sizeof(ThreadId_t)); }
	virtual ~CTask(){}
	virtual BOOL Task()=0;
};


class CThread 
// #ifdef _WINDOWS32
// 	: public CObject // use CObject as a base class so object can be used in lists and
// 	               // object arrays
// #endif
{
private:
	CEventClass   m_event;         // event controller
	int           m_StopTimeout;   // specifies a timeout value for stop
	                               // if a thread fails to stop within m_StopTimeout
	                               // seconds an exception is thrown

	BOOL		  m_bRunning;      // set to TRUE if thread is running
#ifdef _WINDOWS32
	HANDLE		  m_thread;		   // thread handle
#else
	pthread_t     m_thread;        // thread handle
#endif
	ThreadId_t	  m_dwId;          // id of this thread
	LPVOID        *m_lppvQueue;    // task que
	unsigned int  m_chQueue;       // que depth
	unsigned int  m_queuePos;      // current que possition
	LPVOID        m_lpvProcessor;  // data which is currently being processed
	ThreadState_t m_state;         // current state of thread see thread state data
	                               // structure.
	DWORD         m_dwIdle;        // used for Sleep periods
	ThreadType_t  m_type;
	DWORD		  m_stackSize;     // thread stack size
#define NO_ERRORS			       0
#define MUTEX_CREATION		       0x01
#define EVENT_CREATION		       0x02
#define THREAD_CREATION		       0x04
#define UNKNOWN					   0x08
#define ILLEGAL_USE_OF_EVENT       0x10
#define MEMORY_FAULT               0x20
#define EVENT_AND_TYPE_DONT_MATCH  0x40
#define STACK_OVERFLOW             0x80
#define STACK_EMPTY                0x100
#define STACK_FULL                 0x200

	DWORD         m_dwObjectCondition;
	BOOL		  Push(LPVOID lpv);
	BOOL		  Pop();
	BOOL		  Empty();
public:
	/**
	 *
	 * user definable member functions
	 *
	 **/
	CMutexClass	  m_mutex;         // mutex that protects threads internal data

	virtual BOOL OnTask(LPVOID lpvData);     // called when an event occurs
	virtual BOOL OnTask();                   // called when a time interval has elapsed

	CThread(void);
	virtual ~CThread(void);
#ifdef _WINDOWS32
#ifdef USE_BEGIN_THREAD
	friend unsigned __stdcall _THKERNEL(LPVOID lpvData);
#else
	friend DWORD WINAPI _THKERNEL( LPVOID lpvData );
#endif
#else
	friend LPVOID _THKERNEL(LPVOID lpvData);
#endif
	BOOL        FromSameThread();
	float		PercentCapacity();
	void        WaitTillExit();
	BOOL		KernelProcess();
	BOOL		Event(LPVOID lpvData=NULL);
	BOOL        Event(CTask *pvTask);
	void		SetOnStopTimeout(int seconds ) { m_StopTimeout = seconds; }
    BOOL        SetQueueSize( unsigned int ch );
	BOOL		Stop();
	BOOL		Start();
	BOOL        KillThread();
	void		GetId(ThreadId_t *pId) { memcpy(pId,&m_dwId,sizeof(ThreadId_t)); }      // returns thread id
	ThreadId_t  GetId();
	ThreadState_t ThreadState();
	BOOL		PingThread(DWORD dwTimeout=0);
	BOOL        AtCapacity();
#ifdef _WINDOWS32
	void		SetPriority(DWORD dwPriority=THREAD_PRIORITY_NORMAL);
#else
	void		SetPriority(DWORD dwPriority=0);
#endif
	DWORD		GetErrorFlags() { return m_dwObjectCondition; } // returns state of object
	void		SetThreadType(ThreadType_t typ=ThreadTypeNotDefined,DWORD dwIdle=100);
	void		SetIdle(DWORD dwIdle=100);
    unsigned int GetEventsPending();
    static BOOL ThreadIdsEqual(ThreadId_t *p1,
						       ThreadId_t *p2)
	{
#if defined(AS400)||defined(OS400)
		return(( memcmp(p1,p2,sizeof(ThreadId_t))==0)?TRUE:FALSE);
#elif defined(VMS) 
		return (( pthread_equal(*p1,*p2) )?TRUE:FALSE );
#else
		return ((*p1 == *p2)?TRUE:FALSE);
#endif

	}

	static ThreadId_t ThreadId()
	{
		ThreadId_t thisThreadsId ;
#if defined(AS400) || defined(OS400)
		pthread_t thread;
#endif

#ifdef _WINDOWS32
		thisThreadsId = (ThreadId_t)GetCurrentThreadId();
#else

#if defined(AS400) || defined(OS400)
		thread = pthread_self();
		pthread_getunique_np(&thread,&thisThreadsId);
//#elif defined(ALPHA) || defined(DEC) || defined(VMS)
#elif defined(ALPHA) || defined(VMS)
#ifdef VMS
		thisThreadsId = pthread_self();
#else
		thisThreadsId = pthread_getsequence_np(pthread_self());
#endif
#else
		thisThreadsId = pthread_self();
#endif
#endif
		return thisThreadsId;
	}


};
//#endif



//////////////////////////////////////////////////////////////////////////
class CPubFun : public CPubTime, public CPubString, public CPubFile, public CPubLogFile, public CThread
{
public:
	CPubFun();
	virtual ~CPubFun();
};

#endif // !defined(AFX_PUBFUN_H__0E62F986_5E3A_4882_A324_1EAA3D43EEDD__INCLUDED_)
