#ifndef __QINIU_H__
#define __QINIU_H__

#include <stdio.h>
#include <iostream>
#include <curl/curl.h>

#include "headers/http.h"
#include "headers/rs.h"
#include "headers/io.h"
#include "headers/resumable_io.h"
#include "../function/functions.h"

extern Qiniu_Mac mac; //七牛云配置
extern long uptokenExpire; //expireDays
extern char* uptoken; //上传token

class Qiniu
{
public:

    /******************************************************************************
     *  Name        :   Qiniu::init_global
     *  Author      :   xiaojunzhong
     *  Version     :   V1.0.0
     *  Data        :   2021.04.14
     *  Describe    :   初始化七牛云库
     ******************************************************************************/
    static int init_global();


    /******************************************************************************
    *  Name        :   qiniu2::downloadUrl
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.17
    *  Describe    :   文件下载
    ******************************************************************************/
    static char* download_url(const char* baseUrl, const char* key);


    /******************************************************************************
    *  Name        :   qiniu2::refresh
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.17
    *  Describe    :   文件上传
    ******************************************************************************/
    static int upload(const char* key, const char* localFile, int flag = 0);

    /******************************************************************************
    *  Name        :   qiniu2::resumable_upload
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.17
    *  Describe    :   文件分片上传
    ******************************************************************************/
    static int resumable_upload(const char* key, const char* localFile, int flag = 0);

    /******************************************************************************
    *  Name        :   qiniu2::upload_dir_and_delete
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.18
    *  Describe    :   上传目录并删除本地文件
    ******************************************************************************/
    static int upload_dir_and_delete(const char* path, std::string& token, const char* suffix);

    /******************************************************************************
    *  Name        :   qiniu2::upload_file_and_delete
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.18
    *  Describe    :   上传文件并删除本地文件
    ******************************************************************************/
    static int upload_file_and_delete(const char* file_path, const char* key, char* httpUrl,int flag = 0);


private:

    /******************************************************************************
    *  Name        :   qiniu2::check_refresh_token
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.18
    *  Describe    :   检测token是否有效
    ******************************************************************************/
    static bool check_refresh_token();

    /******************************************************************************
    *  Name        :   qiniu2::refresh
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.17
    *  Describe    :   刷新Token
    ******************************************************************************/
    static int refresh_token();



    ///******************************************************************************
    //*  Name        :   HikCamera::init_curl
    //*  Author      :   cqnews
    //*  Version     :   V1.0.0
    //*  Data        :   2020.09.21
    //*  Describe    :   初始化Curl
    //******************************************************************************/
    //static CURL* init_curl();
};



#endif
