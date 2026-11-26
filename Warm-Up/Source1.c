/* CS4-53203: Systems Programming \
/* Name: Dylan Strnad\
/* Date:1/14/2026
/* StrnadDylan-CS43203-Warm-up-Draft.txt\
\
\
\
/* Template for assignments. */
/* Add your work for this assignment here\
Description: creates an array of floats, and calculates the min, max, and average.
*/

#include <stdio.h>
#include<stdlib.h>
#include <assert.h>

struct numlist {
	float* list; /* points to list of numbers */
	int len; /* number of items in list */
	float min, /* the minimal value in list */
	max, /* the maximal value in list */
	avg; /* the mean of the numbers */
};

void compute_stats(struct numlist* listptr) {
	// assigns first number in array
	float min = listptr->list[0];
	float max = listptr->list[0];
	float total = listptr->list[0];

	// finds min/max.
	for (int i = 1; i < listptr->len; ++i) {
		if (listptr->list[i] < min) {
			min = listptr->list[i];
		}
		if (listptr->list[i] > max) {
			max = listptr->list[i];
		}
		total += listptr->list[i]; // needed for calculating avg
	}

	float avg = total / listptr->len;

	printf("min: %f\n", min);
	printf("max: %f\n", max);
	printf("avg: %f\n", avg);
}

int main() {
	struct numlist n1;
	n1.len = 5;

	//allocate memory
	n1.list = malloc(n1.len);

	//assign elements in array
	n1.list[0] = 0.11;
	n1.list[1] = 1.2;
	n1.list[2] = 2.4;
	n1.list[3] = 3.7;
	n1.list[4] = 5.3;

	//creates pointer. Needed for compute_stats func
	struct numlist* n1p = NULL;
	n1p = &n1;

	assert(n1p != NULL); //check if pointer is assigned

	compute_stats(n1p);
}