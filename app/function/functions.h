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
     *  Describe    :   ��ȡunixʱ���
     ******************************************************************************/
    static time_t get_unix_timestamp();


    /******************************************************************************
    *  Name        :   Timer::getUnixTimestamp
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.17
    *  Describe    :   ��ȡunix����ʱ���
    ******************************************************************************/
    static std::string get_current_date_str();

    /******************************************************************************
    *  Name        :   Timer::format_datetime
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.04.13
    *  Describe    :   ����תʱ���
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
    *  Describe    :   ����ļ��Ƿ����
    ******************************************************************************/
    static bool exists_local_file(const char* path);

    /******************************************************************************
    *  Name        :   Files::delete_local_file
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2020.12.18
    *  Describe    :   ɾ�������ļ�
    ******************************************************************************/
    static int delete_local_file(const char* localFile);

    /******************************************************************************
    *  Name        :   Files::delete_expire_local_file
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.09.06
    *  Describe    :   ɾ�����س�ʱ�ļ�
    ******************************************************************************/
    static int delete_expire_local_file(const char* dirPath, int second = 60);


    /******************************************************************************
    *  Name        :   Files::delete_local_file
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.05.11
    *  Describe    :   ��ȡ�ļ����޸�ʱ��
    ******************************************************************************/
    static time_t get_modify_time(const char* filePath);

    /******************************************************************************
    *  Name        :   Files::delete_local_file
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.05.11
    *  Describe    :   ��ȡ�ļ����޸�ʱ��
    ******************************************************************************/
    static time_t get_modify_time(FILE* fp);

    /******************************************************************************
    *  Name        :   Files::get_st_size
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.05.11
    *  Describe    :   ��ȡ�ļ��Ĵ�С
    ******************************************************************************/
    static long get_st_size(const char* filePath);

    /******************************************************************************
    *  Name        :   Files::move
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.09.01
    *  Describe    :   �ƶ��ļ���,ģ��mv
    ******************************************************************************/
    static int move2(const char* sourcePath, const char* targetPath, bool isDelete);

    /******************************************************************************
    *  Name        :   Files::getpwdpath
    *  Author      :   cqnews
    *  Version     :   V1.0.0
    *  Data        :   2021.09.22
    *  Describe    :   ��ȡ��ǰĿ¼·��
    ******************************************************************************/
    static int getpwdpath(char* buffer);
};


class Function
{
public:
    static void get_current_time(); //��ȡ��ǰϵͳʱ��
    static void executeCMD(const char* cmd, char* result); //����ִ��
    static void systemCMD(const char* cmd);
    static int ping_online_device(const char* svrip);// ping�豸�Ƿ����� 
    static void delay(const int time);//�ӳٺ��� ��ȡunixʱ���
    static int get_current_hours(); //��ȡ��ǰСʱ��
    static int get_local_ip(const char* eth_inf, char* ip);//��ȡ������ip��ַ
    static int get_local_gateway(char* ip);
};

#endif //__FUNCTION_H__