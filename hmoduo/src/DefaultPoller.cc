#include "Poller.h"

#include <stdlib.h>
#include "EPollPoller.h"
Poller* Poller::newDefaultPoller(EventLoop* loop)
{
    /*f(::getenv("MUDUO_USE_POLL"))
    {
        return nullptr;
    }
    else{
        return new EPollPoller(loop);
    }*/

    return new EPollPoller(loop);

}