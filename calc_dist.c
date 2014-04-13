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

/* Find the least of the previous least_sum and the sum from the current iteration */
void least_sum_squares(float *image, float *template, int i_width, int t_width,
                        float *least_sum, int offx, int offy) {
  float sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0,
        sum5 = 0.0, sum6 = 0.0, sum7 = 0.0, sum8 = 0.0;
  int offset = offx + i_width*offy;
  int total_pix = t_width*t_width - 1;
  for (int i = 0; i < t_width; i++) {
    for (int j = 0; j < t_width; j++) {
      // normal
      sum1 += squared_distance(image[offset + i*i_width + j], template[i*t_width + j]);
      // normal flip
      sum2 += squared_distance(image[offset + i*i_width + j], template[total_pix - i*t_width - j]);
      // normal transpose
      sum3 += squared_distance(image[offset + i*i_width + j], template[(i+1)*t_width - j-1]);
      // normal transpose flip
      sum4 += squared_distance(image[offset + i*i_width + j], template[total_pix - (i+1)*t_width + j+1]);
    }
  }
  for (int j = 0; j < t_width; j++) {
    for (int i = 0; i < t_width; i++) {
       // normal transpose
      sum5 += squared_distance(image[offset + i*i_width + j], template[j*t_width + i]);
      // normal transpose flip
      sum6 += squared_distance(image[offset + i*i_width + j], template[total_pix - j*t_width - i]);
      // reverse transpose
      sum7 += squared_distance(image[offset + i*i_width + j], template[(j+1)*t_width - i-1]);
      // reverse transpose flip
      sum8 += squared_distance(image[offset + i*i_width + j], template[total_pix - (j+1)*t_width + i+1]);
    }
  }
  if (sum1 < *least_sum) {
    *least_sum = sum1;
  }
  if (sum2 < *least_sum) {
    *least_sum = sum2;
  }
  if (sum3 < *least_sum) {
    *least_sum = sum3;
  }
  if (sum4 < *least_sum) {
    *least_sum = sum4;
  }
  if (sum5 < *least_sum) {
    *least_sum = sum5;
  }
  if (sum6 < *least_sum) {
    *least_sum = sum6;
  }
  if (sum7 < *least_sum) {
    *least_sum = sum7;
  }
  if (sum8 < *least_sum) {
    *least_sum = sum8;
  }
}

/* Returns the squared Euclidean distance between TEMPLATE and IMAGE. The size of IMAGE
 * is I_WIDTH * I_HEIGHT, while TEMPLATE is square with side length T_WIDTH. The template
 * image should be flipped, rotated, and translated across IMAGE.
 */
float calc_min_dist(float *image, int i_width, int i_height, 
                    float *template, int t_width) {
  float min_dist = UINT_MAX;
  // float portion[t_width*t_width];
  #pragma omp parallel
  {
    float dist = UINT_MAX;
    #pragma omp for collapse(2)
    for (int j = 0; j <= (i_height - t_width); j++) {
      for (int i = 0; i <= (i_width - t_width); i++) {
        least_sum_squares(image, template, i_width, t_width, &dist, i, j);
      }
    }
    #pragma omp critical
    {
      if (dist < min_dist) {
        min_dist = dist;
      }
    }
  }
  return min_dist;
}