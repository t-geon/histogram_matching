#define _CRT_SECURE_NO_WARNINGS	
#include <stdio.h>

//input image size
#define MAX_X 512
#define MAX_Y 512

//target image size
#define X 350
#define Y 555

unsigned char inputraw[MAX_Y][MAX_X];
int main()
{
	//input
	//unsigned char inputraw[MAX_Y][MAX_X];
	double inp[256] = { 0, };	//index is the depth value of the pixel, data is the frequency

	//input raw open
	FILE* inraw = fopen("barbara(512x512).raw", "rb");
	if (inraw == NULL) { printf("File open error"); return 0; }

	//input raw read
	for (int i = 0; i < MAX_Y; i++)
	{
		for (int j = 0; j < MAX_X; j++)
		{
			inputraw[i][j] = fgetc(inraw);	//Reading total pixel value (repeated MAX_Y*MAX_X times)
			inp[inputraw[i][j]]++;	//The frequency of the appropriate index (pixel value) is increased by one.
		}
	}

	fclose(inraw);				//close inraw
	/*for (int i = 0; i < 256; i++) {
		//printf("%f \n", r_change[i]);
		printf("%d \n", inputraw[0][i]);
	}*/

	double co = MAX_Y * MAX_X;
	double p_inp[256], cdf_inp[256] = { 0, };		//p_inp: Probability for each pixel, cdf_inp: Probability cumulative value for each pixel
	for (int i = 0; i < 256; i++) {
		p_inp[i] = inp[i] / co;

		if (i != 0) { cdf_inp[i] = cdf_inp[i - 1] + p_inp[i]; }
		else { cdf_inp[i] = p_inp[i]; }

		//printf("%f \n", inp[i]);			//print Frequency of depth value for each pixel
		//printf("%f \n", p_inp[i]);		//print Probability of depth value for each pixel
		//printf("%f \n", cdf_inp[i]);		//print Cdf of depth value for each pixel
	}


	//target
	unsigned char targetraw[Y][X];
	double tp[256] = { 0, };	//index is the depth value of the pixel, data is the frequency

	//target raw open
	FILE* traw = fopen("gAirplane350_555.raw", "rb");
	if (traw == NULL) { printf("File open error"); return 0; }

	//target raw read
	for (int i = 0; i < Y; i++)
	{
		for (int j = 0; j < X; j++)
		{
			targetraw[i][j] = fgetc(traw);	//Reading total pixel value (repeated MAX_Y*MAX_X times)
			tp[targetraw[i][j]]++;			//The frequency of the appropriate index (pixel value) is increased by one.
		}
	}

	fclose(traw);				//close traw

	double co1 = Y * X;
	double p_tp[256], cdf_tp[256] = { 0, };		//p_tp: probability for each pixel, cdf_tp: probability accumulation value for each pixel
	for (int i = 0; i < 256; i++) {
		p_tp[i] = tp[i] / co1;		//Probability is put in p_tp

		if (i != 0) { cdf_tp[i] = cdf_tp[i - 1] + p_tp[i]; }
		else { cdf_tp[i] = p_tp[i]; }

		//printf("%f \n", tp[i]);
		//printf("%f \n", t_inp[i])
		//printf("%f \n", cdf_tp[i]);
	}


	//Mapping the input image to the target image using cdf
	//Using the result value obtained by putting the pixel value in the cdf of the input image, find the data value index by searching in cdf_tp
	//The depth value of the pixel to be replaced is stored by mapping the value of cdf_inp to the nearest value from cdf_tp.

	double r_change[256] = { 0, };//index is the original value, data is the value to be changed

	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			if (cdf_inp[i] <= cdf_tp[j]) {	 //Stop when the input image cdf value is less than the target image cdf value.
				if (j == 0) {
					r_change[i] = j;
					break;
				}
				else if (cdf_inp[i] - cdf_tp[j - 1] < cdf_tp[j] - cdf_inp[i]) {
					r_change[i] = j - 1;	//The resulting image should change i to j-1.
					break;
				}
				else {	//If the latter is smaller or equal when comparing the sizes,
					r_change[i] = j;	//The resulting image should change i to j.
					break;
				}
			}
		}
	}


	for (int i = 0; i < MAX_Y; i++)
	{
		for (int j = 0; j < MAX_X; j++)
		{
			for (int k = 0; k < 256; k++) {
				if (inputraw[i][j] == k) {				//If the input pixel value is k
					inputraw[i][j] = r_change[k];		//Change k to the pixel value to be changed
					break;
				}
			}
		}
	}

	//Write .raw file
	FILE* outraw = fopen("result.raw", "wb");
	fwrite(inputraw, sizeof(char), MAX_X * MAX_Y, outraw);
	fclose(outraw);

	double r[256] = { 0, };			//result image,index is the depth value of the pixel, data is the frequency
	for (int i = 0; i < MAX_Y; i++)
	{
		for (int j = 0; j < MAX_X; j++)
		{
			r[inputraw[i][j]] += 1;	//The frequency of the appropriate index (pixel value) is increased by one.
		}
	}


	double p_r[256], cdf_r[256];	//p_r: probability for each pixel, cdf_r: probability accumulation value for each pixel
	for (int i = 0; i < 256; i++) {
		p_r[i] = r[i] / co;

		if (i != 0) { cdf_r[i] = cdf_r[i - 1] + p_r[i]; }
		else { cdf_r[i] = p_r[i]; }
		//printf("%f \n", r[i]);				
		//printf("%f \n", p_r[i]);				
		//printf("%f \n", cdf_r[i]);				
	}

	for (int i = 0; i < 256; i++) {
		//printf("%f \n", r_change[i]);
		//printf("%d \n", inputraw[0][i]);
	}
	return 0;
}
