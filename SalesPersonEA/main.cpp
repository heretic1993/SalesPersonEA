//
//  main.cpp
//  SalesPersonEA
//
//  Created by Wu Xingfang on 5/20/15.
//

#define CITYNUM 10
#define POPULATION 4

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

using namespace std;

typedef struct city{
    float x,y;
} CityStructure;

CityStructure Cities[CITYNUM];

float ScaledCityDis[CITYNUM][CITYNUM];

vector<vector<int>> matingPool;

float euclideanDis(CityStructure city1,CityStructure city2){
    return sqrt(pow((city1.x-city2.x),2)+pow((city1.y-city2.y),2));
}


void initialize(){
    srand((unsigned int)(time(NULL)));
    for (int i=0; i<CITYNUM; i++) {
        CityStructure temp={((float)rand()/(RAND_MAX)*2-1),((float)rand()/(RAND_MAX)*2-1)};
        Cities[i]=temp;
    }
    
    for (int i=0; i<CITYNUM; i++) {
        for (int j=0; j<CITYNUM; j++) {
            ScaledCityDis[i][j]=euclideanDis(Cities[i], Cities[j])*((float)rand()/(RAND_MAX)*1+1);
            cout<<ScaledCityDis[i][j]<<endl;
        }
    }
    
    //init parents path
    
    for (int j=0; j<POPULATION; j++) {
        vector<int> temp;
        for (int i=0; i<CITYNUM; i++) {
            temp[i]=i;
        }
        for (int i=0; i<CITYNUM; i++) {
            int n=rand()%CITYNUM;
            int m=rand()%CITYNUM;
            
            int tempValue=temp[n];
            temp[n]=temp[m];
            temp[m]=tempValue;
        }
        matingPool.push_back(temp);
    }
    
    
    cout<<"matingpool"<<*matingPool[1]<<endl;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    initialize();
    return 0;
}
