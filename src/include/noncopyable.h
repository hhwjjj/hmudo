
/*
noncopyable派生类无法拷贝和赋值
*/
#pragma once
class noncopyable{
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;   
protected:
    noncopyable()=default;
    ~noncopyable()=default;
};