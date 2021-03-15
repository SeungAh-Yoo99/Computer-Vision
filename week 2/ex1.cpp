#pragma warning(disable:4996) // visual studio에서 발생시키는 에러를 막기 위해 추가한 코드
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

int lenna()
{
	// 1단계: 영상 입력
	BITMAPFILEHEADER hf; // 14바이트
	BITMAPINFOHEADER hInfo; // 40바이트
	RGBQUAD hRGB[256]; // 1024바이트

	FILE* fp;
	fp = fopen("LENNA.bmp", "rb");
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
	for (int i = 0; i < ImgSize; i++)
	{
		Output[i] = 255 - Image[i];
	}

	// 3단계: 처리된 결과 출력
	fp = fopen("LENNA-output.bmp", "wb");

	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);

	fclose(fp);

	free(Image);
	free(Output); // 해주지 않을 시 memory leak 현상이 발생

	return 0;
}

int coin()
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
	for (int i = 0; i < ImgSize; i++)
	{
		Output[i] = 255 - Image[i];
	}

	// 3단계: 처리된 결과 출력
	fp = fopen("coin-output.bmp", "wb");

	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);

	fclose(fp);

	free(Image);
	free(Output); // 해주지 않을 시 memory leak 현상이 발생

	return 0;
}

int main()
{
	int lennaOutput = lenna();
	int coinOutput = coin();

	if (lennaOutput == -1)
		return -1;
	else if (coinOutput == -1)
		return -1;
	else
		return 0;
}