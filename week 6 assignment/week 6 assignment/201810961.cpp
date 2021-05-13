#pragma warning(disable:4996) // visual studio���� �߻���Ű�� ������ ���� ���� �߰��� �ڵ�
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

void Binarization(BYTE* Img, BYTE* Out, int W, int H, BYTE Threshold)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] < Threshold) Out[i] = 0;
		else Out[i] = 255;
	}
}

void InverseImage(BYTE* Img, BYTE* Out, int W, int H)
{
	int ImgSize = W * H;

	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = 255 - Img[i];
	}
}

int push(short* stackx, short* stacky, int arr_size, short vx, short vy, int* top)
{
	if (*top >= arr_size) return(-1);
	(*top)++;
	stackx[*top] = vx;
	stacky[*top] = vy;
	return(1);
}

int pop(short* stackx, short* stacky, short* vx, short* vy, int* top)
{
	if (*top == 0) return(-1);
	*vx = stackx[*top];
	*vy = stacky[*top];
	(*top)--;
	return(1);
}


// GlassFire �˰����� �̿��� �󺧸� �Լ�
void m_BlobColoring(BYTE* CutImage, int height, int width)
{
	int i, j, m, n, top, area, Out_Area, index, BlobArea[1000];
	long k;
	short curColor = 0, r, c;
	//	BYTE** CutImage2;
	Out_Area = 1;


	// �������� ����� �޸� �Ҵ�
	short* stackx = new short[height * width];
	short* stacky = new short[height * width];
	short* coloring = new short[height * width];

	int arr_size = height * width;

	// �󺧸��� �ȼ��� �����ϱ� ���� �޸� �Ҵ�

	for (k = 0; k < height * width; k++) coloring[k] = 0;  // �޸� �ʱ�ȭ

	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			// �̹� �湮�� ���̰ų� �ȼ����� 255�� �ƴ϶�� ó�� ����
			if (coloring[index + j] != 0 || CutImage[index + j] != 255) continue;
			r = i; c = j; top = 0; area = 1;
			curColor++;

			while (1)
			{
			GRASSFIRE:
				for (m = r - 1; m <= r + 1; m++)
				{
					index = m * width;
					for (n = c - 1; n <= c + 1; n++)
					{
						//���� �ȼ��� �����踦 ����� ó�� ����
						if (m < 0 || m >= height || n < 0 || n >= width) continue;

						if ((int)CutImage[index + n] == 255 && coloring[index + n] == 0)
						{
							coloring[index + n] = curColor; // ���� �󺧷� ��ũ
							if (push(stackx, stacky, arr_size, (short)m, (short)n, &top) == -1) continue;
							r = m; c = n; area++;
							goto GRASSFIRE;
						}
					}
				}
				if (pop(stackx, stacky, &r, &c, &top) == -1) break;
			}
			if (curColor < 1000) BlobArea[curColor] = area;
		}
	}

	float grayGap = 255.0f / (float)curColor;

	// ���� ������ ���� ������ ã�Ƴ��� ���� 
	for (i = 1; i <= curColor; i++)
	{
		if (BlobArea[i] >= BlobArea[Out_Area]) Out_Area = i;
	}
	// CutImage �迭 Ŭ����~
	for (k = 0; k < width * height; k++) CutImage[k] = 255;

	// coloring�� ����� �󺧸� ����� (Out_Area�� �����) ������ ���� ū �͸� CutImage�� ����
	for (k = 0; k < width * height; k++)
	{
		if (coloring[k] == Out_Area) CutImage[k] = 0;  // ���� ū �͸� ���� (size filtering)
		// if (BlobArea[coloring[k]]>500) CutImage[k] = 0;  // Ư�� ���� �̻�Ǵ� ������ ���
		// CutImage[k] = (unsigned char)(coloring[k] * grayGap);
	}

	delete[] coloring;
	delete[] stackx;
	delete[] stacky;
}
// �󺧸� �� ���� ���� ������ ���ؼ��� �̾Ƴ��� �ڵ� ����

void BinaryEdgeDetection(BYTE* Bin, BYTE* Out, int W, int H)
{
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (Bin[i * W + j] == 0) { // ����ȭ�Ҷ��
				if (!(Bin[(i - 1) * W + j] == 0 && Bin[(i + 1) * W + j] == 0 &&
					Bin[i * W + j - 1] == 0 && Bin[i * W + j + 1] == 0)) // 4���� ȭ�� �� �ϳ��� ������ �ƴ϶��
					Out[i * W + j] = 255;
			}
		}
	}
}

// �Է� ���� ��ǥ�� ���簢�� �׸���
void DrawRectOutline(BYTE* Img, int W, int H, int LU_X, int LU_Y, int RD_X, int RD_Y)
{
	for (int i = LU_X + LU_Y * W; i <= RD_X + LU_Y * W; i++) Img[i] = 255; // �� ���� ��
	for (int i = LU_X + RD_Y * W; i <= RD_X + RD_Y * W; i++) Img[i] = 255; // �Ʒ� ���� ��
	for (int i = LU_X + LU_Y * W; i <= LU_X + RD_Y * W; i += W) Img[i] = 255; // ���� ���� ��
	for (int i = RD_X + LU_Y * W; i <= RD_X + RD_Y * W; i += W) Img[i] = 255; // ������ ���� ��
}

// �Է� ���� ��ǥ�� ���� �׸���
void DrawCrossLine(BYTE* Img, int W, int H, int Cx, int Cy)
{
	for (int i = Cy * W; i < (Cy + 1) * W; i++) Img[i] = 255; // ���� ��
	for (int i = Cx; i < W * H; i += W) Img[i] = 255; // ���� ��
}

// �������󿡼� ���� �����߽� ���ϱ�
void Obtain2DCenter(int LU_X, int LU_Y, int RD_X, int RD_Y, int* cenx, int* ceny)
{
	int sumx = 0, sumy = 0;
	for (int i = LU_X; i <= RD_X; i++)
		sumx += i;
	if (RD_X - LU_X != 0)
		*cenx = sumx / (RD_X - LU_X);

	for (int i = LU_Y; i <= RD_Y; i++)
		sumy += i;
	if (RD_Y - LU_Y != 0)
		*ceny = sumy / (RD_Y - LU_Y);
}

void Obtain2DBoundingBox(BYTE* Image, int W, int H, int* LU_X, int* LU_Y, int* RD_X, int* RD_Y) {
	int flag = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++)
			if (Image[i * W + j] == 0) {
				*LU_Y = i;
				flag = 1;
				break;
			}
		if (flag == 1) break;
	}
	flag = 0;
	for (int i = H - 1; i >= 0; i--) {
		for (int j = W - 1; j >= 0; j--)
			if (Image[i * W + j] == 0) {
				*RD_Y = i;
				flag = 1;
				break;
			}
		if (flag == 1) break;
	}
	flag = 0;
	for (int i = 0; i < W; i++) {
		for (int j = 0; j < H; j++)
			if (Image[i + W * j] == 0) {
				*LU_X = i;
				flag = 1;
				break;
			}
		if (flag == 1) break;
	}
	flag = 0;
	for (int i = W - 1; i >= 0; i--) {
		for (int j = H - 1; j >= 0; j--)
			if (Image[i + W * j] == 0) {
				*RD_X = i;
				flag = 1;
				break;
			}
		if (flag == 1) break;
	}
}

int main()
{
	// 1�ܰ�: ���� �Է�
	BITMAPFILEHEADER hf; // 14����Ʈ
	BITMAPINFOHEADER hInfo; // 40����Ʈ
	RGBQUAD hRGB[256]; // 1024����Ʈ

	FILE* fp;
	fp = fopen("pupil1.bmp", "rb");
	//fp = fopen("pupil2.bmp", "rb");
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

	int H = hInfo.biHeight, W = hInfo.biWidth;

	// 2�ܰ�: ����ó��
	Binarization(Image, Temp, W, H, 50);
	InverseImage(Temp, Temp, W, H);
	m_BlobColoring(Temp, H, W);
	for (int i = 0; i < ImgSize; i++) Output[i] = Image[i];
	int LU_X = 0, LU_Y = 0, RD_X = 0, RD_Y = 0;

	// 1) ���� ������ �����ϴ� ���簢��
	Obtain2DBoundingBox(Temp, W, H, &LU_X, &LU_Y, &RD_X, &RD_Y);
	DrawRectOutline(Output, W, H, LU_X, LU_Y, RD_X, RD_Y);

	// 2) ���� ������ �����߽��� ����ϴ� ����/������
	int cenx, ceny;
	Obtain2DCenter(LU_X, LU_Y, RD_X, RD_Y, &cenx, &ceny);
	DrawCrossLine(Output, W, H, cenx, ceny);


	// 3�ܰ�: ó���� ��� ���
	SaveBMPFile(hf, hInfo, hRGB, Output, W, H, "pupil1_output.bmp");
	// SaveBMPFile(hf, hInfo, hRGB, Output, W, H, "pupil2_output.bmp");

	free(Image);
	free(Temp);
	free(Output); // ������ ���� �� memory leak ������ �߻�

	return 0;
}