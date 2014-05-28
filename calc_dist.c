/*
 * PROJ1-1: YOUR TASK A CODE HERE
 *
 * You MUST implement the calc_min_dist() function in this file.
 *
 * You do not need to implement/use the swap(), flip_horizontal(), transpose(), or rotate_ccw_90()
 * functions, but you may find them useful. Feel free to define additional helper functions.
 */

#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include "digit_rec.h"
#include "utils.h"
 #include <memory.h>

#define ARRAY_SIZE 100000
#define REPEAT     100

float calc_translate(float *template, float *image, int x, int y, int t_width, int i_width) {
    float min_dist_array[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    float min = FLT_MAX;
    for (int j = 0; j < t_width; j++) {
        for (int i = 0; i < t_width; i++) {
	    float delta0 = template[i + j * t_width] - image[(i + x) + (j + y) * i_width];
	    min_dist_array[0] += delta0 * delta0;

	    float delta1 = template[i * t_width + t_width - j - 1] - image[(i + x) + (j + y) * i_width];
	    min_dist_array[1] += delta1 * delta1;

	    float delta2 = template[t_width - i - 1 + (t_width - j - 1) * t_width] - image[(i + x) + (j + y) * i_width];
	    min_dist_array[2] += delta2 * delta2;

	    float delta3 = template[(t_width - i - 1) * t_width + j] - image[(i + x) + (j + y) * i_width];
	    min_dist_array[3] += delta3 * delta3;

	    float delta4 = template[(t_width - i - 1) + j * t_width] - image[(i + x) + (j + y) * i_width];
	    min_dist_array[4] += delta4 * delta4;

	    float delta5 = template[i + (t_width - j - 1) * t_width] - image[(i + x) + (j + y) * i_width];
	    min_dist_array[5] += delta5 * delta5;

	    float delta6 = template[i * t_width + j] - image[(i + x) + (j + y) * i_width];
	    min_dist_array[6] += delta6 * delta6;

	    float delta7 = template[(t_width - i - 1) * t_width + t_width - j - 1] - image[(i + x) + (j + y) * i_width];
	    min_dist_array[7] += delta7 * delta7;
        }
    }
    for (int i = 0; i < 8; i++) if (min_dist_array[i] < min) min = min_dist_array[i];
    return min;
}

/* Returns the squared Euclidean distance between TEMPLATE and IMAGE. The size of IMAGE
 * is I_WIDTH * I_HEIGHT, while TEMPLATE is square with side length T_WIDTH. The template
 * image should be flipped, rotated, and translated across IMAGE.
 */
float calc_min_dist(float *image, int i_width, int i_height,
                    float *template, int t_width) {
    float min_dist = FLT_MAX;
    /* YOUR CODE HERE */
    int dx = i_width - t_width + 1;
    int dy = i_height - t_width + 1;
    #pragma omp parallel 
    {
        #pragma omp for collapse(2)
	for (int x = 0; x < dx; x++) {
	    for (int y = 0; y < dy; y++) {
		float min = calc_translate(template, image, x, y, t_width, i_width);
		#pragma omp critical
		{
		    if (min < min_dist) {
			min_dist = min;
		    }
		}
	    }
	}
    }
    return min_dist;
}   
