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
 Originalimage，Seedimage，Labelimage全部为CV_8U；即单通道灰度图，
 且图像尺寸必须完全一样，且Seedimage必须为二值图，即仅有0和255。Labelimage初始化全部为0.
 Originalimage，Seedimage，Labelimage
 */
void myWatershed(Mat& Originalimage, Mat& Seedimage, Mat& Labelimage,int& Zonenum)  
{ 
	if(Originalimage.rows!=Seedimage.rows||Originalimage.cols!=Seedimage.cols) 
		cout<<"error:Seedimage与Originalimage图像尺寸不同,请检查\n";
	if(Originalimage.rows!=Labelimage.rows||Originalimage.cols!=Labelimage.cols) 
		cout<<"error:Labelimage与Originalimage图像尺寸不同，请检查\n";
	threshold(Seedimage,Seedimage,1,255,THRESH_BINARY);//Seedimage初始必须为二值图，即仅仅含有0和255
	int row=Originalimage.rows;
	int col=Originalimage.cols;

	 //标记区域标识号，从1开始  
    int Num = 0;  
    /*int i,j;  */
    //保存每个队列种子个数的数组  
    vector<int*>SeedCounts; 
	 int *array;  

    //临时种子队列  
    queue<Point>quetem;  //FIFO，单向队列

    //保存所有标记区域种子队列的数组，里面存放是种子队列的指针  
    vector<queue<Point>*>vque; 
	//指向种子队列的指针  
    queue<Point> *pque;  
  
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
    //预处理，提取区分没给标记区域，并初始化每个标记的种子队列  
    //种子是指标记区域边缘的点，他们可以在水平上升时间时向外淹没（或者说生长）  
    for (int i = 0; i < row; i++)  
    {  
        for (int j = 0; j < col; j++)  
        {  
            //如果找到一个标记区域  
            if (Seedimage.at<uchar>(i,j) == 255) 
            {  
                //区域的标示号加一  初始为0,第一个为1
                Num++;  
				/*cout<<Num<<"\n";*/
                //分配数组并初始化为0，表示可有256灰阶  即0-255
                array = new int[256];  
                memset(array,256*sizeof(int),0);  
                //种子个数数组进vector，每次扫描则生成一个数组，并用区域标示号来做第一维。灰度级做第二维 
				// SeedCounts[m][n] 表示m区域，灰度为n的点的数目——马腾飞
                //表示某个盆地区域中某灰阶所对应的点的数目  
                SeedCounts.push_back(array);  
                //分配本标记号的优先队列，256个种子队列。  
                //表示对应一个灰阶有一个队列，并且每个队列可以存储一个集合的点信息  
                pque = new queue<Point>[256];  
                //加入到队列数组中，对应的是本标记号Num的  
				//vque[m][n]表示m区域，n灰阶对应的点的队列——马腾飞 区域有Num标示
                vque.push_back(pque);  
                //当前点放入本标记区域的临时种子队列中  
                temp.x = i;  
                temp.y = j;  
                quetem.push(temp);  
  
                //当前点标记为已处理  
                Labelimage.at<uchar>(i,j) = Num; 
				/*cout<< (int)Labelimage.at<uchar>(i,j)<<"df\n";*/
                Seedimage.at<uchar>(i,j) = 127;//表示已经处理过  Seedimage最初只有0和255,127仅仅标示已经处理过的点，任何非0和255的整数都能标示
                //让临时种子队列中的种子进行生长直到所有的种子都生长完毕  
                //生长完毕后的队列信息保存在 vque 中，包括对应的区域号和灰阶信息，对应点数存储在 seedcounts 中 
				//while执行完vque存储的是种子最外一层的点的信息。整个种子对应位置Seedimage被标记为127，Labelimage被标记为对应的区域号Num
				/*int fff=0;*/
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
                    //上下左右只要有一点不可生长，那么本点为初始种子队列中的一个  
                    //这里可否生长是由 seedimage 中的值来决定的  
                    if (up || down || right || left || upleft || downleft || upright || downright)  
                    {  
						/*fff++;
						cout<<fff<<"||\n";*/
                        temp.x = m;  
                        temp.y = n;  
                        //下面这个矢量数组：第一维是标记号；第二维是该图像点的灰度级
                        //m,n 点对应//Num 是当前的区域号  
                        //这样这个二维信息就表示了，某个区域中对应某个灰度级对应的成员点的集合与个数  
                        //分别由下面两个量来表达的是 while 循环里面扫描的像素点。  
                        vque[Num-1][Originalimage.at<uchar>(m,n)].push(temp);//把Num 区，Seedimage边缘中Originalimage[m][n]灰阶的点全部加入vque[Num-1][Originalimage[m][n]]这个队列里面  
                        SeedCounts[Num-1][Originalimage.at<uchar>(m,n)]++;//记录Num 区，Seedimage边缘中Originalimage[m][n]灰阶的点，有多少个  
                    }  
  
                }//while 结束，扫描到 quetem 为空而止。也就是对应所有的节点都得到不可生长为止（或者是周围的点要么不可生长，要么已生长）  
            }//if 结束  
        }  
    }  
    //在上述过程中，如果标记的点为 0 则表示，没有扫描到的点，或者表明不是输入的种子点  
    //这里相当于是找 seedimage 传过来的初始区域的分水岭界线的所有的点；并且用标号记录每个区域，同时集水盆的边缘点进入队列。  
    //上面是找集水盆的程序。同时也是连通区域。  

    bool actives=false;//在某一水位处，所有标记的种子生长完的标志  true表示未生长完，false表示生长完了
    int WaterLevel;  
    //淹没过程开始，水位从零开始上升，水位对应灰度级，采用四连通法  
    for (WaterLevel = 0; WaterLevel < 255; WaterLevel++)//第二维。。。  水位0到255 对应灰度从0到255
    {  
        actives = true;  
        while(actives)  
        {  
            actives=false;  
            //依次处理每个标记号所对应的区域，且这个标记号对应的区域的点的个数在 SeedCounts 里面  
            for (int i = 0; i < Num; i++)//第一维。。。[WaterLevel][Num]  
            {   //是否if应改为while，目前if似乎只能保证目前水位线往外扩张一次，并没有保证所有此水位的所有位置被淹没（马腾飞）
				/*
				此处只能是if，为if时能保证每个标记区域均匀向外增长，进而有效分割；
				若为while，将
				*/
                if(!vque[i][WaterLevel].empty())//对应的分水岭不为空集,i+1 表示区域号，waterlevel 表示灰阶  
                {  
                    actives = true;  
                    while(SeedCounts[i][WaterLevel]>0)  
                    {  
                        SeedCounts[i][WaterLevel]--;//取出一个点，个数少一  
                        temp=vque[i][WaterLevel].front();//取出该区域的一个点，
                        vque[i][WaterLevel].pop(); // 清空这个边缘点，表示当前灰度级该像素已经处理。  
                        m = temp.x;  
                        n = temp.y;//当前种子的坐标  
                        if (m > 0)  
                        {  
							
                            if(!Labelimage.at<uchar>(m-1,n))//上方若未处理，表示没有标号，在输入前已经作过初始化为 0  
                            //本函数中在开头也作过初始化  
                            {  
                                temp.x=m-1;  
                                temp.y=n;  
                                //Labelimage.at<uchar>(m-1,n)=i+1;//上方点标记为已淹没区域  i+1为区域号
                                /*
								注意到这个标记是与扫描点的区域号相同，一定在这个标号所属的区域吗？是的。
								问题还有这样标记并没有将它加入到种子队列。也就是说它  只是被淹没而不能向上淹没。
								只有满足vque[i][WaterLevel].push(temp); 才可以向上淹没。
								*/
                                if(Originalimage.at<uchar>(m-1,n)<=WaterLevel)//上方若为可生长点则加入当前队列，当前高度的队列  
                                {  
                                    Labelimage.at<uchar>(m-1,n)=i+1;//上方点标记为已淹没区域  i+1为区域号
									vque[i][WaterLevel].push(temp);  
                                } 
								/*
								否则加入 OriginalImage[m-1][n]对应的灰度级的队列，为什么？
								为下一水位淹没做准备，否则下一水位vque[i][WaterLevel]为空。
								在种子内vque[i][WaterLevel]为空，种子边缘才不为空，随着水位上升vque[i][WaterLevel]往种子外扩张
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
                            if(!Labelimage.at<uchar>(m+1,n))//下方若未处理  
                            {  
                                temp.x=m+1;  
                                temp.y=n;  
                                //Labelimage.at<uchar>(m+1,n)=i+1;//下方点标记为已淹没区域  
                                if(Originalimage.at<uchar>(m+1,n)<=WaterLevel)//下方若为可生长点则加入当前队列  
                                { 
									Labelimage.at<uchar>(m+1,n)=i+1;//下方点标记为已淹没区域  
                                    vque[i][WaterLevel].push(temp);  
                                }  
                                else//否则加入 OriginalImage[m+1][n]级队列  
                                {  
                                    vque[i][Originalimage.at<uchar>(m+1,n)].push(temp);  
                                    SeedCounts[i][Originalimage.at<uchar>(m+1,n)]++;  
                                }  
                            }  
                        }  
  
                        if(n<col-1)  
                        {  
                            if(!Labelimage.at<uchar>(m,n+1))//右边若未处理  
                            {  
                                temp.x=m;  
                                temp.y=n+1;  
                                //Labelimage.at<uchar>(m,n+1)=i+1;//右边点标记为已淹没区域  
                                if(Originalimage.at<uchar>(m,n+1)<=WaterLevel)//右边若为可生长点则加入当前队列  
                                {
									Labelimage.at<uchar>(m,n+1)=i+1;//右边点标记为已淹没区域 
                                    vque[i][WaterLevel].push(temp);  
                                }  
                                else//否则加入 OriginalImage[m][n+1]级队列  
                                {  
                                    vque[i][Originalimage.at<uchar>(m,n+1)].push(temp);  
                                    SeedCounts[i][Originalimage.at<uchar>(m,n+1)]++;  
                                }  
                            }  
                        }  
  
                        if (n > 0)  
                        {  
                            if(!Labelimage.at<uchar>(m,n-1))//左边若未处理  
                            {  
                                temp.x = m;  
                                temp.y = n - 1;  
                                //Labelimage.at<uchar>(m,n-1)=i+1;//左边点标记为已淹没区域  
                                if(Originalimage.at<uchar>(m,n-1)<=WaterLevel)//左边若为可生长点则加入当前队列  
                                {  
									Labelimage.at<uchar>(m,n-1)=i+1;//左边点标记为已淹没区域  
                                    vque[i][WaterLevel].push(temp);  
                                }  
                                else//否则加入 OriginalImage[m][n-1]级队列
                                {  
                                vque[i][Originalimage.at<uchar>(m,n-1)].push(temp);  
                                SeedCounts[i][Originalimage.at<uchar>(m,n-1)]++;  
                                }  
                            }  
                        }  
                    }//while 循环结束 
					
                    SeedCounts[i][WaterLevel]=vque[i][WaterLevel].size();  
                }//if 结束  
            }//for 循环结束  
        }//while 循环结束  
    }//for 循环结束  
  
  
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