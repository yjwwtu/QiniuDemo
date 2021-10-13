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

extern Qiniu_Mac mac; //��ţ������
extern long uptokenExpire; //expireDays
extern char* uptoken; //�ϴ�token

class Qiniu
{
public:

    /******************************************************************************
     *  Name        :   Qiniu::init_global
     *  Author      :   xiaojunzhong
     *  Version     :   V1.0.0
     *  Data        :   2021.04.14
     *  Describe    :   ��ʼ����ţ�ƿ�
     ******************************************************************************/
    static int init_global();


    /******************************************************************************
    *  Name        :   qiniu2::downloadUrl
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.17
    *  Describe    :   �ļ�����
    ******************************************************************************/
    static char* download_url(const char* baseUrl, const char* key);


    /******************************************************************************
    *  Name        :   qiniu2::refresh
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.17
    *  Describe    :   �ļ��ϴ�
    ******************************************************************************/
    static int upload(const char* key, const char* localFile, int flag = 0);

    /******************************************************************************
    *  Name        :   qiniu2::resumable_upload
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.17
    *  Describe    :   �ļ���Ƭ�ϴ�
    ******************************************************************************/
    static int resumable_upload(const char* key, const char* localFile, int flag = 0);

    /******************************************************************************
    *  Name        :   qiniu2::upload_dir_and_delete
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.18
    *  Describe    :   �ϴ�Ŀ¼��ɾ�������ļ�
    ******************************************************************************/
    static int upload_dir_and_delete(const char* path, std::string& token, const char* suffix);

    /******************************************************************************
    *  Name        :   qiniu2::upload_file_and_delete
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.18
    *  Describe    :   �ϴ��ļ���ɾ�������ļ�
    ******************************************************************************/
    static int upload_file_and_delete(const char* file_path, const char* key, char* httpUrl,int flag = 0);


private:

    /******************************************************************************
    *  Name        :   qiniu2::check_refresh_token
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.18
    *  Describe    :   ���token�Ƿ���Ч
    ******************************************************************************/
    static bool check_refresh_token();

    /******************************************************************************
    *  Name        :   qiniu2::refresh
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.17
    *  Describe    :   ˢ��Token
    ******************************************************************************/
    static int refresh_token();



    ///******************************************************************************
    //*  Name        :   HikCamera::init_curl
    //*  Author      :   cqnews
    //*  Version     :   V1.0.0
    //*  Data        :   2020.09.21
    //*  Describe    :   ��ʼ��Curl
    //******************************************************************************/
    //static CURL* init_curl();
};



#endif
