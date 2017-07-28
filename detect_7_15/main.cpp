//#include<fstream>
#include"Detector.h"

int main(int argc, char* argv[])
{
	int line1 = 0;
	int TP = 0;
	int FN = 0;
	//ifstream inf(argv[1]);
	ifstream inf(argv[2]);  //pos sample
	string name;
	while (getline(inf, name)) {
		line1++;
		Detector det(20, 20);
		det.loadModel(argv[1]);
		//det.detect(name.c_str());
		int t = det.face_rect.size();
		if (det.detect(name.c_str())) {
			TP ++;
		}
		//det.drawRec(name.c_str());
	}
	inf.close();
	int FP = 0;
	int TN = 0;
	int line2 = 0;
	ifstream inf1(argv[3]);  //neg sample
	while (getline(inf1, name)) {
		line2 ++;
		Detector det(20, 20);
		det.loadModel(argv[1]);
	//	det.detect(name.c_str());
		int t = det.face_rect.size();
		if (det.detect(name.c_str())) {
			FP ++;
		}
		//det.drawRec(name.c_str());
	}
	cout<<"TP: "<<TP<<endl;
	cout<<"FN: "<<(FN = line1 - TP)<<endl;
	cout<<"TN: "<<(TN = line2 - FP)<<endl;
	cout<<"FP: "<<FP<<endl;

	cout<<"P: "<<(double)TP / (TP + FP)<<endl;
	cout<<"R: "<<(double)TP / (TP + FN)<<endl;
	
	return 0;
}
