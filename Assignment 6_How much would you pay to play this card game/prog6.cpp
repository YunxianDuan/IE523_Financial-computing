#include <iostream>
#include <fstream>
#include "card_game.h"

using namespace std;

int main(int argc, char* const argv[])
{
    int total_number_of_cards;
    double** tem;

    sscanf(argv[1], "%d", &total_number_of_cards);

    tem = new double* [total_number_of_cards / 2 + 1];

    for (int i = 0; i <= total_number_of_cards / 2; i++)
        tem[i] = new double[total_number_of_cards / 2];

    for (int i = 0; i <= total_number_of_cards / 2; i++)
        for (int j = 0; j <= total_number_of_cards / 2; j++)
        {
            tem[i][j] = 0;
        }

    cout << "Total Number of Cards = " << total_number_of_cards << endl;

    cout << "Value of the game = " << value(total_number_of_cards / 2, total_number_of_cards / 2, tem) << endl;

    free(tem);

    return 0;

}