#ifndef _TIFF_H_
#define _TIFF_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "typeutil.h"
struct TIFF_img {
  int32_t			height;
  int32_t			width;
  char			TIFF_type;  	/* 'g' = grayscale;               */
					/* 'p' = palette-color;           */
					/* 'c' = RGB full color           */

  uint8_t		**mono;		/* monochrome data, or indices    */
					/* into color-map; indexed as     */
					/* mono[row][col]                 */

  uint8_t		***color;	/* full-color RGB data; indexed   */
					/* as color[plane][row][col],     */
					/* with planes 0, 1, 2 being red, */
					/* green, and blue, respectively  */

  char 			compress_type;	/* 'u' = uncompressed             */

  uint8_t		**cmap;		/* for palette-color images;      */
					/* for writing, this array MUST   */
					/* have been allocated with       */
					/* height=256 and width=3         */
};
int32_t read_TIFF ( FILE *fp, struct TIFF_img *img ); 
int32_t write_TIFF ( FILE *fp, struct TIFF_img *img );
int32_t get_TIFF ( struct TIFF_img *img, int32_t height, 
int32_t width, char TIFF_type );
void free_TIFF ( struct TIFF_img *img );
#endif

