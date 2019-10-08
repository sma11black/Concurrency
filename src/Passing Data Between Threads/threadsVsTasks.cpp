#include <iostream>
#include <thread>
#include <future>
#include <cmath>
#include <memory>

double divideByNumber(double num, double denom)
{
	std::cout << "Worker thread id = " << std::this_thread::get_id() << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	if (denom == 0)
		throw std::runtime_error("Exception from thread: Division by zero!");
	
	return num / denom;
}

void workerFunction(int n)
{
	std::cout << "Worker thread id = " << std::this_thread::get_id() << std::endl;
	for (int i = 0; i < n; ++i)
	{
		sqrt(12345.6789);
	}
}

int main()
{
	// Example 1
	std::cout << "Example 1:" << std::endl;
	// use async to start a task
	double num = 42.0, denom = 2.0;
	std::future<double> ftr1 = std::async(divideByNumber, num, denom);
	// retrieve result within try-catch-block
	try
	{
		double result = ftr1.get();
		std::cout << "Result = " << result << std::endl;
	}
	catch (std::runtime_error e)
	{
		std::cout << e.what() << std::endl;
	}

	// Example 2
	std::cout << "\nExample 2:" << std::endl;
	std::cout << "Main thread id = " << std::this_thread::get_id() << std::endl;
	std::future<double> ftr2 = std::async(std::launch::deferred, divideByNumber, num, denom);
	try
	{
		double result = ftr2.get();
		std::cout << "Result = " << result << std::endl;
	}
	catch (std::runtime_error e)
	{
		std::cout << e.what() << std::endl;
	}

	// Example 3
	std::cout << "\nExample 3:" << std::endl;
	std::cout << "Main thread id = " << std::this_thread::get_id() << std::endl;
	// start time measurement
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	// launch various tasks
	std::vector<std::future<void>> futures;
	// there is a lot of overhead involved with creating and managing threads.
	// so only when nLoops is large, "async" method runtime performance is better than "deffered" method.
	int nLoops = 100000000, nThreads = 5;
	for (int i = 0; i < nThreads; ++i)
	{
		//futures.emplace_back(std::async(std::launch::any, workerFunction, nLoops));	// decided by system. (any:not work?)
		futures.emplace_back(std::async(std::launch::async, workerFunction, nLoops));
		//futures.emplace_back(std::async(std::launch::deferred, workerFunction, nLoops));
	}
	// wait for tasks to complate
	for (const std::future<void> &ftr : futures)
		ftr.wait();

	// stop time measurement and print execution time
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	std::cout << "Execution finished after " << duration << " microseconds" << std::endl;

	return 0;
}
