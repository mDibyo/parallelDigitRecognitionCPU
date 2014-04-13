/*
 * PROJ1-1: YOUR TASK A CODE HERE
 *
 * You MUST implement the calc_min_dist() function in this file.
 *
 * You do not need to implement/use the swap(), flip_horizontal(), transpose(), or rotate_ccw_90()
 * functions, but you may find them useful. Feel free to define additional helper functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include "digit_rec.h"
#include "utils.h"
#include "limits.h"
// #include <signal.h>

/* Find the squared difference between two pixels */
float squared_distance(float d1, float d2) {
	float distance;
  distance = d1 - d2;
	return (distance * distance);
}

/* Swaps the values pointed to by the pointers X and Y. */
void swap_char(float *x, float *y) {
  float temp;
  temp = *x;
  *x = *y;
  *y = temp;
}

/* Swaps the values pointed to by the pointers X and Y. */
void swap_int(int *x, int *y) {
  int temp;
  temp = *x;
  *x = *y;
  *y = temp;
}

/* Flips the elements of a square array ARR across the y-axis. */
void flip_horizontal(float *arr, int width) {
  #pragma omp parallel for collapse(2)
  for (int i = 0; i < width; i++) {
  	for (int j = 0; j < (width / 2); j++) {
  		swap_char(arr + (width)*i + j, arr + (width)*i + (width) - j - 1);
 		}
  }
}

/* Transposes the square array ARR. */
void transpose(float *arr, int width) {
  float *result;
  result = (float*) malloc(sizeof(float) * width * width);
  // #pragma omp parallel for collapse(2)
  for (int i = 0; i < width; i++) {
    //#pragma omp parallel for
  	for (int j = 0; j < width; j++) {
  		result[i + j * width] = arr[i * width + j];
  	}
  }
  #pragma omp parallel for
  for (int index = 0; index < (width * width); index++) {
    arr[index] = result[index];
  }
  free(result);
}

/* Rotates the square array ARR by 90 degrees counterclockwise. */
void rotate_ccw_90(unsigned char *arr, int width) {

}

/* Find sum of squares pixel to pixel of two square images of the same size and
 * compare to existing sum.
 */
void least_sum_squares(float *i1, float *i2, int width,
                        float *least_sum) {
  float global_sum = 0.0;
  #pragma omp parallel for      \
    reduction(+:global_sum)
    for (int i = 0; i < (width*width); i++) {
        global_sum += squared_distance(i1[i], i2[i]);
    }
  if (global_sum < *least_sum) {
      *least_sum = global_sum;
  }
}

void extract_portion(float *portion, float *image, int i, int j,
                     int t_width, int i_width) {
	// #pragma omp parallel for collapse(2)
  for (int x = 0; x < t_width; x++) {
		for (int y = 0; y < t_width; y++) {
			portion[x + y*t_width] = image[(i + x) + (j + y)*i_width];
		}
	}
}

/* Returns the squared Euclidean distance between TEMPLATE and IMAGE. The size of IMAGE
 * is I_WIDTH * I_HEIGHT, while TEMPLATE is square with side length T_WIDTH. The template
 * image should be flipped, rotated, and translated across IMAGE.
 */
float calc_min_dist(float *image, int i_width, int i_height, 
                    float *template, int t_width) {
  float min_dist = UINT_MAX;
  float * portion;
  portion = (float*) malloc(sizeof(float)* t_width * t_width);
	for (int i = 0; i <= (i_width - t_width); i++) {
		for (int j = 0; j <= (i_height - t_width); j++) {
			extract_portion(portion, image, i, j, t_width, i_width);
			least_sum_squares(portion, template, t_width, &min_dist);
			flip_horizontal(portion, t_width);
			least_sum_squares(portion, template, t_width, &min_dist);
			transpose(portion, t_width);
      least_sum_squares(portion, template, t_width, &min_dist);
      flip_horizontal(portion, t_width);
      least_sum_squares(portion, template, t_width, &min_dist);
      transpose(portion, t_width);
      least_sum_squares(portion, template, t_width, &min_dist);
      flip_horizontal(portion, t_width);
      least_sum_squares(portion, template, t_width, &min_dist);
      transpose(portion, t_width);
      least_sum_squares(portion, template, t_width, &min_dist);
      flip_horizontal(portion, t_width);
      least_sum_squares(portion, template, t_width, &min_dist);
		}
	}
	return min_dist;
}