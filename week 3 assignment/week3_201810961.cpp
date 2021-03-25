#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void ObtainHistogram(BYTE* Img, int* Histo, int W, int H)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		Histo[Img[i]]++;
	}
}

void Binarization(BYTE* Img, BYTE* Out, int W, int H, BYTE Threshold)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] < Threshold) Out[i] = 0;
		else Out[i] = 255;
	}
}

int GozalezBinThresh(int* Histo, int T)
{
	int m1 = 0, m2 = 0, m1NumSum = 0, m2NumSum = 0;

	for (int i = T; i < 256; i++) {
		m1 += i * Histo[i];
		m1NumSum += Histo[i];
	}
	for (int i = 0; i < T; i++) {
		m2 += i * Histo[i];
		m2NumSum += Histo[i];
	}
	if (m1NumSum != 0)
		m1 = m1 / m1NumSum;
	if (m2NumSum != 0)
		m2 = m2 / m2NumSum;

	int t = (m1 + m2) / 2;
	if (((T - t) > -3) && ((T - t) < 3))
		return t;
	else
		GozalezBinThresh(Histo, t);
}

int main()
{
	// 1단계: 영상 입력
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	RGBQUAD hRGB[256];

	FILE* fp;
	fp = fopen("coin.bmp", "rb");
	if (fp == NULL) {
		printf("File not found!\n");
		return -1;
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE* Image = (BYTE*)malloc(ImgSize);
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);

	fclose(fp);

	// 2단계: 영상처리
	int Histo[256] = { 0 };
	ObtainHistogram(Image, Histo, hInfo.biWidth, hInfo.biHeight);

	int T = (0 + 255) / 2;
	int Thres = GozalezBinThresh(Histo, T);
	Binarization(Image, Output, hInfo.biWidth, hInfo.biHeight, Thres);

	// 3단계: 처리된 결과 출력
	fp = fopen("output.bmp", "wb");

	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);

	fclose(fp);

	free(Image);
	free(Output);

	return 0;
}