/* CS61C Sp14 Project 3
 *
 * Defines structs and functions used for both parts of the project.
 *
 * ANY MODIFICATIONS TO THIS FILE WILL BE OVERWRITTEN DURING GRADING
 */

#ifndef UTILS_H
#define UTILS_H

typedef struct {
    float *data;
    int width;
    int height;
} Image;

// loads BMP file as a char array
Image load_bmp(char *filename);

/* Prints the image. May be helpful in debugging. */
void print_bmp(const float *data, int w, int h);

/* Should be called if memory allocation fails. */
void allocation_failed();

#endif
