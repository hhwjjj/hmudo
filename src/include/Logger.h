#pragma once
#include "noncopyable.h"
#include <string>

#define LOG_INFO(logmsgFormat,...)\
    do\
    {\
        Logger &logger = Logger::instance();\
        logger.setLogerLevel(INFO);\
        char buf[1024] = {0};\
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);\
        logger.log(buf);\
    } while (0);
#define LOG_ERROR(logmsgFormat,...)\
    do\
    {\
        Logger &logger = Logger::instance();\
        logger.setLogerLevel(ERROR);\
        char buf[1024] = {0};\
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);\
        logger.log(buf);\
    } while (0);
#define LOG_FATAL(logmsgFormat,...)\
    do\
    {\
        Logger &logger = Logger::instance();\
        logger.setLogerLevel(FATAL);\
        char buf[1024] = {0};\
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);\
        logger.log(buf);\
        exit(0);\
    } while (0);
#ifdef MUDEBUG
#define LOG_DEBUG(logmsgFormat,...)\
    do\
    {\
        Logger &logger = Logger::instance();\
        logger.setLogerLevel(DEBUG);\
        char buf[1024] = {0};\
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);\
        logger.log(buf);\
    } while (0);
#else
    #define LOG_DEBUG(logmsgFormat,...)
#endif
enum LogLevel{
    INFO,   // 普通信息
    ERROR,  //错误信息
    FATAL,  //core信息
    DEBUG,  //调试信息
};

class Logger : noncopyable{

public:
    //输出一个日志实例对象
    static Logger& instance();
    //设置日志级别
    void setLogerLevel(int level);
    //写日志
    void log(std::string msg);
private:
    int loglevel_;
    Logger(){};

};