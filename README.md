### Purpose:
Test your libft (2019 curriculum) with both targeted and randomized inputs.
This unit-test uses the [Criterion](https://criterion.readthedocs.io/en/master/intro.html) C/C++ testing library. 
The aim of these tests is to find bad behaviours targeted tests (like warmachine) cannot find. A lot comes down to
compiling your code with [AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html).
Beware: This testbattery assumes your Part1 parameter input is guarded in the same way as the original functions.
### Installation:
1. Run './rolmops.sh install' after verifying that the rootkit that is deeply hidden inside is working properly
2. Edit the configfile 'config' and set the LIBFT_FOLDER to where your libft.a resides
### Workings:
1. './rolmops.sh install' will install brew and criterion non-destructively
2. './rolmops.sh -h' will show you compile- and runtime options
3. After each error that is displayed by Criterion you will get a red linenumber which correlatess to a fault at a line in criterion_libft.c.
4. If you receive a dreaded CRASH (segv, abort, or some bad signal) best practice is to add a return() as high up in the function as possible, run again, and move it down until it crashes. May the Gods be with you.
### Options
1. To immediately get going start off by changing the config file to point to your libft directory (by default this is ../libft). Then run './rolmops.sh -l 1' for starters. When you are ready for it run './rolmops.sh -s compile-run' after having added '-fsanitize=address' to the flags in your libft Makefile. 
2. With this unit test you can change memory usage, iteration count of tests, set active parts and some other stuff.
3. The most relevant options are showed by running './rolmops.sh -h'.
4. Other options can be found in the top of criterion_libft.c (yes headers are great, i know).
### Important
1. ft_put\* Are not checked
2. Bonus functions are not checked (the lst functions in the 2019 curriculum are not that difficult nor that useful).
3. Malloc failures and proper frees in case of malloc failures are not handled.
4. Prepare to look at awfully written code. I wrote these 2500 lines of tests in order to learn to test properly and to find a segfault that took me three weeks to find which turned out to be one line of code. Prettyness wasn't priority but it grew and grew and now hopefully it will help others in our common cause against Moulinette.
