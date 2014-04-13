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
#include <nmmintrin.h>
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
  for (int i = 0; i < width; i++) {
  	for (int j = 0; j < (width / 2); j++) {
  		swap_char(arr + (width)*i + j, arr + (width)*i + (width) - j - 1);
 		}
  }
}

void flip_vertical(float *arr, int width) {
  float *iwidth, *i_iwidth;
  for (int i = 0; i < (width/2); i++) {
    iwidth = arr + width*i;
    i_iwidth = arr + width*(width-1-i);
    for (int j = 0; j < width; j++) {
      swap_char(iwidth + j, i_iwidth + j);
    }
  }
}

/* Transposes the square array ARR. */
void transpose(float *arr, int width) {
  int ARRAY_SIZE = width*width;
  float result[ARRAY_SIZE];
  // result = (float*) malloc(sizeof(float) * width * width);
  for (int i = 0; i < width; i++) {
  	for (int j = 0; j < width; j++) {
  		result[i + j * width] = arr[i * width + j]; // [j][i] <- [i][j]
  	}
  }

  for(int i=0; i<(ARRAY_SIZE/4)*4; i+=4) {
    arr[i] = result[i];
    arr[i+1] = result[i+1];
    arr[i+2] = result[i+2];
    arr[i+3] = result[i+3];
  }

  for (int i= (ARRAY_SIZE/4)*4; i< ARRAY_SIZE; i++) {
      arr[i] = result[i];
  }
  // free(result);
}

void transpose_broke(float *arr, int width) {
  for (int i = 0; i < width; i++) {
    for (int j =0; j < width; j++) {
      swap_char(arr + i + j*width, arr + i*width + j);
    }
  }
}

/* Rotates the square array ARR by 90 degrees counterclockwise. */
void rotate_ccw_90(unsigned char *arr, int width) {

}

/* Find sum of squares pixel to pixel of two square images of the same size and
 * compare to existing sum.
 */
void least_sum_squares_unrolled(float *image, float *template, int width,
                        float *least_sum) {
  float sum1 = 0.0, sum2 = 0.0;
  // float sum3 = 0.0, sum4 = 0.0;
  int total_pix = width*width;
  for (int i = 0; i < (total_pix/4)*4; i+=4) {
    // normal
    sum1 += squared_distance(image[i], template[i]);
    sum1 += squared_distance(image[i+1], template[i+1]);
    sum1 += squared_distance(image[i+2], template[i+2]);
    sum1 += squared_distance(image[i+3], template[i+3]);
    // normal reverse
    sum2 += squared_distance(image[i], template[total_pix-i-1]);
    sum2 += squared_distance(image[i+1], template[total_pix-i-2]);
    sum2 += squared_distance(image[i+2], template[total_pix-i-3]);
    sum2 += squared_distance(image[i+3], template[total_pix-i-4]);
    // flipped
    // sum3 += squared_distance(image[i], template)
  }
  for (int i=(total_pix/4)*4; i < total_pix; i++) {
    sum1 += squared_distance(image[i], template[i]);
    sum2 += squared_distance(image[i], template[total_pix-i-1]);
  }
  if (sum1 < *least_sum) {
    *least_sum = sum1;
  }
  if (sum2 < *least_sum) {
    *least_sum = sum2;
  }
}

__m128 squared_distance_vector(__m128 a, __m128 b) {
  __m128 c = _mm_sub_ps(a, b);
  return _mm_mul_ps(c, c);
}
/*
void least_sum_squares_vector(float *image, float *template, int width,
                                  float *least_sum) {
  __m128 sum1, sum2, sum3, sum4, sum5, sum6, sum7, sum8, a, b;
  sum1 = sum2 = sum3 = sum4 = sum5 = sum6 = sum7 = sum8 = _mm_setzero_ps();
  int total_pix = width*width - 1;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < (width/4)*4; j+= 4) {
      a = _mm_loadu_si128((__m128i*) (image + i*width + (j+offy)));
      b = _mm_loadu_si128((__m128i*) (template + i*width + j));
      sum1 = _mm_add_epi32(sum1, squared_distance_vector(a, b));
    }
  }
} */

void least_sum_squares(float *image, float *template, int i_width, int t_width,
                        float *least_sum, int offx, int offy) {
  float sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0,
        sum5 = 0.0, sum6 = 0.0, sum7 = 0.0, sum8 = 0.0;
  int offset = offx + i_width*offy;
  int total_pix = t_width*t_width - 1; /*
  float v[4], *pin;
  __m128 sum1_vector, sum2_vector, sum4_vector, a, b;
  sum1_vector = _mm_setzero_ps();
  for (int i = 0; i < t_width; i++) {
    for (int j = 0; j < (t_width/4)*4; j+=4) {
      // normal
      a = _mm_loadu_ps(image + offset + i*i_width + j);
      b = _mm_loadu_ps(template + i*t_width + j);
      sum1_vector = _mm_add_ps(sum1_vector, squared_distance_vector(a, b));
      // normal flip
      sum2 += squared_distance(image[offset + i*i_width + j], template[total_pix - i*t_width - j]);
      sum2 += squared_distance(image[offset + i*i_width + j+1], template[total_pix - i*t_width - (j+1)]);
      sum2 += squared_distance(image[offset + i*i_width + j+2], template[total_pix - i*t_width - (j+2)]);
      sum2 += squared_distance(image[offset + i*i_width + j+3], template[total_pix - i*t_width - (j+3)]);
      pin = template + total_pix - i*t_width - j;
      // b = _mm_loadr_ps(template + total_pix - i*t_width - j - 3);
      b = _mm_set_ps(*pin, *(pin-1), *(pin-2), *(pin-3));
      sum2_vector = _mm_add_ps(sum2_vector, squared_distance_vector(a, b));
      // reverse
      sum3 += squared_distance(image[offset + i*i_width + j], template[(i+1)*t_width - j-1]);
      sum3 += squared_distance(image[offset + i*i_width + j+1], template[(i+1)*t_width - j-2]);
      sum3 += squared_distance(image[offset + i*i_width + j+2], template[(i+1)*t_width - j-3]);
      sum3 += squared_distance(image[offset + i*i_width + j+3], template[(i+1)*t_width - j-4]);
      // reverse flip
      sum4 += squared_distance(image[offset + i*i_width + j], template[total_pix - (i+1)*t_width + j+1]);
      sum4 += squared_distance(image[offset + i*i_width + j+1], template[total_pix - (i+1)*t_width + j+2]);
      sum4 += squared_distance(image[offset + i*i_width + j+2], template[total_pix - (i+1)*t_width + j+3]);
      sum4 += squared_distance(image[offset + i*i_width + j+3], template[total_pix - (i+1)*t_width + j+4]);
      b = _mm_loadu_ps(template + total_pix - (i+1)*t_width + j+1);
      sum4_vector = _mm_add_ps(sum4_vector, squared_distance_vector(a, b));
    }
  }
  _mm_storeu_ps(v, sum1_vector);
  for (int p = 0; p < 4; p++) {
    sum1 += *(v + p);
  }
  _mm_storeu_ps(v, sum2_vector);
  for (int p = 0; p < 4; p++) {
    sum2 += *(v + p);
  }
  _mm_storeu_ps(v, sum4_vector);
  for (int p = 0; p < 4; p++) {
    sum4 += *(v + p);
  }
  if (t_width % 4 != 0) {
    for (int i = 0; i < t_width; i++) {
      for (int j = (t_width/4)*4; j < t_width; j++) {
        sum1 += squared_distance(image[offset + i*i_width + j], template[i*t_width + j]);
        sum2 += squared_distance(image[offset + i*i_width + j], template[total_pix - i*t_width - j]);
        sum3 += squared_distance(image[offset + i*i_width + j], template[(i+1)*t_width - j-1]);
        sum4 += squared_distance(image[offset + i*i_width + j], template[total_pix - (i+1)*t_width + j+1]);
      }
    }
  } */
  for (int i = 0; i < t_width; i++) {
    for (int j = 0; j < t_width; j++) {
      sum1 += squared_distance(image[offset + i*i_width + j], template[i*t_width + j]);
      sum2 += squared_distance(image[offset + i*i_width + j], template[total_pix - i*t_width - j]);
      sum3 += squared_distance(image[offset + i*i_width + j], template[(i+1)*t_width - j-1]);
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

void reverse_least_sum_squares(float *image, float *template, int width,
                        float *least_sum) {
  float sum = 0.0;
  int total_pix = width*width-1;
  for (int i = 0; i < total_pix; i++) {
      sum += squared_distance(image[i], template[total_pix-i]);
  }
  if (sum < *least_sum) {
      *least_sum = sum;
  }
}

void extract_portion(float *portion, float *image, int i, int j,
                     int t_width, int i_width) {
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
