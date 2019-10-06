#include <iostream>
#include <thread>

class Vehicle
{
public:
	void operator()() {
		std::cout << "Vehicle object has been created.\n";
	}
};

class Vehicle3
{
public:
	Vehicle3(int id) {
		_id = id;
	}
	void operator()() {
		std::cout << "Vehicle #" << _id << " has been created." << std::endl;
	}

private:
	int _id;
};

int main()
{
	// Vehicle()(); // Vehicle(): FunctionObjects; Vehicle()(): Function

	// Example 1 !!!
	std::cout << "Example 1:" << std::endl;
	std::thread t0((Vehicle()));	// C++'s most vexing parse!!!
	std::cout << "Finished work in main.\n";
	t0.join();

	// Example 2
	std::cout << "\nExample 2:" << std::endl;
	std::thread t1((Vehicle()));	// Add an extra pair of parentheses
	std::thread t2 = std::thread(Vehicle());	// Use copy initialization
	std::thread t3{ Vehicle() };	// Use uniform initialization with braces
	std::cout << "Finished work in main.\n";
	t1.join();
	t2.join();
	t3.join();

	// Example 3
	std::cout << "\nExample 3:" << std::endl;
	std::thread t = std::thread(Vehicle3(1));
	std::cout << "Finished work in main.\n";
	t.join();

	// Example 4 & 5
	std::cout << "\nExample 4 & 5:" << std::endl;
	std::cout << "See lambdaFunction.cpp" << std::endl;

	// Example 6
	std::cout << "\nExample 6:" << std::endl;
	int id = 0;
	auto f0 = [&id]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		std::cout << "a) ID in Thread (call-by-reference) = " << id << std::endl;
	};
	std::thread t4(f0);
	std::thread t5([id]() mutable {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		std::cout << "b) ID in Thread (call-by-value) = " << id << std::endl;
	});
	++id;
	std::cout << "c) ID in Main (call-by-value) = " << id << std::endl;
	t4.join();
	t5.join();

	return 0;
}
