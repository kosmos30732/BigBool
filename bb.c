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
void shift(bb* vector)
{
    if (vector->last_bit<7 && vector->last_part==0)
    {
        vector->parts[vector->last_part]>>1;
        vector->last_bit++;
        return;
    }

    for (int i = vector->last_part; i >=0; i--)
    {
        if (vector->parts[i] & (1<<7))
        {
            vector->parts[i]<<1;
            vector->parts[i+1] |=(1<<0);
            continue;
        }

        vector->parts[i]<<1;
        vector->parts[i+1] &=~(1<<0);        
    }
    vector->last_bit=0;
    vector->last_part++;
    return;
}
int main ()
{
    bb vector={0};
    vector.last_bit=0;
    vector.last_part=0;

    char bit;
    if (!feof(stdin))
    {
        fscanf(stdin, "%c", &bit);
        
        if (bit=='1')
        {
            vector.parts[vector.last_part] |=(1<<0);
        }

        if (bit=='0')
        {
            vector.parts[vector.last_part] &=~(1<<0);
        }
    }
    
    while (!feof(stdin))
    {
                
        shift(&vector);
        fscanf(stdin, "%c", &bit);
        if(bit==0xa)
        break;

        if (bit=='1')
        {
            vector.parts[vector.last_part] |=(1<<0);
        }

        if (bit=='0')
        {
            vector.parts[vector.last_part] &=~(1<<0);
        }
    }
    
    for (int i = 0; i <= vector.last_part; i++)
    {
        if (vector.last_part==i)
        {
            for (int j = 0; j <= vector.last_bit; j++)
            {
                if (vector.parts[i] & (1<<j))
                {
                    printf("1");
                    continue;
                }

                printf("0");                
            }
            
        }
        for (int j = 0; j <= 7; j++)
        {
            if (vector.parts[i] & (1<<j))
            {
                printf("1");
                continue;
            }

        printf("0");                
        }
    }
    puts("");
    return 0;
}