#include "EPollPoller.h"
#include "Logger.h"
#include <sys/epoll.h>
#include <errno.h>
#include <strings.h>
#include <unistd.h>
const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;

EPollPoller::EPollPoller(EventLoop* loop)
        :Poller(loop)
        ,epollfd_(::epoll_create1(EPOLL_CLOEXEC))
        ,events_(kInitEventListSize)
{
    if(epollfd_<0){
        LOG_FATAL("EPollPoller error %d \n",errno);
    }
}
EPollPoller::~EPollPoller() 
{
    ::close(epollfd_);
}

Timestamp EPollPoller::poll(int timeoutMs, ChannelList* activeChannels)
{
    LOG_INFO("EPollPoller::poll() => fd total count : %lu\n",channels_.size());
    int numEvents = ::epoll_wait(epollfd_
                                ,&*events_.begin()
                                ,static_cast<int>(events_.size())
                                ,timeoutMs);
    int saveErrno = errno;
    Timestamp now(Timestamp::now());
    if(numEvents>0){
        LOG_INFO("%d events happend \n",numEvents);
        fillActiveChannels(numEvents,activeChannels);
        if(numEvents == events_.size())
        {
            events_.resize(events_.size()*2);
        }
    }
    else if(numEvents == 0)
    {
        LOG_DEBUG("nothint happended");
    }
    else{
        if(saveErrno!=EINTR)
        {
            errno = saveErrno;
            LOG_ERROR("EPollPoller::poll()");
        }
    }
    return now;
}

void EPollPoller::updateChannel(Channel* channel)
{
    const int index = channel->index();
    LOG_INFO("fd=%d events=%d index=%d \n",channel->fd(),channel->events(),index);
    if(index == kNew || index == kDeleted){
        int fd = channel->fd();
        if(index == kNew)
        {
            channels_[fd] = channel;
        }
        channel->set_index(kAdded);
        update(EPOLL_CTL_ADD,channel);
    }
    else{
        int fd =  channel->fd();
        if(channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL,channel);
            channel->set_index(kDeleted);
        }
    }
}

//从poll中删除channnel
void EPollPoller::removeChannel(Channel* channel)
{
    int fd = channel->fd();

    int index =  channel->index();
    channels_.erase(fd);

    if(index == kAdded)
    {
        update(EPOLL_CTL_DEL,channel);
    }
    channel->set_index(kNew);
}

//填写活跃的连接
void EPollPoller::fillActiveChannels(int numEvents, ChannelList* activeChannels) const
{
    for(int i=0;i<numEvents;++i)
    {
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);

        channel->set_revents(events_[i].events);
        activeChannels->push_back(channel);        
    }
}
//更新channel通道 epoll_ctl add/mod/del
void EPollPoller::update(int opreation,Channel* channel)
{
    epoll_event event;
    bzero(&event,sizeof event);

    int fd = channel->fd();

    event.events = channel->events();
    event.data.fd = fd;
    event.data.ptr = channel;
    

    if(::epoll_ctl(epollfd_,opreation,fd,&event)<0)
    {
        if(opreation == EPOLL_CTL_DEL)
        {
            LOG_ERROR("epoll del erro %d \n",errno);
        }
        else
        {
            LOG_FATAL("epoll del erro %d \n",errno);
        }
    }
}