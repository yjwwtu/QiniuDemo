#ifndef __HIKCAMERA_H__
#define __HIKCAMERA_H__

#include <string.h>
#include <curl/curl.h>
#include <iostream>
#include <map>
#include <unistd.h>
#include <sys/wait.h>
#include "../function/functions.h"
#include "../log/logger.h"

namespace std
{
    class HttpClient
    {

    private:
        //CURL* curl; //curl库

    public:
        int HttpGet(const char* httpurl, std::string& body, struct curl_slist* headers = NULL); //get获取
        int HttpPost(const char* httpurl, std::string& body, const string& params, struct curl_slist* headers);
        int download(const char* httpUrl, const char* localFile);//文件下载
        int wget(const char* httpUrl, const char* localFile);//wget文件下载
        int download2(const char* httpUrl, const char* localFile);
        bool CheckIsNetWorking(const char* httpurl = "http://api.heclouds.com/"); //检查联网状态
        //post获取
        static size_t receive_data(char* buffer, size_t size, size_t nmemb, std::string* stream);
    private:
        int initCurl(CURL*& curl, struct curl_slist* headers); //初始化Curl
        static HttpClient* m_instance_ptr; //单例变量
    public:
        static pthread_mutex_t mutex;
        static HttpClient* getInstance(); //单例实例化函数
    };
}
#endif
