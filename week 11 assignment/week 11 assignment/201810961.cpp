#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo,
	RGBQUAD* hRGB, BYTE* Output, int W, int H, const char* FileName)
{
	FILE* fp = fopen(FileName, "wb");
	if (hInfo.biBitCount == 24) {
		fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
		fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
		fwrite(Output, sizeof(BYTE), W * H * 3, fp);
	}
	else if (hInfo.biBitCount == 8) {
		fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
		fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
		fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
		fwrite(Output, sizeof(BYTE), W * H, fp);
	}
	fclose(fp);
}

void RGB2YCbCr(BYTE* Image, BYTE* Y, BYTE* Cb, BYTE* Cr, int W, int H)
{
	for (int i = 0; i < H; i++) { // Y좌표
		for (int j = 0; j < W; j++) { // X좌표
			Y[i * W + j] = (BYTE)(0.299 * Image[i * W * 3 + j * 3 + 2] + 0.587 * Image[i * W * 3 + j * 3 + 1] + 0.114 * Image[i * W * 3 + j * 3]);
			Cb[i * W + j] = (BYTE)(-0.16874 * Image[i * W * 3 + j * 3 + 2] - 0.3313 * Image[i * W * 3 + j * 3 + 1] + 0.5 * Image[i * W * 3 + j * 3] + 128.0);
			Cr[i * W + j] = (BYTE)(0.5 * Image[i * W * 3 + j * 3 + 2] - 0.4187 * Image[i * W * 3 + j * 3 + 1] - 0.0813 * Image[i * W * 3 + j * 3] + 128.0);
		}
	}
}

void Obtain2DBoundingBox(BYTE* Image, int W, int H, int* LU_X, int* LU_Y, int* RD_X, int* RD_Y) {
	int flag = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++)
			if (Image[i * W * 3 + j * 3] != 0 || Image[i * W * 3 + j * 3 + 1] != 0 || Image[i * W * 3 + j * 3 + 2] != 0) {
				*LU_Y = i;
				flag = 1;
				break;
			}
		if (flag == 1) break;
	}
	flag = 0;
	for (int i = H - 1; i >= 0; i--) {
		for (int j = W - 1; j >= 0; j--)
			if (Image[i * W * 3 + j * 3] != 0 || Image[i * W * 3 + j * 3 + 1] != 0 || Image[i * W * 3 + j * 3 + 2] != 0) {
				*RD_Y = i;
				flag = 1;
				break;
			}
		if (flag == 1) break;
	}
	flag = 0;
	for (int i = 0; i < W; i++) {
		for (int j = 0; j < H; j++)
			if (Image[i * 3 + W * j * 3] != 0 || Image[i * 3 + W * j * 3 + 1] != 0 || Image[i * 3 + W * j * 3 + 2] != 0) {
				*LU_X = i;
				flag = 1;
				break;
			}
		if (flag == 1) break;
	}
	flag = 0;
	for (int i = W - 1; i >= 0; i--) {
		for (int j = H - 1; j >= 0; j--)
			if (Image[i * 3 + W * j * 3] != 0 || Image[i * 3 + W * j * 3 + 1] != 0 || Image[i * 3 + W * j * 3 + 2] != 0) {
				*RD_X = i;
				flag = 1;
				break;
			}
		if (flag == 1) break;
	}
}

void DrawRectOutline(BYTE* Img, int W, int H, int LU_X, int LU_Y, int RD_X, int RD_Y)
{
	for (int i = LU_X * 3 + LU_Y * W * 3 + 2; i <= RD_X * 3 + LU_Y * W * 3 + 2; i += 3) Img[i] = 255; // 위 가로 선
	for (int i = LU_X * 3 + RD_Y * W * 3 + 2; i <= RD_X * 3 + RD_Y * W * 3 + 2; i += 3) Img[i] = 255; // 아래 가로 선
	for (int i = LU_X * 3 + LU_Y * W * 3 + 2; i <= LU_X * 3 + RD_Y * W * 3 + 2; i += W * 3) Img[i] = 255; // 왼쪽 세로 선
	for (int i = RD_X * 3 + LU_Y * W * 3 + 2; i <= RD_X * 3 + RD_Y * W * 3 + 2; i += W * 3) Img[i] = 255; // 오른쪽 세로 선
}

int main()
{
	BITMAPFILEHEADER hf; // 14바이트
	BITMAPINFOHEADER hInfo; // 40바이트
	RGBQUAD hRGB[256]; // 1024바이트
	FILE* fp;
	fp = fopen("face.bmp", "rb");
	if (fp == NULL) {
		printf("File not found!\n");
		return -1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	int H = hInfo.biHeight, W = hInfo.biWidth;
	BYTE* Image;
	BYTE* Temp;
	BYTE* Output;
	if (hInfo.biBitCount == 24) { // 트루컬러
		Image = (BYTE*)malloc(ImgSize * 3);
		Temp = (BYTE*)malloc(ImgSize * 3);
		Output = (BYTE*)malloc(ImgSize * 3);
		fread(Image, sizeof(BYTE), ImgSize * 3, fp);
	}
	else { // 인덱스(그레이)
		fread(hRGB, sizeof(RGBQUAD), 256, fp);
		Image = (BYTE*)malloc(ImgSize);
		Temp = (BYTE*)malloc(ImgSize);
		Output = (BYTE*)malloc(ImgSize);
		fread(Image, sizeof(BYTE), ImgSize, fp);
	}
	fclose(fp);

	BYTE* Y = (BYTE*)malloc(ImgSize);
	BYTE* Cb = (BYTE*)malloc(ImgSize);
	BYTE* Cr = (BYTE*)malloc(ImgSize);

	RGB2YCbCr(Image, Y, Cb, Cr, W, H);

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (Cb[i * W + j] > 95 && Cb[i * W + j] < 120 && Cr[i * W + j] > 140 && Cr[i * W + j] < 185) {
				Temp[i * W * 3 + j * 3] = Image[i * W * 3 + j * 3];
				Temp[i * W * 3 + j * 3 + 1] = Image[i * W * 3 + j * 3 + 1];
				Temp[i * W * 3 + j * 3 + 2] = Image[i * W * 3 + j * 3 + 2];
			}
			else
				Temp[i * W * 3 + j * 3] = Temp[i * W * 3 + j * 3 + 1] = Temp[i * W * 3 + j * 3 + 2] = 0;
		}
	}

	for (int i = 0; i < ImgSize * 3; i++) Output[i] = Image[i];
	int LU_X = 0, LU_Y = 0, RD_X = 0, RD_Y = 0;

	Obtain2DBoundingBox(Temp, W, H, &LU_X, &LU_Y, &RD_X, &RD_Y);
	DrawRectOutline(Output, W, H, LU_X, LU_Y, RD_X, RD_Y);

	free(Y);
	free(Cb);
	free(Cr);

	SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "output.bmp");

	free(Image);
	free(Output);
	//free(Temp);
	return 0;
}