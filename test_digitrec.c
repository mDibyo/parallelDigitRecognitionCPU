/* CS61C Sp14 Project 3
 *
 * Contains test cases for digit-rec. The test cases here are by no means
 * complete. You are HIGHLY encouraged to write your own tests.
 *
 * ANY MODIFICATIONS TO THIS FILE WILL BE OVERWRITTEN DURING GRADING
 */

#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "digit_rec.h"

int float_equals(float a, float b) {
    float distance = a - b;
    float threshold = 0.005;
    return distance <= threshold * a && distance >= -1.0 * threshold * a;
}

void check_distance(float distance, int test_id) {
    switch(test_id) {
        case 0: printf("Testing with self...%s\n", float_equals(distance,0)?"passed":"failed"); break;
        case 1: printf("Testing with translated image...%s\n", float_equals(distance,0)?"passed":"failed"); break;
        case 2: printf("Testing with rotated image...%s\n", float_equals(distance,0)?"passed":"failed"); break;
        case 3: printf("Testing with image flipped across x-axis...%s\n", float_equals(distance,0)?"passed":"failed"); break;
        case 4: printf("Testing with image flipped across y-axis...%s\n", float_equals(distance,0)?"passed":"failed"); break;
        default: printf("Unrecongized test"); return;
    }
}

/* Finds the minimal distance in DISTANCES (of size NUM_TEMPLATES) and declares
 * the corresponding digit as the classification result for the file FILENAME.
 */
void check_distances(float *distances, int num_templates, char *filename) {
    int i, best_ind;
    float best = FLT_MAX, dist;
    printf("Checking student results for input: %s\n", filename);
    for(i = 0; i < num_templates; i++) {
        dist = distances[i];
        printf("%d: %.3f\n", i, dist);
        if (i == 0 || dist < best) {
            best = dist;
            best_ind = i;
        }
    }
    printf("I think your image is a %d\n", best_ind);
}

/* Performs some basic tests on your code. Feel free to add additional test
 * cases. */
void test_basic() {
    float template[] = {
        1/255.0, 2/255.0, 3/255.0,
        4/255.0, 5/255.0, 6/255.0,
        7/255.0, 8/255.0, 9/255.0
    };

    float translated[] = {
        0, 0, 0/255.0, 0/255.0, 0/255.0,
        0, 0, 1/255.0, 2/255.0, 3/255.0,
        0, 0, 4/255.0, 5/255.0, 6/255.0,
        0, 0, 7/255.0, 8/255.0, 9/255.0
    };

    float rotated[] = {
        3/255.0, 6/255.0, 9/255.0,
        2/255.0, 5/255.0, 8/255.0,
        1/255.0, 4/255.0, 7/255.0
    };

    float flip_x_axis[] = {
        7/255.0, 8/255.0, 9/255.0,
        4/255.0, 5/255.0, 6/255.0,
        1/255.0, 2/255.0, 3/255.0
    };

    float flip_y_axis[] = {
        3/255.0, 2/255.0, 1/255.0,
        6/255.0, 5/255.0, 4/255.0,
        9/255.0, 8/255.0, 7/255.0
    };

    printf("Performing basic tests.\n");
    check_distance(calc_min_dist(template, 3, 3, template, 3), 0);
    check_distance(calc_min_dist(translated, 5, 4, template, 3), 1);
    check_distance(calc_min_dist(rotated, 3, 3, template, 3), 2);
    check_distance(calc_min_dist(flip_x_axis, 3, 3, template, 3), 3);
    check_distance(calc_min_dist(flip_y_axis, 3, 3, template, 3), 4);
}

/* Loads and tests a .BMP file. */
void test_bmp(char *img_name) {
    int num_templates, i;
    Image img;
    float *distances;
    float **templates;

    num_templates = 10;
    templates = (float **) malloc(sizeof(float *) * num_templates);
    if(!templates) allocation_failed();
    img = load_bmp("templates/0_64.bmp");
    templates[0] = img.data;
    img = load_bmp("templates/1_64.bmp");
    templates[1] = img.data;
    img = load_bmp("templates/2_64.bmp");
    templates[2] = img.data;
    img = load_bmp("templates/3_64.bmp");
    templates[3] = img.data;
    img = load_bmp("templates/4_64.bmp");
    templates[4] = img.data;
    img = load_bmp("templates/5_64.bmp");
    templates[5] = img.data;
    img = load_bmp("templates/6_64.bmp");
    templates[6] = img.data;
    img = load_bmp("templates/7_64.bmp");
    templates[7] = img.data;
    img = load_bmp("templates/8_64.bmp");
    templates[8] = img.data;
    img = load_bmp("templates/9_64.bmp");
    templates[9] = img.data;
    distances = (float *) calloc(num_templates, sizeof(float));

    for(i = 0; i < num_templates; i++) {
        distances[i] = FLT_MAX;
    }

    // load image and check:
    img = load_bmp(img_name);
    for(int i = 0; i < num_templates; i++) {
        distances[i] = calc_min_dist(img.data, img.width, img.height, templates[i], TEMPLATE_WIDTH);
    }
    check_distances(distances, num_templates, img_name);

    // free memory:
    free(img.data);
    for(i = 0; i < num_templates; i++) {
        free(templates[i]);
    }
    free(templates);
    free(distances);
}
