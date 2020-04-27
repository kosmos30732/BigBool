#include<stdio.h>
#include<stdlib.h>
#include<inttypes.h>
 
#define legth_of_parts 20
 
struct BigBool
{
    uint8_t parts[legth_of_parts];
    int last_bit;
    int last_part;
};
 
typedef struct BigBool bb;
 
int left_shift (bb* vector, int size_of_shift)
{
    //checking if the shift is possible
    if (size_of_shift >(legth_of_parts-1-vector->last_part)*8+7-vector->last_bit)
    {
        printf ("Too big a shift, going over the border");
        return 1;
    }
 
    //checking if the offset is less than one
    if (size_of_shift <1)
    {
        printf ("The shift cannot be less than one");
        return 1;
    }
 
    //need if add another part during the shift
    int new_part=vector->last_part;
 
    //shift
    for (int j = 0; j < size_of_shift; j++)
    {
        for (int i = vector->last_part; i >= 0; i--)
        {
            //separate shift for the first part if not all 8 bits are occupied in it
            if (i == vector->last_part && vector->last_bit!=7)
            {
                vector->parts[i] <<= 1;
                vector->last_bit++;
                continue;
            }
 
            //inc parts if all bits are occupied in the first part
            if (i == vector->last_part)
            {
                new_part++;
                vector->last_bit=0;
            }
 
            //shift the last bit of one part to the next part to the first bit
            if (vector->parts[i]&(1<<7))
            {
                vector->parts[i] <<= 1;
                vector->parts[i + 1] |= (1 << 0);
            }
            else
            {
                vector->parts[i] <<= 1;
            }
        }
    }
 
    //replacement if a new part was added or not
    vector->last_part = new_part;
    return 0;
}
 
int main ()
{
    bb vector = {0};
    vector.last_bit = 0;
    vector.last_part = 0;
    char bit; //to read bit
    int need_shift = 0; //to add the first bit without shifting
 
    //reading a vector from a stream
    while (fscanf (stdin, "%c", &bit))
    {
        if (bit == '1' || bit =='0')
        {
            if (need_shift)
            {
                if (left_shift (&vector, 1))
                {
                    printf ("Error to do left shift");
                }
            }
            need_shift = 1;
 
            //adding a read bit to the end
            if (bit=='1')
            {
                vector.parts[0] |= (1 << 0);
            }
            //don't need to add a zero because when you shift it is added
        }
        else
        {
            break;
        }
    }
 
    //output the BigBool
    for (int i = vector.last_part; i >=0; i--)
    {
        if (vector.last_part == i)
        {
            for (int j = vector.last_bit; j >=0 ; j--)
            {
                if (vector.parts[i] & (1 << j))
                {
                    printf ("1");
                    continue;
                }
 
                printf ("0");
            }
            continue;
        }
 
        for (int j = 7; j >=0; j--)
        {
            if (vector.parts[i] & (1 << j))
            {
                printf ("1");
                continue;
            }
 
            printf ("0");
        }
    }
    puts ("");
    return 0;
}