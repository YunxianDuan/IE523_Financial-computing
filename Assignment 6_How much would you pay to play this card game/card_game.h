/*
 *  card_game.h
 *  Card Game
 *
 *  Created by Ramavarapu Sreenivas
*/

#ifndef	CARDGAME_H
#define CARDGAME_H
#include <algorithm>

using namespace std;

double value(int r, int b, double** tem)
{

    if (0 == r)
        return ((double)b);
    if (0 == b)
        return (0);

    if (tem[r][b] == 0)

    {
        double term1 = ((double)r / (r + b)) * value(r - 1, b, tem);

        double term2 = ((double)b / (r + b)) * value(r, b - 1, tem);

        tem[r][b] = max((term1 + term2), (double)(b - r));
    }

    //return std::max((term1 + term2), (double) (b - r));
    return tem[r][b];
}
#endif
