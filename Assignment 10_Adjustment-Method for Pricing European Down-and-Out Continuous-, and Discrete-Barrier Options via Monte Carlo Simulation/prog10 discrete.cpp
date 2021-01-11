
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <chrono>
#include <math.h>
using namespace std;

unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);

double risk_free_rate, strike_price;
double initial_stock_price, expiration_time, volatility, barrier_price;
int no_of_trials, no_of_barriers;

double max(double a, double b) {
	return (b < a) ? a : b;
}

double get_uniform() {
	std::uniform_real_distribution <double> distribution(0, 1.0);
	double number = distribution(generator);
	return (number);
}

double N(const double& z) {
	if (z > 6.0) { return 1.0; }; // this guards against overflow 
	if (z < -6.0) { return 0.0; };
	double b1 = 0.31938153;
	double b2 = -0.356563782;
	double b3 = 1.781477937;
	double b4 = -1.821255978;
	double b5 = 1.330274429;
	double p = 0.2316419;
	double c2 = 0.3989423;
	double a = fabs(z);
	double t = 1.0 / (1.0 + a * p);
	double b = c2 * exp((-z) * (z / 2.0));
	double n = ((((b5 * t + b4) * t + b3) * t + b2) * t + b1) * t;
	n = 1.0 - b * n;
	if (z < 0.0) n = 1.0 - n;
	return n;
};

int main(int argc, char* argv[])
{
	sscanf(argv[1], "%lf", &expiration_time);
	sscanf(argv[2], "%lf", &risk_free_rate);
	sscanf(argv[3], "%lf", &volatility);
	sscanf(argv[4], "%lf", &initial_stock_price);
	sscanf(argv[5], "%lf", &strike_price);
	sscanf(argv[6], "%d", &no_of_trials);
	sscanf(argv[7], "%d", &no_of_barriers);
	sscanf(argv[8], "%lf", &barrier_price);

	double call_option_price = 0.0;
	double put_option_price = 0.0;
	double call_option_price_adjusted = 0.0;
	double put_option_price_adjusted = 0.0;

	const double delta_T = expiration_time / ((double)no_of_barriers);
	const double delta_R = (risk_free_rate - 0.5 * pow(volatility, 2)) * delta_T;
	const double delta_SD = volatility * sqrt(delta_T);

	for (int i = 0; i < no_of_trials; i++)
	{
		double current_price1 = initial_stock_price;
		double current_price2 = initial_stock_price;
		double current_price3 = initial_stock_price;
		double current_price4 = initial_stock_price;

		double current_price1_adjusted = initial_stock_price;
		double current_price2_adjusted = initial_stock_price;
		double current_price3_adjusted = initial_stock_price;
		double current_price4_adjusted = initial_stock_price;

		double p1 = 1.0; double p2 = 1.0; double p3 = 1.0; double p4 = 1.0;

		for (int j = 0; j < no_of_barriers; j++)
		{
			// create the unit normal variates using the Box-Muller Transform
			double x = get_uniform();
			double y = get_uniform();
			double a = sqrt(-2.0 * log(x)) * cos(2 * 3.141592653589793 * y);
			double b = sqrt(-2.0 * log(x)) * sin(2 * 3.141592653589793 * y);

			if ((current_price1 * exp(delta_R + delta_SD * a)) > barrier_price)
				current_price1 = (current_price1 * exp(delta_R + delta_SD * a));
			else current_price1 = 0;

			if ((current_price2 * exp(delta_R - delta_SD * a)) > barrier_price)
				current_price2 = (current_price2 * exp(delta_R - delta_SD * a));
			else current_price2 = 0;

			if ((current_price3 * exp(delta_R + delta_SD * b)) > barrier_price)
				current_price3 = (current_price3 * exp(delta_R + delta_SD * b));
			else current_price3 = 0;

			if ((current_price4 * exp(delta_R - delta_SD * b)) > barrier_price)
				current_price4 = (current_price4 * exp(delta_R - delta_SD * b));
			else current_price4 = 0;

			current_price1_adjusted = current_price1_adjusted * exp(delta_R + delta_SD * a);
			current_price2_adjusted = current_price2_adjusted * exp(delta_R - delta_SD * a);
			current_price3_adjusted = current_price3_adjusted * exp(delta_R + delta_SD * b);
			current_price4_adjusted = current_price4_adjusted * exp(delta_R - delta_SD * b);

			double mean1 = (initial_stock_price + (((double)j / (double)no_of_barriers)) * (current_price1_adjusted - initial_stock_price));
			double mean2 = (initial_stock_price + (((double)j / (double)no_of_barriers)) * (current_price2_adjusted - initial_stock_price));
			double mean3 = (initial_stock_price + (((double)j / (double)no_of_barriers)) * (current_price3_adjusted - initial_stock_price));
			double mean4 = (initial_stock_price + (((double)j / (double)no_of_barriers)) * (current_price4_adjusted - initial_stock_price));

			double std = sqrt((((expiration_time / (double)no_of_barriers) * (double)j) * (1 - ((double)j / (double)no_of_barriers))));

			p1 *= (1 - N((barrier_price - mean1) / std));
			p2 *= (1 - N((barrier_price - mean2) / std));
			p3 *= (1 - N((barrier_price - mean3) / std));
			p4 *= (1 - N((barrier_price - mean4) / std));
		}

		double v1, v2, v3, v4;
		if (current_price1 != 0)
			v1 = 1;
		else v1 = 0;

		if (current_price2 != 0)
			v2 = 1;
		else v2 = 0;

		if (current_price3 != 0)
			v3 = 1;
		else v3 = 0;

		if (current_price4 != 0)
			v4 = 1;
		else v4 = 0;

		call_option_price += (max(0.0, current_price1 - strike_price) * v1 + max(0.0, current_price2 - strike_price) * v2
			+ max(0.0, current_price3 - strike_price) * v3 + max(0.0, current_price4 - strike_price) * v4) / 4.0;
		put_option_price += (max(0.0, strike_price - current_price1) * v1 + max(0.0, strike_price - current_price2) * v2
			+ max(0.0, strike_price - current_price3) * v3 + max(0.0, strike_price - current_price4) * v4) / 4.0;

		double pd1, pd2, pd3, pd4;
		if (current_price1_adjusted > barrier_price)
			pd1 = 1;
		else pd1 = 0;

		if (current_price2_adjusted > barrier_price)
			pd2 = 1;
		else pd2 = 0;

		if (current_price3_adjusted > barrier_price)
			pd3 = 1;
		else pd3 = 0;

		if (current_price4_adjusted > barrier_price)
			pd4 = 1;
		else pd4 = 0;

		call_option_price_adjusted += (max(0.0, current_price1_adjusted - strike_price) * p1 * pd1 + max(0.0, current_price2_adjusted - strike_price) * p2 * pd2
			+ max(0.0, current_price3_adjusted - strike_price) * p3 * pd3 + max(0.0, current_price4_adjusted - strike_price) * p4 * pd4) / 4.0;
		put_option_price_adjusted += (max(0.0, strike_price - current_price1_adjusted) * p1 * pd1 + max(0.0, strike_price - current_price2_adjusted) * p2 * pd2
			+ max(0.0, strike_price - current_price3_adjusted) * p3 * pd3 + max(0.0, strike_price - current_price4_adjusted) * p4 * pd4) / 4.0;

	}
	call_option_price = exp(-risk_free_rate * expiration_time) * (call_option_price / ((double)no_of_trials));
	put_option_price = exp(-risk_free_rate * expiration_time) * (put_option_price / ((double)no_of_trials));
	call_option_price_adjusted = exp(-risk_free_rate * expiration_time) * (call_option_price_adjusted / ((double)no_of_trials));
	put_option_price_adjusted = exp(-risk_free_rate * expiration_time) * (put_option_price_adjusted / ((double)no_of_trials));

	cout << "European Down-and-out Discrete Barrier Options Pricing via Monte Carlo Simulation" << endl;
	cout << "Expiration Time (Years) = " << expiration_time << endl;
	cout << "Risk Free Interest Rate = " << risk_free_rate << endl;
	cout << "Volatility (%age of stock value) = " << volatility * 100 << endl;
	cout << "Initial Stock Price = " << initial_stock_price << endl;
	cout << "Strike Price = " << strike_price << endl;
	cout << "Barrier Price = " << barrier_price << endl;
	cout << "Number of Trials = " << no_of_trials << endl;
	cout << "Number of Discrete Barriers = " << no_of_barriers << endl;
	cout << "--------------------------------------" << endl;
	cout << "The average Call Price via explict simulation of price paths           = " << call_option_price << endl;
	cout << "The average Call Price with Brownian-Bridge Correction on final price = " << call_option_price_adjusted << endl;
	cout << "The average Put Price via explict simulation of price paths           = " << put_option_price << endl;
	cout << "The average Put Price with Brownian-Bridge Correction on final price = " << put_option_price_adjusted << endl;
}
