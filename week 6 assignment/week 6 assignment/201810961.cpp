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


// GlassFire 알고리즘을 이용한 라벨링 함수
void m_BlobColoring(BYTE* CutImage, int height, int width)
{
	int i, j, m, n, top, area, Out_Area, index, BlobArea[1000];
	long k;
	short curColor = 0, r, c;
	//	BYTE** CutImage2;
	Out_Area = 1;


	// 스택으로 사용할 메모리 할당
	short* stackx = new short[height * width];
	short* stacky = new short[height * width];
	short* coloring = new short[height * width];

	int arr_size = height * width;

	// 라벨링된 픽셀을 저장하기 위해 메모리 할당

	for (k = 0; k < height * width; k++) coloring[k] = 0;  // 메모리 초기화

	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			// 이미 방문한 점이거나 픽셀값이 255가 아니라면 처리 안함
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
						//관심 픽셀이 영상경계를 벗어나면 처리 안함
						if (m < 0 || m >= height || n < 0 || n >= width) continue;

						if ((int)CutImage[index + n] == 255 && coloring[index + n] == 0)
						{
							coloring[index + n] = curColor; // 현재 라벨로 마크
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

	// 가장 면적이 넓은 영역을 찾아내기 위함 
	for (i = 1; i <= curColor; i++)
	{
		if (BlobArea[i] >= BlobArea[Out_Area]) Out_Area = i;
	}
	// CutImage 배열 클리어~
	for (k = 0; k < width * height; k++) CutImage[k] = 255;

	// coloring에 저장된 라벨링 결과중 (Out_Area에 저장된) 영역이 가장 큰 것만 CutImage에 저장
	for (k = 0; k < width * height; k++)
	{
		if (coloring[k] == Out_Area) CutImage[k] = 0;  // 가장 큰 것만 저장 (size filtering)
		// if (BlobArea[coloring[k]]>500) CutImage[k] = 0;  // 특정 면적 이상되는 영역만 출력
		// CutImage[k] = (unsigned char)(coloring[k] * grayGap);
	}

	delete[] coloring;
	delete[] stackx;
	delete[] stacky;
}
// 라벨링 후 가장 넓은 영역에 대해서만 뽑아내는 코드 포함

void BinaryEdgeDetection(BYTE* Bin, BYTE* Out, int W, int H)
{
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (Bin[i * W + j] == 0) { // 전경화소라면
				if (!(Bin[(i - 1) * W + j] == 0 && Bin[(i + 1) * W + j] == 0 &&
					Bin[i * W + j - 1] == 0 && Bin[i * W + j + 1] == 0)) // 4방향 화소 중 하나라도 전경이 아니라면
					Out[i * W + j] = 255;
			}
		}
	}
}

// 입력 받은 좌표로 직사각형 그리기
void DrawRectOutline(BYTE* Img, int W, int H, int LU_X, int LU_Y, int RD_X, int RD_Y)
{
	for (int i = LU_X + LU_Y * W; i <= RD_X + LU_Y * W; i++) Img[i] = 255; // 위 가로 선
	for (int i = LU_X + RD_Y * W; i <= RD_X + RD_Y * W; i++) Img[i] = 255; // 아래 가로 선
	for (int i = LU_X + LU_Y * W; i <= LU_X + RD_Y * W; i += W) Img[i] = 255; // 왼쪽 세로 선
	for (int i = RD_X + LU_Y * W; i <= RD_X + RD_Y * W; i += W) Img[i] = 255; // 오른쪽 세로 선
}

// 입력 받은 좌표에 십자 그리기
void DrawCrossLine(BYTE* Img, int W, int H, int Cx, int Cy)
{
	for (int i = Cy * W; i < (Cy + 1) * W; i++) Img[i] = 255; // 가로 선
	for (int i = Cx; i < W * H; i += W) Img[i] = 255; // 세로 선
}

// 이진영상에서 원의 무게중심 구하기
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
	// 1단계: 영상 입력
	BITMAPFILEHEADER hf; // 14바이트
	BITMAPINFOHEADER hInfo; // 40바이트
	RGBQUAD hRGB[256]; // 1024바이트

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
	BYTE* Image = (BYTE*)malloc(ImgSize); // BYTE Image[ImgSize]와 같은 의미(단, 변수는 배열의 크기가 될 수 없으므로 이는 문법에 맞지 않음.)
	BYTE* Temp = (BYTE*)malloc(ImgSize); // 임시 배열
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);

	fclose(fp);

	int H = hInfo.biHeight, W = hInfo.biWidth;

	// 2단계: 영상처리
	Binarization(Image, Temp, W, H, 50);
	InverseImage(Temp, Temp, W, H);
	m_BlobColoring(Temp, H, W);
	for (int i = 0; i < ImgSize; i++) Output[i] = Image[i];
	int LU_X = 0, LU_Y = 0, RD_X = 0, RD_Y = 0;

	// 1) 동공 영역에 외접하는 직사각형
	Obtain2DBoundingBox(Temp, W, H, &LU_X, &LU_Y, &RD_X, &RD_Y);
	DrawRectOutline(Output, W, H, LU_X, LU_Y, RD_X, RD_Y);

	// 2) 동공 영역의 무게중심을 통과하는 수평선/수직선
	int cenx, ceny;
	Obtain2DCenter(LU_X, LU_Y, RD_X, RD_Y, &cenx, &ceny);
	DrawCrossLine(Output, W, H, cenx, ceny);


	// 3단계: 처리된 결과 출력
	SaveBMPFile(hf, hInfo, hRGB, Output, W, H, "pupil1_output.bmp");
	// SaveBMPFile(hf, hInfo, hRGB, Output, W, H, "pupil2_output.bmp");

	free(Image);
	free(Temp);
	free(Output); // 해주지 않을 시 memory leak 현상이 발생

	return 0;
}