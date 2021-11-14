//**************************************
// Generated by mcpp translator
// Version 1.2
//**************************************

#ifndef __WAMAPSSOMSG_M_H__
#define __WAMAPSSOMSG_M_H__

#include"mcode/mvector.h"
#include"mcode/mstring.h"
#include"mcode/mstream.h"
#include"mcode/mclient.h"


struct FAD_CALLUP_COMTR_NAME_STRU
{
	MLang::VECTOR<MLang::Long>	fac_seq;
	short	fac_num;
	MLang::ULong	start_time;
	MLang::ULong	end_time;
	MLang::STRING	src_host_name;
	MLang::STRING	src_proc_name;
	FAD_CALLUP_COMTR_NAME_STRU();
	FAD_CALLUP_COMTR_NAME_STRU(const FAD_CALLUP_COMTR_NAME_STRU&);
	FAD_CALLUP_COMTR_NAME_STRU&operator=(const FAD_CALLUP_COMTR_NAME_STRU&);
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
struct FAD_TRANS_REPORT_STRUCT
{
	MLang::Long	fac_id;
	short	type;
	MLang::STRING	filename;
	int	total_frame;
	int	current_frame;
	FAD_TRANS_REPORT_STRUCT();
	FAD_TRANS_REPORT_STRUCT(const FAD_TRANS_REPORT_STRUCT&);
	FAD_TRANS_REPORT_STRUCT&operator=(const FAD_TRANS_REPORT_STRUCT&);
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
#endif
