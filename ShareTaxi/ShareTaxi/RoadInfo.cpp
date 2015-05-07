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

#define ALL_NODES 435288      //ͼ��Node�����ֵ


#define MAX_DIST  1000000000  //��ʼ������
#define RATIO     2           //���ϵ����������SPFA��֦  
#define BLOCK_NUM 18          //ͼ�п�ĸ���

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
 * ��Ϊ·��ת����ͼ֮����ϡ��ͼ��������SPFA�㷨��Դ�����·��
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
        int labelK = roadNodes[k].blockNum;
		while (p) {
			int neighbourId = p->id;
            int labelP = roadNodes[neighbourId].blockNum;
            bool isBorder = (roadNodes[neighbourId].borderNum > 0);
			if (minDist[k] + p->dist < minDist[neighbourId] && (labelP == labelK || isBorder)) {
				minDist[neighbourId] = minDist[k] + p->dist;
				
				//lineDist = Distance::getDist(roadNodes[s].lat, roadNodes[s].lon, roadNodes[neighbourId].lat, roadNodes[neighbourId].lon);
				if (!visit[neighbourId]) {
					visit[neighbourId] = true;
					q.push(neighbourId);
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
	for (int i = 20000; i < ALL_NODES; i++) {
		//if (i >= 10000) break;
        //if (i > 58) break;
		cout<<"Node: "<<i<<endl;
		if (neighbour[i] == NULL) continue;
		int label = roadNodes[i].blockNum;
		int borderSize = borderroadNodes[label].size();
		fout<<i<<" "<<borderSize<<endl;
		SPFA(i);
		for (int j = 0; j < borderSize; j++) {
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
	printf ("ϵͳʱ����: %s", asctime (timeinfo) ); 
	getAllDistToBorder();
	time(&rawtime); 
	timeinfo = localtime(&rawtime); 
	printf ("ϵͳʱ����: %s", asctime (timeinfo) ); 
	recycle();

	return 0;
}




