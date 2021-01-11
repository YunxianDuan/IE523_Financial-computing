// K Peg Tower of Hanoi Solution
// Written by Prof. Sreenivas for IE523: Financial Computing

#include <iostream>
#include "prog1.h"

int main(int argc, char* const argv[])
{
	int number_of_disks, number_of_pegs;

	// See 'k_hanoi.h' for the Towers class definition
	Towers x;

	std::cout << "Number of pegs? ";
	std::cin >> number_of_pegs;
	std::cout << "Number of disks? ";
	std::cin >> number_of_disks;

	x.solve(number_of_pegs, number_of_disks);
}