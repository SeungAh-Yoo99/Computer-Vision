#pragma warning(disable:4996) // visual studio에서 발생시키는 에러를 막기 위해 추가한 코드
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void AverageConv(BYTE* Img, BYTE* Out, int W, int H) // 박스 평활화
{
	double Kernel[3][3] = { 0.11111, 0.11111, 0.11111,
						    0.11111, 0.11111, 0.11111,
						    0.11111, 0.11111, 0.11111 };

	double SumProduct = 0.0;

	for (int i = 1; i < H-1; i++) { // Y좌표 (행)
		for (int j = 1; j < W-1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i+m)*W+(j+n)] * Kernel[m+1][n+1];
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void GaussAvrConv(BYTE* Img, BYTE* Out, int W, int H) // 가우시안 평활화
{
	double Kernel[3][3] = { 0.0625, 0.125, 0.0625,
						    0.125, 0.25, 0.125,
						    0.0625, 0.125, 0.0625 };

	double SumProduct = 0.0;

	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void Prewitt_X_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스트 X
{
	double Kernel[3][3] = { -1.0, 0.0, 1.0,
						    -1.0, 0.0, 1.0,
						    -1.0, 0.0, 1.0 };

	double SumProduct = 0.0;

	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765 ====> 0 ~ 255
			Out[i * W + j] = (BYTE)(abs((long)SumProduct / 3));
			SumProduct = 0.0;
		}
	}
}

void Prewitt_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스트 Y
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
							0.0, 0.0, 0.0,
							1.0, 1.0, 1.0 };

	double SumProduct = 0.0;

	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765 ====> 0 ~ 255
			Out[i * W + j] = (BYTE)(abs((long)SumProduct / 3));
			SumProduct = 0.0;
		}
	}
}

void Sobel_X_Conv(BYTE* Img, BYTE* Out, int W, int H) // Sobel 마스트 X
{
	double Kernel[3][3] = { -1.0, 0.0, 1.0,
							-2.0, 0.0, 2.0,
							-1.0, 0.0, 1.0 };

	double SumProduct = 0.0;

	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 1020 ====> 0 ~ 255
			Out[i * W + j] = (BYTE)(abs((long)SumProduct / 4));
			SumProduct = 0.0;
		}
	}
}

void Sobel_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) // Sobel 마스트 Y
{
	double Kernel[3][3] = { -1.0, -2.0, -1.0,
							0.0, 0.0, 0.0,
							1.0, 2.0, 1.0 };

	double SumProduct = 0.0;

	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 1020 ====> 0 ~ 255
			Out[i * W + j] = (BYTE)(abs((long)SumProduct / 4));
			SumProduct = 0.0;
		}
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

void Laplace_Conv(BYTE* Img, BYTE* Out, int W, int H) // Laplace 마스트
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
							-1.0, 8.0, -1.0,
							-1.0, -1.0, -1.0 };

	double SumProduct = 0.0;

	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 2040 ====> 0 ~ 255
			Out[i * W + j] = (BYTE)(abs((long)SumProduct / 8));
			SumProduct = 0.0;
		}
	}
}

void Laplace_Conv_DC(BYTE* Img, BYTE* Out, int W, int H) // Laplace 마스트
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
							-1.0, 9.0, -1.0,
							-1.0, -1.0, -1.0 };

	double SumProduct = 0.0;

	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			if (SumProduct > 255.0) Out[i * W + j] = 255;
			else if (SumProduct < 0.0) Out[i * W + j] = 0;
			else Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

int main()
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
	BYTE* Temp = (BYTE*)malloc(ImgSize); // 임시 배열
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);

	fclose(fp);

	// 2단계: 영상처리
	// AverageConv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	GaussAvrConv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	// Prewitt_X_Conv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	// Prewitt_Y_Conv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	// Sobel_X_Conv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	// Sobel_Y_Conv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	/* for (int i = 0; i < ImgSize; i++) {
		if (Temp[i] > Output[i]) Output[i] = Temp[i];
	} */
	// Binarization(Output, Output, hInfo.biWidth, hInfo.biHeight, 40); // 이진화로 확실한 경계 검출
	// Laplace_Conv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	Laplace_Conv_DC(Temp, Output, hInfo.biWidth, hInfo.biHeight);

	// 3단계: 처리된 결과 출력
	fp = fopen("LENNA-GaussAvrConv-Laplace_Conv_DC.bmp", "wb");

	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);

	fclose(fp);

	free(Image);
	free(Temp);
	free(Output); // 해주지 않을 시 memory leak 현상이 발생

	return 0;
}