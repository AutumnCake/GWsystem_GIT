/*马腾飞 2018.4.19修改,改掉死循环问题

*/


#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

void Point2Vector(Mat thinImage,vector<Point>&ThinPoints,Point P)
{
	ThinPoints.push_back(P);
	//threshold(thinImage,thinImage,1,255,THRESH_BINARY);
	//bool up, down, right,left,upleft,upright,downleft,downright;
 //   up = down = right = left = false;  
 //   upleft = upright = downleft = downright = false;  //false表示不存在下一点，true表示存在下一点
 //   //取上一个存进ThinPoints的点  
 //   Point temp = ThinPoints[ThinPoints.size()-1];  
 //   int m = temp.x; //row 
 //   int n = temp.y;  //col
	//cout<<m<<" "<<n<<"\n";
 //   if (m > 0)  
 //   {  
	//	if (thinImage.at<uchar>(n,m-1)==255 )  
	//	{  
	//		temp.x = m - 1;  
	//		temp.y = n;  
	//		ThinPoints.push_back(temp);// 
	//		left =true;
	//	}  
 //   }  
 //   if (m >0 && n > 0)  
 //   {  
 //       if (thinImage.at<uchar>(n-1,m-1) == 255)//  
 //       {  
 //           temp.x = m - 1;  
 //           temp.y = n - 1;  
 //           ThinPoints.push_back(temp);  
	//		upleft = true;
 //       }  
 //   }  
 // 
 //   if(m<thinImage.cols-1)  
 //   {  
 //       if(thinImage.at<uchar>(n,m+1)==255)//
 //       {  
 //           temp.x=m+1;  
 //           temp.y=n;  
 //           ThinPoints.push_back(temp);  
	//		right=true;
 //       }  
 //   }  
 // 
 //   if (m < (thinImage.cols - 1) && n < (thinImage.rows - 1))  
 //   {  
 //       if(thinImage.at<uchar>(n+1,m+1)==255)//  
 //       {  
 //           temp.x=m+1;  
 //           temp.y=n+1;  
 //           ThinPoints.push_back(temp);  
	//		downright=true;
 //              
 //       }  
 //   }  
 // 
 //   if(n<thinImage.rows-1)  
 //   {  
 //       if(thinImage.at<uchar>(n+1,m)==255)//
 //       {  
 //           temp.x=m;  
 //           temp.y=n+1;  
 //           ThinPoints.push_back(temp);  
	//		down=true;
 //               
 //       }  
 //   }  
 // 
 //   if(m>0&&n<(thinImage.rows-1))  
 //   {  
 //       if(thinImage.at<uchar>(n+1,m-1)==255)//  
 //       {  
 //           temp.x=m-1;  
 //           temp.y=n+1;  
 //           ThinPoints.push_back(temp); 
	//		downleft = true;
 //       }  
 //   }  
 // 
 //   if(n>0)  
 //   {  
 //       if(thinImage.at<uchar>(n-1,m)==255)// 
 //       {  
 //           temp.x=m;  
 //           temp.y=n-1;  
 //           ThinPoints.push_back(temp); 
	//		up=true;
 //       }  
 //   }  
 // 
 //   if(m<(thinImage.cols-1)&&n>0)  
 //   {  
 //       if(thinImage.at<uchar>(n-1,m+1)==255)//  
 //       {  
 //           temp.x=m+1;  
 //           temp.y=n-1;  
 //           ThinPoints.push_back(temp); 
	//		upright=true;
 //       }  
 //   }  
	//bool IfNextPoint=(up || down || right || left || upleft || downleft || upright || downright);

	bool IfNextPoint=true;
	bool up, down, right,left,upleft,upright,downleft,downright;
	Point temp(0,0);
	int m = 0; //row 
    int n = 0;  //col
	int EX=0;
	int EY=0;

	while(IfNextPoint)  
    {
		up = down = right = left = false;  
        upleft = upright = downleft = downright = false;  //false表示不存在下一点，true表示存在下一点
        //取上一个存进ThinPoints的点  
        temp = ThinPoints[ThinPoints.size()-1];  
        m = temp.x; //row 
        n = temp.y;  //col
		//int EX=ThinPoints[ThinPoints.size()-2].x;
		//int EY=ThinPoints[ThinPoints.size()-2].y;
		/*cout<<"大傻逼\n";*/
		if (m > 0)  
		{  
			if (thinImage.at<uchar>(n,m-1) == 255) 
			{
				if(!((m-1==EX)&&(n==EY)))
				{
					temp.x = m - 1;  
					temp.y = n;  
					ThinPoints.push_back(temp);// 
					left =true;

					m=temp.x;n=temp.y;
					EX=ThinPoints[ThinPoints.size()-2].x;
		            EY=ThinPoints[ThinPoints.size()-2].y;

				}
					
			}  
		}  
		if (m >0 && n > 0)  
		{  
			if (thinImage.at<uchar>(n-1,m-1) == 255)//  
			{ 
				if(!((m-1==EX)&&(n-1==EY)))
				{
					temp.x = m - 1;  
					temp.y = n - 1;  
					ThinPoints.push_back(temp);  
					upleft = true;

					m=temp.x;n=temp.y;
					EX=ThinPoints[ThinPoints.size()-2].x;
		            EY=ThinPoints[ThinPoints.size()-2].y;

				}
					
			}  
		}  
  
		if(m<thinImage.cols-1)  
		{  
			if(thinImage.at<uchar>(n,m+1)==255)//
			{ 
				if(!((m+1==EX)&&(n==EY)))
				{
					temp.x=m+1;  
					temp.y=n;  
					ThinPoints.push_back(temp);  
					right=true;

					m=temp.x;n=temp.y;
					EX=ThinPoints[ThinPoints.size()-2].x;
		            EY=ThinPoints[ThinPoints.size()-2].y;

				}
			}  
		}  
  
		if (m < (thinImage.cols - 1) && n < (thinImage.rows - 1))  
		{  
			if(thinImage.at<uchar>(n+1,m+1)==255)//  
			{ 
				if(!((m+1==EX)&&(n+1==EY)))
				{
					temp.x=m+1;  
					temp.y=n+1;  
					ThinPoints.push_back(temp);  
					downright=true;

					m=temp.x;n=temp.y;
					EX=ThinPoints[ThinPoints.size()-2].x;
		            EY=ThinPoints[ThinPoints.size()-2].y;

				}
			}  
		}  
  
		if(n<thinImage.rows-1)  
		{  
			if(thinImage.at<uchar>(n+1,m)==255)//
			{
				if(!((m==EX)&&(n+1==EY)))
				{
					temp.x=m;  
					temp.y=n+1;  
					ThinPoints.push_back(temp);  
					down=true;

					m=temp.x;n=temp.y;
					EX=ThinPoints[ThinPoints.size()-2].x;
		            EY=ThinPoints[ThinPoints.size()-2].y;

				}
               
			}  
		}  
  
		if(m>0&&n<(thinImage.rows-1))  
		{  
			if(thinImage.at<uchar>(n+1,m-1)==255)//  
			{ 
				if(!((m-1==EX)&&(n+1==EY)))
				{
					temp.x=m-1;  
					temp.y=n+1;  
					ThinPoints.push_back(temp);  
					downleft=true;

					m=temp.x;n=temp.y;
					EX=ThinPoints[ThinPoints.size()-2].x;
		            EY=ThinPoints[ThinPoints.size()-2].y;

				}
			}  
		}  
  
		if(n>0)  
		{  
			if(thinImage.at<uchar>(n-1,m)==255)// 
			{ 
				if(!((m==EX)&&(n-1==EY)))
				{
					temp.x=m;  
					temp.y=n-1;  
					ThinPoints.push_back(temp);  
					up=true;

					m=temp.x;n=temp.y;
					EX=ThinPoints[ThinPoints.size()-2].x;
		            EY=ThinPoints[ThinPoints.size()-2].y;


				}
			}  
		}  
  
		if(m<(thinImage.cols-1)&&n>0)  
		{  
			if(thinImage.at<uchar>(n-1,m+1)==255)//  
			{  
				if(!((m+1==EX)&&(n-1==EY)))
				{
					temp.x=m+1;  
					temp.y=n-1;  
					ThinPoints.push_back(temp);  
					upright=true;

					m=temp.x;n=temp.y;
					EX=ThinPoints[ThinPoints.size()-2].x;
		            EY=ThinPoints[ThinPoints.size()-2].y;

				}
			}  
		}
		IfNextPoint=(up || down || right || left || upleft || downleft || upright || downright);

	}//while 结束
	return;
}
//重载函数，仅仅比void Point2Vector(Mat thinImage,vector<Point>& ThinPoints,Point P)多了一个寻找端点P的步骤，其余全部相同
void Point2Vector(Mat thinImage,vector<Point>& ThinPoints)
{
	//在thinImage上寻找端点P
	Point  P(-1,-1);
	int height=thinImage.rows;
	int width=thinImage.cols;
	for (int i=0;i<height;i++) //第i行
	{
		uchar *p=thinImage.ptr<uchar>(i);
		for(int j=0;j<width;j++)//第j列
		{
			uchar p1=p[j];
			if (p1==0) continue;//直到p1==1*255
			uchar p2= (i==0)? 0 : *(p - thinImage.step+j);
			uchar p3=(i==0||j==width-1)? 0:*(p - thinImage.step+j+1);
			uchar p4= (j==width-1)? 0: *(p+j+1);
			uchar p5=(i == height - 1 ||j==width-1)?0:*(p+thinImage.step+j+1);
			uchar p6=(i==height-1)?0:*(p+thinImage.step+j);
			uchar p7=(i==height-1||j==0)?0:*(p+thinImage.step+j-1);
			uchar p8=(j==0)?0:*(p+j-1);
			uchar p9=(i==0||j==0)?0:*(p-thinImage.step+j-1);
			if(p2+p3+p4+p5+p6+p7+p8+p9==1*255 )
			{
				P=Point(j, i);//i行，j列
				break;
			}

		}
		if(P.x!=-1 || P.y!=-1) break;
	}
	//cout<<P<<"\n";
	if(P.x==-1 || P.y==-1) return ;  
	//与void Point2Vector(Mat thinImage,vector<Point>& ThinPoints,Point P)相同
	ThinPoints.push_back(P);
	//threshold(thinImage,thinImage,1,255,THRESH_BINARY);
	//bool up, down, right,left,upleft,upright,downleft,downright;
 //   //up = down = right = left = false;  
 //   //upleft = upright = downleft = downright = false;  //false表示不存在下一点，true表示存在下一点
 //   //取上一个存进ThinPoints的点 
	//int EX=0;
	//int EY=0;
 //   Point temp = ThinPoints[ThinPoints.size()-1];  
 //   int m = temp.x; //col
 //   int n = temp.y;  //row
	////cout<<m<<" "<<n<<"\n";
 //   if (m > 0)  
 //   {  
	//	if (thinImage.at<uchar>(n,m-1)==255 )  
	//	{  
	//		temp.x = m - 1;  
	//		temp.y = n;  
	//		ThinPoints.push_back(temp);// 
	//		left =true;
	//		EX=ThinPoints[ThinPoints.size()-2].x;
	//	    EY=ThinPoints[ThinPoints.size()-2].y;
	//	}  
 //   }  
 //   if (m >0 && n > 0)  
 //   {  
 //       if (thinImage.at<uchar>(n-1,m-1) == 255)//  
 //       {  
 //           temp.x = m - 1;  
 //           temp.y = n - 1;  
 //           ThinPoints.push_back(temp);  
	//		upleft = true;
	//		EX=ThinPoints[ThinPoints.size()-2].x;
	//	    EY=ThinPoints[ThinPoints.size()-2].y;
 //       }  
 //   }  
 // 
 //   if(m<thinImage.cols-1)  
 //   {  
 //       if(thinImage.at<uchar>(n,m+1)==255)//
 //       {  
 //           temp.x=m+1;  
 //           temp.y=n;  
 //           ThinPoints.push_back(temp);  
	//		right=true;
	//		EX=ThinPoints[ThinPoints.size()-2].x;
	//	    EY=ThinPoints[ThinPoints.size()-2].y;
 //       }  
 //   }  
 // 
 //   if (m < (thinImage.cols - 1) && n < (thinImage.rows - 1))  
 //   {  
 //       if(thinImage.at<uchar>(n+1,m+1)==255)//  
 //       {  
 //           temp.x=m+1;  
 //           temp.y=n+1;  
 //           ThinPoints.push_back(temp);  
	//		downright=true;
	//		EX=ThinPoints[ThinPoints.size()-2].x;
	//	    EY=ThinPoints[ThinPoints.size()-2].y;
 //              
 //       }  
 //   }  
 // 
 //   if(n<thinImage.rows-1)  
 //   {  
 //       if(thinImage.at<uchar>(n+1,m)==255)//
 //       {  
 //           temp.x=m;  
 //           temp.y=n+1;  
 //           ThinPoints.push_back(temp);  
	//		down=true;
	//		EX=ThinPoints[ThinPoints.size()-2].x;
	//	    EY=ThinPoints[ThinPoints.size()-2].y;
 //               
 //       }  
 //   }  
 // 
 //   if(m>0&&n<(thinImage.rows-1))  
 //   {  
 //       if(thinImage.at<uchar>(n+1,m-1)==255)//  
 //       {  
 //           temp.x=m-1;  
 //           temp.y=n+1;  
 //           ThinPoints.push_back(temp); 
	//		downleft = true;
	//		EX=ThinPoints[ThinPoints.size()-2].x;
	//	    EY=ThinPoints[ThinPoints.size()-2].y;
 //       }  
 //   }  
 // 
 //   if(n>0)  
 //   {  
 //       if(thinImage.at<uchar>(n-1,m)==255)// 
 //       {  
 //           temp.x=m;  
 //           temp.y=n-1;  
 //           ThinPoints.push_back(temp); 
	//		up=true;
	//		EX=ThinPoints[ThinPoints.size()-2].x;
	//	    EY=ThinPoints[ThinPoints.size()-2].y;
 //       }  
 //   }  
 // 
 //   if(m<(thinImage.cols-1)&&n>0)  
 //   {  
 //       if(thinImage.at<uchar>(n-1,m+1)==255)//  
 //       {  
 //           temp.x=m+1;  
 //           temp.y=n-1;  
 //           ThinPoints.push_back(temp); 
	//		upright=true;
	//		EX=ThinPoints[ThinPoints.size()-2].x;
	//	    EY=ThinPoints[ThinPoints.size()-2].y;
 //       }  
 //   }  
	//bool IfNextPoint=(up || down || right || left || upleft || downleft || upright || downright);
    bool IfNextPoint=true;
	bool up, down, right,left,upleft,upright,downleft,downright;
	int EX=0;
	int EY=0;
	Point temp(0,0);
	int m=0,n=0;

	temp = ThinPoints[ThinPoints.size()-1];  
    m = temp.x; //row 
    n = temp.y;  //col

	 //cout<<"大傻逼\n";
	while(IfNextPoint)  
    {
		up = down = right = left = false;  
        upleft = upright = downleft = downright = false;  //false表示不存在下一点，true表示存在下一点
        //取上一个存进ThinPoints的点  
		//EX=ThinPoints[ThinPoints.size()-2].x;
		//EY=ThinPoints[ThinPoints.size()-2].y;
		/*cout<<"大傻逼\n";*/
		if (m > 0)  
		{  
			if (thinImage.at<uchar>(n,m-1) == 255) 
			{
				if(!((m-1==EX)&&(n==EY)))
				{
					temp.x = m - 1;  
					temp.y = n;  
					ThinPoints.push_back(temp);// 
					left =true;

				    m=temp.x;n=temp.y;
					EX=ThinPoints[ThinPoints.size()-2].x;
		            EY=ThinPoints[ThinPoints.size()-2].y;
					//continue;
					//cout<<"left\n";
				}
					
			}  
		}  
		if (m >0 && n > 0)  
		{  
			if (thinImage.at<uchar>(n-1,m-1) == 255)//  
			{ 
				if(!((m-1==EX)&&(n-1==EY)))
				{
					temp.x = m - 1;  
					temp.y = n - 1;  
					ThinPoints.push_back(temp);  
					upleft = true;

					m=temp.x;n=temp.y;
					EX=ThinPoints[ThinPoints.size()-2].x;
		            EY=ThinPoints[ThinPoints.size()-2].y;
					//continue;
					//cout<<"upleft\n";
				}
					
			}  
		}  
  
		if(m<thinImage.cols-1)  
		{  
			if(thinImage.at<uchar>(n,m+1)==255)//
			{ 
				if(!((m+1==EX)&&(n==EY)))
				{
					temp.x=m+1;  
					temp.y=n;  
					ThinPoints.push_back(temp);  
					right=true;

					m=temp.x;n=temp.y;
					EX=ThinPoints[ThinPoints.size()-2].x;
		            EY=ThinPoints[ThinPoints.size()-2].y;
					//continue;
					//cout<<"right\n";
				}
			}  
		}  
  
		if (m < (thinImage.cols - 1) && n < (thinImage.rows - 1))  
		{  
			if(thinImage.at<uchar>(n+1,m+1)==255)//  
			{ 
				if(!((m+1==EX)&&(n+1==EY)))
				{
					temp.x=m+1;  
					temp.y=n+1;  
					ThinPoints.push_back(temp);  
					downright=true;

					m=temp.x;n=temp.y;
				    EX=ThinPoints[ThinPoints.size()-2].x;
		            EY=ThinPoints[ThinPoints.size()-2].y;
					//continue;
					//cout<<"downright\n";
				}
			}  
		}  
  
		if(n<thinImage.rows-1)  
		{  
			if(thinImage.at<uchar>(n+1,m)==255)//
			{
				if(!((m==EX)&&(n+1==EY)))
				{
					temp.x=m;  
					temp.y=n+1;  
					ThinPoints.push_back(temp);  
					down=true;

					m=temp.x;n=temp.y;
					EX=ThinPoints[ThinPoints.size()-2].x;
		            EY=ThinPoints[ThinPoints.size()-2].y;
					//continue;
					//cout<<"down\n";
				}
               
			}  
		}  
  
		if(m>0&&n<(thinImage.rows-1))  
		{  
			if(thinImage.at<uchar>(n+1,m-1)==255)//  
			{ 
				if(!((m-1==EX)&&(n+1==EY)))
				{
					temp.x=m-1;  
					temp.y=n+1;  
					ThinPoints.push_back(temp);  
					downleft=true;

					m=temp.x;n=temp.y;
					EX=ThinPoints[ThinPoints.size()-2].x;
		            EY=ThinPoints[ThinPoints.size()-2].y;
					//continue;
					//cout<<"downleft\n";
				}
			}  
		}  
  
		if(n>0)  
		{  
			if(thinImage.at<uchar>(n-1,m)==255)// 
			{ 
				if(!((m==EX)&&(n-1==EY)))
				{
					temp.x=m;  
					temp.y=n-1;  
					ThinPoints.push_back(temp);  
					up=true;

					m=temp.x;n=temp.y;
					EX=ThinPoints[ThinPoints.size()-2].x;
		            EY=ThinPoints[ThinPoints.size()-2].y;
					//continue;
					//cout<<"up\n";
					////cout<<temp<<"\n";

				}
			}  
		}  
  
		if(m<(thinImage.cols-1)&&n>0)  
		{  
			if(thinImage.at<uchar>(n-1,m+1)==255)//  
			{  
				if(!((m+1==EX)&&(n-1==EY)))
				{
					temp.x=m+1;  
					temp.y=n-1;  
					ThinPoints.push_back(temp);  
					upright=true;

					m=temp.x;n=temp.y;
					EX=ThinPoints[ThinPoints.size()-2].x;
		            EY=ThinPoints[ThinPoints.size()-2].y;
					//continue;
					//cout<<"upright\n";
				}
			}  
		}
		//break;
		IfNextPoint=(up || down || right || left || upleft || downleft || upright || downright);

	}//while 结束
	//cout<<"大傻逼\n";
	return ;
}