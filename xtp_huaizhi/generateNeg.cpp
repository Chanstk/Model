#include<iostream>
#include<fstream>
#include<string>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>
using namespace std;
using namespace cv;
const int WINSIZE = 20;
const int WINSTEP = 20;
const float SCALE = 1.1;
int n = 0;

void generateNeg(Mat& ima, const int xx)
{
	int yy = 0;
	string extension(".jpg");
    string path("/home/cst/NPD/NPD_A/MegaNeg/");
	for (int r = 0; r < ima.rows - WINSIZE; r += WINSTEP) {  //slide window
		for (int c = 0; c < ima.cols - WINSIZE; c += WINSTEP) {
			Mat roi(WINSIZE, WINSIZE, CV_8UC1);
			Rect rect(c, r, WINSIZE, WINSIZE);
			ima(rect).copyTo(roi);
			char x[20], y[20];
			sprintf(x, "%d", xx);
			sprintf(y, "%d", yy);
			string name(x);
			name = path + name + y + extension;
			imwrite(name.c_str(), roi);
			yy++;
			n++;
		}
	}
}

void process(const string& ima_name)
{
	Mat ori_ima = imread(ima_name, CV_LOAD_IMAGE_GRAYSCALE);
	static int x = 0;
	for (float scale = 1.0; ori_ima.cols / scale > WINSIZE && ori_ima.rows / scale > WINSIZE; scale *= SCALE) {  //scale the image
		Mat ima;
		resize(ori_ima, ima, Size(), 1 / scale, 1 / scale);
		
		generateNeg(ima, x);
		x++;
	}

	return ;
}

int main(int argc, char** argv)
{
	ifstream inf("a");
	string ima_name;
	while(getline(inf, ima_name)) {
		process(ima_name);  //process each image
	}
	cout<<n<<endl;

	return 0;
}
