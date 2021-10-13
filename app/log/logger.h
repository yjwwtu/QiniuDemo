#ifndef LOGGER_SYS_H
#define LOGGER_SYS_H

#include <iostream>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <mutex>

#include <iterator>
#include "../function/functions.h"

using std::cout;
using std::endl;
using std::to_string;
using std::ios;

typedef struct LOGGER_CONF
{
    std::string path = "";
    std::string nFormat = "%Y%m%d";
    std::string suffix = ".log";
    bool showColor = true;

}LoggerConfig;


class Logger 
{
    public:
        enum log_level { debug_level, info_level, warning_level, error_level };// ��־�ȼ�
        enum log_target { file_target, terminal_target, file_and_terminal_target };// ��־���Ŀ��
    protected:
        Logger(); //���췽��
    private:
        int flush_count = 0;
        std::ofstream outfile; //����־������ļ���������
        log_target target; //��־���Ŀ��
        LoggerConfig logConfig; //��־�ļ�·��
        //std::string path; //��־�ļ�·��
        std::string current_path;
        log_level level; //��־�ȼ�
        void output(log_level act_level, std::string text); //�����Ϊ
        std::string currFormatTime(std::string formatStr, long subtract = 0);
        std::string currTime();
    public:
        void init(log_target ter_target, log_level act_level, LoggerConfig log_config); //���ι��캯��
        void debug(const char* fmt, ...); //�������
        void info(const char* fmt, ...); //��ͨ��Ϣ���
        void warning(const char* fmt, ...); //�������
        void error(const char* fmt, ...); //�������
        void output(const char* buffer, log_level act_level= Logger::info_level); //��־��ͨ���
        void flush(); //ˢ�»���
        void backups(); //���ݵ������־
    private:
        void open(); //�򿪻�����
        std::string generate_current_path(std::string filename="log"); //��־�����������־
        std::string generate_backups_path(int day = 0); //�����µ���־�ļ�
        void delete_old_log(); //ɾ���ɵ���־
    private:
        static Logger* m_instance_ptr; //��������
    public:
        static pthread_mutex_t mutex;
        static Logger* getInstance(); //����ʵ��������
};
# endif

