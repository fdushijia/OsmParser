#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <vector>
#include <cstring>
#include <queue>
#include "Distance.h"

using namespace std;

#define ALL_NODES 500000
#define MAX_DIST  1000000000

float lat[500000], lon[500000];
float minDist[500000];
bool  visit[500000];
queue<int> q;

struct NeighNode {
	int id;
	float dist;
	NeighNode* next;
};
NeighNode* neighbour[500000];
NeighNode* tail[500000];
int n;



void init() {

	cout<<"Constructing the road..."<<endl;
	ifstream fin("newNodes.txt");
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
	ifstream wayIn("newWays.txt");
	string ss, tt;
	int s, t;
	while (wayIn>>ss>>tt) {
		s = atoi(ss.c_str());
		t = atoi(tt.c_str());
		if (neighbour[s] == NULL) {
			neighbour[s]       = (struct NeighNode *)malloc(sizeof(NeighNode));
			neighbour[s]->id   = t;
			neighbour[s]->dist = Distance::getDist(lat[s], lon[s], lat[t], lon[t]);
			neighbour[s]->next = NULL;
			tail[s]            = neighbour[s];
		} else {
			NeighNode *p  = (struct NeighNode*)malloc(sizeof(NeighNode)); 
			p->id         = t;
			p->dist       = Distance::getDist(lat[s], lon[s], lat[t], lon[t]);
			p->next       = NULL;
			tail[s]->next = p;
			tail[s]       = p;
		}
	}
	wayIn.close();		
}

void recycle() {
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

void initDist(int s) {
	q.empty();
	for (int i = 0; i < ALL_NODES; i++) {
		minDist[i] = MAX_DIST;
		visit[i]   = false;
	}
	minDist[s] = 0;
}

/**
 * 因为路网转化成图之后是稀疏图，这里用SPFA算法求单源点最短路径
 */
float SPFA(int s, int t) {
	initDist(s);
	q.push(s);
	int k;
	while (!q.empty()) {
		k = q.front();
		q.pop();
		visit[k] = false;
		NeighNode *p = neighbour[k];
		while (p) {
			if (minDist[k] + p->dist < minDist[p->id]) {
				minDist[p->id] = minDist[k] + p->dist;
			}
			if (!visit[p->id]) {
				visit[p->id] = true;
				q.push(p->id);
			}
			p = p->next;
		}

	}
	return minDist[t];
}

int main() {
	init();
	
	cout<<SPFA(46274, 46293)<<endl;
	recycle();

	return 0;
}

