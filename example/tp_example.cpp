#include <iostream>
#include <random>

#include "../include/Thread_Pool.h"

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<int> dist(-1000, 1000);
auto rnd = std::bind(dist, mt);

void simulate_hard_computation()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2000 + rnd()));
}

// Function that multiples two numbers and prints the result
void multiply(const int a, const int b)
{
    simulate_hard_computation();

    const int res = a * b;
    std::cout << a << " * " << b << " = " << std::endl;
}

// Same as above, now with an output parameter
void multiply_output(int & out, const int a, const int b)
{
    simulate_hard_computation();

    out = a * b;
    std::cout << a << " * " << b << " = " << out << std::endl;
}

// Identical to above two, now with a return type included
int multiply_return(const int a, const int b)
{
    simulate_hard_computation();

    const int res = a * b;
    std::cout << a << " * " << b << " = " << res << std::endl;
    return res;
}

void tp_example()
{
    // Create pool with 3 threads
    Thread_Pool pool(3);

    // Initialize pool
    pool.init();

    // Submit (partial) multiplication table
    for (int i = 1; i < 3; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            pool.submit(multiply, i, j);
        }
    }

    // Submit function with an output parameter passed by ref
    int output_ref;
    auto future1 = pool.submit(multiply_output, std::ref(output_ref), 5, 6);

    // Wait for multiplication output to finalize
    future1.get();
    std::cout << "Last operation result is equal to " << output_ref << std::endl;

    // Submit function with return type parameter
    auto future2 = pool.submit(multiply_return, 5, 3);

    // Wait for multiplication output to finalize
    int res = future2.get();
    std::cout << "Last operation result is equal to " << res << std::endl;

    pool.shutdown();
}