#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <stdio.h>  
#include <cv.h>  
#include "cvaux.h" 
#include "cxcore.h"
#include <iostream>  
#include <fstream>   
//#include "opencv2/nonfree/nonfree.hpp"
using namespace std;
using namespace cv;

int main()
{
	Mat srcImg, grayImg;
	srcImg = imread("2.jpg");       //读入图像并初始化为灰度图灰度图
	cvtColor(srcImg, grayImg, CV_RGB2GRAY);
	//判断图像是否加载成功
	if (grayImg.empty())
	{
		cout << "图像加载失败!" << endl;
		return -1;
	}
	else
		cout << "图像加载成功!" << endl << endl;

	Mat padded;                 //以0填充输入图像矩阵
	int m = grayImg.rows;
	int n = grayImg.cols;
	cout << n << endl << endl;
	cout << m << endl << endl;
	//							//用0填充输入图像grayImg
	copyMakeBorder(grayImg, padded, 0, m - grayImg.rows, 0, n - grayImg.cols, BORDER_CONSTANT, Scalar::all(0));
	padded = grayImg;
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(),CV_32F) };		//初始化时域和复频域
	Mat complexI;
	merge(planes, 2, complexI);     //将实数域和虚数域合并为复数域

	dft(complexI, complexI);			//对图像进行傅里叶变换
	
	
	split(complexI, planes);        //将复数域分割为实部planes[0]，虚部planes[1]

	magnitude(planes[0], planes[1], planes[0]);     //计算二维向量的幅值
	Mat fourier_Img = planes[0].clone();

	fourier_Img += Scalar::all(1);
	Mat jisuan_fu1 = fourier_Img.clone();



	log(fourier_Img, fourier_Img);


	//由于傅里叶变换频谱范围广，将线性坐标换成对数坐标节约空间

	fourier_Img = fourier_Img(Rect(0, 0, fourier_Img.cols&-2, fourier_Img.rows&-2));
	int bx = fourier_Img.cols / 2;
	int by = fourier_Img.rows / 2;
	Mat p0 = Mat(fourier_Img, Rect(0, 0, bx, by));
	Mat p1 = Mat(fourier_Img, Rect(bx, 0, bx, by));
	Mat p2 = Mat(fourier_Img, Rect(0, by, bx, by));
	Mat p3 = Mat(fourier_Img, Rect(bx, by, bx, by));

	Mat tmp1;
	p0.copyTo(tmp1);
	p3.copyTo(p0);
	tmp1.copyTo(p3);

	p1.copyTo(tmp1);
	p2.copyTo(p1);
	tmp1.copyTo(p2);
	normalize(fourier_Img, fourier_Img, 0, 1, CV_MINMAX);

	//将频谱重新按照四个象限进行排列
	int cx = jisuan_fu1.cols / 2;			//初始化频谱图的横纵坐标
	int cy = jisuan_fu1.rows / 2;

	Mat q0 = Mat(complexI, Rect(0, 0, cx, cy));       //左上角图像划定ROI区域
	Mat q1 = Mat(complexI, Rect(cx, 0, cx, cy));      //右上角图像
	Mat q2 = Mat(complexI, Rect(0, cy, cx, cy));      //左下角图像
	Mat q3 = Mat(complexI, Rect(cx, cy, cx, cy));     //右下角图像

											//变换左上角和右下角象限
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	//变换右上角和左下角象限
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	//int v = complexI.rows;
	//int z = complexI.cols;
	//cout << z << endl << endl;
	//cout << v << endl << endl;
	//v = planes[0].rows;
	//z = planes[0].cols;
	//cout << z << end l << endl;
	//cout << v << endl << endl;

	//split(complexI, planes);
	Mat filter = Mat(planes[0].size(), CV_32F);
	//filter = imread("二条.jpg");
	filter = imread("八筒.jpg");
	cvtColor(filter, filter, CV_RGB2GRAY);
	imshow("ada",filter);
	normalize(filter, filter, 0, 1, CV_MINMAX);
	/*cout << planes[0] << endl << endl;*/
	
	split(complexI, planes);
	//for (int i = 0; i < filter.rows; i++) {
	//	for (int j = 0; j < filter.cols; j++) {
	//		float temp, temp1;
	//		temp = planes[0].at<float>(i, j);
	//		temp1 = planes[1].at<float>(i, j);
	//		planes[0].at<float>(i, j) = temp * (filter.at<float>(i, j));
	//		//planes[1].at<float>(i, j) = temp1*filter.at<float>(i, j);
	//	}
	//}

	for (int i = 0; i < filter.rows; i++) {
		uchar* data = filter.ptr<uchar>(i);
		for (int j = 0; j < filter.cols; j++) {
			float temp = data[j];
			planes[0].at<float>(i, j) = planes[0].at<float>(i, j)*temp;
			planes[1].at<float>(i, j) = planes[1].at<float>(i, j)*temp;
			
		}
	}



	merge(planes, 2, complexI);





	//归一化处理，用0-1之间的浮点数将矩阵变换为可视的图像格式
	//normalize(fourier_Img, fourier_Img, 0, 1, CV_MINMAX);

	//int bx = planes[1].cols / 2;
	//int by = planes[1].rows / 2;
	//Mat p0 = Mat(planes[1], Rect(0, 0, bx, by));
	//Mat p1 = Mat(planes[1], Rect(bx, 0, bx, by));
	//Mat p2 = Mat(planes[1], Rect(0, by, bx, by));
	//Mat p3 = Mat(planes[1], Rect(bx, by, bx, by));

	//Mat tmp1;
	//p0.copyTo(tmp1);
	//p3.copyTo(p0);
	//tmp1.copyTo(p3);

	//p1.copyTo(tmp1);
	//p2.copyTo(p1);
	//tmp1.copyTo(p2);



	// cx = jisuan_fu1.cols / 2;			//初始化频谱图的横纵坐标
	// cy = jisuan_fu1.rows / 2;

	// q0 = Mat(planes[0], Rect(0, 0, cx, cy));       //左上角图像划定ROI区域
	// q1 = Mat(planes[0], Rect(cx, 0, cx, cy));      //右上角图像
	// q2 = Mat(planes[0], Rect(0, cy, cx, cy));      //左下角图像
	// q3 = Mat(planes[0], Rect(cx, cy, cx, cy));     //右下角图像

	//											 //变换左上角和右下角象限
	//
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	//变换右上角和左下角象限
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);








	Mat lvwanbo_fu;//idft后的矩阵
	/*merge(planes,2, lvwanbo_fu);*/
	
	idft(complexI, lvwanbo_fu);
	Mat plane1[] = { Mat::zeros(jisuan_fu1.size(), CV_32F), Mat::zeros(jisuan_fu1.size(), CV_32F) };
	split(lvwanbo_fu, plane1);
	magnitude(plane1[0], plane1[1], plane1[0]);

	normalize(plane1[0], plane1[0], 0, 1, CV_MINMAX);
	Mat result;
	result = Mat(plane1[0], Rect(0, 0, grayImg.cols, grayImg.rows));
	
	Mat huanyuantu;
	//huanyuantu = Mat(plane1[0], Rect(0, 0,grayImg.cols,grayImg.rows));
	imshow("sasas", grayImg);
	
	

	imshow("原始图像", result);
	imshow("傅里叶频谱图", fourier_Img);
	/*cout << plane1[0] << endl << endl;*/
	waitKey(0);



	waitKey(0);
	return 0;
}

