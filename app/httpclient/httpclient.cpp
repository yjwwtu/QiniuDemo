#include "httpclient.h"

namespace std {

    /******************************************************************************
     *  Name        :   HttpClient::get
     *  Author      :   cqnews
     *  Version     :   V1.0.0
     *  Data        :   2021.08.12
     *  Describe    :   get获取
     ******************************************************************************/
    int HttpClient::HttpGet(const char* httpurl, std::string& body, struct curl_slist* headers)
    {
        int ret = 0;

        if (body.empty())
        {
            return -1;
        }
        CURL* curl = NULL;

        do
        {
            CURLcode res;
            if (initCurl(curl, headers) != 0)
            {
                ret = -2;
                break;
            }

            std::string receive_data_str = "";
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); //改协议头
            curl_easy_setopt(curl, CURLOPT_URL, httpurl);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpClient::receive_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&receive_data_str);
            curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);//设置超时时间
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);//设置超时时间
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
            curl_easy_setopt(curl, CURLOPT_DNS_SERVERS, "114.114.114.114");

            res = curl_easy_perform(curl); //执行
            if (CURLE_OK != res)
            {
                ret = -2;
                break;
            }

            long retCode = 0;
            res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &retCode);
            if (CURLE_OK != res || retCode != 200)
            {
                ret = static_cast<int>(-1 * retCode);
                break;
            }

            long heart_size = 0;
            res = curl_easy_getinfo(curl, CURLINFO_HEADER_SIZE, &heart_size);

            if (CURLE_OK != res)
            {
                ret = -3;
                break;
            }
            body = receive_data_str.substr(heart_size);
        } while (0);

        if (headers)
        {
            curl_slist_free_all(headers);
        }
        if (curl)
        {
            curl_easy_cleanup(curl);
        }

        return ret;
    }


    /******************************************************************************
     *  Name        :   HttpClient::post
     *  Author      :   cqnews
     *  Version     :   V1.0.0
     *  Data        :   2021.08.13
     *  Describe    :   post获取
     ******************************************************************************/
    int HttpClient::HttpPost(const char* httpurl, std::string& body, const string& params, struct curl_slist* headers)
    {
        int ret = 0;

        if (body.empty())
        {
            return -1;
        }

        CURL* curl = NULL;

        do
        {

            CURLcode res;

            if (initCurl(curl, headers) != 0)
            {
                ret = -2;
                break;
            }

            std::string receive_data_str = "";

            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); //改协议头
            curl_easy_setopt(curl, CURLOPT_POST, 1);
            curl_easy_setopt(curl, CURLOPT_URL, httpurl);

            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params.c_str());//设置post请求的参数
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, params.length());//设置post请求的参数

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpClient::receive_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&receive_data_str);
            curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
            curl_easy_setopt(curl, CURLOPT_DNS_SERVERS, "114.114.114.114");

            //设置超时时间
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

#ifdef DEBUG
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
#endif // DEBUG

            res = curl_easy_perform(curl); //执行

            if (CURLE_OK != res) { ret = -2; break; }


            long retCode = 0;
            res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &retCode);

            if (CURLE_OK != res || retCode != 200)
            {
                ret = static_cast<int>(-1 * retCode); break;
            }


            long heart_size = 0;

            res = curl_easy_getinfo(curl, CURLINFO_HEADER_SIZE, &heart_size);

            if (CURLE_OK != res)
            {
                ret = -3; break;
            }

            body = receive_data_str.substr(heart_size);


        } while (0);

        if (headers) { curl_slist_free_all(headers); }
        if (curl) { curl_easy_cleanup(curl); }

        return ret;
    }


    /******************************************************************************
     *  Name        :   HttpClient::CheckIsNetWorking
     *  Author      :   cqnews
     *  Version     :   V1.0.0
     *  Data        :   2021.08.30
     *  Describe    :   检查联网状态
     ******************************************************************************/
    bool HttpClient::CheckIsNetWorking(const char* httpurl)
    {
        //创建curl对象 
        CURL* curl;
        //创建curlcode对象 
        CURLcode res;

        bool ret = false;

        curl = curl_easy_init();
        if (curl)
        {
            //设置一个请求文件地址 
            curl_easy_setopt(curl, CURLOPT_URL, httpurl);
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);//设置超时时间
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);//设置超时时间
            curl_easy_setopt(curl, CURLOPT_HEADER, 0);    //只需要header头
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1);    //不需要body
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
            //执行请求操作,将返回值交给res 
            res = curl_easy_perform(curl);
            //res=0的情况表示接收到信息,反之则没有收到信息 
            if (res == CURLE_OK)
            {
                //已经联网 
                ret = true;
            }
            else
            {
                //没有联网 
                ret = false;
            }
            /* 释放资源 */
            if (curl) { curl_easy_cleanup(curl); }
        }
        return ret;
    }

    /******************************************************************************
     *  Name        :   HttpClient::receive_data
     *  Author      :   cqnews
     *  Version     :   V1.0.0
     *  Data        :   2021.08.12
     *  Describe    :   解析data
     ******************************************************************************/
    size_t HttpClient::receive_data(char* data, size_t size, size_t nmemb, std::string* stream)
    {

        int res;

        if (stream != NULL)
        {
            res = static_cast<int>((size * nmemb));
            stream->append(data, res);
        }

        return res;
    }

    /******************************************************************************
     *  Name        :   HikCamera::initCurl
     *  Author      :   xiaojunzhong
     *  Version     :   V1.0.0
     *  Data        :   2020.09.21
     *  Describe    :   初始化Curl
     ******************************************************************************/
    int HttpClient::initCurl(CURL*& curl, struct curl_slist* headers)
    {
        headers = curl_slist_append(headers, "Accept:application/json");
        headers = curl_slist_append(headers, "Accept-Language:zh-CN");
        headers = curl_slist_append(headers, "User-Agent:Mozilla/5.0 (Windows NT 6.1;WOW64; Trident/7.0; rv:11.0) like Gecko");
        headers = curl_slist_append(headers, "Accept - Encoding: gzip, deflate");
        /*headers = curl_slist_append(headers, "Host:api.heclouds.com");*/
        headers = curl_slist_append(headers, "Expect:"); //防止Expect：100-Continue
        headers = curl_slist_append(headers, "Content-Type:text/html; charset=utf-8");
        curl = curl_easy_init(); //初始化
        curl_easy_setopt(curl, CURLOPT_DNS_SERVERS, "114.114.114.114");


        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); //改协议头
            return 0;
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
    int HttpClient::download(const char* httpUrl, const char* localFile)
    {
        try
        {
            int ret = -100, i = 0;

            Logger::getInstance()->error("HttpClient::download starttime:%d", Timer::get_unix_timestamp());

            while (i < 5)
            {
                i++;
                Logger::getInstance()->error("HttpClient::download i:%d", i);

                FILE* fp;

                if ((fp = fopen(localFile, "wb")) == NULL) //返回结果用文件存储
                {
                    ret = -101;
                    //usleep(10000);
                    continue;
                }

                CURL* curl = NULL; //curl库 

                struct curl_slist* headers = NULL;
                if (initCurl(curl, headers) != 0) //初始化
                {
                    Logger::getInstance()->error("HttpClient::initCurl error");
                    fclose(fp);
                    ret = -102;
                    //usleep(10000);
                    continue;
                }

                CURLcode res = curl_easy_setopt(curl, CURLOPT_URL, httpUrl);
                if (res != CURLE_OK)
                {
                    fclose(fp);
                    curl_easy_cleanup(curl);
                    ret = -103;
                    Logger::getInstance()->error("HttpClient::curl_easy_setopt(curl, CURLOPT_URL, httpUrl), return res:%d", res);
                    //usleep(10000);
                    continue;
                }

                curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
                curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);//设置超时时间
                //curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, 120);//设置超时时间
                //curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);//设置超时时间
                curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
                //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                curl_easy_setopt(curl, CURLOPT_DNS_SERVERS, "114.114.114.114");

                res = curl_easy_perform(curl); //执行
                if (res != CURLE_OK)
                {
                    Logger::getInstance()->error("HttpClient::curl_easy_perform(curl), return res:%d", res);
                    ret = -abs(CURLE_OK);
                    //usleep(10000);
                }
                else
                {
                    ret = 0;
                }

                fclose(fp);
                curl_easy_cleanup(curl);

                int size = static_cast<int>(Files::get_st_size(localFile));

                if (ret == 0 && size >= 10)//下载文件大于10byte
                {

                    Logger::getInstance()->error("HttpClient::download endtime:%d", Timer::get_unix_timestamp());
                    return size;
                }

            };


            Logger::getInstance()->error("HttpClient::download endtime1:%d", Timer::get_unix_timestamp());
            return ret;
        }
        catch (...)
        {
        }

        Logger::getInstance()->error("HttpClient::download endtime2:%d", Timer::get_unix_timestamp());
        return -5;
    }

    /******************************************************************************
    *  Name        :   HttpClient::wget
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.09.10
    *  Describe    :   wget文件下载
    ******************************************************************************/
    int HttpClient::wget(const char* httpUrl, const char* localFile)
    {
        pid_t pid;

        if ((pid = vfork()) < 0)
        {
            printf("vfork error");
            exit(1);
        }
        else if (pid == 0)
        {
            if (execlp("wget", "wget", httpUrl, "-O", localFile, (char*)0) < 0)
            {
                printf("execlp error\n");
                exit(1);
            }
        }

        int stat;
        waitpid(pid, &stat, 0);

        if (stat == 0)
        {
            int size = static_cast<int>(Files::get_st_size(localFile));

            if (size >= 10)//下载文件大于10byte
            {
                return size;
            }

            return -1;
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
    int HttpClient::download2(const char* httpUrl, const char* localFile)
    {
        try
        {
            Logger::getInstance()->error("HttpClient::download starttime:%d", Timer::get_unix_timestamp());
            FILE* fp;

            if ((fp = fopen(localFile, "wb")) == NULL) //返回结果用文件存储
            {
                return -1;
            }
            CURL* curl = NULL; //curl库

            struct curl_slist* headers = NULL;
            if (initCurl(curl, headers) != 0) //初始化
            {
                return -2;
            }

            CURLcode res = curl_easy_setopt(curl, CURLOPT_URL, httpUrl);
            if (res != CURLE_OK)
            {
                fclose(fp);
                curl_easy_cleanup(curl);
                return -3;
            }

            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_setopt(curl, CURLOPT_DNS_SERVERS, "114.114.114.114");
            res = curl_easy_perform(curl); //执行
            fclose(fp);
            if (res != CURLE_OK)
            {
                curl_easy_cleanup(curl);
                return -4;
            }

            curl_easy_cleanup(curl);
            curl_global_cleanup();

            Logger::getInstance()->error("HttpClient::download starttime:%d", Timer::get_unix_timestamp());

            int size = static_cast<int>(Files::get_st_size(localFile));

            return size;
        }
        catch (...)
        {
        }

        return -5;
    }

    /******************************************************************************
    *  Name        :   Logger::Logger
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.07.28
    *  Describe    :  日志单例函数
    ******************************************************************************/
    HttpClient* HttpClient::getInstance()
    {
        if (m_instance_ptr == nullptr)
        {
            pthread_mutex_lock(&mutex);
            if (m_instance_ptr == nullptr)
            {
                m_instance_ptr = new HttpClient();
                curl_global_init(CURL_GLOBAL_ALL);
            }
            pthread_mutex_unlock(&mutex);
        }

        return m_instance_ptr;
    }

    pthread_mutex_t HttpClient::mutex;
    HttpClient* HttpClient::m_instance_ptr = nullptr;
}
