#include "logger.h"

/******************************************************************************
*  Name        :   Logger::Logger
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.07.28
*  Describe    :  ��־���캯��
******************************************************************************/
Logger::Logger()
{
    pthread_mutex_init(&mutex, NULL); //��ʼ��
    this->target = terminal_target;
    //this->path = this->generate_outfile_path();
    this->current_path = "log.log";
    this->level = debug_level;
}

/******************************************************************************
*  Name        :   Logger::Logger
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.07.23
*  Describe    :   ��־���ι��췽��
******************************************************************************/
void Logger::init(log_target ter_target, log_level act_level, LoggerConfig log_config)
{
    this->target = ter_target;
    this->logConfig = log_config;
    if (this->logConfig.nFormat.empty() || this->logConfig.nFormat == "")
    {
        this->logConfig.nFormat = "%Y%m%d";
    }

    if (this->logConfig.suffix.empty() || this->logConfig.suffix == "")
    {
        this->logConfig.nFormat = ".log";
    }

    if (this->logConfig.path.empty() || this->logConfig.path == "")
    {
        this->logConfig.path = "";
    }
    else if (this->logConfig.path.find_last_of("/") != (this->logConfig.path.length() - 1))
    { //����ҵ���������Ϊ/,Ĭ��������/
        this->logConfig.path += "/";
    }

    this->current_path = this->generate_current_path("log");

    //this->path = this->generate_outfile_path();
    this->level = act_level;

    this->delete_old_log();




    std::string  tmp = "\033[33m\n";
    tmp += "===========================================================================================\n";
    //tmp += "|     11   11         111111111 11    11       11      11         11           |\n";
    //tmp += "|  11 11  1111111111    11  11   11  11    111111111111111111     11111111     |\n";
    //tmp += "|  11111111 11 11 11    111111  111111111      11  11  11         11           |\n";
    //tmp += "| 11  11    11 11 11    11  11     11        1111111111111   1111111111111111  |\n";
    //tmp += "|     11   11 11  11    111111 1111111111    111  111   111       1111         |\n";
    //tmp += "|  111111111 111  11    11  11    1111     11111111111111111      11 11        |\n";
    //tmp += "|     11    111  111    11  11   111 111        111 111           11   11      |\n";
    //tmp += "|     11  1111  1111  1111111111111  1111    1111     1111        11     11    |\n";
    //tmp += "|     11 111    111         11  11    11   111           111      11      11   |\n";
    tmp += " _____ __  _____ _ _       _                         \n";
    tmp += "|  ___/  ||  _  | (_)     | |                        \n";
    tmp += "|___ \\`| || |/' | |_ _ __ | | __  ___ ___  _ __ ___  \n";
    tmp += "    \\ \\| ||  /| | | | '_ \\| |/ / / __/ _ \\| '_ ` _ \\ \n";
    tmp += "/\\__/ /| |\\ |_/ / | | | | |   < | (_| (_) | | | | | |\n";
    tmp += "\\____/\\___/\\___/|_|_|_| |_|_|\\_(_)___\\___/|_| |_| |_|\n\n";
    tmp += "::510Link.com ::                (version:1.0)\n\n";
    tmp += "===========================================================================================\n";
    tmp += "============================== WELCOME TO YINKA LOGGER ====================================\033[0m\n"; // ˫�����µĳ�������ֱ����ӣ�������һ��string������ת��
    std::string  welcome_dialog = tmp + "\033[33m[INIT] " + __FILE__ + " " + currTime() + " : " + "=== Start ===\033[0m\n";
    welcome_dialog += "\033[33m[INIT] this->target:" + std::to_string(this->target) + "\033[0m\n";
    welcome_dialog += "\033[33m[INIT] this->level:" + std::to_string(this->level) + "\033[0m\n";
    //welcome_dialog += "\033[33m[INIT] this->path:" + this->path + "\033[0m\n";

    if (this->target != terminal_target)
    {
        this->open(); //������ļ�
        this->outfile << welcome_dialog;
    }
    if (file_target != ter_target)
    {
        cout << welcome_dialog; //�����־�����ǽ��ļ�
    }
}

/******************************************************************************
*  Name        :   Logger::output
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.07.23
*  Describe    :   ��־���
******************************************************************************/
void Logger::output(log_level act_level, std::string text)
{
    std::string prefix;
    std::string color_prefix = "";
    std::string color_suffix = "";


    if (act_level == debug_level)
    {
        prefix = "[DEBUG]  ";
        color_prefix += "\033[32m";
        color_prefix = "";
        color_suffix = "";
    }
    else if (act_level == info_level)
    {
        prefix = "[INFO]   ";
        color_prefix += "";
    }
    else if (act_level == warning_level)
    {
        prefix = "[WARNING]";
        color_prefix += "\033[33m";
        color_suffix = "\033[0m";
    }
    else if (act_level == error_level)
    {
        prefix = "[ERROR]  ";
        color_prefix += "\033[31m";
        color_suffix = "\033[0m";
    }
    else
    {
        prefix = "";
        color_prefix = "";
        color_suffix = "";
    }

    prefix += "";
    if (this->logConfig.showColor == false)
    {
        color_prefix = "";
        color_suffix = "";
    }


    std::string buffer = prefix + currTime() + " : " + text;
    if (this->level <= act_level && this->target != file_target)
    {
        // ��ǰ�ȼ��趨�ĵȼ��Ż���ʾ���նˣ��Ҳ�����ֻ�ļ�ģʽ
        cout << color_prefix << buffer << color_suffix << std::endl;
    }

    if (this->level <= act_level && this->target != terminal_target)
    {


        if (!outfile.is_open()) //���Դ�
        {
            this->open();
        }

        if (outfile.is_open())
        {
            outfile << buffer << "\n";

            pthread_mutex_lock(&mutex);
            if (this->flush_count >= 100)
            {
                this->flush_count = 0;
                this->flush();
            }
            else
            {
                this->flush_count++;
            }
            pthread_mutex_unlock(&mutex);
        }
    }
}

/******************************************************************************
*  Name        :   Logger::currDateTime
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.07.23
*  Describe    :   ��־�и�ʽ����
******************************************************************************/
std::string Logger::currFormatTime(std::string formatStr, long subtract)
{
    // ��ȡ��ǰʱ�䣬���淶��ʾ
    char tmp[64];
    time_t ptime;
    time(&ptime);  // time_t time (time_t* timer);
    ptime -= subtract;

    strftime(tmp, sizeof(tmp), formatStr.c_str(), localtime(&ptime));
    return tmp;
}

/******************************************************************************
*  Name        :   Logger::currTime
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.07.23
*  Describe    :   ��־�и�ʽ����+ʱ��
******************************************************************************/
std::string Logger::currTime()
{
    char tmp[64];
    time_t ptime;
    time(&ptime);  // time_t time (time_t* timer); // ��ȡ��ǰʱ�䣬���淶��ʾ

    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&ptime));
    return tmp;
}

/******************************************************************************
*  Name        :   Logger::DEBUG
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.07.23
*  Describe    :   ��־DEBUG��ӡ
******************************************************************************/
void Logger::debug(const char* fmt, ...)
{
    va_list argList = {  };
    va_start(argList, fmt);
    char buffer[256] = { 0 };
    vsnprintf(buffer, 256, fmt, argList);
    this->output(debug_level, buffer);
    va_end(argList);
}

/******************************************************************************
*  Name        :   Logger::INFO
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.07.23
*  Describe    :   ��־INFO��ӡ
******************************************************************************/
void Logger::info(const char* fmt, ...)
{
    va_list argList = {  };
    va_start(argList, fmt);
    char buffer[256] = { 0 };
    vsnprintf(buffer, 256, fmt, argList);
    this->output(info_level, buffer);
    va_end(argList);
}


/******************************************************************************
*  Name        :   Logger::buff
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.08.02
*  Describe    :   ��־BUFF��ӡ
******************************************************************************/
void Logger::output(const char* buffer, log_level act_level)
{
    this->output(act_level, buffer);
}

/******************************************************************************
*  Name        :   Logger::warning
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.07.23
*  Describe    :   ��־WARNING��ӡ
******************************************************************************/
void Logger::warning(const char* fmt, ...)
{
    va_list argList = {  };
    va_start(argList, fmt);
    char buffer[512] = { 0 };
    vsnprintf(buffer, 256, fmt, argList);
    this->output(warning_level, buffer);
    va_end(argList);
}



/******************************************************************************
*  Name        :   Logger::error
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.07.23
*  Describe    :   ��־ERROR��ӡ
******************************************************************************/
void Logger::error(const char* fmt, ...)
{
    va_list argList = {  };
    va_start(argList, fmt);
    char buffer[256] = { 0 };
    vsnprintf(buffer, 256, fmt, argList);
    this->output(error_level, buffer);
    va_end(argList);
}

/******************************************************************************
*  Name        :   Logger::flush
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.07.23
*  Describe    :   ��־�����ӡ
******************************************************************************/
void Logger::flush()
{
    if (this->target != terminal_target)
    {

        //std::string newpath = this->generate_outfile_path(); //��ȡ�µ��ļ�����

        //if (newpath.compare(this->path) != 0) { //�ж��ļ������Ƿ�һ��,�����һ�¾����´�
        //    this->open(newpath); //����ִ�к����Ķ���,�رպ��Զ�flush
        //    return;
        //}

        if (!outfile.is_open()) //���Դ�
        {
            this->open();
        }

        if (outfile.is_open())
        {
            outfile.flush();
        }
    }
}


/******************************************************************************
*  Name        :   Logger::open
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.08.02
*  Describe    :  ��־�����������־
******************************************************************************/
void Logger::open()
{


    if (outfile.is_open())
    {
        outfile.close();
    }

 

    outfile.open(this->current_path, ios::out | ios::app);

}




/******************************************************************************
*  Name        :   Logger::backups
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.09.01
*  Describe    :  ���ݵ������־
******************************************************************************/
void Logger::backups()
{
    std::string backup_path = this->generate_backups_path();

    pthread_mutex_lock(&mutex);

    this->flush();//ˢ����־����,׼�����log.txt,׼������
    this->outfile.close();
    Files::move2(this->current_path.c_str(), backup_path.c_str(), true);
    sleep(3);
    this->delete_old_log();

    pthread_mutex_unlock(&mutex);


}

/******************************************************************************
*  Name        :   Logger::open
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.08.02
*  Describe    :  ��־�����������־
******************************************************************************/
std::string Logger::generate_current_path(std::string filename)
{
    std::string return_path = "";
    return_path = this->logConfig.path + filename;
    return_path += this->logConfig.suffix;

    return return_path;

}



/******************************************************************************
*  Name        :   Logger::open
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.08.02
*  Describe    :  ��־�����������־
******************************************************************************/
std::string Logger::generate_backups_path(int day)
{
    std::string return_path = "";
    return_path = this->logConfig.path + currFormatTime(this->logConfig.nFormat, static_cast<long>(day) * 24 * 60 * 60);
    return_path += this->logConfig.suffix;

    return return_path;

}

/******************************************************************************
*  Name        :   Logger::open
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.08.02
*  Describe    :  ��־����ɾ���ɵĹ�����־
******************************************************************************/
void Logger::delete_old_log()
{
    do
    {
        const char* dirPath = this->logConfig.path.c_str();
        if (NULL == dirPath)
        {
            break;
        }

        DIR* dir = opendir(dirPath);

        time_t now_t = Timer::get_unix_timestamp();

        struct dirent* ptr;

        while ((ptr = readdir(dir)) != NULL)
        {
            if (ptr == NULL) //�ſ�
            {
                continue;
            }

            if (strcmp(ptr->d_name, ".") == 0 || 0 == strcmp(ptr->d_name, ".."))
            { //�ų�Ŀ¼���ϼ�Ŀ¼
                continue;
            }

            const std::string fileName = ptr->d_name;

            if (ptr->d_type != DT_REG) //ֻ�������ļ�
            {
                continue;
            }

            if (!fileName.find_last_of(this->logConfig.suffix)) //δ�ҵ���ͬ��׺
            {
                continue;
            }
            std::string cFilePath = "";
            cFilePath.append(dirPath).append(ptr->d_name);

            time_t file_t = Files::get_modify_time(cFilePath.c_str());

            Logger::getInstance()->debug("fileName.find_first_of,file_name:%s,time:%ld.", ptr->d_name, file_t);

            if (now_t - file_t > 604800) //604800��7���������������ɾ������7����ļ�
            {
                remove(cFilePath.c_str());
                Logger::getInstance()->debug("fileName.find_first_of,file_name:%s,time:%ld.", ptr->d_name, file_t);
            }


        }
        closedir(dir);


    } while (0);
}

/******************************************************************************
*  Name        :   Logger::Logger
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.07.28
*  Describe    :  ��־��������
******************************************************************************/
Logger* Logger::getInstance()
{
    if (m_instance_ptr == NULL)
    {
        pthread_mutex_lock(&mutex);
        if (m_instance_ptr == NULL)
        {
            m_instance_ptr = new Logger();
        }
        pthread_mutex_unlock(&mutex);
    }

    return m_instance_ptr;
}

pthread_mutex_t Logger::mutex;
Logger* Logger::m_instance_ptr = NULL;