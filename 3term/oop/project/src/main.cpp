#include "Application/Application.hpp"
#include "ThreadPool/ThreadPool.hpp"

#include <iostream>

int main() {
//    ng::Application application;
//    application.run();

    ng::ThreadPool threadPool(4);

    for (int i = 0; i < 10; ++i)
        std::cout << threadPool.enqueue([i] { return i; }).get() << std::endl;

    return 0;
}
