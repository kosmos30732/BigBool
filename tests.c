#include"bb.h"
#include<stdio.h>
#include<string.h>

#define RESET "\033[0m"
#define RED   "\033[31m"		/* Red */
#define GREEN "\033[32m"		/* Green */

#define LOG_ERR(msg) \
	printf(RED"%s:%d %s [ERR] %s\n"RESET,\
		__FILE__, __LINE__, __func__, msg);

int tests_logical_operations ()
{
	bb* vec1 = bb_from_string ("100100110101");
	bb* vec2 = bb_from_string    ("10011101");

	int count = 0;
	bb* output_vec=NULL;
	char* output_string = NULL;


	output_vec = bb_conjunction (vec1, vec2);
	output_string = bb_to_string (output_vec);
	if (strcmp (output_string, "000000010101") != 0)
	{
		LOG_ERR ("bb_conjunction not work (100100110101 & 10011101 -> != 000000010101)");
		count++;
	}
	if (output_string != NULL) free (output_string);
	if (output_vec != NULL) free (output_vec);


	output_vec = bb_disjunction (vec1, vec2);
	output_string = bb_to_string (output_vec);
	if (strcmp (output_string, "100110111101") != 0)
	{
		LOG_ERR ("bb_disjunction not work (100100110101 | 10011101 -> != 100110111101)");
		count++;
	}
	if (output_string != NULL) free (output_string);
	if (output_vec != NULL) free (output_vec);


	output_vec = bb_xor (vec1, vec2);
	output_string = bb_to_string (output_vec);
	if (strcmp (output_string, "100110101000") != 0)
	{
		LOG_ERR ("bb_xor not work (100100110101 ^ 10011101 -> != 100110101000)");
		count++;
	}
	if (output_string != NULL) free (output_string);
	if (output_vec != NULL) free (output_vec);


	output_vec = bb_inversion (vec1);
	output_string = bb_to_string (output_vec);
	if (strcmp (output_string, "011011001010") != 0)
	{
		LOG_ERR ("bb_inversion not work (100100110101 -> != 011011001010)");
		count++;
	}
	if (output_string != NULL) free (output_string);
	if (output_vec != NULL) free (output_vec);

	if (vec1 != NULL) free (vec1);
	if (vec2 != NULL) free (vec2);

	return count;
}

int tests_shifts ()
{
	bb* vec = bb_from_string ("100001010100110101");

	int count = 0;
	bb* output_vec = NULL;
	char* output_string = NULL;

	output_vec = left_shift (vec, 5);
	output_string = bb_to_string (output_vec);
	if (strcmp (output_string, "10000101010011010100000") != 0)
	{
		LOG_ERR ("left_shift not work (100001010100110101 -5> != 10000101010011010100000)");
		count++;
	}
	if (output_string != NULL) free (output_string);
	if (output_vec != NULL) free (output_vec);


	output_vec = rigth_shift (vec, 8);
	output_string = bb_to_string (output_vec);
	if (strcmp (output_string, "1000010101") != 0)
	{
		LOG_ERR ("rigth_shift not work (100001010100110101 -8> != 1000010101)");
		count++;
	}
	if (output_string != NULL) free (output_string);
	if (output_vec != NULL) free (output_vec);


	output_vec = cycle_left_shift (vec, 5);
	output_string = bb_to_string (output_vec);
	if (strcmp (output_string, "101010011010110000") != 0)
	{
		LOG_ERR ("cycle_left_shift not work (100001010100110101 -5> != 101010011010110000)");
		count++;
	}
	if (output_string != NULL) free (output_string);
	if (output_vec != NULL) free (output_vec);


	output_vec = cycle_rigth_shift (vec, 3);
	output_string = bb_to_string (output_vec);
	if (strcmp (output_string, "101100001010100110") != 0)
	{
		LOG_ERR ("cycle_rigth_shift not work (100001010100110101 -3> != 101100001010100110)");
		count++;
	}
	if (output_string != NULL) free (output_string);
	if (output_vec != NULL) free (output_vec);

	if (vec != NULL) free (vec);

	return count;
}

int tests_errors ()
{
	int count = 0;
	bb* vec = bb_from_string ("11010");
	bb* vector = NULL;
	

	bb_from_string ("1101023");
	if (bb_errno != ERR_WRONG_CHARS)
	{
		LOG_ERR ("Don't set ERR_WRONG_CHARS");
		count++;
	}
	

	left_shift (vec, 0);
	if (bb_errno != ERR_SMALL_SHIFT)
	{
		LOG_ERR ("Don't set ERR_SMALL_SHIFT");
		count++;
	}


	left_shift (vector, 5);
	if (bb_errno != ERR_NULL_INPUT)
	{
		LOG_ERR ("Don't set ERR_NULL_INPUT");
		count++;
	}


	left_shift (vec, 100000);
	if (bb_errno != ERR_BIG_SHIFT)
	{
		LOG_ERR ("Don't set ERR_BIG_SHIFT");
		count++;
	}


	rigth_shift (vec, 16);
	if (bb_errno != ERR_BIG_SHIFT)
	{
		LOG_ERR ("Don't set ERR_BIG_SHIFT");
		count++;
	}


	rigth_shift (vec, -2);
	if (bb_errno != ERR_SMALL_SHIFT)
	{
		LOG_ERR ("Don't set ERR_SMALL_SHIFT");
		count++;
	}

	rigth_shift (vector, 2);
	if (bb_errno != ERR_NULL_INPUT)
	{
		LOG_ERR ("Don't set ERR_NULL_INPUT");
		count++;
	}


	return count;
}

int tests_transform ()
{
	int count = 0;
	bb *vec1 = NULL, *vec2 = NULL;
	char* str1 = NULL, * str2 = NULL;

	vec1 = bb_from_uint64_t (0b110000000001110110111);
	vec2 = bb_from_string ("110000000001110110111");
	str1 = bb_to_string (vec1);
	str2 = bb_to_string (vec2);
	if (strcmp (str1, str2) != 0)
	{
		LOG_ERR ("Transformers don't work properly");
		count++;
	}

	if (vec1 != NULL) free (vec1);
	if (vec2 != NULL) free (vec2);
	if (str1 != NULL) free (str1);
	if (str2 != NULL) free (str2);


	return count;
}

int tests_corner_cases ()
{
	bb* vec = bb_from_string ("110101010101111101");

	int count = 0;
	bb* output_vec = NULL;
	char* output_string = NULL;

	output_vec = left_shift (vec, 142);
	output_string = bb_to_string (output_vec);
	if (strcmp (output_string, "1101010101011111010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000") != 0)
	{
		LOG_ERR ("left_shift not work (110101010101111101 -142> != 1101010101011111010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)");
		count++;
	}
	if (output_string != NULL) free (output_string);
	if (output_vec != NULL) free (output_vec);


	output_vec = rigth_shift (vec, 18);
	output_string = bb_to_string (output_vec);
	if (strcmp (output_string, "0") != 0)
	{
		LOG_ERR ("rigth_shift not work (110101010101111101 -18> != 0)");
		count++;
	}
	if (output_string != NULL) free (output_string);
	if (output_vec != NULL) free (output_vec);

	if (vec != NULL) free (vec);

	return count;
}

int tests_with_random ()
{
	int count = 0;

	srand (time (NULL));
	bb* vec1 = bb_from_uint64_t (rand ());
	bb* vec2 = bb_from_uint64_t (rand ());

	//xor vec1 ^ vec2
	bb* vec_xor = bb_xor (vec1, vec2);
	char* str_vec_xor = bb_to_string (vec_xor);
	if (vec_xor != NULL) free (vec_xor);


	//~vec1
	bb* not_vec1 = bb_inversion (vec1);
	//~vec2
	bb* not_vec2 = bb_inversion (vec2);

	//first part ~vec1 & vec2
	bb* first_part = bb_conjunction (not_vec1, vec2);
	//second part vec1 & ~vec2
	bb* second_part = bb_conjunction (vec1, not_vec2);

	vec_xor = bb_disjunction (first_part, second_part);
	char* str_other_vec_xor = bb_to_string (vec_xor);
	if (vec_xor != NULL) free (vec_xor);
	if (first_part != NULL) free (first_part);
	if (second_part != NULL) free (second_part);
	if (not_vec2 != NULL) free (not_vec2);
	if (vec2 != NULL) free (vec2);


	if (strcmp(str_vec_xor,str_other_vec_xor)!=0)
	{
		LOG_ERR ("(x xor y) not equal ((~x & y) | (x & ~y))");
		count++;
	}
	if (str_vec_xor != NULL) free (str_vec_xor);
	if (str_other_vec_xor != NULL) free (str_other_vec_xor);


	bb* vector = bb_conjunction (vec1, not_vec1);
	char* str_vector = bb_to_string (vector);

	bb* zero_vec = calloc (1, sizeof (bb));
	if (zero_vec == NULL)
	{
		zero_vec = calloc (1, sizeof (bb));
	}
	zero_vec->last_bit = vec1->last_bit;
	zero_vec->last_part = vec1->last_part;
	char* str_zero_vec = bb_to_string (zero_vec);

	if (strcmp(str_zero_vec,str_vector)!=0)
	{
		LOG_ERR ("bb inversion not work");
		count++;
	}

	if (vec1 != NULL) free (vec1);
	if (not_vec1 != NULL) free (not_vec1);
	if (vector != NULL) free (vector);
	if (zero_vec != NULL) free (zero_vec);
	if (str_zero_vec != NULL) free (str_zero_vec);
	if (str_vector != NULL) free (str_vector);

	return count;
}

int main ()
{
	int count=0;

	count += tests_shifts ();
	count += tests_corner_cases ();
	count += tests_logical_operations();
	count += tests_errors ();

	count += tests_with_random ();
	count += tests_with_random ();
	count += tests_with_random ();
	count += tests_with_random ();

	if (count>0)
	{
		puts ("");
		printf ("Number of failed tests: %d\n", count);
		return 0;
	}

	puts (GREEN"[OK] All ok!"RESET);

	return 0;
}