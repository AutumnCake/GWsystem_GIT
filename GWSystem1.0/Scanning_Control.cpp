#include "stdafx.h"
#include "Scanning_Control.h"
#include "lucamerr.h"

#include"GWSystem1.0.h"

//静态变量
bool Scanning_Control::Handlehold_Thread_Running = false;
bool Scanning_Control::CA_Focus_Thread_Running = false;
vector<Mat> Scanning_Control::Scanning_10X_img;

//Tenengrad 梯度函数
double Thenengrad(Mat &img)
{
	double Grad_value = 0;
	double Sx, Sy;
	for (int i = 1; i < img.rows - 1; i++)
	{
		//定义行指针
		uchar *current_ptr = (uchar*)img.data + i * img.cols;//当前行
		uchar *pre_ptr = (uchar*)img.data + (i - 1)*img.cols;//上一行
		uchar *next_ptr = (uchar*)img.data + (i + 1)*img.cols;//下一行
		for (int j = 1; j < img.cols - 1; j++)
		{
			Sx = pre_ptr[j - 1] * (-1) + pre_ptr[j + 1] + current_ptr[j - 1] * (-2) + current_ptr[j + 1] * 2 + next_ptr[j - 1] * (-1) + next_ptr[j + 1];//x方向梯度
			Sy = pre_ptr[j - 1] + 2 * pre_ptr[j] + pre_ptr[j + 1] - next_ptr[j - 1] - 2 * next_ptr[j] - next_ptr[j + 1];//y方向梯度
			//求总和
			Grad_value += Sx * Sx + Sy * Sy;
		}
	}
	return Grad_value / (img.cols - 2) / (img.rows - 2);
}


//输入必须是二值图，
double Cut_ROI_Compute_Focus(Mat Intput_Image, vector<vector<Point>> contours, unsigned int Compute_Operator, bool Masked_Img = false)
{
	//3.遍历轮廓，找出正外接矩形
	double Whole_Focus_Value = 0;
	Mat mask = Mat::zeros(Intput_Image.size(), CV_8U);//同原图一样大小，纯黑的mask图像//CV_8UC3
	Mat draw = Mat::zeros(Intput_Image.size(), CV_8U);//将原图copyTo到draw上，加上mask操作
	Mat temp = Mat::zeros(Intput_Image.size(), CV_8U);//每次循环，重置mask和draw

	Rect boundRect;//存储轮廓的正外接矩形

	int size_CONUTORSS = contours.size();

	for (int i = 0; i < contours.size(); i++)//遍历每个轮廓
	{
		//Scalar color((rand() & 255), (rand() & 255), (rand() & 255));//随机产生颜色 rand()产生随机数
		Mat ROI;
		boundRect = boundingRect(Mat(contours[i]));//获得第i个轮廓的正外接矩形
		if (Masked_Img)
		{
			temp.copyTo(mask);//重置mask
			temp.copyTo(draw);//重置draw
			drawContours(mask, contours, i, Scalar(255), -1);//在mask上用白色绘制轮廓，-1向内填充
			//namedWindow("mask", WINDOW_NORMAL);
			//imshow("mask", mask);
			Intput_Image.copyTo(draw, mask);//将原图copyTo到draw上，加上mask操作

			ROI = draw(boundRect);//定义ROI，即为draw上的boundRect区域

		}
		else
		{
			ROI = Intput_Image(boundRect);
		}
		double Focus_Mean;
		Mat meanValueImage;
		Mat meanStdValueImage;
		Mat LapValueImage;
		switch (Compute_Operator)
		{
		case Laplacian_Operator:
			Laplacian(ROI, LapValueImage, CV_16U);
			Focus_Mean = mean(LapValueImage)[0];
			break;
		case MeanStd_Operator:

			meanStdDev(ROI, meanValueImage, meanStdValueImage);
			Focus_Mean = meanStdValueImage.at<double>(0, 0);
			break;
		case Thenengrad_Operator:
			Focus_Mean = Thenengrad(ROI);
			break;
		default:
			break;
		}

		Whole_Focus_Value += Focus_Mean;

		//namedWindow("srcImg_ROI", WINDOW_NORMAL);
		//imshow("srcImg_ROI", ROI);
		///waitKey(0);

	}

	return Whole_Focus_Value;

}


// 获取大轮廓的区域
vector<vector<Point>> getSizeContours(vector<vector<Point>> &contours, unsigned int &Max_Aero)
{
	int kk0 = contours.size();
	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		unsigned int contour_Aero = contourArea(*itc);
		if (contour_Aero > Max_Aero)
			Max_Aero = contour_Aero;
		itc++;
	}
	////int kk = contours.size();	//if (Max_Aero < 20000)
	//	return contours;

	itc = contours.begin();
	vector<vector<Point>> contours_Max;
	while (itc != contours.end())
	{
		unsigned int contour_Aero = contourArea(*itc);
		if (contour_Aero >(Max_Aero/2))
			contours_Max.push_back(*itc);
		itc++;
	}
	return contours_Max;

}

Scanning_Control::Scanning_Control()
{
	m_hCamera = NULL;                      //相机句柄
	m_bConnected = false;						//建立连接？
	m_bPreviewing = false;						//预览打开
	m_fFrameRate = 100000.0;				// make it run as fast as possible  相机帧率
	m_ulCameraId = 0;						//相机ID
	m_bIsColor = true;						//是否为彩色相机
	m_MotorConnected=false;
	m_fZoom = 0.25;
	m_XVelocty_Model_Starting = false;			//速度模式运行中
	m_YVelocty_Model_Starting = false;
	m_MotorConnected = false;
	m_CameraConnected = false;
	m_StartCapture_Image = false;//启动抓图标志位
	m_EndCapture_Image = false;//关闭自动抓图标志
	m_Sobel_Wrong = false;
	m_Focusing_Equal_Scaning = false;
	m_ScanningImage_Save_Paras.csDiretory = _T("C:\\Users\wench\\Desktop\\focus");
	m_ScanningImage_Save_Paras.csExtension = ".bmp"; //".raw";
	m_ScanningImage_Save_Paras.csFilename = "CB_";
	m_ScanningImage_Save_Paras.lcConversion.DemosaicMethod = LUCAM_DM_HIGH_QUALITY;
	m_ScanningImage_Save_Paras.lcConversion.CorrectionMatrix = LUCAM_CM_HALOGEN;
	Last_Focus_X = {};
	Last_Focus_Z = {};
	Last_Focus_Y = {};
	Focus_Img = {};
	m_Focusing_Num_X = 2;
	m_Focusing_Num_Y = 2;
}

Scanning_Control::~Scanning_Control()
{
}

bool Scanning_Control::Do_Search_Optimal_Focal_Length( bool mean_lalace, int Search_Model)
{
	clock_t StartTime = GetTickCount();////计时开始

	//CString csTemp;
	//GetDlgItem(IDC_Distance)->GetWindowText(csTemp);
	//CT2A strX(csTemp, CP_UTF8);
	//long Z_Distance = atol(strX);

	//long Z_Distance = Focusing_Next_Step;//给定运动步长

	clock_t EndTime = GetTickCount();//计时结束
	double spendtime = (double)(EndTime - StartTime);//计算时间
	StartTime = GetTickCount();//计时开始

	long Z_Pos, Z_Pos1;
	if (m_Z_Move_Arrived)
	{
		if(!s_Dev_Drivers->Dev_Read_Pos(3, Z_Pos1))//读取Z轴当前位置
			return false;
		m_Z_Moveto = Z_Pos1 + Focusing_Next_Step;
		M_Z_Pos = Z_Pos1;
		//m_Z_Moveto = m_Z_Moveto + Focusing_Next_Step;
		m_Z_Move_Arrived = false;
		s_Dev_Drivers->Dev_Single_Mov(3, m_Z_Moveto, 10, true);
	}
	if (m_Respond_Check_Num > 100)
	{
		//s_Dev_Drivers->Dev_ENA(3, 0);
		s_Dev_Drivers->Dev_Single_Mov(3, m_Z_Moveto, 10, true);
		m_Respond_Check_Num = 0;
		return false;
	}
	vector<bool> status_arr = { 0 };
	s_Dev_Drivers->Dev_Read_Status({ 4 }, status_arr, 1);
	if (!status_arr[0])
	{
		m_Respond_Check_Num++;
		return false;
	}
	bool Move_Arrived = false;
	bool Read_Right = s_Dev_Drivers->Dev_Read_Pos(3, Z_Pos);

	bool Move_In_Deviation = (Z_Pos >(m_Z_Moveto - s_Scanning_System_Para.Move_Deviation)) &&
		(Z_Pos <= (m_Z_Moveto + s_Scanning_System_Para.Move_Deviation));
	//long hh = M_Z_Pos;
	//bool Move_Arrived = (Z_Pos >(m_Z_Moveto - 5)) &&
		//(Z_Pos < (m_Z_Moveto + 5));

	if (Read_Right)//读取Z轴当前位置
		Move_Arrived = true;
	else
	{
		m_Respond_Check_Num++;
		Sleep(5);
		return false;
	}
		
	//int k = 0, j = 0;
	//while (k<4)
	//{
	//	long Z_Pos2;
	//	if (!s_Dev_Drivers->Dev_Read_Pos(3, Z_Pos2))
	//		continue;
	//	if (Z_Pos2 == Z_Pos)
	//		j++;
	//	if (j>3)
	//		Move_In_Deviation = true;
	//	k++;
	//}

	if (!Move_In_Deviation)
		return false;

	Focusing_Search_Num++;
	m_Z_Move_Arrived = true;

	EndTime = GetTickCount();//计时结束
	spendtime = (double)(EndTime - StartTime);//计算时间
	StartTime = GetTickCount();//计时开始	
	/********************************/
	//Sleep(100);
	/*****************************/
	Mat Focus_Pic;
	Take_Video_Image(Focus_Pic);

	EndTime = GetTickCount();//计时结束
	spendtime = (double)(EndTime - StartTime);//计算时间
	StartTime = GetTickCount();//计时开始	

	double Fangcha = 0;
	if (mean_lalace)
		Fangcha = Calculate_Sharpness(0, Z_Pos, Focus_Pic);//计算评价函数值,(第1个参数为0表示采用方差评价)且已经保存了当前焦距值
	else
		//Fangcha = Calculate_Sharpness(1, Z_Pos, Focus_Pic);//计算评价函数值,(第1个参数为1表示采用拉普拉斯评价)且已经保存了当前焦距值
		Fangcha = Calculate_Sharpness(1, Z_Pos, Focus_Pic);
	EndTime = GetTickCount();//计时结束
	spendtime = (double)(EndTime - StartTime);//计算时间

	switch (Search_Model)
	{
	case Big_Step:
		if (m_MaxFocLen > Fangcha)
		{
			////if (m_MaxFocLen_Z_Pos<m_Z_Moveto && m_Z_Moveto< Z_Big_Step_Upper_Limit)//----
			////	Z_Big_Step_Upper_Limit = m_Z_Moveto;//----
			////if (m_MaxFocLen_Z_Pos > m_Z_Moveto && m_Z_Moveto > Z_Big_Step_Low_Limit)//----
			////	Z_Big_Step_Low_Limit = m_Z_Moveto;//---
			if (Focusing_Search_Num > 2)
			{
				Focusing_Previous_Step = s_Scanning_System_Para.Focusing_Step_Z[0];
				Focusing_Next_Step = s_Scanning_System_Para.Focusing_Step_Z[0];
				//Focusing_Search_Num = 1;
				Focusing_Search_Num = 0;

				Z_Big_Step_MaxFocus_Pos = m_MaxFocLen_Z_Pos;//---

				return true;
			}
			else
			{
				Focusing_Previous_Step = Focusing_Next_Step;
				Focusing_Next_Step = -2 * s_Scanning_System_Para.Focusing_Step_Z[0];
			}
		}
		else
		{
			m_MaxFocLen_Z_Pos = Z_Pos;
			m_MaxFocLen = Fangcha;
			long Z_Sub = m_Z_Moveto - Focusing_Next_Step;

			////if (m_MaxFocLen_Z_Pos<Z_Sub && Z_Sub< Z_Big_Step_Upper_Limit)
			////	Z_Big_Step_Upper_Limit = m_Z_Moveto - Focusing_Next_Step;
			////if (m_MaxFocLen_Z_Pos>Z_Sub && Z_Sub> Z_Big_Step_Low_Limit)
			////	Z_Big_Step_Low_Limit = m_Z_Moveto - Focusing_Next_Step;

			m_Z_Moveto = Z_Pos + Focusing_Next_Step;
			Focusing_Previous_Step = Focusing_Next_Step;
			Focusing_Next_Step = (Focusing_Next_Step < 0) ? (-1 * s_Scanning_System_Para.Focusing_Step_Z[0]) : Focusing_Next_Step;

		}

		return false;

	case Middle_Step:
		if (m_MaxFocLen >  Fangcha)
		{
			if (Focusing_Search_Num > 2)
			{
				Focusing_Previous_Step = s_Scanning_System_Para.Focusing_Step_Z[1];
				Focusing_Next_Step = s_Scanning_System_Para.Focusing_Step_Z[1];
				Focusing_Search_Num = 0;
				m_MaxFocLen = 0;
				return true;
			}
			else
			{
				Focusing_Previous_Step = Focusing_Next_Step;
				Focusing_Next_Step = -(Focusing_Search_Num+1)* s_Scanning_System_Para.Focusing_Step_Z[1];

			}
		}
		else
		{
			Focusing_Previous_Step = Focusing_Next_Step;
			Focusing_Next_Step = (Focusing_Next_Step < 0) ? (-1 * s_Scanning_System_Para.Focusing_Step_Z[1]) : Focusing_Next_Step;
			m_MaxFocLen_Z_Pos = Z_Pos;
			m_MaxFocLen = Fangcha;

		}

		return false;
	case Little_Step:

		if (m_MaxFocLen >  Fangcha)
		{
			if (Focusing_Search_Num > 2)
			{
				Focusing_Previous_Step = s_Scanning_System_Para.Focusing_Step_Z[2];
				Focusing_Next_Step = s_Scanning_System_Para.Focusing_Step_Z[2];
				Focusing_Search_Num = 0;
				return true;
			}
			else
			{
				Focusing_Previous_Step = Focusing_Next_Step;
				Focusing_Next_Step = -2 * s_Scanning_System_Para.Focusing_Step_Z[2];

			}
		}
		else
		{
			Focusing_Previous_Step = Focusing_Next_Step;
			Focusing_Next_Step = (Focusing_Next_Step < 0) ? (-1 * s_Scanning_System_Para.Focusing_Step_Z[2]) : Focusing_Next_Step;
			m_MaxFocLen_Z_Pos = Z_Pos;
			m_MaxFocLen = Fangcha;

		}

		return false;
	default:
		return false;
	}
}
//获取染色体团在100X下的区域
vector<vector<Point>> Scanning_Control::Get_CAGroup_Region_100X(Mat input_image, bool &Right_CAGroup)
{
	Mat Bi, Bi_gray;
	int Otsu_Threshold = threshold(input_image, Bi_gray, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);//CV_THRESH_BINARY_INV
	double BIN_Threshold = Otsu_Threshold*0.8;
	threshold(input_image, Bi_gray, BIN_Threshold, 255, CV_THRESH_BINARY_INV);
	Bi = Bi_gray.clone();
	/*首先进行膨胀腐蚀去除噪点*/
	Mat element5 = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));//核2 结构元素8
	erode(Bi, Bi, element5);
	dilate(Bi, Bi, element5);
	////////////根据面积，进行目标选择
	vector < vector<Point> > contours_100X;
	vector < vector<Point> >Right_contours_100X;
	vector < vector<Point> > contours_100X_out;
	int CA_Right_Little = 0;
	int CA_Right_Middle = 0;
	int CA_Right_Big = 0;
	findContours(Bi, contours_100X, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));//CV_RETR_CCOMP
	for (int i = 0; i < contours_100X.size(); i++)
	{
		unsigned int contour_Aero = contourArea(contours_100X[i]);
		if ((contour_Aero>20) && (contour_Aero<20))
			CA_Right_Little++;
		else if ((contour_Aero >= 20) && (contour_Aero < 600))
		{
			Right_contours_100X.push_back(contours_100X[i]);
			CA_Right_Middle++;
		}
		else// if(contour_Aero > 600)
			CA_Right_Big++;
	}
	Right_CAGroup = (CA_Right_Middle >20)
		&& (CA_Right_Little + CA_Right_Middle + CA_Right_Little < 200);//(CA_Right_Little < CA_Right_Middle / 3)
	if (!Right_CAGroup)//如果没有检测到染色体团区域，直接退出
		return contours_100X_out;

	Mat mask = Mat::zeros(Bi.size(), CV_8U);//同原图一样大小，纯黑的mask图像//CV_8UC3
	drawContours(mask, Right_contours_100X, -1, Scalar(255), -1);//在mask上用白色绘制轮廓，-1向内填充
	vector < vector<Point> > contours_100X_1;
	Mat element10 = getStructuringElement(MORPH_ELLIPSE, Size(50, 50));//核2 结构元素50,将目标区域联合起来
	dilate(mask, mask, element10);
	findContours(mask, contours_100X_1, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));//CV_RETR_CCOMP
	unsigned int contour_Aero_Max = 0;
	unsigned int contour_Aero_index = 0;
	for (int i = 0; i < contours_100X_1.size(); i++)//遍历每个轮廓
	{

		unsigned int contour_Aero_Temp = contourArea(contours_100X_1[i]);
		if (contour_Aero_Temp>contour_Aero_Max)
		{
			contour_Aero_Max = contour_Aero_Temp;
			contour_Aero_index = i;

		}
	}
	contours_100X_out.push_back(contours_100X_1[contour_Aero_index]);
	return contours_100X_out;
}
//聚焦函数-逐点区焦距，然后区最大值   求解图像的评价函数，并将图像和值保存
double Scanning_Control::Calculate_Sharpness_Every_Point(int Focus_Level_100X)
{
	Mat Focus_Pic, OutGray;
	Take_Video_Image(Current_Pic);

	//if (s_Scanning_Para.Objective_Num == Objective_100X)
	//	Sleep(1000);	
	//Take_Video_Image(Focus_Pic);
	double scale = 0.2;
	Size outsize;
	Mat Resize;
	outsize.height = Current_Pic.rows*scale;
	outsize.width = Current_Pic.cols*scale;
	resize(Current_Pic, Resize, outsize, 0, 0, INTER_LINEAR);
	Mat gray;
	//cvtColor(img, gray, COLOR_RGB2GRAY);
	cvtColor(Resize, OutGray, COLOR_RGB2GRAY);//采用缩小版的图像进行处理
	//namedWindow("灰度图", CV_WINDOW_NORMAL);
	//imshow("灰度图", gray);

	//cvtColor(Focus_Pic, OutGray, CV_BGR2GRAY);
	double Focus_Value;
	double Focus_Value0;
	switch (s_Scanning_Para.Objective_Num)
	{
	case Objective_10X:
	{
		//用方差求解
		//Mat meanValueImage;
		//Mat meanStdValueImage;
		//meanStdDev(OutGray, meanValueImage, meanStdValueImage);
		//Focus_Value = meanStdValueImage.at<double>(0, 0);
		//double Focus_Mean;
		Mat meanValueImage;
		Mat meanStdValueImage;
		Mat LapValueImage;
		switch (MeanStd_Operator)
		{
		case Laplacian_Operator:
			Laplacian(OutGray, LapValueImage, CV_16U);
			Focus_Value = mean(LapValueImage)[0];
			break;
		case MeanStd_Operator:
			meanStdDev(OutGray, meanValueImage, meanStdValueImage);
			Focus_Value = meanStdValueImage.at<double>(0, 0);
			Focus_Value0 = 0;
			break;
		case Thenengrad_Operator:
			Focus_Value = Thenengrad(OutGray);
			break;
		default:
			break;
		}
		
	}
		break;
	case Objective_100X:
	{
		if (Focusing_Search_Num == 0)
		{
			//进入本分支时，自动将之前输出的目标轮廓清零
			contours_Right.clear();//清空元素但不回收空间，size为0，但capacity不为0；
			vector < vector<Point> > contours_100X;
			bool Continue_CA = false;
			contours_100X = Get_CAGroup_Region_100X(OutGray, Continue_CA);
			if (!Continue_CA)
			{
				//namedWindow("灰度图0", CV_WINDOW_NORMAL);
				//imshow("灰度图0", OutGray);
				//cvWaitKey(0);
				Obtain_Optimal_Focal = true;
				Pass_TakeImg = true;
				return -1;
			}

			//Mat Bi, Bi_gray;
			//int Otsu_Threshold = threshold(OutGray, Bi_gray, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);//CV_THRESH_BINARY_INV
			//double BIN_Threshold = Otsu_Threshold*0.8;
			//threshold(OutGray, Bi_gray, BIN_Threshold, 255, CV_THRESH_BINARY_INV);
			//Bi = Bi_gray.clone();
			///*首先进行膨胀腐蚀去除噪点*/
			//Mat element5 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));//核2 结构元素8
			//erode(Bi, Bi, element5);
			//dilate(Bi, Bi, element5);
			//////////////根据面积，进行目标选择
			//vector < vector<Point> > contours_100X;
			//vector < vector<Point> >Right_contours_100X;
			//int CA_Right_Little = 0;
			//int CA_Right_Middle = 0;
			//int CA_Right_Big = 0;
			//findContours(Bi, contours_100X, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));//CV_RETR_CCOMP
			//for (int i = 0; i < contours_100X.size(); i++)
			//{
			//	unsigned int contour_Aero = contourArea(contours_100X[i]);
			//	if ((contour_Aero>20) && (contour_Aero<20))
			//		CA_Right_Little++;
			//	else if ((contour_Aero >= 20) && (contour_Aero < 500))
			//	{
			//		Right_contours_100X.push_back(contours_100X[i]);
			//		CA_Right_Middle++;
			//	}
			//	else// if(contour_Aero > 600)
			//		CA_Right_Big++;
			//}
			//bool Continue_CA = (CA_Right_Middle >20)
			//	&& (CA_Right_Little + CA_Right_Middle + CA_Right_Little < 100);//(CA_Right_Little < CA_Right_Middle / 3)
			//if (!Continue_CA)
			//{
			////	namedWindow("灰度图", CV_WINDOW_NORMAL);
			////	imshow("灰度图", OutGray);
			////	cvWaitKey(0);
			//	Obtain_Optimal_Focal = true;
			//	Pass_TakeImg = true;
			//	return -1;
			//}
			///*直接用注定啊聚集的方式聚焦，本段代码注释后，就是采用爬山法聚焦*/
			////else
			////{
			////	Do_Find_Focus_Level_100X(300, Laplacian_Operator, false, true);
			////	Obtain_Optimal_Focal = true;
			////	return -1;
			////}
			///*r如果满足染色体团的条件，则进行重新中心位置调整*/
			//Mat mask = Mat::zeros(Bi.size(), CV_8U);//同原图一样大小，纯黑的mask图像//CV_8UC3
			//drawContours(mask, Right_contours_100X, -1, Scalar(255), -1);//在mask上用白色绘制轮廓，-1向内填充
			//vector < vector<Point> > contours_100X_1;
			//Mat element10 = getStructuringElement(MORPH_ELLIPSE, Size(50, 50));//核2 结构元素50,将目标区域联合起来
			//dilate(mask, mask, element10);
			//findContours(mask, contours_100X_1, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));//CV_RETR_CCOMP
			//unsigned int contour_Aero_Max = 0;
			//unsigned int contour_Aero_index = 0;
			//for (int i = 0; i < contours_100X_1.size(); i++)//遍历每个轮廓
			//{
			//	unsigned int contour_Aero_Temp = contourArea(contours_100X_1[i]);
			//	if (contour_Aero_Temp>contour_Aero_Max)
			//	{
			//		contour_Aero_Max = contour_Aero_Temp;
			//		contour_Aero_index = i;
			//	}
			//}
			/*r如果满足染色体团的条件，则进行重新中心位置调整*/
			//绘制轮廓的最小外结圆
			Point2f center; float radius;
			minEnclosingCircle(contours_100X[0], center, radius);
			//circle(gray, center, radius, Scalar(255), 2);
			/*计算染色体团当前位置距离理想中心位置的距离*/
			int CA_RightPos_X = (OutGray.cols / 2 - center.x)*s_Scanning_System_Para.View_Field_Width_10X / (OutGray.cols * 10)
				+ m_X_Moveto;
			int CA_RightPos_Y = -(OutGray.rows / 2 - center.y)*s_Scanning_System_Para.View_Field_Higth_10X / (OutGray.rows * 10)
				+ m_Y_Moveto;
			//将染色体团移动到视野中心位置
			m_X_Moveto = CA_RightPos_X;
			s_Dev_Drivers->Dev_Single_Mov(1, m_X_Moveto, 5, true);
			m_Y_Moveto = CA_RightPos_Y;
			s_Dev_Drivers->Dev_Single_Mov(2, m_Y_Moveto, 5, true);
			bool Back_Right0 = false;
			bool Back_Right1 = false;
			while (!Back_Right0 || !Back_Right1)
			{
				Back_Right0 = (Do_Stepping_Finished(1, m_X_Moveto) == State_Finished);
				Back_Right1 = (Do_Stepping_Finished(2, m_Y_Moveto) == State_Finished);
				if (Back_Right0 == Stepping)
					s_Dev_Drivers->Dev_Single_Mov(1, m_X_Moveto, 5, true);
				if (Back_Right1 == Stepping)
					s_Dev_Drivers->Dev_Single_Mov(2, m_Y_Moveto, 5, true);
			}
			//Sleep(100);
			///*重新抓取图像，并染色体团的位置提取*/
			Take_Video_Image(Current_Pic);
			outsize.height = Current_Pic.rows*scale;
			outsize.width = Current_Pic.cols*scale;
			resize(Current_Pic, Resize, outsize, 0, 0, INTER_LINEAR);
			cvtColor(Resize, OutGray, COLOR_RGB2GRAY);
			//namedWindow("灰度图", CV_WINDOW_NORMAL);
			//imshow("灰度图", OutGray);
			//cvWaitKey(0);

			contours_Right = Get_CAGroup_Region_100X(OutGray, Continue_CA);
			if (!Continue_CA)
			{
				//namedWindow("灰度图0", CV_WINDOW_NORMAL);
				//imshow("灰度图0", OutGray);
				//cvWaitKey(0);
				Obtain_Optimal_Focal = true;
				Pass_TakeImg = true;
				return -1;
			}

		}
		//当获得的图像不清晰时，将该图剔除
		if (contours_Right.size() == 0)
		{
			Mat meanValueImage;
			Mat meanStdValueImage;
			Mat LapValueImage;
			switch (MeanStd_Operator)
			{
			case Laplacian_Operator:
				Laplacian(OutGray, LapValueImage, CV_16U);
				Focus_Value = mean(LapValueImage)[0];
				break;
			case MeanStd_Operator:
				meanStdDev(OutGray, meanValueImage, meanStdValueImage);
				Focus_Value = meanStdValueImage.at<double>(0, 0);
				break;
			case Thenengrad_Operator:
				Focus_Value = Thenengrad(OutGray);
				break;
			default:
				break;
			}
		}
		else
		{
			///*以下为基于上面获取的contours_Right的ROI区域进行计算清晰度值*/
			Focus_Value = Cut_ROI_Compute_Focus(OutGray, contours_Right, MeanStd_Operator, true); 
			Focus_Value0 = Cut_ROI_Compute_Focus(OutGray, contours_Right, Laplacian_Operator, true);
		}
		//Focus_Value = Cut_ROI_Compute_Focus(OutGray, contours_Right, Laplacian_Operator, true);

		if (s_Scanning_Para.Objective_Num == Objective_100X)
		{
		/*将定位的图存放在本地分析*/
			string Focus_Value_L = to_string(Focus_Value);
			string Focus_Value_M = to_string(Focus_Value0);
			string Z_POS = to_string(m_Z_Moveto);
		string p2 = to_string(s_Scanning_Para.Current_Slide_Num);
		string p = to_string(Focusing_Search_Num);
		string p1 = to_string(s_Scanning_Para.Current_Point_Num);
		string p_road = "D:\CA_100X_Focus\\F" + p2 + "_" + p1 + "_" + p + Z_POS + "_" + Focus_Value_L + "_" + Focus_Value_M + ".bmp";
		cv::imwrite(p_road, Current_Pic);
		}


		break;
	}
		
	case Objective_40X:
	default:

		//m_Z_Moveto = Focus_Level_100X;
		Do_Find_Focus_Level_100X_Climb(800, MeanStd_Operator, true);
		//Do_Find_Focus_Level_100X(1000, MeanStd_Operator,true,true);
		Do_Find_Focus_Level_100X_Climb(200, Laplacian_Operator, true);
		//Do_Find_Focus_Level_100X(200, Laplacian_Operator, false, true);
		Obtain_Optimal_Focal = true;
		return 1;
		//cvtColor(Focus_Pic, OutGray, CV_BGR2GRAY);//采用的是没有缩小版的图像
		if (Focusing_Search_Num<1)
		{
			Mat imageShold;
			//int Otsu_Threshold = threshold(OutGray, imageShold, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
			//threshold(OutGray, imageShold, 146, 255, 1);   // 必须进行二值化  			
			int Otsu_Threshold = threshold(OutGray, imageShold, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);//CV_THRESH_BINARY_INV
			double BIN_Threshold = Otsu_Threshold*0.8;
			threshold(OutGray, imageShold, BIN_Threshold, 255, CV_THRESH_BINARY_INV);
			//Bi = Bi_gray.clone();
			//40X进行扫描的时候，膨胀腐蚀的结构元大小为 50，
			//10X进行扫描的时候，膨胀腐蚀的结构元大小为 10
			unsigned int Erode_Element_Size = 15;
			Mat element0 = getStructuringElement(MORPH_RECT, Size(Erode_Element_Size, Erode_Element_Size));
			erode(imageShold, imageShold, element0);

			unsigned int Dilate_Element_Size = 18;
			Mat element = getStructuringElement(MORPH_RECT, Size(Dilate_Element_Size, Dilate_Element_Size));
			dilate(imageShold, imageShold, element);

			//namedWindow("膨胀操作", WINDOW_NORMAL);
			//imshow("膨胀操作", imageShold);
			//cv::waitKey(0);

			vector<vector<Point>> contours;
			unsigned int Max_Aero = 0;
			findContours(imageShold, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
			contours_Right = getSizeContours(contours, Max_Aero);
			//if ((contours_Right.size() < 1) || (Max_Aero < 2000))
			//{
			//	//m_Z_Moveto = Last_MaxFocus_Pos;
			//	//Obtain_Optimal_Focal = true;
			//	//return -1;
			//	contours_Right.clear();
			//}
		}

		if (contours_Right.size() == 0)
		{
			Mat meanValueImage;
			Mat meanStdValueImage;
			Mat LapValueImage;
			switch (MeanStd_Operator)
			{
			case Laplacian_Operator:
				Laplacian(OutGray, LapValueImage, CV_16U);
				Focus_Value = mean(LapValueImage)[0];
				break;
			case MeanStd_Operator:
				meanStdDev(OutGray, meanValueImage, meanStdValueImage);
				Focus_Value = meanStdValueImage.at<double>(0, 0);
				break;
			case Thenengrad_Operator:
				Focus_Value = Thenengrad(OutGray);
				break;
			default:
				break;
			}
		}
		else
		{
			Focus_Value = Cut_ROI_Compute_Focus(OutGray, contours_Right, Laplacian_Operator, true);
		}
		break;
	}
	/*以下为保存每个聚焦图像*/
	//string zpos = to_string(m_Z_Moveto);
	//string p_Num = to_string(s_Scanning_Para.Current_Point_Num);
	//string p = to_string(Focusing_Search_Num);
	//string mean_Value = to_string(Focus_Value);
	////string mean_Value0 = to_string(Focus_Value0);
	////vector<int> compression_params;
	////compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	////compression_params.push_back(9);    //png格式下，默认的参数为3.
	//string p_road = "H:\\focusing\\" + p_Num + "-" + p + " " + zpos + "+" + mean_Value
	//	 + ".bmp";
	//cv::imwrite(p_road, Focus_Pic);

	int Focus_Value_Size = Focus_Values.size();
	if (Focus_Value_Size < 5)
	{
		Focus_Values.push_back(Focus_Value);
		Focus_Values_Later.push_back(Focus_Value0);//100X下采用
		Focus_Poss.push_back(m_Z_Moveto);
	}
	else
	{
		if (Get_Dic_N)//前两个为最大值
		{
			Focus_Values[0] = Focus_Value;
			Focus_Values_Later[0] = Focus_Value0;
			Focus_Poss[0] = m_Z_Moveto;
		}
		if (Get_Dic_F)//后两个为最大值
		{
			Focus_Values[4] = Focus_Value;
			Focus_Values_Later[4] = Focus_Value0;
			Focus_Poss[4] = m_Z_Moveto;
		}
	}

	if (Max_Focus_Value <= Focus_Value)
	{
		Max_Focus_Value = Focus_Value;
		Best_Img = Current_Pic;
	}

	if (Max_Focus_Value_Later <= Focus_Value0)
	{
		Max_Focus_Value_Later = Focus_Value0;
		Best_Img_Later = Current_Pic;
	}

	Focusing_Search_Num++;
	//m_height_best = s_Scanning_Para.High[m_point_i];
	//////string zpos = to_string(m_Z_Moveto);
	//////string p_Num = to_string(s_Scanning_Para.Current_Point_Num);
	//////string p = to_string(Focusing_Search_Num);
	//////string mean_Value = to_string(Focus_Value);
	//////critical_section.Lock();
	//////string p_road = "C:\\Users\\wench\\Desktop\\focus\\" + p_Num + "-" + p + " " + zpos + "+" + mean_Value + ".bmp";
	//////cv::imwrite(p_road, Focus_Pic);
	//////critical_section.Unlock();
	bool Sub3_2 = false;
	bool Sub2_1 = false;
	bool Points_Satisfied = false;
	Focus_Value_Size = Focus_Values.size();
	switch (s_Scanning_Para.Objective_Num)
	{
	case Objective_10X:
		//if (Focus_Value_Size > 3)
		//{
		//	Sub3_2 = (Focus_Values[Focus_Value_Size - 1]>Focus_Values[Focus_Value_Size - 2]) &&
		//		(Focus_Values[Focus_Value_Size - 1] > Focus_Values[Focus_Value_Size - 3]) &&
		//		(Focus_Values[Focus_Value_Size - 1] > Focus_Values[Focus_Value_Size - 4]);
		//	Sub2_1 = (Focus_Values[Focus_Value_Size - 1] < Focus_Values[Focus_Value_Size - 4]) &&
		//		(Focus_Values[Focus_Value_Size - 2] < Focus_Values[Focus_Value_Size - 4]) &&
		//		(Focus_Values[Focus_Value_Size - 3] < Focus_Values[Focus_Value_Size - 4]);
		//	Points_Satisfied = true;
		//}
		//break;
	case Objective_100X:
		if (Focus_Value_Size > 4)
		{
			double Focus_Values_Max = 0;
			for (int i = 0; i<Focus_Value_Size; i++)
			{
				double Temp_Value = Focus_Values[i];
				if (Focus_Values_Max < Temp_Value)
				{
					Focus_Values_Max = Temp_Value;
					Focus_Values_Max_Index = i;
				}
			}
			m_MaxFocLen = Focus_Values[Focus_Values_Max_Index];
			m_MaxFocLen_Z_Pos = Focus_Poss[Focus_Values_Max_Index];
			if (Focus_Values_Max_Index < 2)
			{
				if (!Get_Dic_N)
					Focusing_Next_Step = -Focusing_Next_Step;
				Get_Dic_N = true;
				Get_Dic_F = false;
				Focus_Poss[4] = Focus_Poss[3];
				Focus_Poss[3] = Focus_Poss[2];
				Focus_Poss[2] = Focus_Poss[1];
				Focus_Poss[1] = Focus_Poss[0];
				Focus_Poss[0] = 0;
				Focus_Values[4] = Focus_Values[3];
				Focus_Values[3] = Focus_Values[2];
				Focus_Values[2] = Focus_Values[1];
				Focus_Values[1] = Focus_Values[0];
				Focus_Values[0] = 0;
				Focus_Values_Later[4] = Focus_Values_Later[3];//100X下采用
				Focus_Values_Later[3] = Focus_Values_Later[2];
				Focus_Values_Later[2] = Focus_Values_Later[1];
				Focus_Values_Later[1] = Focus_Values_Later[0];
				Focus_Values_Later[0] = 0;
				m_Z_Moveto = m_MaxFocLen_Z_Pos - Focusing_Next_Step * (1 + Focus_Values_Max_Index);
			}
			else if (Focus_Values_Max_Index > 2)
			{
				Get_Dic_N = false;
				Get_Dic_F = true;
				Focus_Poss[0] = Focus_Poss[1];
				Focus_Poss[1] = Focus_Poss[2];
				Focus_Poss[2] = Focus_Poss[3];
				Focus_Poss[3] = Focus_Poss[4];
				Focus_Poss[4] = 0;
				Focus_Values[0] = Focus_Values[1];
				Focus_Values[1] = Focus_Values[2];
				Focus_Values[2] = Focus_Values[3];
				Focus_Values[3] = Focus_Values[4];
				Focus_Values[4] = 0;
				Focus_Values_Later[0] = Focus_Values_Later[1];//100X下采用
				Focus_Values_Later[1] = Focus_Values_Later[2];
				Focus_Values_Later[2] = Focus_Values_Later[3];
				Focus_Values_Later[3] = Focus_Values_Later[4];
				Focus_Values_Later[4] = 0;
				m_Z_Moveto = m_MaxFocLen_Z_Pos - Focusing_Next_Step * (5 - Focus_Values_Max_Index);
			}
			else// if (Focus_Values_Max_Index == 2)
			{
				Get_Dic_N = true;
				Get_Dic_F = true;
				m_Z_Moveto = m_MaxFocLen_Z_Pos;
			}

			Points_Satisfied = true;
		}
		else
		{
			//Get_Dic_N = false;
			//Get_Dic_F = false;
			int jjjjj = m_Z_Moveto;
			m_Z_Moveto = m_Z_Moveto - Focusing_Next_Step;
			int hhh = m_Z_Moveto;
		}
			
		break;
	case Objective_40X:
		if (Focus_Value_Size > 6)
		{
			Sub3_2 = (Focus_Values[Focus_Value_Size - 1]>Focus_Values[Focus_Value_Size - 2]) &&
				(Focus_Values[Focus_Value_Size - 1] > Focus_Values[Focus_Value_Size - 3]) &&
				(Focus_Values[Focus_Value_Size - 1] > Focus_Values[Focus_Value_Size - 4]) &&
				(Focus_Values[Focus_Value_Size - 1] > Focus_Values[Focus_Value_Size - 5]) &&
				(Focus_Values[Focus_Value_Size - 1] > Focus_Values[Focus_Value_Size - 6]);
			Sub2_1 = (Focus_Values[Focus_Value_Size - 1] < Focus_Values[Focus_Value_Size - 6]) &&
				(Focus_Values[Focus_Value_Size - 2] < Focus_Values[Focus_Value_Size - 6]) &&
				(Focus_Values[Focus_Value_Size - 3] < Focus_Values[Focus_Value_Size - 6]) &&
				(Focus_Values[Focus_Value_Size - 4] < Focus_Values[Focus_Value_Size - 6]) &&
				(Focus_Values[Focus_Value_Size - 5] < Focus_Values[Focus_Value_Size - 6]);
			Points_Satisfied = true;
		}
		break;
	default:
		if (Focus_Value_Size > 3)
		{
			Sub3_2 = (Focus_Values[Focus_Value_Size - 1]>Focus_Values[Focus_Value_Size - 2]) &&
				(Focus_Values[Focus_Value_Size - 1] > Focus_Values[Focus_Value_Size - 3]) &&
				(Focus_Values[Focus_Value_Size - 1] > Focus_Values[Focus_Value_Size - 4]);
			Sub2_1 = (Focus_Values[Focus_Value_Size - 1] < Focus_Values[Focus_Value_Size - 4]) &&
				(Focus_Values[Focus_Value_Size - 2] < Focus_Values[Focus_Value_Size - 4]) &&
				(Focus_Values[Focus_Value_Size - 3] < Focus_Values[Focus_Value_Size - 4]);
			Points_Satisfied = true;
		}
		break;
	}

	if (Points_Satisfied)
	{
			//Last_MaxFocus_Pos = m_MaxFocLen_Z_Pos;

			if (Get_Dic_F && Get_Dic_N)
			{
				//m_Z_Moveto = m_MaxFocLen_Z_Pos;
				//以下根据最大清晰度值与前后相邻清晰度值进行比较分析，得出最油焦距值
				if (s_Scanning_Para.Objective_Num == Objective_40X)//Laplacian_Operator
				{	//注意以下推断是聚焦过程中，Z轴从下往上运动的结果
					float Up_Error = Focus_Values[2] - Focus_Values[3];//注意务必保证最佳焦距处于中间位置
					float Down_Error = Focus_Values[2] - Focus_Values[1];
					if (Down_Error * 2 < Up_Error)
						m_Z_Moveto = Focus_Poss[1];
					else if (Down_Error <= Up_Error)
						m_Z_Moveto = (Focus_Poss[1] + Focus_Poss[3]) / 2;
					else
						m_Z_Moveto = Focus_Poss[2];
				}
				else
					m_Z_Moveto = Focus_Poss[2];

				vector <int>().swap(Focus_Poss);
				vector <double>().swap(Focus_Values);
				vector <double>().swap(Focus_Values_Later);
				Obtain_Optimal_Focal = true;
				Get_Dic_F = false;
				Get_Dic_N = false;
				//m_Z_Moveto = m_MaxFocLen_Z_Pos;
				Focusing_Search_Num = 0;
				vector <vector <Point>> temp_contours;
				contours_Right = temp_contours;

				return m_MaxFocLen;
			}

	}
	int Z_Max_Limit_Down = s_Scanning_System_Para.Focus_Max_Limit + 60000;
	//if ((m_Z_Moveto >Z_Max_Limit_Down) || (m_Z_Moveto <s_Scanning_System_Para.Focus_Min_Limit))//***********************
		//此区间很难选择，这里设置很大值，上限还有传感器，下限设置为较大的值
	if (m_Z_Moveto >Z_Max_Limit_Down)//***********************
		//此区间很难选择，这里设置很大值，上限还有传感器，下限设置为较大的值
	{
		//if (s_Scanning_Para.Objective_Num == Objective_10X)
		vector <int>().swap(Focus_Poss);
		vector <double>().swap(Focus_Values);
		vector <double>().swap(Focus_Values_Later);
		m_Z_Moveto = Focus_Level_100X;
		//else
		//m_Z_Moveto = Last_MaxFocus_Pos;
		Get_Dic_F = false;
		Get_Dic_N = false;
		Obtain_Optimal_Focal = true;
		Focusing_Search_Num = 0;
		vector <vector <Point>> temp_contours;
		contours_Right = temp_contours;
		return -1;
	}


	return Focus_Value;//返回最佳焦距的值

}

	//聚焦函数   求解图像的评价函数，并将图像和值保存
double Scanning_Control::Calculate_Sharpness(int Score_Function, int height, Mat picture)
{
		//Mat imageSource = picture(cv::Rect(512,512,1024,1024)); //读取图片
		//string p = to_string(m_point_i);
		//imwrite("C:\\Users\\wench\\Desktop\\focus\\" + p + ".bmp", imageSource);

		Mat imageGrey;
		cvtColor(picture, imageGrey, CV_RGB2GRAY);

		double meanValue = 0.0;
		//bool m_Score_Aero_empty=
		if ((Score_Function == 1) && !m_Focusing_Equal_Scaning)
		{
			Mat imageSobel;
			if (m_Searching_Model == Little_Step )
			{
				if ((Focusing_Search_Num == 0))
				{
				Mat imageShold;
				//threshold(imageGrey, imageShold, 200, 255, 1);   // 必须进行二值化  
				unsigned int Otsu_Threshold = threshold(imageGrey, imageShold, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
				//namedWindow("imageShold", WINDOW_NORMAL);
				//imshow("imageShold", imageSobel);

				//首先求解整个灰度图的标准差和平均灰度值
				//Mat meanValueGray;
				//Mat meanStdValueGray;
				//double Gray_mean, meanStdDevGrayValue;
				//meanStdDev(imageGrey, meanValueGray, meanStdValueGray);
				//meanStdDevGrayValue = meanStdValueGray.at<double>(0, 0);
				//Gray_mean = meanValueGray.at<double>(0, 0);
				//unsigned int Sub_01 = (double)Otsu_Threshold / 255 * 100;
				//unsigned int Sub_02 = ((double)abs(Gray_mean - Otsu_Threshold) / 127) * 100;
				//unsigned int Sub_Mean_Threshold = (Sub_01 + Sub_02) / 2;


				Mat element_dilate_out0 = getStructuringElement(MORPH_RECT, Size(5, 5));
				dilate(imageShold, imageShold, element_dilate_out0);


				//unsigned int Sub_Mean_Threshold = abs(Gray_mean - Otsu_Threshold);
				//unsigned int Erode_Element_Size = abs(100 - (int)Sub_Mean_Threshold);
				unsigned int Erode_Element_Size = 20;
				Mat erode_out, dilate_out;
				//Mat element_erode_out = getStructuringElement(MORPH_RECT, Size(Erode_Element_Size, Erode_Element_Size));
				Mat element_erode_out = getStructuringElement(MORPH_RECT, Size(Erode_Element_Size, Erode_Element_Size));
				erode(imageShold, erode_out, element_erode_out);
				//namedWindow("腐蚀操作", WINDOW_NORMAL);
				//imshow("腐蚀操作", erode_out);
				//erode_out = imageShold;

				//unsigned int Dilate_Element_Size = Sub_Mean_Threshold;
				unsigned int Dilate_Element_Size = 20;
				Mat element_dilate_out = getStructuringElement(MORPH_RECT, Size(Dilate_Element_Size, Dilate_Element_Size));
				dilate(erode_out, dilate_out, element_dilate_out);

				//dilate_out = erode_out;
				//找到阀值后的轮廓				
				vector<vector<Point>> contours,contours_Right;

				findContours(dilate_out, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

				vector<vector<Point>>::const_iterator itc = contours.begin();
				unsigned int Max_Aero = 0;
				contours_Right = getSizeContours(contours, Max_Aero);
				bool little_Size = (Max_Aero < 8000) && (contours.size() < 5);
				if ((Max_Aero < 6000) || little_Size)//当区域的最大面积小于10000，或则区域
				{//m_Sobel_Wrong = true;
					m_Focusing_Equal_Scaning = true;
					//Obtain_Optimal_Focal = true;
				}

				m_Score_Aero = Mat(imageGrey.size(), CV_8U, Scalar(255));
				drawContours(m_Score_Aero, contours_Right, -1, Scalar(0), -1);   // -1 表示所有轮廓 


	
				//Mat element_dilate_out = getStructuringElement(MORPH_RECT, Size(100, 100));
				//dilate(m_Score_Aero, m_Score_Aero, element_dilate_out);


				//求拉普拉斯频域清晰度评价
				Mat Image_score = Mat::zeros(imageGrey.size(), CV_8U);
				int nrow = imageGrey.rows;
				int ncol = imageGrey.cols;
				for (int i = 0; i < nrow; i++)
				{
					uchar* data0 = m_Score_Aero.ptr<uchar>(i);//get the address of row i;  
					uchar* data1 = imageGrey.ptr<uchar>(i);
					uchar* data2 = Image_score.ptr<uchar>(i);
					for (int j = 0; j < ncol; j++)
					{
						if (data0[j] == 0)
						{
							data2[j] = data1[j];
						}
					}

				}

				////Tenengrad 梯度函数
				meanValue = Thenengrad(Image_score);


				////用Laplacian求解
				//Laplacian(Image_score, imageSobel, CV_16U);
				//meanValue = mean(imageSobel)[0];

				//用方差求解
				//Mat meanValueImage;
				//Mat meanStdValueImage;
				//meanStdDev(Image_score, meanValueImage, meanStdValueImage);
				//meanValue = meanStdValueImage.at<double>(0, 0);

				if (meanValue <= 0)
				{//m_Sobel_Wrong = true;
					m_Focusing_Equal_Scaning = true;
					//Obtain_Optimal_Focal = true;
					//当拉普拉斯求不出来的时候用方差（用聚焦替代扫描）
					Mat meanValueImage;
					Mat meanStdValueImage;
					meanStdDev(imageGrey, meanValueImage, meanStdValueImage);
					meanValue = meanStdValueImage.at<double>(0, 0);
				}
				}
				else if (m_Focusing_Equal_Scaning)
				{
					//当拉普拉斯求不出来的时候用方差（用聚焦替代扫描）
					Mat meanValueImage;
					Mat meanStdValueImage;
					meanStdDev(imageGrey, meanValueImage, meanStdValueImage);
					meanValue = meanStdValueImage.at<double>(0, 0);
				}
				else
				{
					//求拉普拉斯频域清晰度评价
					Mat Image_score = Mat::zeros(imageGrey.size(), CV_8U);
					int nrow = imageGrey.rows;
					int ncol = imageGrey.cols;
					for (int i = 0; i < nrow; i++)
					{
						uchar* data0 = m_Score_Aero.ptr<uchar>(i);//get the address of row i;  
						uchar* data1 = imageGrey.ptr<uchar>(i);
						uchar* data2 = Image_score.ptr<uchar>(i);
						for (int j = 0; j < ncol; j++)
						{
							if (data0[j] == 0)
							{
								data2[j] = data1[j];
							}
						}

					}
					////Tenengrad 梯度函数
					meanValue = Thenengrad(Image_score);


					////用Laplacian求解
					//Laplacian(Image_score, imageSobel, CV_16U);
					//meanValue = mean(imageSobel)[0];
				}

			}
			else
			{
				Laplacian(imageGrey, imageSobel, CV_16U);
				meanValue = mean(imageSobel)[0];
			}


		}
		else
		{
			//以下方差求解
			Mat meanValueImage;
			Mat meanStdValueImage;
			meanStdDev(imageGrey, meanValueImage, meanStdValueImage);
			meanValue = meanStdValueImage.at<double>(0, 0);
			//double SquareError = meanValue*meanValue;
		}

		//double to string
		stringstream meanValueStream;
		string meanValueString;
		meanValueStream << meanValue*meanValue;
		meanValueStream >> meanValueString;
		meanValueString = "Articulation(Variance Method): " + meanValueString;

		//putText(imageSource, meanValueString, Point(20, 50), CV_FONT_HERSHEY_COMPLEX, 0.8, Scalar(255, 255, 25), 2);

		//cout<<"图片名称："<<fn6_1+"   "<<meanValueString<<"\n";
		//imshow(fn6_1, imageSource) ;  

		s_Scanning_Para.save[m_point_i] = meanValue;//保存清晰度值
		s_Scanning_Para.High[m_point_i] = height;

		//m_height_best = s_Scanning_Para.High[m_point_i];

		m_point_i++;
		//以下为保存每个聚焦图像

		string zpos = to_string(height);
		string p_Num = to_string(s_Scanning_Para.Current_Point_Num);
		string p = to_string(m_point_i);
		string mean_Value = to_string(meanValue);
		//vector<int> compression_params;
		//compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		//compression_params.push_back(9);    //png格式下，默认的参数为3.
		string p_road = "C:\\Users\\wench\\Desktop\\focus\\" + p_Num+"-"+p + " " + zpos + "+" + mean_Value + ".bmp";
		cv::imwrite(p_road, picture);


		return meanValue;//返回最佳焦距的值

	}

//找平函数
double Scanning_Control::Move_To_Focus(vector <int>a, vector <int>b, vector <int>c, int point_x, int point_y)
{
	double height_xy = 0;//传回点（X,Y）的最佳焦距值
	struct THREEDpoint
	{
		int x;
		int y;
		int z;
	};
	//如果输入的点为24个点中的任意一个，则直接返回高度值
	for (int k = 0; k < 24; k++)
	{

		if ((a[k] == point_x) && (b[k] == point_y))
		{
			height_xy = c[k];
			//cout << "p_1_xy[k].x：" << p_1_xy[k].x << "\n";
			//cout << "p_1_xy[k].y：" << p_1_xy[k].y << "\n";
			//cout << "p_1_xy[k].y：" << p_1_xy[k].z << "\n";
			////return height_xy; //直接完成
			//system("pause");
			return height_xy;
		}
	}

	double a_x, b_y;//a为玻片的宽，x方向；b为玻片的长，y方向；
	double bili_x, bili_y;//计算比例，用于确定输入点的位置以及相连的点
	double a1_x, a2_x, b1_y, b2_y;//用于存放相邻4个点的坐标

	a_x = abs(a[3] - a[0]);
	b_y = abs(b[0] - b[23]);


	bili_x = abs((point_x - a[0]) / (a_x));
	bili_y = abs((b[0] - point_y) / (b_y));

	//cout << "a_x：" << a_x << "\n";
	//cout << "b_y：" << b_y << "\n";
	//cout << "bili_x：" << bili_x << "\n";
	//cout << "bili_y：" << bili_y << "\n";

	THREEDpoint save_xy[4];//用于存放相邻的四个点的x,y坐标;
	THREEDpoint sangedian_xy[3];//用于存放构成平面的3个点的x,y坐标;

	double kk_xy, bb_xy, yy_xy;//y = kx +b,存放k，b的值,y的值
	double aa_xyz, bb_xyz, cc_xyz, dd_xyz, zz_xyz; //存放 0= ax+ by+cz+D 的a, b,c,y的值


	double a1 = (double)1 / 3, a2 = (double)2 / 3;
	//cout << "a1：" << a1 << "\n";
	//cout << "a2：" << a2 << "\n";
	if (bili_x <= a1)
	{
		a1_x = a[0] - 0;
		a2_x = a[0] - (1 * a_x) / 3;
		/* cout<<"1/3 a1_x："<<a1_x<<"\n";*/
	}
	else
	{
		if (bili_x <= a2)
		{
			a1_x = a[0] - (1 * a_x) / 3;
			a2_x = a[0] - (2 * a_x) / 3;
			/*   cout<<"2/3 a1_x："<<a1_x<<"\n";*/
		}
		else
		{
			a1_x = a[0] - (2 * a_x) / 3;
			a2_x = a[0] - a_x;
			/*	    cout<<"3/3 p_1_xy[0].x："<<p_1_xy[0].x<<"\n";
			cout<<"3/3 a_x："<<a_x<<"\n";
			cout<<"3/3 a1_x："<<a1_x<<"\n";*/
		}
	}

	//cout << "a1_x：" << a1_x << "\n";
	//cout << "a2_x：" << a2_x << "\n";

	if (bili_y <= (double)1 / 5)
	{
		b1_y = b[0] - 0;
		b2_y = b[0] - (1 * b_y) / 5;//y向上为正向，所以为 - 减法;
	}

	if ((bili_y >(double)1 / 5) && (bili_y <= (double)2 / 5))
	{
		b1_y = b[0] - (1 * b_y) / 5;
		b2_y = b[0] - (2 * b_y) / 5;//y向上为正向，所以为 - 减法;
	}

	if ((bili_y >(double)2 / 5) && (bili_y <= (double)3 / 5))
	{
		b1_y = b[0] - (2 * b_y) / 5;
		b2_y = b[0] - (3 * b_y) / 5;//y向上为正向，所以为 - 减法;
	}

	if ((bili_y >(double)3 / 5) && (bili_y <= (double)4 / 5))
	{
		b1_y = b[0] - (3 * b_y) / 5;
		b2_y = b[0] - (4 * b_y) / 5;//y向上为正向，所以为 - 减法;
		//cout << "3/5b1_y：" << b1_y << "\n";
		//cout << "3/5b2_y：" << b2_y << "\n";
	}

	if ((bili_y >(double)4 / 5) && (bili_y <= 1))
	{
		b1_y = b[0] - (4 * b_y) / 5;
		b2_y = b[0] - b_y;//y向上为正向，所以为 - 减法;
		//cout << "4/5b1_y：" << b1_y << "\n";
		//cout << "4/5b2_y：" << b2_y << "\n";
	}

	//cout << "b1_y：" << b1_y << "\n";
	//cout << "b2_y：" << b2_y << "\n";


	save_xy[0].x = a1_x;
	save_xy[0].y = b1_y;
	save_xy[0].z = 0;

	save_xy[1].x = a2_x;
	save_xy[1].y = b1_y;
	save_xy[1].z = 0;

	save_xy[2].x = a1_x;
	save_xy[2].y = b2_y;
	save_xy[2].z = 0;

	save_xy[3].x = a2_x;
	save_xy[3].y = b2_y;
	save_xy[3].z = 0;

	//用这些点遍历24个点，找到与24个点相同x,y坐标的点，并取出相同x,y点的Z值
	/*  p_1_xy[k].x =
	p_1_xy[k].y =
	p_1_xy[k].z = */

	for (int k = 0; k < 24; k++)
	{
		for (int i = 0; i < 4; i++)
		{
			if ((a[k] == save_xy[i].x) && (b[k] == save_xy[i].y))
			{
				save_xy[i].z = c[k];
				//cout << "4zzz-p_1_xy[k].x：" << p_1_xy[k].x << "\n";
				//cout << "4zzz-p_1_xy[k].y：" << p_1_xy[k].y << "\n";
			}

		}
	}

	//cout << "save_xy[0].z：" << save_xy[0].z << "\n";
	//cout << "save_xy[1].z：" << save_xy[1].z << "\n";
	//cout << "save_xy[2].z：" << save_xy[2].z << "\n";
	//cout << "save_xy[3].z：" << save_xy[3].z << "\n";

	kk_xy = (save_xy[3].y - save_xy[0].y) / (save_xy[3].x - save_xy[0].x);
	bb_xy = save_xy[0].y - (kk_xy)*(save_xy[0].x);

	yy_xy = kk_xy*point_x + bb_xy;// y= kx+b,判定输入点是在直线上方还是下方，用于确定构成平面的三个点 sangedian_xy[3]

	if (yy_xy >= point_y)//点在直线下方,如在直线上，也默认判断为在直线下方0,3,2三个点
	{
		sangedian_xy[0].x = save_xy[0].x;
		sangedian_xy[0].y = save_xy[0].y;
		sangedian_xy[0].z = save_xy[0].z;

		sangedian_xy[1].x = save_xy[3].x;
		sangedian_xy[1].y = save_xy[3].y;
		sangedian_xy[1].z = save_xy[3].z;

		sangedian_xy[2].x = save_xy[2].x;
		sangedian_xy[2].y = save_xy[2].y;
		sangedian_xy[2].z = save_xy[2].z;
	}
	else//点在直线上方0,3,1三个点
	{
		sangedian_xy[0].x = save_xy[0].x;//x1
		sangedian_xy[0].y = save_xy[0].y;//y1
		sangedian_xy[0].z = save_xy[0].z;//z1

		sangedian_xy[1].x = save_xy[3].x;//x2
		sangedian_xy[1].y = save_xy[3].y;//y2
		sangedian_xy[1].z = save_xy[3].z;//z2

		sangedian_xy[2].x = save_xy[1].x;//x3
		sangedian_xy[2].y = save_xy[1].y;//y3
		sangedian_xy[2].z = save_xy[1].z;//z3
	}
	//cout << "sangedian_xy[0].x：" << sangedian_xy[0].x << "\n";
	//cout << "sangedian_xy[0].y：" << sangedian_xy[0].y << "\n";
	//cout << "sangedian_xy[0].z：" << sangedian_xy[0].z << "\n";
	//cout << "sangedian_xy[1].x：" << sangedian_xy[1].x << "\n";
	//cout << "sangedian_xy[1].y：" << sangedian_xy[1].y << "\n";
	//cout << "sangedian_xy[1].z：" << sangedian_xy[1].z << "\n";
	//cout << "sangedian_xy[2].x：" << sangedian_xy[2].x << "\n";
	//cout << "sangedian_xy[2].y：" << sangedian_xy[2].y << "\n";
	//cout << "sangedian_xy[2].z：" << sangedian_xy[2].z << "\n";


	//计算三个点确定的平面
	aa_xyz = (sangedian_xy[1].y - sangedian_xy[0].y)*(sangedian_xy[2].z - sangedian_xy[0].z) - (sangedian_xy[1].z - sangedian_xy[0].z)*(sangedian_xy[2].y - sangedian_xy[0].y);
	bb_xyz = (sangedian_xy[2].x - sangedian_xy[0].x)*(sangedian_xy[1].z - sangedian_xy[0].z) - (sangedian_xy[1].x - sangedian_xy[0].x)*(sangedian_xy[2].z - sangedian_xy[0].z);
	cc_xyz = (sangedian_xy[1].x - sangedian_xy[0].x)*(sangedian_xy[2].y - sangedian_xy[0].y) - (sangedian_xy[2].x - sangedian_xy[0].x)*(sangedian_xy[1].y - sangedian_xy[0].y);
	dd_xyz = -(aa_xyz*sangedian_xy[0].x + bb_xyz*sangedian_xy[0].y + cc_xyz*sangedian_xy[0].z);
	//ax + by +cz + d =0; z= -(d +ax + by)/c
	zz_xyz = -(dd_xyz + aa_xyz*point_x + bb_xyz*point_y) / (cc_xyz);

	//cout << "aa_xyz：" << aa_xyz << "\n";
	//cout << "bb_xyz：" << bb_xyz << "\n";
	//cout << "cc_xyz：" << cc_xyz << "\n";
	//cout << "dd_xyz：" << dd_xyz << "\n";
	//cout << "zz_xyz：" << zz_xyz << "\n";

	height_xy = zz_xyz;
	return height_xy;

	//m_Dev_Drivers.Dev_Single_Mov(3, height_xy - z_pos, 5, false, true);
	//z_pos = height_xy;
	//m_Scanning_States = Capture_Image; //聚焦完成，则抓图

}

int Scanning_Control::Do_restting_Model()
{

	Handlehold_Thread_Running = false;
	s_Dev_Drivers->Dev_ENA(1, true);
	s_Dev_Drivers->Dev_ENA(2, true);
	s_Dev_Drivers->Dev_ENA(3, true);

	s_Dev_Drivers->Dev_Single_Mov_Para(1);
	s_Dev_Drivers->Dev_Single_Mov_Para(2);
	s_Dev_Drivers->Dev_Single_Mov_Para(3);

	//s_Dev_Drivers->Dev_Single_Mov_Para(4);
	//s_Dev_Drivers->Dev_Single_Mov_Para(5);

	int Back_Num;
	int states = Scanning_intial;
	while (states != Scanning_Finished)
	{
		switch (states)
		{
		case Scanning_intial:
			Back_Num = Do_Scanning_intial();
			if (Back_Num == State_Finished)
			{
				//内部直接增加的Z轴预先下降一定高度，避免物镜复位时，发生干涉
				s_Dev_Drivers->Dev_Single_Mov(3, s_Scanning_System_Para.Switch_Objective_ZPos, 3, false);
				Sleep(200);
				vector<bool> status_arr = { 0 };
				int k = 1;
				//while ((k < 1000) && (!status_arr[0]))
				while (!status_arr[0])
				{
					vector<UINT> Dev_id = { 0 };
					Dev_id[0] = { 3 };
					s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
					Sleep(20);
					//k++;
				}
				states = Resetting_Table;
			}
					
			break;
			//******复位过程，首先Z轴复位，然后物镜转盘复位，最后X,Y同时复位******//
		case Resetting_Table:
			Back_Num = Do_PUSIRobot_Resetting(4);
			states = Resetting_Table_Finished;
			break;
		case Resetting_Table_Finished:
			//Back_Num = Do_Resetting_Finished(4);
			Back_Num = State_Finished;
			if (Back_Num == State_Finished)
			{
				s_Dev_Drivers->Dev_PUSIRobot_Set_Zero_Postion(4);
				states = Resetting_X_Y;
			}
			if (Back_Num == State_Freeze)
				states = Resetting_Table_Finished;
			break;
		case Resetting_X_Y:
			Back_Num = Do_Resetting(1);
			states = Resetting_X_Y_Finished;
			break;
		case Resetting_X_Y_Finished:
			Back_Num = Do_Resetting_Finished(1);
			if (Back_Num == State_Finished)//State_Finished 表示查询到完成状态
			{
				s_Dev_Drivers->Dev_ENA(3, 0);
				states = Resetting_Z;
			}
			if (Back_Num == State_Freeze)//State_Freeze表示本次查询没有查询到完成状态
				states = Resetting_X_Y_Finished;
			break;
		case Resetting_Z:
			Back_Num = Do_Resetting(3, true);//快速复位
			states = Resetting_Z_Finished;
			break;
		case Resetting_Z_Finished:
			Back_Num = Do_Resetting_Finished(3);
			if (Back_Num == State_Finished)
				states = Resetting_Z_Again;
			if (Back_Num == State_Freeze)
				states = Resetting_Z_Finished;
			break;
		case Resetting_Z_Again:
			Back_Num = Do_Resetting(3, false);//低速复位
			states = Resetting_Z_Again_Finished;
			break;
		case Resetting_Z_Again_Finished:
			Back_Num = Do_Resetting_Finished(3);
			if (Back_Num == State_Finished)
			{
				m_X_Moveto = 0;
				m_Y_Moveto = 0;
				m_Z_Moveto = 0;
				states = Scanning_Finished;
			}
					
			if (Back_Num == State_Freeze)
				states = Resetting_Z_Again_Finished;
			break;

		default:
			break;
		}
	}
	return 1;
}
int Scanning_Control::Do_Scanning_Model()
{

	int Back_Num, Back_Num1, Back_Num2;
	//int states = Z_MoveDown_Avoid;
	int states = Move_To_Slide_Start_Postion;
	Handlehold_Thread_Running = false;

	Do_Scanning_intial();//扫描前运行参数初始化函数

	//将扫描参数传递到主界面
	s_Location.x_all = s_User_Para.Scanning_Num_X;
	s_Location.y_all = s_User_Para.Scanning_Num_Y;

	//s_Scanning_Para.Focal_Length.resize(s_User_Para.Focusing_Num_Y*s_User_Para.Focusing_Num_X);
	//s_Scanning_Para.X_Focus_Location.resize(s_User_Para.Focusing_Num_Y*s_User_Para.Focusing_Num_X);
	//s_Scanning_Para.Y_Focus_Location.resize(s_User_Para.Focusing_Num_Y*s_User_Para.Focusing_Num_X);


	//s_Dev_Drivers->Dev_Single_Mov_Para(1);
	//s_Dev_Drivers->Dev_Single_Mov_Para(2);
	//s_Dev_Drivers->Dev_Single_Mov_Para(3);
	//s_Dev_Drivers->Dev_Single_Mov_Para(4);
	//s_Dev_Drivers->Dev_Single_Mov_Para(5);
	//Sleep(1000);

	if (!s_User_Para.NewStart_OR_BreakPoint)//断点启动额外初始化
	{
		Calculate_Scanning_Focal();//首先基于聚焦点计算当前扫描点的焦距值
		m_X_Moveto = s_Scanning_Para.BreakPoint_Pos_X;
		m_Y_Moveto = s_Scanning_Para.BreakPoint_Pos_Y;
		m_Z_Moveto = s_Scanning_Para.BreakPoint_Pos_Z;
	}
	else
	{
		Calculate_Scanning_Focal();//首先基于聚焦点计算当前扫描点的焦距值
			
		s_Scanning_Para.Current_Point_Num = 0;
		//s_Scanning_Para.Current_Slide_Num = 0;
		//s_Scanning_Para.Current_Table_Num = 0;
		//m_X_Moveto = (s_User_Para.Scanning_Aero_X / 2) * 10000;
		//m_Y_Moveto = (s_User_Para.Scanning_Aero_Y / 2) * 10000;
		m_X_Moveto = s_Scanning_Para.X_Scanning_Location[s_Scanning_Para.Current_Point_Num];
		m_Y_Moveto = s_Scanning_Para.Y_Scanning_Location[s_Scanning_Para.Current_Point_Num];
		m_Z_Moveto = s_Scanning_Para.Scanning_Length[s_Scanning_Para.Current_Point_Num];
	}

	bool PushImg2Vector = false;//是否将扫描的图像直接压入Vector，给其他线程同时分析
	bool Back_Right0 = false;
	bool Back_Right1 = false;
	bool Back_Right2 = false;
	while (states != Scanning_Finished)
	{
		switch (states)
		{
			//******运动到本次扫描/聚焦需要用到的物镜******//
		case Z_MoveDown_Avoid:
			m_Z_Moveto = m_Z_Moveto + s_Scanning_System_Para.Switch_Objective_ZPos;
			Back_Num = Do_Stepping(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{
				states = Z_MoveDown_Avoid_Finished;
			}
			if (Back_Num == State_Freeze)
				states = Z_MoveDown_Avoid;
			break;
		case Z_MoveDown_Avoid_Finished:
			Back_Num = Do_Stepping_Finished(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{
				states = Move_To_Object;
			}
			if (Back_Num == State_Freeze)
				states = Z_MoveDown_Avoid_Finished;
			if (Back_Num == Stepping)
				states = Z_MoveDown_Avoid;
			break;
		case Move_To_Object:
			states = Do_Move_To_Object(s_Scanning_Para.Objective_Num);
			//Set_Camera_Paras(s_Scanning_Para.Objective_Num);
			break;
		case Move_To_Object_Fininshed:
			Back_Num = Do_Move_To_Object_Fininshed();
			if (Back_Num == State_Finished)//State_Finished 表示查询到完成状态
				states = Z_MoveDown_AvoidBack;
			if (Back_Num == State_Freeze)//State_Freeze表示本次查询没有查询到完成状态
				states = Move_To_Object_Fininshed;
			break;
		case Z_MoveDown_AvoidBack:
			m_Z_Moveto = m_Z_Moveto - s_Scanning_System_Para.Switch_Objective_ZPos;
			Back_Num = Do_Stepping(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{
				states = Z_MoveDown_AvoidBack_Finished;
			}
			if (Back_Num == State_Freeze)
				states = Z_MoveDown_AvoidBack;
			break;
		case Z_MoveDown_AvoidBack_Finished:
			Back_Num = Do_Stepping_Finished(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{
				states = states = Z_Stepping;
			}
			if (Back_Num == State_Freeze)
				states = Z_MoveDown_AvoidBack_Finished;
			if (Back_Num == Stepping)
				states = Z_MoveDown_AvoidBack;
			break;
			//******运动到扫描的第一个点位置******//
			/***********扫描时不用**************/
		case Move_To_Slide_Start_Postion:
			states = Do_Slide_Start_Postion();
			break;            
			//运动到扫描点之后
		case Move_To_Slide_Start_Postion_Finished:
			Back_Num = Do_Slide_Start_Postion_Finished();
			if (Back_Num == State_Finished)
			{
				states = X_Stepping;
			}
					
			if (Back_Num == State_Freeze)
				states = Move_To_Slide_Start_Postion_Finished;
			break;
			//*************自动扫描*************//
		case Capture_Image:
			if (s_Scanning_Para.Objective_Num == Objective_10X)
				bool PushImg2Vector = true;	
			if (s_Scanning_Para.Current_Point_Num == 0)
				Sleep(2000);
			TakeVeido_And_Save_Image_Raw(m_ScanningImage_Save_Paras, PushImg2Vector);

			s_Location.x_now = s_Scanning_Para.Current_Scanning_Num_X;
			s_Location.y_now = s_Scanning_Para.Current_Scanning_Num_Y;
			//DrawCurrentPosition(s_Location);
			Save_To_Database();

			s_Scanning_Para.Current_Point_Num++;//表示当前点已经拍摄完成，扫描点序号自加1
			states = Is_Scanning_Finished;

			//Sleep(2000);

			break;
		//case Z_Stepping:
		case X_Stepping:
			m_X_Moveto = s_Scanning_Para.X_Scanning_Location[s_Scanning_Para.Current_Point_Num];
			Do_Stepping(1, m_X_Moveto);
			m_Z_Moveto = s_Scanning_Para.Scanning_Length[s_Scanning_Para.Current_Point_Num];
			Do_Stepping(3, m_Z_Moveto);
			states = X_Stepping_Finished;
			break;
		case Y_Stepping:
			m_Y_Moveto = s_Scanning_Para.Y_Scanning_Location[s_Scanning_Para.Current_Point_Num];
			Back_Num2 = Do_Stepping(2, m_Y_Moveto);
			m_Z_Moveto = s_Scanning_Para.Scanning_Length[s_Scanning_Para.Current_Point_Num];
			Back_Num = Do_Stepping(3, m_Z_Moveto);
			states = Y_Stepping_Finished;
			break;

		//case Z_Stepping_Finished:
		case X_Stepping_Finished:
			if (!Back_Right0)
				if (Do_Stepping_Finished(1, m_X_Moveto) == State_Finished)
					Back_Right0 = true;
			if (!Back_Right2)
				if (Do_Stepping_Finished(3, m_Z_Moveto) == State_Finished)
					Back_Right2 = true;

			if (Back_Right0 && Back_Right2)
			{
				Back_Right0 = false;
				Back_Right1 = false;
				Back_Right2 = false;
				states = Capture_Image;
			}
			else
				states = X_Stepping_Finished;
			break;
		case Y_Stepping_Finished:
			if (!Back_Right1)
				if (Do_Stepping_Finished(2, m_Y_Moveto) == State_Finished)
					Back_Right1 = true;
			if (!Back_Right2)
				if (Do_Stepping_Finished(3, m_Z_Moveto) == State_Finished)
					Back_Right2 = true;

			if (Back_Right2 && Back_Right1)
				{
					Back_Right0 = false;
					Back_Right1 = false;
					Back_Right2 = false;
					states = Capture_Image;
				}				
			else
				states = Y_Stepping_Finished;
			break;
			//************判断*****************//
		case Is_Scanning_Finished:
			states = Do_Is_Scanning_Finished();
			break;
		default:
			break;
		}

	}

	//Do_Scanning_Finished();
	return 1;
}

int Scanning_Control::Do_Focusing_Model(int Focusing_Step)
{
	//  Do_restting_Model();//现复位
	//扫描之前清空所有参数
	s_Scanning_Para.X_Scanning_Location.clear();
	s_Scanning_Para.Y_Scanning_Location.clear();
	s_Scanning_Para.Scanning_Length.clear();
	s_Scanning_Para.CA_Pos_10X.clear();
	vector <Point>().swap(s_Scanning_Para.CA_Pos_10X);
	vector <int>().swap(s_Scanning_Para.Focal_Length);
	vector <int>().swap(s_Scanning_Para.X_Scanning_Location);
	vector <int>().swap(s_Scanning_Para.Y_Scanning_Location);
	vector <int>().swap(s_Scanning_Para.X_Focus_Location);
	vector <int>().swap(s_Scanning_Para.Y_Scanning_Location);
	Handlehold_Thread_Running = false;

	int Back_Num;
	int states = Z_MoveDown_Avoid;

	//计算聚焦点数
	m_Focusing_Num_X = s_User_Para.Scanning_Aero_X  / s_User_Para.m_Focus_Spacing_X + 1;
	m_Focusing_Num_Y = s_User_Para.Scanning_Aero_Y / s_User_Para.m_Focus_Spacing_Y + 1;
	if (m_Focusing_Num_X < 2)
		m_Focusing_Num_X = 2;
	if (m_Focusing_Num_Y < 2)
		m_Focusing_Num_Y = 2;

	//正常启动初始化所有变量，聚焦有关的；（包含两个部分，）
	Do_Scanning_intial();//扫描前运行参数初始化函数
	//将扫描参数传递到主界面

	s_Location.x_all = m_Focusing_Num_X;
	s_Location.y_all = m_Focusing_Num_Y;

	s_Dev_Drivers->Dev_Reset_Communication();
	s_Dev_Drivers->Dev_Single_Mov_Para(1);
	s_Dev_Drivers->Dev_Single_Mov_Para(2);
	s_Dev_Drivers->Dev_Single_Mov_Para(3);
	//s_Dev_Drivers->Dev_Single_Mov_Para(4);
	//s_Dev_Drivers->Dev_Single_Mov_Para(5);
	Sleep(1000);

	m_MaxFocLen = 0;
	Focusing_Search_Num = 0;
	Focus_Values_Max_Index = 0;
	long Z_Pos1;
	while (!s_Dev_Drivers->Dev_Read_Pos(3, Z_Pos1))
	{
		Sleep(50);
	}

	m_MaxFocLen_Z_Pos = Z_Pos1;
	Last_MaxFocus_Pos = Z_Pos1;

	//40X下，聚焦变化的区间一般为【2000，2000】，因此实际限位设置在【-5000，+5000】
	//10X下，
	Z_Upper_Limit = Z_Pos1 + Focusing_Step * 25;//s_Scanning_System_Para.Focusing_Step_Z[2]
	Z_Low_Limit = Z_Pos1 - Focusing_Step * 25;
	m_Z_Moveto = Z_Pos1;

	Z_Big_Step_Upper_Limit = Z_Pos1 + Focusing_Step * 50;
	Z_Big_Step_Low_Limit = Z_Pos1 - Focusing_Step * 50;
	Z_Big_Step_MaxFocus_Pos = Z_Pos1;
		
	vector <int>().swap(Focus_Poss);
	vector <double>().swap(Focus_Values);
	Get_Dic_F = false;
	Get_Dic_N = false;


	Focusing_Next_Step = Focusing_Step;
	Focusing_Previous_Step = Focusing_Step;
		
	if (!s_User_Para.NewStart_OR_BreakPoint)//断点启动额外初始化
	{
		m_X_Moveto = s_Scanning_Para.BreakPoint_Pos_X;
		m_Y_Moveto = s_Scanning_Para.BreakPoint_Pos_Y;
		m_Z_Moveto = s_Scanning_Para.BreakPoint_Pos_Z;
	}
	else
	{
		s_Scanning_Para.Focal_Length.resize(m_Focusing_Num_Y*m_Focusing_Num_X);

		s_Scanning_Para.X_Focus_Location.resize(m_Focusing_Num_Y*m_Focusing_Num_X);
		s_Scanning_Para.Y_Focus_Location.resize(m_Focusing_Num_Y*m_Focusing_Num_X);
			
		m_point_i = 0;//聚焦记录点数归零
		s_Scanning_Para.Current_Point_Num = 0;
		//s_Scanning_Para.Current_Slide_Num = 0;
		//s_Scanning_Para.Current_Table_Num = 0;
		m_X_Moveto = (s_User_Para.Scanning_Aero_X / 2) * 10000;
		m_Y_Moveto = (s_User_Para.Scanning_Aero_Y / 2) * 10000;
	}

	//Scanning_10X_img.clear();//清空存放实时图像的Vector
	clock_t start_c = clock();
	clock_t Stop_c = clock();
	int Focus_Level_100X = 0;
	int jjjj = 0;
	while (states != Scanning_Finished)
	{
		switch (states)
		{
			//******运动到本次扫描/聚焦需要用到的物镜******//
		case Z_MoveDown_Avoid:
			m_Z_Moveto = s_Scanning_System_Para.Switch_Objective_ZPos;
			Back_Num = Do_Stepping(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{
				states = Z_MoveDown_Avoid_Finished;
			}
			if (Back_Num == State_Freeze)
				states = Z_MoveDown_Avoid;
			break;
		case Z_MoveDown_Avoid_Finished:
			Back_Num = Do_Stepping_Finished(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{
				states = Move_To_Object;
			}
			if (Back_Num == State_Freeze)
				states = Z_MoveDown_Avoid_Finished;
			if (Back_Num == Stepping)
				states = Z_MoveDown_Avoid;
			break;
		case Move_To_Object:
			states = Do_Move_To_Object(s_Scanning_Para.Objective_Num);
			//Set_Camera_Paras(s_Scanning_Para.Objective_Num);
			break;
		case Move_To_Object_Fininshed:
			Back_Num = Do_Move_To_Object_Fininshed();
			if (Back_Num == State_Finished)//State_Finished 表示查询到完成状态
			{
				//m_Z_Moveto = m_Z_Moveto - s_Scanning_System_Para.Switch_Objective_ZPos;
				m_Z_Moveto = 0;
				states = Z_MoveDown_AvoidBack;
			}
			if (Back_Num == State_Freeze)//State_Freeze表示本次查询没有查询到完成状态
				states = Move_To_Object_Fininshed;
			break;
		case Z_MoveDown_AvoidBack:				
			Back_Num = Do_Stepping(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{
				states = Z_MoveDown_AvoidBack_Finished;
			}
			if (Back_Num == State_Freeze)
				states = Z_MoveDown_AvoidBack;
			break;
		case Z_MoveDown_AvoidBack_Finished:
			Back_Num = Do_Stepping_Finished(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{
				//计算获取实际的初始焦平面
				////一次找基准焦平面
				//int Focus_Level_Search_Step = s_Scanning_System_Para.Focusing_Step_Z[0];
				//if (s_Scanning_Para.Objective_Num != Objective_10X)
				//	Focus_Level_Search_Step = s_Scanning_System_Para.Focusing_Step_Z[0]/2;
				//Do_Find_Focus_Level_100X(Focus_Level_Search_Step);

				//两次找基准焦平面
				int Focus_Level_Search_Step = s_Scanning_System_Para.Focusing_Step_Z[0];
				if (s_Scanning_Para.Objective_Num != Objective_10X)
				{
					Do_Find_Focus_Level_100X(Focus_Level_Search_Step, MeanStd_Operator);
					Focus_Level_Search_Step = s_Scanning_System_Para.Focusing_Step_Z[0] / 5;
					Do_Find_Focus_Level_100X_Climb(Focus_Level_Search_Step, Laplacian_Operator, false);  
					//Do_Find_Focus_Level_100X(Focus_Level_Search_Step, Laplacian_Operator, false);
				}
				else
				{
					//Do_Find_Focus_Level_100X_Climb(Focus_Level_Search_Step, Laplacian_Operator, false);
					Do_Find_Focus_Level_100X(Focus_Level_Search_Step);
				}

				//Sleep(1000);

				Obtain_Optimal_Focal = false;
				Focus_Level_100X = m_Z_Moveto;
				m_MaxFocLen_Z_Pos = m_Z_Moveto;
				Last_MaxFocus_Pos = m_Z_Moveto;
				Z_Upper_Limit = m_Z_Moveto + Focusing_Step * 100;
				Z_Low_Limit = m_Z_Moveto - Focusing_Step * 100;
				Z_Big_Step_Upper_Limit = m_Z_Moveto + Focusing_Step * 50;
				Z_Big_Step_Low_Limit = m_Z_Moveto - Focusing_Step * 50;
				Z_Big_Step_MaxFocus_Pos = m_Z_Moveto;

				states = Move_To_Slide_Start_Postion;
			}
			if (Back_Num == State_Freeze)
				states = Z_MoveDown_AvoidBack_Finished;
			if (Back_Num == Stepping)
				states = Z_MoveDown_AvoidBack;
			break;

			//******运动到扫描的第一个点位置******//
		case Move_To_Slide_Start_Postion:
			states = Do_Slide_Start_Postion();
			break;
		case Move_To_Slide_Start_Postion_Finished:
			Back_Num = Do_Slide_Start_Postion_Finished();
			if (Back_Num == State_Finished)
			{
				//m_Searching_Model = Big_Step;
				m_Searching_Model = Little_Step;
					
				Obtain_Optimal_Focal = false;
				//states = Search_Optimal_Focal_Distance;
				states = Z_Stepping;
			}
			if (Back_Num == State_Freeze)
				states = Move_To_Slide_Start_Postion_Finished;
			break;

			//*************自动聚焦*************//
		case Search_Optimal_Focal_Distance:
			if (Obtain_Optimal_Focal)
			{
				//本次聚焦完成，存储当前的
				s_Scanning_Para.Focal_Length[s_Scanning_Para.Current_Point_Num] = m_Z_Moveto;//保存当前的焦距值
				s_Scanning_Para.X_Focus_Location[s_Scanning_Para.Current_Point_Num] = m_X_Moveto;//保存当前的坐标的X绝对值
				s_Scanning_Para.Y_Focus_Location[s_Scanning_Para.Current_Point_Num] = m_Y_Moveto;//保存当前的坐标的Y绝对值
				//s_Scanning_Para.Focal_Length.push_back(m_Z_Moveto);
				//s_Scanning_Para.X_Focus_Location.push_back(m_X_Moveto);
				//s_Scanning_Para.Y_Focus_Location.push_back(m_Y_Moveto);
				Get_Dic_N = false;
				Get_Dic_F = false;
				Focusing_Search_Num = 0;
				Focus_Values_Max_Index = 0;
				double sum =0;
				vector<int>::const_iterator itc = s_Scanning_Para.Focal_Length.begin();
				int Aero_Temp = 0;
				while (itc != s_Scanning_Para.Focal_Length.end())
				{
					sum = sum + *itc;
					itc++;
				}
				Last_MaxFocus_Pos = sum / (s_Scanning_Para.Current_Point_Num+1); //均值
				//Last_MaxFocus_Pos = m_Z_Moveto;

				TakeVeido_And_Save_Image_Raw(m_FocusingImage_Save_Paras,true);	

				//s_Location.x_now = s_Scanning_Para.Current_Scanning_Num_X;
				//s_Location.y_now = s_Scanning_Para.Current_Scanning_Num_Y;
				//DrawCurrentPosition(s_Location);

				//Get_CA_Pos_10X_Func(s_Scanning_Para.Current_Point_Num + 1);

				//m_Z_Moveto = Z_Upper_Limit;

				s_Scanning_Para.Current_Point_Num++;
				m_point_i = 0;//聚焦记录点数归零
				//m_Searching_Model = Big_Step;
				m_Searching_Model = Little_Step;
				Obtain_Optimal_Focal = false;
				m_Focusing_Equal_Scaning = false;

				//m_Z_Moveto = Focus_Level_100X;//强制将下一个点预测焦距值为当前值-------------

				states = Is_Scanning_Finished;
			}
			else
			{
				start_c = clock();
				//
				m_MaxFocLen = Calculate_Sharpness_Every_Point(Focus_Level_100X);
					
				Stop_c = clock();
				double jj = Stop_c - start_c;				

				states = Z_Stepping;
			}
			break;
		case Z_Stepping:
			//if (m_Focusing_Equal_Scaning)
			//{
			//	states = Search_Optimal_Focal_Distance;
			//	break;
			//}
			//if (m_Sobel_Wrong)
			//{
			//	m_X_Moveto = m_X_Moveto + 3200;

			//	m_point_i = 0;//聚焦记录点数归零
			//	//m_Searching_Model = Big_Step;
			//	m_Searching_Model = Little_Step;
			//	Obtain_Optimal_Focal = false;
			//	m_Sobel_Wrong = false;
			//	states = X_Stepping;
			//	break;
			//}
			start_c = clock();

			jjjj = m_Z_Moveto;

			Back_Num = Do_Stepping(3, m_Z_Moveto);
			states = Z_Stepping_Finished;
			break;
		case Z_Stepping_Finished:
			Back_Num = Do_Stepping_Finished(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{
				Sleep(50);
				Stop_c = clock();
				double jj = Stop_c - start_c;
				states = Search_Optimal_Focal_Distance;
			}
			if (Back_Num == State_Freeze)
				states = Z_Stepping_Finished;
			if (Back_Num == Stepping)
				states = Z_Stepping;
			break;
			//************判断*****************//
		case Is_Scanning_Finished:

			states = Do_Is_Scanning_Finished();
			if (states == Y_Stepping)
				m_Y_Moveto = m_Y_Moveto + m_Scanning_Step_Y;
			if (states == X_Stepping)
				m_X_Moveto = m_X_Moveto + m_Scanning_Step_X;
			break;
			//*************运动到下一个点*******************//
		case X_Stepping:
			//Sleep(1000);
			Back_Num = Do_Stepping(1, m_X_Moveto);
			states = X_Stepping_Finished;
			break;
		case X_Stepping_Finished:
			Back_Num = Do_Stepping_Finished(1, m_X_Moveto);
			if (Back_Num == State_Finished)
			{					
				/*states = Search_Optimal_Focal_Distance;*/
				states = Z_Stepping;
			}
					
			if (Back_Num == State_Freeze)
				states = X_Stepping_Finished;
			if (Back_Num == Stepping)
				states = X_Stepping;
			break;
		case Y_Stepping:
			//Sleep(1000);
			Back_Num = Do_Stepping(2, m_Y_Moveto);
			states = Y_Stepping_Finished;
			break;
		case Y_Stepping_Finished:
			Back_Num = Do_Stepping_Finished(2, m_Y_Moveto);
			if (Back_Num == State_Finished)
			{
				//Sleep(500);
				//states = Search_Optimal_Focal_Distance;
				states = Z_Stepping;
			}
			if (Back_Num == State_Freeze)
				states = Y_Stepping_Finished;
			if (Back_Num == Stepping)
				states = Y_Stepping;
			break;
		default:
			break;
		}

	}

	//Do_Scanning_Finished();
	return 1;
}

int  Scanning_Control::Do_Optimal_Focal_Distance_Searching_Model(int Search_Model)
{
	
	//*******************初始化用到的变量，*****************************//
		
	memset(s_Scanning_Para.save, 0, sizeof(int)* 100);//存储每个聚焦点的评价函数值
	memset(s_Scanning_Para.High, 0, sizeof(int)* 100);//存储每个聚焦点的Z轴绝对位置
	//m_point_i = 0;//聚焦记录点数归零
	m_Z_Move_Arrived = true;
	Obtain_Optimal_Focal = false;

    bool Searching_Now = false;
	m_Respond_Check_Num = 0;

    Mat Focus_Pic;

	switch (Search_Model)
	{
	case Big_Step:
			
		Take_Video_Image(Focus_Pic);
		m_MaxFocLen = Calculate_Sharpness(1, m_Z_Moveto, Focus_Pic);//m_MaxFocLen_Z_Pos,1

		m_Score_Aero = NULL;//将通过阀值标记的矩阵设置为零
		while (!Searching_Now && !m_Sobel_Wrong && !m_Focusing_Equal_Scaning)
			{
				Searching_Now = Do_Search_Optimal_Focal_Length(true, Search_Model);//首先是大步长搜索，采用方差评价（true），模式为0

			}
		//m_Searching_Model = Middle_Step;

		m_Searching_Model = Little_Step;//直接进入小步长
		return Z_Stepping;
	case Middle_Step:
		while (!Searching_Now && !m_Sobel_Wrong)
		{
			Searching_Now = Do_Search_Optimal_Focal_Length(true, Search_Model);//中步长搜索，采用false，模式为1
		}
		m_Searching_Model = Little_Step;
		return Z_Stepping;
	case Little_Step:

		Take_Video_Image(Focus_Pic);
		m_MaxFocLen = Calculate_Sharpness(1, m_Z_Moveto, Focus_Pic);//m_MaxFocLen_Z_Pos,1
			
		while (!Searching_Now && !m_Sobel_Wrong && !m_Focusing_Equal_Scaning)
		{
			Searching_Now = Do_Search_Optimal_Focal_Length(false, Search_Model);//小步长搜索，采用false，模式为2

			if ((m_Z_Moveto < Z_Low_Limit) || (m_Z_Moveto > Z_Upper_Limit))
			{
				m_MaxFocLen_Z_Pos = Last_MaxFocus_Pos;
				Searching_Now = true;
			}
			if (m_Focusing_Equal_Scaning)
			{
				m_MaxFocLen_Z_Pos = Last_MaxFocus_Pos;
			}
			//////if ((m_Z_Moveto < Z_Big_Step_Low_Limit) || (m_Z_Moveto > Z_Big_Step_Upper_Limit))
			//////{
			//////	m_MaxFocLen_Z_Pos = Z_Big_Step_MaxFocus_Pos;
			//////	Searching_Now = true;
			//////}
		}

		Z_Big_Step_Upper_Limit = m_MaxFocLen_Z_Pos + 10000;
		Z_Big_Step_Low_Limit = m_MaxFocLen_Z_Pos - 10000;

		m_Z_Moveto = m_MaxFocLen_Z_Pos;
		Obtain_Optimal_Focal = true;
		return Z_Stepping;

	}
	
	/***************以下添加搜索最佳焦距的算法，例如爬山法算法,判断是否获得了最佳焦距值*********************/
	//说明，如果找到了最佳焦距，那么直接将这个最佳焦距，赋值到自动聚焦相应的矩阵中去
	/*
	Focus_Pic_Num++;

	bool Obtain_Best_focal = true;//如果下面的判断中，得到当前的图像时最清新的图像，那么这个bool变量被赋值为真
	if (Focus_Pic_Num >= 3)
	{
	bool Just_go0, Just_go1, Go_Back, Just_Right;
	Just_go0 = (s_Scanning_Para.save[m_point_i - 2] > s_Scanning_Para.save[m_point_i - 1]) &&
	(s_Scanning_Para.save[m_point_i - 3] > s_Scanning_Para.save[m_point_i - 2]);
	Just_go1 = (s_Scanning_Para.save[m_point_i - 1] > s_Scanning_Para.save[m_point_i - 2]) &&
	(s_Scanning_Para.save[m_point_i - 3] > s_Scanning_Para.save[m_point_i - 2]);

	Go_Back = (s_Scanning_Para.save[m_point_i - 1] > s_Scanning_Para.save[m_point_i - 2]) &&
	(s_Scanning_Para.save[m_point_i - 2] > s_Scanning_Para.save[m_point_i - 3]);

	if (Just_go0 && Just_go1)//正方向继续寻找
	{
	Focus_Pic_Num--;
	m_Focus_Step = s_Scanning_System_Para.Focusing_Step_Z_Max;
	Obtain_Best_focal = false;
	}

	else if (Go_Back)
	{
	Focus_Pic_Num = 1;
	/*
	double F_Value = s_Scanning_Para.save[m_point_i - 1];
	s_Scanning_Para.save[m_point_i - 1] = s_Scanning_Para.save[m_point_i - 3];
	s_Scanning_Para.save[m_point_i - 3] = F_Value;
	double H_Value = s_Scanning_Para.save[m_point_i - 1];
	s_Scanning_Para.High[m_point_i - 1] = s_Scanning_Para.High[m_point_i - 3];
	s_Scanning_Para.High[m_point_i - 3] = H_Value;
	*/
	/*
	m_Focus_Step = -1 * s_Scanning_System_Para.Focusing_Step_Z_Max;

	Obtain_Best_focal = false;
	}
	else
	{
	Focus_Pic_Num = 0;
	Obtain_Best_focal = true;
	}
	}
		
	bool Obtain_Best_focal = true;

	/******************以下是获取了最佳焦距后，进行的系列操作，读取当前的位置，保存当前抓取的图像，***********************/
	/*if (Obtain_Best_focal)
	{
		/*
		long Z_Real_Pos;//当前最佳焦距值；
		s_Dev_Drivers->Dev_Read_Pos({ 3 }, Z_Real_Pos);
		s_Scanning_Para.Focal_Length[s_Scanning_Para.Point_Num] = Z_Real_Pos;//保存当前的焦距值
		s_Dev_Drivers->Dev_Read_Pos({ 1 }, Z_Real_Pos);
		s_Scanning_Para.X_Focus_Location[s_Scanning_Para.Point_Num] = Z_Real_Pos;//保存当前的坐标的X绝对值
		s_Dev_Drivers->Dev_Read_Pos({ 2 }, Z_Real_Pos);
		s_Scanning_Para.Y_Focus_Location[s_Scanning_Para.Point_Num] = Z_Real_Pos;//保存当前的坐标的Y绝对值

		*/
		//long gg = Z_Real_Pos[0];
		//int jj = 0;


		/*return State_Finished;
	}
	else
	{
	//****************将聚焦Z轴步长重新设置为粗步长*******************///
	/*	m_Focus_Step = s_Scanning_System_Para.Focusing_Step_Z[2];
		//*********************************///

		//****************将聚焦Z轴下一步步长进行估计与预测 *******************//



	/*	return State_Freeze;
	}*/

	return State_Finished;

}
	
bool Scanning_Control::Get_Camera_Info()
{

	FLOAT fValue = 0.0;
	LONG lFlag;
	double StartTime = GetTickCount();
	if (!m_bConnected)
	{
		return false;
	}
	//获取参数
	if (!LucamGetFormat(m_hCamera, &m_lffFormat, &m_fFrameRate))
	{
		return (false);
	}
	//判断彩色相机和黑白相机
	if (!LucamGetProperty(m_hCamera, LUCAM_PROP_COLOR_FORMAT, &fValue, &lFlag))
	{
		AfxMessageBox(_T("Failed to determine if camera is mono or color. Assuming mono."));
		m_bIsColor = FALSE;
		return (false);
	}

	if (fValue == LUCAM_CF_MONO)
		m_bIsColor = FALSE;
	else
		m_bIsColor = TRUE;

	// Allocate memory to store frames
	m_iSize = (m_lffFormat.width * m_lffFormat.height) / (m_lffFormat.subSampleX * m_lffFormat.subSampleY);
	if (m_lffFormat.pixelFormat == LUCAM_PF_16)
		m_iSize *= 2;

	m_ulWidth = m_lffFormat.width / m_lffFormat.subSampleX;
	m_ulHeight = m_lffFormat.height / m_lffFormat.subSampleY;

	if (!LucamGetCameraId(m_hCamera, &m_ulCameraId)) m_ulCameraId = 0;
	return true;

}

bool Scanning_Control::Connnect_Camera()
{
	CString csTemp;
	FLOAT fValue = 0.0;
	LONG lFlags;
	ULONG ulCamera = 1;

	if (!m_bConnected)
	{
		m_hCamera = LucamCameraOpen(ulCamera);
		if (m_hCamera != NULL)
		{
			Get_Camera_Info();
			m_bConnected = TRUE;

			if (!LucamGetProperty(m_hCamera, LUCAM_PROP_JPEG_QUALITY, &fValue, &lFlags))
			{
				AfxMessageBox(_T("Failed to get current JPG quality factor."));
			}

			// Get pixel depth
			ULONG ulCount;
			CString csBlah;
			DWORD dwError;
			if (!LucamGetTruePixelDepth(m_hCamera, &ulCount))
			{
				dwError = LucamGetLastError();
				csBlah.Format(_T("Unable to get true pixel depth. Error %d"), dwError);
				AfxMessageBox(csBlah);
			}

			return true;

		}
		else
		{
			AfxMessageBox(_T("Unable to connect to the camera."));
			return false;
		}

	}
	return true;
}

bool Scanning_Control::Video_Preview(HWND Dlg_hWnd)
{
	CString csTemp;
	//RECT rPreview;
	ULONG Zoom_Width;
	ULONG Zoom_Higth;
	if (!m_bPreviewing)
	{
		// Start the preview
		if (!LucamGetFormat(m_hCamera, &m_lffFormat, &m_fFrameRate))
		{
			return (false);
		}

		m_ulWidth = m_lffFormat.width / m_lffFormat.subSampleX;
		m_ulHeight = m_lffFormat.height / m_lffFormat.subSampleY;			

		if (m_fZoom == 0.25)
		{
			Zoom_Width = m_ulWidth / 4;//临时修改为 2 原来为 m_ulWidth / 4
			Zoom_Higth = m_ulHeight /4;
		}
		else if (m_fZoom == 0.5)
		{
			Zoom_Width = m_ulWidth / 2;
			Zoom_Higth = m_ulHeight / 2;
		}
		else
		{
			Zoom_Width = m_ulWidth * (ULONG)m_fZoom;
			Zoom_Higth = m_ulHeight * (ULONG)m_fZoom;
		}
		if (!LucamStreamVideoControl(m_hCamera, START_DISPLAY, Dlg_hWnd))
		{
			AfxMessageBox(_T("Unable start previewing video."));
			LucamDestroyDisplayWindow(m_hCamera);
			return (false);
		}
		else
		{
			m_bPreviewing = true;

		}

		if (!LucamAdjustDisplayWindow(m_hCamera, NULL, 0, 0, Zoom_Width, Zoom_Higth))//Zoom_Width, Zoom_Higth
		{
			AfxMessageBox(_T("Failed to readjust display window."));
		}

	}
	else
	{
		//if (!m_ScrollingPreviewDialog->StopPreview(TRUE))
		//	AfxMessageBox(_T("Unable STOP previewing video."));

		m_bPreviewing = FALSE;
	}

	return true;

}

bool Scanning_Control::Capture_Video_Image(Mat &Out_Video_Imagem)
{
	BYTE *pFrame;
	BYTE *pRgbFrame;
	LUCAM_CONVERSION lcConversion;
	LUCAM_IMAGE_FORMAT lifFormat;
	double StartTime = GetTickCount();

	pFrame = new BYTE[m_iSize];
	if (pFrame == NULL)
	{
		AfxMessageBox(_T("内存不足."));
		return false;
	}
	pRgbFrame = new BYTE[m_iSize * 3];
	if (pRgbFrame == NULL)
	{
		delete pFrame;
		AfxMessageBox(_T("内存不足."));
		return false;
	}

	if (!LucamTakeVideo(m_hCamera, 1, pFrame))
	{
		delete pFrame;
		delete pRgbFrame;
		AfxMessageBox(_T("抓图失败."));
		return false;
	}

	if (!m_bIsColor)
	{
		// Get image format
		lifFormat.Size = sizeof(LUCAM_IMAGE_FORMAT);
		if (LucamGetVideoImageFormat(m_hCamera, &lifFormat))
		{
			// Correct for mono checkerboard pattern (should not be necessary for most cameras)
			if (!LucamPerformMonoGridCorrection(m_hCamera, pFrame, &lifFormat))
			{
				AfxMessageBox(_T("Failed to perform mono grid correction."));
			}
		}
		else
		{

			AfxMessageBox(_T("Failed to perform mono grid correction."));
		}

	}
	double endletime = GetTickCount();
	double pppp = StartTime - endletime;
	StartTime = GetTickCount();
	// Convert to RGB
	lcConversion.CorrectionMatrix = LUCAM_CM_FLUORESCENT;
	lcConversion.DemosaicMethod = LUCAM_DM_HIGHER_QUALITY;

	if (m_lffFormat.pixelFormat == LUCAM_PF_16)
	{
		if (!LucamConvertFrameToRgb48(m_hCamera, (USHORT *)pRgbFrame, (USHORT *)pFrame, m_ulWidth, m_ulHeight, m_lffFormat.pixelFormat, &lcConversion))
		{
			delete pFrame;
			delete pRgbFrame;
			AfxMessageBox(_T("转化为RGB格式失败."));
			return false;
		}
	}
	else
	{
		if (!LucamConvertFrameToRgb24(m_hCamera, pRgbFrame, pFrame, m_ulWidth, m_ulHeight, m_lffFormat.pixelFormat, &lcConversion))
		{
			delete pFrame;
			delete pRgbFrame;
			AfxMessageBox(_T("转化为RGB格式失败."));
			return false;
		}

	}

	endletime = GetTickCount();
	pppp = StartTime - endletime;

	int lineByte = 0;
	lineByte = (m_ulWidth * 3 + 3) / 4 * 4; //格式宽度  

	//cv::Mat iMat(m_ulHeight, m_ulWidth, CV_8UC3, pRgbFrame, lineByte);

	Out_Video_Imagem = Mat(m_ulHeight, m_ulWidth, CV_8UC3, pRgbFrame).clone();

	delete pFrame;
	delete pRgbFrame;
	return true;
}

int Scanning_Control::Calculate_Scanning_Focal()
{

	int All_Point_Scanning = s_User_Para.Scanning_Num_Y*s_User_Para.Scanning_Num_X;
	int All_Point_Foucuing = m_Focusing_Num_Y*m_Focusing_Num_X;
	long Init_X = -((s_Scanning_System_Para.Space_Slides + s_Scanning_System_Para.Slide_Size_X)
		* s_Scanning_Para.Current_Slide_Num - s_User_Para.Scanning_Aero_X / 2) * 10000;
	long Init_Y = m_Y_Moveto = (s_User_Para.Scanning_Aero_Y / 2) * 10000;
	//通过平均值将超出系统误差范围的值强制转化为平均值
	//这个方法对于平台朝一个方向线性变化大的时候是存在很大错误。
	//double Sum_Focus = 0;
	//double Average_Focus = 0;
	//vector<int>::const_iterator itc = s_Scanning_Para.Focal_Length.begin();
	//int Aero_Temp = 0;
	//while (itc != s_Scanning_Para.Focal_Length.end())
	//{
	//	Sum_Focus = Sum_Focus + *itc;
	//	itc++;
	//}
	//Average_Focus = Sum_Focus / s_Scanning_Para.Focal_Length.size();

	//for (int i = 0; i<s_Scanning_Para.Focal_Length.size();i++)
	//{
	//	if (abs(s_Scanning_Para.Focal_Length[i] - Average_Focus) > abs(Average_Focus))
	//	{
	//		if ((i == 0) || (i == s_Scanning_Para.Focal_Length.size()-1))
	//			s_Scanning_Para.Focal_Length[i] = Average_Focus;
	//		else
	//			s_Scanning_Para.Focal_Length[i] = (s_Scanning_Para.Focal_Length[i - 1] +
	//			s_Scanning_Para.Focal_Length[i + 1] + Average_Focus) / 3;
	//	}

	//}

	//首先求出所有扫描点的X,Y坐标值
	s_Scanning_Para.X_Scanning_Location[0] = Init_X;
	s_Scanning_Para.Y_Scanning_Location[0] = Init_Y;

	for (int i = 1; i < All_Point_Scanning; i++)
	{
	
		s_Scanning_Para.X_Scanning_Location[i] = s_Scanning_Para.X_Scanning_Location[i - 1] + m_Scanning_Step_X;
		s_Scanning_Para.Y_Scanning_Location[i] = s_Scanning_Para.Y_Scanning_Location[i - 1];

		int Num_Y = i / m_Scanning_Num_X;
		int Num_X = i % m_Scanning_Num_X;
		if (Num_X == 0)
		{
			m_Scanning_Step_X = -m_Scanning_Step_X;
			s_Scanning_Para.X_Scanning_Location[i] = s_Scanning_Para.X_Scanning_Location[i - 1];
			s_Scanning_Para.Y_Scanning_Location[i] = s_Scanning_Para.Y_Scanning_Location[i - 1] + m_Scanning_Step_Y;
		}
				
	}
	//获取包围目标点的四个自动聚焦值
	long Pos_1_X, Pos_1_Y, Pos_2_X, Pos_2_Y, Pos_4_X, Pos_4_Y, Pos_3_X, Pos_3_Y;
	for (int j = 0; j < All_Point_Scanning; j++)
	{

		int Coord_k, Coord_m;
		for (int k = 0; k < m_Focusing_Num_X - 1; k++)
		{
			bool Get_Low, Get_High;//在比较扫描点位置处于聚焦范围内时，将编码器的误差范围考虑进去
			Get_Low = s_Scanning_Para.X_Scanning_Location[j] <= s_Scanning_Para.X_Focus_Location[k] + s_Scanning_System_Para.Move_Deviation;
			Get_High = s_Scanning_Para.X_Scanning_Location[j] >= s_Scanning_Para.X_Focus_Location[k + 1] - s_Scanning_System_Para.Move_Deviation;
			if (Get_Low && Get_High)
			{
				Pos_1_X = s_Scanning_Para.X_Focus_Location[k];
				Pos_4_X = s_Scanning_Para.X_Focus_Location[k];
				Pos_2_X = s_Scanning_Para.X_Focus_Location[k + 1];
				Pos_3_X = s_Scanning_Para.X_Focus_Location[k + 1];
				Coord_k = k;
				k = m_Focusing_Num_X;
			}
				
		}

		for (int m = 0; m < m_Focusing_Num_Y - 1; m++)
		{
			bool Get_Low, Get_High;//在比较扫描点位置处于聚焦范围内时，将编码器的误差范围考虑进去
			Get_Low = s_Scanning_Para.Y_Scanning_Location[j] <= s_Scanning_Para.Y_Focus_Location[m * m_Focusing_Num_X] + s_Scanning_System_Para.Move_Deviation;
			Get_High = s_Scanning_Para.Y_Scanning_Location[j] >= s_Scanning_Para.Y_Focus_Location[(m + 1) * m_Focusing_Num_X] - s_Scanning_System_Para.Move_Deviation;
			if (Get_Low && Get_High)
			{
				Pos_1_Y = s_Scanning_Para.Y_Focus_Location[m * m_Focusing_Num_X];
				Pos_2_Y = s_Scanning_Para.Y_Focus_Location[m * m_Focusing_Num_X];
				Pos_4_Y = s_Scanning_Para.Y_Focus_Location[(m + 1) * m_Focusing_Num_X];
				Pos_3_Y = s_Scanning_Para.Y_Focus_Location[(m + 1) * m_Focusing_Num_X];
				Coord_m = m;
				m = m_Focusing_Num_Y;
			}

		}

		long Pos_1_Z, Pos_2_Z, Pos_3_Z, Pos_4_Z;
			if ((Coord_m+1) % 2 == 0)
			{
				Pos_1_Z = s_Scanning_Para.Focal_Length[Coord_m * m_Focusing_Num_X + m_Focusing_Num_X - Coord_k - 1];
				Pos_4_Z = s_Scanning_Para.Focal_Length[(Coord_m + 1) * m_Focusing_Num_X + Coord_k];
				Pos_3_Z = s_Scanning_Para.Focal_Length[(Coord_m + 1) * m_Focusing_Num_X + Coord_k + 1];
				Pos_2_Z = s_Scanning_Para.Focal_Length[Coord_m * m_Focusing_Num_X + m_Focusing_Num_X - Coord_k - 2];
			}
			else
			{
				Pos_4_Z = s_Scanning_Para.Focal_Length[(Coord_m + 1) * m_Focusing_Num_X + m_Focusing_Num_X - Coord_k - 1];
				Pos_1_Z = s_Scanning_Para.Focal_Length[Coord_m * m_Focusing_Num_X + Coord_k];
				Pos_2_Z = s_Scanning_Para.Focal_Length[Coord_m * m_Focusing_Num_X + Coord_k + 1];
				Pos_3_Z = s_Scanning_Para.Focal_Length[(Coord_m + 1) * m_Focusing_Num_X + m_Focusing_Num_X - Coord_k - 2];
			}
/******************以下为三点法测量计算平面******************/
			//通过斜率判断属于那个面
		double A, B, C;
		long pos1_Z, pos2_Z, pos3_Z;
		double K_Pos_1_3 = (double)(Pos_1_Y - Pos_3_Y) / (double)(Pos_1_X - Pos_3_X);//这里要注意不能是0
		long Scanning_Sub_X = s_Scanning_Para.X_Scanning_Location[j] - Pos_1_X;
		double Scanning_Sub_Y = s_Scanning_Para.Y_Scanning_Location[j] - Pos_1_Y;
		if ((Scanning_Sub_X == 0))
		{
			//构成面的点为：	     Pos_1，
			//				Pos_3，  Pos_4
			pos1_Z = Pos_1_Z;
			pos2_Z = Pos_3_Z;
			pos3_Z = Pos_4_Z;
			A = (double)(Pos_4_Y - Pos_1_Y)*(Pos_3_Z - Pos_1_Z) - (double)(Pos_4_Z - Pos_1_Z)*(Pos_3_Y - Pos_1_Y);
			B = (double)(Pos_4_Z - Pos_1_Z)*(Pos_3_X - Pos_1_X) - (double)(Pos_4_X - Pos_1_X)*(Pos_3_Z - Pos_1_Z);
			C = (double)(Pos_4_X - Pos_1_X)*(Pos_3_Y - Pos_1_Y) - (double)(Pos_3_X - Pos_1_X)*(Pos_4_Y - Pos_1_Y);

		}
		else if (K_Pos_1_3 <= (Scanning_Sub_Y / (double)Scanning_Sub_X))
		{
			//构成面的点为：Pos_1，Pos_3，Pos_4
			pos1_Z = Pos_1_Z;
			pos2_Z = Pos_4_Z;
			pos3_Z = Pos_3_Z;
			A = (double)(Pos_4_Y - Pos_1_Y)*(Pos_3_Z - Pos_1_Z) - (double)(Pos_4_Z - Pos_1_Z)*(Pos_3_Y - Pos_1_Y);
			B = (double)(Pos_4_Z - Pos_1_Z)*(Pos_3_X - Pos_1_X) - (double)(Pos_4_X - Pos_1_X)*(Pos_3_Z - Pos_1_Z);
			C = (double)(Pos_4_X - Pos_1_X)*(Pos_3_Y - Pos_1_Y) - (double)(Pos_3_X - Pos_1_X)*(Pos_4_Y - Pos_1_Y);
		}
		else
		{
			//构成面的点为： Pos_2, Pos_1，
			//				 Pos_3
			pos1_Z = Pos_1_Z;
			pos2_Z = Pos_2_Z;
			pos3_Z = Pos_3_Z;
			A = (double)(Pos_2_Y - Pos_1_Y)*(Pos_3_Z - Pos_1_Z) - (double)(Pos_2_Z - Pos_1_Z)*(Pos_3_Y - Pos_1_Y);
			B = (double)(Pos_2_Z - Pos_1_Z)*(Pos_3_X - Pos_1_X) - (double)(Pos_2_X - Pos_1_X)*(Pos_3_Z - Pos_1_Z);
			C = (double)(Pos_2_X - Pos_1_X)*(Pos_3_Y - Pos_1_Y) - (double)(Pos_3_X - Pos_1_X)*(Pos_2_Y - Pos_1_Y);
		}
		// A(x-x1) + B(y-y1) + C(z-z1) = 0
		s_Scanning_Para.Scanning_Length[j] = -(A*(s_Scanning_Para.X_Scanning_Location[j] - Pos_1_X) + 
			B*(s_Scanning_Para.Y_Scanning_Location[j] - Pos_1_Y)) / C + Pos_1_Z;
		
/*******************以下为4点法计算焦距*******************/
			//先求解点1，4；和点2，3的中点坐标
			//double K_1_4, b_1_4, K_2_3, b_2_3;
			//K_1_4 = (double)(Pos_4_Z - Pos_1_Z) / (double)(Pos_4_Y - Pos_1_Y);
			//b_1_4 = ((double)Pos_4_Y * Pos_1_Z - (double)Pos_4_Z * Pos_1_Y) / (double)(Pos_4_Y - Pos_1_Y);
			//double Pos_1_4_Z = K_1_4 * (double)s_Scanning_Para.Y_Scanning_Location[j] + b_1_4;
			//K_2_3 = (double)(Pos_3_Z - Pos_2_Z) / (double)(Pos_3_Y - Pos_2_Y);
			//b_2_3 = ((double)Pos_3_Y * Pos_2_Z - (double)Pos_3_Z * Pos_2_Y) / (double)(Pos_3_Y - Pos_2_Y);
			//double Pos_2_3_Z = K_2_3 * (double)s_Scanning_Para.Y_Scanning_Location[j]+ b_2_3;
			////由两个中点组成的平面，坐标为：（Pos_1_X，Pos_1_4_Z），（Pos_2_X，Pos_2_3_Z）
			//double K_Mid, b_Mid;
			//K_Mid = (double)(Pos_2_3_Z - Pos_1_4_Z) / (double)(Pos_2_X - Pos_1_X);
			//b_Mid = ((double)Pos_2_X * Pos_1_4_Z - (double)Pos_2_3_Z * Pos_1_X) / (double)(Pos_2_X - Pos_1_X);
			//s_Scanning_Para.Scanning_Length[j] = K_Mid * (double)s_Scanning_Para.X_Scanning_Location[j] + b_Mid; 

	}

	return 1;

}

bool Scanning_Control::Save_CVimg(MULTICAPTURE_PARAMS Capture_Save_Paras, Mat Img)
{
	CString csFilename;
	CString csIndex;
	CString csSlide_Index;
	int iSize;
	int i;
	int iLen;

	critical_section.Lock();//开启线程锁

	// Convert and save image 
	csIndex.Format(_T("%d"), s_Scanning_Para.Current_Point_Num + 1);
	iLen = 6 - csIndex.GetLength();
	for (; iLen > 0; iLen--)
	{
		csIndex.Insert(0, '0');
	}
	csSlide_Index.Format((_T("%d")), s_Scanning_Para.Current_Slide_Num + 1);
	iLen = 3 - csSlide_Index.GetLength();
	for (; iLen > 0; iLen--)
	{
		csSlide_Index.Insert(0, '0');
	}
	if (Pass_TakeImg == true)
	{
		csIndex = _T("Pass") + csIndex;
	}
	csFilename.Format(_T("%s\\%s%s%s%s"), Capture_Save_Paras.csDiretory, Capture_Save_Paras.csFilename, csSlide_Index, csIndex, Capture_Save_Paras.csExtension);
	string strStr(CT2A(csFilename.GetBuffer()));
	imwrite(strStr, Img);
	csFilename.ReleaseBuffer();
	critical_section.Unlock();//关闭线程锁
	return true;
}
bool Scanning_Control::TakeVeido_And_Save_Image_Raw(MULTICAPTURE_PARAMS Capture_Save_Paras,bool push2Vector)//抓图保存图像
{

	//PMULTICAPTURE_RESULTS pmrResults = NULL;
	//LONGLONG dwCurrentTick;
	//LONGLONG dwStartTick = GetTickCount64();
	DWORD dwError = LucamNoError;
	DWORD dwCheck;
	DOUBLE dWaitTime;
	BYTE *pbRawFrame = NULL;
	BYTE *pbRgbFrame = NULL;
	CString csFilename;
	CString csIndex;
	CString csSlide_Index;
	int iSize;
	int i;
	int iLen;

		// Allocate image buffers
		iSize = (m_lffFormat.width / m_lffFormat.subSampleX) * (m_lffFormat.height / m_lffFormat.subSampleY);
		if (m_lffFormat.pixelFormat == LUCAM_PF_16) iSize *= 2;
		pbRawFrame = new BYTE[iSize];
		pbRgbFrame = new BYTE[iSize * 3];

		if ((pbRawFrame != NULL) && (pbRgbFrame != NULL))
		{

				critical_section.Lock();//开启线程锁

				// capture image
				if (!LucamTakeVideo(m_hCamera, 1, pbRawFrame))
				{
					dwError = LucamGetLastErrorForCamera(m_hCamera);
				}

				// Convert and save image 
				csIndex.Format(_T("%d"), s_Scanning_Para.Current_Point_Num+1);
				iLen = 6 - csIndex.GetLength();
				for (; iLen > 0; iLen--)
				{
					csIndex.Insert(0, '0');
				}
				csSlide_Index.Format((_T("%d")), s_Scanning_Para.Current_Slide_Num+1);
				iLen = 3 - csSlide_Index.GetLength();
				for (; iLen > 0; iLen--)
				{
					csSlide_Index.Insert(0, '0');
				}
				if (Pass_TakeImg == true)
				{
					csIndex = _T("Pass") + csIndex;
				}
				csFilename.Format(_T("%s\\%s%s%s%s"), Capture_Save_Paras.csDiretory, Capture_Save_Paras.csFilename, csSlide_Index, csIndex, Capture_Save_Paras.csExtension);

				if (Capture_Save_Paras.csExtension != ".raw")
				{
					if (m_lffFormat.pixelFormat == LUCAM_PF_16)
					{
						if (!LucamConvertFrameToRgb48(m_hCamera, (USHORT *)pbRgbFrame, (USHORT *)pbRawFrame, (m_lffFormat.width / m_lffFormat.subSampleX), (m_lffFormat.height / m_lffFormat.subSampleY), m_lffFormat.pixelFormat, &Capture_Save_Paras.lcConversion))
						{
							dwError = LucamGetLastErrorForCamera(m_hCamera);

						}
						if (!LucamSaveImageEx(m_hCamera, (m_lffFormat.width / m_lffFormat.subSampleX), (m_lffFormat.height / m_lffFormat.subSampleY), LUCAM_PF_48, pbRgbFrame, CT2A(csFilename)))
						{
							dwError = LucamGetLastErrorForCamera(m_hCamera);
								
						}
					}
					else
					{
						if (!LucamConvertFrameToRgb24(m_hCamera, pbRgbFrame, pbRawFrame, (m_lffFormat.width / m_lffFormat.subSampleX), (m_lffFormat.height / m_lffFormat.subSampleY), m_lffFormat.pixelFormat, &Capture_Save_Paras.lcConversion))
						{
							dwError = LucamGetLastErrorForCamera(m_hCamera);
								
						}

						if (!LucamSaveImageEx(m_hCamera, (m_lffFormat.width / m_lffFormat.subSampleX), (m_lffFormat.height / m_lffFormat.subSampleY), LUCAM_PF_24, pbRgbFrame, CT2A(csFilename)))
						{
							dwError = LucamGetLastErrorForCamera(m_hCamera);
								
						}
					}
				}
				else
				{
					if (m_lffFormat.pixelFormat == LUCAM_PF_16)
					{
						if (!LucamSaveImageEx(m_hCamera, (m_lffFormat.width / m_lffFormat.subSampleX), (m_lffFormat.height / m_lffFormat.subSampleY), LUCAM_PF_16, pbRawFrame, CT2A(csFilename)))
						{
							dwError = LucamGetLastErrorForCamera(m_hCamera);
								
						}

					}
					else
					{
						if (!LucamSaveImageEx(m_hCamera, (m_lffFormat.width / m_lffFormat.subSampleX), (m_lffFormat.height / m_lffFormat.subSampleY), LUCAM_PF_8, pbRawFrame, CT2A(csFilename)))
						{
							dwError = LucamGetLastErrorForCamera(m_hCamera);
								
						}
					}
				}

				critical_section.Unlock();//关闭线程锁
		}

		////if (push2Vector)//表示将图压入Vector中，另外的线程直接处理
		////{
		////	if (Capture_Save_Paras.csExtension == ".raw")
		////	{
		////		if (m_lffFormat.pixelFormat == LUCAM_PF_16)
		////		{
		////			if (!LucamConvertFrameToRgb48(m_hCamera, (USHORT *)pbRgbFrame, (USHORT *)pbRawFrame, (m_lffFormat.width / m_lffFormat.subSampleX), (m_lffFormat.height / m_lffFormat.subSampleY), m_lffFormat.pixelFormat, &Capture_Save_Paras.lcConversion))
		////			{
		////				dwError = LucamGetLastErrorForCamera(m_hCamera);
		////			}
		////		}
		////		else
		////		{
		////			if (!LucamConvertFrameToRgb24(m_hCamera, pbRgbFrame, pbRawFrame, (m_lffFormat.width / m_lffFormat.subSampleX), (m_lffFormat.height / m_lffFormat.subSampleY), m_lffFormat.pixelFormat, &Capture_Save_Paras.lcConversion))
		////			{
		////				dwError = LucamGetLastErrorForCamera(m_hCamera);

		////			}
		////		}
		////	}

		////	int lineByte = 0;
		////	lineByte = (m_ulWidth * 3 + 3) / 4 * 4; //格式宽度  
		////	//cv::Mat iMat(m_ulHeight, m_ulWidth, CV_8UC3, pRgbFrame, lineByte);
		////	Mat img = Mat(m_ulHeight, m_ulWidth, CV_8UC3, pbRgbFrame).clone();
		////	double scale = 0.2;
		////	Size outsize;
		////	Mat Resize;
		////	outsize.height = img.rows * scale;
		////	outsize.width = img.cols * scale;
		////	resize(img, Resize, outsize, 0, 0, INTER_LINEAR);
		////	Scanning_10X_img.insert(Scanning_10X_img.begin(),Resize);//在Vector的头部插入数据

		////}
		delete pbRawFrame;
		delete pbRgbFrame;
		pbRawFrame = NULL;
		pbRgbFrame = NULL;
	/*
	Mat Focus_Pic;
	Take_Video_Image(Focus_Pic);
	long height;
	bool Get_Pos_Move = s_Dev_Drivers->Dev_Read_Pos(3, height);
	string zpos = to_string(height);
	string p_Num = to_string(s_Scanning_Para.Current_Point_Num);
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);    //png格式下，默认的参数为3.
	string p_road = "C:\\Users\\wench\\Desktop\\sacnning\\" + p_Num + "-" + " " + zpos + "+" + ".bmp";
	imwrite(p_road, Focus_Pic);
	*/
	return true;
		
}
bool Scanning_Control::Save_To_Database()//保存当前的数据到数据库中
{


	return true;
}

bool Scanning_Control::Take_Video_Image(Mat &m_Luaptured_Imagem_C)
{
	BYTE *pFrame;
	BYTE *pRgbFrame;
	INT iSize;
	LUCAM_CONVERSION lcConversion;
	LUCAM_IMAGE_FORMAT lifFormat;
	double StartTime = GetTickCount();
	// Allocate memory to store frames
	iSize = (m_lffFormat.width * m_lffFormat.height) / (m_lffFormat.subSampleX * m_lffFormat.subSampleY);
	if (m_lffFormat.pixelFormat == LUCAM_PF_16)
		iSize *= 2;
	pFrame = new BYTE[iSize];
	if (pFrame == NULL)
	{
		AfxMessageBox(_T("Unable to allocate memory to capture image."));
		return false;
	}
	pRgbFrame = new BYTE[iSize * 3];
	if (pRgbFrame == NULL)
	{
		delete pFrame;
		AfxMessageBox(_T("Unable to allocate memory to capture image."));
		return false;
	}

	if (!LucamTakeVideo(m_hCamera, 1, pFrame))
	{
		delete pFrame;
		delete pRgbFrame;
		AfxMessageBox(_T("Failed to capture image."));
		return false;
	}

	if (!m_bIsColor)
	{
		// Get image format
		lifFormat.Size = sizeof(LUCAM_IMAGE_FORMAT);
		if (LucamGetVideoImageFormat(m_hCamera, &lifFormat))
		{
			// Correct for mono checkerboard pattern (should not be necessary for most cameras)
			if (!LucamPerformMonoGridCorrection(m_hCamera, pFrame, &lifFormat))
			{
				AfxMessageBox(_T("Failed to perform mono grid correction."));
			}
		}
		else
		{

			AfxMessageBox(_T("Failed to perform mono grid correction."));
		}

	}
	double endletime = GetTickCount();
	double pppp = StartTime - endletime;
	StartTime = GetTickCount();
	// Convert to RGB
	lcConversion.CorrectionMatrix = LUCAM_CM_HALOGEN;
	lcConversion.DemosaicMethod = LUCAM_DM_FAST;

	m_ulWidth = m_lffFormat.width / m_lffFormat.subSampleX;
	m_ulHeight = m_lffFormat.height / m_lffFormat.subSampleY;

	if (m_lffFormat.pixelFormat == LUCAM_PF_16)
	{
		if (!LucamConvertFrameToRgb48(m_hCamera, (USHORT *)pRgbFrame, (USHORT *)pFrame, m_ulWidth, m_ulHeight, m_lffFormat.pixelFormat, &lcConversion))
		{
			delete pFrame;
			delete pRgbFrame;
			AfxMessageBox(_T("Could not convert snapshot to rgb."));
			return false;
		}
	}
	else
	{
		if (!LucamConvertFrameToRgb24(m_hCamera, pRgbFrame, pFrame, m_ulWidth, m_ulHeight, m_lffFormat.pixelFormat, &lcConversion))
		{
			delete pFrame;
			delete pRgbFrame;
			AfxMessageBox(_T("Could not convert snapshot to rgb."));
			return false;
		}

	}

	endletime = GetTickCount();
	pppp = StartTime - endletime;

	int lineByte = 0;
	lineByte = (m_ulWidth * 3 + 3) / 4 * 4; //格式宽度  

	//cv::Mat iMat(m_ulHeight, m_ulWidth, CV_8UC3, pRgbFrame, lineByte);

	m_Luaptured_Imagem_C = Mat(m_ulHeight, m_ulWidth, CV_8UC3, pRgbFrame).clone();

	delete pFrame;
	delete pRgbFrame;
	return true;
}
/*
FilePath:文件夹路径
FileName：文件名称
RGB_Image：获取并转化为RGB图像
pixelFormat：图像深度，LUCAM_PF_16 或 LUCAM_PF_8（默认）
Convert_Demosaic：转化图像的质量：默认为：2（LUCAM_DM_HIGHER_QUALITY）
Convert_CorrectionMatrix：图像拍摄的光源类型，默认为：3（LUCAM_CM_HALOGEN;//卤素灯）
Image_Width：图像宽度，2048
Image_Height：图形高度，2048
*/
bool Scanning_Control::Read_Raw_And_ConvertToRGB(CString FilePath, CString FileName, Mat &RGB_Image, 
	int pixelFormat, int Convert_Demosaic, int Convert_CorrectionMatrix, int Image_Width, int Image_Height)//本地读取Raw文件，并转化为RGB
{
	CString Whole_Path;
	Whole_Path.Format(_T("%s\\%s"), FilePath, FileName);
	LUCAM_CONVERSION lcConversion;

	switch (Convert_Demosaic)
	{
	case 0:
		lcConversion.DemosaicMethod = LUCAM_DM_FAST;
		break;
	case 1:
		lcConversion.DemosaicMethod = LUCAM_DM_HIGH_QUALITY;
		break;
	case 2:
	default:
		lcConversion.DemosaicMethod = LUCAM_DM_HIGHER_QUALITY;
		break;
	case 3:
		lcConversion.DemosaicMethod = LUCAM_DM_NONE;
		break;
	case 4:
		lcConversion.DemosaicMethod = LUCAM_DM_SIMPLE;
		break;
	}

	switch (Convert_CorrectionMatrix)
	{
	case 0:
		lcConversion.CorrectionMatrix = LUCAM_CM_CUSTOM;
		break;
	case 1:
		lcConversion.CorrectionMatrix = LUCAM_CM_DAYLIGHT;
		break;
	case 2:
	default:
		lcConversion.CorrectionMatrix = LUCAM_CM_FLUORESCENT;//荧光灯
		break;
	case 3:
		lcConversion.CorrectionMatrix = LUCAM_CM_HALOGEN;//卤素灯
		break;
	case 4:
		lcConversion.CorrectionMatrix = LUCAM_CM_IDENTITY;//标识灯
		break;
	case 5:
		lcConversion.CorrectionMatrix = LUCAM_CM_INCANDESCENT;//白炽灯
		break;
	case 6:
		lcConversion.CorrectionMatrix = LUCAM_CM_LED;
		break;
	case 7:
		lcConversion.CorrectionMatrix = LUCAM_CM_NONE;
		break;
	case 8:
		lcConversion.CorrectionMatrix = LUCAM_CM_XENON_FLASH;
		break;
	}
	CFile file;
	BYTE *pFrame;
	BYTE *pRgbFrame;
	long iSize = Image_Width * Image_Height;
	file.Open(Whole_Path, CFile::modeRead | CFile::typeBinary);

	file.SeekToBegin();

	pFrame = new BYTE[iSize];//相机采图分辨率
	pRgbFrame = new BYTE[iSize * 3];
	if (iSize != file.Read(pFrame, iSize))
	{
		//提示文件读取错误  
		AfxMessageBox(_T("Could not open the File."));
		file.Close();
		return false;
	}
	file.Close();
	//m_lffFormat.pixelFormat == LUCAM_PF_16
	if (!LucamConvertFrameToRgb24(m_hCamera, pRgbFrame, pFrame, Image_Width, Image_Height, pixelFormat, &lcConversion))
	{
		delete pFrame;
		delete pRgbFrame;
		AfxMessageBox(_T("Could not convert snapshot to rgb."));
		return false;
	}
	
	int lineByte = 0;
	lineByte = (Image_Width * 3 + 3) / 4 * 4; //格式宽度  

	//cv::Mat iMat(m_ulHeight, m_ulWidth, CV_8UC3, pRgbFrame, lineByte);

	RGB_Image = Mat(Image_Width, Image_Height, CV_8UC3, pRgbFrame).clone();
	/*
	namedWindow("zhuatu", WINDOW_NORMAL);
	cv::imshow("zhuatu", RGB_Image);
	waitKey(5000);
	cv::destroyWindow("zhuatu");
	*/
	delete pFrame;
	delete pRgbFrame;
	return true;

}
/*
Do_CB_Scanning():微核图像扫描时的函数
*/
int Scanning_Control::Do_CB_Scanning()//微核图像扫描
{
		
	if (m_Slide_ID.empty())
	{
		AfxMessageBox(_T("请设置扫描的拨片编号！"));
		return -1;
	}

	//s_User_Para.Slide_Max_Num = m_Slide_ID.size();
	//for (int i = 0; i < s_User_Para.Slide_Max_Num; i++)
	for (vector<int>::iterator it = m_Slide_ID.begin(); it != m_Slide_ID.end(); it++)
	{
		s_Scanning_Para.Current_Slide_Num = *it;
		s_Scanning_Para.Forcuing_OR_Scanning = true;
		Do_Focusing_Model(s_Scanning_System_Para.Focusing_Step_Z[2]);
		s_Scanning_Para.Forcuing_OR_Scanning = false;
		Do_Scanning_Model();
	}
	return 0;

}

UINT Scanning_Control::Handlehold_ThreadFunc(LPVOID Handlehold_ThreadArg)//LPVOID Handlehold_ThreadArg
{


	s_Dev_Drivers->Dev_Reset_Communication();
	Scanning_Control* Scanning_Control_Handlehold = (Scanning_Control*)Handlehold_ThreadArg;

	//Scanning_Control_Handlehold->Handlehold_Thread_Running = false;
	
	long X_real_Analog_Old, Y_real_Analog_Old, Z_real_Analog_Old;
	long X_real_Analog_New, Y_real_Analog_New, Z_real_Analog_New;
	long real_Analog;
	bool X_Moving = false;
	bool Y_Moving = false;
	bool Z_Moving = false;

	int i = 0;
	int Get_Right_Pos_Num = 0;
	Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Read_Analog_Input1(1, X_real_Analog_Old);
	while (i++ < 4)
	{
		Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Read_Analog_Input1(1, X_real_Analog_New);
		if (abs(X_real_Analog_New - X_real_Analog_Old) < Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Error_Range)
		{
			X_real_Analog_New = X_real_Analog_Old;
			Get_Right_Pos_Num++;
		}
	}
	if ((Get_Right_Pos_Num>2) &&
		(abs(X_real_Analog_New - Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Zero_X) <
		Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Error_Range))
	{
		Get_Right_Pos_Num = 0;
		i = 0;
		Scanning_Control_Handlehold->s_Zero_Pos_X = X_real_Analog_New;
	}
	else
	{
		CString STemp;
		STemp.Format(_T("%d"), X_real_Analog_New);
		STemp = _T("手动摇杆X轴故障！ X_Now = ") + STemp;
		AfxMessageBox(STemp);
		Scanning_Control_Handlehold->Handlehold_Thread_Running = false;
		return 0;
	}
	Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Read_Analog_Input1(2, Y_real_Analog_Old);
	while (i++ < 4)
	{
		Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Read_Analog_Input1(2, Y_real_Analog_New);
		if (abs(Y_real_Analog_New - Y_real_Analog_Old) < Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Error_Range)
		{
			Y_real_Analog_New = Y_real_Analog_Old;
			Get_Right_Pos_Num++;
		}
	}
	if ((Get_Right_Pos_Num>2) &&
		(abs(Y_real_Analog_New - Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Zero_Y) <
		Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Error_Range))
	{
		Get_Right_Pos_Num = 0;
		i = 0;
		Scanning_Control_Handlehold->s_Zero_Pos_Y = Y_real_Analog_New;
	}
	else
	{
		CString STemp;
		STemp.Format(_T("%d"), Y_real_Analog_New);
		STemp = _T("手动摇杆Y轴故障！ Y_Now = ") + STemp;
		AfxMessageBox(STemp);
		Scanning_Control_Handlehold->Handlehold_Thread_Running = false;
		return 0;
	}
	Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Read_Analog_Input1(3, Z_real_Analog_Old);
	while (i++ < 4)
	{
		Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Read_Analog_Input1(3, Z_real_Analog_New);
		if (abs(Z_real_Analog_New - Z_real_Analog_Old) < Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Error_Range)
		{
			Z_real_Analog_New = Z_real_Analog_Old;
			Get_Right_Pos_Num++;
		}
	}
	if ((Get_Right_Pos_Num>2) &&
		(abs(Z_real_Analog_New - Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Zero_Z) <
		Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Error_Range))
	{
		Get_Right_Pos_Num = 0;
		i = 0;
		Scanning_Control_Handlehold->s_Zero_Pos_Z = Z_real_Analog_New;
	}
	else
	{
		CString STemp;
		STemp.Format(_T("%d"), Z_real_Analog_New);
		STemp = _T("手动摇杆Z轴故障！ Z_Now = ") + STemp;
		AfxMessageBox(STemp);
		Scanning_Control_Handlehold->Handlehold_Thread_Running = false;
		return 0;
	}

	Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Single_Mov_Para(1, 2);
	Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Single_Mov_Para(2, 2);
	Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Single_Mov_Para(3, 2);

	while (Scanning_Control_Handlehold->Handlehold_Thread_Running)
	{
		Sleep(10);

		//if (!Scanning_Control_Handlehold->Handlehold_Thread_Running)
		//	continue;

		bool X_Get = Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Read_Analog_Input1(1, X_real_Analog_New);
		bool Y_Get = Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Read_Analog_Input1(2, Y_real_Analog_New);
		bool Z_Get = Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Read_Analog_Input1(3, Z_real_Analog_New);
		if (Z_Get && 
			(abs(Z_real_Analog_New - Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Zero_Z) >
		Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Error_Range))
		{
			//这里4000为中间距离最极端的最大数值差，2为最大速度,最大的偏移量对应的模拟量值为16380
			//double V_Model_Speed = (double)(Z_real_Analog_New - 12244) / 4000 * 1.5;
			int Z_Dic = 1;
			if (Z_real_Analog_New > Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Zero_Z)
				Z_Dic = -1;
			double V_Model_Speed = 0;
			if (abs(Z_real_Analog_New - Scanning_Control_Handlehold->s_Zero_Pos_Z)>3200)
			{
				V_Model_Speed = Z_Dic * 10;
			}
			else if (abs(Z_real_Analog_New - Scanning_Control_Handlehold->s_Zero_Pos_Z)<400)
				V_Model_Speed = Z_Dic * 0.005;
			else
				V_Model_Speed = Z_Dic*pow((double)(Z_real_Analog_New - 12244), 2) * 0.0000001169;
			Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(3, Velocity_Model_START, V_Model_Speed, !Z_Moving);
			Z_Moving = true;

		}
		else if (Z_Moving)
		{
			Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(3, Velocity_Model_STOP);
			Z_Moving = false;
		}
				

		bool X_Moved = abs(X_real_Analog_New - Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Zero_X) >
			Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Error_Range;
		bool Y_Moved = abs(Y_real_Analog_New - Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Zero_Y) >
			Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Error_Range;
		//1度=0.01744，tan(10)=0.176; tan(80)=5.636
		//这里设计的X,Y方向的容错率为10度，tan（）=x/y；
		int X_Dic = 1;
		int Y_Dic = 1;
		if (X_real_Analog_New < Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Zero_X)
			X_Dic = -1;
		if (Y_real_Analog_New > Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Zero_Y)
			Y_Dic = -1;
		double X_V_Model_Speed = 0;
		double Y_V_Model_Speed = 0;
		if (abs(X_real_Analog_New - Scanning_Control_Handlehold->s_Zero_Pos_X)>3200)
		{
			X_V_Model_Speed = X_Dic * 10;
		}
		else
			X_V_Model_Speed = X_Dic * pow((double)(X_real_Analog_New - Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Zero_X), 2) 
			* 0.00000031;//4000 * 5
		if (abs(Y_real_Analog_New - Scanning_Control_Handlehold->s_Zero_Pos_Y)>3200)
		{
			Y_V_Model_Speed = Y_Dic * 10;
		}
		else
			Y_V_Model_Speed = Y_Dic * pow((double)(Y_real_Analog_New - Scanning_Control_Handlehold->s_Scanning_System_Para.Handle_Zero_Y), 2)
			* 0.00000031;
		if (Y_Get && X_Get)
		{
			if (X_Moved && Y_Moved)
			{
				if (0.176 < (abs(X_V_Model_Speed) / abs(Y_V_Model_Speed)) < 5.636)
				{
						Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(1, Velocity_Model_START, X_V_Model_Speed, !X_Moving);
						X_Moving = true;

						Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(2, Velocity_Model_START, Y_V_Model_Speed, !Y_Moving);
						Y_Moving = true;
						
				}
				else if (0.176 >= (abs(X_V_Model_Speed) / abs(Y_V_Model_Speed)))
				{
						Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(2, Velocity_Model_START, Y_V_Model_Speed, !Y_Moving);
						Y_Moving = true;

				}
				else
				{
						Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(1, Velocity_Model_START, X_V_Model_Speed, !X_Moving);
						X_Moving = true;

				}
						
			}
			else if (X_Moved)
			{

				Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(1, Velocity_Model_START, X_V_Model_Speed, !X_Moving);
				X_Moving = true;

				if (Y_Moving)
				{
					Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(2, Velocity_Model_STOP);
					Y_Moving = false;
				}
			}
			else if (Y_Moved)
			{
				Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(2, Velocity_Model_START, Y_V_Model_Speed, !Y_Moving);
				Y_Moving = true;
				if (X_Moving)
				{
					Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(1, Velocity_Model_STOP);
					X_Moving = false;
				}
			}
			else
			{
				if (X_Moving)
				{
					Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(1, Velocity_Model_STOP);
					X_Moving = false;
				}
				if (Y_Moving)
				{
					Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(2, Velocity_Model_STOP);
					Y_Moving = false;
				}

			}
				
		}
		else if (X_Get)
		{
			if (X_Moved)
			{
				if (!X_Moving)
				{
					Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(1, Velocity_Model_START, X_V_Model_Speed);
					X_Moving = true;
				}
					
			}
			else if (X_Moving)
			{
				Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(1, Velocity_Model_STOP);
				X_Moving = false;
			}
					
		}
		else if (Y_Get)
		{
			if (Y_Moved)
			{
				if (!Y_Moving)
				{
					Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(2, Velocity_Model_START, Y_V_Model_Speed);
					Y_Moving = true;
				}

			}
			else if (Y_Moving)
			{
				Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(2, Velocity_Model_STOP);
				Y_Moving = false;
			}
					
		}

	}
	Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(1, Velocity_Model_STOP);
	Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(2, Velocity_Model_STOP);
	Scanning_Control_Handlehold->s_Dev_Drivers->Dev_Velocity_Model(3, Velocity_Model_STOP);
	//delete Scanning_Control_Handlehold;

	return 0;
}

vector < vector<Point> > Scanning_Control::Get_CA_Pos_10X(Mat Intput_Image, vector<vector<Point>> contours,
	vector<unsigned int> &ROI_Size, bool Masked_Img)
{
	//3.遍历轮廓，找出正外接矩形
	Mat mask = Mat::zeros(Intput_Image.size(), CV_8U);//同原图一样大小，纯黑的mask图像//CV_8UC3
	Mat draw = Mat::zeros(Intput_Image.size(), CV_8U);//将原图copyTo到draw上，加上mask操作
	Mat temp = Mat::zeros(Intput_Image.size(), CV_8U);//每次循环，重置mask和draw
	vector < vector<Point> >Right_contours;
	Rect boundRect;//存储轮廓的正外接矩形

	for (int i = 0; i < contours.size(); i++)//遍历每个轮廓
	{
		//Scalar color((rand() & 255), (rand() & 255), (rand() & 255));//随机产生颜色 rand()产生随机数
		Mat ROI;
		boundRect = boundingRect(Mat(contours[i]));//获得第i个轮廓的正外接矩形
		//Mat jhjh=Mat::zeros(Intput_Image.size(), CV_8U);;
		//drawContours(jhjh, contours, i, Scalar(255), -1);//在mask上用白色绘制轮廓，-1向内填充
		//namedWindow("jhjh", WINDOW_NORMAL);
		//imshow("jhjh", jhjh);
		//waitKey(0);

		unsigned int contour_Aero = contourArea(contours[i]);
		ROI_Size.push_back(contour_Aero);
		if ((boundRect.width>Intput_Image.cols / 2) || (boundRect.height > Intput_Image.rows / 2))
			continue;
		if (contour_Aero>s_Scanning_10X_Para.Min_CA_Aero_10X)
			Right_contours.push_back(contours[i]);

		if (Masked_Img)
		{
			temp.copyTo(mask);//重置mask
			temp.copyTo(draw);//重置draw
			drawContours(mask, contours, i, Scalar(255), -1);//在mask上用白色绘制轮廓，-1向内填充
			namedWindow("mask", WINDOW_NORMAL);
			imshow("mask", mask);
			Intput_Image.copyTo(draw, mask);//将原图copyTo到draw上，加上mask操作

			ROI = draw(boundRect);//定义ROI，即为draw上的boundRect区域

			waitKey(0);
		}
		else
		{
			ROI = Intput_Image(boundRect);

		}

		//namedWindow("srcImg_ROI", WINDOW_NORMAL);
		//imshow("srcImg_ROI", ROI);
		////waitKey(0);

	}

	return Right_contours;
}

Mat Scanning_Control::Read_Img_From_Dish(int Current_Index, int Current_Solide_Num, MULTICAPTURE_PARAMS Img_Dish_Paras)
{
	CString csFilename;
	CString csIndex;
	CString csSlide_Index;
	int iLen;
	csIndex.Format(_T("%d"), Current_Index);
	iLen = 6 - csIndex.GetLength();
	for (; iLen > 0; iLen--)
	{
		csIndex.Insert(0, '0');
	}
	csSlide_Index.Format((_T("%d")), Current_Solide_Num);
	iLen = 3 - csSlide_Index.GetLength();
	for (; iLen > 0; iLen--)
	{
		csSlide_Index.Insert(0, '0');
	}
	csFilename.Format(_T("%s\\%s%s%s%s"), Img_Dish_Paras.csDiretory,
		Img_Dish_Paras.csFilename,
		csSlide_Index, csIndex, Img_Dish_Paras.csExtension);
	//将CString 转化为 string
	DWORD  num = WideCharToMultiByte(CP_ACP, 0, csFilename, -1, NULL, 0, NULL, 0);
	char *cword;
	cword = (char*)calloc(num, sizeof(char));
	if (cword == NULL)
	{
		free(cword);
	}
	memset(cword, 0, num*sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, csFilename, -1, cword, num, NULL, 0);
	string strFilename = cword;
	free(cword);
	//读取图像
	return imread(strFilename);

}

UINT Scanning_Control::Get_CA_Pos_10X_ThreadFunc(LPVOID CA_10X_ThreadArg)
{
	Scanning_Control Get_CA_PosThreadFunc = *(Scanning_Control *)CA_10X_ThreadArg;

	Get_CA_PosThreadFunc.s_Scanning_Para.CA_Pos_10X.clear();//首先清除存放CA位置的向量空间

	int Focus_CA_10X_Num = 1;
	int Focus_CA_10X_WholeNum = Get_CA_PosThreadFunc.s_User_Para.Scanning_Num_X *
								Get_CA_PosThreadFunc.s_User_Para.Scanning_Num_Y;
	while (Focus_CA_10X_Num < Focus_CA_10X_WholeNum+1)
	{

		if (!CA_Focus_Thread_Running)
			break;
		if (Focus_CA_10X_Num > Get_CA_PosThreadFunc.s_Scanning_Para.Current_Point_Num+1)
			continue;
		////***** 以下为从本地磁盘读取对应的扫描文件 *****///
		//锁住线程
		//critical_section.Lock();
		Mat Img = Get_CA_PosThreadFunc.Read_Img_From_Dish(Focus_CA_10X_Num, Get_CA_PosThreadFunc.s_Scanning_Para.Current_Slide_Num+1,
			Get_CA_PosThreadFunc.m_ScanningImage_Save_Paras);
		//critical_section.Unlock();

		if (Img.empty())
			continue;
		double scale = 0.2;
		Size outsize;
		Mat Resize;
		outsize.height = Img.rows * scale;
		outsize.width = Img.cols * scale;
		resize(Img, Resize, outsize, 0, 0, INTER_LINEAR);
		Mat gray,Bi;
		cvtColor(Resize, gray, COLOR_RGB2GRAY);

		///***** 以下为扫描时，直接将抓取的图像压入到Scanning_10X_img的这个vector中，本线程直接读取 *****///
		////if (Get_CA_PosThreadFunc.Scanning_10X_img.empty())
		////	continue;
		////Mat Resize = *(Get_CA_PosThreadFunc.Scanning_10X_img.end()-1);
		////Get_CA_PosThreadFunc.Scanning_10X_img.pop_back();
		////Mat gray,Bi;
		////cvtColor(Resize, gray, COLOR_RGB2GRAY);

		clock_t start_c = clock();
		clock_t Stop_c = clock();


		int Otsu_Threshold = threshold(gray, Bi, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
		//**注意，如果或的二值图是白底黑目标区域的，就必须将图边界增加几个像素，否则后面的轮廓提取会把整体图片的边界也
		//作为是一个轮廓，这个轮廓会包含于边界相连通的所有区域***
		//copyMakeBorder(Bi, Bi, 2, 2, 2, 2, BORDER_CONSTANT, Scalar(0));
		Mat Bi_contour = Bi.clone();
		Mat Biii = Bi.clone();
		Mat erode_element = getStructuringElement(MORPH_ELLIPSE, Size(
			Get_CA_PosThreadFunc.s_Scanning_10X_Para.Erode_element, Get_CA_PosThreadFunc.s_Scanning_10X_Para.Erode_element));
		//腐蚀，将染色体去除，留下非染色体区域
		erode(Biii, Biii, erode_element);
		for (int i = 0; i<Biii.rows - 1; i++)
		{
			for (int j = 0; j<Biii.cols - 1; j++)
			{
				if (Biii.at<uchar>(i, j) == 255)
				{
					Rect ccomp;
					floodFill(Biii, Point(j, i), Scalar(0), &ccomp, Scalar(1), Scalar(1), 8);
					//如果是白底黑区域，则Biii.at<uchar>(i, j) == 0， Scalar(255)
					floodFill(Bi, Point(j, i), Scalar(0), &ccomp, Scalar(1), Scalar(1), 8);
				}
			}

		}

		Mat dilate_element = getStructuringElement(MORPH_ELLIPSE, Size(
			Get_CA_PosThreadFunc.s_Scanning_10X_Para.Dilate_element, Get_CA_PosThreadFunc.s_Scanning_10X_Para.Dilate_element));
		dilate(Bi, Bi, dilate_element);
		Mat erodedilate_element = getStructuringElement(MORPH_ELLIPSE, Size(
			Get_CA_PosThreadFunc.s_Scanning_10X_Para.ErodeDilate_element, Get_CA_PosThreadFunc.s_Scanning_10X_Para.ErodeDilate_element));
		erode(Bi, Bi, erodedilate_element);
		dilate(Bi, Bi, erodedilate_element);
		//查找轮廓
		vector<unsigned int> ROI_Size;
		vector < vector<Point> > contours;
		vector < vector<Point> >Right_contours;
		vector < vector<Point> >temp_contours;
		findContours(Bi, contours, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));//CV_RETR_CCOMP
		Rect boundRect;//存储轮廓的正外接矩形
		Point2f center; float radius;
		Mat mask_contour = Mat::zeros(Bi.size(), CV_8U);//同原图一样大小，纯黑的mask图像//CV_8UC3
		Mat draw_contour = Mat::zeros(Bi.size(), CV_8U);//将原图copyTo到draw上，加上mask操作
		Mat temp = Mat::zeros(Bi_contour.size(), CV_8U);//每次循环，重置mask和draw
		for (int i = 0; i < contours.size(); i++)//遍历每个轮廓
		{
			//Scalar color((rand() & 255), (rand() & 255), (rand() & 255));//随机产生颜色 rand()产生随机数
			Mat ROI;
			//boundRect = boundingRect(Mat(contours[i]));//获得第i个轮廓的正外接矩形
			minEnclosingCircle(Mat(contours[i]), center, radius);
			unsigned int contour_Aero = contourArea(contours[i]);
			ROI_Size.push_back(contour_Aero);
			if ((boundRect.width > Bi.cols / 2) || (boundRect.height > Bi.rows / 2))
				continue;
			//去除面积小的区域
			if ((contour_Aero < Get_CA_PosThreadFunc.s_Scanning_10X_Para.Min_CA_Aero_10X)
				|| (contour_Aero >2000))
				continue;

			temp.copyTo(mask_contour);//重置mask
			temp.copyTo(draw_contour);//重置draw
			drawContours(mask_contour, contours, i, Scalar(255), -1);//在mask上用白色绘制轮廓，-1向内填充

			Bi_contour.copyTo(draw_contour, mask_contour);//将原图copyTo到draw上，加上mask操作
			findContours(draw_contour, temp_contours, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));//CV_RETR_CCOMP
			//满足总体面积，区域内检测的染色体调试大于5，得到最终的染色体区域
			if (temp_contours.size()>Get_CA_PosThreadFunc.s_Scanning_10X_Para.Min_CA_Num_10X)
			{
				Right_contours.push_back(contours[i]);
				//当前目标区域的像素坐标
				//int CA_row = boundRect.y + boundRect.height / 2;
				//int CA_col = boundRect.x + boundRect.width / 2;
				//当前目标位置的实际脉冲位置
				int CA_Pos_X = (Bi.cols / 2 - center.x)*Get_CA_PosThreadFunc.s_Scanning_System_Para.View_Field_Width_10X / Bi.cols
					+ Get_CA_PosThreadFunc.s_Scanning_Para.X_Scanning_Location[Focus_CA_10X_Num - 1];// +s_Scanning_System_Para.CA_XPos_Compensate;
				int CA_Pos_Y = (Bi.rows / 2 - center.y)*Get_CA_PosThreadFunc.s_Scanning_System_Para.View_Field_Higth_10X / Bi.rows
					+ Get_CA_PosThreadFunc.s_Scanning_Para.Y_Scanning_Location[Focus_CA_10X_Num - 1];// +s_Scanning_System_Para.CA_YPos_Compensate;
				Get_CA_PosThreadFunc.s_Scanning_Para.CA_Pos_10X.push_back(Point(CA_Pos_X, CA_Pos_Y));
			}
				
			//gray.at<uchar>(center.y, center.x) = 0;
		}

		Mat mask = Mat::zeros(Bi.size(), CV_8U);//同原图一样大小，纯黑的mask图像//CV_8UC3
		Mat draw = Mat::zeros(Bi.size(), CV_8U);//将原图copyTo到draw上，加上mask操作
		drawContours(mask, Right_contours, -1, Scalar(255), -1);//在mask上用白色绘制轮廓，-1向内填充

		//erode(mask, mask, dilate_element);
		gray.copyTo(draw, mask);//将原图copyTo到draw上，加上mask操作
			

		int size_CONUTORSS = Right_contours.size();
		int i_X,i_Y,i_W,i_H;
		if (size_CONUTORSS > 0)
		{

			for (int i = 0; i < Right_contours.size(); i++)//遍历每个轮廓
			{
				//Scalar color((rand() & 255), (rand() & 255), (rand() & 255));//随机产生颜色 rand()产生随机数
				minEnclosingCircle(Mat(Right_contours[i]), center, radius);
				center.x = center.x * 2048 / 409;
				center.y = center.y * 2048 / 409;
				radius = radius * 2048 / 409;
				circle(Img, center, radius, Scalar(255,0,0), 2);
				//circle(gray, center, radius, Scalar(0), 2);
				//当前目标位置的实际脉冲位置

				//int CA_Pos_X = (Bi.cols / 2 - CA_col)*Get_CA_PosThreadFunc.s_Scanning_System_Para.View_Field_Width_40X / Bi.cols
				//	+ Get_CA_PosThreadFunc.s_Scanning_Para.X_Scanning_Location[Focus_CA_10X_Num];
				//int CA_Pos_Y = (Bi.rows / 2 - CA_row)*Get_CA_PosThreadFunc.s_Scanning_System_Para.View_Field_Higth_40X / Bi.rows
				//	+ Get_CA_PosThreadFunc.s_Scanning_Para.Y_Scanning_Location[Focus_CA_10X_Num];
				//Get_CA_PosThreadFunc.s_Scanning_Para.CA_Pos_10X.push_back(Point(CA_Pos_X, CA_Pos_Y));

				//i_X = boundRect.x;
				//i_Y = boundRect.y;
				//i_W = boundRect.width;
				//i_H = boundRect.height;
				//draw.at<uchar>(i_Y, i_X) = 255;
				//draw.at<uchar>(i_Y, i_X + i_W) = 255;
				//draw.at<uchar>(i_Y + i_H, i_X) = 255;
				//draw.at<uchar>(i_Y + i_H, i_X + i_W) = 255;

			}

		}
		/*将定位的图存放在本地分析*/
		///*critical_section.Lock();*/
		string p2 = to_string(Get_CA_PosThreadFunc.s_Scanning_Para.Current_Slide_Num);
		string p = to_string(Focus_CA_10X_Num);
		string p_road = "D:\CA_10X_Pos\\CA_Pos_" + p2 + p + ".bmp";
		cv::imwrite(p_road, Img);
		/*critical_section.Unlock();*/
		//Sleep(10);

		Stop_c = clock();
		double jj = Stop_c - start_c;
		Focus_CA_10X_Num++;
	}

	CA_Focus_Thread_Running = false;
	return 0;

}

UINT Scanning_Control::Get_CA_Pos_10X_Func(int Focus_CA_10X_Num)
{
	s_Scanning_Para.CA_Pos_10X.clear();//首先清除存放CA位置的向量空间

		//////锁住线程
		////critical_section.Lock();
		////Mat Img = Get_CA_PosThreadFunc.Read_Img_From_Dish(Focus_CA_10X_Num, Get_CA_PosThreadFunc.s_Scanning_Para.Current_Slide_Num+1,
		////	Get_CA_PosThreadFunc.m_FocusingImage_Save_Paras);
		////critical_section.Unlock();
		////if (Img.empty())
		////	continue;
		////double scale = 0.2;
		////Size outsize;
		////Mat Resize;
		////outsize.height = Img.rows * scale;
		////outsize.width = Img.cols * scale;
		////resize(Img, Resize, outsize, 0, 0, INTER_LINEAR);
		////Mat gray,Bi;
		////cvtColor(Resize, gray, COLOR_RGB2GRAY);
		if (Scanning_10X_img.empty())
			return 0;
		int error_TIME;
		clock_t start_c = clock();
		clock_t Stop_c = clock();
		Mat Resize = *(Scanning_10X_img.end() - 1);
		Scanning_10X_img.pop_back();
		Mat gray, Bi;
		cvtColor(Resize, gray, COLOR_RGB2GRAY);
		int Otsu_Threshold = threshold(gray, Bi, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);//CV_THRESH_BINARY_INV
		double BIN_Threshold = Otsu_Threshold*0.8;

		threshold(gray, Bi, BIN_Threshold, 255, CV_THRESH_BINARY_INV);
		//**注意，如果或的二值图是白底黑目标区域的，就必须将图边界增加几个像素，否则后面的轮廓提取会把整体图片的边界也
		//作为是一个轮廓，这个轮廓会包含于边界相连通的所有区域***
		//copyMakeBorder(Bi, Bi, 2, 2, 2, 2, BORDER_CONSTANT, Scalar(0));

		Mat Biii = Bi.clone();
		Mat erode_element = getStructuringElement(MORPH_ELLIPSE, Size(
			s_Scanning_10X_Para.Erode_element, s_Scanning_10X_Para.Erode_element));
		//腐蚀，将染色体去除，留下非染色体区域
		erode(Biii, Biii, erode_element);
		for (int i = 0; i<Biii.rows - 1; i++)
		{
			for (int j = 0; j<Biii.cols - 1; j++)
			{
				if (Biii.at<uchar>(i, j) == 255)
				{
					Rect ccomp;
					//floodFill(Biii, Point(j, i), Scalar(0), &ccomp, Scalar(1), Scalar(1), 8);
					//如果是白底黑区域，则Biii.at<uchar>(i, j) == 0， Scalar(255)
					floodFill(Bi, Point(j, i), Scalar(0), &ccomp, Scalar(1), Scalar(1), 8);
				}
			}

		}
			
		Stop_c = clock();
		error_TIME = Stop_c - start_c;
		start_c = clock();
		Mat dilate_element = getStructuringElement(MORPH_ELLIPSE, Size(
			s_Scanning_10X_Para.Dilate_element, s_Scanning_10X_Para.Dilate_element));
		dilate(Bi, Bi, dilate_element);
		//查找轮廓
		vector<unsigned int> ROI_Size;
		vector < vector<Point> > contours;
		vector < vector<Point> >Right_contours;
		findContours(Bi, contours, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));//CV_RETR_CCOMP
		Rect boundRect;//存储轮廓的正外接矩形
		for (int i = 0; i < contours.size(); i++)//遍历每个轮廓
		{
			//Scalar color((rand() & 255), (rand() & 255), (rand() & 255));//随机产生颜色 rand()产生随机数
			Mat ROI;
			boundRect = boundingRect(Mat(contours[i]));//获得第i个轮廓的正外接矩形
			unsigned int contour_Aero = contourArea(contours[i]);
			ROI_Size.push_back(contour_Aero);
			if ((boundRect.width > Bi.cols / 2) || (boundRect.height > Bi.rows / 2))
				continue;
			if (contour_Aero > 250)
				Right_contours.push_back(contours[i]);
		}
		Stop_c = clock();
		error_TIME = Stop_c - start_c;
		start_c = clock();
		Mat mask = Mat::zeros(Bi.size(), CV_8U);//同原图一样大小，纯黑的mask图像//CV_8UC3
		Mat draw = Mat::zeros(Bi.size(), CV_8U);//将原图copyTo到draw上，加上mask操作
		drawContours(mask, Right_contours, -1, Scalar(255), -1);//在mask上用白色绘制轮廓，-1向内填充

		//erode(mask, mask, dilate_element);
		gray.copyTo(draw, mask);//将原图copyTo到draw上，加上mask操作


		int size_CONUTORSS = Right_contours.size();
		int i_X, i_Y, i_W, i_H;
		if (size_CONUTORSS > 0)
		{

			for (int i = 0; i < Right_contours.size(); i++)//遍历每个轮廓
			{
				//Scalar color((rand() & 255), (rand() & 255), (rand() & 255));//随机产生颜色 rand()产生随机数
				boundRect = boundingRect(Mat(Right_contours[i]));//获得第i个轮廓的正外接矩形
				//当前目标区域的像素坐标
				int CA_row = boundRect.y + boundRect.height / 2;
				int CA_col = boundRect.x + boundRect.width / 2;
				//当前目标位置的实际脉冲位置

				//int CA_Pos_X = (Bi.cols / 2 - CA_col)*Get_CA_PosThreadFunc.s_Scanning_System_Para.View_Field_Width_40X / Bi.cols
				//	+ Get_CA_PosThreadFunc.s_Scanning_Para.X_Scanning_Location[Focus_CA_10X_Num];
				//int CA_Pos_Y = (Bi.rows / 2 - CA_row)*Get_CA_PosThreadFunc.s_Scanning_System_Para.View_Field_Higth_40X / Bi.rows
				//	+ Get_CA_PosThreadFunc.s_Scanning_Para.Y_Scanning_Location[Focus_CA_10X_Num];
				//Get_CA_PosThreadFunc.s_Scanning_Para.CA_Pos_10X.push_back(Point(CA_Pos_X, CA_Pos_Y));

				//i_X = boundRect.x;
				//i_Y = boundRect.y;
				//i_W = boundRect.width;
				//i_H = boundRect.height;
				//draw.at<uchar>(i_Y, i_X) = 255;
				//draw.at<uchar>(i_Y, i_X + i_W) = 255;
				//draw.at<uchar>(i_Y + i_H, i_X) = 255;
				//draw.at<uchar>(i_Y + i_H, i_X + i_W) = 255;

			}

		}

		critical_section.Lock();
		string p = to_string(Focus_CA_10X_Num);
		string p_road = "C:\\Users\\wench\\Desktop\\capos\\CA_Pos_" + p + ".bmp";
		cv::imwrite(p_road, draw);
		critical_section.Unlock();
		//Sleep(10);

		Stop_c = clock();
		double jj = Stop_c - start_c;

	return 0;

}
int Scanning_Control::Do_Focusing_100X()
{

	Handlehold_Thread_Running = false;

	int states = Z_MoveDown_Avoid;

	//正常启动初始化所有变量，聚焦有关的；（包含两个部分，）
	Do_Scanning_intial();//扫描前运行参数初始化函数
	s_Dev_Drivers->Dev_Single_Mov_Para(1);
	s_Dev_Drivers->Dev_Single_Mov_Para(2);
	s_Dev_Drivers->Dev_Single_Mov_Para(3);
	s_Dev_Drivers->Dev_Single_Mov_Para(4);
	s_Dev_Drivers->Dev_Single_Mov_Para(5);
	Sleep(100);

	m_MaxFocLen = 0;
	Focusing_Search_Num = 0;
	Focus_Values_Max_Index = 0;

	long Z_Pos1, Z_Pos2;
	bool Read_Righ = true;
	while (Read_Righ)
	{
		while (!s_Dev_Drivers->Dev_Read_Pos(3, Z_Pos1))
			Sleep(50);
		Sleep(1000);
		while (!s_Dev_Drivers->Dev_Read_Pos(3, Z_Pos2))
			Sleep(50);
		if (Z_Pos2 == Z_Pos1)
			Read_Righ = false;
	}
	//以下已经通过基准焦平面设置
	//m_MaxFocLen_Z_Pos = Z_Pos1;
	//Last_MaxFocus_Pos = Z_Pos1;
	//Z_Upper_Limit = -8000 + s_Scanning_System_Para.Focusing_Step_Z[2] * 25;//-8000
	//Z_Low_Limit = -8000 - s_Scanning_System_Para.Focusing_Step_Z[2] * 25;
	//Z_Big_Step_Upper_Limit = -8000 + s_Scanning_System_Para.Focusing_Step_Z[2] * 50;
	//Z_Big_Step_Low_Limit = -8000 - s_Scanning_System_Para.Focusing_Step_Z[2] * 50;
	//Z_Big_Step_MaxFocus_Pos = -8000;

	vector <int>().swap(Focus_Poss);
	vector <double>().swap(Focus_Values);
	vector <double>().swap(Focus_Values_Later);
	vector <int>().swap(CA_Real_Pos_Y);
	vector <int>().swap(CA_Real_Pos_X);
	Get_Dic_F = false;
	Get_Dic_N = false;

	Focusing_Next_Step = s_Scanning_System_Para.Focusing_Step_Z[2];
	Focusing_Previous_Step = s_Scanning_System_Para.Focusing_Step_Z[2];
	int Focus_Level_100X = 0;//当前100X基准焦平面

	int ALL_CV_Num_100X;

	//vector<int> Get_Oil_Pos_X ;//已经泵油的位置
	//vector<int> Get_Oil_Pos_Y ;
	//Get_Oil_Pos_X.push_back(m_X_Moveto);
	//Get_Oil_Pos_Y.push_back(m_Y_Moveto);
	//bool First_Oil_Pos = true;

	if (!s_User_Para.NewStart_OR_BreakPoint)//断点启动额外初始化
	{
		//从数据库中读取已经完成的染色体位置值出来
	}
	else
	{
		ALL_CV_Num_100X = s_Scanning_Para.CA_Pos_10X.size();
		//s_Scanning_Para.Focal_Length.resize(ALL_CV_Num_100X);
		//s_Scanning_Para.X_Focus_Location.resize(s_User_Para.Focusing_Num_Y*s_User_Para.Focusing_Num_X);
		//s_Scanning_Para.Y_Focus_Location.resize(s_User_Para.Focusing_Num_Y*s_User_Para.Focusing_Num_X);

		m_point_i = 0;//聚焦记录点数归零
		s_Scanning_Para.Current_Point_Num = 0;
		//s_Scanning_Para.Current_Slide_Num = 0;
		//s_Scanning_Para.Current_Table_Num = 0;

		if (s_Scanning_Para.CA_Pos_10X.empty())
			return -1;
		m_X_Moveto = s_Scanning_Para.CA_Pos_10X[0].x;
		m_Y_Moveto = s_Scanning_Para.CA_Pos_10X[0].y;
	}
	m_Z_Moveto = s_Scanning_System_Para.Switch_Objective_ZPos;//首先向下运动2圈
	bool Z_Fisrt_MoveDown = true;
	//Scanning_10X_img.clear();//清空存放实时图像的Vector
	clock_t start_c = clock();
	clock_t Stop_c = clock();
	int Back_Num;
	bool Back_True0 = false;
	bool Back_True1 = false;
	bool Back_True2 = false;
	int Back_Num0 = 0;
	int Back_Num1 = 0;
	int Back_Num2 = 0;

	Pass_TakeImg = false;

	Last_Focus_X.clear();
	Last_Focus_Y.clear();
	Last_Focus_Z.clear();

	int Max_Focus_Z = 0;
	float CA2CA_Dis = 0;
	bool CA2CA_inside = false;
	Max_Focus_Value = -1;
	Max_Focus_Value_Later = -1;
	int i = 0;
	vector<int>::iterator Last_Focus_Z_it;
	Mat Focus_Pic, Focus_Pic1, OutGray;
	while (states != Scanning_Finished)
	{
		switch (states)
		{
		case Z_MoveDown_Avoid:
			Do_Stepping(3, m_Z_Moveto);
			states = Z_MoveDown_Avoid_Finished;
			break;
		case Z_MoveDown_Avoid_Finished:
			Back_Num = Do_Stepping_Finished(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{
				if (Z_Fisrt_MoveDown)
				{
					Z_Fisrt_MoveDown = false;
					states = Move_To_Object;
				}
				else
					states = Is_Scanning_Finished;//不会被执行
			}
			if (Back_Num == State_Freeze)
				states = Z_MoveDown_Avoid_Finished;
			if (Back_Num == Stepping)
				states = Z_MoveDown_Avoid;
			break;
			//******运动到本次扫描/聚焦需要用到的物镜******//
		case Move_To_Object:
			states = Do_Move_To_Object(s_Scanning_Para.Objective_Num);
			//Set_Camera_Paras(s_Scanning_Para.Objective_Num);
			break;
		case Move_To_Object_Fininshed:
			Back_Num = Do_Move_To_Object_Fininshed();
			if (Back_Num == State_Finished)//State_Finished 表示查询到完成状态
			{
				Pump_Oil_100X_Func();//给扫描区域泵油
				//Do_Find_Focus_Level_100X_Climb(800, Laplacian_Operator, false);
				//Do_Find_Focus_Level_100X_Climb(300, Laplacian_Operator, true);
				
				Do_Find_Focus_Level_100X(s_Scanning_System_Para.Focusing_Step_Z[0], MeanStd_Operator);//搜索100X下的基准焦平面
				Do_Find_Focus_Level_100X_Climb(300, Laplacian_Operator, true);
				//Do_Find_Focus_Level_100X(s_Scanning_System_Para.Focusing_Step_Z[0],false);
				//获得了最佳的焦距值，记录当前的X,Y,Z的坐标
				Last_Focus_X.push_back(m_X_Moveto);
				Last_Focus_Y.push_back(m_Y_Moveto);
				Last_Focus_Z.push_back(m_Z_Moveto);

				Focus_Level_100X = m_Z_Moveto;
				m_MaxFocLen_Z_Pos = Focus_Level_100X;
				Last_MaxFocus_Pos = Focus_Level_100X;
				Z_Upper_Limit = Focus_Level_100X + s_Scanning_System_Para.Focusing_Step_Z[2] * 25;//-8000
				Z_Low_Limit = Focus_Level_100X - s_Scanning_System_Para.Focusing_Step_Z[2] * 25;
				Z_Big_Step_Upper_Limit = Focus_Level_100X + s_Scanning_System_Para.Focusing_Step_Z[2] * 50;
				Z_Big_Step_Low_Limit = Focus_Level_100X - s_Scanning_System_Para.Focusing_Step_Z[2] * 50;
				Z_Big_Step_MaxFocus_Pos = Focus_Level_100X;

				m_Searching_Model = Little_Step;
				Obtain_Optimal_Focal = false;
				Focusing_Search_Num = 0;
				Focus_Values_Max_Index = 0;
				states = Is_Scanning_Finished;
			}		
			if (Back_Num == State_Freeze)//State_Freeze表示本次查询没有查询到完成状态
				states = Move_To_Object_Fininshed;
			break;
		////case Z_MoveDown_AvoidBack:
		////	m_Z_Moveto = Focus_Level_100X;//这个是100X的清晰图像位置  -8000（没加油）
		////	Back_Num = Do_Stepping(3, m_Z_Moveto);
		////	if (Back_Num == State_Finished)
		////	{
		////		states = Z_MoveDown_AvoidBack_Finished;
		////	}
		////	if (Back_Num == State_Freeze)
		////		states = Z_MoveDown_AvoidBack;
		////	break;
		////case Z_MoveDown_AvoidBack_Finished:
		////	Back_Num = Do_Stepping_Finished(3, m_Z_Moveto);
		////	if (Back_Num == State_Finished)
		////	{
		////		//m_Searching_Model = Big_Step;
		////		m_Searching_Model = Little_Step;
		////		Obtain_Optimal_Focal = false;
		////		//states = Search_Optimal_Focal_Distance;
		////		m_Z_Moveto = m_Z_Moveto + 10000;
		////		states = Z_MoveDown_Avoid;
		////	}
		////	if (Back_Num == State_Freeze)
		////		states = Z_MoveDown_AvoidBack_Finished;
		////	if (Back_Num == Stepping)
		////		states = Z_MoveDown_AvoidBack;
		////	break;
			//*************自动聚焦*************//
		case Search_Optimal_Focal_Distance:
			/*如果当前位置距离上一个聚焦点位置大于某一个阀值时，则对当前点进行高精度聚焦，用来获得新的焦距值*/

			Last_Focus_Z_it = Last_Focus_Z.begin();
			i = 0;
			CA2CA_inside = false;
			while ((Last_Focus_Z_it != Last_Focus_Z.end()) && !CA2CA_inside && (Focusing_Search_Num == 0))
			{
				CA2CA_Dis = sqrt(pow((double)(Last_Focus_X[i] - m_X_Moveto) / s_Scanning_System_Para.Pulse_ratio, 2)
					+ pow((double)(Last_Focus_Y[i] - m_Y_Moveto) / s_Scanning_System_Para.Pulse_ratio, 2));
				CA2CA_inside = CA2CA_Dis < s_Scanning_System_Para.Focus_Equal_Dis;
				if (CA2CA_inside)
				{
					m_Z_Moveto = *Last_Focus_Z_it;
					Focus_Level_100X = m_Z_Moveto;
					Last_Focus_Z_it = Last_Focus_Z.end();
					Do_Stepping(3, m_Z_Moveto);
					vector<bool> status_arr = { 0 };
					bool Move_In_Deviation = false;
					long Current_Pos = 0;
					while ((!status_arr[0]) && Move_In_Deviation)
					{
						s_Dev_Drivers->Dev_Read_Status({ 3 }, status_arr, 1);
						bool Get_Pos_Move = s_Dev_Drivers->Dev_Read_Pos(3, Current_Pos);
						Move_In_Deviation = (Current_Pos >(m_Z_Moveto - s_Scanning_System_Para.Move_Deviation)) &&
							(Current_Pos < (m_Z_Moveto + s_Scanning_System_Para.Move_Deviation));
					}
				}
				else
				{
					Last_Focus_Z_it++;
					i++;
				}
			}
			if (!CA2CA_inside && (Focusing_Search_Num == 0))
			{
				Do_Find_Focus_Level_100X_Climb(400, MeanStd_Operator, true);
				/*Do_Find_Focus_Level_100X(300, Laplacian_Operator, false, true);*/
				//获得了最佳的焦距值，记录当前的X,Y,Z的坐标
				//表示，如果当前位置的聚焦值，比所有的最大值还要大1000个脉冲，说明，本次聚焦是失败的，舍去这个聚焦值
				Last_Focus_X.push_back(m_X_Moveto);
				Last_Focus_Y.push_back(m_Y_Moveto);
				Last_Focus_Z.push_back(m_Z_Moveto);

				//if (Max_Focus_Z < m_X_Moveto)
				//{
				//	Max_Focus_Z = m_Z_Moveto;
				//}
				Focusing_Search_Num = 0;
				Focus_Values_Max_Index = 0;
			}

			m_MaxFocLen = Calculate_Sharpness_Every_Point(Focus_Level_100X);

			if (Obtain_Optimal_Focal)
			{
				//本次聚焦完成，存储当前的
				//s_Scanning_Para.Focal_Length[s_Scanning_Para.Current_Point_Num] = m_Z_Moveto;//保存当前的焦距值
				//s_Scanning_Para.X_Focus_Location[s_Scanning_Para.Current_Point_Num] = m_X_Moveto;//保存当前的坐标的X绝对值
				//s_Scanning_Para.Y_Focus_Location[s_Scanning_Para.Current_Point_Num] = m_Y_Moveto;//保存当前的坐标的Y绝对值
				//s_Scanning_Para.Focal_Length.push_back(m_Z_Moveto);
				//s_Scanning_Para.X_Focus_Location.push_back(m_X_Moveto);
				//s_Scanning_Para.Y_Focus_Location.push_back(m_Y_Moveto);
				Get_Dic_N = false;
				Get_Dic_F = false;
				Focusing_Search_Num = 0;
				Focus_Values_Max_Index = 0;
				Max_Focus_Value = -1;
				Max_Focus_Value_Later = -1;

				double sum = 0;
				vector<int>::const_iterator itc = s_Scanning_Para.Focal_Length.begin();
				int Aero_Temp = 0;
				while (itc != s_Scanning_Para.Focal_Length.end())
				{
					sum = sum + *itc;
					itc++;
				}
				Last_MaxFocus_Pos = sum / (s_Scanning_Para.Current_Point_Num + 1); //均值
				//Last_MaxFocus_Pos = m_Z_Moveto;

				if (!Pass_TakeImg)
				{//直接存储聚焦图像序列中的清晰图像
					//Save_CVimg(m_FocusingImage_Save_Paras, Best_Img);
					Save_CVimg(m_FocusingImage_Save_Paras, Best_Img_Later);

					//Sleep(50);
					//TakeVeido_And_Save_Image_Raw(m_FocusingImage_Save_Paras, true);//抓图在聚焦
				}
				Pass_TakeImg = false;
				//Get_CA_Pos_10X_Func(s_Scanning_Para.Current_Point_Num + 1);

				//m_Z_Moveto = Z_Upper_Limit;

				s_Scanning_Para.Current_Point_Num++;
				m_point_i = 0;//聚焦记录点数归零
				//m_Searching_Model = Big_Step;
				m_Searching_Model = Little_Step;
				Obtain_Optimal_Focal = false;
				m_Focusing_Equal_Scaning = false;
				//将当前已经拍下的染色体目标坐标压入对应的向量中，并保存
				CA_Real_Pos_Y.push_back(m_Y_Moveto);
				CA_Real_Pos_X.push_back(m_X_Moveto);
				states = Is_Scanning_Finished;
			}
			else
			{
				start_c = clock();
				//只有当只是从聚焦图片中获取最佳图片并保存时用到，这里还需要在最前面有近距离聚焦点匹配
				//m_MaxFocLen = Calculate_Sharpness_Every_Point(Focus_Level_100X);

				Stop_c = clock();
				double jj = Stop_c - start_c;
				states = Z_Stepping;
			}
			break;
		case Z_Stepping:
			start_c = clock();
			Do_Stepping(3, m_Z_Moveto);
			states = Z_Stepping_Finished;
			break;
		case Z_Stepping_Finished:
			Back_Num = Do_Stepping_Finished(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{
				Sleep(20);
				states = Search_Optimal_Focal_Distance;
			}
			if (Back_Num == State_Freeze)
				states = Z_Stepping_Finished;
			if (Back_Num == Stepping)
				states = Z_Stepping;
			break;
			//************判断*****************//
		case Is_Scanning_Finished:
			if (s_Scanning_Para.Current_Point_Num > s_Scanning_Para.CA_Pos_10X.size()-1)
				states = Scanning_Finished;
			else
			{
				//m_Z_Moveto = Focus_Level_100X;/启用，表示采用了每个点都从基准焦平面出发
				m_Y_Moveto = s_Scanning_Para.CA_Pos_10X[s_Scanning_Para.Current_Point_Num].y;
				m_X_Moveto = s_Scanning_Para.CA_Pos_10X[s_Scanning_Para.Current_Point_Num].x;
				states = X_Stepping;
			}
			break;
			//*************运动到下一个点*******************//
		case X_Stepping:
		case Y_Stepping:
			Do_Stepping(2, m_Y_Moveto);
			Do_Stepping(1, m_X_Moveto);
			//Sleep(2000);
			Do_Stepping(3, m_Z_Moveto);
			Back_True0 = false;
			Back_True1 = false;
			Back_True2 = false;
			states = X_Stepping_Finished;
			break;
		case X_Stepping_Finished:
		case Y_Stepping_Finished:
			Back_Num0 = Do_Stepping_Finished(1, m_X_Moveto);
			Back_Num1 = Do_Stepping_Finished(2, m_Y_Moveto);
			Back_Num2 = Do_Stepping_Finished(3, m_Z_Moveto);

			if ((Back_Num0 == State_Finished) && (Back_Num1 == State_Finished) && (Back_Num2 == State_Finished))
			{
				Back_True0 = false;
				Back_True1 = false;
				Back_True2 = false;
				////Pump Oil 依据已获取的染色体的位置，实时跟随加油
				//if (Get_Oil_Pos_X.empty())
				//{
				//	while (Do_Stepping(5, s_Scanning_System_Para.Pump_Oil_Pulse) != State_Finished)
				//	{
				//	}
				//	while (Do_Stepping_Finished(5, m_5_Moveto) != State_Finished)
				//	{
				//	}
				//	Sleep(2000);
				//	//将当前泵油的位置保存
				//	Get_Oil_Pos_X.push_back(m_X_Moveto);
				//	Get_Oil_Pos_Y.push_back(m_Y_Moveto);
				//	bool First_Oil_Pos = false;
				//}
				//else
				//{
				//	bool Need_Pump_Oil = false;
				//	vector<int>::iterator it = Get_Oil_Pos_X.begin();
				//	vector<int>::iterator it2 = Get_Oil_Pos_Y.begin();
				//	while ((it != Get_Oil_Pos_X.end()) && (!Need_Pump_Oil))
				//	{
				//		Need_Pump_Oil = (abs(*it - m_X_Moveto) > s_Scanning_System_Para.Pump_Oil_DisPulse)
				//			&& (abs(*it2 - m_Y_Moveto) > s_Scanning_System_Para.Pump_Oil_DisPulse);
				//	}
				//	if (Need_Pump_Oil)
				//	{
				//		while (Do_Stepping(5, s_Scanning_System_Para.Pump_Oil_Pulse) != State_Finished)
				//		{
				//		}
				//		vector<bool> status_arr = { 0 };
				//		while (!status_arr[0])
				//		{
				//			s_Dev_Drivers->Dev_Read_Status({ 5 }, status_arr, 1);
				//		}
				//		//将当前泵油的位置保存
				//		Get_Oil_Pos_X.push_back(m_X_Moveto);
				//		Get_Oil_Pos_Y.push_back(m_Y_Moveto);
				//	}
				//Take_Video_Image(Focus_Pic);
				//Sleep(30);
				//Take_Video_Image(Focus_Pic1);
				//while (!Img_Same(Focus_Pic, Focus_Pic1));
				//{
				//	Take_Video_Image(Focus_Pic);
				//	Sleep(5);
				//	Take_Video_Image(Focus_Pic1);
				//}

				if (s_Scanning_Para.Current_Point_Num == 0)//如果是第一个点位置，则等待3s，等待油禁止
					Sleep(3000);
				states = Search_Optimal_Focal_Distance;

			}
			else if ((Back_Num0 == Stepping) || (Back_Num1 == Stepping) || (Back_Num2 == Stepping))
			{
				states = X_Stepping;
			}
			else
				states = X_Stepping_Finished;
			break;

		default:
			break;
		}

	}

	Scanning_Completed_Status.push_back(true);
	//Do_Scanning_Finished();
	return 1;

}
//这里采用爬山法多点逐点聚焦
int Scanning_Control::Do_Find_Focus_Level_100X_Climb(int Focus_LevelSearch_Step_Z, int Focus_Method, bool Current_Point)
{
	Handlehold_Thread_Running = false;

	if (!Current_Point)
	{
		if (s_Scanning_Para.CA_Pos_10X.empty())
		{
			m_Y_Moveto = 0;
			m_X_Moveto = -s_Scanning_Para.Current_Slide_Num *(s_Scanning_System_Para.Slide_Size_X
				+ s_Scanning_System_Para.Space_Slides)*s_Scanning_System_Para.Pulse_ratio;
		}
		else
		{
			m_Y_Moveto = s_Scanning_Para.CA_Pos_10X[s_Scanning_Para.CA_Pos_10X.size() / 2].y;
			m_X_Moveto = s_Scanning_Para.CA_Pos_10X[s_Scanning_Para.CA_Pos_10X.size() / 2].x;
		}
	}

	vector <int>().swap(Focus_Poss);
	vector <double>().swap(Focus_Values); 
	Obtain_Optimal_Focal = false;
	//Scanning_10X_img.clear();//清空存放实时图像的Vector
	clock_t start_c = clock();
	clock_t Stop_c = clock();
	int Back_Num;
	bool Back_True0 = false;
	bool Back_True1 = false;
	bool Back_True2 = false;
	bool P_Dir=true;
	int states = Z_MoveDown_Avoid;
	while (states != Scanning_Finished)
	{
		switch (states)
		{
		case Z_MoveDown_Avoid:
			Do_Stepping(3, m_Z_Moveto);
			states = Z_MoveDown_Avoid_Finished;
			break;
		case Z_MoveDown_Avoid_Finished:
			Back_Num = Do_Stepping_Finished(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{

				states = X_Stepping;
			}
			if (Back_Num == State_Freeze)
				states = Z_MoveDown_Avoid_Finished;
			if (Back_Num == Stepping)
				states = Z_MoveDown_Avoid;
			break;

			//*************自动聚焦*************//
		case Search_Optimal_Focal_Distance:
		{
			Mat Focus_Pic,Focus_Pic1, OutGray;
			Take_Video_Image(Focus_Pic);
			//Sleep(10);
			//Take_Video_Image(Focus_Pic1);
			//while (!Img_Same(Focus_Pic, Focus_Pic1));
			//{
			//	Take_Video_Image(Focus_Pic);
			//	Sleep(10);
			//	Take_Video_Image(Focus_Pic1);
			//}

			double scale = 0.2;
			Size outsize;
			Mat Resize;
			outsize.height = Focus_Pic.rows*scale;
			outsize.width = Focus_Pic.cols*scale;
			resize(Focus_Pic, Resize, outsize, 0, 0, INTER_LINEAR);
			cvtColor(Resize, OutGray, CV_BGR2GRAY);
			double Focus_Value;
			Mat meanValueImage;
			Mat meanStdValueImage;
			Mat LapValueImage;
			switch (Focus_Method)
			{
			case Laplacian_Operator:
				Laplacian(OutGray, LapValueImage, CV_16U);
				Focus_Value = mean(LapValueImage)[0];
				break;
			case MeanStd_Operator:

				meanStdDev(OutGray, meanValueImage, meanStdValueImage);
				Focus_Value = meanStdValueImage.at<double>(0, 0);
				break;
			case Thenengrad_Operator:
				Focus_Value = Thenengrad(OutGray);
				break;
			default:
				break;
			}
			////以下为保存每个聚焦图像
			//Focusing_Search_Num++;//临时
			//string zpos = to_string(m_Z_Moveto);
			//string p_Num = to_string(s_Scanning_Para.Current_Point_Num);
			//string p = to_string(Focusing_Search_Num);
			//string mean_Value = to_string(Focus_Value);
			//string p_road = "H:\\focusing_onebyone2\\" + p_Num + "-" + p + " " + zpos + "+" + mean_Value
			//	+ ".bmp";
			//cv::imwrite(p_road, Focus_Pic);

			int FocusSize = Focus_Values.size();
			if (FocusSize < 7)
			{
				Focus_Values.push_back(Focus_Value);
				Focus_Poss.push_back(m_Z_Moveto);
				m_Z_Moveto = m_Z_Moveto + Focus_LevelSearch_Step_Z;
				P_Dir = true;
				states = Z_Stepping;
				break;
			}

			if (P_Dir)
			{
				Focus_Values.push_back(Focus_Value);
				Focus_Poss.push_back(m_Z_Moveto);
			}
			else
			{
				Focus_Values.insert(Focus_Values.begin(), Focus_Value);
				Focus_Poss.insert(Focus_Poss.begin(), m_Z_Moveto);
			}
			std::vector<double>::iterator biggest = std::max_element(std::begin(Focus_Values), std::end(Focus_Values));
			int MaxFocusPos = std::distance(std::begin(Focus_Values), biggest);
			int n = 5;//表示爬山的点数，总点数为 2*n+1
			FocusSize = Focus_Values.size();
			if (FocusSize - MaxFocusPos >= n + 1)//0,1,2,3,4,...,MaxFocusPos,.,.,.,.,.FocusSize
			{
				if (MaxFocusPos >= n)
				{
					Obtain_Optimal_Focal = true;
					if (Focus_Method == Laplacian_Operator)
					{	//注意以下推断是聚焦过程中，Z轴从下往上运动的结果
						float Down_Error = Focus_Values[MaxFocusPos] - Focus_Values[MaxFocusPos + 1];//注意务必保证最佳焦距处于中间位置
						float Up_Error = Focus_Values[MaxFocusPos] - Focus_Values[MaxFocusPos - 1];
						if (Down_Error * 2 < Up_Error)
						{
							if (s_Scanning_Para.Objective_Num == Objective_40X)
								m_Z_Moveto = Focus_Poss[MaxFocusPos + 1];//测得当位置值大的时候更合适
							else
								m_Z_Moveto = Focus_Poss[MaxFocusPos - 1];
						}
						else if (Down_Error <= Up_Error)
						{
							if (s_Scanning_Para.Objective_Num == Objective_40X)
								//m_Z_Moveto = (Focus_Poss[MaxFocusPos] + Focus_Poss[MaxFocusPos - 1]) / 2;
								m_Z_Moveto = Focus_Poss[MaxFocusPos + 1];
							else
								m_Z_Moveto = (Focus_Poss[MaxFocusPos] + Focus_Poss[MaxFocusPos - 1]) / 2;
						}
						else
						{
							if (s_Scanning_Para.Objective_Num == Objective_40X)
								//m_Z_Moveto = (Focus_Poss[MaxFocusPos] + Focus_Poss[MaxFocusPos - 1]) / 2;
								m_Z_Moveto = Focus_Poss[MaxFocusPos + 1];
							else
								m_Z_Moveto = Focus_Poss[MaxFocusPos];
						}
					}
					else
					{
						m_Z_Moveto = Focus_Poss[MaxFocusPos];
					}

					vector <int>().swap(Focus_Poss);
					vector <double>().swap(Focus_Values);

				}
				else
				{
					m_Z_Moveto = Focus_Poss[MaxFocusPos] - (MaxFocusPos + 1)*Focus_LevelSearch_Step_Z;
					P_Dir = false;
					states = Z_Stepping;
				}
			}
			else
			{
				m_Z_Moveto = Focus_Poss[MaxFocusPos] + (FocusSize-MaxFocusPos)*Focus_LevelSearch_Step_Z;
				P_Dir = true;
				states = Z_Stepping;
			}

			if (Obtain_Optimal_Focal)
			{
				Obtain_Optimal_Focal = false;
				Focusing_Search_Num = 0;
				Do_Stepping(3, m_Z_Moveto);
				Back_Num = 0;
				while (Back_Num != State_Finished)
				{
					Back_Num = Do_Stepping_Finished(3, m_Z_Moveto);
				}


				states = Scanning_Finished;
			}
			break;
		}
		case Z_Stepping:
			start_c = clock();
			Back_Num = Do_Stepping(3, m_Z_Moveto);
			states = Z_Stepping_Finished;
			break;
		case Z_Stepping_Finished:
			Back_Num = Do_Stepping_Finished(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{
				states = Search_Optimal_Focal_Distance;
			}
			if (Back_Num == State_Freeze)
				states = Z_Stepping_Finished;
			if (Back_Num == Stepping)
				states = Z_Stepping;
			break;

			//*************运动到下一个点*******************//
		case X_Stepping:
		case Y_Stepping:
			Do_Stepping(2, m_Y_Moveto);
			Do_Stepping(1, m_X_Moveto);
			Back_True0 = false;
			Back_True1 = false;
			states = X_Stepping_Finished;
			break;
		case X_Stepping_Finished:
		case Y_Stepping_Finished:
			if (!Back_True0)
			{
				if (Do_Stepping_Finished(1, m_X_Moveto) == State_Finished)
					Back_True0 = true;
			}
			if (!Back_True1)
			{
				if (Do_Stepping_Finished(2, m_Y_Moveto) == State_Finished)
					Back_True1 = true;
			}

			if (Back_True0 && Back_True1)
			{
				states = Search_Optimal_Focal_Distance;
			}
			else
				states = X_Stepping_Finished;
			break;
		default:
			break;
		}

	}

	return 1;
}
//聚焦函数-逐点区焦距，然后区最大值   求解图像的评价函数，并将图像和值保存
int Scanning_Control::Do_Find_Focus_Level_100X(int Focus_LevelSearch_Step_Z, int Focus_Method, bool BigORLittle, bool Current_Point)
{
	Handlehold_Thread_Running = false;

	int states = Z_MoveDown_Avoid;
	//默认大范围内自动聚焦，大范围的范围又系统管理员给定
	int Z_OneByOne_Uper = s_Scanning_System_Para.Focus_Min_Limit;//电机正转向下运动，因此对扫描来讲Uper表示的是最小值
	int Z_OneByOne_Low = s_Scanning_System_Para.Focus_Max_Limit;
	if (!BigORLittle)
	{	//进行小范围内自动聚焦
		switch (s_Scanning_Para.Objective_Num)
		{
		case Objective_100X:
			Z_OneByOne_Uper = m_Z_Moveto - 4000;
			Z_OneByOne_Low = m_Z_Moveto + 5000;
			break;
		case Objective_10X:
			Z_OneByOne_Uper = m_Z_Moveto - Focus_LevelSearch_Step_Z * 4;
			Z_OneByOne_Low = m_Z_Moveto + Focus_LevelSearch_Step_Z * 4;
			break;
		case Objective_40X:
			Z_OneByOne_Uper = m_Z_Moveto - Focus_LevelSearch_Step_Z * 10;
			Z_OneByOne_Low = m_Z_Moveto + Focus_LevelSearch_Step_Z * 10;
			break;
		case Objective_20X:
			Z_OneByOne_Uper = m_Z_Moveto - Focus_LevelSearch_Step_Z * 4;
			Z_OneByOne_Low = m_Z_Moveto + Focus_LevelSearch_Step_Z * 4;
			break;
		default:
			break;
		}

	}

	m_Z_Moveto = Z_OneByOne_Low;
	if (!Current_Point)
	{
		if (s_Scanning_Para.CA_Pos_10X.empty())
		{
			m_Y_Moveto = 0;
			m_X_Moveto = -s_Scanning_Para.Current_Slide_Num *(s_Scanning_System_Para.Slide_Size_X
				+ s_Scanning_System_Para.Space_Slides)*s_Scanning_System_Para.Pulse_ratio;
		}
		else
		{
			m_Y_Moveto = s_Scanning_Para.CA_Pos_10X[s_Scanning_Para.CA_Pos_10X.size() / 2].y;
			m_X_Moveto = s_Scanning_Para.CA_Pos_10X[s_Scanning_Para.CA_Pos_10X.size() / 2].x;
		}
	}

	vector <int>().swap(Focus_Poss);
	vector <double>().swap(Focus_Values);
	Obtain_Optimal_Focal = false;
	//Scanning_10X_img.clear();//清空存放实时图像的Vector
	clock_t start_c = clock();
	clock_t Stop_c = clock();
	int Back_Num;
	bool Back_True0 = false;
	bool Back_True1 = false;
	bool Back_True2 = false;
	while (states != Scanning_Finished)
	{
		switch (states)
		{
		case Z_MoveDown_Avoid:
			Do_Stepping(3, m_Z_Moveto);
			states = Z_MoveDown_Avoid_Finished;
			break;
		case Z_MoveDown_Avoid_Finished:
			Back_Num = Do_Stepping_Finished(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{
				states = X_Stepping;
			}
			if (Back_Num == State_Freeze)
				states = Z_MoveDown_Avoid_Finished;
			if (Back_Num == Stepping)
				states = Z_MoveDown_Avoid;
			break;

			//*************自动聚焦*************//
		case Search_Optimal_Focal_Distance:

			Calculate_Sharpness_OneByOne(Z_OneByOne_Uper, Focus_Method);

			if (Obtain_Optimal_Focal)
			{
				Obtain_Optimal_Focal = false;
				Do_Stepping(3, m_Z_Moveto);
				while (Back_Num != State_Finished)
				{
					Back_Num = Do_Stepping_Finished(3, m_Z_Moveto);
				}
				states = Scanning_Finished;
			}
			else
			{
				//start_c = clock();
				//if (s_Scanning_Para.Objective_Num == Objective_100X)
				//	m_Z_Moveto = m_Z_Moveto - Focus_LevelSearch_Step_Z / 4;
				//else
					m_Z_Moveto = m_Z_Moveto - Focus_LevelSearch_Step_Z;

				//Stop_c = clock();
				//double jj = Stop_c - start_c;
				states = Z_Stepping;
			}
			break;
		case Z_Stepping:
			//start_c = clock();
			Do_Stepping(3, m_Z_Moveto);
			states = Z_Stepping_Finished;
			break;
		case Z_Stepping_Finished:
			Back_Num = Do_Stepping_Finished(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{
				states = Search_Optimal_Focal_Distance;
			}
			if (Back_Num == State_Freeze)
				states = Z_Stepping_Finished;
			if (Back_Num == Stepping)
				states = Z_Stepping;
			break;

			//*************运动到下一个点*******************//
		case X_Stepping:
		case Y_Stepping:
			Back_Num = Do_Stepping(2, m_Y_Moveto);
			Back_Num = Do_Stepping(1, m_X_Moveto);
			Back_True0 = false;
			Back_True1 = false;
			states = X_Stepping_Finished;
			break;
		case X_Stepping_Finished:
		case Y_Stepping_Finished:
			if (!Back_True0)
			{
				if (Do_Stepping_Finished(1, m_X_Moveto) == State_Finished)
					Back_True0 = true;
			}
			if (!Back_True1)
			{
				if (Do_Stepping_Finished(2, m_Y_Moveto) == State_Finished)
					Back_True1 = true;
			}

			if (Back_True0 && Back_True1)
			{
				Back_True0 = false;
				Back_True1 = false;

				if (s_Scanning_Para.Objective_Num == Objective_100X)
					Sleep(10000);

				states = Search_Optimal_Focal_Distance;
			}
			else
				states = X_Stepping_Finished;
			break;
		default:
			break;
		}

	}

	return 1;
}
int Scanning_Control::Calculate_Sharpness_OneByOne(int Z_OneByOne_Uper, int Focus_Method)
{
	Mat Focus_Pic, OutGray;
	Take_Video_Image(Focus_Pic);

	double scale = 0.2;
	Size outsize;
	Mat Resize;
	outsize.height = Focus_Pic.rows*scale;
	outsize.width = Focus_Pic.cols*scale;
	resize(Focus_Pic, Resize, outsize, 0, 0, INTER_LINEAR);

	cvtColor(Resize, OutGray, CV_BGR2GRAY);
	double Focus_Value;

	Mat meanValueImage;
	Mat meanStdValueImage;
		Mat LapValueImage;
		switch (Focus_Method)
	{
		case Laplacian_Operator:
				Laplacian(OutGray, LapValueImage, CV_16U);
				Focus_Value = mean(LapValueImage)[0];
				break;
		case MeanStd_Operator:

			meanStdDev(OutGray, meanValueImage, meanStdValueImage);
			Focus_Value = meanStdValueImage.at<double>(0, 0);
			break;
		case Thenengrad_Operator:
			Focus_Value = Thenengrad(OutGray);
			break;
		default:
			break;
		}

	Focus_Values.push_back(Focus_Value);
	Focus_Poss.push_back(m_Z_Moveto);

	//以下为保存每个聚焦图像
	//Focusing_Search_Num++;//临时
	//string zpos = to_string(m_Z_Moveto);
	//string p_Num = to_string(s_Scanning_Para.Current_Point_Num);
	//string p = to_string(Focusing_Search_Num);
	//string mean_Value = to_string(Focus_Value);
	////string mean_Value0 = to_string(Focus_Value0);
	////vector<int> compression_params;
	////compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	////compression_params.push_back(9);    //png格式下，默认的参数为3.
	//string p_road = "H:\\focusing_onebyone\\" + p_Num + "-" + p + " " + zpos + "+" + mean_Value
	//	+ ".bmp";
	//cv::imwrite(p_road, Focus_Pic);

	//////critical_section.Lock();
	//////string p_road = "C:\\Users\\wench\\Desktop\\focus\\" + p_Num + "-" + p + " " + zpos + "+" + mean_Value + ".bmp";
	//////cv::imwrite(p_road, Focus_Pic);
	//////critical_section.Unlock();

	if( (m_Z_Moveto < Z_OneByOne_Uper) || Obtain_Optimal_Focal)
	{
		std::vector<double>::iterator biggest = std::max_element(std::begin(Focus_Values), std::end(Focus_Values));
		int MaxFocusPos = std::distance(std::begin(Focus_Values), biggest);
		//以下根据最大清晰度值与前后相邻清晰度值进行比较分析，得出最油焦距值
		if (Focus_Method == Laplacian_Operator)
		{	//注意以下推断是聚焦过程中，Z轴从下往上运动的结果
			//float Up_Error = Focus_Values[MaxFocusPos] - Focus_Values[MaxFocusPos + 1];//注意务必保证最佳焦距处于中间位置
			//float Down_Error = Focus_Values[MaxFocusPos] - Focus_Values[MaxFocusPos - 1];
			//if (Down_Error * 2 < Up_Error)
			//	m_Z_Moveto = Focus_Poss[MaxFocusPos-1];
			//else if (Down_Error <= Up_Error)
			//	m_Z_Moveto = (Focus_Poss[MaxFocusPos] + Focus_Poss[MaxFocusPos - 1])/2;
			//else
				m_Z_Moveto = Focus_Poss[MaxFocusPos];
		}
		else
			m_Z_Moveto = Focus_Poss[MaxFocusPos];
		Obtain_Optimal_Focal = true;
		vector <int>().swap(Focus_Poss);
		vector <double>().swap(Focus_Values);
		return m_Z_Moveto;

		Focusing_Search_Num = 0;//临时
	}

	return 0;//正常返回

}
int Scanning_Control::Pump_Oil_100X_Func()
{
	double Pump_Oil_Aero_X;
	double Pump_Oil_Aero_Y;
	if (s_User_Para.Scanning_Aero_X > s_Scanning_System_Para.Pump_Oil_Dis)
		Pump_Oil_Aero_X = s_User_Para.Scanning_Aero_X - s_Scanning_System_Para.Pump_Oil_Dis / 2;
	else
		Pump_Oil_Aero_X = s_User_Para.Scanning_Aero_X/2;

	if (s_User_Para.Scanning_Aero_Y > s_Scanning_System_Para.Pump_Oil_Dis)
		Pump_Oil_Aero_Y = s_User_Para.Scanning_Aero_Y - s_Scanning_System_Para.Pump_Oil_Dis / 2;
	else
		Pump_Oil_Aero_Y = s_User_Para.Scanning_Aero_Y;

	double Real_View_Field_Higth = s_Scanning_System_Para.View_Field_Higth_10X / 10;
	double Real_View_Field_Width = s_Scanning_System_Para.View_Field_Width_10X / 10;
	int Pump_Oil_Num_Y = ceil(Pump_Oil_Aero_Y  / (double)s_Scanning_System_Para.Pump_Oil_Dis) + 1;
	int Pump_Oil_Num_X = ceil(Pump_Oil_Aero_X  / (double)s_Scanning_System_Para.Pump_Oil_Dis) + 1;

	int Pump_Oil_Step_X = -(Pump_Oil_Aero_X / (Pump_Oil_Num_X-1))
		*s_Scanning_System_Para.Pulse_ratio;
	int Pump_Oil_Step_Y = ((double)Pump_Oil_Aero_Y / (Pump_Oil_Num_Y-1))
		*s_Scanning_System_Para.Pulse_ratio;

	//int Pump_Oil_Step_X = -(Pump_Oil_Aero_X / Pump_Oil_Num_X)
	//	*s_Scanning_System_Para.Pulse_ratio;
	//int Pump_Oil_Step_Y = -((double)Pump_Oil_Aero_Y / Pump_Oil_Num_Y)
	//	*s_Scanning_System_Para.Pulse_ratio;

	int Pump_Oil_Start_X = -((s_Scanning_System_Para.Space_Slides + s_Scanning_System_Para.Slide_Size_X)
		* s_Scanning_Para.Current_Slide_Num - Pump_Oil_Aero_X / 2) * s_Scanning_System_Para.Pulse_ratio;
	int Pump_Oil_Start_Y = -(Pump_Oil_Aero_Y / 2+3) * s_Scanning_System_Para.Pulse_ratio;//由于注射针头靠外，所以纠正3mm
	int Pump_Oil_Current_Point_Num = 0;
	int Pump_Oil_Current_Scanning_Num_Y = 0;
	int Pump_Oil_Current_Scanning_Num_X = 0;

	int states = Z_MoveDown_AvoidBack;
	int Back_Num = State_Freeze;
	int Back_Num1 = State_Freeze;
	int Back_Num2 = State_Freeze;
	while (states != Scanning_Finished)
	{
		switch (states)
		{
			//*****运动到预测的100X焦平面区域//
		case Z_MoveDown_AvoidBack:
			m_Z_Moveto = 0;
			Back_Num = Do_Stepping(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{
				states = Z_MoveDown_AvoidBack_Finished;
			}
			if (Back_Num == State_Freeze)
				states = Z_MoveDown_AvoidBack;
			break;
		case Z_MoveDown_AvoidBack_Finished:
			Back_Num = Do_Stepping_Finished(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
			{
				states = Move_To_Slide_Start_Postion;
			}
			if (Back_Num == State_Freeze)
				states = Z_MoveDown_AvoidBack_Finished;
			if (Back_Num == Stepping)
				states = Z_MoveDown_AvoidBack;
			break;

			//******运动到扫描的第一个点位置******//
		case Move_To_Slide_Start_Postion:
			m_X_Moveto = Pump_Oil_Start_X;
			m_Y_Moveto = Pump_Oil_Start_Y;
			Back_Num2 = Do_Stepping(1, m_X_Moveto);
			Back_Num2 = Do_Stepping(2, m_Y_Moveto);
			Back_Num = State_Freeze;
			Back_Num1 = State_Freeze;
			states = Move_To_Slide_Start_Postion_Finished;
			break;
		case Move_To_Slide_Start_Postion_Finished:
				
			if (Back_Num != State_Finished)
				Back_Num = Do_Stepping_Finished(1, m_X_Moveto);
			if (Back_Num1 != State_Finished)
				Back_Num1 = Do_Stepping_Finished(2, m_Y_Moveto);
			if ((Back_Num == State_Finished) && (Back_Num1 == State_Finished))
			{
				//Pump_Oil_Current_Point_Num++;
				states = Is_Scanning_Finished;
			}
					
			break;
		case Is_Scanning_Finished:

			//Pump Oil //////////////////////
			if (s_Scanning_Para.Objective_Num == Objective_100X)
			{
				if (Pump_Oil_Current_Point_Num < Pump_Oil_Num_X*Pump_Oil_Num_Y-1)
					s_Dev_Drivers->Dev_PUSIRobot_Single_Mov(5, s_Scanning_System_Para.Pump_Oil_Pulse, 600, false);
				//while (Do_Stepping(5, s_Scanning_System_Para.Pump_Oil_Pulse) != State_Finished)
				//{
				//}
				vector<bool> status_arr = { 0 };
				while (!status_arr[0])
				{
					s_Dev_Drivers->Dev_PUSIRobot_Read_Status({ 5 }, status_arr, 1);
				}
				Sleep(3000);//等待1s时间使得油滴落下
				s_Dev_Drivers->Dev_PUSIRobot_ENA(5, false);

			}
			Pump_Oil_Current_Point_Num++;
			///////////////////////////////

			Pump_Oil_Current_Scanning_Num_Y = Pump_Oil_Current_Point_Num / Pump_Oil_Num_X;
			Pump_Oil_Current_Scanning_Num_X = Pump_Oil_Current_Point_Num % Pump_Oil_Num_X;

			if (Pump_Oil_Current_Scanning_Num_Y < Pump_Oil_Num_Y)//判断当前的玻片是否Pump完成
			{
				if (Pump_Oil_Current_Scanning_Num_X == 0)//判断当前X轴方向Pump是否完成
				{
					Pump_Oil_Step_X = -Pump_Oil_Step_X;
					states = Y_Stepping;////X轴方向Pump已经完成，开始步进一个Y方向位移
				}
				else
				{
					states = X_Stepping;//X轴方向没有完成，继续转到X轴方向扫描上去
				}
			}

			else//当前玻片扫描完毕
			{					
				states = Scanning_Finished;//自动聚焦时只会聚焦一张玻片
			}

			//if ((Pump_Oil_Current_Point_Num == 0) && (Pump_Oil_Num_X > 1))
			//	states == X_Stepping;
			if (states == Y_Stepping)
				m_Y_Moveto = m_Y_Moveto + Pump_Oil_Step_Y;
			if (states == X_Stepping)
				m_X_Moveto = m_X_Moveto + Pump_Oil_Step_X;

			break;
			//*************运动到下一个点*******************//
		case X_Stepping:
			Back_Num = Do_Stepping(1, m_X_Moveto);
			states = X_Stepping_Finished;
			break;
		case X_Stepping_Finished:
			Back_Num = Do_Stepping_Finished(1, m_X_Moveto);
			if (Back_Num == State_Finished)
			{
				states = Is_Scanning_Finished;
			}
			if (Back_Num == State_Freeze)
				states = X_Stepping_Finished;
			if (Back_Num == Stepping)
				states = X_Stepping;
			break;
		case Y_Stepping:
			Back_Num = Do_Stepping(2, m_Y_Moveto);
			states = Y_Stepping_Finished;
			break;
		case Y_Stepping_Finished:
			Back_Num = Do_Stepping_Finished(2, m_Y_Moveto);
			if (Back_Num == State_Finished)
			{
				states = Is_Scanning_Finished;
			}
			if (Back_Num == State_Freeze)
				states = Y_Stepping_Finished;
			if (Back_Num == Stepping)
				states = Y_Stepping;
			break;
		default:
			break;
		}

	}
	return 1;
}
//判断两幅图像是否相同
bool Scanning_Control::Img_Same(Mat Input_Img1, Mat Input_Img2)
{
	Mat OutGray1, OutGray2;
	double scale = 0.1;
	Size outsize;
	Mat Resize;
	outsize.height = Input_Img1.rows*scale;
	outsize.width = Input_Img1.cols*scale;
	resize(Input_Img1, Resize, outsize, 0, 0, INTER_LINEAR);
	cvtColor(Resize, OutGray1, CV_BGR2GRAY);
	resize(Input_Img2, Resize, outsize, 0, 0, INTER_LINEAR);
	cvtColor(Resize, OutGray2, CV_BGR2GRAY);
	double Error_Value;
	Mat meanValueImage;
	Mat meanStdValueImage;
	Mat LapValueImage;
	Mat Img_Error = abs(Input_Img1 - Input_Img1);
	meanStdDev(Img_Error, meanValueImage, meanStdValueImage);
	Error_Value = meanValueImage.at<double>(0, 0);
	if (Error_Value < 0.01)
		return true;
	else
		return false;
}
bool Scanning_Control::Set_Camera_Paras(unsigned int Objective_Num)
{
	FLOAT fValue = 0.26;
	switch (Objective_Num)
	{
	case Objective_10X:
		fValue = 0.26;
		if (!LucamSetProperty(m_hCamera, LUCAM_PROP_EXPOSURE, fValue, 0))
		{
			AfxMessageBox(_T("Unable to set current exposure."));
			return false;
		}
		fValue = 1.6;
		if (!LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_RED, fValue, 0))
		{
			AfxMessageBox(_T("Unable to set current GAIN_RED."));
			return false;
		}
		fValue = 1.51;
		if (!LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_BLUE, fValue, 0))
		{
			AfxMessageBox(_T("Unable to set current GAIN_BLUE."));
			return false;
		}
		break;
	case Objective_40X:
		fValue = 4.0;
		if (!LucamSetProperty(m_hCamera, LUCAM_PROP_EXPOSURE, fValue, 0))
		{
			AfxMessageBox(_T("Unable to set current exposure."));
			return false;
		}
		fValue = 1.6;
		if (!LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_RED, fValue, 0))
		{
			AfxMessageBox(_T("Unable to set current GAIN_RED."));
			return false;
		}
		fValue = 1.92;
		if (!LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_BLUE, fValue, 0))
		{
			AfxMessageBox(_T("Unable to set current GAIN_BLUE."));
			return false;
		}
		break;
	case Objective_100X:
		fValue = 26;
		if (!LucamSetProperty(m_hCamera, LUCAM_PROP_EXPOSURE, fValue, 0))
		{
			AfxMessageBox(_T("Unable to set current exposure."));
			return false;
		}
		fValue = 1.6;
		if (!LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_RED, fValue, 0))
		{
			AfxMessageBox(_T("Unable to set current GAIN_RED."));
			return false;
		}
		fValue = 1.35;
		if (!LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_BLUE, fValue, 0))
		{
			AfxMessageBox(_T("Unable to set current GAIN_BLUE."));
			return false;
		}
		break;
	default:
		break;
	}
	return true;
}

int Scanning_Control::DrawCurrentPosition(S_LOCATION currrentLocation)
{
	CString windowname;
	//AfxGetApp()->m_pMainWnd->GetDlgItem(pDlg->IDC_SlideEdit[i])->SetWindowText(NULL);

	AfxGetApp()->m_pMainWnd ->GetWindowText(windowname);//修改主窗口标题
	HWND   hwnd = ::FindWindow(NULL, windowname);//调用消息处理函数刷新页面	//HWND hwnd = ::GetParent(m_hWnd);
	::SendMessage(hwnd, WM_CURRENTPOSITION, (WPARAM)&currrentLocation, NULL);
	return 0;
}