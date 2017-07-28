#ifndef DETECTOR_H
#define DETECTOR_H

#include<iostream>
#include<string>
#include<fstream>
#include <stdio.h>
#include <vector>
#include <math.h>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<omp.h>
#include "pugixml.hpp"
using namespace pugi;
using namespace std;
using namespace cv;

class xNode {
	public:
		int ID;
		float leftFit, rightFit;
		unsigned char threshold1, threshold2;
		int featId;
		xNode *lChild, *rChild;

		xNode():ID(-1), leftFit(0), rightFit(0), threshold1(0), threshold2(0),
		featId(0) {}
};

class DQT {
	public:
		xNode *root;
		float threshold;

		DQT():root(NULL), threshold(0) {}
};

class Pair {
	public:
		Point p1;
		Point p2;

		Pair():p1(0, 0), p2(0, 0) {}
		Pair(Point pp1, Point pp2): p1(pp1), p2(pp2) {}
};

class Detector {
	public:
		vector<DQT*> model;
		vector<xNode*> aux_vec;
		vector<Rect> face_rect;
		vector<Mat> win;
		vector<float> score;

		vector<Pair> index;
		Mat npd_table;

	private:
		void getFea(Mat&, vector<unsigned char>&);
		void getNpdTable();
		void getIndex(const int, const int);
		int getNode(xNode* &root, xml_node& node);
		void getTree(int);
		DQT* getDQT(xml_node& tree);
		void clear();
		float testDQT(xNode* &root, Mat& subwin);
		bool testModel(Mat&);
		unsigned char getFea(Mat& subwin, int& fea_id);
		void scan(Mat&, vector<Rect>&, float);
		void NPDClassify(vector<Rect>&);
		void filter(float rate);
		float overlapRate(Rect& r1, Rect& r2);
		unsigned char getFea(int, Mat&);

	public:
		Detector(const int, const int);
		void loadModel(const char*);
		int detect(const char*);
		void drawRec(const char*);
};
#endif
