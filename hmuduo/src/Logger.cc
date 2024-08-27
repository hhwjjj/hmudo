#include "Logger.h"
#include"Timestamp.h"
#include<iostream>
Logger& Logger::instance(){
    static Logger logger;
    return logger;
}
void Logger::setLogerLevel(int level){
    loglevel_ = level;
}

void Logger::log(std::string msg){
    switch (loglevel_)
    {
    case INFO:
           std::cout<<"[INFO]";
        break;
    case ERROR:
           std::cout<<"[ERRO]";
        break;
    case FATAL:
           std::cout<<"[FATAL]";
        break;
    case DEBUG:
           std::cout<<"[DEBUG]";
        break;
    default:
        break;
    }
    std::cout<<Timestamp::now().tostring()<<" :"<<msg<<std::endl;
}