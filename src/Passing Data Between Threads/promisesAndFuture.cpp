#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <cmath>
#include <memory>

void printMessage(std::string message)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	std::cout << "Thread 1: " << message << std::endl;
}

void modifyMessage(std::promise<std::string> && prms, std::string message)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(4000));
	std::string modifiedMessage = message + " has been modified";
	prms.set_value(modifiedMessage);
}

void computeSqrt(std::promise<double> &&prms, double input)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	double output = sqrt(input);
	prms.set_value(output);
}

void divideByNumber(std::promise<double> &&prms, double num, double denom)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	try
	{
		if (denom == 0)
			throw std::runtime_error("Exception from thread: Division by zero!");
		else
			prms.set_value(num / denom);
	}
	catch (...)
	{
		prms.set_exception(std::current_exception());
	}
}

int main()
{
	// Example 1
	std::cout << "Example 1:" << std::endl;
	// define message
	std::string message = "My Message";
	// start thread using variadic templates
	std::thread t1(printMessage, message);
	// start thread using a Lambda
	std::thread t2([message] {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		std::cout << "Thread 2: " << message << std::endl;
	});
	// thread barrier
	t1.join();
	t2.join();

	// Example 2
	std::cout << "\nExample 2:" << std::endl;
	std::string messageToThread = "My Message";
	// create promise and future
	std::promise<std::string> prms1;
	std::future<std::string> ftr1 = prms1.get_future();
	// start thread and pass promise as argument
	std::thread t3(modifyMessage, std::move(prms1), messageToThread);
	// print original message to console
	std::cout << "Original message from main(): " << messageToThread << std::endl;
	// retrieve modified message via future and print to console
	std::string messageFromThread = ftr1.get();
	std::cout << "Modified message from thread(): " << messageFromThread << std::endl;
	// thread barrier
	t3.join();

	// Example 3
	std::cout << "\nExample 3:" << std::endl;
	double inputData = 42.0;
	std::promise<double> prms2;
	std::future<double> ftr2 = prms2.get_future();
	std::thread t4(computeSqrt, std::move(prms2), inputData);
	// wait for result to become available
	auto status = ftr2.wait_for(std::chrono::milliseconds(1000));
	if (status == std::future_status::ready)	// result is ready
	{
		std::cout << "Result = " << ftr2.get() << std::endl;
	}
	// timeout has expired or function has not yet been started
	else if (status == std::future_status::timeout || status == std::future_status::deferred)
	{
		std::cout << "Result unavailable" << std::endl;
	}
	t4.join();

	// Example 4
	std::cout << "\nExample 4:" << std::endl;
	std::promise<double> prms3;
	std::future<double> ftr3 = prms3.get_future();
	double num = 42.0, denom = 0.0;
	std::thread t5(divideByNumber, std::move(prms3), num, denom);
	// retrieve result within try-catch-block
	try
	{
		double result = ftr3.get();
		std::cout << "Result = " << result << std::endl;
	}
	catch (std::runtime_error e)
	{
		std::cout << e.what() << std::endl;
	}
	t5.join();

	return 0;
}