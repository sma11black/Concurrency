#include <iostream>
#include <string>
#include <thread>
#include <future>

class Vehicle1
{
public:
	// default constructor
	Vehicle1() : _id(0), _name(new std::string("Default Name"))
	{
		std::cout << "Vehicle #" << _id << " Default constructor called" << std::endl;
	}
	// initializing constructor
	Vehicle1(int id, std::string name) : _id(id), _name(new std::string(name))
	{
		std::cout << "Vehicle #" << _id << " Initializing constructor called" << std::endl;
	}

	// setter and getter
	void setID(int id) { _id = id; }
	int getID() { return _id; }
	void setName(std::string name) { *_name = name; }
	std::string getName() { return *_name; }

private:
	int _id;
	std::string *_name;
};

class Vehicle2
{
public:
	// default constructor
	Vehicle2() : _id(0), _name(new std::string("Default Name"))
	{
		std::cout << "Vehicle #" << _id << " Default constructor called" << std::endl;
	}
	// initializing constructor
	Vehicle2(int id, std::string name) : _id(id), _name(new std::string(name))
	{
		std::cout << "Vehicle #" << _id << " Initializing constructor called" << std::endl;
	}
	// copy constructor
	Vehicle2(Vehicle2 const &src)
	{
		//...
		std::cout << "Vehicle #" << _id << " copy constructor called" << std::endl;
	}
	// move constructor
	Vehicle2(Vehicle2 && src)
	{
		_id = src.getID();
		_name = new std::string(src.getName());
		src.setID(0);
		src.setName("Default Name");

		std::cout << "Vehicle #" << _id << " move constructor called" << std::endl;
	}

	// setter and getter
	void setID(int id) { _id = id; }
	int getID() { return _id; }
	void setName(std::string name) { *_name = name; }
	std::string getName() { return *_name; }

private:
	int _id;
	std::string *_name;
};

class Vehicle3
{
public:
	//default constructor
	Vehicle3() : _id(0), _name(new std::string("Default Name"))
	{
		std::cout << "Vehicle #" << _id << " Default constructor called" << std::endl;
	}

	//initializing constructor
	Vehicle3(int id, std::string name) : _id(id), _name(new std::string(name))
	{
		std::cout << "Vehicle #" << _id << " Initializing constructor called" << std::endl;
	}

	// move constructor with unique pointer
	Vehicle3(Vehicle3 && src) : _name(std::move(src._name))
	{
		// move id to this and reset id in source
		_id = src.getID();
		src.setID(0);

		std::cout << "Vehicle #" << _id << " move constructor called" << std::endl;
	};

	// setter and getter
	void setID(int id) { _id = id; }
	int getID() { return _id; }
	void setName(std::string name) { *_name = name; }
	std::string getName() { return *_name; }

private:
	int _id;
	std::unique_ptr<std::string> _name;
};

class Vehicle4
{
public:
	// default constructor
	Vehicle4() : _id(0), _name(new std::string("Default Name"))
	{
		std::cout << "Vehicle #" << _id << " Default constructor called" << std::endl;
	}
	// initializing constructor
	Vehicle4(int id, std::string name) : _id(id), _name(new std::string(name))
	{
		std::cout << "Vehicle #" << _id << " Initializing constructor called" << std::endl;
	}
	// copy constructor
	Vehicle4(Vehicle4 const &src)
	{
		_id = src._id;
		if (src._name != nullptr)
		{
			_name = new std::string;
			*_name = *src._name;
		}
		std::cout << "Vehicle #" << _id << " copy constructor called" << std::endl;
	}

	// setter and getter
	void setID(int id) { _id = id; }
	int getID() { return _id; }
	void setName(std::string name) { *_name = name; }
	std::string getName() { return *_name; }

private:
	int _id;
	std::string *_name;
};

int main()
{
	// Example 1
	std::cout << "Example 1:" << std::endl;
	// create instances of class Vehicle
	Vehicle1 v0;	// default constructor
	Vehicle1 v1(1, "Vehicle 1"); // initializing constructor
	std::future<void> ftr1 = std::async([](Vehicle1 v) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		v.setID(2);
	}, v0);

	v0.setID(3);

	ftr1.wait();
	std::cout << "Vehicle #" << v0.getID() << std::endl;

	// Example 2
	std::cout << "\nExample 2:" << std::endl;
	// launch a thread that modifies the Vehicle name
	std::future<void> ftr2 = std::async([](Vehicle1 v) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		v.setName("Vehicle 2");
	}, v0);

	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	v0.setName("Vehicle 3");
	// read and write name in different threads (which one of the above creates a data race?) => name:Yes, id:No

	ftr2.wait();
	std::cout << v0.getName() << std::endl;

	// Example 3
	std::cout << "\nExample 3:" << std::endl;
	Vehicle2 v2;	// default constructor
	Vehicle2 v3(1, "Vehicle 1");	// initializing constructor
	// launch a thread that modifies the Vehicle name
	std::future<void> ftr3 = std::async([](Vehicle2 v) {
		v.setName("Vehicle 2");
	}, std::move(v2));

	ftr3.wait();
	std::cout << v2.getName() << std::endl;

	// Example 4
	std::cout << "\nExample 4:" << std::endl;
	Vehicle3 v4;	// default constructor
	Vehicle3 v5(1, "Vehicle 1");	// initializing constructor

	// launch a thread that modifies the Vehicle name
	std::future<void> ftr4 = std::async([](Vehicle3 v) {
		v.setName("Vehicle 2");
	}, std::move(v4));

	ftr4.wait();
	//std::cout << v4.getName() << std::endl; // this will now cause an exception

	// Quiz
	std::cout << "\nQuiz:" << std::endl;
	Vehicle4 v6;
	Vehicle4 v7(1, "Vehicle 1");
	// launch a thread that modifies the Vehicle name
	std::future<void> ftr5 = std::async([](Vehicle4 v) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate work
		v.setName("Vehicle 2");
	}, v6);

	v6.setName("Vehicle 3");

	ftr5.wait();
	std::cout << v6.getName() << std::endl;

	return 0;
}
