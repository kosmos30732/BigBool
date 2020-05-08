#include"bb.h"

 int bb_errno=ERR_OK;

int left_shift (bb* vector, int size_of_shift)
{
    //checking if the shift is possible
    if (size_of_shift >(legth_of_parts-1-vector->last_part)*8+7-vector->last_bit)
    {
        bb_errno = ERR_BIG_SHIFT;
        return ERR_BIG_SHIFT;
    }
 
    //checking if the offset is less than one
    if (size_of_shift <1)
    {
        bb_errno = ERR_SMALL_SIZE;
        return ERR_SMALL_SIZE;
    }  
 
    //shift
    for (int j = 0; j < size_of_shift; j++)
    {
        //need if add another part during the shift
        int new_part = vector->last_part;
 
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
 
        //replacement if a new part was added or not
        vector->last_part = new_part;
    }
 
    return ERR_OK;
}
 
int rigth_shift (bb* vector, int size_of_shift)
{
    //checking if the shift is possible
    if (size_of_shift >= vector->last_part*8+1+vector->last_bit)
    {
        bb_errno = ERR_BIG_SHIFT;
        return ERR_BIG_SHIFT;
    }
 
    //checking if the offset is less than one
    if (size_of_shift < 1)
    {
        bb_errno = ERR_SMALL_SIZE;
        return ERR_SMALL_SIZE;
    }
 
    //shift
    for (int j = 0; j < size_of_shift; j++)
    {
        //need if one part can be removed during the shift
        int new_part = vector->last_part;
 
        //buffer for the first bit of part
        int buf;
 
        for (int i = vector->last_part; i >= 0; i--)
        {
            //left shift of the very first part
            if (i == vector->last_part)
            {
                if (vector->parts[i] & (1<<0))
                {
                    buf = 1;
                }
                else
                {
                    buf = 0;
                }
 
                vector->parts[i] >>= 1;
 
                if (vector->last_bit==0)
                {
                    new_part--;
                    vector->last_bit = 7;
                }
                else
                {
                    vector->last_bit--;
                }
                continue;
            }
 
            //left shift of parts between the first and last
            if (i!=0)
            {
                //another buffer to store the last bit of the current part
                int last_bit_of_part;
                if (vector->parts[i] & (1 << 0))
                {
                    last_bit_of_part = 1;
                }
                else
                {
                    last_bit_of_part = 0;
                }
 
                //left shift of the current part
                vector->parts[i] >>= 1;
 
                //recording a bit from the previous part
                if (buf)
                {
                    vector->parts[i] |= (1 << 7);
                }
 
                //replacing the buffer with the current part bit
                buf = last_bit_of_part;
            }
 
            //left shift of the last part
            else
            {
                vector->parts[i] >>= 1;
                if (buf)
                {
                    vector->parts[i] |= (1 << 7);
                }
            }
        }
 
        //replacement if one part was deleted
        vector->last_part = new_part;
    }

    return ERR_OK;
}
 
bb* bb_from_string (char* string)
{
    bb* vector = malloc (sizeof (bb));
    vector->last_bit = 0;
    vector->last_part = 0;
 
    //to add the first bit without shifting
    int need_shift = 0;
 
    //index for the string
    int ind_string = 0;
 
    //reading bits from a string and writing them to a vector
    while (string[ind_string]!='\0')
    {
        //checks if the string contains something other than 1 and 0
        if (string[ind_string] == '1' || string[ind_string] == '0')
        {
            if (need_shift)
            {
                if (left_shift (vector, 1))
                {
                    return NULL;
                }
            }
            need_shift = 1;
 
            //adding a bit to the end
            if (string[ind_string] == '1')
            {
                vector->parts[0] |= (1 << 0);
            }
            //don't need to add a zero because when you shift it is added
        }
        //if it contains it we output a message
        else
        {
            return NULL;
        }
        ind_string++;
    }

    return vector;
}
 
char* bb_to_string (bb* vector)
{
    //initializing a string and its index
    int ind_string = 0;
    char* string = malloc ((ind_string + 1) * sizeof (char));
 
    //separate addition of bits from the first part since it can be of different lengths
    for (int i = vector->last_bit; i >=0; i--)
    {
        //checking which bit is 1 or 0
        if (vector->parts[vector->last_part]&(1<<i))
        {
            string[ind_string] = '1';
        }
        else
        {
            string[ind_string] = '0';
        }
 
        //after writing the bit, re-create the string by +1 element
        ind_string++;
        string = (char*)realloc (string, (ind_string + 1) * sizeof (char));
    }
 
    //adding bits from other parts of the
    for (int i = vector->last_part-1; i >=0; i--)
    {
        for (int j = 7; j >=0; j--)
        {
            //checking which bit is 1 or 0
            if (vector->parts[i] & (1 << j))
            {
                string[ind_string] = '1';
            }
            else
            {
                string[ind_string] = '0';
            }
 
            //after writing the bit, re-create the string by +1 element
            ind_string++;
            string = (char*)realloc (string, (ind_string + 1) * sizeof (char));
        }
    }
 
    //adding a zero to the end of a line
    string[ind_string] = '\0';

    return string;
}
 
bb* bb_disjunction (bb* vector1, bb* vector2)
{
    //Checking that the vectors are the same length
    if (vector1->last_part!=vector2->last_part || vector1->last_bit!=vector2->last_bit)
    {
        return NULL;
    }

    bb* vector = malloc (sizeof (bb));
    vector->last_bit = vector1->last_bit;
    vector->last_part = vector1->last_part;
 
    //disjunction
    for (int i = 0; i <= vector1->last_part; i++)
    {
        vector->parts[i] = vector1->parts[i] | vector2->parts[i];
    }
    
    return vector;
}
 
bb* bb_conjunction (bb* vector1, bb* vector2)
{
    //Checking that the vectors are the same length
    if (vector1->last_part != vector2->last_part || vector1->last_bit != vector2->last_bit)
    {
        return NULL;
    }

    bb* vector = malloc (sizeof (bb));
    vector->last_bit = vector1->last_bit;
    vector->last_part = vector1->last_part;
 
    //conjunction
    for (int i = 0; i <= vector1->last_part; i++)
    {
        vector->parts[i] = vector1->parts[i] & vector2->parts[i];
    }

    return vector;
}
 
bb* bb_xor (bb* vector1, bb* vector2)
{
    //Checking that the vectors are the same length
    if (vector1->last_part != vector2->last_part || vector1->last_bit != vector2->last_bit)
    {
        return NULL;
    }

    bb* vector = malloc (sizeof (bb));
    vector->last_bit = vector1->last_bit;
    vector->last_part = vector1->last_part;
 
    //xor
    for (int i = 0; i <= vector1->last_part; i++)
    {
        vector->parts[i] =vector1->parts[i] ^ vector2->parts[i];
    }

    return vector;
}
 
bb* bb_inversion (bb* vector1)
{
    bb* vector = malloc (sizeof (bb));
    vector->last_bit = vector1->last_bit;
    vector->last_part = vector1->last_part;

    for (int i = 0; i <= vector1->last_part; i++)
    {
        vector->parts[i] = ~vector1->parts[i];
    }

    return vector;
}
 
int cycle_left_shift (bb* vector, int size_of_shift)
{
    //checking if the offset is less than one
    if (size_of_shift < 1)
    {
        bb_errno = ERR_SMALL_SIZE;
        return ERR_SMALL_SIZE;
    }
 
    //cycle left shift
    for (int i = 0; i < size_of_shift; i++)
    {
        //need to remember the current last_bit and last_part and the first bit
        int first_left_bit=0;
        int last_part = vector->last_part;
        int last_bit = vector->last_bit;
 
        //if need it to set the first left bit to 1
        if (vector->parts[last_part]&(1<<last_bit))
        {
            first_left_bit = 1;
        }
 
        //making a left shift
        if (left_shift(vector,1))
        {
            return 1;
        }
 
        //returning the values last_bit and last_part to their places
        vector->last_bit = last_bit;
        vector->last_part = last_part;
 
        //setting the first left bit to the end
        if (first_left_bit)
        {
            vector->parts[0] |= (1 << 0);
        }
    }

    return ERR_OK;
}
 
int cycle_rigth_shift (bb* vector, int size_of_shift)
{
    //checking if the offset is less than one
    if (size_of_shift < 1)
    {
        bb_errno = ERR_SMALL_SIZE;
        return ERR_SMALL_SIZE;
    }
 
    //cycle left shift
    for (int i = 0; i < size_of_shift; i++)
    {
        //need to remember the current last_bit and last_part and the last_rigth_bit
        int last_rigth_bit = 0;
        int last_part = vector->last_part;
        int last_bit = vector->last_bit;
 
        //if need it to set the last_rigth_bit to 1
        if (vector->parts[0] & (1 << 0))
        {
            last_rigth_bit = 1;
        }
 
        //making a rigth shift
        if (rigth_shift (vector, 1))
        {
            return 1;
        }
 
        //returning the values last_bit and last_part to their places
        vector->last_bit = last_bit;
        vector->last_part = last_part;
 
        //setting the last right bit to the beginning
        if (last_rigth_bit)
        {
            vector->parts[last_part] |= (1 << last_bit);
        }
    }  

    return ERR_OK;
}