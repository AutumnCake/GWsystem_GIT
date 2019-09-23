#include "stdafx.h"
#include"iostream"
#include "queue"  
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include <opencv2/video/video.hpp>

#include"ClassClusterInfo.h"


using namespace std;
using namespace cv;

bool lessX(Point a,Point b)
{
	return a.x<b.x;
}
bool lessY(Point a,Point b)
{
	return a.y<b.y;
}
bool greaterX(Point a,Point b)
{
	return a.x>b.x;
}


bool SortClassClusterInfoLess_S( ClassClusterInfo p1,  ClassClusterInfo  p2)   
{  
	return p1.GetminRectShort() < p2.GetminRectShort();  
} 
bool SortClassClusterInfoGreater_S( ClassClusterInfo p1,  ClassClusterInfo  p2)   
{  
    return p1.GetminRectShort() > p2.GetminRectShort();  
} 


bool SortClassClusterInfoLess_L( ClassClusterInfo p1,  ClassClusterInfo  p2)   
{  
	return p1.GetminRectLong() < p2.GetminRectLong();  
} 
bool SortClassClusterInfoGreater_L( ClassClusterInfo p1,  ClassClusterInfo  p2)   
{  
    return p1.GetminRectLong() > p2.GetminRectLong();  
} 


bool SortClassClusterInfoLess_SL( ClassClusterInfo p1,  ClassClusterInfo  p2)   
{  
	return p1.GetminRectShort()+p1.GetminRectLong() < p2.GetminRectShort()+p2.GetminRectLong();  
} 
bool SortClassClusterInfoGreater_SL( ClassClusterInfo p1,  ClassClusterInfo  p2)   
{  
    return  p1.GetminRectShort()+p1.GetminRectLong() > p2.GetminRectShort()+p2.GetminRectLong();  
} 