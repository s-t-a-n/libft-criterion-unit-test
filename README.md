### Purpose:
Test your libft with some basic input.
Beware: This testbattery only check basic input and assumes your parameter input is guarded in the same way the original
functions (a la Darwin atleast) are. You can ignore _segv notifications if you think this is necessary (which it isn't
unless you are running your programs on embedded devices or something).
### Steps:
1. Run 'criterion_42_install.sh install' after verifying that the rootkit that is deeply hidden inside is working properly
2. Edit the Makefile and set the LIBFT_FOLDER to where your libft.a resides
3. Run 'make f'
### Workings:
1. 'criterion_42_install install' will install brew and criterion non-destructively
2. 'make love' will recompile your libft and run the tests
3. After each error that is displayed by Criterion you will see a line number which corresponds to the line on which the test
   failed
### TO DO
1. add ft_put.. functions
2. add vastly bigger strings to check proper datatype usage
3. re-read code and add clearly necessary functions
