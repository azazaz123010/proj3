#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include <map>
#include <utility>
#include <algorithm>
#include <queue>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

int main() {


	//ifstream file("trial.txt");//name of the text file);
	//priority_queue < pair<float, string>> pq;  // to create data for each line of a txt while //
	//string line; 
	//string s; 
	//float f;
	//string tempf;
	//while (getline(file,line)) {
	//	stringstream ss(line);
	//	getline(ss, s, ' ');
	//	getline(ss, tempf, ' ');
	//	pq.push(make_pair(stof(tempf), s));
	//}
	//pair <float, string> top;
	//for (int i = 0; i < 5; i++) {
	//	top = pq.top();
	//	cout << top.second << " " << top.first<<endl;
	//	pq.pop();
	//}

	std::cout << "start" << endl;
	int i = 0;
	string tempf; string line; string s; float f;
	string name;
	string page;
	string lastName;
	ofstream myfile;
	myfile.open("newdata.txt");
	for (auto itEntry = fs::recursive_directory_iterator("papers");
		itEntry != fs::recursive_directory_iterator();
		++itEntry) {
		const auto filePath = itEntry->path();
		if (filePath.extension() == ".txt") {
			priority_queue < pair<float, string>> pq;
			ifstream file(filePath.string());
			while (getline(file, line)) {
				stringstream ss(line);
				getline(ss, s, ' ');
				getline(ss, tempf, ' ');
				//pq.push(make_pair(stof(tempf), s));
				if (s != "the") {
					pq.push(make_pair(stof(tempf), s));
				}
			}
			pair <float, string> top;
			if (pq.size() >= 10) {
				i++;
				name = filePath.filename().string();
				if (!name.empty()) {
					name.resize(name.size() - 7);
					myfile << name << endl;
				}
				for (int i = 0; i < 10; i++) {
					top = pq.top();
					//cout << top.second << " " << top.first << endl;
					pq.pop();
					myfile << top.second << " " << top.first << endl;
				}
			}

		}


		if (i >= 10000) { break; }
	}
	myfile.close();
	std::cout << "done";
}
