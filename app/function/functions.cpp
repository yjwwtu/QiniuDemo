#include "functions.h"



/******************************************************************************
*  Name        :   Timer::getUnixTimestamp
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2020.12.17
*  Describe    :   ��ȡunixʱ���
******************************************************************************/
time_t Timer::get_unix_timestamp()
{
    try
    {
        const time_t t = time(NULL);

#ifdef DEBUG
        std::cout << "sizeof(time_t) is: " << sizeof(time_t) << std::endl;
#endif

        return t;
    }
    catch (std::exception& e)
    {
    }


    return -1;
}


/******************************************************************************
*  Name        :   Timer::getUnixTimestamp
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2020.12.17
*  Describe    :   ��ȡunix����ʱ���
******************************************************************************/
std::string Timer::get_current_date_str()
{
    char date[9] = { 0 };

    try
    {
        const time_t t = time(NULL);
        strftime(date, sizeof(date), "%Y%m%d", localtime(&t)); //������
    }
    catch (...)
    {
        for (int i = 0; i < 9; i++)
        {
            date[i] = { 0 };
        }
    }

    return date;
}

/******************************************************************************
 *  Name        :   Timer::format_datetime
 *  Author      :   cqnews
 *  Version     :   V1.0.0
 *  Data        :   2021.04.13
 *  Describe    :   ����תʱ���
 ******************************************************************************/
time_t Timer::format_datetime(int uiYear, int uiMonth, int uiDay, int uiHour, int uiMinute, int uiSecond)
{
    printf("format_datetime,uiYear:%d\n", uiYear);

    tm t;                                    // ����tm�ṹ�塣

    t.tm_year = uiYear - 1900;                 // �꣬����tm�ṹ��洢���Ǵ�1900�꿪ʼ��ʱ�䣬����tm_yearΪint��ʱ������ȥ1900��
    t.tm_mon = uiMonth - 1;                    // �£�����tm�ṹ����·ݴ洢��ΧΪ0-11������tm_monΪint��ʱ������ȥ1��
    t.tm_mday = uiDay;                         // �ա�
    t.tm_hour = uiHour;                        // ʱ��
    t.tm_min = uiMinute;                       // �֡�
    t.tm_sec = uiSecond;                       // �롣
    t.tm_isdst = 0;                          // ������ʱ��
    time_t t_ = mktime(&t);                  // ��tm�ṹ��ת����time_t��ʽ��

    return t_;                                 // ����ֵ��
}


/******************************************************************************
*  Name        :   Files::exists
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.05.07
*  Describe    :   ����ļ��Ƿ����
******************************************************************************/
bool Files::exists_local_file(const char* name)
{
    try
    {

        struct stat buffer;
        return (stat(name, &buffer) == 0);

    }
    catch (...)
    {

    }

    return false;
}

/******************************************************************************
*  Name        :   Files::delete_local_file
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2020.12.18
*  Describe    :   ɾ�������ļ�
******************************************************************************/
int Files::delete_local_file(const char* localFile)
{
    try
    {
        return remove(localFile);
    }
    catch (...)
    {
    }

    return -1;
}

/******************************************************************************
*  Name        :   Files::delete_expire_local_file
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.09.06
*  Describe    :   ɾ�����س�ʱ�ļ�
******************************************************************************/
int Files::delete_expire_local_file(const char* dirPath, int second)
{
    try
    {
        if (NULL == dirPath)
        {
            return -1;
        }

        //��Ŀ¼
        DIR* dir = opendir(dirPath);

        time_t now_t = Timer::get_unix_timestamp();
        struct dirent* ptr;

        while ((ptr = readdir(dir)) != nullptr)
        {
            if (ptr == nullptr) //�ſ�
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

            std::string cFilePath = "";
            cFilePath.append(dirPath).append(ptr->d_name);

            time_t file_t = Files::get_modify_time(cFilePath.c_str());
            Logger::getInstance()->debug("fileName.find_first_of,file_name:%s,time:%ld.", ptr->d_name, file_t);

            if (now_t - file_t > second) //604800��7���������������ɾ������7����ļ�
            {
                remove(cFilePath.c_str());
                Logger::getInstance()->debug("fileName.find_first_of,file_name:%s,time:%ld.", ptr->d_name, file_t);
            }

        }
    }
    catch (...)
    {

    }

    return 0;

}

/******************************************************************************
*  Name        :   Files::delete_local_file
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.06.07
*  Describe    :   ��ȡ�ļ����޸�ʱ��
******************************************************************************/
time_t Files::get_modify_time(const char* filePath)
{

    FILE* fp;

    fp = fopen(filePath, "r");

    if (!fp)
    {
        return 0;
    }

    time_t t = get_modify_time(fp);
    fclose(fp);

    return t;
}




/******************************************************************************
*  Name        :   Files::delete_local_file
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.05.11
*  Describe    :   ��ȡ�ļ����޸�ʱ��
******************************************************************************/
time_t Files::get_modify_time(FILE* fp)
{
    struct stat buf;
    int fd = fileno(fp);

    fstat(fd, &buf);
    long modify_time = buf.st_mtime;
    //std::cout << "file: "<< buf.st_size <<",last modify time:" << modify_time << std::endl;

    return modify_time;
}

/******************************************************************************
*  Name        :   Files::get_st_size
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.05.11
*  Describe    :   ��ȡ�ļ��Ĵ�С
******************************************************************************/
long Files::get_st_size(const char* filePath)
{
    struct stat info;
    stat(filePath, &info);
    long size = info.st_size;
    return size;
}

/******************************************************************************
*  Name        :   Files::move
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.09.01
*  Describe    :   �ƶ��ļ���,ģ��mv
******************************************************************************/
int Files::move2(const char* sourcePath, const char* targetPath, bool isDelete)
{

    FILE* source = fopen(sourcePath, "r");

    if (source == NULL)
    {
        perror("file no exsit!\n");
        return -1;
    }

    struct stat* statbuf = (struct stat*)malloc(sizeof(statbuf));
    stat(targetPath, statbuf); //�ж�Ŀ��·����һ����ͨ�ļ�����һ��Ŀ¼
    if (S_ISDIR(statbuf->st_mode)) //Ŀ���Ǹ�Ŀ¼
    {
        fclose(source); //�ر�Դ�ļ�
        return -2; //Ŀ���Ǹ�Ŀ¼
    }

    FILE* target = fopen(targetPath, "at");

    while (!feof(source))
    {//��Դ�ļ�����ȫ������Ŀ���ļ�
        char  buf[10] = {};
        fread(buf, 1, 10, source);
        fwrite(buf, 1, 10, target);
    }

    fclose(target);
    fclose(source);

    if (isDelete)
    {
        Logger::getInstance()->debug("isDelete");
    }

    return 0;

}

/******************************************************************************
*  Name        :   Files::getpwdpath
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.09.22
*  Describe    :   ��ȡ��ǰĿ¼·��
******************************************************************************/
int Files::getpwdpath(char* buffer)
{
    // Get the current working directory:
    if ((buffer = getcwd(NULL, 0)) == NULL)
    {
        perror("_getcwd error");
        return -1;
    }
    else
    {
        printf("%s \nLength: %d\n", buffer, strlen(buffer));
        free(buffer);
    }

    return 0;
}

/******************************************************************************
 *  Name        :   Function::executeCMD
 *  Author      :   cqnews
 *  Version     :   V1.0.0
 *  Data        :   2020.07.30
 *  Describe    :   ִ���ն�����
 ******************************************************************************/
void Function::systemCMD(const char* cmd)
{
    //char result[1024] = {0};
    //if (cmd == nullptr)
    //{
    //    return;
    //}

    //Function::executeCMD(cmd, result);
    system(cmd);
}

/******************************************************************************
 *  Name        :   executeCMD
 *  Author      :   cqnews
 *  Version     :   V1.0.0
 *  Data        :   2020.07.30
 *  Describe    :   ִ���ն�����
 ******************************************************************************/
void Function::executeCMD(const char* cmd, char* result)
{
    FILE* ptr;
    char ps[1024] = { 0 }, buf_ps[1024] = { 0 };

    strcpy(ps, cmd);
    if ((ptr = popen(ps, "r")) != NULL)
    {
        while (fgets(buf_ps, 1024, ptr) != NULL)
        {
            strcat(result, buf_ps);
            if (strlen(result) > 1024)
            {
                break;
            }
        }
        pclose(ptr);
        ptr = NULL;
    }
    else
    {
        printf("popen %s error\n", ps);
    }
}



/******************************************************************************
 *  Name        :   Function::get_current_time
 *  Author      :   cqnews
 *  Version     :   V1.0.0
 *  Data        :   2021.04.14
 *  Describe    :   ��ȡϵͳʱ��
 ******************************************************************************/
void Function::get_current_time()
{
    char time_buf[50] = { 0 };

    time_t t = time(NULL);
    struct tm* tm = localtime(&t);

    snprintf(time_buf, 50 - 1, "%04d/%02d/%02d %02d-%02d-%02d", (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour,
        tm->tm_min, tm->tm_sec);
    cout << time_buf << ":";
}


/******************************************************************************
 *  Name        :   ping_online_device
 *  Author      :   xiao
 *  Version     :   V1.0.0
 *  Data        :   2021.02.02
 *  Describe    :   ping�豸�Ƿ�����
 ******************************************************************************/
int Function::ping_online_device(const char* ipAddress)
{
    pid_t pid;

    if ((pid = vfork()) < 0)
    {
        printf("vfork error");
        exit(1);
    }
    else if (pid == 0)
    {
        if (execlp("ping", "ping", "-c", "1", ipAddress, (char*)0) < 0)
        {
            printf("execlp error\n");
            exit(1);
        }
    }

    int stat;
    waitpid(pid, &stat, 0);

    if (stat == 0)
    {
        return 1;
    }

    return 0;
}


/******************************************************************************
*  Name        :   Function::delay
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2020.12.17
*  Describe    :   ��ȡunixʱ���
*  time        :   �ӳ�ʱ�� time*1000Ϊ����
******************************************************************************/
void Function::delay(const int time)
{
    clock_t   now = clock();
    while (clock() - now < time);
}



/******************************************************************************
*  Name        :   Function::get_current_hour
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.06.24
*  Describe    :   ��ȡ��ǰСʱ��
******************************************************************************/
int Function::get_current_hours()
{
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);

    return tm->tm_hour;
}

/******************************************************************************
*  Name        :   Function::get_local_ip
*  Author      :   cqnews
*  Version     :   V1.0.0
*  Data        :   2021.09.08
*  Describe    :   ��ȡ����ip��ַ
******************************************************************************/
int Function::get_local_ip(const char* eth_inf, char* ip)
{
    int sd;
    struct sockaddr_in sin;
    struct ifreq ifr;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sd)
    {
        printf("socket error: %s\n", strerror(errno));
        return -1;
    }

    strncpy(ifr.ifr_name, eth_inf, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;

    // if error: No such device
    if (ioctl(sd, SIOCGIFADDR, &ifr) < 0)
    {
        printf("ioctl error: %s\n", strerror(errno));
        close(sd);
        return -1;
    }

    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
    snprintf(ip, IP_SIZE, "%s", inet_ntoa(sin.sin_addr));

    close(sd);

    return 0;
}

/******************************************************************************
 *  Name        :   get_local_gateway
 *  Author      :   cqnews
 *  Version     :   V1.0.0
 *  Data        :   2021.09.08
 *  Describe    :   ping�豸�Ƿ�����
 ******************************************************************************/
int Function::get_local_gateway(char* ip)
{
    FILE* fp;
    char buf[512];
    char cmd[128];
    char* tmp;

    strcpy(cmd, "ip route");
    fp = popen(cmd, "r");
    if (NULL == fp)
    {
        perror("popen error");
        return -1;
    }
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        tmp = buf;
        while (*tmp && isspace(*tmp))
            ++tmp;
        if (strncmp(tmp, "default", strlen("default")) == 0)
            break;
    }
    sscanf(buf, "%*s%*s%s", ip);
    printf("default gateway:%s/n", ip);
    pclose(fp);

    return 0;
}