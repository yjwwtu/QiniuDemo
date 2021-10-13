#include <cstdio>
#include <qiniu/io.h>
#include <qiniu/rs.h>
#include <qiniu/http.h>
#include "app/qiniu/qiniu.h"



int main()
{
    printf("%s 向你问好!\n", "QiniuDemo");

    Qiniu::init_global(); //初始化七牛云
    Logger::getInstance()->info("------------------init qiniu success---------------");

    std::string key = "test/";
    key.append(std::to_string(Timer::get_unix_timestamp()));
    key.append(".txt");

    std::string file_path = "test.txt";

    int ret = Qiniu::upload(key.c_str(), file_path.c_str());

    printf("七牛云上传情况!%d \n", ret);
    return 0;
}
