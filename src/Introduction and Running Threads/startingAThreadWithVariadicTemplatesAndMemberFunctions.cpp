#include <iostream>
#include <string>
#include <thread>

void printID(int id)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	std::cout << "ID = " << id << std::endl;
}

void printIDAndName(int id, std::string name)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "ID = " << id << ", name = " << name << std::endl;
}

void printName(std::string name, int waitTime)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
	std::cout << "Name (from Thread) = " << name << std::endl;
}

void printNameRef(std::string &name, int waitTime)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
	name += " (from Thread)";
	std::cout << name << std::endl;
}

class Vehicle
{
public:
	Vehicle() {
		_id = 0;
		_name = "";
	}
	void addID(int id) {
		_id = id;
	}
	void setName(std::string name) {
		_name = name;
	}
	void printID() {
		std::cout << "Vehicle ID = " << _id << std::endl;
	}
	void printName() {
		std::cout << "Vehicle Name = " << _name << std::endl;
	}

private:
	int _id;
	std::string _name;
};

int main()
{
	// Example 1
	std::cout << "Example 1:" << std::endl;
	int id = 0;
	std::thread t1(printID, id);
	std::thread t2(printIDAndName, ++id, "MyString");
	// std::thread t3(printIDAndName, ++id);  // this procudes a compiler error
	t1.join();
	t2.join();
	// t3.join();

	// Example 2
	std::cout << "\nExample 2:" << std::endl;
	std::string name1 = "MyThread1";
	std::string name2 = "MyThread2";
	std::thread t3(printName, name1, 50);
	std::thread t4(printName, std::move(name2), 100);
	t3.join();
	t4.join();
	std::cout << "Name (from Main) = " << name1 << std::endl;
	std::cout << "Name (from Main) = " << name2 << std::endl;

	// Example 3
	std::cout << "\nExample 3:" << std::endl;
	std::string name3("MyThread");
	std::thread t5(printNameRef, std::ref(name3), 50);
	t5.join();
	name3 += " (from Main)";
	std::cout << name3 << std::endl;

	// Example 4
	std::cout << "\nExample 4:" << std::endl;
	Vehicle v1, v2;
	std::thread t6 = std::thread(&Vehicle::addID, v1, 1);
	std::thread t7 = std::thread(&Vehicle::addID, &v2, 2);
	t6.join();
	t7.join();
	v1.printID();
	v2.printID();

	// Example 5
	std::cout << "\nExample 5:" << std::endl;
	std::shared_ptr<Vehicle> v3(new Vehicle);
	std::thread t8 = std::thread(&Vehicle::addID, v3, 1);
	t8.join();
	v3->printID();

	// Quiz
	std::cout << "\nQuiz:" << std::endl;
	std::shared_ptr<Vehicle> v4(new Vehicle);
	std::thread t9 = std::thread(&Vehicle::addID, v4, 1);
	std::thread t10 = std::thread(&Vehicle::setName, v4, "Vehicle");
	t9.join();
	t10.join();
	v4->printID();
	v4->printName();

	return 0;
}
