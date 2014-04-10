/* CS61C Sp14 Project 3
 *
 * Performs digit recognition via a pixel-by-pixel comparison of an input image
 * with template digit images. You will implement the function calc_min_dist()
 * in the file calc_dist.c
 *
 * ANY MODIFICATIONS TO THIS FILE WILL BE OVERWRITTEN DURING GRADING
 */

#ifndef DIGIT_REC_H
#define DIGIT_REC_H

#define TEMPLATE_WIDTH 64

// implement this in calc_dist.c
float calc_min_dist(float *image, int i_width, int i_height,
        float *template, int t_width);

int float_equals(float a, float b);

void check_distance(float distance, int test_id);

void check_distances(float *distances, int num_templates, char *filename);

void test_basic();

void test_bmp(char *img_name);

#endif
