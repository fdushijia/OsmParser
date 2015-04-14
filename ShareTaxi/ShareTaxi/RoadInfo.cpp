#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <vector>
#include "Distance.h"
using namespace std;

float lat[500000], lon[500000];

struct NeighNode {
	int id;
	NeighNode* next;
};
NeighNode* neighbour[500000];
NeighNode* tail[500000];
int n;



void init() {

	cout<<"Constructing the road..."<<endl;
	ifstream fin("newNodes_long.txt");
	string idTemp, latTemp, lonTemp;
	int i = 0;

	cout<<"Init..."<<endl;
	while (fin>>idTemp>>latTemp>>lonTemp) {
		lat[i] = atof(latTemp.c_str());
		lon[i] = atof(lonTemp.c_str());
		neighbour[i] = NULL;
		tail[i] = NULL;
		i++;
	}
	n = i;
	fin.close();
	
	cout<<"Constructing..."<<endl;
	ifstream wayIn("newWays_long.txt");
	string ss, tt;
	int s, t;
	while (wayIn>>ss>>tt) {
		s = atoi(ss.c_str());
		t = atoi(tt.c_str());
		if (neighbour[s] == NULL) {
			neighbour[s] = (struct NeighNode *)malloc(sizeof(NeighNode));
			neighbour[s]->id = t;
			neighbour[s]->next = NULL;
			tail[s] = neighbour[s];
		} else {
			NeighNode *p = (struct NeighNode*)malloc(sizeof(NeighNode));
			p->id = t;
			p->next = NULL;
			tail[s]->next = p;
			tail[s] = p;
		}
	}
	wayIn.close();		
}

void recyle() {
	cout<<"End..."<<endl;
	ofstream fout("neighbourInfo.txt");
	for (int i = 0; i < n; i++) {
		NeighNode* p = neighbour[i];
		NeighNode *q;
		if (p == NULL) continue;
		while (p != NULL) {
			fout<<p->id<<" ";
			q = p;
			p = p->next;
			delete q;
		}
		fout<<endl;
	}
	fout.close();
}


int main() {
	init();
	recyle();
	return 0;
}

