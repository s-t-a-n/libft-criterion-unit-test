#!/bin/zsh
echo go for a smokebreak while randomized tests will try to fuck with your code
echo hit ctrl+c to kill the test
echo smoking in && echo 3 && sleep 0.5 && echo 2 && sleep 0.5 && echo 1
clear
i=0; while (./run-test); do i=$((i+1)); printf "$i" sleep 0.1; done
