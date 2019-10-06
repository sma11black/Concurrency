#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

void printHello()
{
	std::cout << "Hello from Worker thread #" << std::this_thread::get_id() << std::endl;
}

int main()
{
	// Example 1
	std::cout << "Example 1:" << std::endl;
	std::vector<std::thread> threads1;

	for (size_t i = 0; i < 5; ++i)
	{
		// copying thread objects causes a compile error		
		//std::thread t(printHello);
		//threads.push_back(t);
		
		// moving thread objects will work
		threads1.emplace_back(std::thread(printHello));
	}
	
	std::cout << "Hello from Main thread #" << std::this_thread::get_id() << std::endl;
	for (auto &t : threads1)
	{
		t.join();
	}

	// Example 2
	std::cout << "\nExample 2:" << std::endl;
	std::vector<std::thread> threads2;

	for (size_t i = 0; i < 10; ++i)
	{
		threads2.emplace_back([i]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(10 * i));
			std::cout << "Hello from Worker thread #" << i << std::endl;
		});
	}
	std::cout << "Hello from Main thread" << std::endl;

	for (auto &t : threads2)
	{
		t.join();
	}

	return 0;
}
