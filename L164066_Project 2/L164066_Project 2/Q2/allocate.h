/* Modified for 64 bit memory allocation 8/29/2013 A. Mohan               */

#ifndef _ALLOCATE_H_
#define _ALLOCATE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "typeutil.h"
void *get_spc(size_t num, size_t size)
{
	void *pt;
	if ((pt = calloc((size_t)num, size)) == NULL) 
	{
		fprintf(stderr, "==> calloc() error, num=%zu, size=%zu\n", num, size);
		exit(-1);
	}
	return(pt);
}
void *mget_spc(size_t num, size_t size)
{
	void *pt;
	if ((pt = malloc((size_t)(num*size))) == NULL) 
	{
		fprintf(stderr, "==> malloc() error, num=%zu, size=%zu\n", num, size);
		exit(-1);
	}
	return(pt);
}
void **get_img(size_t wd, size_t ht, size_t size)
{
	size_t i;
	void  **ppt;
	char   *pt;
	ppt = (void **)mget_spc(ht, sizeof(void *));
	pt = (char *)mget_spc(wd*ht, size);
	for (i = 0; i<ht; i++) 
		ppt[i] = pt + i*wd*size;
	return(ppt);
}
void free_img(void **pt)
{
	free((void *)pt[0]);
	free((void *)pt);
}
void *multialloc(size_t s, size_t d, ...)
{
	va_list ap; 
	size_t max,
		*q;
	char **r,**s1, *t, *tree;
	size_t i, j;
	size_t *d1;
	va_start(ap, d);
	d1 = (size_t *)mget_spc(d, sizeof(size_t));
	for (i = 0;i<d;i++)
		d1[i] = va_arg(ap, size_t);
	if (d == 1) {
		tree = (char *)mget_spc(d1[0], s*sizeof(char));
		free((void *)d1);
		return((void *)tree);
	}
	r = &tree;
	q = d1;
	max = 1;
	for (i = 0; i < d - 1; i++, q++) 
	{ 
		max *= (*q);
		r[0] = (char *)mget_spc(max, sizeof(char **));
		r = (char **)r[0];  
	}
	max *= (size_t)s * (*q);
	r[0] = (char *)mget_spc(max, sizeof(char));
	r = (char **)tree;
	q = d1;
	max = 1;
	for (i = 0; i < d - 2; i++, q++)
	{
		max *= (*q);
		for (j = 1, s1 = r + 1, t = r[0]; j<max; j++) 
		{ 
			*s1 = (t += sizeof(char **) * *(q + 1));
			s1++;
		}
		r = (char **)r[0]; 
	}
	max *= (*q);             
	for (j = 1, s1 = r + 1, t = r[0]; j < max; j++)
		*s1++ = (t += s * *(q + 1));
	va_end(ap);
	free((void *)d1);
	return((void *)tree);
}
void multifree(void *r, size_t d)
{
	void **p;
	void *next = NULL;
	size_t i;

	for (p = (void **)r, i = 0; i < d; p = (void **)next, i++)
		if (p != NULL) {
			next = *p;
			free((void *)p);
		}
}
#endif