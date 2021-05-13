#pragma warning(disable:4996) // visual studio에서 발생시키는 에러를 막기 위해 추가한 코드
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo, RGBQUAD* hRGB, BYTE* Output, int W, int H, const char* FileName)
{
	FILE* fp = fopen(FileName, "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), W * H, fp);

	fclose(fp);
}

void swap(BYTE* a, BYTE* b)
{
	BYTE temp = *a;
	*a = *b;
	*b = temp;
}

BYTE Median(BYTE* Arr, int Size)
{
	// 오름차순 정렬
	const int S = Size;
	for (int i = 0; i < Size - 1; i++) { // Pivot index
		for (int j = i + 1; j < Size; j++) {
			if (Arr[i] > Arr[j])
				swap(&Arr[i], &Arr[j]);
		}
	}

	return Arr[S / 2];
}

BYTE MaxPooling(BYTE* Arr, int Size)
{
	// 오름차순 정렬
	const int S = Size;
	for (int i = 0; i < Size - 1; i++) { // Pivot index
		for (int j = i + 1; j < Size; j++) {
			if (Arr[i] > Arr[j])
				swap(&Arr[i], &Arr[j]);
		}
	}

	return Arr[S - 1];
}

BYTE MinPooling(BYTE* Arr, int Size)
{
	// 오름차순 정렬
	const int S = Size;
	for (int i = 0; i < Size - 1; i++) { // Pivot index
		for (int j = i + 1; j < Size; j++) {
			if (Arr[i] > Arr[j])
				swap(&Arr[i], &Arr[j]);
		}
	}

	return Arr[0];
}

int main()
{
	// 1단계: 영상 입력
	BITMAPFILEHEADER hf; // 14바이트
	BITMAPINFOHEADER hInfo; // 40바이트
	RGBQUAD hRGB[256]; // 1024바이트

	FILE* fp;
	fp = fopen("lenna_impulse.bmp", "rb");
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
	BYTE Temp[9];
	int W = hInfo.biWidth, H = hInfo.biHeight;
	for (int i = 1; i < H - 1; i++) {
		for (int j = 1; j < W - 1; j++) {
			for (int m = -1; m <= 1; m++) {
				for (int n = -1; n <= 1; n++) {
					Temp[(m + 1) * 3 + (n + 1)] = Image[(i + m) * W + (j + n)];
				}
			}
			// Output[i * W + j] = Median(Temp, 9);
			// Output[i * W + j] = MaxPooling(Temp, 9);
			Output[i * W + j] = MinPooling(Temp, 9);
		}
	}

	// 3단계: 처리된 결과 출력
	// SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "lenna_impulse_median.bmp");
	// SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "lenna_impulse_MaxPooling.bmp");
	SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "lenna_impulse_MinPooling.bmp");

	free(Image);
	free(Output); // 해주지 않을 시 memory leak 현상이 발생

	return 0;
}