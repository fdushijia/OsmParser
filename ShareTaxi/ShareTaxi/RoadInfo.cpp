#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <vector>
using namespace std;

class RoadInfo {

	struct NeighNode {
		string id;
		double lat, lon;
	};
	
	/**
	 * ·���е�Node��id��Ϣ����γ����Ϣ
	 * neighbour���list�����洢���ڵ�Node�ı��
	 */
	struct Node {
		double lat, lon;
		string id;
		list <NeighNode *> neighbour;
	};


	
	list<Node*> hashNode[10000]; //�洢�ڵ��hash��
public:

	/**
	 * ��ʼ��ͼ����
	 */

	RoadInfo(string nodeFileName, string wayFileName) {
		
		cout<<"Constructing nodes..."<<endl;

		//��ʼ���ڵ���Ϣ
		ifstream nodeFin(nodeFileName);
		if (!nodeFin) {
			cout<<"File not found"<<endl;
		} else {
			string id, lat, lon;
			while (nodeFin>>id>>lat>>lon) {
				Node *node = new Node();
				node->id = id;
				node->lat = atof(lat.c_str());
				node->lon = atof(lon.c_str());
				int hashNum = getHashNumber(id);
				hashNode[hashNum].push_back(node);

			}
			nodeFin.close();
		}

		//��ʼ���ߵ���Ϣ
		cout<<"Constructing roads..."<<endl;
		ifstream wayFin(wayFileName);
		if (!wayFin) {
			cout<<"File not found"<<endl;
		} else {
			string s, t;
			list<Node *>::iterator node, nd, endNode, endNd;
			int num;
			while (wayFin>>s>>t) {
				int hashNum = getHashNumber(s);
				int num     = getHashNumber(t);
				endNode     = hashNode[hashNum].end();
				for (node = hashNode[hashNum].begin(); node != endNode; ++node) {
					if ((*node)->id == s) {
						endNd = hashNode[num].end();
						for (nd = hashNode[num].begin(); nd != endNd; nd++) {
							if ((*nd)->id == t){
								NeighNode * neighbourNode = new NeighNode();
								neighbourNode->id = t;
								neighbourNode->lat = ((*nd)->lat);
								neighbourNode->lon = ((*nd)->lon);
								(*node)->neighbour.push_back(neighbourNode);
							}
						}
						break;
					}
				}
			}
			wayFin.close();
		}
	}

	void getOutputFile(string filename) {
		cout<<"Generating output..."<<endl;
		ofstream fout(filename);
		list<Node *>::iterator node, endNode;
		list<NeighNode *>::iterator neighbourNode, endNeighbour;
		for (int i = 0; i < 10000; i++) {
			fout<<i<<" "<<hashNode[i].size()<<endl;
			endNode = hashNode[i].end();
			for (node = hashNode[i].begin(); node != endNode; node ++) {
				fout<<(*node)->id<<" "<<(*node)->lat<<" "<<(*node)->lon<<" "<<(*node)->neighbour.size()<<endl;
				endNeighbour = (*node)->neighbour.end();
				for (neighbourNode = (*node)->neighbour.begin(); neighbourNode != endNeighbour; neighbourNode++) {
					fout<<(*neighbourNode)->id<<" "<<(*neighbourNode)->lat<<" "<<(*neighbourNode)->lon<<" "<<endl;
				}
				fout<<endl;
			}
		}
		fout.close();
	}

	~RoadInfo() {
		cout<<"end..."<<endl;
		list<Node *>::iterator j, endJ;
		list<NeighNode *>::iterator k, endK;
		for (int i = 0; i < 10000; i++) {
			endJ = hashNode[i].end();
			for (j = hashNode[i].begin(); j != endJ; ++j) {
				endK = (*j)->neighbour.end();
				for (k = (*j)->neighbour.begin(); k != endK;k++)
					delete *k;
				delete *j;
			}
		}
	}

private:
	int getHashNumber(string id) {
		long number = atol(id.c_str());
		return number % 10000;
	}

};

int main() {
	RoadInfo roadInfo("nodes.txt", "ways.txt");
	roadInfo.getOutputFile("road.txt");
	return 0;
}