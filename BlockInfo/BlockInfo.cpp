#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define ALL_NODES 435289      //图中Node的最大值
#define BLOCK_NUM 18
int blockNum[ALL_NODES];
float lat[ALL_NODES], lon[ALL_NODES];
int blockOffset[ALL_NODES];
int borderNum[ALL_NODES];
int nums[BLOCK_NUM];
int countBorders[BLOCK_NUM];
int blockBorderNum[BLOCK_NUM];
int curBorders = 0;

void init() {
	ifstream fin("Node_With_Labels.txt");
	cout<<"Input nodes ..."<<endl;
	for (int i = 0; i < BLOCK_NUM; i++) {
		nums[i] = 0;
	}

	for (int i = 0; i < ALL_NODES; i++) {
		borderNum[i] = -1;
		blockBorderNum[i] = -1;
	}
	string latTemp, lonTemp, idTemp, label;
	int i = 0;
	while (fin>>idTemp>>latTemp>>lonTemp>>label) {
		lat[i] = atof(latTemp.c_str());
		lon[i] = atof(lonTemp.c_str());
		blockNum[i] = atoi(label.c_str());
		blockOffset[i] = nums[blockNum[i]] ++;
		i ++;
	}
	fin.close();

	cout<<"Input ways..."<<endl;
	ifstream wayIn("newWays_long.txt");
	string ss, tt;
	int s, t;
	while (wayIn>>ss>>tt) {
		s = atoi(ss.c_str());
		t = atoi(tt.c_str());
		if (blockNum[s] != blockNum[t]) {
			//不在同一个块内
			if (borderNum[s] == -1) {
				borderNum[s] = curBorders ++;
				blockBorderNum[s] = countBorders[blockNum[s]] ++;

			}

			if (borderNum[t] == -1) {
				borderNum[t] = curBorders ++;
				blockBorderNum[t] = countBorders[blockNum[t]] ++;
			}
		}
	}
	wayIn.close();
}

void output() {
	cout<<"Output ..."<<endl;
	ofstream fout("Nodes_with_block_infos.txt");
	for (int i = 0; i < ALL_NODES; i++) {
		fout<<i<<" "<<lat[i]<<" "<<lon[i]<<" "<<blockNum[i]<<" "<<blockOffset[i]<<" "<<borderNum[i]<<" "<<blockBorderNum[i]<<endl;
	}
	fout.close();

	ofstream blockout("Block_infos.txt");
	for (int i = 0; i < BLOCK_NUM; i++) {
		blockout<<"Block "<<i<<" :"<<" "<<countBorders[i]<<endl;
	}
	blockout.close();
}
int main() {
	init();
	output();
	return 0;
}