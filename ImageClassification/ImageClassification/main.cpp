#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <cvcam.h>

using namespace std;

// 이미지 데이터 구조체
typedef struct imageData{
	int width;			// 가로
	int height;			// 세로
	int area;			// 넓이
	int perimeter;		// 둘레
	int **i_image;		// 이진 이미지 배열
	int MBR;			// MBR
	double rectangularity;
	double thickness;
	double distance[3];
}imageData;

void ImageToBinarry(IplImage *image, imageData *b_image);
void ImagePrint(imageData *b_image);
void ImageRelease(imageData *b_image);
void ImageClose(imageData *b_image);
void ImageArea(imageData *b_image);
void ImagePerimeter(imageData *b_image);
void ImageMBR(imageData *b_image);
void printTable(imageData *b_image, int index);

int accuracy = 0;

void main()
{
	IplImage *r[3], *t[27], *s[6];
	imageData b_r[3], b_t[27], b_s[6];

	// 학습 이미지 불러오기
	r[0] = cvLoadImage("image\\r1.bmp",-1);
	r[1] = cvLoadImage("image\\r2.bmp",-1);
	r[2] = cvLoadImage("image\\r3.bmp",-1);
	// 테스트 이미지 불러오기
	t[0] = cvLoadImage("image\\t01.bmp",-1);
	t[1] = cvLoadImage("image\\t02.bmp",-1);
	t[2] = cvLoadImage("image\\t03.bmp",-1);
	t[3] = cvLoadImage("image\\t04.bmp",-1);
	t[4] = cvLoadImage("image\\t05.bmp",-1);
	t[5] = cvLoadImage("image\\t06.bmp",-1);
	t[6] = cvLoadImage("image\\t07.bmp",-1);
	t[7] = cvLoadImage("image\\t08.bmp",-1);
	t[8] = cvLoadImage("image\\t09.bmp",-1);
	t[9] = cvLoadImage("image\\t10.bmp",-1);
	t[10] = cvLoadImage("image\\t11.bmp",-1);
	t[11] = cvLoadImage("image\\t12.bmp",-1);
	t[12] = cvLoadImage("image\\t13.bmp",-1);
	t[13] = cvLoadImage("image\\t14.bmp",-1);
	t[14] = cvLoadImage("image\\t15.bmp",-1);
	t[15] = cvLoadImage("image\\t16.bmp",-1);
	t[16] = cvLoadImage("image\\t17.bmp",-1);
	t[17] = cvLoadImage("image\\t18.bmp",-1);
	t[18] = cvLoadImage("image\\t19.bmp",-1);
	t[19] = cvLoadImage("image\\t20.bmp",-1);
	t[20] = cvLoadImage("image\\t21.bmp",-1);
	t[21] = cvLoadImage("image\\t22.bmp",-1);
	t[22] = cvLoadImage("image\\t23.bmp",-1);
	t[23] = cvLoadImage("image\\t24.bmp",-1);
	t[24] = cvLoadImage("image\\t25.bmp",-1);
	t[25] = cvLoadImage("image\\t26.bmp",-1);
	t[26] = cvLoadImage("image\\t27.bmp",-1);
	//
	s[0] = cvLoadImage("image\\s01.bmp",-1);
	s[1] = cvLoadImage("image\\s02.bmp",-1);
	s[2] = cvLoadImage("image\\s03.bmp",-1);
	s[3] = cvLoadImage("image\\s04.bmp",-1);
	s[4] = cvLoadImage("image\\s05.bmp",-1);
	s[5] = cvLoadImage("image\\s06.bmp",-1);
	
	//
	// binary image 생성
	//
	for(int i = 0; i < 3; i++){
		ImageToBinarry(r[i], &b_r[i]);
	}
	for(int i = 0; i < 27; i++){
		ImageToBinarry(t[i], &b_t[i]);
	}
	for(int i = 0; i < 6; i++){
		ImageToBinarry(s[i], &b_s[i]);
	}
	
	//
	// 제거 연산
	//
	for(int i = 0; i < 3; i++){
		ImageClose(&b_r[i]);
	}
	for(int i = 0; i < 27; i++){
		ImageClose(&b_t[i]);
	}
	for(int i = 0; i < 6; i++){
		ImageClose(&b_s[i]);
	}
		
	//
	// feature vector 추출 및 저장
	//
	for(int i = 0; i < 3; i++){
		ImageArea(&b_r[i]);
		ImagePerimeter(&b_r[i]);
		ImageMBR(&b_r[i]);
		b_r[i].rectangularity = (double)b_r[i].area/b_r[i].MBR;
		b_r[i].thickness = (double)b_r[i].area/(b_r[i].perimeter*b_r[i].perimeter);
	}
	for(int i = 0; i < 27; i++){
		ImageArea(&b_t[i]);
		ImagePerimeter(&b_t[i]);
		ImageMBR(&b_t[i]);
		b_t[i].rectangularity = (double)b_t[i].area/b_t[i].MBR;
		b_t[i].thickness = (double)b_t[i].area/(b_t[i].perimeter*b_t[i].perimeter);
	}
	for(int i = 0; i < 6; i++){
		ImageArea(&b_s[i]);
		ImagePerimeter(&b_s[i]);
		ImageMBR(&b_s[i]);
		b_s[i].rectangularity = (double)b_s[i].area/b_s[i].MBR;
		b_s[i].thickness = (double)b_s[i].area/(b_s[i].perimeter*b_s[i].perimeter);
	}

	double w1 = 1;
	double w2 = 1;
	for(int i = 0; i < 27; i++){
		for(int j = 0; j < 3; j++){
			b_t[i].distance[j] = sqrt(w1*(b_r[j].rectangularity - b_t[i].rectangularity) * (b_r[j].rectangularity - b_t[i].rectangularity)
								+ w2*(b_r[j].thickness - b_t[i].thickness) * (b_r[j].thickness - b_t[i].thickness));
		}
	}

	// 테이블 그리기
	for(int i = 0; i < 27; i++){
		cout << "<< t" << i+1 << " >>" << endl;
		printTable(&b_t[i], i);
		cout << endl;
	}

	// 정확도 출력
	cout << "1순위에 대한 정확도 : " << (double) accuracy / 27 * 100 << "% (" << accuracy << "/" << "27)" << endl;

	// 메모리 할당 해제
	for(int i = 0; i < 3; i++){
		cvReleaseImage(&r[i]);
		ImageRelease(&b_r[i]);
	}
	for(int i = 0; i < 27; i++){
		cvReleaseImage(&t[i]);
		ImageRelease(&b_t[i]);
	}
	for(int i = 0; i < 6; i++){
		cvReleaseImage(&s[i]);
		ImageRelease(&b_s[i]);
	}	
}

void ImageToBinarry(IplImage *image, imageData *b_image)
{
	CvScalar value;
	b_image->height = image->height;
	b_image->width = image->width;

	// 학습 이미지를 저장하기 위한 배열을 동적 할당 및 배열에 학습이미지 저장
	b_image->i_image = new int* [b_image->height];
	for(int i = 0; i < b_image->height; i++)
	{
		b_image->i_image[i] = new int [b_image->width];
	}

	for(int i = 0; i < b_image->height; i++)
	{
		for(int j = 0; j < b_image->width; j++)
		{
			value = cvGet2D(image, i, j) ;
			if(value.val[0] < 10)
			{
				b_image->i_image[i][j] = 0;
			}
			else
			{
				b_image->i_image[i][j] = 1;
			}
		}
	}
}

void ImagePrint(imageData *b_image)
{
	for(int i = 0; i < b_image->height; i++)
	{
		for(int j = 0; j < b_image->width; j++)
		{
			cout << b_image->i_image[i][j];
		}
		cout << endl;
	}
}

void ImageRelease(imageData *b_image)
{
	for(int i = 0; i < b_image->height; i++)
	{
		delete [] b_image->i_image[i];
	}
	delete [] b_image->i_image;
}

void ImageClose(imageData *b_image)
{
	int **temp;
	// 임시변수 할당
	temp = new int* [b_image->height];
	for(int i = 0; i < b_image->height; i++)
	{
		temp[i] = new int [b_image->width];
	}

	// 침식 연산
	for(int i = 1; i < b_image->height - 1; i++)
	{
		for(int j = 1; j < b_image->width - 1; j++)
		{
			if( b_image->i_image[i-1][j-1] == 1 &&
				b_image->i_image[i-1][j] == 1 &&
				b_image->i_image[i-1][j+1] == 1 &&
				b_image->i_image[i][j-1] == 1 &&
				b_image->i_image[i][j] == 1 &&
				b_image->i_image[i][j+1] == 1 &&
				b_image->i_image[i+1][j-1] == 1 &&
				b_image->i_image[i+1][j] == 1 &&
				b_image->i_image[i+1][j+1] == 1)
			{
				temp[i][j] = 1;			
			}
			else{
				temp[i][j] = 0;
			}
		}
	}
	// 적용
	for(int i = 1; i < b_image->height - 1; i++)
	{
		for(int j = 1; j < b_image->width - 1; j++)
		{
			b_image->i_image[i][j] = temp[i][j];
		}
	}

	// 팽창 연산
	for(int i = 1; i < b_image->height - 1; i++)
	{
		for(int j = 1; j < b_image->width - 1; j++)
		{
			if(b_image->i_image[i-1][j-1] == 1 ||
				b_image->i_image[i-1][j] == 1 ||
				b_image->i_image[i-1][j+1] == 1 ||
				b_image->i_image[i][j-1] == 1 ||
				b_image->i_image[i][j] == 1 ||
				b_image->i_image[i][j+1] == 1 ||
				b_image->i_image[i+1][j-1] == 1 ||
				b_image->i_image[i+1][j] == 1 ||
				b_image->i_image[i+1][j+1] == 1)
			{
				temp[i][j] = 1;			
			}
			else{
				temp[i][j] = 0;
			}
		}
	}
	// 적용
	for(int i = 1; i < b_image->height - 1; i++)
	{
		for(int j = 1; j < b_image->width - 1; j++)
		{
			b_image->i_image[i][j] = temp[i][j];
		}
	}
	
	// delete
	for(int i = 0; i < b_image->height; i++)
	{
		delete [] temp[i];
	}
	delete [] temp;
}

void ImageArea(imageData *b_image)
{
	int count = 0;
	for(int i = 0; i < b_image->height; i++)
	{
		for(int j = 0; j < b_image->width; j++)
		{
			if(b_image->i_image[i][j] == 1)
			{
				count++;
			}
		}
	}
	b_image->area = count;
}

void ImagePerimeter(imageData *b_image)
{
	int count = 0;
	for(int i = 1; i < b_image->height - 1; i++)
	{
		for(int j = 1; j < b_image->width - 1; j++)
		{
			if(b_image->i_image[i][j] == 1)
			{
				if(	b_image->i_image[i-1][j] == 0 ||
					b_image->i_image[i][j-1] == 0 ||
					b_image->i_image[i][j+1] == 0 ||
					b_image->i_image[i+1][j] == 0 )
				{
					count++;		
				}
			}
		}
	}
	b_image->perimeter = count;
}

void ImageMBR(imageData *b_image)
{
	int min_x = 9999, max_x = -1, min_y = 9999, max_y = -1;
	for(int i = 0; i < b_image->height; i++)
	{
		for(int j = 0; j < b_image->width; j++)
		{
			if(b_image->i_image[i][j] == 1)
			{
				if(min_x > j)
					min_x = j;
				if(max_x < j)
					max_x = j;
				if(min_y > i)
					min_y = i;
				if(max_y < i)
					max_y = i;
			}
		}
	}
	b_image->MBR = (max_x - min_x)*(max_y - min_y);
}

void printTable(imageData *b_image, int index)
{
	int rank[3] = {2, 2, 2};
	if(b_image->distance[0] < b_image->distance[1] && b_image->distance[0] < b_image->distance[2]){
		rank[0] = 1;
	}
	if(b_image->distance[1] < b_image->distance[0] && b_image->distance[1] < b_image->distance[2]){
		rank[1] = 1;
	}
	if(b_image->distance[2] < b_image->distance[0] && b_image->distance[2] < b_image->distance[1]){
		rank[2] = 1;
	}
	if(b_image->distance[0] > b_image->distance[1] && b_image->distance[0] > b_image->distance[2]){
		rank[0] = 3;
	}
	if(b_image->distance[1] > b_image->distance[0] && b_image->distance[1] > b_image->distance[2]){
		rank[1] = 3;
	}
	if(b_image->distance[2] > b_image->distance[0] && b_image->distance[2] > b_image->distance[1]){
		rank[2] = 3;
	}
	cout << "| 원 r1 | " << rank[0] << endl;
	cout << "| 방 r2 | " << rank[1] << endl;
	cout << "| 각 r3 | " << rank[2] << endl;

	if(index < 9 && rank[0] == 1){
		accuracy++;
	}
	if(index >= 9){
		if(index < 18 && rank[1] == 1){
			accuracy++;
		}
		if(index >= 18){
			if(index < 27 && rank[2] == 1){
				accuracy++;
			}
		}
	}
}