//
//  evolution.h
//  SalesPersonEA
//
//  Created by Shin-Fong Wu on 5/21/15.
//  Copyright (c) 2015 Xingfang Wu. All rights reserved.
//

#ifndef __SalesPersonEA__evolution__
#define __SalesPersonEA__evolution__

#define CITYNUM 10
#define POPULATION 10
#define MUTATION_RATE 0.3
#define CROSSOVER_RATE 0.5

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>


using namespace std;

typedef struct city{
    float x,y;
} CityStructure;

class evolution
{
public:
    evolution();
    ~evolution();
    
    float euclideanDis(CityStructure,CityStructure);
    void printPresentPath();
    double getPathCost(int);
    double getFitness(int);
    void evolve(int);
    double getBestCost();
    double getAverageCost();
    
private:
    CityStructure Cities[CITYNUM];
    float ScaledCityDis[CITYNUM][CITYNUM];
    vector<vector<int>> matingPool;
    void parentSelection();
    void crossOver();
    void mutation();

};

#endif /* defined(__SalesPersonEA__evolution__) */
