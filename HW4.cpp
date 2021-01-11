//
//  main.cpp
//  Stable Marriage Problem
//
//  Created by Ramavarapu Sreenivas on 8/29/14.
//  Copyright (c) 2014 Ramavarapu Sreenivas. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class stable_marriage_instance
{
    // Private
    int no_of_couples;
    vector <vector <int> > Preference_of_men;
    vector <vector <int> > Preference_of_women;
    vector <int> match_for_men;
    vector <int> match_for_women;

    // private member function: checks if anybody is free in boolean "my_array"
    // returns the index of the first-person who is free in "my_array"
    // if no one is free it returns a -1.
    int anybody_free(vector <bool> my_array)
    {
        // fill the necessary code for this function
        int i = size(my_array);
        for (int k = 0; k < i; k++)
        {
            if (my_array[k] == 1)
            {
                return k;
            }
        }
        return -1;
    }

    // private member function: if index1 is ranked higher than index2
    // in a preference array called "my_array" it returns "true"; otherwise
    // it returns "false"
    bool rank_check(vector <int> my_array, int index1, int index2)
    {
        // fill the necessary code for this function
        int i = size(my_array);
        int rank1;
        int rank2;
        for (int k = 0; k < i; k++)
        {
            if (my_array[k] == index1)
                rank1 = k;
        }
        for (int k = 0; k < i; k++)
        {
            if (my_array[k] == index2)
                rank2 = k;
        }
        if (rank1 < rank2)
        {
            return true;
        }
        else return false;

    }

    // private member function: implements the Gale-Shapley algorithm
    void Gale_Shapley()
    {
        vector <bool> propose;
        vector <bool> is_man_free;
        vector <bool> is_woman_free;
        vector <vector <bool> > has_this_man_proposed_to_this_woman;

        int man_index, woman_index;

        for (int i = 0; i < no_of_couples; i++)
        {
            propose.push_back(false);
        }

        // initializing everything
        for (int i = 0; i < no_of_couples; i++)
        {
            // do the necessary initialization here
            is_man_free.push_back(1);
            is_woman_free.push_back(1);
            match_for_men.push_back(0);
            match_for_women.push_back(0);
            has_this_man_proposed_to_this_woman.push_back(propose);
        }

        // Gale-Shapley Algorithm
        while ((man_index = anybody_free(is_man_free)) >= 0)
        {
            // fill the necessary code here
            for (int k = 0; k < no_of_couples; k++)
            {
                if (has_this_man_proposed_to_this_woman[man_index][Preference_of_men[man_index][k]] == 0)
                {
                    woman_index = Preference_of_men[man_index][k];
                    break;
                }
            }

            if (is_woman_free[woman_index] == 1)
            {
                match_for_men[man_index] = woman_index;
                match_for_women[woman_index] = man_index;
                has_this_man_proposed_to_this_woman[man_index][woman_index] = 1;
                is_man_free[man_index] = 0;
                is_woman_free[woman_index] = 0;
            }

            else
            {
                if (rank_check(Preference_of_women[woman_index], man_index, match_for_women[woman_index]))
                {
                    is_man_free[match_for_women[woman_index]] = 1;
                    is_man_free[man_index] = 0;
                    match_for_men[man_index] = woman_index;
                    match_for_women[woman_index] = man_index;
                    has_this_man_proposed_to_this_woman[man_index][woman_index] = 1;
                }
                else
                {
                    has_this_man_proposed_to_this_woman[man_index][woman_index] = 1;
                }
            }
        }
    }

    // private member function: reads data
    void read_data(int argc, const char* argv[])
    {
        // fill the necessary code here.  The first entry in the input
        // file is the #couples, followed by the preferences of the men
        // and preferences of the women.  Keep in mind all indices start
        // from 0.
        ifstream input_filename(argv[1]);

        if (input_filename.is_open())
        {
            cout << "Input File Name: " << argv[1] << endl;
            int value_just_read;
            input_filename >> no_of_couples;
            cout << "Number of couples = " << no_of_couples << endl;

            for (int k = 0; k < no_of_couples; k++)
            {
                vector<int>Preference;
                Preference_of_men.push_back(Preference);
                Preference_of_women.push_back(Preference);
            }

            cout << endl;
            cout << "Preference of Men" << endl;
            cout << "------------------" << endl;

            for (int k = 0; k < no_of_couples; k++)
            {
                cout << "(" << k << "): ";
                for (int i = 0; i < no_of_couples; i++)
                {
                    input_filename >> value_just_read;
                    cout << value_just_read << " ";
                    Preference_of_men[k].push_back(value_just_read);
                }
                cout << endl;
            }

            cout << endl;
            cout << "Preference of Women" << endl;
            cout << "------------------" << endl;

            for (int k = 0; k < no_of_couples; k++)
            {
                cout << "(" << k << "): ";
                for (int i = 0; i < no_of_couples; i++)
                {
                    input_filename >> value_just_read;
                    cout << value_just_read << " ";
                    Preference_of_women[k].push_back(value_just_read);
                }
                cout << endl;
            }

        }
        else
        {
            cout << "Input file missing" << endl;
            exit(0);
        }
    }

    // private member function: print solution
    void print_soln()
    {
        // write the appropriate code here
        cout << endl;
        cout << "Matching for Men" << endl;
        for (int k = 0; k < no_of_couples; k++)
            cout << "Man: " << k << " -> " << "Woman" << match_for_men[k] << endl;

        cout << endl;

        cout << "Matching for Women" << endl;
        for (int k = 0; k < no_of_couples; k++)
            cout << "Woman: " << k << " -> " << "Man" << match_for_women[k] << endl;
    }

public:
    void solve_it(int argc, const char* argv[])
    {
        read_data(argc, argv);

        Gale_Shapley();

        print_soln();
    }
};

int main(int argc, const char* argv[])
{
    stable_marriage_instance x;
    x.solve_it(argc, argv);
}
