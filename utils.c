/* CS61C Sp14 Project 3
 *
 * Implements functions used for both parts of the project. You do not need to
 * understand how this code works.
 *
 * ANY MODIFICATIONS TO THIS FILE WILL BE OVERWRITTEN DURING GRADING
 */

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

// reads a little-endian int from a buffer
// works on both little and big endian hosts
unsigned int readInt(const unsigned char *buffer) {
    return(buffer[0] & 0xff) | ((buffer[1] & 0xff) << 8)
        | ((buffer[2] & 0xff) << 16) | ((buffer[3] & 0xff) << 24);
}

/* Loads the bitmap file in FILENAME. */
Image load_bmp(char *filename) {
    unsigned char info[54], *data, *color_table;
    int width, height, size, color_table_size, check, i;

    FILE* f = fopen(filename, "rb");
    if(!f) {
        printf("Error opening file: %s\n", filename);
        exit(1);
    }
    check = fread(info, sizeof(unsigned char), 54, f); // Read the 54-byte info header
    if (check != 54) {
        printf("Bad file format..\n");
        exit(1);
    }

    color_table_size = readInt(info + 0x0a) - 54;

    if (readInt(info + 0x0e) != 40 || readInt(info + 0x1c) != 8 ) {
        printf( "Unsupported image format. Please specify an 8-bit (grayscale) BMP image.\n" );
        exit(1);
    }

    color_table = (unsigned char *) malloc(sizeof(unsigned char) * color_table_size);
    check = fread(color_table, sizeof(unsigned char), color_table_size, f);

    width = readInt(info + 0x12);
    height = readInt(info + 0x16);
    width = (width > 0) ? width : -1 * width;
    height = (height > 0) ? height : -1 * height;
    size = width * height;

    data = (unsigned char*) malloc(sizeof(unsigned char) * size);
    if(!data) allocation_failed();
    //load data from bottom to top
    for(i = height-1; i >= 0; i--) {
        check = fread(data+i*width, sizeof(unsigned char), width, f);
    }
    fclose(f);

    float* floatData = (float*) malloc(sizeof(float) * size);
    if(!data) allocation_failed();
    for(i = 0;i < size; i++) {
        floatData[i] = data[i] / 255.0;
    }
    Image img = { .data = floatData, .width = width, .height = height };
    free(color_table);
    return img;
}

/* Prints the 2-D array of size W * H stored in DATA. */
void print_bmp(const float *data, int w, int h) {
    int x, y;
    for(y = 0; y < h; y++) {
        for(x = 0; x < w; x++) {
            printf("%.3f ", data[y*w+x]);
        }
        printf("\n");
    }
}

void allocation_failed() {
    printf("Memory allocation failed.");
    exit(1);
}
