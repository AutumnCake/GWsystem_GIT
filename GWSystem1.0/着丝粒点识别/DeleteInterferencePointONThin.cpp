#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

#include"CR.h"

void DeleteInterferencePointONThin(Mat& srcimage)
{
	for (int i=0;i<srcimage.rows;i++) //第i行
	{
		for(int j=0;j<srcimage.cols;j++)//第j列
		{
			if(srcimage.at<uchar>(i,j)>0)
			{
				vector<Point> p1=CR(srcimage,Point(j,i),1);
				vector<Point> p1a=CRA(srcimage,Point(j,i),1);
				int x=j;
				int y=i;
				if(p1a.size()>p1.size())
				{
					for(int plaN=0;plaN<p1a.size();plaN++)
					{
						int X=abs(p1a[plaN].x-x);
						int Y=abs(p1a[plaN].y-y);
						//cout<<X<<"\t"<<Y<<"\n";
						if(!(X*Y) )
						{
							if(X==0)
							{
								uchar F=(p1a[plaN].x-1>=0&&p1a[plaN].x-1<srcimage.size().width)? srcimage.at<uchar>(p1a[plaN].y,p1a[plaN].x-1):0;
								uchar T=1;
								uchar B=(p1a[plaN].x+1>=0&&p1a[plaN].x+1<srcimage.size().width)? srcimage.at<uchar>(p1a[plaN].y,p1a[plaN].x+1):0;
								bool IFremove=(F*T||T*B);
								if(IFremove) srcimage.at<uchar>(p1a[plaN].y,p1a[plaN].x)=0;
							}
							else
							{
								uchar F=(p1a[plaN].y-1>=0&&p1a[plaN].y-1<srcimage.size().height)? srcimage.at<uchar>(p1a[plaN].y-1,p1a[plaN].x):0;
								uchar T=1;
								uchar B=(p1a[plaN].y+1>=0&&p1a[plaN].y+1<srcimage.size().height)? srcimage.at<uchar>(p1a[plaN].y+1,p1a[plaN].x):0;
								bool IFremove=(F*T||T*B);
								if(IFremove) srcimage.at<uchar>(p1a[plaN].y,p1a[plaN].x)=0;
							}
						}
					}
				}
				
			}
	    }
    }
}