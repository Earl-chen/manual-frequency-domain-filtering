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
	srcImg = imread("2.jpg");       //����ͼ�񲢳�ʼ��Ϊ�Ҷ�ͼ�Ҷ�ͼ
	cvtColor(srcImg, grayImg, CV_RGB2GRAY);
	//�ж�ͼ���Ƿ���سɹ�
	if (grayImg.empty())
	{
		cout << "ͼ�����ʧ��!" << endl;
		return -1;
	}
	else
		cout << "ͼ����سɹ�!" << endl << endl;

	Mat padded;                 //��0�������ͼ�����
	int m = grayImg.rows;
	int n = grayImg.cols;
	cout << n << endl << endl;
	cout << m << endl << endl;
	//							//��0�������ͼ��grayImg
	copyMakeBorder(grayImg, padded, 0, m - grayImg.rows, 0, n - grayImg.cols, BORDER_CONSTANT, Scalar::all(0));
	padded = grayImg;
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(),CV_32F) };		//��ʼ��ʱ��͸�Ƶ��
	Mat complexI;
	merge(planes, 2, complexI);     //��ʵ�����������ϲ�Ϊ������

	dft(complexI, complexI);			//��ͼ����и���Ҷ�任
	
	//idft(gaussianBlur, inverseDFT, DFT_SCALE | DFT_REAL_OUTPUT); // Applying IDFT
	//inverseDFT.convertTo(inverseDFTconverted, CV_8U);
	//Scalar s1 = sum(gaussianBlur);
	//imshow("Output", inverseDFTconverted);
	split(complexI, planes);        //��������ָ�Ϊʵ��planes[0]���鲿planes[1]

	magnitude(planes[0], planes[1], planes[0]);     //�����ά�����ķ�ֵ
	Mat fourier_Img = planes[0].clone();

	fourier_Img += Scalar::all(1);
	Mat jisuan_fu1 = fourier_Img.clone();



	log(fourier_Img, fourier_Img);


	//���ڸ���Ҷ�任Ƶ�׷�Χ�㣬���������껻�ɶ��������Լ�ռ�

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

	//��Ƶ�����°����ĸ����޽�������
	int cx = jisuan_fu1.cols / 2;			//��ʼ��Ƶ��ͼ�ĺ�������
	int cy = jisuan_fu1.rows / 2;

	Mat q0 = Mat(complexI, Rect(0, 0, cx, cy));       //���Ͻ�ͼ�񻮶�ROI����
	Mat q1 = Mat(complexI, Rect(cx, 0, cx, cy));      //���Ͻ�ͼ��
	Mat q2 = Mat(complexI, Rect(0, cy, cx, cy));      //���½�ͼ��
	Mat q3 = Mat(complexI, Rect(cx, cy, cx, cy));     //���½�ͼ��

											//�任���ϽǺ����½�����
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	//�任���ϽǺ����½�����
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
	//filter = imread("����.jpg");
	filter = imread("��Ͳ.jpg");
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

	/*planes[0] = planes[0].mul(filter);
	planes[1] = planes[1].mul(filter);*/

	merge(planes, 2, complexI);





	//��һ��������0-1֮��ĸ�����������任Ϊ���ӵ�ͼ���ʽ
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



	// cx = jisuan_fu1.cols / 2;			//��ʼ��Ƶ��ͼ�ĺ�������
	// cy = jisuan_fu1.rows / 2;

	// q0 = Mat(planes[0], Rect(0, 0, cx, cy));       //���Ͻ�ͼ�񻮶�ROI����
	// q1 = Mat(planes[0], Rect(cx, 0, cx, cy));      //���Ͻ�ͼ��
	// q2 = Mat(planes[0], Rect(0, cy, cx, cy));      //���½�ͼ��
	// q3 = Mat(planes[0], Rect(cx, cy, cx, cy));     //���½�ͼ��

	//											 //�任���ϽǺ����½�����
	//
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	//�任���ϽǺ����½�����
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);






	/* bx = jisuan_fu2.cols / 2;
	 by = jisuan_fu2.rows / 2;
	 p0 = Mat(planes[1], Rect(0, 0, bx, by));
	 p1 = Mat(planes[1], Rect(bx, 0, bx, by));
	 p2 = Mat(planes[1], Rect(0, by, bx, by));
	 p3 = Mat(planes[1], Rect(bx, by, bx, by));*/

	/*p0.copyTo(tmp1);
	p3.copyTo(p0);
	tmp1.copyTo(p3);

	p1.copyTo(tmp1);
	p2.copyTo(p1);
	tmp1.copyTo(p2);*/
	//int mx = jisuan_fu2.cols / 2;
	//int my = jisuan_fu2.rows / 2;
	//Mat r0 = Mat(planes[1], Rect(0, 0, mx, my));
	//Mat r1 = Mat(planes[1], Rect(mx, 0, mx, my));
	//Mat r2 = Mat(planes[1], Rect(0, my, mx, my));
	//Mat r3 = Mat(planes[1], Rect(mx, my, mx, my));

	//Mat tmp2;
	//r0.copyTo(tmp2);
	//r3.copyTo(r0);
	//tmp2.copyTo(r3);

	//r1.copyTo(tmp2);
	//r2.copyTo(r1);
	//tmp2.copyTo(r2);


	//int nx = jisuan_fu1.cols / 2;			//��ʼ��Ƶ��ͼ�ĺ�������
	//int ny = jisuan_fu1.rows / 2;

	//Mat t0(planes[0], Rect(0, 0, nx, ny));       //���Ͻ�ͼ�񻮶�ROI����
	//Mat t1(planes[0], Rect(nx, 0, nx, ny));      //���Ͻ�ͼ��
	//Mat t2(planes[0], Rect(0, ny, nx, ny));      //���½�ͼ��
	//Mat t3(planes[0], Rect(nx, ny, nx, ny));     //���½�ͼ��

	//											  //�任���ϽǺ����½�����
	//Mat tmp3;
	//t0.copyTo(tmp3);
	//t3.copyTo(t0);
	//tmp3.copyTo(t3);

	////�任���ϽǺ����½�����
	//t1.copyTo(tmp3);
	//t2.copyTo(t1);
	//tmp3.copyTo(t2);

	Mat lvwanbo_fu;//idft��ľ���
	/*merge(planes,2, lvwanbo_fu);*/
	
	idft(complexI, lvwanbo_fu);
	Mat plane1[] = { Mat::zeros(jisuan_fu1.size(), CV_32F), Mat::zeros(jisuan_fu1.size(), CV_32F) };
	split(lvwanbo_fu, plane1);
	magnitude(plane1[0], plane1[1], plane1[0]);

	normalize(plane1[0], plane1[0], 0, 1, CV_MINMAX);
	Mat result;
	result = Mat(plane1[0], Rect(0, 0, grayImg.cols, grayImg.rows));
	//�õ���һ����Ľ��ͼ��
	//Mat plane1 []= { Mat::zeros(jisuan_fu.size(), CV_32F), Mat::zeros(jisuan_fu.size(), CV_32F) };
	//idft(jisuan_fu, jisuan_fu);
	//split(jisuan_fu, plane1);
	//magnitude(plane1[0], plane1[1], plane1[0]);
	//
	//cv::normalize(plane1[0], plane1[0], 0, 1, CV_MINMAX);
	///*plane1[0] = 100000000*plane1[0];*/
	///*normalize(plane1[0], plane1[0], 0, 1, CV_MINMAX);*/
	//cout << plane1[0] << endl << endl;
	Mat huanyuantu;
	//huanyuantu = Mat(plane1[0], Rect(0, 0,grayImg.cols,grayImg.rows));
	imshow("sasas", grayImg);
	
	

	imshow("ԭʼͼ��", result);
	imshow("����ҶƵ��ͼ", fourier_Img);
	/*cout << plane1[0] << endl << endl;*/
	waitKey(0);



	waitKey(0);
	return 0;
}

