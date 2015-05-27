//
//  evolution.cpp
//  SalesPersonEA
//
//  Created by Shin-Fong Wu on 5/21/15.
//  Copyright (c) 2015 Xingfang Wu. All rights reserved.
//

#include "evolution.h"


float evolution::euclideanDis(CityStructure city1,CityStructure city2){
    return sqrt(pow((city1.x-city2.x),2)+pow((city1.y-city2.y),2));
}

evolution::evolution(){
    srand((unsigned int)(time(NULL)));
    for (int i=0; i<CITYNUM; i++) {
        CityStructure temp={((float)rand()/(RAND_MAX)*2-1),((float)rand()/(RAND_MAX)*2-1)};
        evolution::Cities[i]=temp;
    }
    
    for (int i=0; i<CITYNUM; i++) {
        for (int j=0; j<CITYNUM; j++) {
            ScaledCityDis[i][j]=euclideanDis(Cities[i], Cities[j])*((float)rand()/(RAND_MAX)*1+1);
            //cout<<ScaledCityDis[i][j]<<endl;
        }
    }
    
    //init parents path
    
    for (int j=0; j<POPULATION; j++) {
        vector<int> temp;
        
        for (int i=0; i<CITYNUM; i++) {
            temp.push_back(i);
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
}


void evolution::printPresentPath(){
    for (vector<vector<int>>::iterator itr=matingPool.begin(); itr!=matingPool.end(); itr++) {
        cout<<"Path of the "<<itr-matingPool.begin()<<" parent is:";
        for (vector<int>::iterator iterator=(*itr).begin(); iterator!=(*itr).end(); ++iterator) {
            cout<<*iterator;
        }
        cout<<endl;
    }
}

double evolution::getPathCost(int index){
    double sum=0;
    matingPool[index];
    for (vector<int>::iterator itr=matingPool[index].begin(); itr!=matingPool[index].end()-1; ++itr) {
        sum+=ScaledCityDis[*itr][*(itr+1)];
        if (itr==matingPool[index].end()-1) {
            sum+=ScaledCityDis[*(itr+1)][*(matingPool[index].begin())];
            // return to the first dot
        }
    }
    //cout<<"Cost:"<<sum<<endl;
    return sum;
}

double evolution::getFitness(int index){
    return 1.0/getPathCost(index);
}

typedef struct indexedFitness{
    double fitness;
    int index;
} INDEXED_FITNESS;

bool compareFitness(INDEXED_FITNESS i,INDEXED_FITNESS j){
    if (i.fitness>=j.fitness) {
        return true;
    }else{
        return false;
    }
}

void evolution::parentSelection(){
    
    vector<INDEXED_FITNESS> parents;
    //cout<<"Parents before selecting:"<<endl;
    //printPresentPath();
    //cout<<"------------------------"<<endl;
    for (int i=0; i<POPULATION; i++) {
        INDEXED_FITNESS tempIndexFitness = {getFitness(i),i};
        parents.push_back(tempIndexFitness);
    }
    sort(parents.begin(),parents.end(),compareFitness);
    vector<vector<int>> newMatingPool;
    for (int i=0; i<POPULATION-1; i++) {
        if (i==0) {
            newMatingPool.push_back(matingPool[parents[i].index]);
            newMatingPool.push_back(matingPool[parents[i].index]);
        }else{
            newMatingPool.push_back(matingPool[parents[i].index]);
        }
    }
    matingPool=newMatingPool;
    //cout<<"successfully update parents!"<<endl;
    //printPresentPath();
}

void evolution::crossOver(){
    //pair randomly
    for (int i=0; i<POPULATION; i++) {
        int index1=rand()%POPULATION;
        int index2=rand()%POPULATION;
        vector<int> temp=matingPool[index1];
        matingPool[index1]=matingPool[index2];
        matingPool[index2]=temp;
    }
    //cout<<"before crossOver"<<endl;
    //printPresentPath();
    //Order CrossOver
    int switchPoint1,switchPoint2;
    for (int i=0; i<POPULATION/2; i++) {
    randomGen:
        switchPoint1=rand()%CITYNUM;
        switchPoint2=rand()%CITYNUM;
        
        //cout<<switchPoint1<<"+"<<switchPoint2<<endl;
        
        if(switchPoint2>switchPoint1){
            int temp=switchPoint2;
            switchPoint2=switchPoint1;
            switchPoint1=temp;
        }else if(switchPoint1==switchPoint2){
            if (switchPoint2!=CITYNUM-1) {
                switchPoint2--;
            }else if(switchPoint1!=0){
                switchPoint1++;
            }
        }
        if ((switchPoint1-switchPoint2)<4) {
            goto randomGen;
        }
        
        vector<int> gap1,gap2,left1,left2;
        for (int j=switchPoint2; j<switchPoint1; j++) {
            gap1.push_back(matingPool[i*2][j]);
            gap2.push_back(matingPool[i*2+1][j]);
        }
        for (int j=0; j<CITYNUM; j++) {
            for (int k=0; k<switchPoint1-switchPoint2; k++) {
                if (matingPool[2*i+1][j] != gap1[k] && k==(switchPoint1-switchPoint2-1)) {
                    left1.push_back(matingPool[2*i+1][j]);
                }else if(matingPool[2*i+1][j] == gap1[k]){
                    break;
                }
            }
            for (int k=0; k<switchPoint1-switchPoint2; k++) {
                if (matingPool[2*i][j] != gap2[k] && k==(switchPoint1-switchPoint2-1)) {
                    left2.push_back(matingPool[2*i][j]);
                }else if(matingPool[2*i][j] == gap2[k]){
                    break;
                }
            }
        }
        
        for (int j=0; j<switchPoint2; j++) {
            matingPool[2*i][j] = left1.back();
            matingPool[2*i+1][j] = left2.back();
            left1.pop_back();
            left2.pop_back();
        }
        for (int j=switchPoint1; j<CITYNUM; j++) {
            matingPool[2*i][j] = left1.back();
            matingPool[2*i+1][j] = left2.back();
            left1.pop_back();
            left2.pop_back();
        }
    }
    //cout<<"after crossOver"<<endl;
    //printPresentPath();
}

void evolution::mutation(){
    int mutationPoint1,mutationPoint2,temp;
    for (int i= 0; i<POPULATION; i++) {
        if (((double) rand() / (RAND_MAX))<=MUTATION_RATE) {
            mutationPoint1=rand()%CITYNUM;
            mutationPoint2=rand()%CITYNUM;
            temp=matingPool[i][mutationPoint1];
            matingPool[i][mutationPoint1]=matingPool[i][mutationPoint2];
            matingPool[i][mutationPoint2]=temp;
        }
    }
    //printPresentPath();
}

double evolution::getBestCost(){
    double min=99999;
    for (int i = 0; i<POPULATION; i++) {
        if (getPathCost(i)<=min) {
            min=getPathCost(i);
        }
    }
    return min;
}

double evolution::getAverageCost(){
    double sum=0;
    for (int i = 0; i<POPULATION; i++) {
        sum+=getPathCost(i);
    }
    sum/=POPULATION;
    return sum;
}

void evolution::evolve(int time){
    for (int i=0; i<time; i++) {
        //parent selection
        //crossover
        //mutation
        //printPresentPath();
        parentSelection();
        if(((double)rand()/RAND_MAX) <= CROSSOVER_RATE){
            crossOver();
        }
            mutation();
        //printPresentPath();
        cout<<"average:               "<<getAverageCost()<<endl;
        cout<<"best:     "<<getBestCost()<<endl;
        
    }
}

evolution::~evolution(){
}
