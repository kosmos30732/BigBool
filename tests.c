#include"bb.h"
#include<stdio.h>
#include<string.h>

#define RESET "\033[0m"
#define RED   "\033[31m"		/* Red */
#define GREEN "\033[32m"		/* Green */

#define LOG_ERR(msg) \
	printf(RED"%s:%d %s [ERR] %s\n"RESET,\
		__FILE__, __LINE__, __func__, msg);

int main ()
{
	bb* vector = calloc (1, sizeof (bb));;
	vector->last_bit = 1;
	vector->last_part = 0;
	vector->parts[0] = 2;

	if (strcmp(bb_to_string (vector),"10")!=0)
	{
		LOG_ERR ("bb_to_string not work (10!=10)");
		return 1;
	}

	if (strcmp(bb_to_string(bb_from_string ("11010000110")), "11010000110")!= 0)
	{
		LOG_ERR ("bb_from_string not work (11010000110 != 11010000110)");
		return 1;
	}

	if (strcmp (bb_to_string (left_shift(bb_from_string("1001"),5)), "100100000") != 0)
	{
		LOG_ERR ("left_shift not work (1001 -5> != 100100000)");
		return 1;
	}

	if (strcmp (bb_to_string (rigth_shift (bb_from_string ("101100001"), 5)), "1011") != 0)
	{
		LOG_ERR ("rigth_shift not work (101100001 -5> != 1011)");
		return 1;
	}

	if (strcmp (bb_to_string (bb_inversion (bb_from_string ("1011001"))), "0100110") != 0)
	{
		LOG_ERR ("bb_inversion not work (1011001-> != 0100110)");
		return 1;
	}

	if (strcmp (bb_to_string (bb_xor (bb_from_string ("001"), bb_from_string ("11111"))), "11110") != 0)
	{
		LOG_ERR ("bb_xor not work (001 ^ 11111-> != 0100110)");
		return 1;
	}

	if (strcmp (bb_to_string (bb_disjunction (bb_from_string ("1111"), bb_from_string ("1000101"))), "1001111") != 0)
	{
		LOG_ERR ("bb_disjunction not work (1111 | 1000101 -> != 1001111)");
		return 1;
	}

	if (strcmp (bb_to_string (bb_conjunction (bb_from_string ("0101"), bb_from_string ("1101101"))), "0000101") != 0)
	{
		LOG_ERR ("bb_conjunction not work (0101 & 1101101 -> != 0000101)");
		return 1;
	}

	if (strcmp (bb_to_string (cycle_left_shift (bb_from_string ("10010100000"), 3)), "10100000100") != 0)
	{
		LOG_ERR ("cycle_left_shift not work (10010100000 -3> != 10100000100)");
		return 1;
	}

	if (strcmp (bb_to_string (cycle_rigth_shift (bb_from_string ("101100001"), 5)), "000011011") != 0)
	{
		LOG_ERR ("cycle_rigth_shift not work (101100001 -5> != 000011011)");
		return 1;
	}

	bb_from_string ("1101023");
	if (bb_errno != ERR_WRONG_CHARS)
	{
		LOG_ERR ("Don't set ERR_WRONG_CHARS");
		return 1;
	}

	left_shift (bb_from_string ("11010"), 0);
	if (bb_errno != ERR_SMALL_SHIFT)
	{
		LOG_ERR ("Don't set ERR_SMALL_SHIFT");
		return 1;
	}

	left_shift (bb_from_string ("11010"), 100000);
	if (bb_errno != ERR_BIG_SHIFT)
	{
		LOG_ERR ("Don't set ERR_BIG_SHIFT");
		return 1;
	}

	rigth_shift (bb_from_string ("11010"), 16);
	if (bb_errno != ERR_BIG_SHIFT)
	{
		LOG_ERR ("Don't set ERR_BIG_SHIFT");
		return 1;
	}

	rigth_shift (bb_from_string ("11010"), -2);
	if (bb_errno != ERR_SMALL_SHIFT)
	{
		LOG_ERR ("Don't set ERR_SMALL_SHIFT");
		return 1;
	}

	puts (GREEN"[OK] All ok!"RESET);

	return 0;
}