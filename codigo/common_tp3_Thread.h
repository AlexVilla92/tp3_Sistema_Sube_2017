#ifndef COMMON_TP3_THREAD_H_
#define COMMON_TP3_THREAD_H_

#include <iostream>
#include <thread>

class Thread {
private:
    std::thread thread;
 
public:
    Thread();
    void Start();
    void Join();
    virtual void run() = 0;
    virtual ~Thread();
};

#endif
