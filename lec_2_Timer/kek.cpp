#include <chrono>
#include <iostream>

class Timer{
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds = std::chrono::microseconds;
public:
    Timer()
        : start_(clock_t::now())
    {
    }

    ~Timer(){
        const auto finish = clock_t::now();
        const auto us = 
            std::chrono::duration_cast<microseconds>
                (finish - start_).count();
        std::cout << us << " us" << std::endl;
    }

private:
    const clock_t::time_point start_;
};


int main(){
int N = 10000;

int** matrix = new int*[N];
for(size_t i = 0 ; i < N ; ++i)
    matrix[i] = new int[N];


int s = 0;

{
    Timer t;

    volatile int s = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
             s += matrix[i][j];
    
}


{
    Timer t;

    volatile int s = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
             s += matrix[j][i];

}

return 0;

}