#include"bb.h"
#include<stdio.h>

#define length 1000

int main()
{
    char string1[length], string2[length];

    fscanf(stdin, "%s", string1);
    fscanf(stdin, "%s", string2);
    puts("");

    bb* vector1=bb_from_string(string1);
    bb* vector2=bb_from_string(string2);

    bb* vector=bb_disjunction(vector1, vector2);
    char* string=bb_to_string(vector);
    printf("Disjunction %s\n",string);

    vector=bb_conjunction(vector1, vector2);
    string=bb_to_string(vector);
    printf("Conjunction %s\n",string);

    vector=bb_xor(vector1, vector2);
    string=bb_to_string(vector);
    printf("Xor %s\n",string);

    vector=bb_inversion(vector1);
    string=bb_to_string(vector);
    printf("Inversion %s\n",string);

    left_shift(vector1, -1);
    if (bb_errno!=ERR_SMALL_SIZE)
    {
        printf("[ERR] Don't set a flug ERR_SMALL_SIZE\n");
    }

    left_shift(vector1, 1000);
    if (bb_errno!=ERR_BIG_SHIFT)
    {
        printf("[ERR] Don't set a flug ERR_BIG_SHIFT\n");
    }
    
    left_shift(vector1, 3);
    string=bb_to_string(vector1);
    printf("%s\n",string);

    rigth_shift(vector2, 4);
    string=bb_to_string(vector2);
    printf("%s\n",string);

    cycle_left_shift(vector1,2);
    string=bb_to_string(vector1);
    printf("%s\n",string);

    cycle_rigth_shift(vector2, 10);
    string=bb_to_string(vector2);
    printf("%s\n",string);

    return 0;
}