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

string Int_to_String(int n)  //C++11�У��ú������Ա�to_string���
{
	ostringstream stream;
	stream<<n;  //nΪint����
	return stream.str();
}