#include <iostream>

int main()
{
	int num1{ 0 };
	int num0{ 0 };
	
	for (int i = 0; i < 1000000; i++)
	{
		int dir = std::rand() % 2;
		if (dir == 0) num0++;
		else if (dir == 1) num1++;
		else std::cout << "uh oh" << std::endl;
	}

	std::cout << "num0: " << num0 << std::endl;
	std::cout << "num1: " << num1 << std::endl;

	return EXIT_SUCCESS;
}