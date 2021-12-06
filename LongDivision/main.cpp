#include <iostream>
#include <vector>

void longDivide(int numerator, const int& denominator, std::vector<int>& results, int maxDepth);

int main()
{
	std::cout << "\n ----------- RECURSIVE LONG DIVISION ----------\n\n\n";

	// 
	// ASSUMPTIONS
	//		- NUMERATOR > DENOMINATOR
	//		- WHOLE NUMBER NUMERATOR AND DENOMINATOR
	//
		
	int n = 355;
	int d = 113;
	int maxDepth = 1000;

	//std::cout << "Numerator => " << n << std::endl;
	//std::cout << "Denominator => " << d << std::endl;
	//std::cout << "\n\n\n";

	// do division
	std::vector<int> decimals;
	longDivide(n, d, decimals, maxDepth);
	std::cout << "\n\n\n";

	// display results
	std::cout << n << " / " << d << " = ";	
	for (int i = 0; i < decimals.size(); ++i)
	{
		if ((i == 0) && (d > n))
		{
			std::cout << "0." << decimals.at(i);
		}
		else if ((i == 0) && (d < n))
		{
			std::cout << decimals.at(i) << ".";
		}
		else
		{
			std::cout << decimals.at(i);
		}
	}
	std::cout << "\n\n\n";

	return EXIT_SUCCESS;
}


void longDivide(int numerator, const int& denominator, std::vector<int>& results, int maxDepth)
{	
	if (maxDepth == 0) return;
	if (numerator == 0) return;

	if (denominator > numerator) 
	{
		numerator *= 10;
	}

	// step 1 - num times denominator fits in numerator
	int quotient = numerator / denominator;	
	results.push_back(quotient);
	
	// step 2 - multiple the quotient by the denominator
	int product = quotient * denominator;
	
	// step 3 - calculate difference between numerator and product
	int diff = numerator - product;
	
	// diff is new numerator
	longDivide(diff, denominator, results, --maxDepth);
}