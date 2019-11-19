### Purpose:
Test your libft with some basic input.
Beware: This testbattery only check basic input and assumes your parameter input is guarded in the same way the original
functions (a la Darwin atleast) are. You can ignore _segv notifications if you think this is necessary.
### Steps:
1. Run 'criterion_42_install.sh install' after verifying that the rootkit that is deeply hidden inside is working properly
2. Edit the Makefile and set the LIBFT_FOLDER to where your libft.a resides
3. Run 'make love'
### Workings:
1. 'criterion_42_install install' will install brew and criterion non-destructively
2. 'make love' will recompile your libft and run the tests
3. After each error that is displayed by Criterion you will see a line number which corresponds to the line on which the test
   failed
### Options
0. Set the following options in the top of criterion_libft.c (yeah yeah, headers are nice and all)
1. set 'PART2_PROTECT_CHECK' to 1 in 'criterion_libft.c' if you want to check whether your Part 2(substr, split, etc.) are overprotected. 
2. set PART1 or PART2 to 1 or 0 depending on whether you want to test it
3. set ITOA_ATOI_CHECK_ENTIRE_RANGE to 1 if you want to go full OCd on itoa and atoi and check all between INT_MIN and INT_MAX
4. set RANDOMIZED_TESTS to 0 to disable randomized testing if your console is flooded by it's results
5. set SPLIT_EXPECT_ARRAY to 0 if you return NULL in your ft_split when input string only contains delimiters or string doesnt exist
### TO DO
1. add ft_put.. functions
2. add vastly bigger strings to check proper datatype usage
3. re-read code and add clearly necessary functions
