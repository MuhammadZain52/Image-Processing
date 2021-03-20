#ifndef _SOLVE_H_
#define _SOLVE_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "typeutil.h"

double  solve(
  double (*f)(),
  double a,
  double b,
  double err,
  int *code
  )
{
	int     signa, signb, signc;
	double  fa, fb, fc, c, signaling_nan();
	double  dist;
	fa = (*f)(a);  signa = fa>0;
	fb = (*f)(b);  signb = fb>0;
	if (signa == signb) {
		if (signa == 1) *code = 1;
		else *code = -1;
		return(0.0);
	}
	else *code = 0;
	if ((dist = b - a)<0) dist = -dist;
	while (dist>err) {
		c = (b + a) / 2;
		fc = (*f)(c);  signc = fc>0;
		if (signa == signc) { a = c; fa = fc; }
		else { b = c; fb = fc; }
		if ((dist = b - a)<0) dist = -dist;
	}
	if ((fb - fa) == 0) return(a);
	else {
		c = (a*fb - b*fa) / (fb - fa);
		return(c);
	}
}
#endif
