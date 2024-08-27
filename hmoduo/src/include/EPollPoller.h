#pragma once
#include <vector>
#include "sys/epoll.h"
#include "Poller.h"
#include "Timestamp.h"
#include "Channel.h"


class EPollPoller : public Poller
{
public:
    EPollPoller(EventLoop* loop);
    ~EPollPoller() override;

    Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;
    void updateChannel(Channel* channel) override;
    void removeChannel(Channel* channel) override;
private:
    static const int kInitEventListSize = 16;
    
    void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
    void update(int opreation,Channel* channel);

    using EventList = std::vector<epoll_event>;

    int epollfd_;
    EventList events_;
};