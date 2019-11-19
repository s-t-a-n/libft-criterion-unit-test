#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#include <criterion/criterion.h>
#include <criterion/logging.h>

#include "libft.h"


// this is up to you:

// define SPLIT_EXPECT_ARRAY AS 1 if the following applies to your code:
// 		if string only contains delimiters or string doesnt exist ->
// 			expect an array with an empty string on the zero'th position and null on the second
// define SPLIT_EXPECT_ARRAY as 0 if the following applies to your code:
// 		if string only contains delimiters or string doesnt exist ->
// 			expect NULL as output
#define SPLIT_EXPECT_ARRAY 1

// define ITOA_ATOI_CHECK_ENTIRE_RANGE as 1 to go full OCD and check from INT_MIN to INT_MAX instead of from -10000 : 10000; this will drastically slow down the test
#define ITOA_ATOI_CHECK_ENTIRE_RANGE 0

// disable a part if needed
#define PART1 1
#define PART2 1

// disable this if your output is totally flooded by errors from <random strings>
#define RANDOMIZED_TESTS 1

// enable this if you want to be absolutely sure that your functions crash when given NULL on their inputs (or if you
// are scared of moulinette like me disable this and guard the shit out of your part2 -> do however pay attention that
// it COULD be that Moulinette will try to write to a NULL you are falsely returning)
#define PART2_PROTECT_CHECK 0

// THINGS TO TAKE NOTE OF:
// malloc failure is not handled
//
// lst functions as found in the 2019 curriculum are not tested, that is beyond the scope of these tests, besides the
// list functions are not that terribly usefull
//
// put.. functions are NOT tested (again, scope)

char *CRIT_randstring(size_t max_length)
{
	int length = rand() % --max_length;
	char *random_string = malloc(sizeof(char) * (length + 1));
    if (random_string)
	{
        for (int i = 0; i < length; i++)
		{
            random_string[i] = rand() % 128;
			if (random_string[i] == '\0' && rand() % 2)
				random_string[i] = 42;
		}
        random_string[length] = '\0';
        return random_string;
    }
	return (NULL);
}

// randstring_nullbyte garantees a nullbyte ONLY at the end of string
char *CRIT_randstring_nullbyte(size_t max_length)
{
	int length = rand() % --max_length;
	char *random_string = malloc(sizeof(char) * (length + 1));
    if (random_string)
	{
        for (int i = 0; i < length; i++)
		{
            random_string[i] = rand() % 128;
			if (random_string[i] == '\0')
				random_string[i] = 42;
		}
        random_string[length] = '\0';
        return random_string;
    }
	return (NULL);
}

unsigned char *CRIT_randmem(size_t length)
{
	unsigned char *random_string = malloc(sizeof(unsigned char) * length);
    if (random_string)
	{
        for (size_t i = 0; i < length; i++)
            random_string[i] = rand() % 256;
        return random_string;
    }
	return (NULL);
}

#if PART1

Test(strings, ft_strlen_segv, .signal = SIGSEGV)
{
   ft_strlen(NULL);
}

Test(strings, ft_strlen)
{
	char *str;

	str = "Hello";
    cr_expect_eq(ft_strlen(str), strlen(str), "Your ft_strlen doesnt work for |%s|",str);
	str = "He\0llo";
    cr_expect_eq(ft_strlen(str), strlen(str), "Your ft_strlen doesnt work for |%s|",str);
	str = "Heøllo";
    cr_expect_eq(ft_strlen(str), strlen(str), "Your ft_strlen doesnt work for |%s|",str);
	str = "";
    cr_expect_eq(ft_strlen(str), strlen(str), "Your ft_strlen doesnt work for |%s|",str);

#if RANDOMIZED_TESTS
	long bound = 1000;
	srand(time(NULL) * 470);
	for (long i = -bound; i < bound; i++)
	{
		str = CRIT_randstring(bound);
    	cr_expect_eq(ft_strlen(str), strlen(str), "Your ft_strlen doesnt work for <RANDOM STRING>");
		free(str);
	}
#endif
}

Test(strings, ft_strncmp_segv1, .signal = SIGSEGV)
{
	ft_strncmp(NULL, "Yallaaaaa", 100);
}

Test(strings, ft_strncmp_segv2, .signal = SIGSEGV)
{
	ft_strncmp("Yallaaaaa", NULL, 100);
}

Test(strings, ft_strncmp)
{
	char *str1;
	char *str2;
	size_t n = 0;

	str1 = "a";
	str2 = "b";
	n = 0;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "a\0";
	str2 = "b\200";
	n = 3;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "aa";
	str2 = "b";
	n = 2;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "a";
	str2 = "bb";
	n = 2;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "aYallaaaaa";
	str2 = "bYallaaaaa";
	n = 0;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "aYallaaaaa";
	str2 = "bYallaaaaa";
	n = 1;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "Yallbaaaa";
	str2 = "Yallaaaaa";
	n = 100;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "Yallbaaaa";
	str2 = "Yallaaaaa";
	n = 4;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "Yallbaaaa";
	str2 = "Yallaaaaa";
	n = 5;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "Yallbaaaa";
	str2 = "Yallaaaaa";
	n = 6;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "";
	str2 = "Yallaaaaa";
	n = 0;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "";
	str2 = "Yallaaaaa";
	n = 1;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "";
	str2 = "Yallaaaaa";
	n = 10;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "";
	str2 = "Yallaaaaa";
	n = 100;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "Yallaaaaa";
	str2 = "";
	n = 0;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "Yallaaaaa";
	str2 = "";
	n = 1;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "Yallaaaaa";
	str2 = "";
	n = 10;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "Yallaaaaa";
	str2 = "";
	n = 100;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "";
	str2 = "";
	n = 0;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "";
	str2 = "";
	n = 1;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "";
	str2 = "";
	n = 100;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "skrr";
	str2 = "";
	n = 0;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "skrr";
	str2 = "";
	n = 1;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "skrr";
	str2 = "";
	n = 100;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "";
	str2 = "skrr";
	n = 0;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "";
	str2 = "skrr";
	n = 1;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);

	str1 = "";
	str2 = "skrr";
	n = 100;
	cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1{%s} s2{%s} n{%lu}", str1, str2, n);


#if RANDOMIZED_TESTS
	srand(time(NULL) * 8366);
	long bound = 1000;
	n = 0;
	for (long i = -bound; i < bound; i++)
	{
		str1 = CRIT_randstring(bound);
		str2 = CRIT_randstring(bound);
		n++;
		cr_expect_eq(ft_strncmp(str1, str2, n), strncmp(str1, str2, n),"Your ft_strncmp doesnt work for s1<RANDOM STRING> s2<RANDOM STRING> n{%lu}", n);
		free(str1);
		free(str2);
	}
#endif
}

Test(strings, ft_strnstr_segv1, .signal = SIGSEGV)
{
	ft_strnstr(NULL, "needle", 100);
}

Test(strings, ft_strnstr_segv2, .signal = SIGSEGV)
{
	ft_strnstr("haystack", NULL, 100);
}

Test(strings,strnstr)
{
	char *needle;
	char *haystack;
	size_t n = 0;

	needle = "Skere";
	haystack = "Ministerie der Skere Tijden";
	n = 100;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "Skere";
	haystack = "Ministerie der Skere Tijden";
	n = 15;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "Skere";
	haystack = "Ministerie der Skere Tijden";
	n = 16;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "Skere";
	haystack = "Ministerie der Skere Tijden";
	n = 17;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "Skere";
	haystack = "Ministerie der Skere Tijden";
	n = 18;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "Skere";
	haystack = "Ministerie der Skere Tijden";
	n = 19;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "Skere";
	haystack = "Ministerie der Skere Tijden";
	n = 20;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "Skere";
	haystack = "Ministerie der Skere Tijden";
	n = 21;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "Skere";
	haystack = "Ministerie der Skere Tijden";
	n = 22;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "Skere";
	haystack = "Ministerie der Sker\0e Tijden";
	n = 19;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "Skere";
	haystack = "Ministerie der Sker\0e Tijden";
	n = 20;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "";
	haystack = "";
	n = 0;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "";
	haystack = "";
	n = 1;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "";
	haystack = "";
	n = 100;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "Skrrr";
	haystack = "";
	n = 0;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "Skrrr";
	haystack = "";
	n = 1;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "Skrr";
	haystack = "";
	n = 100;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "";
	haystack = "Skrr";
	n = 0;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "";
	haystack = "Skrr";
	n = 1;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "";
	haystack = "Skrr";
	n = 100;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "1234";
	haystack = "123";
	n = 0;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "1234";
	haystack = "123";
	n = 1;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "1234";
	haystack = "123";
	n = 100;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "123";
	haystack = "123";
	n = 0;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "123";
	haystack = "123";
	n = 1;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

	needle = "123";
	haystack = "123";
	n = 100;
	cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1{%s} s2{%s} n{%lu}", haystack, needle, n);

#if RANDOMIZED_TESTS
	srand(time(NULL) * 83663);
	long bound = 1000;
	n = 0;
	for (long i = -bound; i < bound; i++)
	{
		needle = CRIT_randstring(10);
		haystack = CRIT_randstring(1000);
		n++;
		cr_expect_eq(ft_strnstr(haystack, needle, n), strnstr(haystack, needle, n),"Your ft_strnstr doesnt work for s1<RANDOM STRING> s2<RANDOM STRING> n{%lu}", n);
		free(haystack);
		free(needle);
	}
#endif
}

Test(strings, ft_strchr_segv, .signal = SIGSEGV)
{
	ft_strchr(NULL, 100);
}

Test(strings, ft_strchr)
{
	char *str;
	int c;

	str = "";
	c = 0;
	cr_expect_eq(ft_strchr(str, c), strchr(str, c),"Your ft_strchr doesnt work for s1{%s} c{%i}", str, c);

	str = "";
	c = (int)'a';
	cr_expect_eq(ft_strchr(str, c), strchr(str, c),"Your ft_strchr doesnt work for s1{%s} c{%i}", str, c);

	str = "abc";
	c = (int)'d';
	cr_expect_eq(ft_strchr(str, c), strchr(str, c),"Your ft_strchr doesnt work for s1{%s} c{%i}", str, c);

	str = "abc";
	c = (int)'a';
	cr_expect_eq(ft_strchr(str, c), strchr(str, c),"Your ft_strchr doesnt work for s1{%s} c{%i}", str, c);

	str = "abc";
	c = (int)'b';
	cr_expect_eq(ft_strchr(str, c), strchr(str, c),"Your ft_strchr doesnt work for s1{%s} c{%i}", str, c);

	str = "abc";
	c = (int)'c';
	cr_expect_eq(ft_strchr(str, c), strchr(str, c),"Your ft_strchr doesnt work for s1{%s} c{%i}", str, c);

	str = "bc\0a";
	c = (int)'a';
	cr_expect_eq(ft_strchr(str, c), strchr(str, c),"Your ft_strchr doesnt work for s1{%s} c{%i}", str, c);

	// are you typecasting properly?
	str = "INT_MIN";
	c = INT_MIN;
	cr_expect_eq(ft_strchr(str, c), strchr(str, c),"Your ft_strchr doesnt work for s1{%s} c{%i}", str, c);

	str = "\200";
	c = 200;
	cr_expect_eq(ft_strrchr(str, c), strrchr(str, c),"Your ft_strrchr doesnt work for s1{%s} c{%i}", str, c);

	str = "INT_MAX";
	c = INT_MAX;
	cr_expect_eq(ft_strchr(str, c), strchr(str, c),"Your ft_strchr doesnt work for s1{%s} c{%i}", str, c);

	str = "aaaaaaaaaaaaaaaaaaaaaaaaa";
	c = (int)'a';
	cr_expect_eq(ft_strchr(str, c), strchr(str, c),"Your ft_strchr doesnt work for s1{%s} c{%i}", str, c);

	str = "aaaaaaaaaaaaaaaaaaaaaaaab";
	c = (int)'b';
	cr_expect_eq(ft_strchr(str, c), strchr(str, c),"Your ft_strchr doesnt work for s1{%s} c{%i}", str, c);

	str = "baaaaaaaaaaaaaaaaaaaaaaab";
	c = (int)'b';
	cr_expect_eq(ft_strchr(str, c), strchr(str, c),"Your ft_strchr doesnt work for s1{%s} c{%i}", str, c);

#if RANDOMIZED_TESTS
	srand(time(NULL) * 83616);
	long bound = 1000;
	for (long i = -bound; i < bound; i++)
	{
		str = CRIT_randstring(1000);
		c = rand() % 256;
		cr_expect_eq(ft_strchr(str, c), strchr(str, c),"Your ft_strchr doesnt work for s1 <RANDOM STRING> c{%d}", c);
		free(str);
	}
#endif
}

Test(strings, ft_strrchr_segv, .signal = SIGSEGV)
{
	ft_strrchr(NULL, 100);
}

Test(strings, ft_strrchr)
{
	char *str;
	int c;

	str = "";
	c = 0;
	cr_expect_eq(ft_strrchr(str, c), strrchr(str, c),"Your ft_strrchr doesnt work for s1{%s} c{%i}", str, c);

	str = "";
	c = (int)'a';
	cr_expect_eq(ft_strrchr(str, c), strrchr(str, c),"Your ft_strrchr doesnt work for s1{%s} c{%i}", str, c);

	str = "abc";
	c = (int)'d';
	cr_expect_eq(ft_strrchr(str, c), strrchr(str, c),"Your ft_strrchr doesnt work for s1{%s} c{%i}", str, c);

	str = "abc";
	c = (int)'a';
	cr_expect_eq(ft_strrchr(str, c), strrchr(str, c),"Your ft_strrchr doesnt work for s1{%s} c{%i}", str, c);

	str = "abc";
	c = (int)'b';
	cr_expect_eq(ft_strrchr(str, c), strrchr(str, c),"Your ft_strrchr doesnt work for s1{%s} c{%i}", str, c);

	str = "abc";
	c = (int)'c';
	cr_expect_eq(ft_strrchr(str, c), strrchr(str, c),"Your ft_strrchr doesnt work for s1{%s} c{%i}", str, c);

	str = "bc\0a";
	c = (int)'a';
	cr_expect_eq(ft_strrchr(str, c), strrchr(str, c),"Your ft_strrchr doesnt work for s1{%s} c{%i}", str, c);

	// are you typecasting properly?
	str = "INT_MIN";
	c = INT_MIN;
	cr_expect_eq(ft_strrchr(str, c), strrchr(str, c),"Your ft_strrchr doesnt work for s1{%s} c{%i}", str, c);

	str = "\200";
	c = 200;
	cr_expect_eq(ft_strrchr(str, c), strrchr(str, c),"Your ft_strrchr doesnt work for s1{%s} c{%i}", str, c);

	str = "INT_MAX";
	c = INT_MAX;
	cr_expect_eq(ft_strrchr(str, c), strrchr(str, c),"Your ft_strrchr doesnt work for s1{%s} c{%i}", str, c);

	str = "aaaaaaaaaaaaaaaaaaaaaaaaa";
	c = (int)'a';
	cr_expect_eq(ft_strrchr(str, c), strrchr(str, c),"Your ft_strrchr doesnt work for s1{%s} c{%i}", str, c);

	str = "aaaaaaaaaaaaaaaaaaaaaaaab";
	c = (int)'b';
	cr_expect_eq(ft_strrchr(str, c), strrchr(str, c),"Your ft_strrchr doesnt work for s1{%s} c{%i}", str, c);

	str = "baaaaaaaaaaaaaaaaaaaaaaab";
	c = (int)'b';
	cr_expect_eq(ft_strrchr(str, c), strrchr(str, c),"Your ft_strrchr doesnt work for s1{%s} c{%i}", str, c);

#if RANDOMIZED_TESTS
	srand(time(NULL) * 83216);
	int bound = 1000;
	for (int i = -bound; i < bound; i++)
	{
		str = CRIT_randstring(1000);
		c = rand() % 256;
		cr_expect_eq(ft_strrchr(str, c), strrchr(str, c),"Your ft_strrchr doesnt work for s1 <RANDOM STRING> c{%i}", c);
		free(str);
	}
#endif
}

Test(strings, ft_strlcpy_segv1, .signal = SIGSEGV)
{
	ft_strlcpy(NULL, malloc(100), 100);
}

Test(strings, ft_strlcpy_segv2, .signal = SIGSEGV)
{
	ft_strlcpy(malloc(100), NULL, 100);
}

Test(strings, ft_strlcpy)
{
	int msize = 1000;
	char *dst1 = malloc(msize);
	char *dst2 = malloc(msize);
	char *src;
	size_t n;

	src = "";
	n = 0;
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"Your ft_strlcpy doesnt work for src{%s} n{%i}", src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcpy doesnt work -> strlcpy{%s}, ft_strlcpy{%s}", dst1, dst2);

	src = "";
	n = 1;
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"Your ft_strlcpy doesnt work for src{%s} n{%i}", src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcpy doesnt work -> strlcpy{%s}, ft_strlcpy{%s}", dst1, dst2);

	src = "";
	n = 100;
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"Your ft_strlcpy doesnt work for src{%s} n{%i}", src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcpy doesnt work -> strlcpy{%s}, ft_strlcpy{%s}", dst1, dst2);

	src = "ab\0ab";
	n = 2;
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"Your ft_strlcpy doesnt work for src{%s} n{%i}", src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcpy doesnt work -> strlcpy{%s}, ft_strlcpy{%s}", dst1, dst2);

	src = "ab\0ab";
	n = 3;
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"Your ft_strlcpy doesnt work for src{%s} n{%i}", src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcpy doesnt work -> strlcpy{%s}, ft_strlcpy{%s}", dst1, dst2);

	src = "ab\0ab";
	n = 4;
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"Your ft_strlcpy doesnt work for src{%s} n{%i}", src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcpy doesnt work -> strlcpy{%s}, ft_strlcpy{%s}", dst1, dst2);

	src = "ab\0ab";
	n = 5;
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"Your ft_strlcpy doesnt work for src{%s} n{%i}", src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcpy doesnt work -> strlcpy{%s}, ft_strlcpy{%s}", dst1, dst2);

	src = "ab\0ab";
	n = 6;
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"Your ft_strlcpy doesnt work for src{%s} n{%i}", src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcpy doesnt work -> strlcpy{%s}, ft_strlcpy{%s}", dst1, dst2);

	src = "Yallaaaaa";
	n = 0;
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"Your ft_strlcpy doesnt work for src{%s} n{%i}", src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcpy doesnt work -> strlcpy{%s}, ft_strlcpy{%s}", dst1, dst2);

	src = "Yallaaaaa";
	n = 1;
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"Your ft_strlcpy doesnt work for src{%s} n{%i}", src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcpy doesnt work -> strlcpy{%s}, ft_strlcpy{%s}", dst1, dst2);

	src = "Yallaaaaa";
	n = 10;
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"Your ft_strlcpy doesnt work for src{%s} n{%i}", src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcpy doesnt work -> strlcpy{%s}, ft_strlcpy{%s}", dst1, dst2);

	src = "Yallaaaaa";
	n = 100;
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"Your ft_strlcpy doesnt work for src{%s} n{%i}", src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcpy doesnt work -> strlcpy{%s}, ft_strlcpy{%s}", dst1, dst2);

	src = "Yallaaaaa";
	n = 8;
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"Your ft_strlcpy doesnt work for src{%s} n{%i}", src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcpy doesnt work -> strlcpy{%s}, ft_strlcpy{%s}", dst1, dst2);

	src = "Yallaaaaa";
	n = 9;
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"Your ft_strlcpy doesnt work for src{%s} n{%i}", src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcpy doesnt work -> strlcpy{%s}, ft_strlcpy{%s}", dst1, dst2);

	src = "Yallaaaaa";
	n = 10;
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"Your ft_strlcpy doesnt work for src{%s} n{%i}", src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcpy doesnt work -> strlcpy{%s}, ft_strlcpy{%s}", dst1, dst2);

	src = "Yallaaaaa";
	n = 11;
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"Your ft_strlcpy doesnt work for src{%s} n{%i}", src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcpy doesnt work -> strlcpy{%s}, ft_strlcpy{%s}", dst1, dst2);

#if RANDOMIZED_TESTS
	srand(time(NULL) * 83616);
	int bound = 1000;
	for (int i = -bound; i < bound; i++)
	{
		src = CRIT_randstring(1000);
		n = rand() % 256;
		memset(dst1, 'A', msize);
		memset(dst2, 'A', msize);
		cr_expect_eq(ft_strlcpy(dst1, src, n), strlcpy(dst2, src, n),"RETURN: Your ft_strlcpy doesnt work for src <RANDOM STRING> n{%i}", (int)n);
		cr_expect(memcmp(dst1, dst2, msize) == 0,"MEMCMP: Your ft_strlcpy doesnt work -> strlcpy <RANDOM STRING>, ft_strlcpy <RANDOM STRING>");
		free(src);
	}
#endif
	free (dst1);
	free (dst2);
}

Test(strings, ft_strlcat_segv1, .signal = SIGSEGV)
{
	ft_strlcat(malloc(100), NULL, 100);
}

Test(strings, ft_strlcat_segv2, .signal = SIGSEGV)
{
	ft_strlcat(NULL, malloc(100), 100);
}

Test(strings, ft_strlcat)
{
	int msize = 1000;
	char *dst1 = malloc(msize);
	char *dst2 = malloc(msize);
	char *src;
	char *dst;
	size_t n;

	// original doesnt crash with this input!
	ft_strlcat(NULL, strdup("a"), 0);

	src = "";
	dst = "";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 0;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "";
	dst = "";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 1;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "";
	dst = "";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 100;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "def";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 0;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "def";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 1;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "def";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 3;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "def";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 4;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "def";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 5;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "def";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 6;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "def";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 7;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "def";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 8;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "ab";
	dst = "def";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 1;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "ab";
	dst = "def";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 2;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "ab";
	dst = "def";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 3;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "ab";
	dst = "def";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 4;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "ab";
	dst = "def";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 5;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "ab";
	dst = "def";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 6;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "ab";
	dst = "def";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 7;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "ab";
	dst = "def";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 8;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "de";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 1;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "de";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 2;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "de";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 3;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "de";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 4;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "de";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 5;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "de";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 6;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "de";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 7;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "de";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 8;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 0;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 1;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "abc";
	dst = "";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 100;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"Your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "";
	dst = "abc";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 0;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "";
	dst = "abc";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 1;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

	src = "";
	dst = "abc";
	memset(dst1, 'A', msize);
	memset(dst2, 'A', msize);
	strcpy(dst1, dst);
	strcpy(dst2, dst);
	n = 100;
	cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"your ft_strlcat doesnt work for dst{%s} src{%s} n{%i}", dst, src, (int)n);
	cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_strlcat doesnt work -> strlcat{%s}, ft_strlcat{%s}", dst1, dst2);

#if RANDOMIZED_TESTS
	srand(time(NULL) * 83616);
	int bound = 1000;
	for (int i = -bound; i < bound; i++)
	{
		src = CRIT_randstring(200);
		dst = CRIT_randstring(200);
		n = rand() % 1000;
		memset(dst1, 'A', msize);
		memset(dst2, 'A', msize);
		strcpy(dst1, dst);
		strcpy(dst2, dst);
		cr_expect_eq(ft_strlcat(dst1, src, n), strlcat(dst2, src, n),"RETURN: your ft_strlcat doesnt work for dst <RANDOM STRING> src <RANDOM STRING> n{%i}", (int)n);
		cr_expect(memcmp(dst1, dst2, msize) == 0,"MEMCMP: Your ft_strlcat doesnt work -> strlcat <RANDOM STRING>, ft_strlcat <RANDOM STRING>");
		free (src);
		free (dst);
	}
#endif
	free (dst1);
	free (dst2);
}

Test(strings, ft_strdup_segv1, .signal = SIGSEGV)
{
	ft_strdup(NULL);
}

Test(strings, ft_strdup)
{
	int msize;
	char *dst1;
	char *dst2;
	char *src;


	src = "";
	dst1 = ft_strdup(src);
	dst2 = strdup(src);
	msize = strlen(src) + 1;
	cr_expect(memcmp(dst1, dst2, msize) == 0,"1: Your ft_strdup doesnt work -> strdup{%s}", src);
	// mutability test
	dst1[0] = 'a';
	free (dst1);
	free (dst2);

	src = "Yallaaaaaaaaaa";
	dst1 = ft_strdup(src);
	dst2 = strdup(src);
	msize = strlen(src) + 1;
	cr_expect(memcmp(dst1, dst2, msize) == 0,"2: Your ft_strdup doesnt work -> strdup{%s}", src);
	// mutability test
	dst1[0] = 'a';
	free (dst1);
	free (dst2);

	src = "Yallaaa\0aaaaaaa";
	dst1 = ft_strdup(src);
	dst2 = strdup(src);
	msize = strlen(src) + 1;
	cr_expect(memcmp(dst1, dst2, msize) == 0,"3: Your ft_strdup doesnt work -> strdup{%s}", src);
	free (dst1);
	free (dst2);

	src = "";
	dst1 = ft_strdup(src);
	dst2 = strdup(src);
	msize = strlen(src) + 1;
	cr_expect(memcmp(dst1, dst2, msize) == 0,"4: Your ft_strdup doesnt work -> strdup{%s}", src);
	free (dst1);
	free (dst2);

	src = "";
	dst1 = ft_strdup(src);
	dst2 = strdup(src);
	msize = strlen(src) + 1;
	cr_expect(memcmp(dst1, dst2, msize) == 0,"5: Your ft_strdup doesnt work -> strdup{%s}", src);
	free (dst1);
	free (dst2);

#if RANDOMIZED_TESTS
	srand(time(NULL) * 83616);
	int bound = 1000;
	for (int i = -bound; i < bound; i++)
	{
		src = CRIT_randstring(200);
		dst1 = ft_strdup(src);
		dst2 = strdup(src);
		msize = strlen(src) + 1;
		cr_expect(memcmp(dst1, dst2, msize) == 0,"5: Your ft_strdup doesnt work -> strdup <RANDOM STRING>");
		free (src);
		free (dst1);
		free (dst2);
	}
#endif
} 

Test(strings, ft_isalpha)
{
	for (int i = -1000; i < 1000; i++)
	{
    	cr_expect_eq(ft_isalpha(i), isalpha(i), "Your ft_isalpha doesnt work for |%i|",i);
	}
}

Test(strings, ft_isdigit)
{
	for (int i = -1000; i < 1000; i++)
	{
    	cr_expect_eq(ft_isdigit(i), isdigit(i), "Your ft_isdigit doesnt work for |%i|",i);
	}
}

Test(strings, ft_isalnum)
{
	for (int i = -1000; i < 1000; i++)
	{
    	cr_expect_eq(ft_isalnum(i), isalnum(i), "Your ft_isalnum doesnt work for |%i|",i);
	}
}

Test(strings, ft_isascii)
{
	for (int i = -1000; i < 1000; i++)
	{
    	cr_expect_eq(ft_isascii(i), isascii(i), "Your ft_isascii doesnt work for |%i|",i);
	}
}

Test(strings, ft_isprint)
{
	for (int i = -1000; i < 1000; i++)
	{
    	cr_expect_eq(ft_isprint(i), isprint(i), "Your ft_isprint doesnt work for |%i|",i);
	}
}

Test(strings, ft_toupper)
{
	for (int i = -1000; i < 1000; i++)
	{
    	cr_expect_eq(ft_toupper(i), toupper(i), "Your ft_toupper doesnt work for |%i|",i);
	}
}

Test(strings, ft_tolower)
{
	for (int i = -1000; i < 1000; i++)
	{
    	cr_expect_eq(ft_tolower(i), tolower(i), "Your ft_tolower doesnt work for |%i|",i);
	}
}

Test(strings, ft_atoi_segv, .signal = SIGSEGV)
{
	ft_atoi(NULL);
}

Test(strings, ft_atoi)
{
	char *str;

	str = "";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);
	
	str = "Yallaaaaa";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "\0\0\0\0";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "a10";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "abc10";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "10a";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "10abc";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "-10abc";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "1a1a1a1a1a1a";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "10a10a10a";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "-10a-10a-10a";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "-a10";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "+10";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "+a10";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "+0";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "+-10";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "-+10";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "-2147483648";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	// your atoi overflows differently; do you care?
	str = "-2147483649";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "2147483647";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = "2147483648";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);
	
	str = "-0";
    cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

	str = malloc(256);
#if ITOA_ATOI_CHECK_ENTIRE_RANGE
	for (long long i = INT_MIN; i <= INT_MAX; i++)
#else
	long long bound  = 10000;
	for (long long i = -bound; i <= bound; i++)
#endif
	{
		sprintf(str,"-%lld",i);
    	cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

		sprintf(str,"%lld",i);
    	cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);

		sprintf(str,"+%lld",i);
    	cr_expect_eq(ft_atoi(str), atoi(str), "Your ft_atoi doesnt work for |%s|", str);
	}
	free (str);
}

Test(memory, ft_memset_segv1, .signal = SIGSEGV)
{
	ft_memset(NULL, 0, 100);
}

Test(memory, ft_memset_segv2, .signal = SIGSEGV)
{
	ft_memset(malloc(1), 0, -1);
}

Test(memory, ft_memset)
{ 
	int		msize = 1000;
	void	*dst1 = malloc (msize);
	void	*dst2 = malloc (msize);

	int bound = msize;
	for (int i = 0; i < bound; i++)
	{
		memset(dst1, 'A', msize);
		memset(dst2, 'A', msize);

		memset(dst1, i, i);
		ft_memset(dst2, i, i);

		cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_memset doesnt work -> ft_memset{int %i, size %i}", i, msize);
	}
	free(dst1);
	free(dst2);
}

Test(memory, ft_bzero_segv1, .signal = SIGSEGV)
{
	ft_bzero(NULL, 1000);
}
Test(memory, ft_bzero_segv2, .signal = SIGSEGV)
{
	ft_bzero(malloc(10), -1);
}

Test(memory, ft_bzero)
{
	int		msize = 1000;
	void	*dst1 = malloc (msize);
	void	*dst2 = malloc (msize);

	int bound = msize;
	for (int i = 0; i < bound; i++)
	{
		memset(dst1, 'A', msize);
		memset(dst2, 'A', msize);

		bzero(dst1, i);
		ft_bzero(dst2, i);

		cr_expect(memcmp(dst1, dst2, msize) == 0,"Your ft_memset doesnt work -> ft_memset{int %i, size %i}", i, msize);
	}
	free(dst1);
	free(dst2);
}

Test(memory, ft_memcpy_segv1, .signal = SIGSEGV)
{
	ft_memcpy(NULL, strdup("Yallaaa"), 1000);
}

Test(memory, ft_memcpy_segv2, .signal = SIGSEGV)
{
	ft_memcpy(strdup("Yallaaa"), NULL, 1000);
}

Test(memory, ft_memcpy)
{
	int		msize = 1000;
	void	*dst1 = malloc (msize);
	void	*dst2 = malloc (msize);

	cr_expect_null(ft_memcpy(NULL, NULL, 1000), "Your ft_memcpy doesnt work -> ft_memcpy{ dst : |NULL|, src: |NULL|, n |1000|}");
	cr_expect_null(ft_memcpy(NULL, dst1, 0), "Your ft_memcpy doesnt work -> ft_memcpy{ dst : |NULL|, src: |%s|, n |0|}", dst1);
	cr_expect(ft_memcpy(dst1, NULL, 0) == dst1, "Your ft_memcpy doesnt work -> ft_memcpy{ dst : |%s|, src: |NULL|, n |0|}", dst1);

#if RANDOMIZED_TESTS
	int bound = msize;
	for (int i = 0; i < bound; i++)
	{
		memset(dst1, 'A', msize);
		memset(dst2, 'A', msize);
		srand(time(NULL) * (i + 1));

		size_t length = rand() % msize;
		unsigned char *src = CRIT_randmem(length);
		if (src)
		{
			memcpy(dst1, src, length);
			void *ret = ft_memcpy(dst2, src, length);
			cr_expect(ret == dst2, "RETURN: Your ft_memcpy doesnt work -> ft_memcpy{dst: <RANDOM STRING>, src: <RANDOM STRING>,  n |%i|}", msize);
			cr_expect(memcmp(dst1, dst2, msize) == 0,"MEMCMP: Your ft_memcpy doesnt work -> ft_memcpy{dst: <RANDOM STRING>, src: <RANDOM STRING>,  n |%i|}", msize);
			free (src);
		}
	}
#endif
	free (dst1);
	free (dst2);
}

Test(memory, ft_memccpy_segv1, .signal = SIGSEGV)
{
	ft_memccpy(NULL, strdup("Yallaaa"), 0, 1000);
}

Test(memory, ft_memccpy_segv2, .signal = SIGSEGV)
{
	ft_memccpy(strdup("Yallaaa"), NULL, 0, 1000);
}

Test(memory, ft_memccpy_segv3, .signal = SIGSEGV)
{
	ft_memccpy(NULL, NULL, 0, 1000);
}

Test(memory, ft_memccpy)
{
	int		msize = 1000;
	void	*dst1 = malloc (msize);
	void	*dst2 = malloc (msize);

	cr_expect_null(ft_memccpy(NULL, dst1, 0, 0), "Your ft_memccpy doesnt work -> ft_memccpy{ dst : |NULL|, src: |%s|, n |0|}", dst1);
	cr_expect_null(ft_memccpy(dst1, NULL, 0, 0), "Your ft_memccpy doesnt work -> ft_memccpy{ dst : |%s|, src: |NULL|, n |0|}", dst1);

#if RANDOMIZED_TESTS
	int bound = msize;
	for (int i = 0; i < bound; i++)
	{
		memset(dst1, 'A', msize);
		memset(dst2, 'A', msize);
		srand(time(NULL) * (i + 1));

		size_t length = rand() % msize;
		unsigned char *src = CRIT_randmem(length);
		if (src)
		{
			int rand_char = rand() % 256;
			void *orig_m = memccpy(dst1, src, rand_char, length);
			void *your_m = ft_memccpy(dst2, src, rand_char, length);
			cr_expect(memcmp(dst1, dst2, msize) == 0,"MEMCMP: Your ft_memccpy doesnt work -> ft_memccpy{dst: <RANDOM STRING>, src: <RANDOM STRING>, n |%i|}", msize);
			if (orig_m == NULL || your_m == NULL)
				cr_expect(orig_m == your_m, "NULL: Your ft_memccpy doesnt work -> ft_memccpy{dst: <RANDOM STRING>, src: <RANDOM STRING>, n |%i|}", msize);
			else
				cr_expect(memcmp(orig_m, your_m, 10) == 0,"RETURN: Your ft_memccpy doesnt work -> ft_memccpy{dst: <RANDOM STRING>, src: <RANDOM STRING>, n |%i|}", msize);
			free (src);
		}
	}
#endif
	free (dst1);
	free (dst2);
}

Test(memory, ft_memmove_segv1, .signal = SIGSEGV)
{
	ft_memmove(NULL, strdup("Yallaaa"), 1000);
}

Test(memory, ft_memmove_segv2, .signal = SIGSEGV)
{
	ft_memmove(strdup("Yallaaa"), NULL, 1000);
}

Test(memory, ft_memmove)
{
	int		msize = 1000;
	void	*dst1 = malloc (msize);
	void	*dst2 = malloc (msize);

	cr_expect_null(ft_memmove(NULL, NULL, 1000), "Your ft_memmove doesnt work -> ft_memmove{ dst : |NULL|, src: |NULL|, n |1000|}");
	cr_expect_null(ft_memmove(NULL, NULL, 0), "Your ft_memmove doesnt work -> ft_memmove{ dst : |NULL|, src: |%s|, n |0|}", dst1);
	cr_expect_null(ft_memmove(NULL, dst1, 0), "Your ft_memmove doesnt work -> ft_memmove{ dst : |NULL|, src: |%s|, n |0|}", dst1);
	cr_expect(ft_memmove(dst1, NULL, 0) == dst1, "Your ft_memmove doesnt work -> ft_memmove{ dst : |%s|, src: |NULL|, n |0|}", dst1);

#if RANDOMIZED_TESTS
	int bound = msize;
	for (int i = 0; i < bound; i++)
	{
		memset(dst1, 'A', msize);
		memset(dst2, 'A', msize);
		srand(time(NULL) * i);

		size_t length = rand() % (msize / 2);
		unsigned char *src = CRIT_randmem(length);
		if (src)
		{
			memcpy(dst1, src, length);
			memcpy(dst2, src, length);
			if (src[0] > 64)
			{
				memmove(dst1 + 5, dst1, length);
				void *ret = ft_memmove(dst2 + 5, dst2, length);
				cr_expect(ret == dst2 + 5, "RETURN: Your ft_memmove doesnt work -> ft_memmove{dst: <RANDOM STRING>, src: <RANDOM STRING>,  n |%i|}", msize);
				cr_expect(memcmp(dst1, dst2, msize) == 0,"MEMCMP: Your ft_memmove doesnt work -> ft_memmove{dst: <RANDOM STRING>, src: <RANDOM STRING>,  n |%i|}",  msize);
			}
			else
			{
				memmove(dst1, dst1 + 5, length);
				void *ret = ft_memmove(dst2, dst2 + 5 , length);
				cr_expect(ret == dst2, "RETURN: Your ft_memmove doesnt work -> ft_memmove{dst: <RANDOM STRING>, src: <RANDOM STRING>,  n |%i|}", msize);
				cr_expect(memcmp(dst1, dst2, msize) == 0,"MEMCMP: Your ft_memmove doesnt work -> ft_memmove{dst: <RANDOM STRING>, src:, <RANDOM STRING>  n |%i|}", msize);
			}
			free (src);
		}
	}
#endif
	free (dst1);
	free (dst2);
}

Test(memory, ft_memchr_segv1, .signal = SIGSEGV)
{
	ft_memchr(NULL, 0, 1000);
}

Test(memory, ft_memchr)
{
	int				msize = 1000;
	void			*dst = malloc (msize);
	unsigned char	c;
	void 			*orig;
	void 			*yours;

	
	memset(dst, 'A', msize);
	c = 255;
	*(unsigned char *)(dst + 10) = c;
	orig = memchr(dst, c, msize);
	yours = ft_memchr(dst, c, msize);
	cr_expect(orig == yours, "TYPECAST: Your ft_memchr doesnt work -> ft_memchr{dst: |%s|, char: |%c|,  n |%i|}", dst, c, msize);
	
	cr_expect_null(ft_memchr(NULL, 100, 0), "NULL: Your ft_memchr doesnt work -> ft_memchr{ dst : |NULL|, c : |100|, n : |0|}");

#if RANDOMIZED_TESTS
	int bound = msize;
	for (int i = -bound; i < bound; i++)
	{
		memset(dst, 'A', msize);
		srand(time(NULL) * i);

		size_t length = rand() % (msize / 2);
		unsigned char *src = CRIT_randmem(length);
		if (src)
		{
			memcpy(dst, src, length);
			c = rand() % 256;
			orig = memchr(dst, c, msize);
			yours = ft_memchr(dst, c, msize);

			cr_expect(orig == yours, "Your ft_memchr doesnt work -> ft_memchr{dst: <RANDOM STRING>, char: |%c|,  n |%i|}", c, msize);
			free (src);
		}
	}
#endif
	free (dst);
}

// this could be a false error if you check if "dst == src" and return 0 immediately (which is not part of original
// functions but otherwise a good idea, no?)
Test(memory, ft_memcmp_segv1, .signal = SIGSEGV)
{
	ft_memcmp(NULL, NULL, 1000);
}

Test(memory, ft_memcmp_segv2, .signal = SIGSEGV)
{
	ft_memcmp(NULL, strdup("abc"), 1000);
}

Test(memory, ft_memcmp_segv3, .signal = SIGSEGV)
{
	ft_memcmp(strdup("abc"), NULL, 1000);
}

Test(memory, ft_memcmp)
{
	int		msize = 1000;
	unsigned char	*dst1 = malloc(msize);
	unsigned char	*dst2 = malloc(msize);

	cr_expect(ft_memcmp(NULL, NULL, 0) == 0, "Your ft_memcmp doesnt work -> ft_memcmp{ s1 : |NULL|, s2 : |NULL|, n : |0|}");

#if RANDOMIZED_TESTS
	size_t bound = msize;
	for (size_t i = 0; i < bound; i++)
	{
		memset(dst1, 'A', msize);
		memset(dst2, 'A', msize);
		srand(time(NULL) * (i + 1));

		size_t src1_length = rand() % (msize / 2);
		unsigned char *src1 = CRIT_randmem(src1_length);

		size_t src2_length = rand() % (msize / 2);
		unsigned char *src2 = CRIT_randmem(src2_length);

		memcpy(dst1, src1, src1_length);
		memcpy(dst2, src2, src2_length);
		if (src1 && src2)
		{
			int orig = memcmp(dst1, dst2, i);
			int yours = ft_memcmp(dst1, dst2, i);
			cr_expect(orig == yours, "Your ft_memcmp doesnt work -> ft_memcmp{s1: <RANDOM STRING>, s2: <RANDOM STRING>,  n |%i|}", msize);
			free (src1);
			free (src2);
		}
	}
#endif
	free (dst1);
	free (dst2);
} 

Test(memory, ft_calloc)
{
	int		msize = 1000;
	int		count = 10;
	void	*dst1 = calloc (count, msize);
	void	*dst2 = ft_calloc (count, msize);

	cr_expect(!(dst1 == NULL ^ dst2 == NULL), "Your ft_calloc doesnt work -> ft_calloc{count : |%i|, size : |%i|}", count, msize);
	cr_expect(memcmp(dst1, dst2, msize * count) == 0, "Your ft_calloc doesnt work -> ft_calloc{count : |%i|, size : |%i|}", count, msize);
	free (dst1);
	free (dst2);
}

#endif

#if PART2

#if PART2_PROTECT_CHECK == 1
Test(strings, ft_substr_segv1, .signal = SIGSEGV)
{
	ft_substr(NULL, 10, 10);
}
#endif

// your malloc'ing way to much for a string that can only contain 'C'!
Test(strings, ft_substr_segv2, .signal = SIGSEGV)
{
	// huge size is due too memory paging (malloc(1) will reserve just one page (4KB of memory and you can freely write
	// to that without segfaulting)
	char *str = ft_substr("ABC", 2, 500000000);
	str[49999000] = 'a';
}

Test(strings, ft_substr)
{
	char *dst;
	char *src;
	unsigned int start;
	size_t len;
	
	src = "Beter een zekere snack dan een onzekere maaltijd zei de kannibaal tegen de patatboer";
	start = 6;
	len = 4;
	dst = ft_substr(src, start, len);
	cr_expect(strncmp(dst, "een ", len + 1) == 0, "Your ft_substr doesnt work -> ft_substr{s : |%s|, start : |%i|, len : |%i|}", src, start, (int)len);
	free(dst);

	src = "Beter een zekere snack dan een onzekere maaltijd zei de kannibaal tegen de patatboer";
	start = 31;
	len = 17;
	dst = ft_substr(src, start, len);
	cr_expect(strncmp(dst, "onzekere maaltijd", len + 1) == 0, "Your ft_substr doesnt work -> ft_substr{s : |%s|, start : |%i|, len : |%i|}", src, start, (int)len);
	// check mutability
	dst[5] = 'a';
	free(dst);

	src = "abc";
	start = 100;
	len = 100;
	dst = ft_substr(src, start, len);
	cr_expect(strncmp(dst, "", len + 1) == 0, "Your ft_substr doesnt work -> ft_substr{s : |%s|, start : |%i|, len : |%i|}", src, start, (int)len);
	free(dst);

	src = "abc";
	start = 100;
	len = 0;
	dst = ft_substr(src, start, len);
	cr_expect(strncmp(dst, "", len + 1) == 0, "Your ft_substr doesnt work -> ft_substr{s : |%s|, start : |%i|, len : |%i|}", src, start, (int)len);
	// check mutability
	dst[0] = 'a';
	free(dst);

	src = "abc";
	start = 100;
	len = 0;
	dst = ft_substr(src, start, len);
	cr_expect(strncmp(dst, "", len + 1) == 0, "Your ft_substr doesnt work -> ft_substr{s : |%s|, start : |%i|, len : |%i|}", src, start, (int)len);
	free(dst);

	src = "abc";
	start = 2;
	len = 1;
	dst = ft_substr(src, start, len);
	cr_expect(strncmp(dst, "c", len + 1) == 0, "Your ft_substr doesnt work -> ft_substr{s : |%s|, start : |%i|, len : |%i|}", src, start, (int)len);
	free(dst);

	src = "abc";
	start = 2;
	len = 100;
	dst = ft_substr(src, start, len);
	cr_expect(strncmp(dst, "c", len + 1) == 0, "Your ft_substr doesnt work -> ft_substr{s : |%s|, start : |%i|, len : |%i|}", src, start, (int)len);
	free(dst);

	src = "abc";
	start = 3;
	len = 1;
	dst = ft_substr(src, start, len);
	cr_expect(strncmp(dst, "", len + 1) == 0, "Your ft_substr doesnt work -> ft_substr{s : |%s|, start : |%i|, len : |%i|}", src, start, (int)len);
	free(dst);

	src = "abc\0abc";
	start = 0;
	len = 6;
	dst = ft_substr(src, start, len);
	cr_expect(strncmp(dst, "abc", len + 1) == 0, "Your ft_substr doesnt work -> ft_substr{s : |%s|, start : |%i|, len : |%i|}", src, start, (int)len);
	free(dst);

#if RANDOMIZED_TESTS
	// try to crash it
	size_t msize = 1000;
	long bound = 1000;
	for (long i = -bound; i < bound; i++)
	{
		srand(time(NULL) * (i + 1));
		src = CRIT_randstring(msize);
		start = rand() % (msize / 2);
		len = start + rand() % (msize / 2);
		dst = ft_substr(src, start, len);
		free (src);
		free (dst);
	}
#endif
}

#if PART2_PROTECT_CHECK
Test(strings, ft_strjoin_segv2, .signal = SIGSEGV)
{
	ft_strjoin(NULL, "abc");
}

Test(strings, ft_strjoin_segv3, .signal = SIGSEGV)
{
	ft_strjoin("abc", NULL);
}

Test(strings, ft_strjoin_segv4, .signal = SIGSEGV)
{ 
	ft_strjoin(NULL, NULL);
}
#endif

Test(strings, ft_strjoin)
{
	char *src1;
	char *src2;
	char *dst;

	src1 = "abc ";
	src2 = "def";
	dst = ft_strjoin(src1, src2);
	cr_expect(strcmp(dst, "abc def") == 0, "Your ft_strjoin doesnt work -> ft_strjoin{s1 : |%s|, s2 : |%s|}", src1, src2);
	// mutability check
	dst[0] = 'a';
	free (dst);

	src1 = "";
	src2 = "def";
	dst = ft_strjoin(src1, src2);
	cr_expect(strcmp(dst, "def") == 0, "Your ft_strjoin doesnt work -> ft_strjoin{s1 : |%s|, s2 : |%s|}", src1, src2);
	free (dst);

	src1 = "";
	src2 = "";
	dst = ft_strjoin(src1, src2);
	cr_expect(strcmp(dst, "") == 0, "Your ft_strjoin doesnt work -> ft_strjoin{s1 : |%s|, s2 : |%s|}", src1, src2);
	// mutability check
	dst[0] = 'a';
	free (dst);

	src1 = "a\tbcdefghi";
	src2 = "abc\0defghi";
	dst = ft_strjoin(src1, src2);
	cr_expect(strcmp(dst, "a\tbcdefghiabc") == 0, "Your ft_strjoin doesnt work -> ft_strjoin{s1 : |%s|, s2 : |%s|}", src1, src2);
	free (dst);

#if RANDOMIZED_TESTS
	int msize = 1000;
	char *dst1 = malloc(msize);
	int bound = msize / 2;
	for (int i = -bound; i < bound; i++)
	{
		srand(time(NULL) * (i + 1));
		memset(dst1, 'A', msize);
		src1 = CRIT_randstring_nullbyte(msize / 2);
		src2 = CRIT_randstring_nullbyte(msize / 2);
		strlcpy(dst1, src1, msize / 2);
		strlcat(dst1, src2, msize);
		if (src1 && src2)
		{
			char *dst2 = ft_strjoin(src1, src2);

			cr_expect(strlen(dst1) == strlen(dst2), "STRLEN: Your ft_strjoin doesnt work -> ft_memcmp{s1: <RANDOM STRING>, s2: <RANDOM STRING>");
			cr_expect(memcmp(dst1, dst2, strlen(dst1)) == 0, "MEMCMP: Your ft_strjoin doesnt work -> ft_memcmp{s1: <RANDOM STRING>, s2: <RANDOM STRING>");
			free (src1);
			free (src2);
			free (dst2);
		}
	}
	free (dst1);
#endif
}

#if PART2_PROTECT_CHECK == 1
Test(strings, ft_strtrim_segv1, .signal = SIGSEGV)
{ 
	ft_strtrim(NULL, NULL);
}

Test(strings, ft_strtrim_segv2, .signal = SIGSEGV)
{ 
	ft_strtrim(NULL, strdup("abc"));
}
#endif

Test(strings, ft_strtrim)
{
	char *src;
	char *set;
	char *dst;

	src = "abrakadabra";
	set = NULL;
	dst = ft_strtrim(src, set);
	cr_expect(strcmp(dst, src) == 0, "Your ft_strtrim doesnt work -> ft_strtrim{s1 : |%s|, set : |%s|}", src, set);
	// mutability test
	dst[0] = 'a';
	free (dst);

	src = "abrakadabra";
	set = "a";
	dst = ft_strtrim(src, set);
	cr_expect(strcmp(dst, "brakadabr") == 0, "Your ft_strtrim doesnt work -> ft_strtrim{s1 : |%s|, set : |%s|}", src, set);
	// mutability test
	dst[0] = 'a';
	free (dst);

	src = "abc";
	set = "a";
	dst = ft_strtrim(src, set);
	cr_expect(strcmp(dst, "bc") == 0, "Your ft_strtrim doesnt work -> ft_strtrim{s1 : |%s|, set : |%s|}", src, set);
	free (dst);

	src = "abc";
	set = "c";
	dst = ft_strtrim(src, set);
	cr_expect(strcmp(dst, "ab") == 0, "Your ft_strtrim doesnt work -> ft_strtrim{s1 : |%s|, set : |%s|}", src, set);
	free (dst);

	src = "brakadabr";
	set = "a";
	dst = ft_strtrim(src, set);
	cr_expect(strcmp(dst, "brakadabr") == 0, "Your ft_strtrim doesnt work -> ft_strtrim{s1 : |%s|, set : |%s|}", src, set);
	free (dst);

	src = "aaaaaaaaaaaaaaaaaaa";
	set = "bcdefghaijkl";
	dst = ft_strtrim(src, set);
	cr_expect(strcmp(dst, "") == 0, "Your ft_strtrim doesnt work -> ft_strtrim{s1 : |%s|, set : |%s|}", src, set);
	free (dst);

	src = "";
	set = "";
	dst = ft_strtrim(src, set);
	cr_expect(strcmp(dst, "") == 0, "Your ft_strtrim doesnt work -> ft_strtrim{s1 : |%s|, set : |%s|}", src, set);
	// mutability test
	dst[0] = 'a';
	free (dst);

	src = "";
	set = "abrakadabra";
	dst = ft_strtrim(src, set);
	cr_expect(strcmp(dst, "") == 0, "Your ft_strtrim doesnt work -> ft_strtrim{s1 : |%s|, set : |%s|}", src, set);
	free (dst);

	src = "abrakadabra";
	set = "";
	dst = ft_strtrim(src, set);
	cr_expect(strcmp(dst, "abrakadabra") == 0, "Your ft_strtrim doesnt work -> ft_strtrim{s1 : |%s|, set : |%s|}", src, set);
	free (dst);

	src = "\t\b\n";
	set = "\b";
	dst = ft_strtrim(src, set);
	cr_expect(strcmp(dst, "\t\b\n") == 0, "Your ft_strtrim doesnt work -> ft_strtrim{s1 : |%s|, set : |%s|}", src, set);
	free (dst);

	src = "\t\b\n";
	set = "\t";
	dst = ft_strtrim(src, set);
	cr_expect(strcmp(dst, "\b\n") == 0, "Your ft_strtrim doesnt work -> ft_strtrim{s1 : |%s|, set : |%s|}", src, set);
	free (dst);

#if RANDOMIZED_TESTS
	// try to crash it
	int msize = 1000;
	int bound = 10000;
	for (int i = -bound; i < bound; i++)
	{
		srand(time(NULL) * (i + 1));
		src = CRIT_randstring(msize);
		set = CRIT_randstring(10);
		dst = ft_strtrim(src, set);
		free(src);
		free(set);
		free(dst);
	}
#endif
}

int		CRIT_is_array_equal(char **returned, char **expected, int len)
{
	while (len >= 0)
	{
		if ((expected[len] != NULL && returned[len] == NULL) || (expected[len] == NULL && returned[len] != NULL))
			return (1);	
		else if (expected[len] != NULL && strcmp(expected[len], returned[len]) != 0)
			return (1);
		len--;
	}
	return (0);
}

void	*CRIT_free_array(void **ptr_array)
{
	size_t	ctr;

	if (!ptr_array)
		return (NULL);
	ctr = 0;
	while (ptr_array[ctr] != NULL)
	{
		free(ptr_array[ctr]);
		ctr++;
	}
	free(ptr_array);
	return (NULL);
}

Test(strings, ft_split)
{
	char *src;
	char delim;
	char **expected;
	char **returned;
	int len;

	len = 2;
	expected = malloc(sizeof(char *) * (len + 1));
	expected[0] = "abc";
	expected[1] = "def";
	expected[2] = NULL;
	src = "abc:def";
	delim = ':';
	returned = ft_split(src, delim);
	cr_expect(CRIT_is_array_equal(returned, expected, len) == 0, "Your ft_split doesnt work -> ft_split{s : |%s|, c : |%c|}", src, delim);
	free (expected);
	CRIT_free_array((void **)returned);

	len = 1;
	expected = malloc(sizeof(char *) * (len + 1));
	expected[0] = ":::::::";
	expected[1] = NULL;
	src = ":::::::";
	delim = 'b';
	returned = ft_split(src, delim);
	cr_expect(CRIT_is_array_equal(returned, expected, len) == 0, "Your ft_split doesnt work -> ft_split{s : |%s|, c : |%c|}", src, delim);
	free (expected);
	CRIT_free_array((void **)returned);

	len = 1;
	expected = malloc(sizeof(char *) * (len + 1));
	expected[0] = "def";
	expected[1] = NULL;
	src = ":::::::def";
	delim = ':';
	returned = ft_split(src, delim);
	cr_expect(CRIT_is_array_equal(returned, expected, len) == 0, "Your ft_split doesnt work -> ft_split{s : |%s|, c : |%c|}", src, delim);
	free (expected);
	CRIT_free_array((void **)returned);

	len = 1;
	expected = malloc(sizeof(char *) * (len + 1));
	expected[0] = "abc";
	expected[1] = NULL;
	src = "abc::::::::::";
	delim = ':';
	returned = ft_split(src, delim);
	cr_expect(CRIT_is_array_equal(returned, expected, len) == 0, "Your ft_split doesnt work -> ft_split{s : |%s|, c : |%c|}", src, delim);
	free (expected);
	CRIT_free_array((void **)returned);

	len = 3;
	expected = malloc(sizeof(char *) * (len + 1));
	expected[0] = "abra";
	expected[1] = "ka";
	expected[2] = "dabra";
	expected[3] = NULL;
	src = "::::::::::::abra:::::::::::ka:::::::::::dabra:::::::";
	delim = ':';
	returned = ft_split(src, delim);
	cr_expect(CRIT_is_array_equal(returned, expected, len) == 0, "Your ft_split doesnt work -> ft_split{s : |%s|, c : |%c|}", src, delim);
	free (expected);
	CRIT_free_array((void **)returned);

#if SPLIT_EXPECT_ARRAY == 0
	len = 0;
	expected = malloc(sizeof(char *) * (len + 1));
	expected[0] = NULL;
	src = "::::::\0::::::abra:::::::::::ka:::::::::::dabra:::::::";
	delim = ':';
	returned = ft_split(src, delim);
	cr_expect(CRIT_is_array_equal(returned, expected, len) == 0, "Your ft_split doesnt work -> ft_split{s : |%s|, c : |%c|}", src, delim);
	free (expected);
	CRIT_free_array((void **)returned);

	len = 0;
	expected = malloc(sizeof(char *) * (len + 1));
	expected[0] = NULL;
	src = "";
	delim = '\0';
	returned = ft_split(src, delim);
	cr_expect(CRIT_is_array_equal(returned, expected, len) == 0, "Your ft_split doesnt work -> ft_split{s : |%s|, c : |%c|}", src, delim);
	free (expected);
	CRIT_free_array((void **)returned);
#else
	len = 1;
	expected = malloc(sizeof(char *) * (len + 1));
	expected[0] = "";
	expected[1] = NULL;
	src = "::::::\0::::::abra:::::::::::ka:::::::::::dabra:::::::";
	delim = ':';
	returned = ft_split(src, delim);
	cr_expect(CRIT_is_array_equal(returned, expected, len) == 0, "Your ft_split doesnt work -> ft_split{s : |%s|, c : |%c|}", src, delim);
	free (expected);
	CRIT_free_array((void **)returned);

	len = 1;
	expected = malloc(sizeof(char *) * (len + 1));
	expected[0] = "";
	expected[1] = NULL;
	src = "";
	delim = '\0';
	returned = ft_split(src, delim);
	cr_expect(CRIT_is_array_equal(returned, expected, len) == 0, "Your ft_split doesnt work -> ft_split{s : |%s|, c : |%c|}", src, delim);
	free (expected);
	CRIT_free_array((void **)returned);
#endif

#if RANDOMIZED_TESTS
	// try to crash it
	int msize = 1000;
	int bound = 10000;
	for (int i = -bound; i < bound; i++)
	{
		srand(time(NULL) * (i + 1));
		src = CRIT_randstring(msize);
		delim = rand() % 128;
		returned = ft_split(src, delim);
		free(src);
		CRIT_free_array((void **)returned);
	}
#endif
}

Test(strings, ft_itoa)
{
	size_t msize = 256;
	char *cmp = malloc(msize);
	long	n;

	n = INT_MIN;
	sprintf(cmp,"%li",n);
	cr_expect(strcmp(ft_itoa(n), cmp) == 0, "Your ft_itoa doesnt work -> ft_itoa{n : |%li|\n", n);

	n = INT_MAX;
	sprintf(cmp,"%li",n);
	cr_expect(strcmp(ft_itoa(n), cmp) == 0, "Your ft_itoa doesnt work -> ft_itoa{n : |%li|\n", n);

	n = -0;
	sprintf(cmp,"%li",n);
	cr_expect(strcmp(ft_itoa(n), cmp) == 0, "Your ft_itoa doesnt work -> ft_itoa{n : |%li|\n", n);

#if ITOA_ATOI_CHECK_ENTIRE_RANGE
	for (long i = INT_MIN; i <= INT_MAX; i++)
#else
	long bound = 10000;
	for (long i = -bound; i <= bound; i++)
#endif
	{
		n = i;
		sprintf(cmp,"%li",n);
		cr_expect(strcmp(ft_itoa(n), cmp) == 0, "BOUNDED: Your ft_itoa doesnt work -> ft_itoa{n : |%li|\n", n);

		// and again, randomized, just to fuck with the computer a bit
		n = rand() % (INT_MAX);
		sprintf(cmp,"%li",n);
		cr_expect(strcmp(ft_itoa(n), cmp) == 0, "RANDOM: Your ft_itoa doesnt work -> ft_itoa{n : |%li|\n", n);
	}
	free (cmp);
}

char CRIT_f1 (unsigned int i, char c)
{
	i = 0;
	return (c + 1);
}

char CRIT_f2 (unsigned int i, char c)
{
	if (i == 2)
		return ('A');
	else
		return (c + 1);
}

char CRIT_f3 (unsigned int i, char c)
{
	i = c;
	return (0);
}

#if PART2_PROTECT_CHECK == 1
Test(strings, ft_strmapi_segv1, .signal = SIGSEGV)
{ 
	ft_strmapi(NULL, CRIT_f1);
}

Test(strings, ft_strmapi_segv2, .signal = SIGSEGV)
{ 
	ft_strmapi("De Zon De Maan Veel Werk Geen Baan", NULL);
}
#endif

Test(strings, ft_strmapi)
{
	char *cmp;
	char *src;
	char *dst;

	src = "123";
	cmp = "234";
	dst = ft_strmapi(src, CRIT_f1);
	cr_expect(strcmp(dst, cmp) == 0, "Your ft_strmapi doesnt work -> ft_strmapi{s : |%s|, f : |f1|", src);
	// check mutability
	dst[0] = 'a';
	free (dst);

	src = "";
	cmp = "";
	dst = ft_strmapi(src, CRIT_f1);
	cr_expect(strcmp(dst, cmp) == 0, "Your ft_strmapi doesnt work -> ft_strmapi{s : |%s|, f : |f1|", src);
	free (dst);

	src = "123";
	cmp = "23A";
	dst = ft_strmapi(src, CRIT_f2);
	cr_expect(strcmp(dst, cmp) == 0, "Your ft_strmapi doesnt work -> ft_strmapi{s : |%s|, f : |f1|", src);
	free (dst);

	src = "";
	cmp = "";
	dst = ft_strmapi(src, CRIT_f2);
	cr_expect(strcmp(dst, cmp) == 0, "Your ft_strmapi doesnt work -> ft_strmapi{s : |%s|, f : |f1|", src);
	free (dst);

	src = "De Zon De Maan Veel Werk Geen Baan";
	cmp = "";
	dst = ft_strmapi(src, CRIT_f3);
	cr_expect(strcmp(dst, cmp) == 0, "Your ft_strmapi doesnt work -> ft_strmapi{s : |%s|, f : |f1|", src);
	free (dst);

#if RANDOMIZED_TESTS
	// try to crash it
	int msize = 1000;
	int bound = 10000;
	for (int i = -bound; i < bound; i++)
	{
		src = CRIT_randstring(msize);
		dst = ft_strmapi(src, CRIT_f3);
		free (dst);
	}
#endif
}

#endif
