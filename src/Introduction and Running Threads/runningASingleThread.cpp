#include <iostream>
#include <thread>

void threadFunction3()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));	// simulate work
	std::cout << "Finished work in thread.\n";
}

void threadFunction4()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	std::cout << "Finished work 1 in thread.\n";

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	std::cout << "Finished work 2 in thread.\n";
}

void threadFunction5()
{
	threadFunction4();
}

void threadFunction6()
{
	threadFunction3();
}

void threadFunctionEven()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	std::cout << "Even thread.\n";
}

void threadFunctionOdd()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	std::cout << "Odd thread.\n";
}

int main()
{
	// Example 1
	std::cout << "Example 1:" << std::endl;
	std::cout << "Hello concurrent world from main! Thread id = "
		<< std::this_thread::get_id() << std::endl;

	// Example 2
	std::cout << "\nExample 2:" << std::endl;
	unsigned int nCores = std::thread::hardware_concurrency();
	std::cout << "This machine supports concurrency with " << nCores
		<< " cores available." << std::endl;

	// Example 3
	std::cout << "\nExample 3:" << std::endl;
	std::thread t3(threadFunction3);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	std::cout << "Finished work in main.\n";
	// wait for thread to finish
	t3.join();

	// Example 4
	std::cout << "\nExample 4:" << std::endl;
	std::thread t4(threadFunction4);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	std::cout << "Finished work 1 in main.\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	std::cout << "Finished work 2 in main.\n";
	t4.join();

	// Example 5
	std::cout << "\nExample 5:" << std::endl;
	std::thread t5(threadFunction5);
	t5.join();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	std::cout << "Finished work 1 in main.\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	std::cout << "Finished work 2 in main.\n";

	// Example 6
	std::cout << "\nExample 6:" << std::endl;
	std::thread t6(threadFunction6);
	// detach thread and continue with main
	t6.detach();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	std::cout << "Finished work in main.\n";

	// Student Task
	std::cout << "\nStudent Task:" << std::endl;
	for (int i = 0; i < 6; ++i)
	{
		if (i % 2 == 0)
		{
			std::thread t(threadFunctionEven);
			t.detach();
		}
		else
		{
			std::thread t(threadFunctionOdd);
			t.detach();
		}
	}

	// ensure that main does not return before the threads are finished
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	std::cout << "End of main is reached." << std::endl;

	return 0;
}