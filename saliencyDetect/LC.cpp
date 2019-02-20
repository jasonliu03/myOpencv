#include <iostream>  
#include <string>  
#include "opencv2/core.hpp"  
#include "opencv2/imgproc.hpp"  
#include "opencv2/highgui.hpp"  
  
using namespace std;  
using namespace cv;  


void SalientRegionDetectionBasedonLC(Mat &src){
    int HistGram[256]={0};
    int row=src.rows,col=src.cols;
    int gray[row][col];
    //int Sal_org[row][col];
    int val;
    Mat Sal=Mat::zeros(src.size(),CV_8UC1 );
    Point3_<uchar>* p;
    for (int i=0;i<row;i++){
        for (int j=0;j<col;j++){
            p=src.ptr<Point3_<uchar> > (i,j);
            val=(p->x + (p->y) *2 + p->z)/4;
            HistGram[val]++;
            gray[i][j]=val;
        }
    }
 
    int Dist[256];
    int Y,X;
    int max_gray=0;
    int min_gray=1<<28;
    for (Y = 0; Y < 256; Y++)
    {
        val = 0;
        for (X = 0; X < 256; X++) 
            val += abs(Y - X) * HistGram[X];                //    论文公式（9），灰度的距离只有绝对值，这里其实可以优化速度，但计算量不大，没必要了
        Dist[Y] = val;
        max_gray=max(max_gray,val);
        min_gray=min(min_gray,val);
    }
 
    
    for (Y = 0; Y < row; Y++)
    {
        for (X = 0; X < col; X++)
        {
            Sal.at<uchar>(Y,X) = (Dist[gray[Y][X]] - min_gray)*255/(max_gray - min_gray);        //    计算全图每个像素的显著性
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
    SalientRegionDetectionBasedonLC(srcMat);
}
