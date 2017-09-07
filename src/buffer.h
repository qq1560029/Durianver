#pragma once

#include <vector>
#include <string>

namespace DURIANVER
{
class Buffer
{
  public:
    Buffer();
    ~Buffer();

    int size() { return buf_.size(); }
    char *wPtr() { return buf_.data() + writeIndex_; }
    int writable() { return buf_.size() - writeIndex_; }
    int append(const void *data, int len);
    std::string retriveAsString();
    char *retrive(int len);

  private:
    void moveToFirst();
    std::vector<char> buf_;
    int readIndex_;
    int writeIndex_;
};
}