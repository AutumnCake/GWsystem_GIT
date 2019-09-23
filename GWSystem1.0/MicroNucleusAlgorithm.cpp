#include "stdafx.h"
#include "MicroNucleusAlgorithm.h"


//细胞分割的头文件
#include "mywatershed.h"
#include "findUltra_erosion.h"

//细胞核分割的头文件
#define PI 3.1415926
#include"ErodeDilateSeparateMN.h"
#include"BGRMeanDeviation.h"
#include"CutOutGrayRGB.h"
#include "ReadAndWriteForAccess.h"

#include "python.h"
#include "numpy\core\include\numpy\arrayobject.h"

#include <direct.h>
#include<functional>
#include<numeric>
#include <string>

#include<iostream>
#include<fstream>
#include<string>
using namespace std;

extern PyObject* pFunc;
//全局变量
CReadAndWriteForAccess ReadMN;

vector <string>  imgNames4_1;
vector <string>  imgNames0_1;
vector <string>  imgNames5_2;
vector <string>  imgNames_imgvec; 
vector <string>  imgNames_imgvec_NoBlack;
vector<cv::Mat> imgvec;//存放待CNN识别的MAT数组

CMicroNucleusAlgorithm::CMicroNucleusAlgorithm()
{
	//pB->allcell = 0;
	//pB->singlecell = 0;
	//pB->doublecell = 0;
	//pB->multiplecell = 0;
	//pB->singlemn = 0;
	//pB->doublemn = 0;
	//pB->multiplemn = 0;
	m_Conn.OnInitADOAccess();
	SaveNum = 0;
}

CMicroNucleusAlgorithm::~CMicroNucleusAlgorithm()
{
}

int  CMicroNucleusAlgorithm::shape_judge(Mat img)
{
	RNG rng(12345);
	//cv::Mat src;
	cvtColor(img, img, CV_BGR2GRAY);
	int a = 0;
	vector<vector<Point> > contours;
	vector<Vec4i>hierarchy;
	Mat dst = Mat::zeros(img.rows, img.cols, CV_8UC3);
	//findContours(img, contours,hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);//原来的代码
	findContours(img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//更改-只取外轮廓
	Point p; int x, y, s;
	if (!contours.empty() && !hierarchy.empty())
	{
		int idx = 0;
		for (; idx >= 0; idx = hierarchy[idx][0])
		{
			Scalar color((rand() & 255), (rand() & 255), (rand() & 255));
			drawContours(dst, contours, idx, color, 1, 8, hierarchy);
		}
	}
	vector<vector<Point> > conPoint(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());
	for (int ii = 0; ii < contours.size(); ii++)
	{
		// 多边形近似
		approxPolyDP(Mat(contours[ii]), conPoint[ii], 1, true);
		// 最小外接矩形计算
		boundRect[ii] = boundingRect(Mat(conPoint[ii]));
		// 最小封闭圆生成
		minEnclosingCircle(conPoint[ii], center[ii], radius[ii]);
	}
	// 绘图显示
	Mat resultMat = Mat::zeros(img.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		// 随机生成颜色
		Scalar color = Scalar(rng.uniform(0, 255),
			rng.uniform(0, 255), rng.uniform(0, 255));

		float c = boundRect[i].height;
		float d = boundRect[i].width;
		double EI;
		double DR;

		int area1 = 1;//设定最小的面积值
		if (contourArea(contours[i])>area1)
		{
			double areacircle;
			areacircle = 3.1415926*radius[i] * radius[i];
			DR = (areacircle - contourArea(contours[i])) / contourArea(contours[i]);

			EI = min(c, d) / max(c, d);//延展率

			if ((EI >= 0.75) && (EI <= 1.0))
			{
				if ((DR >= 0) && (DR <= 0.5))
					a = 1;
			}

			return a;


		}
	}

	dst.release();
	resultMat.release();


	//contours.erase(contours.begin(), contours.end());
	//hierarchy.erase(hierarchy.begin(), hierarchy.end());
	//conPoint.erase(conPoint.begin(), conPoint.end());
	//boundRect.erase(boundRect.begin(), boundRect.end());
	//center.erase(center.begin(), center.end());
	//radius.erase(radius.begin(), radius.end());

	vector<vector<Point> >().swap(contours);
	vector<Vec4i>().swap(hierarchy);
	vector<vector<Point> >().swap(conPoint);
	vector<Rect>().swap(boundRect);
	vector<Point2f>().swap(center);
	vector<float>().swap(radius);

}

//生成随机颜色函数
Vec3b CMicroNucleusAlgorithm::RandomColorMN(int value)
{
	value = value * 20 + 50; //生成0~255的随机数  
	RNG rng;
	int aa = rng.uniform(0, value);
	int bb = rng.uniform(0, value);
	int cc = rng.uniform(0, value);
	return Vec3b(aa, bb, cc);
}

bool CMicroNucleusAlgorithm::Screenshot(IplImage* src, IplImage* dst, CvRect rect)
{
	cvSetImageROI(src, rect);
	cvCopy(src, dst, 0);
	cvResetImageROI(src);
	return 0;
}

// 安全重置矩形大小  
void CMicroNucleusAlgorithm::SafeResetSizeOfRect(IplImage* src, CvRect& rect)
{
	rect.x = rect.x < 0 ? 0 : rect.x;
	rect.y = rect.y < 0 ? 0 : rect.y;
	rect.width = rect.width < 0 ? 0 : rect.width;
	rect.height = rect.height < 0 ? 0 : rect.height;

	if (rect.x > src->width || rect.y > src->height)
	{
		rect = cvRect(0, 0, src->width, src->height);
	}
	rect.width = (std::min)(rect.width, src->width - rect.x);
	rect.height = (std::min)(rect.height, src->height - rect.y);
}

//去除非最大轮廓外其他杂质的子函数
void  CMicroNucleusAlgorithm::cacBounding(string pathName1, IplImage *src1)
{
	cv::Mat image = cv::Mat(src1);
	//Mat image = imread( pathName1, 1 );//读取原图
	cv::Mat src;
	cvtColor(image, src, CV_BGR2GRAY);//灰度图
	RNG rng(12345);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//cv::imshow("开始的src", src);
	/*	IplImage *src1;  */
	//const char *saveFilePath    =  pathName1.c_str();---可减少读取
	//src1=cvLoadImage(saveFilePath); //读取原图---可减少读取
	//src1=&IplImage(image);


	CvScalar cs, cs1;
	cvAvgSdv(src1, &cs, &cs1);//cvAvgSdv是用于求图像像素平均值与标准差的函数，求出的像素平均值赋予给cs，标准差赋予给cs1  
	int width = src1->width;/* 图像宽像素数 */
	int height = src1->height;/* 图像高像素数*/
	int step = src1->widthStep / sizeof(uchar);//opencv遍历图像，指针访问(单通道)
	// 计算边界轮廓
	findContours(src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	// 多边形逼近轮廓,获取矩形和圆形边界框
	vector<vector<Point> > conPoint(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());
	for (unsigned int i = 0; i < contours.size(); i++)
	{
		// 多边形近似
		approxPolyDP(Mat(contours[i]), conPoint[i], 3, true);
		// 最小外接矩形计算
		boundRect[i] = boundingRect(Mat(conPoint[i]));
		// 最小封闭圆生成
		minEnclosingCircle(conPoint[i], center[i], radius[i]);
	}
	// 绘图显示
	Mat resultMat = Mat::zeros(src.size(), CV_8UC3);
	for (unsigned int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(0, 0, 255);
		float c = boundRect[i].height;
		float d = boundRect[i].width;
		if (c*d>2000)
		{
			//// 绘制多边形轮廓
			drawContours(resultMat, conPoint, i, color,
				1, 8, vector<Vec4i>(), 0, Point());
			// 绘制多边形包围的矩形框
			rectangle(resultMat, boundRect[i].tl(),
				boundRect[i].br(), color, 2, 8, 0);
			circle(resultMat, center[i], radius[i],
				color, 2, 8, 0);

			cv::Point p1;
			//Point点类,Point类是一个包含两个整形数据成员x和y的以及一些简单成员方法的类类型, 
			/**********************************************************************************************
			*【1】二维空间中,点的类模板
			*【2】这个类定义了一个二维空间中的点,这个点的坐标可以被作为一个模板参数被指定.
			*【3】这个类也有一些比较短的别名可以方便用户的使用,比如:
			*     cv::Point, cv::Point2i, cv::Point2f and cv::Point2d
			*
			**********************************************************************************************/
			for (int x = 0; x < width; x++)
			for (int y = 0; y < height; y++)
			{

				if (pointPolygonTest(contours[i], cv::Point(x, y), false) == 1)
				{
					/*pointPolygonTest用于判断一个点是否在轮廓中
					当measureDist设置为true时，若返回值为正，表示点在轮廓内部，返回值为负，表示在轮廓外部，返回值为0，表示在轮廓上。
					当measureDist设置为false时，若返回值为+1，表示点在轮廓内部，返回值为-1，表示在轮廓外部，返回值为0，表示在轮廓上。*/
					p1 = Point(x, y);//点在轮廓内部

				}
				else{
					src.at<uchar>(y, x) = 0;
					//这是指将灰度图的第y行，第x列的置为灰度为0的黑色（0为黑色，255为白色）

				}


			}


		}
	}
	Mat img_masked;
	image.copyTo(img_masked, src);
	cv::imwrite(pathName1, img_masked);//保存滤波结果，生成文件

}

//pathName1作为到图片的路径，fileName2作为合格图像存储的路径，细胞分割后进行再次判定并写入到合格的细胞文件夹的子函数
void  CMicroNucleusAlgorithm::distinguish(string pathName1, string fileName2, string  pathName2, string fileName3)  //pathName1作为到图片的路径，fileName2作为合格图像存储的路径，细胞分割后进行再次判定并写入到合格的细胞文件夹的子函数
{
	RNG rng(12345);
	Mat img;
	img = imread(pathName1, 0);//0，代表灰度图

	Mat img2;
	Mat img3;
	//imshow(pathName1, img);
	if (img.empty())
	{
		cout << "Could not read input image file: " << pathName1 << endl;

	}

	//namedWindow("Img", 1);
	//imshow("Img", img);

	vector<vector<Point> > contours;
	vector<Vec4i>hierarchy;
	Mat dst = Mat::zeros(img.rows, img.cols, CV_8UC3);


	//findContours(img, contours,hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);//原来的代码
	findContours(img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//更改-只取外轮廓
	Point p; int x, y, s;

	if (!contours.empty() && !hierarchy.empty())
	{
		int idx = 0;
		for (; idx >= 0; idx = hierarchy[idx][0])
		{
			Scalar color((rand() & 255), (rand() & 255), (rand() & 255));
			drawContours(dst, contours, idx, color, 1, 8, hierarchy);
		}
	}


	vector<vector<Point> > conPoint(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		// 多边形近似
		approxPolyDP(Mat(contours[i]), conPoint[i], 3, true);
		// 最小外接矩形计算
		boundRect[i] = boundingRect(Mat(conPoint[i]));
		// 最小封闭圆生成
		minEnclosingCircle(conPoint[i], center[i], radius[i]);
	}
	// 绘图显示
	Mat resultMat = Mat::zeros(img.size(), CV_8UC3);

	const char* saveFilePath;
	const char* saveFilePath_1;

	for (int i = 0; i< contours.size(); i++)
	{
		// 随机生成颜色
		Scalar color = Scalar(rng.uniform(0, 255),
			rng.uniform(0, 255), rng.uniform(0, 255));



		float c = boundRect[i].height;
		float d = boundRect[i].width;
		double EI;
		double DR;

		int area1 = 4000;//设定最小的面积值，分割后的细胞可以设定小一点?//40X用4000，20X用2500
		int area2 = 15000;
		if ((contourArea(contours[i])<area2) && (contourArea(contours[i])>area1))
		{
			double areacircle;
			areacircle = 3.1415926*radius[i] * radius[i];
			DR = (areacircle - contourArea(contours[i])) / contourArea(contours[i]);

			EI = min(c, d) / max(c, d);//延展率

			//cout<<"第个矩形框的长和宽"<<","<<i<<endl;
			//cout<<c<<","<<d<<","<<EI;
			//cout <<endl;
			//cout<<contourArea(contours[i]);
			//cout <<endl;
			//cout<<DR;
			//cout <<endl;

			int a = 0;

			if ((EI >= 0.5) && (EI <= 1.0))
			{
				if ((DR >= 0) && (DR <= 0.7))   //是否也可以适当放宽？
					a = 1;
			}

			if (a == 1)//表示符合这几个条件，认为是完整的单个细胞
			{
				saveFilePath = fileName2.c_str();//将路径转换成 const char
				saveFilePath_1 = fileName3.c_str();//将路径转换成 const char

				img2 = imread(pathName1, 1);//1,代表彩图
				Rect rect(boundRect[i].x, boundRect[i].y, boundRect[i].width, boundRect[i].height);
				//delete saveFilePath;
				Mat image_roi = img2(rect);
				Mat up;
				int resize_height = 50;
				int resize_width = 50;
				cv::resize(image_roi, up, cv::Size(resize_width, resize_height), (0, 0), (0, 0), cv::INTER_LINEAR);
				//变成50X50，与识别模型相同
				imwrite(saveFilePath, up);//保存分割后，判断完整的细胞
				//delete[] pszDst;
				imgvec.push_back(up);
				imgNames_imgvec.push_back(saveFilePath);

				img3 = imread(pathName2, 1);//无黑框的图,读取路径
				Rect rect1(boundRect[i].x, boundRect[i].y, boundRect[i].width, boundRect[i].height);
				//delete saveFilePath;
				Mat image_roi1 = img3(rect1);
				Mat up2;
				cv::resize(image_roi1, up2, cv::Size(resize_width, resize_height), (0, 0), (0, 0), cv::INTER_LINEAR);
				imwrite(saveFilePath_1, up2);
			}
			//// 绘制多边形轮廓
			drawContours(resultMat, conPoint, i, color,
				1, 8, vector<Vec4i>(), 0, Point());
			// 绘制多边形包围的矩形框
			rectangle(resultMat, boundRect[i].tl(),
				boundRect[i].br(), color, 2, 8, 0);

			Scalar color1 = Scalar(0, 0, 255);

			circle(resultMat, center[i], radius[i],
				color1, 2, 8, 0);
		}


	}

	img.release();
	img2.release();
	dst.release();
	resultMat.release();

	//contours.erase(contours.begin(), contours.end());
	//hierarchy.erase(hierarchy.begin(), hierarchy.end());
	//conPoint.erase(conPoint.begin(), conPoint.end());
	//boundRect.erase(boundRect.begin(), boundRect.end());
	//center.erase(center.begin(), center.end());
	//radius.erase(radius.begin(), radius.end());

	vector<vector<Point> >().swap(contours);
	vector<Vec4i>().swap(hierarchy);
	vector<vector<Point> >().swap(conPoint);
	vector<Rect>().swap(boundRect);
	vector<Point2f>().swap(center);
	vector<float>().swap(radius);

}


//格式转换
string CMicroNucleusAlgorithm::WChar2Ansi(LPCWSTR pwszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);

	if (nLen <= 0) return std::string("");

	char* pszDst = new char[nLen];
	if (NULL == pszDst) return std::string("");

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;

	std::string strTemp(pszDst);
	delete[] pszDst;

	return strTemp;
}

// 利用winWIN32_FIND_DATA读取文件下的文件名
void CMicroNucleusAlgorithm::readImgNamefromFile(char* fileName, vector <string> &imgNames)
{
	// vector清零 参数设置
	imgNames.clear();
	WIN32_FIND_DATA file;
	int i = 0;
	char tempFilePath[MAX_PATH + 1];
	char tempFileName[1000];
	//vector<char> tempFileName;
	// 转换输入文件名
	sprintf_s(tempFilePath, "%s/*", fileName);
	// 多字节转换
	WCHAR   wstr[MAX_PATH] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, tempFilePath, -1, wstr, sizeof(wstr));
	// 查找该文件待操作文件的相关属性读取到WIN32_FIND_DATA
	HANDLE handle = FindFirstFile(wstr, &file);

	if (handle != INVALID_HANDLE_VALUE)
	{
		FindNextFile(handle, &file);
		FindNextFile(handle, &file);
		// 循环遍历得到文件夹的所有文件名    
		do
		{
			sprintf(tempFileName, "%s", fileName);
			imgNames.push_back(WChar2Ansi(file.cFileName));

			//imgNames[i].insert(0, tempFileName);
			i++;
		} while (FindNextFile(handle, &file));
	}
	FindClose(handle);
}

// 利用winWIN32_FIND_DATA读取文件下的文件名与上面有区别
void CMicroNucleusAlgorithm::readImgNamefromFileFG(char* fileName, vector <string> &imgNames)
{
	// vector清零 参数设置
	imgNames.clear();
	WIN32_FIND_DATA file;
	int i = 0;
	char tempFilePath[MAX_PATH + 1];
	char tempFileName[1000];
	//vector<char> tempFileName;
	// 转换输入文件名
	sprintf_s(tempFilePath, "%s/*", fileName);
	// 多字节转换
	WCHAR   wstr[MAX_PATH] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, tempFilePath, -1, wstr, sizeof(wstr));
	// 查找该文件待操作文件的相关属性读取到WIN32_FIND_DATA
	HANDLE handle = FindFirstFile(wstr, &file);
	if (handle != INVALID_HANDLE_VALUE)
	{
		FindNextFile(handle, &file);
		FindNextFile(handle, &file);
		// 循环遍历得到文件夹的所有文件名    
		do
		{
			sprintf(tempFileName, "%s", fileName);
			imgNames.push_back(WChar2Ansi(file.cFileName));
			imgNames[i] = tempFileName + imgNames[i];

			//imgNames[i].insert(0, tempFileName);
			i++;
		} while (FindNextFile(handle, &file));
	}
	FindClose(handle);
}

PyObject* StringToPyByWin(std::string str)
{
	int wlen = ::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), int(str.size()), NULL, 0);
	wchar_t* wszString = new wchar_t[wlen + 1];
	::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), int(str.size()), wszString, wlen);
	wszString[wlen] = '\0';
	PyObject* pobj = PyUnicode_FromUnicode((const Py_UNICODE*)wszString, wlen);
	delete wszString;
	return pobj;
}
//处理主函数
MN_HandleResult* CMicroNucleusAlgorithm::handlemicronucleus(string road1name1, string file1name1, string writename, CString patientname, MN_HandleResult *pB, string processfile)
{
	//将该照片的分析结果初始

	
	string read1name1 = road1name1 + file1name1;//读取的总路径

	cout << "read1name1:" << read1name1 << endl;
	//ofstream os("‪C:\\Users\\xibao\\Desktop\\2.txt");     //创建一个文件输出流对象
	FILE *fp = NULL;
	fp = fopen("E:\\1.txt", "w");
	fprintf(fp, read1name1.c_str());
	fclose(fp);

	int pos0_3 = read1name1.rfind("_"); //, s_file_path.length()
	string fn1_2 = read1name1.substr(pos0_3 + 1, read1name1.length());//读取路径最后的文件名,仅保留数字名字，不要中文
	string filename = "t" + fn1_2 + ".txt";

	//调用分析软件.exe
	//string str12 = "‪C:\\Users\\xibao\\Desktop\\opencv_cpp2py.exe.lnk";
	//string command_rd = "start " + str12;
	//cout <<" command_rd:"<< command_rd << endl;
	//system(command_rd.c_str());
	//WinExec("D:\\Desktop back up\\新建文件夹 (3)\\cpp_call_py\\111vector_predict - 已集成全流程-0820-py生成TXT\\x64\\Release\\opencv_cpp2py.exe", SW_SHOWMAXIMIZED);
	HINSTANCE hNewExe = ShellExecuteA(NULL, "open", "E:\\GWsystem\\111vector_predict - 已集成全流程-0820-py生成TXT\\x64\\Release\\opencv_cpp2py.exe", NULL, NULL, SW_HIDE);

	//从out.txt中读取处理结果
	bool nothing = true;
	fstream file;
	int ch;
	while (nothing)
	{
		string wholefilename = "E:\\tmp\\321\\" + filename;
		/*cout << "wholefilename" << wholefilename<<endl;*/

		int nFileExit = _access(wholefilename.c_str(), 0);
		if (nFileExit == 0)
		{
			//存在
			nothing = false;
			//读取TXT中内容，看是否为000
			FILE* pf = fopen(wholefilename.c_str(), "r");
			if (NULL == pf)
				return pB;
			char cstr[256];
			fscanf(pf, "%s", cstr);
			cout << cstr << endl;
			if (cstr == "000")
				return pB;
			else
			{
				//解析结果
				fstream fin(wholefilename);
				// 打开文件成功
				string aa[20000];
				int rowNum = 0;
				if (fin)
				{

					for (int i = 0; fin >> aa[i]; i++)
					{
						//cout<<"第 "<<(++rowNum)<<" 行数据："<<a[i] <<" i："<<i <<" " << endl;
						++rowNum;
					}

					fin.close();
				}
				else
				{
					cerr << "打开文件失败！" << endl;
				}
				;

				for (int i = 0; i<rowNum; i++)
				{
					//cout << "第 " << (i) << " 行数据：" << aa[i] << " " << endl;


				}


				char* cc4_3;
				const int len4_1 = aa[0].length();
				cc4_3 = new char[len4_1 + 1];
				strcpy(cc4_3, aa[0].c_str());
				char* fileName4_3 = cc4_3;
				//cout << "fileName4_3: " << fileName4_3 << endl;

				int a = 0;
				char *ptr;//释放内存
				char *p;
				char strs[] = " ";
				string result[3000];

				string result_name[1500];
				string result_num[1500];


				if (strcmp(fileName4_3, strs) != 0)
				{
					ptr = strtok_s(fileName4_3, ",", &p);
					while (ptr != NULL)
					{
						//从1开始
						//printf("ptr%d=%s\n", a, ptr);


						//cout << "ptr为：" << endl;
						//cout << ptr;
						//cout << endl;

						result[a] = ptr;



						//cout << "result[a]为：" << a<<endl;
						//cout << result[a];
						//cout << endl;
						a++;
						ptr = strtok_s(NULL, ",", &p);

					}
				}

				cout << "11111a: " << a << endl;
				for (int i = 1; i <a; i++)//从1开始，到a结束
				{
					if (i<((a + 1) / 2)){
						result_name[i] = result[i];
						//cout <<i<< "result_name[i]: " << result_name[i] << endl;
					}
					else{
						result_num[i - ((a + 1) / 2) + 1] = result[i];
						//cout << i - ((a + 1) / 2) + 1 << "result_num[i - ((a+1)/2)]: " << result_num[i - ((a + 1) / 2) + 1] << endl;
					}



				}
				int pos6_1;
				string fn6_1;
				//cout << "resul_name[i]: " << result_name[0] << endl;
				for (int i = 1; i < ((a + 1) / 2); i++)//从1开始
				{
					string strSaveNum;
					//SaveNum = i;
					int pos6_1 = result_name[i].rfind("\\");
					fn6_1 = result_name[i].substr(pos6_1 + 1, result_name[i].length());//读取路径最后的文件名
					int posforward = result[0].rfind("\\");
					string forwardname = result[0].substr(0, posforward);
					posforward = forwardname.rfind("\\");
					forwardname = forwardname.substr(0, posforward);
					posforward = forwardname.rfind("\\");
					forwardname = forwardname.substr(0, posforward);

					string wholename = forwardname + "\\wuhei-fuhe\\" + fn6_1;
					result_name[i] = wholename;

					cout << "[i]: " << i << endl;
					cout << "resul_name[i]: " << result_name[i] << endl;
					cout << "result_num[i]: " << result_num[i] << endl;

					WCHAR   wstr[MAX_PATH];
					char sucTempFilePath[MAX_PATH + 1];
					sprintf_s(sucTempFilePath, "%s", result_name[i].c_str());
					MultiByteToWideChar(CP_ACP, 0, sucTempFilePath, -1, wstr, sizeof(wstr));


					switch (stoi(result_num[i]))
					{
					case 0://普通双核
						//pB->DoubleCellsWithMN[0]++;
						//DoubleCellsWithMN[0]++;
						//pB->doublecell++;
						//pB->sumcell++;
						pB->DoubleCellsWithMN[1]++;
						DoubleCellsWithMN[1]++;//无法判断双核细胞中的微核数量
						pB->doublecell++;
						pB->sumcell++;
						pB->doublecell_wh++;//含有微核的单核细胞数
						// 有双核微核，保存图片和结果
						char DesTempFilePath[MAX_PATH + 1];
						WCHAR   wstr_des[MAX_PATH];
						//strSaveNum = to_string(SaveNum);
						strSaveNum = fn6_1;
						sprintf_s(DesTempFilePath, "%s", (writename + strSaveNum).c_str());
						MultiByteToWideChar(CP_ACP, 0, DesTempFilePath, -1, wstr_des, sizeof(wstr_des));
						CopyFile(wstr, wstr_des, TRUE);
						OnePicResult->picpath = (writename + strSaveNum).c_str();
						OnePicResult->NumCells = 2;
						OnePicResult->NumMicro = 1;
						OnePicResult->patientname = patientname;
						//将该张照片的分析结果存到数据库
						ReadMN.SaveOnePicMNResult(OnePicResult);
						SaveNum++;
						break;
					case 3:
						pB->SingleCellsWithMN[0]++;
						SingleCellsWithMN[0]++;
						pB->singlecell++;
						pB->sumcell++;

						break;
					case 1://双核为何
						//pB->DoubleCellsWithMN[1]++;
						//DoubleCellsWithMN[1]++;//无法判断双核细胞中的微核数量
						//pB->doublecell++;
						//pB->sumcell++;
						//pB->doublecell_wh++;//含有微核的单核细胞数
						//// 有双核微核，保存图片和结果
						//char DesTempFilePath[MAX_PATH + 1];
						//WCHAR   wstr_des[MAX_PATH];
						////strSaveNum = to_string(SaveNum);
						//strSaveNum = fn6_1;
						//sprintf_s(DesTempFilePath, "%s", (writename + strSaveNum).c_str());
						//MultiByteToWideChar(CP_ACP, 0, DesTempFilePath, -1, wstr_des, sizeof(wstr_des));
						//CopyFile(wstr, wstr_des, TRUE);
						//OnePicResult->picpath = (writename + strSaveNum).c_str();
						//OnePicResult->NumCells = 2;
						//OnePicResult->NumMicro = 1;
						//OnePicResult->patientname = patientname;
						////将该张照片的分析结果存到数据库
						//ReadMN.SaveOnePicMNResult(OnePicResult);
						//SaveNum++;

						break;

					case 5:
						TripleCellsWithMN[0]++;
						pB->TripleCellsWithMN[0]++;
						pB->multiplecell++;
						pB->sumcell++;
						break;
					case 4:
						MultiCellsWithMN[0]++;
						pB->MultiCellsWithMN[0]++;
						pB->multiplecell++;
						pB->sumcell++;
						break;
					}

					DeleteFile(wstr);

				}

				vector<string>().swap(imgNames4_1);
				vector<string>().swap(imgNames0_1);
				//imgNames0_1.erase(imgNames0_1.begin(),imgNames0_1.end());
			}
			fclose(pf);


			WCHAR   wstr[MAX_PATH];
			char sucTempFilePath[MAX_PATH + 1];
			sprintf_s(sucTempFilePath, "%s", wholefilename.c_str());
			MultiByteToWideChar(CP_ACP, 0, sucTempFilePath, -1, wstr, sizeof(wstr));
			DeleteFile(wstr);
		}
		else
		{
			//不存在
			nothing = true;
		}

	}



	//for (int i = 1; i < a / 2 + 1; i++)//从1开始file3name6
	//{


	//	int pos6_1 = resul_name[i].rfind("\\");
	//	fn6_1 = resul_name[i].substr(pos6_1 + 1, resul_name[i].length());//读取路径最后的文件名
	//	resul_name[i] = fn6_1;

	//	//cout << "resul_name[i]: " << file3name2 + "\\"+resul_name[i] << endl;
	//	//file3name4 + "\\" + resul_name[i]
	//	WCHAR   wstr[MAX_PATH];
	//	char sucTempFilePath[MAX_PATH + 1];
	//	sprintf_s(sucTempFilePath, "%s", (file3name4 + "\\" + resul_name[i]).c_str());
	//	MultiByteToWideChar(CP_ACP, 0, sucTempFilePath, -1, wstr, sizeof(wstr));


	//	cout << "result_num[i]: " << result_num[i] << endl;
	//	String strSaveNum;
	//	switch (stoi(result_num[i]))
	//	{
	//	case 0:
	//		pB->DoubleCellsWithMN[0]++;
	//		DoubleCellsWithMN[0]++;
	//		pB->doublecell++;
	//		pB->sumcell++;
	//		break;
	//	case 1:
	//		pB->DoubleCellsWithMN[1]++;
	//		DoubleCellsWithMN[1]++;//无法判断双核细胞中的微核数量
	//		pB->doublecell++;
	//		pB->sumcell++;
	//		pB->doublecell_wh++;//含有微核的单核细胞数
	//		// 有双核微核，保存图片和结果
	//		char DesTempFilePath[MAX_PATH + 1];
	//		WCHAR   wstr_des[MAX_PATH];
	//		strSaveNum = to_string(SaveNum);
	//		sprintf_s(DesTempFilePath, "%s", (writename + strSaveNum + ".bmp").c_str());
	//		MultiByteToWideChar(CP_ACP, 0, DesTempFilePath, -1, wstr_des, sizeof(wstr_des));
	//		CopyFile(wstr, wstr_des, TRUE);
	//		OnePicResult->picpath = (writename + strSaveNum + ".bmp").c_str();
	//		OnePicResult->NumCells = 2;
	//		OnePicResult->NumMicro = 1;
	//		OnePicResult->patientname = patientname;
	//		//将该张照片的分析结果存到数据库
	//		ReadMN.SaveOnePicMNResult(OnePicResult);
	//		SaveNum++;

	//		break;
	//	case 2:
	//		pB->SingleCellsWithMN[0]++;
	//		SingleCellsWithMN[0]++;
	//		pB->singlecell++;
	//		pB->sumcell++;

	//		break;
	//	case 3:
	//		TripleCellsWithMN[0]++;
	//		pB->TripleCellsWithMN[0]++;
	//		pB->multiplecell++;
	//		pB->sumcell++;
	//		break;
	//	case 4:
	//		MultiCellsWithMN[0]++;
	//		pB->MultiCellsWithMN[0]++;
	//		pB->multiplecell++;
	//		pB->sumcell++;
	//		break;
	//	}

	//	DeleteFile(wstr);

	//}


	//删除原图
	//char sourcePicPath[MAX_PATH + 1];
	//WCHAR   wstr_sourpic[MAX_PATH];
	//sprintf_s(sourcePicPath, "%s", (road1name1 + "\\" + file1name1).c_str());
	//MultiByteToWideChar(CP_ACP, 0, sourcePicPath, -1, wstr_sourpic, sizeof(wstr_sourpic));
	//int del = DeleteFile(wstr_sourpic);
	return pB;//返回该病人的处理结果

	}


