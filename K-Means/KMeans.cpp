#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <vector>
#include <cstring>
#include <stdlib.h> 
#include <time.h>
#include "Distance.h"
using namespace std;

#define ALL_NODES      435288
#define K_MEANS_TIMES  30
#define BLOCK_NUM      18
#define MAX_DIST       1000000000  //³õÊ¼×î´ó¾àÀë

float lat[ALL_NODES], lon[ALL_NODES];
int label[ALL_NODES];
double meansLat[BLOCK_NUM], meansLon[BLOCK_NUM];
list<int> cluster[BLOCK_NUM];

void init() {
	cout<<"Input the file..."<<endl;
	ifstream fin("newNodes_long.txt");
	string idTemp, latTemp, lonTemp;
	int i = 0;

	while (fin>>idTemp>>latTemp>>lonTemp) {
		lat[i] = atof(latTemp.c_str());
		lon[i] = atof(lonTemp.c_str());
		i++;
	}

	fin.close();
}

int getLabel(int num) {
	double minDist = MAX_DIST;
	int k;
	for (int i = 0; i < BLOCK_NUM; i++) {
		double x = Distance::getDist(lat[num], lon[num], meansLat[i], meansLon[i]);
		if (x < minDist) {
			minDist = x;
			k = i;
		}
	}
	return k;
}

void reCalcMeans() {
	for (int i = 0; i < BLOCK_NUM; i++) {
		double x = 0, y = 0;
		list<int>::iterator itor = cluster[i].begin();
		while (itor != cluster[i].end()) {
			x += lat[*itor];
			y += lon[*itor];
			itor ++;
		}
		meansLat[i] = x / cluster[i].size();
		meansLon[i] = y / cluster[i].size();
		cluster[i].clear();
	}
}
int main() {
	init();

	for (int i = 0; i < BLOCK_NUM; i++) {
		int num = i * (ALL_NODES / BLOCK_NUM) + (ALL_NODES / BLOCK_NUM / 2);
		meansLat[i] = lat[num];
		meansLon[i] = lon[num];
		cluster[i].clear();
	}

	for (int itTimes = 0; itTimes < K_MEANS_TIMES; itTimes++) {
		cout<<itTimes<<endl;

		for (int i = 0; i < ALL_NODES; i++) {
			label[i] = getLabel(i);
			cluster[label[i]].push_back(i);
		}
		if (itTimes == K_MEANS_TIMES - 1) {
			for (int i = 0; i < BLOCK_NUM; i++)
				cout<<i<<" "<<cluster[i].size()<<endl;
		}
		reCalcMeans();
	}

	ofstream fout("Node_With_Labels.txt");
	for (int i = 0; i < ALL_NODES; i++) {
		fout<<i<<" "<<lat[i]<<" "<<lon[i]<<" "<<label[i]<<endl;
	}
	fout.close();
	cout<<"END..."<<endl;
	return 0;
}

