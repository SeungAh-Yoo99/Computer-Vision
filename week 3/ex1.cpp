#pragma warning(disable:4996) // visual studio에서 발생시키는 에러를 막기 위해 추가한 코드
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void InverseImage(BYTE* Img, BYTE* Out, int W, int H)
{
	int ImgSize = W * H;

	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = 255 - Img[i];
	}
}

void BrightnessAd(BYTE* Img, BYTE* Out, int W, int H, int Val)
{
	int ImgSize = W * H;

	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] + Val > 255)
			Out[i] = 255;
		else if (Img[i] + Val < 0)
			Out[i] = 0;
		else
			Out[i] = Img[i] + Val;
	}
}

void ContrastAd(BYTE* Img, BYTE* Out, int W, int H, double Val)
{
	int ImgSize = W * H;

	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] * Val > 255.0)
			Out[i] = 255;
		else
			Out[i] = (BYTE)(Img[i] * Val);
	}
}

void ObtainHistogram(BYTE* Img, int* Histo, int W, int H)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		Histo[Img[i]]++;
	}
	// for (int i = 0; i < 256; i++) printf("%d\n", Histo[i]);
}

void HistogramStretching(BYTE* Img, BYTE* Out, int* Histo, int W, int H)
{
	int ImgSize = W * H;
	// 영상의 최소값, 최대값 구하기
	BYTE Low = 255, High = 0;
	for (int i = 0; i < 256; i++) {
		if (Histo[i] != 0) {
			Low = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--) {
		if (Histo[i] != 0) {
			High = i;
			break;
		}
	}
	// 스트래칭
	for (int i = 0; i < ImgSize; i++) {
		Out[i] = (BYTE)((Img[i] - Low) / (double)(High - Low) * 255.0);
	}
}

void ObtainAHistogram(int* Histo, int* AHisto)
{
	for (int i = 0; i < 256; i++) {
		AHisto[i] = AHisto[i - 1] + Histo[i];
	}
}

void HistogramEqualization(BYTE* Img, BYTE* Out, int* AHisto, int W, int H)
{
	// 평활화
	int Nt = W * H, Gmax = 255, ImgSize = W * H;
	double Ratio = Gmax / (double)Nt;
	BYTE NormSum[256];
	for (int i = 0; i < 256; i++) {
		NormSum[i] = (BYTE)(Ratio * AHisto[i]);
	}
	for (int i = 0; i < ImgSize; i++) {
		Out[i] = NormSum[Img[i]];
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

int main()
{
	// 1단계: 영상 입력
	BITMAPFILEHEADER hf; // 14바이트
	BITMAPINFOHEADER hInfo; // 40바이트
	RGBQUAD hRGB[256]; // 1024바이트

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
	BYTE* Image = (BYTE*)malloc(ImgSize); // BYTE Image[ImgSize]와 같은 의미(단, 변수는 배열의 크기가 될 수 없으므로 이는 문법에 맞지 않음.)
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);

	fclose(fp);

	// 2단계: 영상처리
	// InverseImage(Image, Output, hInfo.biWidth, hInfo.biHeight); //반전
	// BrightnessAd(Image, Output, hInfo.biWidth, hInfo.biHeight, 70); //밝기 70 up
	// ContrastAd(Image, Output, hInfo.biWidth, hInfo.biHeight, 0.5); // contrast 1.5배
	Binarization(Image, Output, hInfo.biWidth, hInfo.biHeight, 80); // 이진화

	// 영상의 히스토그램 배열 구하기
	// int Histo[256] = { 0 };
	// ObtainHistogram(Image, Histo, hInfo.biWidth, hInfo.biHeight);
	// HistogramStretching(Image, Output, Histo, hInfo.biWidth, hInfo.biHeight); // 스트레칭
	// 누적 히스토그램 배열 구하기
	// int AHisto[256] = { 0 };
	// ObtainAHistogram(Histo, AHisto);
	// HistogramEqualization(Image, Output, AHisto, hInfo.biWidth, hInfo.biHeight); // 평활화

	// 3단계: 처리된 결과 출력
	fp = fopen("coin-Binarization-output.bmp", "wb");

	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);

	fclose(fp);

	free(Image);
	free(Output); // 해주지 않을 시 memory leak 현상이 발생

	return 0;
}