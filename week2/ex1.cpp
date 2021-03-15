#pragma warning(disable:4996) // visual studio���� �߻���Ű�� ������ ���� ���� �߰��� �ڵ�
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

int lenna()
{
	// 1�ܰ�: ���� �Է�
	BITMAPFILEHEADER hf; // 14����Ʈ
	BITMAPINFOHEADER hInfo; // 40����Ʈ
	RGBQUAD hRGB[256]; // 1024����Ʈ

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
	BYTE* Image = (BYTE*)malloc(ImgSize); // BYTE Image[ImgSize]�� ���� �ǹ�(��, ������ �迭�� ũ�Ⱑ �� �� �����Ƿ� �̴� ������ ���� ����.)
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);

	fclose(fp);

	// 2�ܰ�: ����ó��
	for (int i = 0; i < ImgSize; i++)
	{
		Output[i] = 255 - Image[i];
	}

	// 3�ܰ�: ó���� ��� ���
	fp = fopen("LENNA-output.bmp", "wb");

	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);

	fclose(fp);

	free(Image);
	free(Output); // ������ ���� �� memory leak ������ �߻�

	return 0;
}

int coin()
{
	// 1�ܰ�: ���� �Է�
	BITMAPFILEHEADER hf; // 14����Ʈ
	BITMAPINFOHEADER hInfo; // 40����Ʈ
	RGBQUAD hRGB[256]; // 1024����Ʈ

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
	BYTE* Image = (BYTE*)malloc(ImgSize); // BYTE Image[ImgSize]�� ���� �ǹ�(��, ������ �迭�� ũ�Ⱑ �� �� �����Ƿ� �̴� ������ ���� ����.)
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);

	fclose(fp);

	// 2�ܰ�: ����ó��
	for (int i = 0; i < ImgSize; i++)
	{
		Output[i] = 255 - Image[i];
	}

	// 3�ܰ�: ó���� ��� ���
	fp = fopen("coin-output.bmp", "wb");

	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);

	fclose(fp);

	free(Image);
	free(Output); // ������ ���� �� memory leak ������ �߻�

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