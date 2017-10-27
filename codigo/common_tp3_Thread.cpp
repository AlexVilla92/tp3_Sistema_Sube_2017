#include "common_tp3_Thread.h"
#include <thread>

Thread::Thread() {}

void Thread::Start() {
    thread = std::thread(&Thread::run, this);
}

void Thread::Join() {
    thread.join();
}

Thread::~Thread() {} 
