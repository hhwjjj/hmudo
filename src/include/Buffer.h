#pragma once

#include <vector>
#include <string>

class Buffer
{
public:
    static const size_t kCheapPrend = 8;
    static const size_t kInitialSize = 1024;

    explicit Buffer(size_t initilalSize = kInitialSize)
            : buffer_(kCheapPrend + initilalSize)
            , readerIndex_(kCheapPrend)
            , writerIndex_(kCheapPrend)
    {

    }

    size_t readableBytes() const
    { return writerIndex_ - readerIndex_;  }

    size_t writableBytes() const 
    { return buffer_.size() - writerIndex_;  }

    size_t prependableBytes() const
    { return readerIndex_;  }

    const char* peek() const
    {
        return begin() + readerIndex_;
    }

    //onMessage string <- buffer
    void retrieve(size_t len)
    {
        if(len < readableBytes())
        {
            readerIndex_ += len;//应用只读取了缓冲区的一部分
        }
        else{
            retrieveAll();
        }
    }

    void retrieveAll()
    {
        readerIndex_ = kCheapPrend;
        writerIndex_ = kCheapPrend;
    }

    //把onMessage函数上报的Buffer数据 转成string类型的数据返回
    std::string retrieveAllAsString()
    {
       return retrieveAsString(readableBytes());
    }

    std::string retrieveAsString(size_t len)
    {
        std::string result(peek(),len);
        retrieve(len);
        return result;
    }

    void ensureWritableBytes(size_t len)
    {
        if(writableBytes() < len)
        {
            makeSpace(len);
        }
    }

    //把 [data, data+len]内存上的数据 添加到writable缓冲区当中
    void append(const char* data, size_t len)
    {
        ensureWritableBytes(len);
        std::copy(data,data+len,beginWrite());
        writerIndex_ += len;
    }

    char* beginWrite()
    {
        return begin() + writerIndex_;
    }
    const char* beginWrite() const
    { return begin() + writerIndex_;  }

    size_t readFd(int fd, int* savedErrno);
    size_t writeFd(int fd, int* saveErrno);
private:
    char* begin()
    {
        return &*buffer_.begin();
    }
    const char* begin() const
    { return &*buffer_.begin();  }

    void makeSpace(size_t len)
    {
        if(writableBytes() + prependableBytes() < len + kCheapPrend)
        {
            buffer_.resize(writerIndex_ + len);
        }
        else
        {
            size_t readable = readableBytes();
            std::copy( begin() + readerIndex_,
                       begin() + writerIndex_,
                       begin() + kCheapPrend);
            readerIndex_ = kCheapPrend;
            writerIndex_ = readerIndex_ + readable;
        }
    }
    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writerIndex_;
};