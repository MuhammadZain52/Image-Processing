#define _CRT_SECURE_NO_DEPRECATE
#include <math.h>
#include "tiff.h"
#include "allocate.h"
#include "randlib.h"
#include "typeutil.h"
#include "vector.h"


struct TIFF_img output_img;
int Row = 45;
int Col = 67;

int found_seg[256][384] = {0};

struct pixel {
	int row, col; //row , col
};


void vector_init(vector *v)
{
	v->data = NULL;
	v->size = 0;
	v->count = 0;
}

int vector_count(vector *v)
{
	return v->count;
}

void vector_add(vector *v, void *e)
{
	if (v->size == 0) {
		v->size = 10;
		v->data = malloc(sizeof(void*) * v->size);
		memset(v->data, '\0', sizeof(void*) * v->size);
	}

	if (v->size == v->count) {
		v->size *= 2;
		v->data = realloc(v->data, sizeof(void*) * v->size);
	}

	v->data[v->count] = e;
	v->count++;
}

void vector_set(vector *v, int index, void *e)
{
	if (index >= v->count) {
		return;
	}

	v->data[index] = e;
}

void *vector_get(vector *v, int index)
{
	if (index >= v->count) {
		return NULL;
	}

	return v->data[index];
}

void vector_delete(vector *v, int index)
{
	if (index >= v->count) {
		return;
	}

	for (int i = index, j = index; i < v->count; i++) {
		v->data[j] = v->data[i];
		j++;
	}

	v->count--;
}

void vector_free(vector *v)
{
	free(v->data);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////           ConnectedNeighbors																		///////////////
////////																									//////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConnectedNeighbors(struct pixel s, double T, unsigned char **img, int width, int height, int *M, struct pixel c[4]) {
	*M = 0;

	if (s.row - 1 >= 0 && abs((int)img[s.row - 1][s.col] - (int)img[s.row][s.col]) <= T)
	{
		c[0].col = s.col;
		c[0].row = s.row - 1;
		*M = *M + 1;
	}
	else
	{
		c[0].col = -1;
		c[0].row = -1;
	}

	if (s.col - 1 >= 0 && abs((int)img[s.row][s.col - 1] - (int)img[s.row][s.col]) <= T)
	{
		c[1].col = s.col - 1;
		c[1].row = s.row;
		*M++;
	}
	else
	{
		c[1].col = -1;
		c[1].row = -1;
	}

	if (s.row + 1 < height && abs((int)img[s.row + 1][s.col] - (int)img[s.row][s.col]) <= T)
	{
		c[2].col = s.col;
		c[2].row = s.row + 1;
		*M++;
	}
	else
	{
		c[2].col = -1;
		c[2].row = -1;
	}



	if (s.col + 1 < width && abs((int)img[s.row][s.col + 1] - (int)img[s.row][s.col]) <= T)
	{
		c[3].col = s.col + 1;
		c[3].row = s.row;
		*M++;
	}
	else
	{
		c[3].col = -1;
		c[3].row = -1;
	}
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////           ConnectedSet Function																		///////////////
////////																									//////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConnectedSet(struct pixel s, double T, unsigned char **img, int width, int height, int ClassLabel, int seg[][384], int *NumConPixels)
{

	int neighbors = 0;
	struct pixel pixels[4];

	vector v;
	vector_init(&v);

	vector_add(&v, &s);
	do
	{
		struct pixel *point = vector_get(&v, v.count - 1);

		int r = point->row;
		int c = point->col;
		seg[r][c] = ClassLabel;

		*NumConPixels += 1;
		struct pixel *vis = (struct pixel *) malloc(sizeof(struct pixel));
		vis->col = c;
		vis->row = r;

		printf("row:%d ", vis->row);
		printf("col:%d \n", vis->col);


		ConnectedNeighbors(*point, T, img, width, height, &neighbors, pixels);
		vector_delete(&v, v.count - 1);

		for (int j = 0; j < 4; j++) {

			if (seg[pixels[j].row][pixels[j].col] == 0 && pixels[j].row != -1) {

				struct pixel *point3 = (struct pixel *) malloc(sizeof(struct pixel));
				point3->col = pixels[j].col;
				point3->row = pixels[j].row;
				vector_add(&v, point3);
				seg[point3->row][point3->col] = 1;
			}
		}
	} while (v.count >= 1);

	vector_free(&v);
}



void segmentation(struct pixel s, double T, unsigned char **img, int width, int height, int ClassLabel, int seg[][384], int *NumConPixels)
{
	int count = 0;

	ClassLabel = 1;

	//Doing raster way to find the segments 
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 384; j++)
		{
			s.row = i;
			s.col = j;

			if (seg[i][j] == 0) 
			{
				*NumConPixels = 0;
				//passing new Label
				ConnectedSet(s, T, img, width, height, ClassLabel, seg, NumConPixels);
				
				//checking if the number of connected pixels are greater than 100
				if (*NumConPixels > 100)
				{
					count++;
					for (int k=0; k<256; k++)
					{
						for (int l=0; l<384; l++)
						{
							if (seg[k][l]==ClassLabel)
							{
								found_seg[k][l] = count;
							}
						}
					}
				}
				ClassLabel++;
			}
		}
	}
	printf("\nTotal found segments :%d\n",count);
}


int main()
{
	struct pixel p;
	p.row = Row;
	p.col = Col;

	double threshold = 2;
	int numOfconn = 0;


	FILE *fp;
	struct TIFF_img input_img;

	/* open image file */
	if ((fp = fopen("img22gd2.tif", "rb")) == NULL) {
		fprintf(stderr, "cannot open file %s\n", "image");
		exit(1);
	}

	/* read image */
	if (read_TIFF(fp, &input_img)) {
		fprintf(stderr, "error reading file %s\n", "image file");
		exit(1);
	}

	output_img = input_img;

	int seg[256][384] = {0};

	int row = input_img.height;
	int col = input_img.width;

	segmentation(p, threshold, input_img.mono, col, row, 1, seg, &numOfconn);

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
				output_img.mono[i][j] = found_seg[i][j];
		}
	}

	if ((fp = fopen("color.tif", "wb")) == NULL) {
		fprintf(stderr, "cannot open file color.tif\n");
		exit(1);
	}

	if (write_TIFF(fp, &output_img)) {
		fprintf(stderr, "error writing TIFF file %s\n", "output image");
		exit(1);
	}

	fclose(fp);

	return 0;
}