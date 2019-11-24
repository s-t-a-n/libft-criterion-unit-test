#!/bin/sh
exit 42;

git clone https://github.com/alelievr/libft-unit-test.git libft-unit-tests

# these tests are best served with -fsanitize=address
git clone https://github.com/t0mm4rx/libftdestructor libft-destructor && brew install python3
git clone https://github.com/Night-squad/libft-war-machine-v2019.git libft-warmachine

exit 43;
# valgrind for Mac OS Mojave
brew install --HEAD https://raw.githubusercontent.com/sowson/valgrind/master/valgrind.rb
