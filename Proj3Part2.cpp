#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <algorithm>
using namespace std;
typedef pair<float, int> fiPair;

map<string, map<string,float>> getData(string fileName, map<string,int>& keyValMap) {
	map<string, map<string,float>> returnMap;
	ifstream file(fileName);
	string name, line, word, correlation;
	int k = 1;
	for (int i = 0; i < 10000; i++) {
		map<string, float> fvs;
		getline(file, line);
		stringstream ss1(line);
		getline(ss1, name, '-'); //AAMAS02
		for (int j = 0; j < 10; j++) {
			getline(file, line);
			stringstream ss(line);
			getline(ss, word, ' ');
			getline(ss, correlation, ' ');
			float corf = stof(correlation);
			fvs[word] = corf;
		}
		map<string, float>::iterator it;
		for (it = fvs.begin(); it != fvs.end(); it++) {
			try {
				float val2 = returnMap.at(name).at(it->first);
				returnMap[name][it->first] = (val2 + it->second);
			}
			catch (exception e) {}

		}
		try {
			returnMap.at(name).insert(fvs.begin(), fvs.end());
		}
		catch (exception e) { 
			returnMap[name] = fvs;
			keyValMap[name] = k;
			k++;
		}

	}
	return returnMap;
}

float edgeAlg(map<string, float> map1, map<string, float> map2) {
	int inCommon = 0;
	float diff = 0;
	map<string, float>::iterator it;

	for (it = map1.begin(); it != map1.end(); it++) {
		try {
			float val2 = map2.at(it->first);
			inCommon++;
			diff += abs(val2 - it->second);
		}
		catch (exception e) {}

	}
	return ((inCommon - diff) / map1.size());
}

void adjListGen(map<string, map<string, float>> V, string fileName) {
	ofstream myfile;
	myfile.open(fileName);
	map<string, map<string, float>>::iterator it1;
	int i = 0;
	for (it1 = V.begin(); it1 != V.end(); it1++) {
		i++;
		map<string, map<string, float>>::iterator it2;
		myfile << i << " ";
		int j = 0;
		priority_queue<pair<float, int>> pq;
		for (it2 = V.begin(); it2 != V.end(); it2++) {
			j++;
			float edgeVal = edgeAlg(it1->second, it2->second);
			pair<float, int> temp = make_pair(edgeVal, j);
			pq.push(temp);
		}
		pq.pop();
		for (int k = 0; k < 50; k++) {
			myfile << pq.top().second << " " << pq.top().first << " ";
			pq.pop();
		}
		cout << "1 done" << endl;
		myfile << endl;
	}
}

void adjMatrixGen(string listFile, string matrixFile, int V) {
	ifstream ifile(listFile);
	ofstream ofile;
	ofile.open(matrixFile);
	for (int i = 0; i < 173; i++) { ofile << 0 << " "; }
	string line;
	while (getline(ifile, line)) {
		stringstream ss(line);
		string iStr; string valStr;
		getline(ss, iStr, ' ');
		float arr[173] = { 0 };
		ofile << endl;
		while (getline(ss, iStr, ' ')) {
			getline(ss, valStr, ' ');
			int i = stoi(iStr);
			float val = stof(valStr);
			arr[i] = val;
		}
		for (int i = 0; i < 173; i++) { ofile << arr[i] << " "; }
	}
}

vector<vector<float>> adjM(string filename) {
	ifstream ifile(filename);
	string line;
	vector<vector<float>> adjMatrix;
	while (getline(ifile, line)) {
		stringstream ss(line);
		string fStr;
		vector<float> v1;
		for (int i = 0; i < 173; i++) {
			getline(ss, fStr, ' ');
			float f = stof(fStr);
			v1.push_back(f);
		}
		adjMatrix.push_back(v1);
	}
	return adjMatrix;
}

vector<int> shortestPathM(vector<vector<float>> adjMatrix, int n, int V, int src) {

	vector<float> d(V, INT16_MAX);
	priority_queue<fiPair,vector<fiPair>,greater<fiPair>> pq; //strange implementation of float min-queue
	d[src] = 0;
	pq.push(make_pair(0, src));
	while (!pq.empty())
	{
		int u = pq.top().second;
		pq.pop();
		for (int i = 1; i < V; i++) {
			int v = i;
			float weight = (adjMatrix[u][i]);
			if (adjMatrix[u][i] != 0 && d[v] > d[u] + weight) {
				d[v] = d[u] + weight;
				pq.push(make_pair(d[v], v));
			}
		}
	}
	priority_queue<fiPair, vector<fiPair>, greater<fiPair>> pq2;
	vector<int> returnKeys;
	for (int i = 1; i < V; i++) {
		if (d[i] != 0) { pq2.push(make_pair(d[i], i)); }
		//cout << i << " " << d[i] << endl;
	}
	for (int i = 0; i < n; i++) {
		returnKeys.push_back(pq2.top().second);
		pq2.pop();
	}
	return returnKeys;
}

struct Edge {
	int src, dest;
	float weight;
	Edge(int _src, int _dest, float _weight) { src = _src; dest = _dest; weight = _weight; }
};

class Graph {
public:
	int numVertices;
	vector<vector<pair<int, float>>> adjList;

	Graph(const vector<Edge>& edges, int vertices) {
		numVertices = vertices;

		adjList.resize(vertices);

		for (auto& edge : edges) {
			adjList[edge.src].push_back(make_pair(edge.dest, edge.weight));
		}
	}
};

vector<float>dijkstra(Graph& graph, int src) {
	int vertices = graph.numVertices;
	vector<float> dist(vertices, 999);
	priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;
	dist[src] = 0;
	pq.push({ 0, src });
	while (!pq.empty()) {
		int u = pq.top().second;
		pq.pop();
		for (auto v : graph.adjList[u]) {
			if (dist[v.first] > dist[u] + v.second) {
				dist[v.first] = dist[u] + v.second;
				pq.push({ dist[v.first], v.first });
			}
		}
	}
	return dist;
}

vector<int> insertEdge(int source, int recAmount) {
	ifstream inFile("adjlist.txt");
	string nodeNumU, nodeNumV, line, dist;
	vector<Edge> edges;
	int n = 172;
	float dist1;

	while (getline(inFile, line)) {
		stringstream ss(line);
		getline(ss, nodeNumU, ' ');
		while (getline(ss, nodeNumV, ' ')) {
			getline(ss, dist, ' ');
			dist1 = stof(dist);
			edges.push_back({ stoi(nodeNumU), stoi(nodeNumV), dist1 });
		}
	}
	Graph graph(edges, 173);
	vector<float> dis = dijkstra(graph, source);

	//for (int i = 0; i < n; i++) {
	//	cout << i << "    " << dis[i] << endl; 
	//}
	priority_queue<fiPair, vector<fiPair>, greater<fiPair>> pq2;
	vector<int> returnKeys;
	for (int i = 1; i < 173; i++) {
		if (dis[i] != 0) { pq2.push(make_pair(dis[i], i)); }
	}

	for (int i = 0; i < recAmount; i++) {
		returnKeys.push_back(pq2.top().second);
		pq2.pop();
	}
	return returnKeys;
}


void printMenu() {
	cout << "1: Find Similar Publications from Adjacency List" << endl;
	cout << "2: Find Similar Publications from Adjacency Matrix" << endl;
	cout << "0: Exit Program" << endl;
	cout << "Please Enter a Selection: ";
}

int main()
{
    map<string,map<string,float>> testMap;
	map<string, int> keyValMap;
    testMap = getData("newdata.txt",keyValMap);
	vector<vector<float>> adjMat = adjM("adjmatrix.txt");
	int choice = 0;
	printMenu();
	while (cin >> choice) {

		if (choice == 0) {
			cout << "Terminating Program";
			break;
		}

		if (choice == 1) {
				cout << "Please enter a publication name and the last 2 digits of its date (e.g. AAMAS02):";
				string name;
				cin >> name;

				try {
					keyValMap.at(name);
					cout << "Please enter the amount of publication recommendations (recommended 5 to 30):";
					int n;
					cin >> n;
					vector<int> adjKeys = insertEdge(keyValMap[name],n);
					cout << "Recommended Publications: " << endl;
					for (int i = 0; i < adjKeys.size(); i++) {
						map<string, int>::iterator it;
						for (it = keyValMap.begin(); it != keyValMap.end(); it++) {
							if (it->second == adjKeys[i]) { cout << it->first << endl; }
						}
					}
				}
				catch (exception e) { cout << "ERROR: Publication not in database." << endl; }
		}

		if (choice == 2) {
			cout << "Please enter a publication name and the last 2 digits of its date (e.g. AAMAS02):";
			string name;
			cin >> name;
			
			try  {
				keyValMap.at(name);
				cout << "Please enter the amount of publication recommendations (recommended 5 to 30):";
				int n;
				cin >> n;
				vector<int> adjKeys = shortestPathM(adjMat, n, 173, keyValMap[name]);
				cout << "Recommended Publications: " << endl;
				for (int i = 0; i < adjKeys.size(); i++) {
					map<string, int>::iterator it;
					for (it = keyValMap.begin(); it != keyValMap.end(); it++) {
						if (it->second == adjKeys[i]) { cout << it->first << endl; }
					}
				}
			}
			catch (exception e) { cout << "ERROR: Publication not in database." << endl; }
		}

		else { cout << "ERROR: Choice out of range." << endl; }
		printMenu();
	}
}

