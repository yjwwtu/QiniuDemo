#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <memory.h>
#include <dirent.h>
#include <netdb.h>
#include <net/if.h>
#include <wait.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

#include "../log/logger.h"

#define IP_SIZE                     16
#define UNUSED(x) (void)x

class Timer
{
public:

    /******************************************************************************
     *  Name        :   Timer::getUnixTimestamp
     *  Author      :   cqnews
     *  Version     :   V1.0.0
     *  Data        :   2020.12.17
     *  Describe    :   获取unix时间戳
     ******************************************************************************/
    static time_t get_unix_timestamp();


    /******************************************************************************
    *  Name        :   Timer::getUnixTimestamp
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.17
    *  Describe    :   获取unix日期时间戳
    ******************************************************************************/
    static std::string get_current_date_str();

    /******************************************************************************
    *  Name        :   Timer::format_datetime
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.04.13
    *  Describe    :   数字转时间戳
    ******************************************************************************/
    static time_t format_datetime(int uiYear, int uiMonth, int uiDay, int uiHour, int uiMinute, int uiSecond);
};


class Files {
public:
    /******************************************************************************
    *  Name        :   Files::exists
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.05.07
    *  Describe    :   检测文件是否存在
    ******************************************************************************/
    static bool exists_local_file(const char* path);

    /******************************************************************************
    *  Name        :   Files::delete_local_file
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.18
    *  Describe    :   删除本地文件
    ******************************************************************************/
    static int delete_local_file(const char* localFile);

    /******************************************************************************
    *  Name        :   Files::delete_expire_local_file
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.09.06
    *  Describe    :   删除本地超时文件
    ******************************************************************************/
    static int delete_expire_local_file(const char* dirPath, int second = 60);


    /******************************************************************************
    *  Name        :   Files::delete_local_file
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.05.11
    *  Describe    :   获取文件的修改时间
    ******************************************************************************/
    static time_t get_modify_time(const char* filePath);

    /******************************************************************************
    *  Name        :   Files::delete_local_file
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.05.11
    *  Describe    :   获取文件的修改时间
    ******************************************************************************/
    static time_t get_modify_time(FILE* fp);

    /******************************************************************************
    *  Name        :   Files::get_st_size
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.05.11
    *  Describe    :   获取文件的大小
    ******************************************************************************/
    static long get_st_size(const char* filePath);

    /******************************************************************************
    *  Name        :   Files::move
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.09.01
    *  Describe    :   移动文件夹,模拟mv
    ******************************************************************************/
    static int move2(const char* sourcePath, const char* targetPath, bool isDelete);

    /******************************************************************************
    *  Name        :   Files::getpwdpath
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.09.22
    *  Describe    :   获取当前目录路径
    ******************************************************************************/
    static int getpwdpath(char* buffer);
};


class Function
{
public:
    static void get_current_time(); //获取当前系统时间
    static void executeCMD(const char* cmd, char* result); //命令执行
    static void systemCMD(const char* cmd);
    static int ping_online_device(const char* svrip);// ping设备是否在线 
    static void delay(const int time);//延迟函数 获取unix时间戳
    static int get_current_hours(); //获取当前小时数
    static int get_local_ip(const char* eth_inf, char* ip);//获取本机的ip地址
    static int get_local_gateway(char* ip);
};

#endif //__FUNCTION_H__