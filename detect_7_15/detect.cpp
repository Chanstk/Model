#include"Detector.h"

#define RATE 0.5
#define WINSIZE 20
#define WINSTEP 10
#define FACTOR 2

Detector::Detector(const int width, const int height):npd_table(256, 256, CV_8UC1)
{
	getNpdTable();
	getIndex(width, height);

	return ;
}

bool Detector::testModel(Mat& win)
{
	int T = model.size();
	float score = 0.0;
	int i = 0;
	for (; i < T; i++) {
		score += testDQT(model[i]->root, win);
		if (score < model[i]->threshold) break;
	}
	return i == T ? true : false;
	/*if (i == T) {
	  this->score.push_back(score);
	  return true;
	  } else {
	  return false;
	  }*/
}

/*float Detector::testDQT(xNode* &root, Mat& win)
  {
  xNode *p = root;
  xNode *q = p;
  unsigned char t_fea;
  for (int i = 0; i < 79800; i++) {
  unsigned char pixel1 = win.at<unsigned char>(
  index[ i ].p1);
  unsigned char pixel2 = win.at<unsigned char>(
  index[ i ].p2);
  t_fea = npd_table.at<unsigned char>((int)pixel1, (int)pixel2);
  cout<<int(t_fea)<<endl;
  }
  while (p) {
  q = p;
  unsigned char pixel1 = win.at<unsigned char>(
  index[ p->featId ].p1.x, index[ p->featId ].p1.y);
  unsigned char pixel2 = win.at<unsigned char>(
  index[ p->featId ].p2.x, index[ p->featId ].p2.y);
  unsigned char pixel1 = win.at<unsigned char>(
  index[ p->featId ].p1);
  unsigned char pixel2 = win.at<unsigned char>(
  index[ p->featId ].p2);
  t_fea = npd_table.at<unsigned char>((int)pixel1, (int)pixel2);

  cout<<"p->ID: "<<p->ID<<" "<<p->featId<<"t_fea: "<<(int)t_fea<<endl;
  p = t_fea < (unsigned char)(p->threshold1) ||
  t_fea > (unsigned char)(p->threshold2) ?
  p->lChild : p->rChild;
  }
  return t_fea < (unsigned char)(q->threshold1) ||
  t_fea > (unsigned char)(q->threshold2) ?
  q->leftFit : q->rightFit ;
  }*/

void Detector::getFea(Mat& win, vector<unsigned char>& fea)
{
	unsigned char *addr = win.data;
	for (int i = 0; i < 400; i++) {
		addr = win.data + i;
		for (int j = i + 1; j < 400; j++) {
			fea.push_back( (unsigned char) (npd_table.at<unsigned char>(*addr, *(addr - i + j))) );
		}
	}
	return ;
}

int getI(int dim){
	int sum = 0;
	int start = WINSIZE * WINSIZE - 1;
	for(int i = 0; ; i++){
		sum += start;
		start--;
		if(sum > dim)
			return i;
	}

}
unsigned char Detector::getFea(int dim, Mat& win)
{
	int i = getI(dim);
	int j = i + 1 + dim - i * (799 - i) /2 ;
	//Point p1( i / 20,  i % 20);
	//Point p2( j / 20, j % 20);
	int x1 = i / 20; 
	int y1 = i % 20; 
	int x2 = j / 20; 
	int y2 = j % 20; 
	int p1 = (int)win.at<unsigned char>(x1, y1);
	int p2 = (int)win.at<unsigned char>(x2, y2);
	return npd_table.at<unsigned char>(
			p1,p2);

	/*	return npd_table.at<unsigned char>(
		win.at<unsigned char>(p1),
		win.at<unsigned char>(p2));*/

}

/*unsigned char Detector::getFea(int dim, Mat& win)
  {
  int s = 0;
  int x = 0, y = 0;
  for (; s < dim + 1; x++)
  s += (399 - x);
  y = 399 - (s - dim - 1);
//x = (s - dim - 1) == 0 ? x : x - 1;
x--;
Point p1(x / 20, x % 20);
Point p2(y / 20, y % 20);
return npd_table.at<unsigned char>(
win.at<unsigned char>(p1),
win.at<unsigned char>(p2));
}*/

//int COUNT = 0;
float Detector::testDQT(xNode* &root, Mat& win)
{
	xNode *p = root;
	xNode *q = p;
	vector<unsigned char> fea;
	//getFea(win, fea);
	/*if (COUNT++ == 0)
	  for (int i = 0; i < 79800; i++) 
	  cout<<(int)fea[i]<<endl;*/
	unsigned char t_fea;
	while (p) {
		q = p;
		//t_fea = fea[ p->featId ];
		t_fea = getFea(p->featId, win);
		//cout<<"p->ID: "<<p->ID<<" "<<p->featId<<" "<<"t_fea: "<<(int)t_fea<<endl;
		p = t_fea < (unsigned char)(p->threshold1) ||
			t_fea > (unsigned char)(p->threshold2) ?
			p->lChild : p->rChild;
	}
	return t_fea < (unsigned char)(q->threshold1) ||
		t_fea > (unsigned char)(q->threshold2) ?
		q->leftFit : q->rightFit ;
}

void Detector::getIndex(int width, int height)
{
	int pixels = width * height;
	for (int i = 0; i < pixels; i++) {
		for (int j = i + 1 ; j < pixels; j++) {
			index.push_back(
					Pair(Point(i / width, i % width),
						Point(j / width, j % width)));
		}
	}
	/*for (int i = 0; i < 79800; i++) {
	  cout<<index[i].p1.x<<" "<<index[i].p1.y<<" "; cout<<index[i].p2.x<<" "<<index[i].p2.y<<endl;
	  }*/

	return ;
}

void Detector::getNpdTable()
{
	for (int i = 0; i < 256; i++) {
//#pragma omp parallel for
		for (int j = 0; j < 256; j++) {
			double fea = 0.5;
			if (i > 0 || j > 0) fea = double(i) / ((double)i + (double)j); 
			fea = floor(256 * fea);
			if (fea > 255) fea = 255;

			npd_table.at<unsigned char>(i, j) = (unsigned char) fea;
		}   
	}   
	return ;
}

void visit(xNode* root, int i)
{
	if (root) {
		visit(root->lChild, 2 * i); 
		cout<<i<<endl;
		cout<<root->leftFit<<" "<<root->rightFit<<" "
			<<(int)(root->threshold1)<<" "<<(int)(root->threshold2)<<" "
			<<(int)root->featId<<endl<<endl;
		visit(root->rChild, 2 * i + 1); 
	}
	return ;
}

void Detector::loadModel(const char *xml_file)
{
	xml_document doc;	
	xml_parse_result res = doc.load_file(xml_file);
	for (xml_node tree = doc.child("Tree"); tree; tree = tree.next_sibling("Tree")) {
		clear();  //clear the array
		for (xml_node node = tree.child("xNode"); node; node = node.next_sibling("xNode")) {
			xNode *root;
			aux_vec[ getNode(root, node) ] = root;  //flag
		}
		model.push_back( getDQT(tree) );
	}
	/*for (int i = 0; i < model.size(); i++) {
	  visit(model[i]->root, 1);
	  }*/
	return ;
}

void Detector::clear()
{
	aux_vec.clear();
	for (int i = 0; i < 513; i++) {
		aux_vec.push_back(NULL);
	}
	return ;
}

DQT* Detector::getDQT(xml_node& tree)
{
	DQT* dqt = new DQT();
	sscanf(tree.attribute("threshold").value(), "%f", &(dqt->threshold));
	getTree(1);
	dqt->root = aux_vec[1];

	return dqt;
}

void Detector::getTree(int i)
{
	if (aux_vec[i]) {
		aux_vec[i]->lChild = aux_vec[2 * i];
		getTree(2 * i);

		aux_vec[i]->rChild = aux_vec[2 * i + 1];
		getTree(2 * i + 1);
	}
	return ;
}

int  Detector::getNode(xNode* &root, xml_node& node)
{
	root = new xNode();
	sscanf(node.attribute("ID").value(), "%d", &(root->ID));
	sscanf(node.attribute("leftFit").value(), "%f", &(root->leftFit));
	sscanf(node.attribute("rightFit").value(), "%f", &(root->rightFit));
	sscanf(node.attribute("threshold1").value(), "%d", &(root->threshold1));
	sscanf(node.attribute("threshold2").value(), "%d", &(root->threshold2));
	sscanf(node.attribute("featID").value(), "%d", &(root->featId));

	return root->ID;
}

int Detector::detect(const char* ima_name)
{
	Mat ori_ima = imread(ima_name, CV_LOAD_IMAGE_GRAYSCALE);
	if(ori_ima.cols!= 20 || ori_ima.rows!=20){
//		cout<<ori_ima.cols<<" "<<ori_ima.rows<<endl;
		cv::resize(ori_ima, ori_ima, cv::Size(20,20));
	}
//	return testModel(ori_ima);
	/*Mat win1 = ori_ima;
	  unsigned char *addr = win1.data;
	  for (int i = 0; i < 400; i++) {
	  addr = win1.data + i;
	  for (int j = i + 1; j < 400; j++) {
	  cout<<(int)( (unsigned char) (npd_table.at<unsigned char>(*addr, *(addr - i + j))) )<<endl;
	  }
	  }*/

	//cvNamedWindow("test", CV_WINDOW_NORMAL);
	//imshow("test", ori_ima);
	//waitKey(0);
	for (float scale = 1.0; ori_ima.rows / scale >= WINSIZE && ori_ima.cols / scale >= WINSIZE;scale *= FACTOR) {  //scale the image
		Mat ima;
		resize(ori_ima, ima, Size(), 1.0 / scale, 1 / scale);
		//cvNamedWindow("test", CV_WINDOW_NORMAL); imshow("test", ori_ima); waitKey(0);
		vector<Rect> t_rect;

		scan(ima, t_rect, scale);  //scan current scaled image
		NPDClassify(t_rect);  //classify current subwin
		win.clear();
	}
	filter(RATE);  //filt
	return face_rect.size();
}

void Detector::scan(Mat& ima, vector<Rect>& t_rect, float scale)
{
//#pragma omp parallel for
	for (int i =0; i <= ima.rows - WINSIZE; i += WINSTEP) {
		for (int j = 0; j <= ima.cols - WINSIZE; j += WINSTEP) {
			t_rect.push_back(Rect(j * scale, i * scale, WINSIZE * scale, WINSIZE * scale));
			win.push_back( ima(Rect(j, i, WINSIZE, WINSIZE)) );
		}
	}
	return ;
}

void Detector::NPDClassify(vector<Rect>& t_rect)
{
//#pragma omp parallel for
	for (int i = 0; i < (int)t_rect.size(); i++)
		if (testModel(win[i])) face_rect.push_back(t_rect[i]);
}

float Detector::overlapRate(Rect& r1, Rect& r2)
{
	return float ((r1 & r2).area()) / r1.area();
}

void Detector::filter(float rate)
{
	//sort the face_rect
	int num = face_rect.size();
	vector<bool> flag(num, true);
	for (int i = 0; i < num; i++) {
		if (!flag[i]) continue;
		int track = i;
		for (int j = i + 1; j < num; j++) {
			if (overlapRate(face_rect[i], face_rect[j]) >= rate) {
				flag[track] = false;
				track = j;
				/*if (score[j] > score[track]) {
				  flag[track] = false;
				  track = j;
				  } else {
				  flag[j] = false;
				  }*/
			}
		}
	}
	int k = 0;
	for (int i = 0; i < num; i++)
		if (flag[i]) face_rect[k++] = face_rect[i];
	face_rect.resize(k);
	return ;
}

void Detector::drawRec(const char *ima_name)
{
	Mat ima = imread(ima_name, CV_LOAD_IMAGE_COLOR);
	cout<<face_rect.size()<<endl;
	for (int i = 0; i < (int)face_rect.size(); i++) {
		rectangle(ima, face_rect[i], Scalar(255, 0, 0));
		cvNamedWindow("test", CV_WINDOW_NORMAL);
		imshow("test", ima);
		waitKey(0);
	}
	return ;
}

