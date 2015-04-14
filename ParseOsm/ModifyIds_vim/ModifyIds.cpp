#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
using namespace std;

#define n 500000;

long originId[500000];
double lat[500000], lon[500000];

int compareints (const void * a, const void * b)  
{  
  return ( *(int*)a - *(int*)b );  
}  
  

int main() {
	ifstream fin("nodes.txt");
	
	string oldId;
	string latTemp, lonTemp;
	int i = 0;
	while (!fin.eof()) {
		fin>>oldId>>latTemp>>lonTemp;
		originId[i] = atol(oldId.c_str());
		//fout<<i<<" "<<latTemp<<" "<<lonTemp<<endl;
		lat[i] = atof(latTemp.c_str());
		lon[i] = atof(lonTemp.c_str());
		i++;
	}
	fin.close();
	int allNodes = i;
	ofstream fout("newNodes.txt");
	for (int i = 0; i < allNodes; i++) {
		fout<<i<<" "<<lat[i]<<" "<<lon[i]<<endl;
        //if (i >= 50000) return 0;
    }
	fout.close();
	ifstream finway("ways.txt");
	ofstream foutway("newWays.txt");
	string ss, tt;
	while (finway>>ss>>tt) {
		
		long* pItem;  
		long key = atol(ss.c_str());
		pItem = (long*) bsearch (&key, originId, allNodes, sizeof (long), compareints);  

		int sId = pItem - originId;

		key = atol(tt.c_str());
		pItem = (long*) bsearch (&key, originId, allNodes, sizeof (long), compareints);
		int tId = pItem - originId;

		foutway<<sId<<" "<<tId<<endl;
	}
	finway.close();
	foutway.close();
}

