// OCR.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h" 
#include "cv.h" 
#include "highgui.h"   

#define uint  unsigned int

struct MouseArgs{
	IplImage* img;
    CvRect box;
	bool Drawing;

	MouseArgs():Drawing(false),img(0){
		box=cvRect(0,0,-1,-1);
	}
	// destroy
	void Destroy(){
		if(!img)
			cvReleaseImage(&img);
	}
};

/*
描述：在图像上绘制矩形
*/
void DrawRect(IplImage*img,CvRect rect)
{
	cvRectangle(img,
		cvPoint(rect.x,rect.y),
		cvPoint(rect.x+rect.width,rect.y+rect.height),
		cvScalar(255,0,0),3);
}

/*
描述：鼠标事件的回调函数
函数原型：  void Foo(int event, int x, int y, int flags, void* param);
参数： event -- CV_EVENT_*变量之一，
       x,y   -- 鼠标指针在图像坐标系的坐标（不是窗口坐标系）
       flags -- CV_EVENT_FLAG的组合
       param -- 用户定义的传递到cvSetMouseCallback函数调用的参数
*/
void MouseDraw(int event,int x,int y,int flags,void*param)
{
	MouseArgs* m_arg = (MouseArgs*) param;
    if(!m_arg->img)
		return;

	switch(event)
	{
	case CV_EVENT_MOUSEMOVE: // 鼠标移动时
		{
			if(m_arg->Drawing)
			{
				m_arg->box.width = x-m_arg->box.x;
				m_arg->box.height = y-m_arg->box.y;
			}
		}
		break;
	case CV_EVENT_LBUTTONDOWN: // 左键按下
		{
			m_arg->Drawing = true;
			m_arg->box = cvRect(x,y,0,0);
		}
		break;
	case CV_EVENT_LBUTTONUP: // 左键弹起
		{
			m_arg->Drawing = false;
			if (m_arg->box.width<0)
			{
				m_arg->box.x += m_arg->box.width;
				m_arg->box.width *= -1;
			}
			if (m_arg->box.height<0)
			{
				m_arg->box.y += m_arg->box.height;
				m_arg->box.height *= -1;
			}
			DrawRect(m_arg->img, m_arg->box);
		}
		break;
	}
}

void reduceRedColor(IplImage *src, IplImage *dst)
{
	int i, j;
	unsigned char b, g, r;

	assert(src->width == dst->width && src->height == dst->height && src->nChannels == dst->nChannels);

	for (i = 0; i < dst->height; i++)
	{
		for (j = 0; j < dst->width; j++)
		{
			//dst->imageData[i*dst->nChannels + 3 * i + 0] = IplImage *
			b = src->imageData[i*src->widthStep + src->nChannels * j + 0];
			g = src->imageData[i*src->widthStep + src->nChannels * j + 1];
			r = src->imageData[i*src->widthStep + src->nChannels * j + 2];

			if (r>g && r>b && r>215)
			{
				dst->imageData[i*dst->widthStep + dst->nChannels * j + 0] = 255;
				dst->imageData[i*dst->widthStep + dst->nChannels * j + 1] = 255;
				dst->imageData[i*dst->widthStep + dst->nChannels * j + 2] = 255;
			}
			else
			{
				dst->imageData[i*dst->widthStep + dst->nChannels * j + 0] = b;
				dst->imageData[i*dst->widthStep + dst->nChannels * j + 1] = g;
				dst->imageData[i*dst->widthStep + dst->nChannels * j + 2] = r;
			}

		}
	}

}


#include <opencv2/highgui/highgui.hpp>      
#include <opencv2/imgproc/imgproc.hpp>  
#include <iostream>  
#include <iostream>
#include <fstream>
using namespace cv;

int main11(int argc, char *argv[])
{
	Mat image = imread("E://图片/山西农信存单/Y20171124M0005 正/99000000202/1.bmp", 1);
	if (image.empty())
	{
		std::cout << "打开图片失败,请检查" << std::endl;
		return -1;
	}
	imshow("原图像", image);
	Mat imageRGB[3];
	split(image, imageRGB);
	for (int i = 0; i < 3; i++)
	{
		equalizeHist(imageRGB[i], imageRGB[i]);
	}
	merge(imageRGB, 3, image);
	imshow("直方图均衡化图像增强效果", image);
	waitKey();
	return 0;
}

#include <opencv2/highgui/highgui.hpp>      
#include <opencv2/imgproc/imgproc.hpp>  
#include <iostream>  

using namespace cv;

int main232(int argc, char *argv[])
{
	Mat image = imread("E://图片/山西农信存单/Y20171124M0005 正/99000000202/1.bmp", 1);
	if (image.empty())
	{
		std::cout << "打开图片失败,请检查" << std::endl;
		return -1;
	}
	imshow("原图像", image);
	Mat imageEnhance;
	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
	filter2D(image, imageEnhance, CV_8UC3, kernel);
	imshow("拉普拉斯算子图像增强效果", imageEnhance);
	waitKey();
	return 0;
}

#include <opencv2/highgui/highgui.hpp>      
#include <opencv2/imgproc/imgproc.hpp>  

#include "opencv2/opencv.hpp"
#include <iostream>
using namespace std;
using namespace cv;



void mainMatch()
{
	Mat temp = imread("E://银联.bmp", 1 );
	Mat src = imread("D://id_data/id_data/front(锦绣潇湘旅游卡).bmp", 1);
	Mat dst = src.clone();
	imshow("temp", temp);
	 
	int width = src.cols - temp.cols + 1;//result宽度
	int height = src.rows - temp.rows + 1;//result高度

	Mat result(height, width, CV_32FC1);//创建结果映射图像
	Mat sortPoint(height, width, CV_32FC1);//创建结果映射图像
	//matchTemplate(srcImg, templateImg, resultImg, CV_TM_SQDIFF); //平方差匹配法(最好匹配0)
	//matchTemplate(srcImg, templateImg, resultImg, CV_TM_SQDIFF_NORMED); //归一化平方差匹配法(最好匹配0)
	//matchTemplate(srcImg, templateImg, resultImg, CV_TM_CCORR); //相关匹配法(最坏匹配0)
	//matchTemplate(src, temp, result, CV_TM_CCORR_NORMED); //归一化相关匹配法(最坏匹配0)
	//matchTemplate(srcImg, templateImg, resultImg, CV_TM_CCOEFF); //系数匹配法(最好匹配1)
	matchTemplate(src, temp, result, CV_TM_CCOEFF_NORMED);//化相关系数匹配,最佳值1
	imshow("result", result);

	float valMin=0xFFFF, valMax=-9999;
	Point minPoint, maxPoint;

	for(int i=0; i<result.rows; i++)
	{
		uchar* data= result.ptr<uchar>(i);
		for(int j=0; j<result.cols; j++)
		{
			if(valMax < result.at<float>(i,j))
			{
				valMax = result.at<float>(i,j);
				maxPoint.x = i;
				maxPoint.y = j;
			}
		}

	}

	if(valMax < 0.9)
	{
		printf("Cannot Find the match area\n");
	}

	normalize(result, result, 0, 1, NORM_MINMAX, -1);//归一化到0-1范围

	
	double minValue, maxValue;
	Point minLoc, maxLoc;
	minMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc);

	//result.copyTo(sortPoint);
	//sort(sortPoint, sortPoint, CV_SORT_EVERY_ROW + CV_SORT_DESCENDING);

	cout << "minValue=" << minValue << endl;
	cout << "maxValue=" << maxValue << endl;

	rectangle(dst, maxLoc, Point(maxLoc.x + temp.cols, maxLoc.y + temp.rows), Scalar(0, 255, 0), 2, 8);
	imshow("dst", dst);

	waitKey(0);
}

const string filename = "test.dat";
const string picname = "cat.jpg";

typedef struct ImageDataTag{
	int imagewidth;
	int imageheight;
	int imagechannels;
	int imagereserve;
}ImageData;




int _tmain(int argc, _TCHAR* argv[]) 
{  

	ImageData *pimageTemp;

	pimageTemp = (ImageData *)malloc(sizeof(ImageData));

	Mat temp = imread("E:\\银联.bmp", 1 );
	if(temp.empty())
	{
		printf("Cannot Open Image file.\n");
		return 0;
	}

	FILE *fp, *fp1;
	if( (fp = fopen( "test.dat", "wb")) == NULL )
	{
		return 0;
	}

	int channels = temp.channels();
	int step  = temp.step1(0);
	uchar *buf = (uchar *)malloc(step * temp.rows);
	uchar *buf1 = (uchar *)malloc(step * temp.rows);

	memset(buf, 0x00, step * temp.rows);
	memset(buf1, 0x00, step * temp.rows);

	int len=0;
	for( int i = 0; i < temp.rows; i++ )
	{
		uchar* data= temp.ptr<uchar>(i);
		//fwrite(data + temp.elemSize()*temp.cols * i, sizeof(char), temp.elemSize()*temp.cols, fp);
		for(int j=0; j<channels*temp.cols; j++)
		{
			buf[len++] = data[j];
		}
	}

	pimageTemp->imagewidth = temp.cols;
	pimageTemp->imageheight = temp.rows;
	pimageTemp->imagechannels = channels;

	fwrite((void *)pimageTemp, sizeof(char), sizeof(ImageData), fp);
	fwrite(buf, sizeof(char), channels*temp.cols*temp.rows, fp);


	fclose(fp);

	if( (fp1 = fopen( "test.dat", "rb")) == NULL )
	{
		return 0;
	}

	memset(buf1, 0x00, step * temp.rows);
	memset((void *)pimageTemp, 0x00, sizeof(ImageData));
	
	fseek(fp1, 0, SEEK_SET);

	fread((void*)pimageTemp, sizeof(char),  sizeof(ImageData), fp1);

	fseek(fp1, sizeof(ImageData), SEEK_SET);

	fread(buf1, sizeof(char), channels*temp.cols*temp.rows, fp1);

	fclose(fp1);

	Mat image;
	
	image.create(temp.rows, temp.cols, CV_8UC3);

	channels = image.channels();
	len = 0;
	for( int i = 0; i < image.rows; i++ )
	{
		uchar* data= image.ptr<uchar>(i);
		
		for(int j=0; j<channels*image.cols; j++)
		{
			data[j] = buf1[len++];
		}
	}

	imshow("Pic", image);

	waitKey();

	free(pimageTemp);
	free(buf);
	free(buf1);


	return 1;
	

} 
