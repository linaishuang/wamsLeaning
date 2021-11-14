
#include "FileOpSop.h"
const char*  file_server_addr = "10.0.0.2";
#ifdef _NUSP
//#include "sam_service.h"
#else
#include "sam_service.h"
#endif // _NUSP

#include "system.h"
#include <sys/time.h>
#include <sys/stat.h>
//#include <CommanService.h>

CFileOpSop::CFileOpSop()
{
    // printf("CFileOpSop构造开始\n");
}

CFileOpSop::~CFileOpSop()
{

}

//上传文件
//svrfile   服务端文件路径
//localfile 本地文件路径
//filesize  返回文件大小
//>=0 OK
int CFileOpSop::Upload(const char * host, const char * svrfile, const char * localfile, int &filesize)
{
    //读取本地文件
    if( access(localfile, F_OK) != 0 )
    {
        //sprintf(logstr, "本地文件%s没有准备好!", localfile); 
        //WriteLog(logstr, LOG_LEVEL_ERROR);
        return -1;
    }
    
    //sprintf(logstr, "CFileOpSop::Upload");
    //WriteLog(logstr);

    //创建服务器目录
    char fdir[512]={0};
    sprintf( fdir, "%s", string(svrfile).substr(0, string(svrfile).find_last_of("/")).c_str() );
    
    m_FileOp.setSocket(host, HISFILEPORT_SOP);
    
    //sprintf(logstr, "CFileOpSop::Upload, setSocket");
    //WriteLog(logstr);
    /*
    FileInfo tmp_dirinfo;
    int ret = m_FileOp.list_dir(fdir, tmp_dirinfo);  //文件多时耗时长
    if( ret != 0 ) //目录不存在 - 创建
    {
        ret = m_FileOp.create_dir( fdir ); 
        if( ret != 1 ) 
        {
            if( m_FileOp.get_error_code() == EEXIST )
            {
                printf("服务器目录已经存在: %s\n", fdir); 
            }else{
                sprintf(logstr, "服务器创建目录失败: %s\n", fdir); 
                WriteLog(logstr, LOG_LEVEL_ERROR);
                return -1;
            }
        }
    }
    */
    int  ret = m_FileOp.create_dir( fdir ); printf("create_dir[%s]=%d\n", fdir, ret); 
    if( ret != 1 ) 
    {
        if( m_FileOp.get_error_code() == EEXIST )
        {
            printf("服务器目录已经存在: %s\n", fdir); 
        }else{
            //sprintf(logstr, "服务器创建目录失败: %s\n", fdir); 
            //WriteLog(logstr, LOG_LEVEL_ERROR);
            return -1;
        }
    }
   // sprintf(logstr, "CFileOpSop::Upload, create_dir");
    //WriteLog(logstr);
    
    //删除同名文件
    m_FileOp.delete_file( svrfile );
    
    //sprintf(logstr, "CFileOpSop::Upload, delete_file");
    //WriteLog(logstr);

    //上传文件
    FILE            *fp_putread = NULL;	//文件访问指针
    unsigned char   read_filebuf[HISFILEREADBUFSIZE_SOP+1]; //文件读写缓冲区
    unsigned char * p_putfilebuf = NULL; //文件缓存指针
    unsigned char * p_tmp_putfilebuf = NULL; //文件临时缓存指针
    int             act_read_count = 0; //文件单次读取字节长度
    int             total_file_size = 0;//文件总读取字节长度
    int             offset = 0;//文件读取位移    
    
    if((fp_putread = fopen (localfile, "rb"))==NULL)
    {
       // sprintf(logstr,"读取本地文件失败: %s\n", localfile); 
        //WriteLog(logstr, LOG_LEVEL_ERROR);
        return -1;
    }
    p_putfilebuf = (unsigned char*)malloc(1);
    while (!feof(fp_putread))
    {
        // Read the data from file 
        memset(read_filebuf,0,(HISFILEREADBUFSIZE_SOP+1)*sizeof(char));
        act_read_count = fread(read_filebuf, sizeof(unsigned char), HISFILEREADBUFSIZE_SOP, fp_putread);
        read_filebuf[act_read_count]='\0';
        
        total_file_size += act_read_count;
        p_tmp_putfilebuf  = p_putfilebuf;
        p_putfilebuf = (unsigned char *)realloc(p_putfilebuf, total_file_size);
        if (p_putfilebuf == NULL)
        {		
            if(p_tmp_putfilebuf) free(p_tmp_putfilebuf); 
            if(p_putfilebuf) free(p_putfilebuf);
            fclose(fp_putread);
            //sprintf(logstr,"读取本地文件失败2: %s\n", localfile); 
            //WriteLog(logstr, LOG_LEVEL_ERROR);
            return -1;
        }
        memcpy(p_putfilebuf + offset, read_filebuf, act_read_count);
        offset += act_read_count;
    }	
    //if(p_tmp_putfilebuf) free(p_tmp_putfilebuf); 
    fclose(fp_putread); 

    //创建文件
    filesize = total_file_size;
    ret = m_FileOp.create_file(svrfile, (const char*)p_putfilebuf, total_file_size);
    if(ret != 1)
    {
        //sprintf(logstr,"上传文件失败[%d]: %s\n", ret, svrfile); 
        //WriteLog(logstr, LOG_LEVEL_ERROR);
        if (p_putfilebuf)
        {
            free(p_putfilebuf);
            p_putfilebuf = NULL;
        }
        return - 1;
    }
    if (p_putfilebuf)
    {
        free(p_putfilebuf); 
        p_putfilebuf = NULL; 
    }
    //sprintf(logstr, "CFileOpSop::Upload, create_file");
    //WriteLog(logstr);
    
    //sprintf(logstr,"上传文件成功[%d]: %s\n", ret, svrfile); 
    //WriteLog(logstr, LOG_LEVEL_KEY);
    return 1; 
}

//下载文件
//svrfile   服务端文件路径  var/dir/dir2/file.txt
//localfile 本地文件全路径  /home/d5000/wending/file.txt
int CFileOpSop::Download(const char * host,const char * svrfile, const char * localfile)
{
    char * body = NULL;
    int len = 0;
    m_FileOp.setSocket(host, HISFILEPORT_SOP); 
    int ret = m_FileOp.get_file (svrfile, -1, &body, len);
    if( ret == 1 )
    {
        printf( "localfile = %s\n",  localfile); 
        FILE * fp = fopen(localfile, "wb");
        if( fp )
        {
            int wt_len = fwrite(body, sizeof(char), len, fp );
            fclose(fp); 
            if( wt_len != len ){
                printf("fwrite error!\n"); 
                return -1;
            }
            if( body ) free( body ); 
            return 1;
        }
        else
        {
            if( body ) free( body ); 
            return -1; 
        }
    }
    return -1;
}


//删除文件
int CFileOpSop::Delete(const char * host, const char * svrfile)
{
    m_FileOp.setSocket(host, HISFILEPORT_SOP); 
	int ret = m_FileOp.delete_file(svrfile);
    printf("删除文件%s, %s\n", svrfile, (ret==1)?"成功":"失败" ); 
	return ret;	
}

//获取文件主机
//flag=1主机, 2备机
bool CFileOpSop::GetFileHost(char * host, int flag)
{
    CServicesManage serinfo;
    int tmp_ser=serinfo.RequestService(AP_DATA_SRV, flag, host, AC_REALTIME_NO);
    if(tmp_ser>=0)
    {
        printf("AP_DATA_SRV = %s\n", host);
    }
    else
    {
        printf("找不到文件服务器\n"); 
        return false;
    } 
    return true; 
}
