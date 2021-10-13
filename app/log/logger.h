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
        enum log_level { debug_level, info_level, warning_level, error_level };// 日志等级
        enum log_target { file_target, terminal_target, file_and_terminal_target };// 日志输出目标
    protected:
        Logger(); //构造方法
    private:
        int flush_count = 0;
        std::ofstream outfile; //将日志输出到文件的流对象
        log_target target; //日志输出目标
        LoggerConfig logConfig; //日志文件路径
        //std::string path; //日志文件路径
        std::string current_path;
        log_level level; //日志等级
        void output(log_level act_level, std::string text); //输出行为
        std::string currFormatTime(std::string formatStr, long subtract = 0);
        std::string currTime();
    public:
        void init(log_target ter_target, log_level act_level, LoggerConfig log_config); //带参构造函数
        void debug(const char* fmt, ...); //调试输出
        void info(const char* fmt, ...); //普通信息输出
        void warning(const char* fmt, ...); //警告输出
        void error(const char* fmt, ...); //错误输出
        void output(const char* buffer, log_level act_level= Logger::info_level); //日志普通输出
        void flush(); //刷新缓冲
        void backups(); //备份当天的日志
    private:
        void open(); //打开缓冲区
        std::string generate_current_path(std::string filename="log"); //日志单例打开输出日志
        std::string generate_backups_path(int day = 0); //生成新的日志文件
        void delete_old_log(); //删除旧的日志
    private:
        static Logger* m_instance_ptr; //单例变量
    public:
        static pthread_mutex_t mutex;
        static Logger* getInstance(); //单例实例化函数
};
# endif

