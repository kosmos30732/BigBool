#include"bb.h"

int bb_errno = ERR_OK;

bb* left_shift (bb* vector, int size_of_shift)
{
    if (vector == NULL)
    {
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    //checking if the shift is possible
    if (size_of_shift > (LENGTH_OF_PARTS - 1 - vector->last_part) * 8 + 7 - vector->last_bit)
    {
        bb_errno = ERR_BIG_SHIFT;
        return NULL;
    }

    //checking if the offset is less than one
    if (size_of_shift < 1)
    {
        bb_errno = ERR_SMALL_SHIFT;
        return NULL;
    }

    bb* vector1 = calloc (1, sizeof (bb));
    if (vector1 == NULL)
    {
        bb_errno = ERR_MEM_NOT_ALLOC;
        return NULL;
    }

    vector1->last_bit = 0;
    vector1->last_part = 0;
    uint8_t last_part_of_bits = 0;
    uint8_t first_part_of_bits = 0;
    for (int i = 0; i <= vector->last_part; i++)
    {
        last_part_of_bits = vector->parts[i] >> (8 - size_of_shift % 8);
        vector1->parts[i + size_of_shift / 8] = first_part_of_bits | vector->parts[i] << (size_of_shift % 8);
        first_part_of_bits = last_part_of_bits;
    }

    //checks if another byte needs to be created when the last bit is shifted
    if (vector->last_bit + size_of_shift % 8 > 7)
    {
        vector1->last_part++;
        vector1->parts[size_of_shift / 8 + vector->last_part + 1] |= last_part_of_bits;
    }

    //calculating the dimension of the resulting vector after the shift
    vector1->last_bit += (vector->last_bit + size_of_shift % 8) % 8;
    vector1->last_part += vector->last_part + size_of_shift / 8;

    return vector1;
}

bb* rigth_shift (bb* vector, int size_of_shift)
{
    if (vector == NULL)
    {
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    //checking if the shift is possible
    if (size_of_shift > vector->last_part * 8 + 1 + vector->last_bit)
    {
        bb_errno = ERR_BIG_SHIFT;
        return NULL;
    }

    //checking if the offset is less than one
    if (size_of_shift < 1)
    {
        bb_errno = ERR_SMALL_SHIFT;
        return NULL;
    }

    bb* vector1 = calloc (1, sizeof (bb));

    //checking whether memory is allocated
    if (vector1 == NULL)
    {
        bb_errno = ERR_MEM_NOT_ALLOC;
        return NULL;
    }

    vector1->last_bit = 0;
    vector1->last_part = 0;
    uint8_t first_part_of_bits = 0;

    //separate right shift of the desired right part
    vector1->parts[0] = vector->parts[size_of_shift / 8] >> (size_of_shift % 8);

    //right shift of the other parts
    for (int i = size_of_shift / 8 + 1; i <= vector->last_part; i++)
    {
        first_part_of_bits = vector->parts[i] << (8 - size_of_shift % 8);
        vector1->parts[i - size_of_shift / 8] = vector->parts[i] >> (size_of_shift % 8);
        vector1->parts[i - size_of_shift / 8 - 1] |= first_part_of_bits;
    }

    //calculating the dimension of the resulting vector after the shift
    vector1->last_part = (vector->last_part * 8 + vector->last_bit - size_of_shift) / 8;
    vector1->last_bit = (vector->last_part * 8 + vector->last_bit - size_of_shift) % 8;

    if (size_of_shift == vector->last_part * 8 + 1 + vector->last_bit)
    {
        vector1->last_bit = 0;
    }

    return vector1;
}

bb* bb_from_string (char* string)
{
    if (string == NULL)
    {
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    bb* vector = calloc (1, sizeof (bb));

    //checking whether memory is allocated
    if (vector == NULL)
    {
        bb_errno = ERR_MEM_NOT_ALLOC;
        return NULL;
    }
    vector->last_bit = 0;
    vector->last_part = 0;

    //index for the string
    int string_idx = 1;

    //read first bit
    if (string[0] != '\0' && string[0] == '1')
    {
        vector->parts[0] |= 1;
    }

    //reading bits from a string and writing them to a vector
    while (string[string_idx] != '\0')
    {
        //checks if the string contains something other than 1 and 0
        if (string[string_idx] == '1' || string[string_idx] == '0')
        {
            vector = left_shift (vector, 1);

            //adding a bit to the end
            vector->parts[0] |= (string[string_idx] - '0');
        }
        //if it contains it we output a message
        else
        {
            bb_errno = ERR_WRONG_CHARS;
            goto cleanup;
        }
        string_idx++;
    }

    return vector;

cleanup:
    if (vector != NULL) free (vector);

    return NULL;
}

char* bb_to_string (bb* vector)
{
    if (vector == NULL)
    {
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    //initializing a string and its index
    int string_idx = 0;
    char* string = calloc (vector->last_part * 8 + vector->last_bit+2, sizeof (char));

    //checking whether memory is allocated
    if (string == NULL)
    {
        bb_errno = ERR_MEM_NOT_ALLOC;
        return NULL;
    }

    //separate addition of bits from the first part since it can be of different lengths
    for (int i = vector->last_bit; i >= 0; i--)
    {
        if (vector->parts[vector->last_part] & (1 << i))
        {
            string[string_idx] = '1';
        }
        else
        {
            string[string_idx] = '0';
        }
        string_idx++;
    }

    //adding bits from other parts
    for (int i = vector->last_part - 1; i >= 0; i--)
    {
        for (int j = 7; j >= 0; j--)
        {
            if (vector->parts[i] & (1 << j))
            {
                string[string_idx] = '1';
            }
            else
            {
                string[string_idx] = '0';
            }
            string_idx++;
        }
    }

    //adding a zero to the end of a line
    string[string_idx] = '\0';

    return string;
}

bb* bb_disjunction (bb* vector1, bb* vector2)
{
    if (vector1 == NULL || vector2 == NULL)
    {
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    //if the length of the second vector is longer than the first one then swap them
    if (vector2->last_part * 8 + vector2->last_bit > vector1->last_part * 8 + vector1->last_bit)
    {
        return bb_disjunction (vector2, vector1);
    }

    bb* vector = calloc (1, sizeof (bb));

    //checking whether memory is allocated
    if (vector == NULL)
    {
        bb_errno = ERR_MEM_NOT_ALLOC;
        return NULL;
    }
    vector->last_bit = vector1->last_bit;
    vector->last_part = vector1->last_part;

    //disjunction
    for (int i = 0; i <= vector2->last_part; i++)
    {
        vector->parts[i] = vector1->parts[i] | vector2->parts[i];
    }
    for (int i = vector2->last_part + 1; i <= vector1->last_part; i++)
    {
        vector->parts[i] = vector1->parts[i];
    }

    return vector;
}

bb* bb_conjunction (bb* vector1, bb* vector2)
{
    if (vector1 == NULL || vector2 == NULL)
    {
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    //if the length of the second vector is longer than the first one then swap them
    if (vector2->last_part * 8 + vector2->last_bit > vector1->last_part * 8 + vector1->last_bit)
    {
        return bb_conjunction (vector2, vector1);
    }

    bb* vector = calloc (1, sizeof (bb));

    //checking whether memory is allocated
    if (vector == NULL)
    {
        bb_errno = ERR_MEM_NOT_ALLOC;
        return NULL;
    }
    vector->last_bit = vector1->last_bit;
    vector->last_part = vector1->last_part;

    int length = vector2->last_part;

    //conjunction
    for (int i = 0; i < length; i++)
    {
        vector->parts[i] = vector1->parts[i] & vector2->parts[i];
    }
    for (int i = 0; i <= vector2->last_bit; i++)
    {
        if ((vector1->parts[length] & (1<<i)) && (vector2->parts[length] & (1 << i)))
        {
            vector->parts[length] |= (1 << i);
        }
    }

    return vector;
}

bb* bb_xor (bb* vector1, bb* vector2)
{
    if (vector1 == NULL || vector2 == NULL)
    {
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    //if the length of the second vector is longer than the first one then swap them
    if (vector2->last_part * 8 + vector2->last_bit > vector1->last_part * 8 + vector1->last_bit)
    {
        return bb_xor (vector2, vector1);
    }

    bb* vector = calloc (1, sizeof (bb));

    //checking whether memory is allocated
    if (vector == NULL)
    {
        bb_errno = ERR_MEM_NOT_ALLOC;
        return NULL;
    }

    vector->last_bit = vector1->last_bit;
    vector->last_part = vector1->last_part;

    //xor
    for (int i = 0; i <= vector2->last_part; i++)
    {
        vector->parts[i] = vector1->parts[i] ^ vector2->parts[i];
    }
    for (int i = vector2->last_part + 1; i <= vector1->last_part; i++)
    {
        vector->parts[i] = vector1->parts[i];
    }

    return vector;
}

bb* bb_inversion (bb* vector1)
{
    if (vector1 == NULL)
    {
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    bb* vector = calloc (1, sizeof (bb));

    //checking whether memory is allocated
    if (vector == NULL)
    {
        bb_errno = ERR_MEM_NOT_ALLOC;
        return NULL;
    }

    vector->last_bit = vector1->last_bit;
    vector->last_part = vector1->last_part;

    for (int i = 0; i <= vector1->last_part; i++)
    {
        vector->parts[i] = ~vector1->parts[i];
    }

    return vector;
}

bb* cycle_left_shift (bb* vector, int size_of_shift)
{
    //checking if the shift is less than one
    if (size_of_shift < 1)
    {
        bb_errno = ERR_SMALL_SHIFT;
        return NULL;
    }

    if (vector == NULL)
    {
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    bb* new_vector1 = left_shift (vector, size_of_shift);
    if (new_vector1==NULL)
    {
        goto free1;
    }

    bb* new_vector2 = rigth_shift (vector, vector->last_part * 8 + vector->last_bit + 1 - size_of_shift);
    if (new_vector2 == NULL)
    {
        goto free2;
    }

    bb* new_vector = bb_disjunction (new_vector1, new_vector2);
    if (new_vector == NULL)
    {
        goto free3;
    }

    free (new_vector2);
    free (new_vector1);

    new_vector->last_part = vector->last_part;
    new_vector->last_bit = vector->last_bit;

    return new_vector;

free3:
    free (new_vector);
free2:
    free (new_vector2);
free1:
    free (new_vector1);

    return NULL;
}

bb* cycle_rigth_shift (bb* vector, int size_of_shift)
{
    //checking if the shift is less than one
    if (size_of_shift < 1)
    {
        bb_errno = ERR_SMALL_SHIFT;
        return NULL;
    }

    if (vector == NULL)
    {
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    bb* new_vector1 = rigth_shift (vector, size_of_shift);

    if (new_vector1 == NULL)
    {
        goto free1;
    }

    bb* new_vector2 = left_shift (vector, vector->last_part * 8 + vector->last_bit + 1 - size_of_shift);
    if (new_vector2 == NULL)
    {
        goto free2;
    }

    bb* new_vector = bb_disjunction (new_vector1, new_vector2);
    if (new_vector == NULL)
    {
        goto free3;
    }

    free (new_vector2);
    free (new_vector1);

    new_vector->last_part = vector->last_part;
    new_vector->last_bit = vector->last_bit;

    return new_vector;

free3:
    free (new_vector);
free2:
    free (new_vector2);
free1:
    free (new_vector1);

    return NULL;
}

bb* bb_from_uint64_t (uint64_t num)
{
    bb* vector = calloc (1, sizeof (bb));

    //checking whether memory is allocated
    if (vector == NULL)
    {
        bb_errno = ERR_MEM_NOT_ALLOC;
        return NULL;
    }

    for (int i = 0; i < 8; i++)
    {
        vector->parts[i] |= (num >> 8 * i);
    }

    int position = 0;
    if (num>0)
    {
        position = trunc (log (num) / log (256));
    }

    vector->last_part = position;
    vector->last_bit = 0;

    for (int i = 7; i >=0; i--)
    {
        if (vector->parts[position]>>i)
        {
            vector->last_bit = i;
            break;
        }
    }

    return vector;
}