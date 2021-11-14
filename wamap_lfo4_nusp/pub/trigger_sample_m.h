//**************************************
// Generated by mcpp translator
// Version 1.2
//**************************************

#ifndef __TRIGGER_SAMPLE_M_H__
#define __TRIGGER_SAMPLE_M_H__

#include"mcode/mvector.h"
#include"mcode/mstring.h"
#include"mcode/mstream.h"
#include"mcode/mclient.h"
#include"public_m.h"


const short SAMPLE_DATATYPE_LONG = 0;
const short SAMPLE_DATATYPE_FLOAT = 1;
const short SAMPLE_DATATYPE_STRING = 2;
const short OFFERDATA_SAMPLE_MODE = 0;
const short UNPROMPTED_SAMPLE_MODE = 1;
typedef MLang::VECTOR<TKeyID> SEQKeyID;
struct USampleDataValue
{
private:
	union
	{
		MLang::Long	c_long;
		float	c_float;
		char*	c_string;
	}__u_val;
	bool __u_init;
	unsigned short __u_index;
public:
	USampleDataValue();
	~USampleDataValue();
	USampleDataValue(const USampleDataValue&);
	USampleDataValue&operator=(const USampleDataValue&);
	short _d()const;
	void __clear();
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
	void c_long(const MLang::Long);
	const MLang::Long	c_long()const;
	void c_float(const float);
	const float	c_float()const;
	void c_string(const char*);
	void	c_string(const MLang::STRING&);
	const char*c_string()const;
};
typedef MLang::VECTOR<USampleDataValue> SEQSampleDataValue;
struct TSampleKeyIDInfo
{
	TKeyID	keyid;
	short	keyid_datatype;
	unsigned char	is_key;
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
typedef MLang::VECTOR<TSampleKeyIDInfo> SEQSampleKeyIDInfo;
struct TSampleDefineRequest
{
	int	sample_type;
	int	app_no;
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
typedef MLang::VECTOR<TSampleDefineRequest> SEQSampleDefineRequest;
struct TSEQSampleDefineRequest
{
	int	seq_length;
	SEQSampleDefineRequest	seq_sample_define_request;
	TSEQSampleDefineRequest();
	TSEQSampleDefineRequest(const TSEQSampleDefineRequest&);
	TSEQSampleDefineRequest&operator=(const TSEQSampleDefineRequest&);
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
struct TSampleDefineAnswer
{
	int	sample_type;
	int	app_no;
	bool	is_success;
	TDBErrorStru	error_info;
	short	keyid_num;
	SEQSampleKeyIDInfo	seq_keyid_info;
	TSampleDefineAnswer();
	TSampleDefineAnswer(const TSampleDefineAnswer&);
	TSampleDefineAnswer&operator=(const TSampleDefineAnswer&);
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
typedef MLang::VECTOR<TSampleDefineAnswer> SEQSampleDefineAnswer;
struct TSEQSampleDefineAnswer
{
	int	seq_length;
	SEQSampleDefineAnswer	seq_sample_define_answer;
	TSEQSampleDefineAnswer();
	TSEQSampleDefineAnswer(const TSEQSampleDefineAnswer&);
	TSEQSampleDefineAnswer&operator=(const TSEQSampleDefineAnswer&);
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
struct TSampleKeyIDValue
{
	TKeyID	keyid;
	short	keyid_datatype;
	USampleDataValue	keyid_value;
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
typedef MLang::VECTOR<TSampleKeyIDValue> SEQSampleKeyIDValue;
struct TSampleProcessRequest
{
	int	sample_type;
	int	app_no;
	unsigned char	sample_mode;
	int	occur_time;
	int	flag;
	int	reserve_1;
	int	reserve_2;
	short	keyid_num;
	SEQSampleKeyIDValue	seq_keyid_value;
	TSampleProcessRequest();
	TSampleProcessRequest(const TSampleProcessRequest&);
	TSampleProcessRequest&operator=(const TSampleProcessRequest&);
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
typedef MLang::VECTOR<TSampleProcessRequest> SEQSampleProcessRequest;
struct TSEQSampleProcessRequest
{
	int	seq_length;
	SEQSampleProcessRequest	seq_sample_process_request;
	TSEQSampleProcessRequest();
	TSEQSampleProcessRequest(const TSEQSampleProcessRequest&);
	TSEQSampleProcessRequest&operator=(const TSEQSampleProcessRequest&);
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
struct TSampleProcessInfo
{
	TKeyID	keyid;
	short	keyid_datatype;
	USampleDataValue	keyid_value;
	bool	is_valid;
	MLang::STRING	invalid_string;
	TSampleProcessInfo();
	TSampleProcessInfo(const TSampleProcessInfo&);
	TSampleProcessInfo&operator=(const TSampleProcessInfo&);
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
typedef MLang::VECTOR<TSampleProcessInfo> SEQSampleProcessInfo;
struct TSampleProcessAnswer
{
	int	sample_type;
	int	app_no;
	unsigned char	sample_mode;
	int	occur_time;
	short	keyid_num;
	SEQSampleProcessInfo	seq_process_info;
	bool	is_success;
	TDBErrorStru	error_info;
	TSampleProcessAnswer();
	TSampleProcessAnswer(const TSampleProcessAnswer&);
	TSampleProcessAnswer&operator=(const TSampleProcessAnswer&);
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
typedef MLang::VECTOR<TSampleProcessAnswer> SEQSampleProcessAnswer;
struct TSEQSampleProcessAnswer
{
	int	seq_length;
	SEQSampleProcessAnswer	seq_sample_process_answer;
	TSEQSampleProcessAnswer();
	TSEQSampleProcessAnswer(const TSEQSampleProcessAnswer&);
	TSEQSampleProcessAnswer&operator=(const TSEQSampleProcessAnswer&);
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
#endif
