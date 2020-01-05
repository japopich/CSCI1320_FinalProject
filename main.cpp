/****************************************************/
/*                                                  */
/*                                                  */
/*             CSCI1320 Final Project               */
/*               Genetic Algorithm                  */
/*                                                  */
/*                  11/29/2019                      */
/*                                                  */
/*                 Jason Popich                     */
/*                                                  */
/****************************************************/

/* TO COMPILE USE g++ -std=c++17 -o GAP main.cpp*/
// Reason for this is because of uniform initialization of variables.

#include "main.hpp"

int main()
{
    srand(time(NULL));

    std::string targetPhrase {"The mountains shall rise"};
    std::string outputfile_name {"GAPCSV.csv"};
    int population_size {4000};
    int mating_factor {10};
    int maxGenerations {100000};
    double mutationFactor {4}; // In percent...

    POPULATION ai(outputfile_name, targetPhrase, population_size, maxGenerations, mating_factor, mutationFactor);
    ai.loop(maxGenerations);


    return 0;
}