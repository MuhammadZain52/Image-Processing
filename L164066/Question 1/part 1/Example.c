#define _CRT_SECURE_NO_DEPRECATE
#include <math.h>
#include "tiff.h"
#include "allocate.h"
#include "randlib.h"
#include "typeutil.h"
#include "vector.h"



vector visited;
struct TIFF_img output_img;
int Row = 45;
int Col = 67;


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

//void ConnectedSet(struct pixel s, double T, unsigned char **img, int width, int height, int ClassLabel, int seg[][384], int *NumConPixels)
//{
//
//	int neighbors = 0;
//	struct pixel pixels[4];
//
//	vector v;
//	vector_init(&v);
//	vector_init(&visited);
//	vector_add(&v, &s);
//	do
//	{
//		struct pixel *point = vector_get(&v, v.count - 1);
//
//		int r = point->row;
//		int c = point->col;
//		seg[r][c] = 1;
//
//		*NumConPixels += 1;
//		struct pixel *vis = (struct pixel *) malloc(sizeof(struct pixel));
//		vis->col = c;
//		vis->row = r;
//		vector_add(&visited, vis);
//		printf("found:%d \n", *NumConPixels);
//		printf("count:%d \n", v.count);
//		printf("row:%d ", vis->row);
//		printf("col:%d \n", vis->col);
//
//
//		ConnectedNeighbors(*point, T, img, width, height, &neighbors, pixels);
//		vector_delete(&v, v.count - 1);
//
//		int check = 0;
//		for (int j = 0; j < 4; j++) {
//
//			check = 0;
//
//			if (pixels[j].col != -1 && pixels[j].row != -1)
//			{
//				if (seg[pixels[j].row][pixels[j].col] == 0) {
//					for (int i = 0; i < visited.count; i++) {
//
//
//						struct pixel *point2 = vector_get(&visited, i);
//
//						if (point2->col == pixels[j].col && point2->row == pixels[j].row)
//						{
//							check = 1;
//							break;
//						}
//					}
//
//					if (check == 0)
//					{
//						for (int i = 0; i < v.count; i++) {
//
//
//							struct pixel *point2 = vector_get(&v, i);
//
//							if (point2->col == pixels[j].col && point2->row == pixels[j].row)
//							{
//								check = 1;
//								break;
//							}
//						}
//					}
//
//					if (check == 0)
//					{
//						struct pixel *point3 = (struct pixel *) malloc(sizeof(struct pixel));
//						point3->col = pixels[j].col;
//						point3->row = pixels[j].row;
//						vector_add(&v, point3);
//					}
//				}
//			}
//
//
//		}
//
//		//struct pixel *a = (struct pixel *) malloc(sizeof(struct pixel));
//		//vector_add(&v, a);
//
//		//int i;
//
//		//for (i = 0; i < vector_count(&v); i++) {
//		//	struct pixel c;
//		//	c = *(struct pixel*)vector_get(&v, i);
//		//	printf("%dm=", c.row);
//		//	printf("%dn=", c.col);
//		//}
//
//	} while (v.count >= 1);
//
//
//
//	//vector_free(&v);
//}

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
		
		printf("found:%d \n", *NumConPixels);
		printf("count:%d \n", v.count);
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
				seg[point3->row][point3->col] = ClassLabel;
			}
		}
	} while (v.count >= 1);

	vector_free(&v);
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

	ConnectedSet(p, threshold, input_img.mono, col, row, 1, seg, &numOfconn);

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (seg[i][j] != 1)
			{
				output_img.mono[i][j] = 255;
			}
			else
			{
				output_img.mono[i][j] = 0;
			}
		}
	}

	/* open color image file */
	if ((fp = fopen("color.tif", "wb")) == NULL) {
		fprintf(stderr, "cannot open file color.tif\n");
		exit(1);
	}

	/* write color image */
	if (write_TIFF(fp, &output_img)) {
		fprintf(stderr, "error writing TIFF file %s\n", "output image");
		exit(1);
	}

	fclose(fp);
	return 0;
}