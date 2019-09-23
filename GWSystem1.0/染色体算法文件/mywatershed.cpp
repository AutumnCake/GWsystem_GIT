#include "StdAfx.h"
#include"iostream"
#include "queue"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;
 /*
 Originalimage��Seedimage��Labelimageȫ��ΪCV_8U������ͨ���Ҷ�ͼ��
 ��ͼ��ߴ������ȫһ������Seedimage����Ϊ��ֵͼ��������0��255��Labelimage��ʼ��ȫ��Ϊ0.
 Originalimage��Seedimage��Labelimage
 */
void myWatershed(Mat& Originalimage, Mat& Seedimage, Mat& Labelimage,int& Zonenum)  
{ 
	if(Originalimage.rows!=Seedimage.rows||Originalimage.cols!=Seedimage.cols) 
		cout<<"error:Seedimage��Originalimageͼ��ߴ粻ͬ,����\n";
	if(Originalimage.rows!=Labelimage.rows||Originalimage.cols!=Labelimage.cols) 
		cout<<"error:Labelimage��Originalimageͼ��ߴ粻ͬ������\n";
	threshold(Seedimage,Seedimage,1,255,THRESH_BINARY);//Seedimage��ʼ����Ϊ��ֵͼ������������0��255
	int row=Originalimage.rows;
	int col=Originalimage.cols;

	 //��������ʶ�ţ���1��ʼ  
    int Num = 0;  
    /*int i,j;  */
    //����ÿ���������Ӹ���������  
    vector<int*>SeedCounts; 
	 int *array;  

    //��ʱ���Ӷ���  
    queue<Point>quetem;  //FIFO���������

    //�������б���������Ӷ��е����飬�����������Ӷ��е�ָ��  
    vector<queue<Point>*>vque; 
	//ָ�����Ӷ��е�ָ��  
    queue<Point> *pque;  
  
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
    //Ԥ������ȡ����û��������򣬲���ʼ��ÿ����ǵ����Ӷ���  
    //������ָ��������Ե�ĵ㣬���ǿ�����ˮƽ������ʱ������û������˵������  
    for (int i = 0; i < row; i++)  
    {  
        for (int j = 0; j < col; j++)  
        {  
            //����ҵ�һ���������  
            if (Seedimage.at<uchar>(i,j) == 255) 
            {  
                //����ı�ʾ�ż�һ  ��ʼΪ0,��һ��Ϊ1
                Num++;  
				/*cout<<Num<<"\n";*/
                //�������鲢��ʼ��Ϊ0����ʾ����256�ҽ�  ��0-255
                array = new int[256];  
                memset(array,256*sizeof(int),0);  
                //���Ӹ��������vector��ÿ��ɨ��������һ�����飬���������ʾ��������һά���Ҷȼ����ڶ�ά 
				// SeedCounts[m][n] ��ʾm���򣬻Ҷ�Ϊn�ĵ����Ŀ�������ڷ�
                //��ʾĳ�����������ĳ�ҽ�����Ӧ�ĵ����Ŀ  
                SeedCounts.push_back(array);  
                //���䱾��Ǻŵ����ȶ��У�256�����Ӷ��С�  
                //��ʾ��Ӧһ���ҽ���һ�����У�����ÿ�����п��Դ洢һ�����ϵĵ���Ϣ  
                pque = new queue<Point>[256];  
                //���뵽���������У���Ӧ���Ǳ���Ǻ�Num��  
				//vque[m][n]��ʾm����n�ҽ׶�Ӧ�ĵ�Ķ��С������ڷ� ������Num��ʾ
                vque.push_back(pque);  
                //��ǰ����뱾����������ʱ���Ӷ�����  
                temp.x = i;  
                temp.y = j;  
                quetem.push(temp);  
  
                //��ǰ����Ϊ�Ѵ���  
                Labelimage.at<uchar>(i,j) = Num; 
				/*cout<< (int)Labelimage.at<uchar>(i,j)<<"df\n";*/
                Seedimage.at<uchar>(i,j) = 127;//��ʾ�Ѿ������  Seedimage���ֻ��0��255,127������ʾ�Ѿ�������ĵ㣬�κη�0��255���������ܱ�ʾ
                //����ʱ���Ӷ����е����ӽ�������ֱ�����е����Ӷ��������  
                //������Ϻ�Ķ�����Ϣ������ vque �У�������Ӧ������źͻҽ���Ϣ����Ӧ�����洢�� seedcounts �� 
				//whileִ����vque�洢������������һ��ĵ����Ϣ���������Ӷ�Ӧλ��Seedimage�����Ϊ127��Labelimage�����Ϊ��Ӧ�������Num
				/*int fff=0;*/
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
                    //��������ֻҪ��һ�㲻����������ô����Ϊ��ʼ���Ӷ����е�һ��  
                    //����ɷ��������� seedimage �е�ֵ��������  
                    if (up || down || right || left || upleft || downleft || upright || downright)  
                    {  
						/*fff++;
						cout<<fff<<"||\n";*/
                        temp.x = m;  
                        temp.y = n;  
                        //�������ʸ�����飺��һά�Ǳ�Ǻţ��ڶ�ά�Ǹ�ͼ���ĻҶȼ�
                        //m,n ���Ӧ//Num �ǵ�ǰ�������  
                        //���������ά��Ϣ�ͱ�ʾ�ˣ�ĳ�������ж�Ӧĳ���Ҷȼ���Ӧ�ĳ�Ա��ļ��������  
                        //�ֱ��������������������� while ѭ������ɨ������ص㡣  
                        vque[Num-1][Originalimage.at<uchar>(m,n)].push(temp);//��Num ����Seedimage��Ե��Originalimage[m][n]�ҽ׵ĵ�ȫ������vque[Num-1][Originalimage[m][n]]�����������  
                        SeedCounts[Num-1][Originalimage.at<uchar>(m,n)]++;//��¼Num ����Seedimage��Ե��Originalimage[m][n]�ҽ׵ĵ㣬�ж��ٸ�  
                    }  
  
                }//while ������ɨ�赽 quetem Ϊ�ն�ֹ��Ҳ���Ƕ�Ӧ���еĽڵ㶼�õ���������Ϊֹ����������Χ�ĵ�Ҫô����������Ҫô��������  
            }//if ����  
        }  
    }  
	
    //�����������У������ǵĵ�Ϊ 0 ���ʾ��û��ɨ�赽�ĵ㣬���߱���������������ӵ�  
    //�����൱������ seedimage �������ĳ�ʼ����ķ�ˮ����ߵ����еĵ㣻�����ñ�ż�¼ÿ������ͬʱ��ˮ��ı�Ե�������С�  
    //�������Ҽ�ˮ��ĳ���ͬʱҲ����ͨ����  
	//cout << "����Ҽ�ˮ��\n";
    bool actives=false;//��ĳһˮλ�������б�ǵ�����������ı�־  true��ʾδ�����꣬false��ʾ��������
    int WaterLevel;  
    //��û���̿�ʼ��ˮλ���㿪ʼ������ˮλ��Ӧ�Ҷȼ�����������ͨ��  
    for (WaterLevel = 0; WaterLevel < 255; WaterLevel++)//�ڶ�ά������  ˮλ0��255 ��Ӧ�Ҷȴ�0��255
    {  
        actives = true;  
        while(actives)  
        {  
            actives=false;  
            //���δ���ÿ����Ǻ�����Ӧ�������������ǺŶ�Ӧ������ĵ�ĸ����� SeedCounts ����  
            for (int i = 0; i < Num; i++)//��һά������[WaterLevel][Num]  
            {   //�Ƿ�ifӦ��Ϊwhile��Ŀǰif�ƺ�ֻ�ܱ�֤Ŀǰˮλ����������һ�Σ���û�б�֤���д�ˮλ������λ�ñ���û�����ڷɣ�
				/*
				�˴�ֻ����if��Ϊifʱ�ܱ�֤ÿ����������������������������Ч�ָ
				��Ϊwhile������������ǰ��Ĵ�����������ռ����������ռ䣻
				�����м�ˮ���Ӧ��vque[i][WaterLevel].empty()ȫ��Ϊ��ʱ������ȫ��������ȫ����������򣨷ָ���
				*/
                if(!vque[i][WaterLevel].empty())//��Ӧ�ķ�ˮ�벻Ϊ�ռ�,i+1 ��ʾ����ţ�waterlevel ��ʾ�ҽ�  
                {  
                    actives = true;  
                    while(SeedCounts[i][WaterLevel]>0)  
                    {
						if (vque[i][WaterLevel].size() <= 0) break; //mtf 20190529 ����Ī���������
						//cout << "SeedCounts[i][WaterLevel]:" << SeedCounts[i][WaterLevel] << "\tvque[i][WaterLevel]:" << vque[i][WaterLevel].size() << "\n";
                        SeedCounts[i][WaterLevel]--;//ȡ��һ���㣬������һ  
                        temp=vque[i][WaterLevel].front();//ȡ���������һ���㣬
                        vque[i][WaterLevel].pop(); // ��������Ե�㣬��ʾ��ǰ�Ҷȼ��������Ѿ�����  
                        m = temp.x;  
                        n = temp.y;//��ǰ���ӵ�����  
                        if (m > 0)  
                        {  
							
                            if(!Labelimage.at<uchar>(m-1,n))//�Ϸ���δ������ʾû�б�ţ�������ǰ�Ѿ�������ʼ��Ϊ 0  
                            //���������ڿ�ͷҲ������ʼ��  
                            {  
                                temp.x=m-1;  
                                temp.y=n;  
                                //Labelimage.at<uchar>(m-1,n)=i+1;//�Ϸ�����Ϊ����û����  i+1Ϊ�����
                                /*
								ע�⵽����������ɨ�����������ͬ��һ�����������������������ǵġ�
								���⻹��������ǲ�û�н������뵽���Ӷ��С�Ҳ����˵��  ֻ�Ǳ���û������������û��
								ֻ������vque[i][WaterLevel].push(temp); �ſ���������û��
								*/
                                if(Originalimage.at<uchar>(m-1,n)<=WaterLevel)//�Ϸ���Ϊ������������뵱ǰ���У���ǰ�߶ȵĶ���  
                                {  
                                    Labelimage.at<uchar>(m-1,n)=i+1;//�Ϸ�����Ϊ����û����  i+1Ϊ�����
									vque[i][WaterLevel].push(temp);  
                                } 
								/*
								������� OriginalImage[m-1][n]��Ӧ�ĻҶȼ��Ķ��У�Ϊʲô��
								Ϊ��һˮλ��û��׼����������һˮλvque[i][WaterLevel]Ϊ�ա�
								��������vque[i][WaterLevel]Ϊ�գ����ӱ�Ե�Ų�Ϊ�գ�����ˮλ����vque[i][WaterLevel]������������
								*/
                                else
                                {  
                                    vque[i][Originalimage.at<uchar>(m-1,n)].push(temp);  
                                    SeedCounts[i][Originalimage.at<uchar>(m-1,n)]++;  
                                }  
                            }  
                        }  
  
                        if (m < row - 1)  
                        {  
                            if(!Labelimage.at<uchar>(m+1,n))//�·���δ����  
                            {  
                                temp.x=m+1;  
                                temp.y=n;  
                                //Labelimage.at<uchar>(m+1,n)=i+1;//�·�����Ϊ����û����  
                                if(Originalimage.at<uchar>(m+1,n)<=WaterLevel)//�·���Ϊ������������뵱ǰ����  
                                { 
									Labelimage.at<uchar>(m+1,n)=i+1;//�·�����Ϊ����û����  
                                    vque[i][WaterLevel].push(temp);  
                                }  
                                else//������� OriginalImage[m+1][n]������  
                                {  
                                    vque[i][Originalimage.at<uchar>(m+1,n)].push(temp);  
                                    SeedCounts[i][Originalimage.at<uchar>(m+1,n)]++;  
                                }  
                            }  
                        }  
  
                        if(n<col-1)  
                        {  
                            if(!Labelimage.at<uchar>(m,n+1))//�ұ���δ����  
                            {  
                                temp.x=m;  
                                temp.y=n+1;  
                                //Labelimage.at<uchar>(m,n+1)=i+1;//�ұߵ���Ϊ����û����  
                                if(Originalimage.at<uchar>(m,n+1)<=WaterLevel)//�ұ���Ϊ������������뵱ǰ����  
                                {
									Labelimage.at<uchar>(m,n+1)=i+1;//�ұߵ���Ϊ����û���� 
                                    vque[i][WaterLevel].push(temp);  
                                }  
                                else//������� OriginalImage[m][n+1]������  
                                {  
                                    vque[i][Originalimage.at<uchar>(m,n+1)].push(temp);  
                                    SeedCounts[i][Originalimage.at<uchar>(m,n+1)]++;  
                                }  
                            }  
                        }  
  
                        if (n > 0)  
                        {  
                            if(!Labelimage.at<uchar>(m,n-1))//�����δ����  
                            {  
                                temp.x = m;  
                                temp.y = n - 1;  
                                //Labelimage.at<uchar>(m,n-1)=i+1;//��ߵ���Ϊ����û����  
                                if(Originalimage.at<uchar>(m,n-1)<=WaterLevel)//�����Ϊ������������뵱ǰ����  
                                {  
									Labelimage.at<uchar>(m,n-1)=i+1;//��ߵ���Ϊ����û����  
                                    vque[i][WaterLevel].push(temp);  
                                }  
                                else//������� OriginalImage[m][n-1]������
                                {  
                                vque[i][Originalimage.at<uchar>(m,n-1)].push(temp);  
                                SeedCounts[i][Originalimage.at<uchar>(m,n-1)]++;  
                                }  
                            }  
                        }  
                    }//while ѭ������ 
                    SeedCounts[i][WaterLevel]=vque[i][WaterLevel].size();  
                }//if ���� 
            }//for ѭ������  
        }//while ѭ������  
    }//for ѭ������  
  
  
    while(!vque.empty())  
    {  
		pque=vque.back();  
		delete[] pque;  
		vque.pop_back();  
    }  
    while(!SeedCounts.empty())  
    {  
        array=SeedCounts.back();  
        delete[] array;  
        SeedCounts.pop_back();  
    } 
	Zonenum=Num;
}