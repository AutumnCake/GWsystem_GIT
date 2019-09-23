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
 Originalimage������RGBͼ���Ҷ�ͼ���Լ���ֵͼ��
 �հ�Mat����Labelimage��
 Zonenum��ʼ��Ϊ0�����ڼ�¼��ͨ��������
 */
void D8ConnectedComponent(Mat InputOriginalimage, Mat& Labelimage,int& Zonenum)  
{ 
	
	Mat Seedimage=InputOriginalimage.clone();
	if(InputOriginalimage.type() != CV_8UC1)  cvtColor(Seedimage,Seedimage,CV_RGB2GRAY);//����ǲ�ͼ��ת��Ϊ�Ҷ�ͼ
	threshold(Seedimage,Seedimage,1,255,THRESH_BINARY);//Seedimage��ʼ����Ϊ��ֵͼ������������0��255
	Mat Originalimage=Seedimage.clone();
	Labelimage=Mat::zeros(Originalimage.size(),CV_8U);
	int row=Originalimage.rows;
	int col=Originalimage.cols;

	 //��������ʶ�ţ���1��ʼ  
    int Num = 0;  
    //��ʱ���Ӷ���  
    queue<Point>quetem;  //FIFO���������

  
    Point temp=Point(0,0);

    for (int i = 0; i < row; i++)  
    {  
        for (int j = 0; j < col; j++)  
        {  
            Labelimage.at<uchar>(i,j) = 0;  //Labelimage����
        }  
    }  
  
  
    int m,n,k = 0;  
    bool up,down,right,left,upleft,upright,downleft,downright;//8 directions  ��������
    //��ʼ��ÿ�����Ӷ���  
    //������ָ��������Ե�ĵ㣬���ǿ�����ˮƽ����ʱ������û������˵������  
    for (int i = 0; i < row; i++)  
    {  
        for (int j = 0; j < col; j++)  
        {  
            //����ҵ�һ���������  
            if (Seedimage.at<uchar>(i,j) == 255) 
            {  
                //����ı�ʾ�ż�һ  ��ʼΪ0,��һ��Ϊ1
                Num++;  
                temp.x = i;  
                temp.y = j;  
                quetem.push(temp);  
  
                //��ǰ����Ϊ�Ѵ���  
                Labelimage.at<uchar>(i,j) = Num; 
				/*cout<< (int)Labelimage.at<uchar>(i,j)<<"df\n";*/
                Seedimage.at<uchar>(i,j) = 127;//��ʾ�Ѿ������  Seedimage���ֻ��0��255,127������ʾ�Ѿ�������ĵ㣬�κη�0��255���������ܱ�ʾ
                while(!quetem.empty())  
                {  
					/*cout<<"��ɵ��\n";*/
                    up = down = right = left = false;  
                    upleft = upright = downleft = downright = false;  //false��ʾ��������true��ʾ��������
                    //������ȡ��һ������  
                    temp = quetem.front();  
                    m = temp.x; //row 
                    n = temp.y;  //col
                    quetem.pop();  
                    //ע�⵽ 127 ��ɨ����̵�Ӱ�죬Ӱ������ıȽϣ����ǲ�Ӱ�� while ����е�ɨ��  
  
                    if (m > 0)  
                    {  
                        if (Seedimage.at<uchar>(m-1,n) == 255)  
                        {  
                            temp.x = m - 1;  
                            temp.y = n;  
                            quetem.push(temp);//��������Ļ�����ô��Щ��ǹ��������ٴ��� while ѭ���б�ɨ�赽�����ᣬ��Ϊֵ�� 127  ��Ӱ��ɨ��
                            //�����ӵ���Ϊ����û���򣬶����ǵ�ǰ���򣬲���¼����ŵ� labelImage  
                            Labelimage.at<uchar>(m-1,n) = Num;  
                            Seedimage.at<uchar>(m-1,n) = 127;  
                        }  
                        else//�����Ϸ�Ϊ��������  
                        {  
                            up = true;  
                        }  
                    }  
  
                    if (m >0 && n > 0)  
                    {  
                        if (Seedimage.at<uchar>(m-1,n-1) == 255)//���Ϸ���Ϊ�����������Ϊ������  
                        {  
                            temp.x = m - 1;  
                            temp.y = n - 1;  
                            quetem.push(temp);  
                            //�����ӵ���Ϊ����û���򣬼���һ��ѭ������ 127 ����ʶ����ɨ�裬�����ǵ�ǰ����  
                            Labelimage.at<uchar>(m-1,n-1) = Num;  
                            Seedimage.at<uchar>(m-1,n-1) = 127;  
                        }  
                        else//�������Ϸ�Ϊ��������  
                        {  
                            upleft = true;  
                        }  
                    }  
  
                    if(m<row-1)  
                    {  
                        if(Seedimage.at<uchar>(m+1,n)==255)//�·���Ϊ�����������Ϊ������  
                        {  
                            temp.x=m+1;  
                            temp.y=n;  
                            quetem.push(temp);  
                            //�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����  
                            Labelimage.at<uchar>(m+1,n)=Num;  
                            Seedimage.at<uchar>(m+1,n)=127;  
                        }  
                        else//�����·�Ϊ��������  
                        {  
                            down=true;  
                        }  
                    }  
  
                    if (m < (row - 1) && n < (col - 1))  
                    {  
                        if(Seedimage.at<uchar>(m+1,n+1)==255)//���·���Ϊ�����������Ϊ������  
                        {  
                            temp.x=m+1;  
                            temp.y=n+1;  
                            quetem.push(temp);  
                            //�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����  
                            Labelimage.at<uchar>(m+1,n+1)=Num;  
                            Seedimage.at<uchar>(m+1,n+1)=127;  
                        }  
                        else//�����·�Ϊ��������  
                        {  
                            downright=true;  
                        }  
                    }  
  
                    if(n<col-1)  
                    {  
                        if(Seedimage.at<uchar>(m,n+1)==255)//�ҷ���Ϊ�����������Ϊ������  
                        {  
                            temp.x=m;  
                            temp.y=n+1;  
                            quetem.push(temp);  
                            //�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����  
                            Labelimage.at<uchar>(m,n+1)=Num;  
                            Seedimage.at<uchar>(m,n+1)=127;  
                        }  
                        else//�����ҷ�Ϊ��������  
                        {  
                            right=true;  
                        }  
                    }  
  
                    if(m>0&&n<(col-1))  
                    {  
                        if(Seedimage.at<uchar>(m-1,n+1)==255)//���Ϸ���Ϊ�����������Ϊ������  
                        {  
                            temp.x=m-1;  
                            temp.y=n+1;  
                            quetem.push(temp);  
                            //�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����  
                            Labelimage.at<uchar>(m-1,n+1)=Num;  
                            Seedimage.at<uchar>(m-1,n+1)=127;  
                        }  
                        else//�������Ϸ�Ϊ��������  
                        {  
                            upright = true;  
                        }  
                    }  
  
                    if(n>0)  
                    {  
                        if(Seedimage.at<uchar>(m,n-1)==255)//����Ϊ�����������Ϊ������  
                        {  
                            temp.x=m;  
                            temp.y=n-1;  
                            quetem.push(temp);  
                            //�����ӵ���Ϊ����û����  
                            Labelimage.at<uchar>(m,n-1)=Num;  
                            Seedimage.at<uchar>(m,n-1)=127;  
                        }  
                        else//������Ϊ��������  
                        {  
                            left=true;  
                        }  
                    }  
  
                    if(m<(row-1)&&n>0)  
                    {  
                        if(Seedimage.at<uchar>(m+1,n-1)==255)//���·���Ϊ�����������Ϊ������  
                        {  
                            temp.x=m+1;  
                            temp.y=n-1;  
                            quetem.push(temp);  
                            //�����ӵ���Ϊ����û����  
                            Labelimage.at<uchar>(m+1,n-1)=Num;  
                            Seedimage.at<uchar>(m+1,n-1)=127;  
                        }  
                        else//������Ϊ��������  
                        {  
                            downleft=true;  
                        }  
                    }  
                }//while ������ɨ�赽 quetem Ϊ�ն�ֹ��Ҳ���Ƕ�Ӧ���еĽڵ㶼�õ���������Ϊֹ����������Χ�ĵ�Ҫô����������Ҫô��������  
            }//if ����  
        }  
    } 
	Zonenum=Num;
}