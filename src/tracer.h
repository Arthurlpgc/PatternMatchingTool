#include <chrono>
#include <iostream>
#ifndef tracerdef
#define tracerdef
struct Tracer {
    long long timed[10] = {0};
    std::chrono::time_point<std::chrono::system_clock> start_time;
    int started = 9;
    void start(int i) {
        timed[started] += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start_time).count();
        started = i;
        start_time = std::chrono::system_clock::now();
    }
    void finish() {
        start(9);
        for(int i = 0; i < 10; i++)
            std::cout << i << " = " << timed[i] << std::endl;
    }
};
#endif