#pragma warning(disable:4996) // visual studio���� �߻���Ű�� ������ ���� ���� �߰��� �ڵ�
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void AverageConv(BYTE* Img, BYTE* Out, int W, int H) // �ڽ� ��Ȱȭ
{
	double Kernel[3][3] = { 0.11111, 0.11111, 0.11111,
						    0.11111, 0.11111, 0.11111,
						    0.11111, 0.11111, 0.11111 };

	double SumProduct = 0.0;

	for (int i = 1; i < H-1; i++) { // Y��ǥ (��)
		for (int j = 1; j < W-1; j++) { // X��ǥ (��)
			for (int m = -1; m <= 1; m++) { // Kernel ��
				for (int n = -1; n <= 1; n++) { // Kernel ��
					SumProduct += Img[(i+m)*W+(j+n)] * Kernel[m+1][n+1];
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void GaussAvrConv(BYTE* Img, BYTE* Out, int W, int H) // ����þ� ��Ȱȭ
{
	double Kernel[3][3] = { 0.0625, 0.125, 0.0625,
						    0.125, 0.25, 0.125,
						    0.0625, 0.125, 0.0625 };

	double SumProduct = 0.0;

	for (int i = 1; i < H - 1; i++) { // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++) { // X��ǥ (��)
			for (int m = -1; m <= 1; m++) { // Kernel ��
				for (int n = -1; n <= 1; n++) { // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void Prewitt_X_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt ����Ʈ X
{
	double Kernel[3][3] = { -1.0, 0.0, 1.0,
						    -1.0, 0.0, 1.0,
						    -1.0, 0.0, 1.0 };

	double SumProduct = 0.0;

	for (int i = 1; i < H - 1; i++) { // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++) { // X��ǥ (��)
			for (int m = -1; m <= 1; m++) { // Kernel ��
				for (int n = -1; n <= 1; n++) { // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765 ====> 0 ~ 255
			Out[i * W + j] = (BYTE)(abs((long)SumProduct / 3));
			SumProduct = 0.0;
		}
	}
}

void Prewitt_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt ����Ʈ Y
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
							0.0, 0.0, 0.0,
							1.0, 1.0, 1.0 };

	double SumProduct = 0.0;

	for (int i = 1; i < H - 1; i++) { // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++) { // X��ǥ (��)
			for (int m = -1; m <= 1; m++) { // Kernel ��
				for (int n = -1; n <= 1; n++) { // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765 ====> 0 ~ 255
			Out[i * W + j] = (BYTE)(abs((long)SumProduct / 3));
			SumProduct = 0.0;
		}
	}
}

void Sobel_X_Conv(BYTE* Img, BYTE* Out, int W, int H) // Sobel ����Ʈ X
{
	double Kernel[3][3] = { -1.0, 0.0, 1.0,
							-2.0, 0.0, 2.0,
							-1.0, 0.0, 1.0 };

	double SumProduct = 0.0;

	for (int i = 1; i < H - 1; i++) { // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++) { // X��ǥ (��)
			for (int m = -1; m <= 1; m++) { // Kernel ��
				for (int n = -1; n <= 1; n++) { // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 1020 ====> 0 ~ 255
			Out[i * W + j] = (BYTE)(abs((long)SumProduct / 4));
			SumProduct = 0.0;
		}
	}
}

void Sobel_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) // Sobel ����Ʈ Y
{
	double Kernel[3][3] = { -1.0, -2.0, -1.0,
							0.0, 0.0, 0.0,
							1.0, 2.0, 1.0 };

	double SumProduct = 0.0;

	for (int i = 1; i < H - 1; i++) { // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++) { // X��ǥ (��)
			for (int m = -1; m <= 1; m++) { // Kernel ��
				for (int n = -1; n <= 1; n++) { // Kernel ��
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

void Laplace_Conv(BYTE* Img, BYTE* Out, int W, int H) // Laplace ����Ʈ
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
							-1.0, 8.0, -1.0,
							-1.0, -1.0, -1.0 };

	double SumProduct = 0.0;

	for (int i = 1; i < H - 1; i++) { // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++) { // X��ǥ (��)
			for (int m = -1; m <= 1; m++) { // Kernel ��
				for (int n = -1; n <= 1; n++) { // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 2040 ====> 0 ~ 255
			Out[i * W + j] = (BYTE)(abs((long)SumProduct / 8));
			SumProduct = 0.0;
		}
	}
}

void Laplace_Conv_DC(BYTE* Img, BYTE* Out, int W, int H) // Laplace ����Ʈ
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
							-1.0, 9.0, -1.0,
							-1.0, -1.0, -1.0 };

	double SumProduct = 0.0;

	for (int i = 1; i < H - 1; i++) { // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++) { // X��ǥ (��)
			for (int m = -1; m <= 1; m++) { // Kernel ��
				for (int n = -1; n <= 1; n++) { // Kernel ��
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
	BYTE* Temp = (BYTE*)malloc(ImgSize); // �ӽ� �迭
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);

	fclose(fp);

	// 2�ܰ�: ����ó��
	// AverageConv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	GaussAvrConv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	// Prewitt_X_Conv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	// Prewitt_Y_Conv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	// Sobel_X_Conv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	// Sobel_Y_Conv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	/* for (int i = 0; i < ImgSize; i++) {
		if (Temp[i] > Output[i]) Output[i] = Temp[i];
	} */
	// Binarization(Output, Output, hInfo.biWidth, hInfo.biHeight, 40); // ����ȭ�� Ȯ���� ��� ����
	// Laplace_Conv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	Laplace_Conv_DC(Temp, Output, hInfo.biWidth, hInfo.biHeight);

	// 3�ܰ�: ó���� ��� ���
	fp = fopen("LENNA-GaussAvrConv-Laplace_Conv_DC.bmp", "wb");

	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);

	fclose(fp);

	free(Image);
	free(Temp);
	free(Output); // ������ ���� �� memory leak ������ �߻�

	return 0;
}