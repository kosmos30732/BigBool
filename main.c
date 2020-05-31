#include"bb.h"
#include<stdio.h>

#define length 1000

int main ()
{
    char string1[length], string2[length];

    fscanf (stdin, "%s", string1);
    fscanf (stdin, "%s", string2);
    puts ("");

    bb* vector1 = bb_from_string (string1);
    bb* vector2 = bb_from_string (string2);

    printf ("cycle left shift first vector %s\n", bb_to_string (cycle_left_shift (vector1, 10)));

    printf ("cycle rigth shift second vector %s\n", bb_to_string (cycle_rigth_shift (vector2, 6)));

    printf ("disjunction first and second vectors %s\n", bb_to_string (bb_disjunction (vector1, vector2)));

    printf ("conjunction first and second vectors %s\n", bb_to_string (bb_conjunction (vector1, vector2)));

    printf ("xor first and second vectors %s\n", bb_to_string (bb_xor (vector1, vector2)));

    printf ("inversion first vector%s\n", bb_to_string (bb_inversion (vector1)));

    return 0;
}