//��ͨ������ֵ�����ͼ�� �Ǽܻ���ϸ����
//ֻ�ܴ���8ͨ����ֵͼ
//����ֵΪMat���ͣ���ͼ����Ϣ
//�ο�����A Fast Parallel Algorithm for Thinning Digital Patterns
//��ԭ���ǼܵĻ����Ͻ�һ����ʴ����ʴ��8-������ֻ���������ڵ㣬���������Point2Vector��ʹ��8-�������ͻ���ó����Ժ�ȫ������8-���򷽷�
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
	Mat resimage = srcimage.clone();//��ֹӰ��srcimage������ֱ����srcimage�ϲ�����������resimage�в�������󷵻�resimage��Mat��
	 //if(resimage.type()!=CV_8UC1)//�����ж������ͼ����Ϣ�Ƿ�Ϊ��ͨ��8bits Unsignedͼ��(���Ҷ�ͼ���ֵͼ)
     //      {
	 //	cout<<"ֻ�ܴ����ֵ��Ҷ�ͼ��\n";
	 //	return resimage;
     //      }
    vector<Point> deletelist1; //���ڴ洢��һ������ɸѡ���Ĵ�ɾ���ĵ������ 
	vector<Point> deletelist2;//���ڴ洢�ڶ�������ɸѡ���Ĵ�ɾ���ĵ������ 
	int Zhangmude[9];   
    int nl = resimage.rows; //rows �C Number of rows in a 2D array.��
    int nc = resimage.cols; //cols ��Number of columns in a 2D array�� 
    while (true)
    {  
        for (int j = 1; j<(nl - 1); j++)   //֮���Դ�1��ʼȡ���Լ���β��1���Ƿ��㴦��8���򣬷�ֹԽ�硣��ͬ
        {  
            uchar* data_last = resimage.ptr<uchar>(j - 1); //j=1ʱ�� uchar* data_last = resimage.ptr<uchar>(0);   ��ȡ��0�е�ָ��
            uchar* data = resimage.ptr<uchar>(j);  //��ȡj�е�ָ�룬��ͨ��[i]����j�е�i��Ԫ�أ���data[i]��
            uchar* data_next = resimage.ptr<uchar>(j + 1);  
            for (int i = 1; i<(nc - 1); i++)  
            {  
                if (data[i] == 255)  //data[1]�����Ǵ�data[0]��ʼ����ֹ8����Խ��
                {  
                    Zhangmude[0] = 1;  //P1 ��0��255ת��Ϊ0��1.
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
                    for (int k = 1; k < 9; k++)  //whitepointtotal=p2+p3+p4����+p9
                    {  
                        whitepointtotal = whitepointtotal + Zhangmude[k];  
                    }  
                    if ((whitepointtotal >= 2) && (whitepointtotal <= 6))  //����1��ͬʱ��������1-4����ɾ���õ����
                    {  
                        int ap = 0;  //p2-p9��01ģʽ������
                        if ((Zhangmude[1] == 0) && (Zhangmude[2] == 1)) ap++;  
                        if ((Zhangmude[2] == 0) && (Zhangmude[3] == 1)) ap++;  
                        if ((Zhangmude[3] == 0) && (Zhangmude[4] == 1)) ap++;  
                        if ((Zhangmude[4] == 0) && (Zhangmude[5] == 1)) ap++;  
                        if ((Zhangmude[5] == 0) && (Zhangmude[6] == 1)) ap++;  
                        if ((Zhangmude[6] == 0) && (Zhangmude[7] == 1)) ap++;  
                        if ((Zhangmude[7] == 0) && (Zhangmude[8] == 1)) ap++;  
                        if ((Zhangmude[8] == 0) && (Zhangmude[1] == 1)) ap++;  
                        if (ap == 1)  //����2
                        {  
                            if ((Zhangmude[1] * Zhangmude[3] * Zhangmude[5] == 0) && (Zhangmude[3] * Zhangmude[5] * Zhangmude[7] == 0)) 
								//����3������4 removes only the south-east boundary points and the north-west corner points
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
            uchar* data = resimage.ptr<uchar>(tem.y);  //tem.y��
            data[tem.x] = 0; //(x,y)��ֵ��Ϊ0 
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
	//��ʱ������Ǽܷ�λ�����Ѷ�����Ҫ��һ����ʴ�Ա���������Ǽܵ�8-������
	  for (int j = 1; j<(nl - 1); j++)   //֮���Դ�1��ʼȡ���Լ���β��1���Ƿ��㴦��8���򣬷�ֹԽ�硣��ͬ
        {  
            uchar* data_last = resimage.ptr<uchar>(j - 1); //j=1ʱ�� uchar* data_last = resimage.ptr<uchar>(0);   ��ȡ��0�е�ָ��
            uchar* data = resimage.ptr<uchar>(j);  //��ȡj�е�ָ�룬��ͨ��[i]����j�е�i��Ԫ�أ���data[i]��
            uchar* data_next = resimage.ptr<uchar>(j + 1);  
            for (int i = 1; i<(nc - 1); i++)  
            {  
                if (data[i] == 255)  //data[1]�����Ǵ�data[0]��ʼ����ֹ8����Խ��
                {  
                    Zhangmude[0] = 1;  //P1 ��0��255ת��Ϊ0��1.
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
                    for (int k = 1; k < 9; k++)  //whitepointtotal=p2+p3+p4����+p9
                    {  
                        whitepointtotal = whitepointtotal + Zhangmude[k];  
                    }  
                    if (Zhangmude[1] * Zhangmude[3]==11||Zhangmude[3] * Zhangmude[5]==1||Zhangmude[5] * Zhangmude[7]==1||Zhangmude[7] * Zhangmude[1]==1)  //����1��ͬʱ��������1-4����ɾ���õ����
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