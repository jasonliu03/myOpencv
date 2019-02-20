#include <iostream>  
#include <string>  
#include "opencv2/core.hpp"  
#include "opencv2/imgproc.hpp"  
#include "opencv2/highgui.hpp"  
  
using namespace std;  
using namespace cv;  


void SalientRegionDetectionBasedonFT(Mat &src){
	Mat Lab;
	cvtColor(src, Lab, CV_BGR2Lab); 
 
	int row=src.rows,col=src.cols;
 
	int Sal_org[row][col];
	memset(Sal_org,0,sizeof(Sal_org));
	
	Point3_<uchar>* p;
 
	int MeanL=0,Meana=0,Meanb=0;
	for (int i=0;i<row;i++){
		for (int j=0;j<col;j++){
			p=Lab.ptr<Point3_<uchar> > (i,j);
			MeanL+=p->x;
			Meana+=p->y;
			Meanb+=p->z;
		}
	}
	MeanL/=(row*col);
	Meana/=(row*col);
	Meanb/=(row*col);
 
	GaussianBlur(Lab,Lab,Size(3,3),0,0);
 
	Mat Sal=Mat::zeros(src.size(),CV_8UC1 );
 
	int val;
 
	int max_v=0;
	int min_v=1<<28;
 
	for (int i=0;i<row;i++){
		for (int j=0;j<col;j++){
			p=Lab.ptr<Point3_<uchar> > (i,j);
			val=sqrt( (MeanL - p->x)*(MeanL - p->x)+ (p->y - Meana)*(p->y-Meana) + (p->z - Meanb)*(p->z - Meanb) );
			Sal_org[i][j]=val;
			max_v=max(max_v,val);
			min_v=min(min_v,val);		
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
    SalientRegionDetectionBasedonFT(srcMat);
}
