#include "qiniu.h"

Qiniu_Mac mac;
long uptokenExpire; //expireDays
char* uptoken; //上传token

/******************************************************************************
 *  Name        :   Qiniu::init_global
 *  Author      :   xiaojunzhong
 *  Version     :   V1.0.0
 *  Data        :   2021.04.14
 *  Describe    :   初始化七牛云库
 ******************************************************************************/
int Qiniu::init_global()
{
    try
    {
        Qiniu_Global_Init(-1);

        mac.accessKey = "EJVQa8CyiPUElQCyotFlqxcmos52OTMzmqAjc0So";
        mac.secretKey = "RM0oGopOUz4gISSeXCfSF8IUFCZTSHJnuPI4EyJs";
        Qiniu_RS_PutPolicy putPolicy;
        Qiniu_Io_PutExtra putExtra;

        Qiniu_Zero(putPolicy);
        Qiniu_Zero(putExtra);
        putPolicy.scope = "airobot2";
        putPolicy.deleteAfterDays = 1;
        putPolicy.expires = 3600 * 25; //单位秒

        unsigned int expire = (unsigned int)(putPolicy.expires + Timer::get_unix_timestamp()); //1小时后过期

        uptoken = Qiniu_RS_PutPolicy_Token(&putPolicy, &mac);
        if (uptoken)
        {
            uptokenExpire = expire;
            printf("Qiniu::init_global(),success,uptoken:%s", uptoken);
        }
        else
        {
            uptokenExpire = -1;
            printf("Qiniu::init_global(),Qiniu_RS_PutPolicy_Token error");
        }
        Qiniu_Use_Zone_Huadong(Qiniu_False);
        printf("Qiniu::init_global(),Timer::get_current_date_str():%s", Timer::get_current_date_str().c_str());

#ifdef DEBUG
        Logger::getInstance()->debug("qiniu init,uptoken is:%s", "airobot2");
#endif

        return 0;
    }
    catch (...)
    {
        printf("qiniu init fail..");
    }

    return -1;
}


/******************************************************************************
*  Name        :   qiniu2::check_refresh_token
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2020.12.18
*  Describe    :   检测token是否有效
******************************************************************************/
bool Qiniu::check_refresh_token()
{
    try
    {
        const time_t t = Timer::get_unix_timestamp();
        if (uptokenExpire > t && NULL != uptoken) //如果token正常,返回有效true
        {
            return true;
        }

        refresh_token(); //尝试刷新token
        if (uptokenExpire > t) //检测刷新情况
        {
            return true;
        }

        return false; //刷新失败
    }
    catch (...)
    {
    }

    return false;
}

/******************************************************************************
*  Name        :   qiniu2::refresh
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2020.12.17
*  Describe    :   刷新Token
******************************************************************************/
int Qiniu::refresh_token()
{
    try
    {
        Qiniu_RS_PutPolicy putPolicy;
        Qiniu_Io_PutExtra putExtra;

        Qiniu_Zero(putPolicy);
        Qiniu_Zero(putExtra);
        putPolicy.scope = "airobot2";
        putPolicy.deleteAfterDays = 1;
        putPolicy.expires = 3600 * 24; //单位秒

        const long expire = 3600 * 24 + Timer::get_unix_timestamp(); //1小时后过期

        uptoken = Qiniu_RS_PutPolicy_Token(&putPolicy, &mac);

        if (NULL != uptoken)
        {
            uptokenExpire = expire;
        }
        else
        {
            uptokenExpire = -1;
        }

        return 0;
    }
    catch (...)
    {
    }

    return -1;
}

/******************************************************************************
*  Name        :   qiniu2::refresh
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2020.12.17
*  Describe    :   文件上传
******************************************************************************/
int Qiniu::upload(const char* key, const char* localFile, int flag)
{
    try
    {
        if (key == NULL || localFile == NULL)
        {
            return -4;
        }

        const char* key2 = key;
        const char* localFile2 = localFile;

        if (!check_refresh_token())
        {
            return -3;
        }

        Qiniu_Client client; //七牛云客户端

        Qiniu_Io_PutRet putRet;
        Qiniu_Io_PutExtra putExtra;
        Qiniu_Zero(putExtra);

        Qiniu_Client_InitMacAuth(&client, 1024, &mac);
        const Qiniu_Error error = Qiniu_Io_PutFile(&client, &putRet, uptoken, key2, localFile2, &putExtra);

        if (error.code != 200)
        {
            Logger::getInstance()->warning("upload file error,flag:%d", flag);
            Logger::getInstance()->warning("upload file error,config.bucket:%s", "airobot2");
            Logger::getInstance()->warning("upload file error,error.code:%d,error.message::%s", error.code, error.message);
            const int eCode = error.code / 100;

            switch (eCode)
            {
                case 1:
                {
                    Logger::getInstance()->warning("upload connect error");
                }
                break;

                case 3:
                {
                    Logger::getInstance()->warning("upload transfer error");
                }
                break;

                case 4:
                {
                    Logger::getInstance()->warning("upload auth error");
                    Qiniu::refresh_token();
                }
                break;

                case 5:
                {
                    Logger::getInstance()->warning("upload server error");
                }
                break;

                default:
                    break;
            }
            Qiniu_Client_Cleanup(&client);

            return -2;
        }
        else
        {
            std::string bucket = "if-pbl";
            int days = 30;
            Qiniu_Error error = Qiniu_RS_DeleteAfterDays(&client, bucket.c_str(), key, days);
            (void)error;

            /*200, 正确返回了, 你可以通过statRet变量查询一些关于这个文件的信息*/
            Logger::getInstance()->debug("upload file %s:%s success", bucket.c_str(), key2);
            Logger::getInstance()->debug("key:\t%s\n", putRet.key);
            Logger::getInstance()->debug("hash:\t%s\n", putRet.hash);
        }
        Qiniu_Client_Cleanup(&client);

        return 0;
    }
    catch (...)
    {
    }

    return -1;
}

/******************************************************************************
*  Name        :   qiniu2::resumable_upload
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2020.12.17
*  Describe    :   文件分片上传
******************************************************************************/
int Qiniu::resumable_upload(const char* key, const char* localFile, int flag)
{
    try
    {
        if (key == NULL || localFile == NULL)
        {
            return -4;
        }

        const char* key2 = key;
        const char* localFile2 = localFile;

        if (!check_refresh_token())
        {
            return -3;
        }

        Qiniu_Client client; //七牛云客户端

        Qiniu_Rio_PutRet putRet;
        Qiniu_Rio_PutExtra extra;
        Qiniu_Zero(extra);

        Qiniu_Client_InitMacAuth(&client, 1024, &mac);
        const Qiniu_Error error = Qiniu_Rio_PutFile(&client, &putRet, uptoken, key2, localFile2, &extra);

        if (error.code != 200)
        {
            Logger::getInstance()->warning("resumable_upload file error,flag:%d", flag);
            Logger::getInstance()->warning("resumable_upload file error,config.bucket:%s", "airobot2");
            Logger::getInstance()->warning("resumable_upload file error,error.code:%d,error.message::%s", error.code, error.message);
            Logger::getInstance()->warning("key2:", key2);
            Logger::getInstance()->warning("localFile2:", localFile2);

            const int eCode = error.code / 100;

            switch (eCode)
            {
                case 1:
                {
                    Logger::getInstance()->warning("resumable_upload connect error");
                }
                break;

                case 3:
                {
                    Logger::getInstance()->warning("resumable_upload transfer error");
                }
                break;

                case 4:
                {
                    Logger::getInstance()->warning("resumable_upload auth error");
                    Qiniu::refresh_token();
                }
                break;

                case 5:
                {
                    Logger::getInstance()->warning("resumable_upload server error");
                }
                break;

                default:
                    break;
            }

            Qiniu_Client_Cleanup(&client);
            return -2;
        }
        else
        {
            std::string bucket = "if-pbl";
            int days = 30;
            Qiniu_Error error = Qiniu_RS_DeleteAfterDays(&client, bucket.c_str(), key, days);
            (void)error;
            Logger::getInstance()->debug("upload file %s:%s success", bucket.c_str(), key2);
            Logger::getInstance()->debug("key:\t%s\n", putRet.key);
            Logger::getInstance()->debug("hash:\t%s\n", putRet.hash);
        }
        Qiniu_Client_Cleanup(&client);

        return 0;
    }
    catch (...)
    {
    }

    return -1;
}

/******************************************************************************
*  Name        :   qiniu2::downloadUrl
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2020.12.17
*  Describe    :   文件下载
******************************************************************************/
char* Qiniu::download_url(const char* baseUrl, const char* key)
{
    try
    {
        char* url = 0;
        char* baseUrl2 = 0;
        Qiniu_RS_GetPolicy getPolicy;
        Qiniu_Zero(getPolicy);
        baseUrl2 = Qiniu_RS_MakeBaseUrl(baseUrl, key);
        url = Qiniu_RS_GetPolicy_MakeRequest(&getPolicy, baseUrl, &mac);

        cout << "url:" << url << endl;
#ifdef DEBUG
#endif

        Qiniu_Free(baseUrl2);

        return url;
    }
    catch (...)
    {
    }

    return nullptr;
}


/******************************************************************************
*  Name        :   qiniu2::upload_file_and_delete
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2020.12.18
*  Describe    :   上传文件并删除本地文件
******************************************************************************/
int Qiniu::upload_file_and_delete(const char* file_path, const char* key, char* httpUrl, int flag)
{
    try
    {
        if (NULL == file_path || strlen(file_path) <= 2)
        {
            return -1;
        }

        if (NULL == key || strlen(key) <= 2)
        {
            return -1;
        }

        if (!Files::exists_local_file(file_path))
        {
            return -2;
        }

        int i = 5, ret = -1;

        if (Files::get_st_size(file_path) > 4 * 1024 * 1024) //针对大于4m的文件进行分片上传
        {
            while ((ret = resumable_upload(key, file_path, flag)) < 0 && i > 0)
            {
                i--;
                sleep(1);
            }
        }
        else
        {
            while ((ret = upload(key, file_path, flag)) < 0 && i > 0)
            {
                i--;
                sleep(1);
            }
        }

        if (ret < 0)
        {
            Logger::getInstance()->warning("upload_file_and_delete fail,please check");
            return -3;
        }
        Files::delete_local_file(file_path);
        std::string tempUrl = "";

        tempUrl = "https://airobot.510link.com/";
        tempUrl.append(key);
        memcpy(httpUrl, tempUrl.data(), tempUrl.size());

        return httpUrl ? (int)strlen(httpUrl) : 0;
    }
    catch (...)
    {
    }

    return -4;
}

/******************************************************************************
*  Name        :   qiniu2::upload_dir_and_delete
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2020.12.18
*  Describe    :   上传目录并删除本地文件
******************************************************************************/
int Qiniu::upload_dir_and_delete(const char* path, std::string& token, const char* suffix)
{
    try
    {
        const char* _path;
        struct dirent* ptr;

        if (!path || strlen(path) == 0)
        {
            _path = "/mnt/picture";
        }
        else
        {
            _path = path;
        }
        mkdir(_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        const char* _suffix;

        if (!suffix || strlen(suffix))
        {
            _suffix = suffix;
        }
        else
        {
            _suffix = ".jpg";
        }
        const std::string dir_path = _path;
        DIR* dir = opendir(_path);

        while ((ptr = readdir(dir)) != nullptr)
        {
            if (ptr == nullptr)
            {
                continue;
            }

            if (ptr->d_name[0] == '.')
            {
                continue;
            }

            const std::string file_name = ptr->d_name;
            if (!file_name.find_last_of(_suffix))
            {
                return -1;
            }

            const std::string key = token + "/" + Timer::get_current_date_str() + "/" + file_name;
            const std::string file_path = dir_path + "/" + file_name;
            upload(key.c_str(), file_path.c_str());
            Files::delete_local_file(file_path.c_str());

            Logger::getInstance()->debug("upload_dir_and_delete,file_name:%s", ptr->d_name);
            Logger::getInstance()->debug("upload_dir_and_delete,d_type:%s", ptr->d_type);
            Logger::getInstance()->debug("upload_dir_and_delete,file_path:%s", file_path.c_str());
            Logger::getInstance()->debug("upload_dir_and_delete,key:%s", key.c_str());
        }
        closedir(dir);

        return 0;
    }
    catch (...)
    {
    }

    return -1;
}
//
///******************************************************************************
// *  Name        :   HikCamera::init_curl
// *  Author      :   cqnews
// *  Version     :   V1.0.0
// *  Data        :   2020.09.21
// *  Describe    :   初始化Curl
// ******************************************************************************/
//CURL* Qiniu::init_curl()
//{
//    try
//    {
//        struct curl_slist* headers = NULL;
//
//        headers = curl_slist_append(headers, "Accept:text/html, application/xhtml+xml, */*");
//        headers = curl_slist_append(headers, "Accept-Language:zh-CN");
//        headers = curl_slist_append(
//            headers, "User-Agent:Mozilla/5.0 (Windows NT 6.1;WOW64; Trident/7.0; rv:11.0) like Gecko");
//        headers = curl_slist_append(headers, "Accept - Encoding: gzip, deflate");
//        //headers = curl_slist_append(headers, "Host:114.114.114.114");
//        headers = curl_slist_append(headers, "Expect:"); //防止Expect：100-Continue
//        headers = curl_slist_append(headers, "Content-Type:text/html; charset=utf-8");
//
//        CURL* curl = curl_easy_init(); //初始化
//
//        if (curl)
//        {
//            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); //改协议头
//            return curl;
//        }
//    }
//    catch (...)
//    {
//    }
//
//    return nullptr;
//}
