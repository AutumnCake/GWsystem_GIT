#include "stdafx.h"
#include"iostream"
#include "queue"  
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"


using namespace std;
using namespace cv;

string Int_to_String(int n)  //C++11中，该函数可以被to_string替代
{
	ostringstream stream;
	stream<<n;  //n为int类型
	return stream.str();
}