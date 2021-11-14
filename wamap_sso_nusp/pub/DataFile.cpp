//#include "StdAfx.h" // comment out this line if not using pre-compiled header

#include "DataFile.h"

// supress warnings (restored at end)
#ifdef _WINDOWS32
#pragma warning(push)

#pragma warning(disable:4786) // 255 char truncation
#pragma warning(disable:4702) // unreachable code in std::vector
#endif

// using declarations
using namespace DF;
using std::exception;
using std::find_if;
using std::for_each;
using std::ifstream;
using std::istream;
using std::ostream;
using std::string;
using std::vector;

// local functions and function objects

// Counts the number of columns in an istream denoted by delim.
static int CountCols(istream& is, const char& delim)
{
	int nCols = 1;
	do
	{
		if (is.peek() == delim)
			nCols++;
	} while (is.get() != '\n');

	return nCols;
}

// Counts the number of columns in an istream denoted by delim.
static int CountColsData(istream& is, const char& delim,const int nflag)
{
	int nCols;
	if (nflag == 1)
	{
		// 行末尾有分隔符
		nCols = 0;
	}
	else
	{
		// 行末尾无分隔符
		nCols = 1;
	}
	// 读取第一行
	do
	{
		if (is.peek() == delim)
			nCols++;
	} while (is.get() != '\n');

	return nCols;
}

// Counts the number of columns in an istream denoted by delim.
// static int CountColsDataNew(istream& is, const char& delim)
// {
// 	// 行末尾有分隔符
// 	int nCols = 0;
// 	// 读取第一行
// 	do
// 	{
// 		if (is.peek() == delim)
// 			nCols++;
// 	} while (is.get() != '\n');
// 	
// 	// 读取第二行
// 	nCols = 0;
// 	do
// 	{
// 		if (is.peek() == delim)
// 			nCols++;
// 	} while (is.get() != '\n');
// 	
// 	return nCols;
// }

// A function object that adds things to a vector.
class AddTo
{
public:
	AddTo(vector<vector<string> >& rTarget) : pvvStrTarget(&rTarget) {}
	AddTo(vector<vector<double> >& rTarget) : pvvTarget(&rTarget)    {}
	AddTo(vector<string>& rTarget)          : pvStrTarget(&rTarget)  {}
	AddTo(vector<double>& rTarget)          : pvTarget(&rTarget)     {}

	void operator()(const vector<string>& rSource)
		{ pvvStrTarget->push_back(rSource); }

	void operator()(const vector<double>& rSource)
		{ pvvTarget->push_back(rSource);    }

	void operator()(const string& rSource)
		{ pvStrTarget->push_back(rSource);  }

	void operator()(const double& rSource)
		{ pvTarget->push_back(rSource);     }

private:
	vector<vector<string> >* pvvStrTarget;
	vector<vector<double> >* pvvTarget;
	vector<string>*          pvStrTarget;
	vector<double>*          pvTarget;
};

// A function object designed to be used as a predicate
// for remove_if(). Returns true if names match, false if 
// they don't.
class ByName:
	public std::unary_function<std::string, bool>
{	
public:
	ByName(const char* szName) : m_szName(szName) {}

	bool operator()(const std::string& szVarName) const
	{
		return (szVarName == m_szName);
	}

private: 
	std::string m_szName;
};

// end local stuff

// The default constructor.  Instantiates an instance of CDataFile and 
// initialize data members to default values.
CDataFile::CDataFile(void)
{
	m_delim       = DEFAULT_DELIMITER;
	m_dwReadFlags = RF_READ_AS_DOUBLE;
	m_reserve     = DEFAULT_RESERVE;
	m_szError     = "";
	ClearDataHead(m_DataHead);
}

// Copy constructor.  Instantiates an instance of CDataFile with the
// contents of another CDataFile.
CDataFile::CDataFile(const CDataFile& df)
{
	*this = df;
}

// Quasi-Copy constructor.  Instantiates an instance of CDataFile with the
// contents of another CDataFile as defined by the DF_SELECTION.
CDataFile::CDataFile(const DF::DF_SELECTION& dfs)
{
	m_delim       = DEFAULT_DELIMITER;
	m_dwReadFlags = RF_READ_AS_DOUBLE;
	m_reserve     = DEFAULT_RESERVE;
	m_szError     = "";
	ClearDataHead(m_DataHead);
	
	*this = dfs;
}

// Misc. constructor.  Instantiates an instance of CDataFile with the
// specified read flags.
CDataFile::CDataFile(const int& dwReadFlags)
{
	m_delim       = DEFAULT_DELIMITER;
	m_dwReadFlags = dwReadFlags;
	m_reserve     = DEFAULT_RESERVE;
	m_szError     = "";
	ClearDataHead(m_DataHead);
}
	
// Misc. constructor.  Instantiates an instance of CDataFile and reads the
// specified file with the specified read flags.
CDataFile::CDataFile(const char* szFilename, const int& dwReadFlags /* = DF::RF_READ_AS_DOUBLE */)
{
	m_delim       = DEFAULT_DELIMITER;
	m_dwReadFlags = dwReadFlags;
	m_reserve     = DEFAULT_RESERVE;
	m_szError     = "";
	ClearDataHead(m_DataHead);
	
	this->ReadFile(szFilename);
}

// Returns a CDataFile with one variable created with the specified name and data.
// Assumes readflag DF::RF_READ_AS_DOUBLE.
CDataFile CDataFile::FromVector(const char* szVariableName, const std::vector<double>& vData)
{
	CDataFile retDF;

	retDF.m_vstrVariableNames.push_back(szVariableName);
	retDF.m_vstrSourceFilenames.push_back(retDF.m_szFilename);
	
	retDF.m_v2dData.push_back(vector<double>());
	retDF.m_v2dData.back().reserve(retDF.m_reserve);

	for_each
	(
		vData.begin(),
		vData.end(),
		AddTo(retDF.m_v2dData)
	);

	return retDF;
}

// sets one CDataFile equal to another CDataFile.
CDataFile& CDataFile::operator=(const CDataFile& df)
{
	m_delim               = df.m_delim;
	m_dwReadFlags         = df.m_dwReadFlags;
	m_reserve             = df.m_reserve;
	m_szFilename          = df.m_szFilename;
	m_szError             = df.m_szError;
    m_vstrVariableNames   = df.m_vstrVariableNames;
	m_vstrSourceFilenames = df.m_vstrSourceFilenames;
	m_v2dStrData          = df.m_v2dStrData;
	m_v2dData             = df.m_v2dData;

	return *this;
}

// sets one CDataFile equal to a selection of another CDataFile.
CDataFile& CDataFile::operator=(const DF_SELECTION& dfs)
{
	// clear data vectors
	vector<string>().swap(m_vstrVariableNames);
	vector<string>().swap(m_vstrSourceFilenames);
	vector<vector<double> >().swap(m_v2dData);
	vector<vector<string> >().swap(m_v2dStrData);

	// set the read flags
	m_dwReadFlags = dfs.pDataFile->m_dwReadFlags;

	// normalize the selection
	int firstVar  = dfs.iVar[0]  < dfs.iVar[1]  ? dfs.iVar[0]  : dfs.iVar[1];
	int lastVar   = dfs.iVar[1]  > dfs.iVar[0]  ? dfs.iVar[1]  : dfs.iVar[0];
	int firstSamp = dfs.iSamp[0] < dfs.iSamp[1] ? dfs.iSamp[0] : dfs.iSamp[1];
	int lastSamp  = dfs.iSamp[1] > dfs.iSamp[0] ? dfs.iSamp[1] : dfs.iSamp[0];

	// push_back new variable names
	int iVar;
	for(iVar=firstVar; iVar<=lastVar; iVar++)
	{
		m_vstrVariableNames.push_back(dfs.pDataFile->m_vstrVariableNames.at(iVar).c_str());
		m_vstrSourceFilenames.push_back(dfs.pDataFile->m_vstrSourceFilenames.at(iVar).c_str());

		if(dfs.pDataFile->m_dwReadFlags & RF_READ_AS_DOUBLE)
			m_v2dData.push_back(vector<double>());

		else if(dfs.pDataFile->m_dwReadFlags & RF_READ_AS_STRING)
			m_v2dStrData.push_back(vector<string>());
	}

	// fill up the data
	for(int iSamp = firstSamp; iSamp<=lastSamp; iSamp++)
	{
		for(iVar = firstVar; iVar<=lastVar; iVar++)
		{
			if(dfs.pDataFile->m_dwReadFlags & RF_READ_AS_DOUBLE)
			{
				try	
				{ 
					m_v2dData.at(iVar-firstVar).push_back(dfs.pDataFile->m_v2dData.at(iVar).at(iSamp)); 
				}
				catch(...) {}
			}

			else if(dfs.pDataFile->m_dwReadFlags & RF_READ_AS_STRING)
			{
				try	
				{	
					m_v2dStrData.at(iVar-firstVar).push_back(dfs.pDataFile->m_v2dStrData.at(iVar).at(iSamp).c_str()); 
				}
				catch(...) {}
			}
		}
	}

	return *this;
}

// adds one CDataFile to another CDataFile
CDataFile& CDataFile::operator+=(const CDataFile& df)
{
	for_each
	(
		df.m_vstrVariableNames.begin(),
		df.m_vstrVariableNames.end(),
		AddTo(m_vstrVariableNames)
	);

	for_each
	(
		df.m_vstrSourceFilenames.begin(),
		df.m_vstrSourceFilenames.end(),
		AddTo(m_vstrSourceFilenames)
	);

	for_each
	(
		df.m_v2dData.begin(),
		df.m_v2dData.end(),
		AddTo(m_v2dData)
	);

	for_each
	(
		df.m_v2dStrData.begin(),
		df.m_v2dStrData.end(),
		AddTo(m_v2dStrData)
	);

	return *this;
}

// adds CDataFile(s) together
CDataFile CDataFile::operator+ (const CDataFile& df) const
{
	return CDataFile(*this) += df;
}

// returns a DF_SELECTION based on the coordinates passed.
DF_SELECTION CDataFile::operator()(const int& left, const int& top, const int& right, const int& bottom)
{
	DF_SELECTION dfs;

	dfs.iSamp[0]  = top;
	dfs.iSamp[1]  = bottom;
	dfs.iVar[0]   = left;
	dfs.iVar[1]   = right;
	dfs.pDataFile = this;

	return dfs;
}

// Returns a copy of the value specified by the given coordinates.
double CDataFile::operator()(const int& iVariable, const int& iSample)
{
    return GetData(iVariable,iSample);
}

int CDataFile::operator()(const int& iVariable, const int& iSample, char* lpStr)
{
	int retVal = 0;

	if(m_dwReadFlags & RF_READ_AS_STRING)
	{
		try
		{
			strcpy(lpStr,m_v2dStrData.at(iVariable).at(iSample).c_str());
			retVal = static_cast<int>(m_v2dStrData.at(iVariable).at(iSample).length());
		}

		catch(const exception& e) 
		{	
			m_szError = e.what(); 
			retVal = -1;
		}
		
		catch(...) 
		{ 
			m_szError = ERRORCODES[1]; 
			retVal = -1;
		}
	}

	else
	{
		m_szError = ERRORCODES[3];
		retVal = -1;
	}

	return retVal;
}


// Creates a CDataFile in STRING mode from a vector with a supplied variable name.
CDataFile CDataFile::FromVector(const char* szVariableName, const std::vector<string>& vData)
{
	CDataFile retDF(RF_READ_AS_STRING);

	retDF.m_vstrVariableNames.push_back(szVariableName);
	retDF.m_vstrSourceFilenames.push_back(retDF.m_szFilename);
	
	retDF.m_v2dStrData.push_back(vector<string>());
	retDF.m_v2dStrData.back().reserve(retDF.m_reserve);

	for_each
	(
		vData.begin(),
		vData.end(),
		AddTo(retDF.m_v2dStrData)
	);

	return retDF;
}

// destructor
CDataFile::~CDataFile(void)
{
	this->ClearData();
}

// Reads the specified file.
// Returns true if successful, false if an error occurred.
bool CDataFile::ReadFile(const char* szFilename, const unsigned& dwReadFlags)
{
	try
	{
		m_szFilename  = szFilename;
		m_dwReadFlags = dwReadFlags;

		ifstream inFile;
		inFile.open(szFilename);

		if(inFile.rdstate() & std::ios::failbit)
		{
			inFile.close();
			m_szError = ERRORCODES[7]; 
			m_szError += "\nDetails: ";
			m_szError += szFilename;
			return false;
		}

		inFile >> *this;
						
		inFile.close();
		return true;
	}

	catch(const exception& e)
	{
		m_szError = e.what();
	}

	catch(...)
	{
		m_szError = ERRORCODES[0];
	}

	return false;
}

// Reads the specified file.
// Returns true if successful, false if an error occurred.
// bool CDataFile::ReadFileData(const char* szFilename,int &posStart,const int timeleng)
// {
// 	try
// 	{
// 		m_szFilename  = szFilename;
// 		// m_dwReadFlags = dwReadFlags;
// 		if (!m_vstrSourceFilenames.empty())
// 		{
// 			m_vstrSourceFilenames.clear();
// 		}
// 		if (!m_vstrVariableNames.empty())
// 		{
// 			m_vstrVariableNames.clear();
// 		}
// 		if (!m_v2dData.empty())
// 		{
// 			m_v2dData.clear();
// 		}
// 		if (!m_v2dStrData.empty())
// 		{
// 			m_v2dStrData.clear();
// 		}
// 
// 		ifstream inFile;
// 		inFile.open(szFilename);
// 		
// 		if(inFile.rdstate() & std::ios::failbit)
// 		{
// 			inFile.close();
// 			m_szError = ERRORCODES[7]; 
// 			m_szError += "\nDetails: ";
// 			m_szError += szFilename;
// 			return false;
// 		}
// 		
// 		if(!(m_dwReadFlags & RF_APPEND_DATA))
// 			ClearData();
// 		
// 		// 获取案例参数
// 		std::fstream::pos_type pos = GetDataHead(inFile,m_DataHead);
// 
// 		// count the number of variables
// 		int nVars = CountColsData(inFile, m_delim.at(0));
// 		
// 		// make sure data columns are equal to nVars
// 		// in order to prevent mis-aligned data
// 		inFile.ignore(MAX_LINE_BUFFER,'\n');
// 		
// 		if(CountColsData(inFile, m_delim.at(0)) != nVars)
// 		{
// 			m_szError = ERRORCODES[8];
// 			//throw exception(m_szError.c_str());
// 			throw m_szError.c_str();
// 		}
// 		
// 		// rewind the file
// 		// 定位到数据行
// 		inFile.seekg(pos);
// 		
// 		int iVar = 0;
// 		int varOffset = GetNumberOfVariables();
// 		
// 		char buff[MAX_FIELD_BUFFER] = {0};
// 		
// 		for(iVar=0; iVar<nVars; iVar++)
// 		{
// 			inFile.getline(buff, sizeof buff, (iVar == nVars-1) ? '\n' : m_delim.at(0));	
// 			m_vstrVariableNames.push_back(buff);
// 			m_vstrSourceFilenames.push_back(m_szFilename);
// 			
// 			if(m_dwReadFlags & RF_READ_AS_DOUBLE)
// 			{
// 				m_v2dData.push_back(vector<double>());
// 				m_v2dData.back().reserve(m_reserve);
// 			}
// 			
// 			else if(m_dwReadFlags & RF_READ_AS_STRING)
// 			{
// 				m_v2dStrData.push_back(vector<string>());
// 				m_v2dStrData.back().reserve(m_reserve);
// 			}
// 		}
// 		
// 		// 定位文件指针
// 		if (posStart > 0)
// 		{
// 			inFile.seekg(posStart);
// 		}
// 
// 		int tmpRowNum =0,RowNum =0;
// 		if (m_DataHead.time_sample>0)
// 		{
// 			RowNum = int(timeleng * m_DataHead.time_sample);
// 		}
// 		else
// 		{
// 			m_DataHead.time_sample = 25.0f;
// 			RowNum = int(timeleng * m_DataHead.time_sample);
// 		}
// 		
// 		while(!inFile.eof())
// 		{
// 			for(iVar=0; iVar<nVars; iVar++)
// 			{
// 				inFile.getline(buff, sizeof(buff), (iVar == nVars-1) ? '\n' : m_delim.at(0));	
// 				
// 				// make sure we didn't pick up extra junk @ eof.
// 				if(/*!inFile.eof() &&*/ buff[0] != '\n')
// 				{
// 					if(m_dwReadFlags & RF_READ_AS_DOUBLE)
// 						m_v2dData.at(iVar+varOffset).push_back(atof(buff));
// 					
// 					else if(m_dwReadFlags & RF_READ_AS_STRING)
// 						m_v2dStrData.at(iVar+varOffset).push_back(buff);
// 				}
// 			}
// 			tmpRowNum ++;
// 			if (RowNum>0)
// 			{
// 				// 读取数据满足个数
// 				if (tmpRowNum >= RowNum)
// 				{
// 					break;
// 				}
// 			}
// 			
// 		}
// 		
// 		// 保存文件指针
// 		posStart = inFile.tellg();
// 		
// 		if(m_vstrVariableNames.back().find("\n")!=-1)
// 			m_vstrVariableNames.back().resize(m_vstrVariableNames.back().length()-1);
// 		
// 		inFile.close();
// 		if (tmpRowNum<RowNum)
// 		{
// 			// 未读取指定行数数据
// 			return false;
// 		}
// 		return true;
// 	}
// 
// // 	catch(const char * error)
// // 	{
// // 		m_szError = error;
// // 	}
// 
// 	catch(const exception& e)
// 	{
// 		m_szError = e.what();
// 	}
// 	
// 	catch(...)
// 	{
// 		m_szError = ERRORCODES[0];
// 	}
// 	
// 	return false;
// }

bool CDataFile::ReadFileData(const char* szFilename,int &posStart,const int timeleng, const int initime)
{
	try
	{
		if (timeleng<initime)
		{
			return false;
		}

		m_szFilename  = szFilename;
		// m_dwReadFlags = dwReadFlags;
		ClearData();

		ifstream inFile;
		inFile.open(szFilename);
		
		if(inFile.rdstate() & std::ios::failbit)
		{
			inFile.close();
			m_szError = ERRORCODES[7]; 
			m_szError += "\nDetails: ";
			m_szError += szFilename;
			return false;
		}
		
		if(!(m_dwReadFlags & RF_APPEND_DATA))
			ClearData();
		
		// 获取案例参数
		std::fstream::pos_type pos = GetDataHeadNew(inFile,m_DataHead);
		int nVars;
		if (m_DataHead.is_old_file == 1)
		{
			// 旧数据格式,定位到首行
			inFile.seekg(0);
			pos = GetDataHead(inFile,m_DataHead);
			// count the number of variables
			nVars = CountColsData(inFile, m_delim.at(0),m_DataHead.is_delim_end);
			
			// make sure data columns are equal to nVars
			// in order to prevent mis-aligned data
			inFile.ignore(MAX_LINE_BUFFER,'\n');
			
			if(CountColsData(inFile, m_delim.at(0),m_DataHead.is_delim_end) != nVars)
			{
				m_szError = ERRORCODES[8];
				//throw exception(m_szError.c_str());
				throw m_szError.c_str();
			}
		}
		else
		{
			// 新数据格式
			// count the number of variables
			nVars = CountColsData(inFile, m_delim.at(0),m_DataHead.is_delim_end);
			
			// make sure data columns are equal to nVars
			// in order to prevent mis-aligned data
			inFile.ignore(MAX_LINE_BUFFER,'\n');
			
			if(CountColsData(inFile, m_delim.at(0),m_DataHead.is_delim_end) != nVars)
			{
				m_szError = ERRORCODES[8];
				//throw exception(m_szError.c_str());
				throw m_szError.c_str();
			}
		}

		// rewind the file
		// 定位到数据行
		// if (pos >= 0)
		{
			inFile.seekg(pos);
		}

		int iVar = 0;
		int ifbreak = 0;
		int varOffset = GetNumberOfVariables();
		
		char buff[MAX_FIELD_BUFFER] = {0};
		std::string tmpstring;
		for(iVar=0; iVar<nVars; iVar++)
		{
			if(m_DataHead.is_delim_end == 1)
			{
				// 行末尾有分隔符
				inFile.getline(buff, sizeof buff, m_delim.at(0));
			}
			else
			{
				// 行末尾无分隔符
				inFile.getline(buff, sizeof buff, (iVar == nVars-1) ? '\n' : m_delim.at(0));
			}
			tmpstring = buff;
			DF_StringTrimAll(tmpstring);
			m_vstrVariableNames.push_back(tmpstring);
			m_vstrSourceFilenames.push_back(m_szFilename);
			
			if(m_dwReadFlags & RF_READ_AS_DOUBLE)
			{
				m_v2dData.push_back(vector<double>());
				m_v2dData.back().reserve(m_reserve);
			}
			else if(m_dwReadFlags & RF_READ_AS_STRING)
			{
				m_v2dStrData.push_back(vector<string>());
				m_v2dStrData.back().reserve(m_reserve);
			}
		}
		
		// 定位文件指针
		if (posStart > 0)
		{
			inFile.seekg(posStart);
		}

		// 根据窗口时间计算读取数据行数
		int tmpRowNum =0,nRowNum =0,nRowPos =0;
		if (m_DataHead.time_sample>0)
		{
			nRowNum = int(timeleng * m_DataHead.time_sample);
		}
		else
		{
			m_DataHead.time_sample = 25.0f;
			nRowNum = int(timeleng * m_DataHead.time_sample);
		}
		// 根据交叠窗口时间计算下轮数据指针行数
		nRowPos = int((timeleng - initime) * m_DataHead.time_sample);

		while(!inFile.eof())
		{
			ifbreak = 0;
			for(iVar=0; iVar<nVars; iVar++)
			{
				if(m_DataHead.is_delim_end == 1)
				{
					// 行末尾有分隔符
					inFile.getline(buff, sizeof(buff), m_delim.at(0));
				}
				else
				{
					// 行末尾无分隔符
					inFile.getline(buff, sizeof(buff), (iVar == nVars-1) ? '\n' : m_delim.at(0));	
				}

				tmpstring = buff;
				DF_StringTrimAll(tmpstring);

				string temp_str="附加信息";//检测到“附加信息”，程序结束读文件
				if (!strcmp(temp_str.c_str(),tmpstring.c_str()))
				{
					ifbreak = 1;
				}
				// make sure we didn't pick up extra junk @ eof.
				if(/*!inFile.eof() &&*/ buff[0] != '\n')
				{
					if(m_dwReadFlags & RF_READ_AS_DOUBLE)
						m_v2dData.at(iVar+varOffset).push_back(atof(tmpstring.c_str()));
					
					else if(m_dwReadFlags & RF_READ_AS_STRING)
						m_v2dStrData.at(iVar+varOffset).push_back(tmpstring.c_str());
				}
			}
			tmpRowNum ++;
			// 保存下轮文件指针
			if (nRowPos>=0)
			{
				if (tmpRowNum == nRowPos)
				{
					posStart = inFile.tellg();
				}
			}
			if (nRowNum>=0)
			{
				// 读取数据满足个数
				if (tmpRowNum >= nRowNum || ifbreak == 1)
				{
					break;
				}
			}
		}
		
		if(m_vstrVariableNames.back().find("\n")!= string::npos)
		{
			m_vstrVariableNames.back().resize(m_vstrVariableNames.back().length()-1);
		}
		
		inFile.close();
		if (tmpRowNum<nRowPos)
		{
			// 文件指针未更新
			return false;
		}

		if (tmpRowNum<nRowNum)
		{
			// 未读取指定行数数据
			return false;
		}
		return true;
	}

// 	catch(const char * error)
// 	{
// 		m_szError = error;
// 	}

	catch(const exception& e)
	{
		m_szError = e.what();
	}
	
	catch(...)
	{
		m_szError = ERRORCODES[0];
	}
	
	return false;
}

int CDataFile::GetDataHead( istream &is, DATA_FILE_DEV_INFO &tmpDataHead )
{
	ClearDataHead(m_DataHead);
	
	// 首行
	string tmpstring,strtimestart,strtimeend,strtimesample;
	std::getline(is,tmpstring);
	// 判断行末尾是否有分隔符
	std::string::size_type idx;
	idx = tmpstring.find_last_of(m_delim.at(0));
	DF_StringTrimAll(tmpstring);
	if (idx+1 == tmpstring.length())
	{
		// 行末尾有分隔符
		tmpDataHead.is_delim_end = 1;
	}
	else
	{
		// 行末尾无分隔符
		tmpDataHead.is_delim_end = 0;
	}

	idx = tmpstring.find_first_of(m_delim.at(0));
	strtimestart = tmpstring.substr(0,idx);
	tmpstring.erase(0,idx+1);
	idx = tmpstring.find_first_of(m_delim.at(0));
	strtimeend = tmpstring.substr(0,idx);
	tmpstring.erase(0,idx+1);
	idx = tmpstring.find_first_of(m_delim.at(0));
	if (idx == string::npos)
	{
		tmpDataHead.time_sample = (float)atof(tmpstring.c_str());
	}
	else
	{
		tmpDataHead.time_sample = (float)atof(tmpstring.substr(0,idx).c_str());
	}
	tmpDataHead.start_time = GetSecondByTimeString(strtimestart.c_str());
	tmpDataHead.end_time = GetSecondByTimeString(strtimeend.c_str());

	// 第二行
	std::getline(is,tmpstring);
	idx = tmpstring.find_first_of(m_delim.at(0));
	tmpDataHead.dev_num = atoi(tmpstring.substr(0,idx).c_str());
	for (int ii=0; ii<tmpDataHead.dev_num; ii++)
	{
		DF_StringTrimAll(tmpstring);
		tmpstring.erase(0,idx+1);
		idx = tmpstring.find_first_of(m_delim.at(0));
		tmpDataHead.vec_devid_s.push_back(tmpstring.substr(0,idx));
	}

	tmpDataHead.is_old_file = 1;

	std::fstream::pos_type pos = is.tellg();
	return pos;
}

int CDataFile::GetDataHeadNew( std::istream &is, DATA_FILE_DEV_INFO &tmpDataHead )
{
	ClearDataHead(tmpDataHead);
	
	// 第一行
	std::string tmpstring,strtimestart,strtimeend,strtimesample,strtype,temp_str;
	std::string::size_type idx;	
	//判断第一行否有“类型”并忽略
	temp_str="类型";
	std::getline(is,tmpstring);
	DF_StringTrimAll(tmpstring);
	idx = tmpstring.find_first_of(",");
	strtype = tmpstring.substr(0,idx);
	if (!strcmp(temp_str.c_str(),strtype.c_str()))
	{
		//忽略第一行
		tmpstring.clear();	
		std::getline(is,tmpstring);
		DF_StringTrimAll(tmpstring);
	}
	// 判断行末尾是否有分隔符
	idx = tmpstring.find_last_of(m_delim.at(0));
	if (idx+1 == tmpstring.length())
	{
		// 行末尾有分隔符
		tmpDataHead.is_delim_end = 1;
	}
	else
	{
		// 行末尾无分隔符
		tmpDataHead.is_delim_end = 0;
	}

	idx = tmpstring.find_first_of(m_delim.at(0));
	strtimestart = tmpstring.substr(0,idx);
	tmpstring.erase(0,idx+1);
	idx = tmpstring.find_first_of(m_delim.at(0));
	strtimeend = tmpstring.substr(0,idx);
	tmpstring.erase(0,idx+1);
	tmpDataHead.start_time = GetSecondByTimeString(strtimestart.c_str());
	tmpDataHead.end_time = GetSecondByTimeString(strtimeend.c_str());
	
	// 设备ID，设备名称为列名
	tmpDataHead.dev_num = 0;
	while(!tmpstring.empty())
	{
		DF_StringTrimAll(tmpstring);
		idx = tmpstring.find_first_of(m_delim.at(0));
		if (idx == std::string::npos)
		{
			if (tmpDataHead.is_delim_end == 0)
			{
				// 末尾无分隔符,添加最后一个设备
				tmpDataHead.vec_devid_s.push_back(tmpstring);
			}
			break;
		}
		tmpDataHead.vec_devid_s.push_back(tmpstring.substr(0,idx));
		tmpstring.erase(0,idx +1);
	}

	// 设备个数
	tmpDataHead.dev_num = tmpDataHead.vec_devid_s.size();
	if (!tmpDataHead.vec_devid_s.empty())
	{
		long tmpid;
#ifdef _D5000
		tmpid = atol(tmpDataHead.vec_devid_s.front().c_str());
#else
		tmpid = atoi(tmpDataHead.vec_devid_s.front().c_str());
#endif
		if (tmpid < 1000000)
		{
			// 非设备ID，旧数据格式
			tmpDataHead.is_old_file = 1;
			return 0;
		}
		else
		{
			// 新数据格式
			tmpDataHead.is_old_file = 0;
		}
	}
	else
	{
		// 设备为空，旧数据格式
		tmpDataHead.is_old_file = 1;
		return 0;
	}
	// 记录数据指针
		std::fstream::pos_type pos = is.tellg();
	// 第三行
	//	tmpstring.clear();
//	std::getline(is,tmpstring);
//**************************************************
	//判断一行否有“电压等级”并获取设备电压基值
	temp_str.clear();
	temp_str="电压等级";
	std::getline(is,tmpstring);
	DF_StringTrimAll(tmpstring);
	idx = tmpstring.find_first_of(",");
	strtype = tmpstring.substr(0,idx);
	if (!strcmp(temp_str.c_str(),strtype.c_str()))
	{
		tmpstring.erase(0,idx+1);
		idx = tmpstring.find_first_of(",");
		tmpstring.erase(0,idx+1);		
		while(!tmpstring.empty())
		{
			DF_StringTrimAll(tmpstring);
			idx = tmpstring.find_first_of(m_delim.at(0));
			if (idx == std::string::npos)
			{
				if (tmpDataHead.is_delim_end == 0)
				{
					// 末尾无分隔符,添加最后一个设备
					tmpDataHead.vec_devvolt_s.push_back(tmpstring);
				}
				break;
			}
			tmpDataHead.vec_devvolt_s.push_back(tmpstring.substr(0,idx));
			tmpstring.erase(0,idx +1);
		}
		// 记录数据指针
		pos = is.tellg();
		tmpstring.clear();	
		std::getline(is,tmpstring);
		DF_StringTrimAll(tmpstring);
	}

//**************************************************
	
	// 第四行
//	tmpstring.clear();
	std::getline(is,tmpstring);
	// 日期时间
	idx = tmpstring.find_first_of(m_delim.at(0));
	// 将起始时间转换为世纪秒
	char tmpTime[20];
	sprintf(tmpTime,"%s",tmpstring.substr(0,idx).c_str());
	tmpDataHead.gps_time = GetSecondByTimeString(tmpTime);
	tmpstring.erase(0,idx+1);
	// 毫秒
	idx = tmpstring.find_first_of(m_delim.at(0));
	float tmpStartTime = (float)atof(tmpstring.substr(0,idx).c_str())/1000.0f;

	// 第四行
	std::getline(is,tmpstring);
	// 日期时间
	idx = tmpstring.find_first_of(m_delim.at(0));
	tmpstring.erase(0,idx+1);
	// 毫秒
	idx = tmpstring.find_first_of(m_delim.at(0));
	float tmpEndTime = (float)atof(tmpstring.substr(0,idx).c_str())/1000.0f;

	// 数据时间间隔
	tmpDataHead.time_sample = 1.0f/(tmpEndTime - tmpStartTime);

	return pos;
}

int CDataFile::GetSecondByTimeString(const char * timestring)
{
	struct tm  sTime;

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
    
	time_t     timeN =mktime(&sTime);

    return timeN;
}

// reads the data from the stream and returns the stream when done.
istream& operator>>(istream& inFile, CDataFile& df)
{
	try
	{	
		if(!(df.m_dwReadFlags & RF_APPEND_DATA))
			df.ClearData();

		// count the number of variables
		int nVars = CountCols(inFile, df.m_delim.at(0));
			
		// make sure data columns are equal to nVars
		// in order to prevent mis-aligned data
		inFile.ignore(MAX_LINE_BUFFER,'\n');
			
		if(CountCols(inFile, df.m_delim.at(0)) != nVars)
		{
			df.m_szError = ERRORCODES[8];
			//throw exception(df.m_szError.c_str());
			throw df.m_szError.c_str();
		}

		// rewind the file
		inFile.seekg(0);
				
		int iVar = 0;
		int varOffset = df.GetNumberOfVariables();
	
		char buff[MAX_FIELD_BUFFER] = {0};

		for(iVar=0; iVar<nVars; iVar++)
		{
			inFile.getline(buff, sizeof buff, (iVar == nVars-1) ? '\n' : df.m_delim.at(0));	
			df.m_vstrVariableNames.push_back(buff);
			df.m_vstrSourceFilenames.push_back(df.m_szFilename);

			if(df.m_dwReadFlags & RF_READ_AS_DOUBLE)
			{
				df.m_v2dData.push_back(vector<double>());
				df.m_v2dData.back().reserve(df.m_reserve);
			}

			else if(df.m_dwReadFlags & RF_READ_AS_STRING)
			{
				df.m_v2dStrData.push_back(vector<string>());
				df.m_v2dStrData.back().reserve(df.m_reserve);
			}
		}
					
		do
		{
			for(iVar=0; iVar<nVars; iVar++)
			{
				inFile.getline(buff, sizeof(buff), (iVar == nVars-1) ? '\n' : df.m_delim.at(0));	

				// make sure we didn't pick up extra junk @ eof.
				if(/*!inFile.eof() &&*/ buff[0] != '\n')
				{
					if(df.m_dwReadFlags & RF_READ_AS_DOUBLE)
						df.m_v2dData.at(iVar+varOffset).push_back(atof(buff));

					else if(df.m_dwReadFlags & RF_READ_AS_STRING)
						df.m_v2dStrData.at(iVar+varOffset).push_back(buff);
				}
			}	

		}while(!inFile.eof());

		if(df.m_vstrVariableNames.back().find("\n")!= string::npos)
		{
			df.m_vstrVariableNames.back().resize(df.m_vstrVariableNames.back().length()-1);
		}
	}

	catch(const exception& e) 
	{ 
		df.m_szError = e.what();
		throw e; 
	}
	
	catch(...) 
	{ 
		df.m_szError = ERRORCODES[0];
		//throw exception(df.m_szError.c_str());
		throw df.m_szError.c_str();
	}

	return inFile;
}

ostream& operator << (ostream& outFile, const CDataFile& df)
{
	try
	{
		int nVars = static_cast<int>(df.m_vstrVariableNames.size());
		int iVar  = 0;

		for(iVar = 0; iVar<nVars; iVar++)
			outFile << df.m_vstrVariableNames.at(iVar).c_str() << ((iVar==nVars-1) ? "\n" : df.m_delim.c_str());

		int nSamps = df.GetLargestVectorSize_();

		for(int iSamp = 0; iSamp<nSamps; iSamp++)
		{
			for(iVar = 0; iVar<nVars; iVar++)
			{
				if(df.m_dwReadFlags & RF_READ_AS_DOUBLE)
				{
					try	{ outFile << df.m_v2dData.at(iVar).at(iSamp); }
					catch(...) {}
				}

				else if(df.m_dwReadFlags & RF_READ_AS_STRING)
				{
					try	{ outFile << df.m_v2dStrData.at(iVar).at(iSamp).c_str(); }
					catch(...) {}
				}

				outFile << ((iVar==nVars-1) ? "\n" : df.m_delim.c_str());
			}
		}
	}

	// catch and re-throw any exceptions
	catch(const exception& e) { throw e; }
	//catch(...) { throw exception(ERRORCODES[0]); }
	catch(...) { throw ERRORCODES[0]; }

	return outFile;
}

std::ostream& operator << (std::ostream& os, const DF::DF_SELECTION& dfs)
{
	try
	{
		int firstVar  = dfs.iVar[0]  < dfs.iVar[1]  ? dfs.iVar[0]  : dfs.iVar[1];
		int lastVar   = dfs.iVar[1]  > dfs.iVar[0]  ? dfs.iVar[1]  : dfs.iVar[0];
		int firstSamp = dfs.iSamp[0] < dfs.iSamp[1] ? dfs.iSamp[0] : dfs.iSamp[1];
		int lastSamp  = dfs.iSamp[1] > dfs.iSamp[0] ? dfs.iSamp[1] : dfs.iSamp[0];
		int iVar;
		for(iVar=firstVar; iVar<=lastVar; iVar++)
			os << dfs.pDataFile->m_vstrVariableNames.at(iVar).c_str() << ((iVar==lastVar) ? "\n" : dfs.pDataFile->m_delim.c_str());

		for(int iSamp = firstSamp; iSamp<=lastSamp; iSamp++)
		{
			for(iVar = firstVar; iVar<=lastVar; iVar++)
			{
				if(dfs.pDataFile->m_dwReadFlags & RF_READ_AS_DOUBLE)
				{
					try	{ os << dfs.pDataFile->m_v2dData.at(iVar).at(iSamp); }
					catch(...) {}
				}

				else if(dfs.pDataFile->m_dwReadFlags & RF_READ_AS_STRING)
				{
					try	{ os << dfs.pDataFile->m_v2dStrData.at(iVar).at(iSamp).c_str(); }
					catch(...) {}
				}

				os << ((iVar==lastVar) ? "\n" : dfs.pDataFile->m_delim.c_str());
			}
		}
	}

	// catch and re-throw any exceptions
	catch(const exception& e) { throw e; }
	//catch(...) { throw exception(ERRORCODES[0]); }
	catch(...) { throw ERRORCODES[0]; }

	return os;
}

bool CDataFile::SetData(const int&  iVariable, const int& iSample, const double& value)
{
	try
	{
		m_v2dData.at(iVariable).at(iSample) = value;
		return true;
	}

	catch(const exception& e) { m_szError = e.what(); }
	catch(...) { m_szError = ERRORCODES[0]; }
	return false;
}

bool CDataFile::SetData(const int&  iVariable, const int& iSample, const char* szValue)
{
	try
	{
		m_v2dStrData.at(iVariable).at(iSample) = szValue;
		return true;
	}

	catch(const exception& e) { m_szError = e.what(); }
	catch(...) { m_szError = ERRORCODES[0]; }
	return false;
}

bool CDataFile::AppendData(const int& iVariable, const double& value)
{
	try
	{
		m_v2dData.at(iVariable).push_back(value);
		return true;
	}

	catch(const exception& e) { m_szError = e.what(); }
	catch(...) { m_szError = ERRORCODES[0]; }
	return false;
}

bool CDataFile::AppendData(const int& iVariable, const char* szValue)
{
	try
	{
		m_v2dStrData.at(iVariable).push_back(szValue);
		return true;
	}

	catch(const exception& e) { m_szError = e.what(); }
	catch(...) { m_szError = ERRORCODES[0]; }
	return false;
}

bool CDataFile::AppendData(const int& iVariable, const std::vector<double>& vData)
{
	try
	{
		for_each
		(
			vData.begin(),
			vData.end(),
			AddTo(m_v2dData.at(iVariable))
		);

		return true;
	}

	catch(const exception& e) { m_szError = e.what(); }
	catch(...) { m_szError = ERRORCODES[0]; }
	return false;
}

bool CDataFile::AppendData(const int& iVariable, const std::vector<std::string>& vszData)
{
	try
	{
		for_each
		(
			vszData.begin(),
			vszData.end(),
			AddTo(m_v2dStrData.at(iVariable))
		);

		return true;
	}

	catch(const exception& e) { m_szError = e.what(); }
	catch(...) { m_szError = ERRORCODES[0]; }
	return false;
}

// Attempts to create a variable with the name specified by szName with a size of 
// iSize and an initial_value. Returns true if successful, false if an error is encountered.
bool CDataFile::CreateVariable(const char* szName, const double& initial_value, const int& iSize)
{
	try
	{
		m_vstrSourceFilenames.push_back("");
		m_vstrVariableNames.push_back(szName);
		m_v2dData.push_back(vector<double>(iSize, initial_value));
		return true;
	}

	catch(const exception& e) { m_szError = e.what(); }
	catch(...) { m_szError = ERRORCODES[0]; }
	return false;
}

// Attempts to create a variable with the name specified by szName with a size of 
// iSize and an initial_value. Returns true if successful, false if an error is encountered.
bool CDataFile::CreateVariable(const char* szName, const std::string& initial_value, const int& iSize)
{
	try
	{
		m_vstrSourceFilenames.push_back("");
		m_vstrVariableNames.push_back(szName);
		m_v2dStrData.push_back(vector<string>(iSize, initial_value));
		return true;
	}

	catch(const exception& e) { m_szError = e.what(); }
	catch(...) { m_szError = ERRORCODES[0]; }
	return false;
}

// Attempts to create a variable with the name specified by szName and the values
// contained in vData. Returns true if successful, false if an error is encountered.
bool CDataFile::CreateVariable(const char* szName, const std::vector<double>& vData)
{
	try
	{
		m_vstrSourceFilenames.push_back("");
		m_vstrVariableNames.push_back(szName);
		m_v2dData.push_back(vData);
		return true;
	}

	catch(const exception& e) { m_szError = e.what(); }
	catch(...) { m_szError = ERRORCODES[0]; }
	return false;
}

// Attempts to create a variable with the name specified by szName and the values
// contained in vszData. Returns true if successful, false if an error is encountered.
bool CDataFile::CreateVariable(const char* szName, const std::vector<std::string>& vszData)
{
	try
	{
		m_vstrSourceFilenames.push_back("");
		m_vstrVariableNames.push_back(szName);
		m_v2dStrData.push_back(vszData);
		return true;
	}

	catch(const exception& e) { m_szError = e.what(); }
	catch(...) { m_szError = ERRORCODES[0]; }
	return false;
}

// Deletes the variable whose index is iVariable from a CDataFile.
// Returns true if successful, false if an error is encountered.
bool CDataFile::DeleteVariable(const int& iVariable)
{
	try
	{
		// DeleteVariable() uses the erase/remove paradigm to delete
		// the specified variable from the internal vectors.  See my
		// article on std::vector (http://www.codeproject.com/vcpp/stl/std_vector.asp)
		// for more information on how this code works.

		// remove the variable from m_vstrSourceFilenames
		m_vstrSourceFilenames.erase
		(
			std::remove(m_vstrSourceFilenames.begin(),
						m_vstrSourceFilenames.end(),
						m_vstrSourceFilenames.at(iVariable)),
			m_vstrSourceFilenames.end()
		);

		// remove the variable from m_vstrVariableNames
		m_vstrVariableNames.erase
		(
			std::remove(m_vstrVariableNames.begin(),
						m_vstrVariableNames.end(),
						m_vstrVariableNames.at(iVariable)),
			m_vstrVariableNames.end()
		);

		if(m_dwReadFlags & RF_READ_AS_DOUBLE)
		{
			// remove the variable from m_v2dData
			m_v2dData.erase
			(
				std::remove(m_v2dData.begin(), 
							m_v2dData.end(), 
							m_v2dData.at(iVariable)),
				m_v2dData.end()
			);
		}

		else
		{
			// remove the variable from m_v2dStrData
			m_v2dStrData.erase
			(
				std::remove(m_v2dStrData.begin(), 
							m_v2dStrData.end(), 
							m_v2dStrData.at(iVariable)),
				m_v2dStrData.end()
			);
		}

		return true;
	}

	catch(const exception& e) { m_szError = e.what(); }
	catch(...) { m_szError = ERRORCODES[0]; }
	return false;
}

bool CDataFile::WriteFile(const char* szFilename, const char* szDelim)
{
	try
	{
		SetDelimiter(szDelim);

		std::ofstream of(szFilename);
		of << *this;
		of.close();
		return true;
	}

	catch(const exception& e) { m_szError = e.what(); }
	catch(...) { m_szError = ERRORCODES[0]; }
	return false;
}

bool CDataFile::WriteFileData(const char* szFilename, const char* szDelim)
{
	try
	{
		SetDelimiter(szDelim);
		
		std::ofstream of(szFilename);
		of << GetTimeStringBySeconds(m_DataHead.start_time) << m_delim << GetTimeStringBySeconds(m_DataHead.end_time) << m_delim << m_DataHead.time_sample << std::endl;
		of << *this;
		of.close();
		return true;
	}
	
	catch(const exception& e) { m_szError = e.what(); }
	catch(...) { m_szError = ERRORCODES[0]; }
	return false;
}


int CDataFile::GetVariableName(const int& iVariable, char* lpStr)
{
	try
	{
		strcpy(lpStr, m_vstrVariableNames.at(iVariable).c_str());
		return static_cast<int>(m_vstrVariableNames.at(iVariable).length());
	}

	catch(const exception& e) {	m_szError = e.what(); }
	catch(...)  { m_szError = ERRORCODES[1]; }
	return -1;
}

int CDataFile::GetVariableName(const int& iVariable, std::string& rStr)
{
	try
	{
		rStr = m_vstrVariableNames.at(iVariable).c_str();
		return static_cast<int>(rStr.length());
	}

	catch(const exception& e) {	m_szError = e.what(); }
	catch(...)  { m_szError = ERRORCODES[1]; }
	return -1;
}

int CDataFile::GetLargestVectorSize_() const
{
	int retVal = 0;
	int curVal = 0;

	if(m_dwReadFlags & RF_READ_AS_DOUBLE)
	{
		std::vector<std::vector<double> >::const_iterator it = m_v2dData.begin();

		while(it != m_v2dData.end())
		{
			curVal = static_cast<int>(it->size());
			retVal = (curVal > retVal) ? curVal : retVal;
			it++;
		}
	}

	else if(m_dwReadFlags & RF_READ_AS_STRING)
	{
		std::vector<std::vector<std::string> >::const_iterator it = m_v2dStrData.begin();

		while(it != m_v2dStrData.end())
		{
			curVal = static_cast<int>(it->size());
			retVal = (curVal > retVal) ? curVal : retVal;
			it++;
		}
	}

	return retVal;
}

// Clears all data in the CDataFile
void CDataFile::ClearData()
{
	m_szError = "";
	std::vector<std::string>().swap(m_vstrVariableNames);
	std::vector<std::string>().swap(m_vstrSourceFilenames);
	std::vector<std::vector<std::string> >().swap(m_v2dStrData);
	std::vector<std::vector<double> >().swap(m_v2dData);
	ClearDataHead(m_DataHead);
}

// Returns the value at the specified location if successful.
// Returns DF::ERRORVALUE if an error is encountered.
double CDataFile::GetData(const int& iVariable, const int& iSample)
{
	if(m_dwReadFlags & RF_READ_AS_DOUBLE)
	{
		try
		{
			return m_v2dData.at(iVariable).at(iSample);
		}

		catch(const exception& e) 
		{	
			m_szError = e.what(); 
		}
		
		catch(...) 
		{ 
			m_szError = ERRORCODES[2]; 
		}
	}

	else
		m_szError = ERRORCODES[4];

	return ERRORVALUE;
}

// Returns the length of the string if successful. 
// Returns -1 if an error is encountered.
int CDataFile::GetData(const int& iVariable, const int& iSample, std::string& rStr)
{
	if(m_dwReadFlags & RF_READ_AS_STRING)
	{
		try
		{
			rStr = m_v2dStrData.at(iVariable).at(iSample).c_str();
			return static_cast<int>(rStr.length());
		}

		catch(const exception& e) 
		{	
			m_szError = e.what(); 
			return -1;
		}
		
		catch(...) 
		{ 
			m_szError = ERRORCODES[1]; 
			return -1;
		}
	}

	else
	{
		m_szError = ERRORCODES[3];
		return -1;
	}
}

// Returns the length of the string if successful. 
// Returns -1 if an error is encountered.
int CDataFile::GetData(const char* szVariableName, const int& iSample, std::string& rStr)
{
	int retVal = 0;

	int index = LookupVariableIndex_(szVariableName);
	
	if(index != -1)
		retVal = GetData(index, iSample, rStr);
	
	else
	{
		m_szError = ERRORCODES[5]; 
		retVal = -1;
	}
	
	return retVal;
}


// Returns the length of the string if successful. 
// Returns -1 if an error is encountered.
int CDataFile::GetData(const int& iVariable, const int& iSample, char* lpStr)
{
	int retVal = 0;

	if(m_dwReadFlags & RF_READ_AS_STRING)
	{
		try
		{
			strcpy(lpStr,m_v2dStrData.at(iVariable).at(iSample).c_str());
			retVal = static_cast<int>(m_v2dStrData.at(iVariable).at(iSample).length());
		}

		catch(const exception& e) 
		{	
			m_szError = e.what(); 
			retVal = -1;
		}
		
		catch(...) 
		{ 
			m_szError = ERRORCODES[1]; 
			retVal = -1;
		}
	}

	else
	{
		m_szError = ERRORCODES[3];
		retVal = -1;
	}

	return retVal;
}

// Returns the new size of rVector if successful.
// Returns -1 if an error is encountered.
int CDataFile::GetData(const int& iVariable, std::vector<double>& rVector)
{
	int retVal = 0;

	try
	{
		rVector = m_v2dData.at(iVariable);
		retVal  = static_cast<int>(rVector.size());
	}

	catch(const exception& e) 
	{	
		m_szError = e.what();
		retVal = -1;
	}
	
	catch(...) 
	{ 
		m_szError = ERRORCODES[2]; 
		retVal = -1;
	}

	return retVal;
}

// Returns the new size of rVector if successful.
// Returns -1 if an error is encountered.
int CDataFile::GetData(const int& iVariable, std::vector<std::string>& rVector)
{
	int retVal = 0;

	try
	{
		rVector = m_v2dStrData.at(iVariable);
		retVal  = static_cast<int>(rVector.size());
	}

	catch(const exception& e) 
	{	
		m_szError = e.what();
		retVal = -1;
	}
	
	catch(...) 
	{ 
		m_szError = ERRORCODES[2]; 
		retVal = -1;
	}

	return retVal;
}

// Returns the value at the specified location if successful.
// Returns DF::ERRORVALUE if an error is encountered.
double CDataFile::GetData(const char* szVariableName, const int& iSample)
{
	int index = LookupVariableIndex_(szVariableName);
	
	if(index != -1)
		return GetData(index, iSample);
	
	else
		m_szError = ERRORCODES[5]; 
	
	return ERRORVALUE;
}

// Returns true if successful. 
// Returns false if an error is encountered.
int CDataFile::GetData(const char* szVariableName, const int& iSample, char* lpStr)
{
	int retVal = 0;

	int index = LookupVariableIndex_(szVariableName);
	
	if(index != -1)
		retVal = GetData(index, iSample, lpStr);
	
	else
	{
		m_szError = ERRORCODES[5]; 
		retVal = -1;
	}
	
	return retVal;
}

// Returns the new size of rVector if successful.
// Returns -1 if an error is encountered.
int CDataFile::GetData(const char* szVariableName, std::vector<double>& rVector)
{
	int index = LookupVariableIndex_(szVariableName);
	
	if(index != -1)
		return GetData(index, rVector);
	
	else
		m_szError = ERRORCODES[5]; 
	
	return -1;
}

// Returns the new size of rVector if successful.
// Returns -1 if an error is encountered.
int CDataFile::GetData(const char* szVariableName, std::vector<std::string>& rVector)
{
	int index = LookupVariableIndex_(szVariableName);
	
	if(index != -1)
		return GetData(index, rVector);
	
	else
		m_szError = ERRORCODES[5]; 
	
	return -1;
}

// Returns the index of the first instance of the specified variable
// found AFTER iStartingIndex.
// Returns -1 if the variable is not found.
int CDataFile::GetVariableIndex(const char* szVariableName, const int& iStartingIndex /*=0*/)
{
	return LookupVariableIndex_(szVariableName, iStartingIndex);
}

// Returns the index of the specified variable.
// Returns -1 if the variable is not found.
int CDataFile::GetVariableIndex(const char* szVariableName, const char* szSourceFilename, const int& iStartingIndex /*=0*/)
{
	int offset = 0;

	std::vector<std::string>::iterator it = 
		find_if
		(
			m_vstrSourceFilenames.begin(), 
			m_vstrSourceFilenames.end(),
			ByName(szSourceFilename)
		);

	if(it == m_vstrSourceFilenames.end())
	{
		m_szError = ERRORCODES[6];
		return -1;
	}

	while(it != m_vstrSourceFilenames.begin())
	{
		offset++;
		it--;
	}
	
	return LookupVariableIndex_(szVariableName, iStartingIndex+offset);
}

// Returns the index of the first variable name that matches szName.
// Returns -1 if szName is not found.
int CDataFile::LookupVariableIndex_(const char* szName,  const int& offset /*=0*/) const
{
	int retVal = 0;
	
	std::vector<std::string>::const_iterator it = 
		find_if
		(
			m_vstrVariableNames.begin() + offset, 
			m_vstrVariableNames.end(),
			ByName(szName)
		);

	if(it == m_vstrVariableNames.end())
		retVal = -1;

	else
	{
		while(it != m_vstrVariableNames.begin())
		{
			retVal++;
			it--;
		}
	}

	return retVal;
}

/*
功能: 将时间类型转换成秒级的日期时间字符串
输入: 需转换的时间
输出: 转换后的秒级日期时间字符串(YYYY-MM-DD HH:mm:ss)
*/
char * CDataFile::GetTimeStringBySeconds(const time_t tSecond)
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

bool CDataFile::ReadFileHead( const char* szFilename )
{
	try
	{
		m_szFilename  = szFilename;
		// m_dwReadFlags = dwReadFlags;
		ClearData();
		
		ifstream inFile;
		inFile.open(szFilename);
		
		if(inFile.rdstate() & std::ios::failbit)
		{
			inFile.close();
			m_szError = ERRORCODES[7]; 
			m_szError += "\nDetails: ";
			m_szError += szFilename;
			return false;
		}
		
		if(!(m_dwReadFlags & RF_APPEND_DATA))
			ClearData();
		
		// 获取案例参数
		std::fstream::pos_type pos = GetDataHeadNew(inFile,m_DataHead);
		int nVars;
		if (m_DataHead.is_old_file == 1)
		{
			// 旧数据格式,定位到首行
			inFile.seekg(0);
			pos = GetDataHead(inFile,m_DataHead);
			// count the number of variables
			nVars = CountColsData(inFile, m_delim.at(0),m_DataHead.is_delim_end);

			// make sure data columns are equal to nVars
			// in order to prevent mis-aligned data
			inFile.ignore(MAX_LINE_BUFFER,'\n');
			
			if(CountColsData(inFile, m_delim.at(0),m_DataHead.is_delim_end) != nVars)
			{
				m_szError = ERRORCODES[8];
				//throw exception(m_szError.c_str());
				throw m_szError.c_str();
			}
		}
		else
		{
			// 新数据格式
			// count the number of variables
			nVars = CountColsData(inFile, m_delim.at(0),m_DataHead.is_delim_end);

			// make sure data columns are equal to nVars
			// in order to prevent mis-aligned data
			inFile.ignore(MAX_LINE_BUFFER,'\n');
			
			if(CountColsData(inFile, m_delim.at(0),m_DataHead.is_delim_end) != nVars)
			{
				m_szError = ERRORCODES[8];
				//throw exception(m_szError.c_str());
				throw m_szError.c_str();
			}
		}
		
		// rewind the file
		// 定位到数据行
		// if (pos > 0)
		{
			inFile.seekg(pos);
		}
		
		int iVar = 0;
		int varOffset = GetNumberOfVariables();
		
		char buff[MAX_FIELD_BUFFER] = {0};
		std::string tmpstring;
		for(iVar=0; iVar<nVars; iVar++)
		{
			if(m_DataHead.is_delim_end == 1)
			{
				// 行末尾有分隔符
				inFile.getline(buff, sizeof buff, m_delim.at(0));
			}
			else
			{
				// 行末尾无分隔符
				inFile.getline(buff, sizeof buff, (iVar == nVars-1) ? '\n' : m_delim.at(0));
			}
			tmpstring = buff;
			DF_StringTrimAll(tmpstring);
			m_vstrVariableNames.push_back(tmpstring);
			m_vstrSourceFilenames.push_back(m_szFilename);
			
			if(m_dwReadFlags & RF_READ_AS_DOUBLE)
			{
				m_v2dData.push_back(vector<double>());
				m_v2dData.back().reserve(m_reserve);
			}
			
			else if(m_dwReadFlags & RF_READ_AS_STRING)
			{
				m_v2dStrData.push_back(vector<string>());
				m_v2dStrData.back().reserve(m_reserve);
			}
		}
		
		inFile.close();

		return true;
	}
	
	catch(const exception& e)
	{
		m_szError = e.what();
	}
	
	catch(...)
	{
		m_szError = ERRORCODES[0];
	}
	
	return false;
}

int CDataFile::ClearDataHead( DATA_FILE_DEV_INFO &tmpDataHead )
{
	tmpDataHead.is_old_file = 0;
	tmpDataHead.start_time = 0;
	tmpDataHead.end_time = 0;
	tmpDataHead.time_sample = 0.0f;
	tmpDataHead.dev_num = 0;
	tmpDataHead.gps_time = 0;
	tmpDataHead.is_delim_end = 0;
	if (!tmpDataHead.vec_devid_s.empty())
	{
		std::vector<std::string>().swap(tmpDataHead.vec_devid_s);
		tmpDataHead.vec_devid_s.clear();
 		std::vector<std::string>().swap(tmpDataHead.vec_devvolt_s);
 		tmpDataHead.vec_devvolt_s.clear();
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
int CDataFile::DF_StringTrimAll(std::string &str)
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

// restore warnings
#ifdef _WINDOWS32
#pragma warning(pop)
#endif
