/* CS61C Sp14 Project 3
 *
 * ANY MODIFICATIONS TO THIS FILE WILL BE OVERWRITTEN DURING GRADING
 */

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "digit_rec.h"

int main(int argc, char **argv) {
    if ( argc != 2 ) {
        test_basic();
    } else {
        test_bmp(argv[1]);
    }
    return 0;
}
