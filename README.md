# hmudo

基于C++11实现    取消muduo对于Boost依赖

开发环境：Ubuntu VSCode

编译器：g++

编程语言：C++

# 编译

bin/autobuild.sh

# 测试

一个简单的回显服务器用于测试。在example目录。

# 压力测试

可参考

 https://blog.csdn.net/Solstice/article/details/5864889

# 项目概述

学习muduo源码后的简单练习

## 设计

Reactor模型：

- Event：事件
- Reactor： 反应堆
- Demultiplex：多路事件分发器
- EventHandler：事件处理器

EventLoop其实就是reactor，其执行在一个Thread上，实现了one loop per thread的设计。 每个EventLoop中，有一个Poller和很多的Channel，Poller就是demultiplex , 而Channel对应的就是event

## 类

### 辅助类

##### noncopyable

不可拷贝的基类，所有类都继承此类

##### TimeStamp

提供系统时间

##### Logger

提供四个日志等级 INFO、ERROR、FATAL、DEBUG

##### Buffer

模仿Java中的netty设计

prepend、read、write三个标志位划分缓冲区的数据

#### Reactor相关类

##### InetAddress

封装socket所需ip地址和端口号

##### Channel

封装sockfd及其所感兴趣的事件，发生事件调用的回调函数

##### EpollPoller

封装epoll，即底层demultiplex，包含一个指向Channel的指针，以及内核事件表中的fd

##### EventLoop

事件循环类  Poller + Channel

##### Thread

直接使用C++11中的thread类

##### EventLoopThread

one loop per thread

一个线程一个eventloop

##### EventLoopThreadPool

subreactor池

设置thread_nums创建相应数量的subreactor

#### mainReactor

##### Socket

封装socket 与Channel不同的是，其封装的是socket编程流程bind、listen、accept及socket属性信息等

##### Acceptor

mainreactor 分发客户端连接

#### TcpServer

网络库入口

轮询算法选择一个sub reactor

根据连接成功的sockfd创建一个连接对象并放入map中

给连接设置相应回调

#### TcpConnection

将底层包装给用户使用
