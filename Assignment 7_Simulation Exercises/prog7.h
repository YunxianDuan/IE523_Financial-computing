/*
 *  alice_and_bob.h
 *  Loosing as little as possible
 *
 *  Created by Ramavarapu Sreenivas on 9/2/12.
 *  Copyright 2012 University of Illinois at Urbana-Champaign. All rights reserved.
 *
 */

#ifndef ALICE_AND_BOB
#define ALICE_AND_BOB

#include <cmath>
#include <vector>
#include <fstream>

using namespace std;

class I_have_nothing_apropos_for_this_class
{
private:
	double alice_probability, bob_probability;

	// private member function: uniform RV generator
	double get_uniform()
	{
		// write the appropriate code here
		return (((double)rand()) / (pow(2.0, 15.0) - 1.0));
	}

	// private member function: nCi (i.e. n-take-i)
	long long int take(int n, int i)
	{
		// write a **RECURSIVE** implementation of n-take-i.
		// If you made it non-recurisive (i.e. n!/((n-i)!i!)) -- it
		// will take too long for large sizes
		if (i == 0)
		{
			return 1;
		}
		else
		{
			return (((n - i + 1) * take(n, i - 1)) / i);
		}

	}

	// this routine implements the probability that Alice has more
	// heads than Bob after n-many coin tosses
	double theoretical_value(double q, double p, int n)
	{
		// implement equation 1.1 of Addona-Wagon-Wilf paper
		double alice_win = 0;

		for (int r = 0; r < n; r++)
		{
			for (int s = r + 1; s <= n; s++)
			{
				alice_win += take(n, r) * pow(p, r) * pow((1 - p), (n - r)) * take(n, s) * pow(q, s) * pow((1 - q), (n - s));
			}
		}

		return alice_win;
	}

public:

	// public function:
	void set_probability(double alice_p, double bob_p)
	{
		alice_probability = alice_p;
		bob_probability = bob_p;
	}

	// probability of Alice winning the game.
	double simulated_value(int number_of_coin_tosses_in_each_game, int no_of_trials)
	{
		int no_of_wins_for_alice = 0;
		for (int i = 0; i < no_of_trials; i++)
		{
			int number_of_heads_for_alice = 0;
			int number_of_heads_for_bob = 0;
			for (int j = 0; j < number_of_coin_tosses_in_each_game; j++)
			{
				if (get_uniform() < alice_probability)
					number_of_heads_for_alice++;
				if (get_uniform() < bob_probability)
					number_of_heads_for_bob++;
			}
			if (number_of_heads_for_alice > number_of_heads_for_bob)
				no_of_wins_for_alice++;
		}
		return (((double)no_of_wins_for_alice) / ((double)no_of_trials));
	}

	int search_result()
	{
		// implememt a discrete-search procedure for the optimal n-value.
		// start with n = 1 and find the discrete-value of n that has
		// the largest probability for Alice winning.  Why would this work?
		// See Theorem 2.2 of the paper for the reason!
		int optimal_value = 0;

		for (int i = 1; i <= 100; i++)
		{
			if (((theoretical_value(alice_probability, bob_probability, i)) >= (theoretical_value(alice_probability, bob_probability, i - 1))) && ((theoretical_value(alice_probability, bob_probability, i)) >= (theoretical_value(alice_probability, bob_probability, i + 1))))
			{
				optimal_value = i;
				break;
			}
		}

		return optimal_value;
	}

	vector <double> simulated_data;
	vector <double> theoretical_data;

	void output()
	{
		ofstream data;
		data.open("data.csv");
		data << "simulation" << "," << "theory" << endl;
		for (int i = 0; i < 60; i++)
		{
			theoretical_data.push_back(theoretical_value(alice_probability, bob_probability, i));
			simulated_data.push_back(simulated_value(i, 500000));
			data << simulated_data[i] << "," << theoretical_data[i] << endl;
		}
	}

};
#endif

