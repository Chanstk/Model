#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

int main(int argc, char** argv)
{
	ifstream inf(argv[1]);
	string name;
	vector<string> file;
	file.reserve(1000000);
	while(getline(inf, name)) {
		file.push_back(name);
	}
	inf.close();
	inf.clear();

	random_shuffle(file.begin(), file.end());

	ofstream of(argv[2]);
	for (int i = 0; i < file.size(); i++) {
		of<<file[i]<<endl;
	}
	of.close();
	
	return 0;
}
