#ifndef _RANDLIB_H_
#define _RANDLIB_H_


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "typeutil.h"
#define _CRT_SECURE_NO_DEPRECATE
#define MAXPRIME  2147483647
#define PI        3.14159265358979323846
static long int tmp;
static uint32_t   sd[2];
double random2()
{
	*(sd + 1) *= 16807;
	*sd *= 16807;
	tmp = ((*sd) >> 15) + (((*sd) & 0x7fff) << 16);
	tmp += (*(sd + 1));
	if (tmp & 0x80000000) {
		tmp++;
		tmp &= 0x7fffffff;
	}
	*sd = tmp >> 16;
	*(sd + 1) = tmp & 0xffff;
	return(((double)tmp) / MAXPRIME);
}
int32_t random3()
{
	*(sd + 1) *= 16807;
	*sd *= 16807;
	tmp = ((*sd) >> 15) + (((*sd) & 0x7fff) << 16);
	tmp += (*(sd + 1));
	if (tmp & 0x80000000) {
		tmp++;
		tmp &= 0x7fffffff;
	}
	*sd = tmp >> 16;
	*(sd + 1) = tmp & 0xffff;
	return((int)tmp);
}
void srandom2(uint32_t num)
{
	tmp = num;
	*sd = tmp >> 16;
	*(sd + 1) = tmp & 0xffff;
}
void readseed()
{
	FILE	*fp1;
	void	writeseed();

	if ((fp1 = fopen("/tmp/randomseed", "r")) == NULL) {
		fprintf(stderr, "readseed: creating file /tmp/randomseed\n");
		tmp = 143542612;
		writeseed();
		srandom2(tmp);
	}
	else {
		fscanf_s(fp1, "%ld", &tmp);
		srandom2(tmp);
		fclose(fp1);
	}
}
void writeseed()
{
	FILE  *fp1;

	if ((fp1 = fopen("/tmp/randomseed", "w")) == NULL) {
		fprintf(stderr, "writeseed: can't open file /tmp/randomseed\n");
		exit(1);
	}
	else {
		fprintf(fp1, "%ld", tmp);
		fclose(fp1);
	}
}
double normal()
{
	static int32_t   even = 1;
	static double   b;
	double a, r, theta, random2();
	if ((even = !even)) {
		return(b);
	}
	else {
		theta = 2 * PI*random2();
		r = sqrt(-2 * log(random2()));
		a = r*cos(theta);
		b = r*sin(theta);
		return(a);
	}
}
double dexprand()
{
	double  a, random2();
	a = -log(random2());
	if (random2()>0.5)   a = (-a);
	return(a);
}
#endif

