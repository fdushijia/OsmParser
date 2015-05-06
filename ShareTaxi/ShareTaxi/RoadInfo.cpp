#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <vector>
#include <cstring>
#include <queue>
#include <stdlib.h> 
#include <time.h>
#include "Distance.h"
#include <malloc.h>

using namespace std;

#define ALL_NODES 435288      //图中Node的最大值


#define MAX_DIST  1000000000  //初始最大距离
#define RATIO     2           //这个系数用来控制SPFA剪枝  
#define BLOCK_NUM 18          //图中块的个数

float minDist[ALL_NODES];
bool  visit[ALL_NODES];
queue<int> q;

struct RoadNode {
	int blockNum, blockOffset, borderNum, blockBorderNum;
	float lat, lon;
	vector<int> distToBorder;
};

struct NeighNode {
	int id;
	float dist;
	NeighNode* next;
};
NeighNode* neighbour[ALL_NODES];
NeighNode* tail[ALL_NODES];
RoadNode roadNodes[ALL_NODES];

vector<int> borderroadNodes[BLOCK_NUM];
int n;



void init() {

	cout<<"Constructing the road..."<<endl;
	ifstream fin("Nodes_with_block_infos.txt");
	string idTemp, latTemp, lonTemp, blockNumTemp, blockOffsetTemp, borderNumTemp, blockBorderNumTemp;
	int i = 0;

	cout<<"Init..."<<endl;
	while (fin>>idTemp>>latTemp>>lonTemp>>blockNumTemp>>blockOffsetTemp>>borderNumTemp>>blockBorderNumTemp) {
		//cout<<i<<endl;
		roadNodes[i].lat = atof(latTemp.c_str());
		roadNodes[i].lon = atof(lonTemp.c_str());
		roadNodes[i].blockNum       = atoi(blockNumTemp.c_str());
        roadNodes[i].blockOffset    = atoi(blockOffsetTemp.c_str());
		roadNodes[i].borderNum      = atoi(borderNumTemp.c_str());
		roadNodes[i].blockBorderNum = atoi(blockBorderNumTemp.c_str());
		if (roadNodes[i].borderNum != -1) {
			borderroadNodes[roadNodes[i].blockNum].push_back(i);
		}
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
        //cout<<s<<" "<<t<<endl;
		if (neighbour[s] == NULL) {
			neighbour[s]       = (struct NeighNode *)malloc(sizeof(NeighNode));
			neighbour[s]->id   = t;
            //cout<<roadNodes[s].lat<<roadNodes[s].lon<<roadNodes[t].lat<<roadNodes[t].lon<<endl;
			neighbour[s]->dist = Distance::getDist(roadNodes[s].lat, roadNodes[s].lon, roadNodes[t].lat, roadNodes[t].lon);
			neighbour[s]->next = NULL;
			tail[s]            = neighbour[s];
		} else {
			NeighNode *p  = (struct NeighNode*)malloc(sizeof(NeighNode)); 
			p->id         = t;
			p->dist       = Distance::getDist(roadNodes[s].lat, roadNodes[s].lon, roadNodes[t].lat, roadNodes[t].lon);
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

	//delete(roadNodes);
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
void SPFA(int s) {
	initDist(s);
	q.push(s);
	int k;
	float lineDist;
	while (!q.empty()) {
		k = q.front();
		q.pop();
		visit[k] = false;
		NeighNode *p = neighbour[k];
		while (p) {
			if (minDist[k] + p->dist < minDist[p->id]) {
				minDist[p->id] = minDist[k] + p->dist;
				lineDist = Distance::getDist(roadNodes[s].lat, roadNodes[s].lon, roadNodes[p->id].lat, roadNodes[p->id].lon);
				if (!visit[p->id] /*&& minDist[p->id] <= 4 * lineDist*/) {
					visit[p->id] = true;
					q.push(p->id);
				}
			}
			p = p->next;

		}

	}
}

void getAllDistToBorder() {
	cout<<"get All Dist Border"<<endl;
	//cout<<SPFA(46274, 13988)<<endl;
	//int b;
	//cin>>b;
	ofstream fout("Dist_to_borders.txt");
	for (int i = 0; i < ALL_NODES; i++) {
		if (i >= 10000) break;
	    fout<<i<<endl;
		cout<<"Node: "<<i<<endl;
		if (neighbour[i] == NULL) continue;
		int label = roadNodes[i].blockNum;
		SPFA(i);
		for (int j = 0; j < borderroadNodes[label].size(); j++) {
			fout<<minDist[borderroadNodes[label][j]]<<" ";
		}
		fout<<endl;
		
	}

	fout.close();

}

int main() {
	init();

	time_t rawtime; 
	struct tm * timeinfo; 

	time(&rawtime); 
	timeinfo = localtime(&rawtime); 
	printf ("系统时间是: %s", asctime (timeinfo) ); 
	getAllDistToBorder();
	time(&rawtime); 
	timeinfo = localtime(&rawtime); 
	printf ("系统时间是: %s", asctime (timeinfo) ); 
	recycle();

	return 0;
}




