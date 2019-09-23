#include "stdafx.h"
#include"iostream"
#include "queue"  
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include <opencv2/video/video.hpp>

using namespace std;
using namespace cv;

#define PI 3.1415926

/*
 Originalimage可以是RGB图，灰度图，以及二值图；
 空白Mat类型Labelimage；
 Zonenum初始化为0，用于记录联通区域数。
 */
void D8ConnectedComponent(Mat InputOriginalimage, Mat& Labelimage,int& Zonenum)  
{ 
	
	Mat Seedimage=InputOriginalimage.clone();
	if(InputOriginalimage.type() != CV_8UC1)  cvtColor(Seedimage,Seedimage,CV_RGB2GRAY);//如果是彩图，转化为灰度图
	threshold(Seedimage,Seedimage,1,255,THRESH_BINARY);//Seedimage初始必须为二值图，即仅仅含有0和255
	Mat Originalimage=Seedimage.clone();
	Labelimage=Mat::zeros(Originalimage.size(),CV_8U);
	int row=Originalimage.rows;
	int col=Originalimage.cols;

	 //标记区域标识号，从1开始  
    int Num = 0;  
    //临时种子队列  
    queue<Point>quetem;  //FIFO，单向队列

  
    Point temp=Point(0,0);

    for (int i = 0; i < row; i++)  
    {  
        for (int j = 0; j < col; j++)  
        {  
            Labelimage.at<uchar>(i,j) = 0;  //Labelimage清零
        }  
    }  
  
  
    int m,n,k = 0;  
    bool up,down,right,left,upleft,upright,downleft,downright;//8 directions  （八邻域）
    //初始化每个种子队列  
    //种子是指标记区域边缘的点，他们可以在水平上升时向外淹没（或者说生长）  
    for (int i = 0; i < row; i++)  
    {  
        for (int j = 0; j < col; j++)  
        {  
            //如果找到一个标记区域  
            if (Seedimage.at<uchar>(i,j) == 255) 
            {  
                //区域的标示号加一  初始为0,第一个为1
                Num++;  
                temp.x = i;  
                temp.y = j;  
                quetem.push(temp);  
  
                //当前点标记为已处理  
                Labelimage.at<uchar>(i,j) = Num; 
				/*cout<< (int)Labelimage.at<uchar>(i,j)<<"df\n";*/
                Seedimage.at<uchar>(i,j) = 127;//表示已经处理过  Seedimage最初只有0和255,127仅仅标示已经处理过的点，任何非0和255的整数都能标示
                while(!quetem.empty())  
                {  
					/*cout<<"大傻逼\n";*/
                    up = down = right = left = false;  
                    upleft = upright = downleft = downright = false;  //false表示可生长，true表示不可生长
                    //队列中取出一个种子  
                    temp = quetem.front();  
                    m = temp.x; //row 
                    n = temp.y;  //col
                    quetem.pop();  
                    //注意到 127 对扫描过程的影响，影响下面的比较，但是不影响 while 语句中的扫描  
  
                    if (m > 0)  
                    {  
                        if (Seedimage.at<uchar>(m-1,n) == 255)  
                        {  
                            temp.x = m - 1;  
                            temp.y = n;  
                            quetem.push(temp);//如果这样的话，那么这些标记过的区域将再次在 while 循环中被扫描到，不会，因为值是 127  而影响扫描
                            //新种子点标记为已淹没区域，而且是当前区域，并记录区域号到 labelImage  
                            Labelimage.at<uchar>(m-1,n) = Num;  
                            Seedimage.at<uchar>(m-1,n) = 127;  
                        }  
                        else//否则上方为不可生长  
                        {  
                            up = true;  
                        }  
                    }  
  
                    if (m >0 && n > 0)  
                    {  
                        if (Seedimage.at<uchar>(m-1,n-1) == 255)//左上方若为可生长点则加为新种子  
                        {  
                            temp.x = m - 1;  
                            temp.y = n - 1;  
                            quetem.push(temp);  
                            //新种子点标记为已淹没区域，即下一个循环中以 127 来标识不再扫描，而且是当前区域  
                            Labelimage.at<uchar>(m-1,n-1) = Num;  
                            Seedimage.at<uchar>(m-1,n-1) = 127;  
                        }  
                        else//否则左上方为不可生长  
                        {  
                            upleft = true;  
                        }  
                    }  
  
                    if(m<row-1)  
                    {  
                        if(Seedimage.at<uchar>(m+1,n)==255)//下方若为可生长点则加为新种子  
                        {  
                            temp.x=m+1;  
                            temp.y=n;  
                            quetem.push(temp);  
                            //新种子点标记为已淹没区域，而且是当前区域  
                            Labelimage.at<uchar>(m+1,n)=Num;  
                            Seedimage.at<uchar>(m+1,n)=127;  
                        }  
                        else//否则下方为不可生长  
                        {  
                            down=true;  
                        }  
                    }  
  
                    if (m < (row - 1) && n < (col - 1))  
                    {  
                        if(Seedimage.at<uchar>(m+1,n+1)==255)//右下方若为可生长点则加为新种子  
                        {  
                            temp.x=m+1;  
                            temp.y=n+1;  
                            quetem.push(temp);  
                            //新种子点标记为已淹没区域，而且是当前区域  
                            Labelimage.at<uchar>(m+1,n+1)=Num;  
                            Seedimage.at<uchar>(m+1,n+1)=127;  
                        }  
                        else//否则下方为不可生长  
                        {  
                            downright=true;  
                        }  
                    }  
  
                    if(n<col-1)  
                    {  
                        if(Seedimage.at<uchar>(m,n+1)==255)//右方若为可生长点则加为新种子  
                        {  
                            temp.x=m;  
                            temp.y=n+1;  
                            quetem.push(temp);  
                            //新种子点标记为已淹没区域，而且是当前区域  
                            Labelimage.at<uchar>(m,n+1)=Num;  
                            Seedimage.at<uchar>(m,n+1)=127;  
                        }  
                        else//否则右方为不可生长  
                        {  
                            right=true;  
                        }  
                    }  
  
                    if(m>0&&n<(col-1))  
                    {  
                        if(Seedimage.at<uchar>(m-1,n+1)==255)//右上方若为可生长点则加为新种子  
                        {  
                            temp.x=m-1;  
                            temp.y=n+1;  
                            quetem.push(temp);  
                            //新种子点标记为已淹没区域，而且是当前区域  
                            Labelimage.at<uchar>(m-1,n+1)=Num;  
                            Seedimage.at<uchar>(m-1,n+1)=127;  
                        }  
                        else//否则右上方为不可生长  
                        {  
                            upright = true;  
                        }  
                    }  
  
                    if(n>0)  
                    {  
                        if(Seedimage.at<uchar>(m,n-1)==255)//左方若为可生长点则加为新种子  
                        {  
                            temp.x=m;  
                            temp.y=n-1;  
                            quetem.push(temp);  
                            //新种子点标记为已淹没区域  
                            Labelimage.at<uchar>(m,n-1)=Num;  
                            Seedimage.at<uchar>(m,n-1)=127;  
                        }  
                        else//否则左方为不可生长  
                        {  
                            left=true;  
                        }  
                    }  
  
                    if(m<(row-1)&&n>0)  
                    {  
                        if(Seedimage.at<uchar>(m+1,n-1)==255)//左下方若为可生长点则加为新种子  
                        {  
                            temp.x=m+1;  
                            temp.y=n-1;  
                            quetem.push(temp);  
                            //新种子点标记为已淹没区域  
                            Labelimage.at<uchar>(m+1,n-1)=Num;  
                            Seedimage.at<uchar>(m+1,n-1)=127;  
                        }  
                        else//否则左方为不可生长  
                        {  
                            downleft=true;  
                        }  
                    }  
                }//while 结束，扫描到 quetem 为空而止。也就是对应所有的节点都得到不可生长为止（或者是周围的点要么不可生长，要么已生长）  
            }//if 结束  
        }  
    } 
	Zonenum=Num;
}