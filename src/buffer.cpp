#include "buffer.h"
#include "assert.h"
#include "string.h"
#include "logwrap.h"

namespace DURIANVER
{

Buffer::Buffer()
{
    buf_.resize(1024);
    readIndex_ = 0;
    writeIndex_ = 0;
}

Buffer::~Buffer()
{
}

int Buffer::append(const void *data, int len)
{
    if (len > size() - writeIndex_)
    {
        if (len < (size() - (writeIndex_ - readIndex_)))
        {
            moveToFirst();
        }
        else
        {
            buf_.resize(writeIndex_ + len);
        }
    }
    memcpy(buf_.data(), data, len);
    return 0;
}

std::string Buffer::retriveAsString(){
    std::string res(buf_.data()+readIndex_,writeIndex_-readIndex_);
    readIndex_=0;
    writeIndex_=0;
    return res;
}

void Buffer::moveToFirst()
{
    assert(readIndex_ <= writeIndex_);
    memcpy(buf_.data(), buf_.data() + readIndex_, writeIndex_ - readIndex_);
}
}