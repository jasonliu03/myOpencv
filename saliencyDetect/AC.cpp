#include <iostream>  
#include <string>  
#include "opencv2/core.hpp"  
#include "opencv2/imgproc.hpp"  
#include "opencv2/highgui.hpp"  
  
using namespace std;  
using namespace cv;  


void SalientRegionDetectionBasedonAC(Mat &src,int MinR2, int MaxR2,int Scale){
	Mat Lab;
	cvtColor(src, Lab, CV_BGR2Lab); 
 
	int row=src.rows,col=src.cols;
	int Sal_org[row][col];
	memset(Sal_org,0,sizeof(Sal_org));
	
	Mat Sal=Mat::zeros(src.size(),CV_8UC1 );
 
	Point3_<uchar>* p;
	Point3_<uchar>* p1;
	int val;
	Mat filter;
 
	int max_v=0;
	int min_v=1<<28;
	for (int k=0;k<Scale;k++){
		int len=(MaxR2 - MinR2) * k / (Scale - 1) + MinR2;
		blur(Lab, filter, Size(len,len ));
		for (int i=0;i<row;i++){
			for (int j=0;j<col;j++){
				p=Lab.ptr<Point3_<uchar> > (i,j);
				p1=filter.ptr<Point3_<uchar> > (i,j);
				//cout<<(p->x - p1->x)*(p->x - p1->x)+ (p->y - p1->y)*(p->y-p1->y) + (p->z - p1->z)*(p->z - p1->z) <<" ";
				
				val=sqrt( (p->x - p1->x)*(p->x - p1->x)+ (p->y - p1->y)*(p->y-p1->y) + (p->z - p1->z)*(p->z - p1->z) );
				Sal_org[i][j]+=val;
				if(k==Scale-1){
					max_v=max(max_v,Sal_org[i][j]);
					min_v=min(min_v,Sal_org[i][j]);
				}
			}
		}
	}
	
	cout<<max_v<<" "<<min_v<<endl;
	int X,Y;
    for (Y = 0; Y < row; Y++)
    {
        for (X = 0; X < col; X++)
        {
            Sal.at<uchar>(Y,X) = (Sal_org[Y][X] - min_v)*255/(max_v - min_v);        //    计算全图每个像素的显著性
        	//Sal.at<uchar>(Y,X) = (Dist[gray[Y][X]])*255/(max_gray);        //    计算全图每个像素的显著性
        }
    }
    imshow("sal",Sal);
    waitKey(0);
}


int main(int argc, char** argv)
{
    String imgName = "test.jpg";
    if(argc == 2)
    {
        imgName = argv[1];
    }
    Mat srcMat = imread(imgName);
    SalientRegionDetectionBasedonAC(srcMat, srcMat.rows/8, srcMat.rows/2, 3);
}
