#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

class ORGANISM
{
    public:
        //----------------------------------
        //    CONSTRUCTOR AND DESTRUCTOR
        //----------------------------------
        ORGANISM(int organism_length);
        ~ORGANISM();

        //----------------------------------
        //        PUBLIC VARIABLES
        //----------------------------------
        std::vector<int> DNA;
        std::vector<int> ORGANISM_S;

        int raffle {0};
        int organismLength {0};
        double fitness {0};                    

        //----------------------------------
        //         MAIN FUNCTIONS
        //----------------------------------
        void calculateFitness(std::vector<int> target);
        std::string printCharacters();

    private:
    protected:
};

class POPULATION
{
    public:
        //----------------------------------
        //    CONSTRUCTOR AND DESTRUCTOR
        //----------------------------------
        POPULATION(std::string outputfile_name, std::string target_string, int population_size, int max_generation, int mating_factor, int mutation_factor);
        ~POPULATION();

        //----------------------------------
        //        PUBLIC VARIABLES
        //----------------------------------
        std::vector<int> target;
        std::vector<int> matingPool;
        std::vector<ORGANISM> population;

        int populationSize {0};
        int maxGeneration {0};
        int matingFactor {0};
        int mutationFactor {0};
        int targetLength {0};
        double maxFitness {0};
        double avgFitness {0};

        std::ofstream fileOutput;

        //----------------------------------
        //           MAIN LOOP
        //----------------------------------
        void loop(int max_generation);

        //----------------------------------
        //         MAIN FUNCTIONS
        //----------------------------------
        void buildPopulation();
        void calculateFitness();
        void buildMatingPool();

        template<typename T>
        std::vector<int> breed(T parentOne, T parentTwo);

        template<typename T>
        std::vector<int> causeMutation(T child);

        //----------------------------------
        //      AUXILIARY FUNCTIONS
        //----------------------------------
        bool checkTarget(int generation);

    private:
    protected:
};

/*****************************************************************/
/*                POPULATION CLASS FUNCTIONS                     */
/*****************************************************************/

POPULATION::POPULATION(std::string outputfile_name, std::string target_string, int population_size, int max_generation, int mating_factor, int mutation_factor) 
                        : targetLength(target_string.length()), populationSize(population_size), maxGeneration(max_generation),
                                matingFactor(mating_factor), mutationFactor(mutation_factor)
{
    for(size_t i = 0; i < POPULATION::targetLength; i++)
    {
        POPULATION::target.push_back((int)target_string[i]);
    }
    POPULATION::buildPopulation();

    POPULATION::fileOutput.open(outputfile_name);
    POPULATION::fileOutput << "Generation,Fitness\n";
}

POPULATION::~POPULATION()
{
    POPULATION::fileOutput.close();
}

void POPULATION::loop(int max_generation)
{
    int x {0};
    int amount {0};
    int parentOne {0};
    int parentTwo {0};
    int generation {0};
    std::vector<int> mut_index;

    while(!POPULATION::checkTarget(generation))
    {
        if(generation>=max_generation)
        {
            return;
        }
        std::cout << "Generation: " << generation << " ";

        POPULATION::calculateFitness();
        POPULATION::buildMatingPool();
        for (size_t i = 0; i < POPULATION::populationSize; i++)
        {
            parentOne = rand() % POPULATION::matingPool.size();
            parentTwo = rand() % POPULATION::matingPool.size();
            while(parentOne == parentTwo)
            {
                parentOne = rand() % POPULATION::matingPool.size();
                parentTwo = rand() % POPULATION::matingPool.size();
            }
            POPULATION::population[i].ORGANISM_S = POPULATION::breed(POPULATION::population[POPULATION::matingPool[parentOne]].ORGANISM_S, POPULATION::population[POPULATION::matingPool[parentTwo]].ORGANISM_S);
        }
        mut_index.clear();
        amount = rand() % (POPULATION::mutationFactor*POPULATION::populationSize)/100;
        for (size_t i = 0; i < amount; i++)
        {   
            x = rand() % POPULATION::populationSize;
            if(std::find(mut_index.begin(), mut_index.end(), x)==mut_index.end())
            {
                mut_index.push_back(x);
            }
            else
            {
                i--;
            }
        }
        for(auto& i : mut_index)
        {
            POPULATION::population[i].ORGANISM_S = POPULATION::causeMutation(POPULATION::population[i].ORGANISM_S);
        }
        POPULATION::fileOutput << generation << "," << POPULATION::maxFitness << "," << POPULATION::avgFitness << "\n";
        POPULATION::calculateFitness();
        generation++;
    }
    POPULATION::fileOutput << generation << "," << 1 << "," << POPULATION::avgFitness << "\n";
}

void POPULATION::buildPopulation()
{
    for (size_t i = 0; i < POPULATION::populationSize; i++)
    {
        population.push_back(ORGANISM(POPULATION::targetLength));
    }
}

void POPULATION::calculateFitness()
{
    for (size_t i = 0; i < POPULATION::populationSize; i++)
    {
        POPULATION::population[i].calculateFitness(POPULATION::target);
    }
}

void POPULATION::buildMatingPool()
{
    std::vector<double> fitness_vec;
    double fitness_avg {0};
    POPULATION::avgFitness = 0;
    POPULATION::matingPool.clear();
    for (size_t i = 0; i < POPULATION::populationSize; i++)
    {
        fitness_vec.push_back(POPULATION::population[i].fitness);
        fitness_avg += POPULATION::population[i].fitness;
    }
    POPULATION::maxFitness = *std::max_element(fitness_vec.begin(), fitness_vec.end());
    std::cout << " Fitness: " << POPULATION::maxFitness << " ";
    POPULATION::avgFitness = fitness_avg/POPULATION::populationSize;
    POPULATION::population[std::distance(fitness_vec.begin(),std::max_element(fitness_vec.begin(), fitness_vec.end()))].printCharacters();
    for (size_t i = 0; i < POPULATION::populationSize; i++)
    {
        fitness_vec[i] = (POPULATION::population[i].fitness)/POPULATION::maxFitness;
        POPULATION::population[i].raffle = fitness_vec[i]*POPULATION::matingFactor;
        if(POPULATION::population[i].raffle!=0)
        {
            for (size_t j = 0; j < POPULATION::population[i].raffle; j++)
            {
                POPULATION::matingPool.push_back(i);
            }
        }   
    }
}

template<typename T>
std::vector<int> POPULATION::breed(T parentOne, T parentTwo)
{
    std::vector<int> child;
    // -------------------
    //    FIRST METHOD
    // -------------------
    // int random_midpoint = rand() % (parentOne.size()-1); // -1 because dont want the very last index.
    // for (size_t i = 0; i < random_midpoint; i++)
    // {
    //     child.push_back(parentOne[i]);
    // }
    // for (size_t i = random_midpoint; i < parentOne.size(); i++)
    // {
    //     child.push_back(parentTwo[i]);
    // }

    // -------------------
    //   SECOND METHOD
    // -------------------
    int x {0};
    std::vector<int> rand_index;
    int amount = (rand() % (parentOne.size()-2))+1;
    for (size_t i = 0; i < amount; i++)
    {   
        x = rand() % parentOne.size();
        if(std::find(rand_index.begin(), rand_index.end(), x)==rand_index.end())
        {
            rand_index.push_back(x);
        }
        else
        {
            i--;
        }
    }
    for (size_t i = 0; i < parentOne.size(); i++)
    {
        if(std::find(rand_index.begin(), rand_index.end(), i)==rand_index.end())
        {
            child.push_back(parentOne[i]);
        }
        else
        {
            child.push_back(parentTwo[i]);
        }
    }
    return child;
}

template<typename T>
std::vector<int> POPULATION::causeMutation(T child)
{
    int x {0};
    std::string characters = "ABCDEFGHIJKLMNOPQRSTUVXYZ abdcdefghijklmnopqrstuvxyz?.!,";
    std::vector<int> mut_index;
    int amount = rand() % child.size();
    for (size_t i = 0; i < amount; i++)
    {   
        x = rand() % child.size();
        if(std::find(mut_index.begin(), mut_index.end(), x)==mut_index.end())
        {
            mut_index.push_back(x);
        }
        else
        {
            i--;
        }
    }
    for(auto &i : mut_index)
    {
        x = rand() % characters.length();
        child[i] = characters[x];
    }
    return child;
}

bool POPULATION::checkTarget(int generation)
{
    for (size_t i = 0; i < POPULATION::populationSize; i++)
    {
        if(POPULATION::population[i].fitness>=1)
        {
            std::cout << "Generation: " << generation << " Fitness: " << POPULATION::population[i].fitness << " ";
            POPULATION::population[i].printCharacters();
            return true;
        }
    }
    return false;
}

/*****************************************************************/
/*                  ORGANSIM CLASS FUNCTIONS                     */
/*****************************************************************/

ORGANISM::ORGANISM(int organism_length) : organismLength(organism_length)
{
    int x {0};
    std::string characters = "ABCDEFGHIJKLMNOPQRSTUVXYZ abdcdefghijklmnopqrstuvxyz?.!,";
    for (size_t i = 0; i < organism_length; i++)
    {
        x = rand() % characters.length();
        ORGANISM::ORGANISM_S.push_back(characters[x]);
        ORGANISM::DNA.push_back(0);
    }
}

ORGANISM::~ORGANISM()
{

}

void ORGANISM::calculateFitness(std::vector<int> target)
{
    for (size_t i = 0; i < ORGANISM::organismLength; i++)
    {
        if(ORGANISM_S[i] == target[i])
        {
            DNA[i] = 1;
        }else
        {
            DNA[i] = 0;
        }   
    }
    ORGANISM::fitness = std::count(ORGANISM::DNA.begin(), ORGANISM::DNA.end(), 1)/(double)ORGANISM::organismLength;
}

std::string ORGANISM::printCharacters()
{
    std::stringstream ss;
    for (size_t i = 0; i < ORGANISM::organismLength; i++)
    {
        std::cout << (char)ORGANISM::ORGANISM_S[i];
        ss << (char)ORGANISM::ORGANISM_S[i];
    }
    std::cout << std::endl;
    return ss.str();
}

#endif