//单通道、二值化后的图像 骨架化（细化）
//只能处理8通道二值图
//返回值为Mat类型，即图像信息
//参考文献A Fast Parallel Algorithm for Thinning Digital Patterns
//在原来骨架的基础上进一步腐蚀，腐蚀成8-邻域内只有两个相邻点，否则与后续Point2Vector中使用8-邻域检测冲突。该程序以后全部采用8-邻域方法
#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

Mat ThinImage(Mat srcimage) 
{  
	Mat resimage = srcimage.clone();//防止影响srcimage，即不直接在srcimage上操作，而是在resimage中操作，最后返回resimage（Mat）
	 //if(resimage.type()!=CV_8UC1)//用来判断输入的图像信息是否为单通道8bits Unsigned图像(即灰度图或二值图)
     //      {
	 //	cout<<"只能处理二值或灰度图像\n";
	 //	return resimage;
     //      }
    vector<Point> deletelist1; //用于存储第一组条件筛选出的待删除的点的坐标 
	vector<Point> deletelist2;//用于存储第二组条件筛选出的待删除的点的坐标 
	int Zhangmude[9];   
    int nl = resimage.rows; //rows – Number of rows in a 2D array.行
    int nc = resimage.cols; //cols ：Number of columns in a 2D array列 
    while (true)
    {  
        for (int j = 1; j<(nl - 1); j++)   //之所以从1开始取，以及结尾减1，是方便处理8邻域，防止越界。下同
        {  
            uchar* data_last = resimage.ptr<uchar>(j - 1); //j=1时， uchar* data_last = resimage.ptr<uchar>(0);   获取第0行的指针
            uchar* data = resimage.ptr<uchar>(j);  //获取j行的指针，并通过[i]访问j行的i列元素，如data[i]。
            uchar* data_next = resimage.ptr<uchar>(j + 1);  
            for (int i = 1; i<(nc - 1); i++)  
            {  
                if (data[i] == 255)  //data[1]而不是从data[0]开始，防止8邻域越界
                {  
                    Zhangmude[0] = 1;  //P1 将0和255转化为0和1.
                    if (data_last[i] == 255) Zhangmude[1] = 1;  //p2
                    else  Zhangmude[1] = 0;  
                    if (data_last[i + 1] == 255) Zhangmude[2] = 1; //p3 
                    else  Zhangmude[2] = 0;  
                    if (data[i + 1] == 255) Zhangmude[3] = 1;  //p4
                    else  Zhangmude[3] = 0;  
                    if (data_next[i + 1] == 255) Zhangmude[4] = 1;  //p5
                    else  Zhangmude[4] = 0;  
                    if (data_next[i] == 255) Zhangmude[5] = 1;  //p6
                    else  Zhangmude[5] = 0;  
                    if (data_next[i - 1] == 255) Zhangmude[6] = 1; //p7
                    else  Zhangmude[6] = 0;  
                    if (data[i - 1] == 255) Zhangmude[7] = 1;  //p8
                    else  Zhangmude[7] = 0;  
                    if (data_last[i - 1] == 255) Zhangmude[8] = 1;  //p9
                    else  Zhangmude[8] = 0;  
                    int whitepointtotal = 0;  
                    for (int k = 1; k < 9; k++)  //whitepointtotal=p2+p3+p4……+p9
                    {  
                        whitepointtotal = whitepointtotal + Zhangmude[k];  
                    }  
                    if ((whitepointtotal >= 2) && (whitepointtotal <= 6))  //条件1（同时满足条件1-4，则删除该点儿）
                    {  
                        int ap = 0;  //p2-p9：01模式的数量
                        if ((Zhangmude[1] == 0) && (Zhangmude[2] == 1)) ap++;  
                        if ((Zhangmude[2] == 0) && (Zhangmude[3] == 1)) ap++;  
                        if ((Zhangmude[3] == 0) && (Zhangmude[4] == 1)) ap++;  
                        if ((Zhangmude[4] == 0) && (Zhangmude[5] == 1)) ap++;  
                        if ((Zhangmude[5] == 0) && (Zhangmude[6] == 1)) ap++;  
                        if ((Zhangmude[6] == 0) && (Zhangmude[7] == 1)) ap++;  
                        if ((Zhangmude[7] == 0) && (Zhangmude[8] == 1)) ap++;  
                        if ((Zhangmude[8] == 0) && (Zhangmude[1] == 1)) ap++;  
                        if (ap == 1)  //条件2
                        {  
                            if ((Zhangmude[1] * Zhangmude[3] * Zhangmude[5] == 0) && (Zhangmude[3] * Zhangmude[5] * Zhangmude[7] == 0)) 
								//条件3、条件4 removes only the south-east boundary points and the north-west corner points
                            {  
                                deletelist1.push_back(Point(i, j));  
                            }  
                        }  
                    }  
                }  
            }  
        }  
		if (deletelist1.size() == 0 ) break;  //
        for (size_t i = 0; i < deletelist1.size(); i++)  
        {  
            Point tem;  
            tem = deletelist1[i];  
            uchar* data = resimage.ptr<uchar>(tem.y);  //tem.y行
            data[tem.x] = 0; //(x,y)处值设为0 
        }  
        deletelist1.clear();  
		
        for (int j = 1; j<(nl - 1); j++)  
        {  
            uchar* data_last = resimage.ptr<uchar>(j - 1);  
            uchar* data = resimage.ptr<uchar>(j);  
            uchar* data_next = resimage.ptr<uchar>(j + 1);  
            for (int i = 1; i<(nc - 1); i++)  
            {  
                if (data[i] == 255)  
                {  
                    Zhangmude[0] = 1;  
                    if (data_last[i] == 255) Zhangmude[1] = 1;  
                    else  Zhangmude[1] = 0;  
                    if (data_last[i + 1] == 255) Zhangmude[2] = 1;  
                    else  Zhangmude[2] = 0;  
                    if (data[i + 1] == 255) Zhangmude[3] = 1;  
                    else  Zhangmude[3] = 0;  
                    if (data_next[i + 1] == 255) Zhangmude[4] = 1;  
                    else  Zhangmude[4] = 0;  
                    if (data_next[i] == 255) Zhangmude[5] = 1;  
                    else  Zhangmude[5] = 0;  
                    if (data_next[i - 1] == 255) Zhangmude[6] = 1;  
                    else  Zhangmude[6] = 0;  
                    if (data[i - 1] == 255) Zhangmude[7] = 1;  
                    else  Zhangmude[7] = 0;  
                    if (data_last[i - 1] == 255) Zhangmude[8] = 1;  
                    else  Zhangmude[8] = 0;  
                    int whitepointtotal = 0;  
                    for (int k = 1; k < 9; k++)  
                    {  
                        whitepointtotal = whitepointtotal + Zhangmude[k];  
                    }  
                    if ((whitepointtotal >= 2) && (whitepointtotal <= 6))  
                    {  
                        int ap = 0;  
                        if ((Zhangmude[1] == 0) && (Zhangmude[2] == 1)) ap++;  
                        if ((Zhangmude[2] == 0) && (Zhangmude[3] == 1)) ap++;  
                        if ((Zhangmude[3] == 0) && (Zhangmude[4] == 1)) ap++;  
                        if ((Zhangmude[4] == 0) && (Zhangmude[5] == 1)) ap++;  
                        if ((Zhangmude[5] == 0) && (Zhangmude[6] == 1)) ap++;  
                        if ((Zhangmude[6] == 0) && (Zhangmude[7] == 1)) ap++;  
                        if ((Zhangmude[7] == 0) && (Zhangmude[8] == 1)) ap++;  
                        if ((Zhangmude[8] == 0) && (Zhangmude[1] == 1)) ap++;  
                        if (ap == 1)  
                        {  
							/*remove north-west boundary point or a south-east corner point*/
                            if ((Zhangmude[1] * Zhangmude[3] * Zhangmude[7] == 0) && (Zhangmude[1] * Zhangmude[5] * Zhangmude[7] == 0))  
                            {  
                                deletelist2.push_back(Point(i, j));  

                            }  
                        }  
                    }  
                }  
            }  
        }  
		
		if (deletelist2.size() == 0 ) break;  //
        for (size_t i = 0; i < deletelist2.size(); i++)  
        {  
            Point tem;  
            tem = deletelist2[i];  
            uchar* data = resimage.ptr<uchar>(tem.y);  
            data[tem.x] = 0;  
        }  
        deletelist2.clear();  
    } 
	//此时，总体骨架方位基本已定，需要进一步腐蚀以便满足后续骨架的8-邻域检测
	  for (int j = 1; j<(nl - 1); j++)   //之所以从1开始取，以及结尾减1，是方便处理8邻域，防止越界。下同
        {  
            uchar* data_last = resimage.ptr<uchar>(j - 1); //j=1时， uchar* data_last = resimage.ptr<uchar>(0);   获取第0行的指针
            uchar* data = resimage.ptr<uchar>(j);  //获取j行的指针，并通过[i]访问j行的i列元素，如data[i]。
            uchar* data_next = resimage.ptr<uchar>(j + 1);  
            for (int i = 1; i<(nc - 1); i++)  
            {  
                if (data[i] == 255)  //data[1]而不是从data[0]开始，防止8邻域越界
                {  
                    Zhangmude[0] = 1;  //P1 将0和255转化为0和1.
                    if (data_last[i] == 255) Zhangmude[1] = 1;  //p2
                    else  Zhangmude[1] = 0;  
                    if (data_last[i + 1] == 255) Zhangmude[2] = 1; //p3 
                    else  Zhangmude[2] = 0;  
                    if (data[i + 1] == 255) Zhangmude[3] = 1;  //p4
                    else  Zhangmude[3] = 0;  
                    if (data_next[i + 1] == 255) Zhangmude[4] = 1;  //p5
                    else  Zhangmude[4] = 0;  
                    if (data_next[i] == 255) Zhangmude[5] = 1;  //p6
                    else  Zhangmude[5] = 0;  
                    if (data_next[i - 1] == 255) Zhangmude[6] = 1; //p7
                    else  Zhangmude[6] = 0;  
                    if (data[i - 1] == 255) Zhangmude[7] = 1;  //p8
                    else  Zhangmude[7] = 0;  
                    if (data_last[i - 1] == 255) Zhangmude[8] = 1;  //p9
                    else  Zhangmude[8] = 0;  
                    int whitepointtotal = 0;  
                    for (int k = 1; k < 9; k++)  //whitepointtotal=p2+p3+p4……+p9
                    {  
                        whitepointtotal = whitepointtotal + Zhangmude[k];  
                    }  
                    if (Zhangmude[1] * Zhangmude[3]==11||Zhangmude[3] * Zhangmude[5]==1||Zhangmude[5] * Zhangmude[7]==1||Zhangmude[7] * Zhangmude[1]==1)  //条件1（同时满足条件1-4，则删除该点儿）
                    {
						resimage.at<uchar>(j,i)=0;
                      
                    }  
                }  
            }  
        }  
		
       // for (int j = 1; j<(nl - 1); j++)  
       // {  
       //     uchar* data_last = resimage.ptr<uchar>(j - 1);  
       //     uchar* data = resimage.ptr<uchar>(j);  
       //     uchar* data_next = resimage.ptr<uchar>(j + 1);  
       //     for (int i = 1; i<(nc - 1); i++)  
       //     {  
       //         if (data[i] == 255)  
       //         {  
       //             Zhangmude[0] = 1;  
       //             if (data_last[i] == 255) Zhangmude[1] = 1;  
       //             else  Zhangmude[1] = 0;  
       //             if (data_last[i + 1] == 255) Zhangmude[2] = 1;  
       //             else  Zhangmude[2] = 0;  
       //             if (data[i + 1] == 255) Zhangmude[3] = 1;  
       //             else  Zhangmude[3] = 0;  
       //             if (data_next[i + 1] == 255) Zhangmude[4] = 1;  
       //             else  Zhangmude[4] = 0;  
       //             if (data_next[i] == 255) Zhangmude[5] = 1;  
       //             else  Zhangmude[5] = 0;  
       //             if (data_next[i - 1] == 255) Zhangmude[6] = 1;  
       //             else  Zhangmude[6] = 0;  
       //             if (data[i - 1] == 255) Zhangmude[7] = 1;  
       //             else  Zhangmude[7] = 0;  
       //             if (data_last[i - 1] == 255) Zhangmude[8] = 1;  
       //             else  Zhangmude[8] = 0;  
       //             int whitepointtotal = 0;  
       //             for (int k = 1; k < 9; k++)  
       //             {  
       //                 whitepointtotal = whitepointtotal + Zhangmude[k];  
       //             }  
       //             if ((whitepointtotal >= 2) && (whitepointtotal <= 6))  
       //             {  
       //                 int ap = 0;  
       //                 if ((Zhangmude[1] == 0) && (Zhangmude[2] == 1)) ap++;  
       //                 if ((Zhangmude[2] == 0) && (Zhangmude[3] == 1)) ap++;  
       //                 if ((Zhangmude[3] == 0) && (Zhangmude[4] == 1)) ap++;  
       //                 //if ((Zhangmude[4] == 0) && (Zhangmude[5] == 1)) ap++;  
       //                 if ((Zhangmude[5] == 0) && (Zhangmude[6] == 1)) ap++;  
       //                 if ((Zhangmude[6] == 0) && (Zhangmude[7] == 1)) ap++;  
       //                 if ((Zhangmude[7] == 0) && (Zhangmude[8] == 1)) ap++;  
       //                 if ((Zhangmude[8] == 0) && (Zhangmude[1] == 1)) ap++;  
       //                 if (ap == 1)  
       //                 {  
							///*remove north-west boundary point or a south-east corner point*/
       //                     if ((Zhangmude[1] * Zhangmude[3] * Zhangmude[7] == 0) && (Zhangmude[1] * Zhangmude[5] * Zhangmude[7] == 0))  
       //                     {  
       //                         resimage.at<uchar>(j,i)=0;  

       //                     }  
       //                 }  
       //             }  
       //         }  
       //     }  
       // }  
	return resimage;
}