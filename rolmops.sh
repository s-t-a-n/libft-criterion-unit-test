#!/bin/zsh
# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    rolmops.sh                                         :+:    :+:             #
#                                                      +:+                     #
#    By: sverschu </var/mail/sverschu>                +#+                      #
#                                                    +#+                       #
#    Created: 2019/11/23 06:56:05 by sverschu      #+#    #+#                  #
#    Updated: 2019/11/23 09:14:16 by sverschu      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# ONLY EDIT THINGS IN ./config OR THE GODS WILL BE DISPLEASED
source ./config

callee=$0
banner=false

# TRAP
function finish {
	[ "$banner" = "true" ] && logp endsection
	exit
}
trap finish EXIT
trap exit 1 SIGINT SIGTERM

# LOGGING FUNCTION
logp()
{
	case "$1" in
		info)
			zsh -c "echo -e \"\e[32m\e[1m* \e[0m$2\""
			;;
		info_nnl)
			zsh -c "echo -n -e \"\e[32m\e[1m* \e[0m$2\""
			;;
		warning)
			zsh -c "echo -e \"\033[31m\e[1m* \e[0m$2\""
			;;
		fatal)
			zsh -c "echo -e \"\e[31m\e[1m* \e[0m\e[30m\e[101m$2\e[0m\""
			exit 1
			;;
		beginsection)
			banner=true
			zsh -c "echo -e \"\e[1m\e[33m*********************************************************************************************\""
			zsh -c "echo -e \"\e[1m\e[33m|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\e[0m\""
			;;
		endsection)
			zsh -c "echo -e \"\e[1m\e[33m|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\""
			zsh -c "echo -e \"\e[1m\e[33m*********************************************************************************************\e[0m\""
			;;
	esac
}

# INSTALLERS
_brew()
{
	if [ ! -d /Users/$USER/.brew ]
	then
		HOMEBREW_SH='/tmp/homebrew_install.sh'
		logp info "Let's set you up with brew first.."
		curl -fsSL https://rawgit.com/gcamerli/42brew/master/set.sh > $HOMEBREW_SH
		logp warning "------> READ THE FOLLOWING BEFORE YOU AGREE" && sleep 2
		less $HOMEBREW_SH
		logp info "Agreed? : y/n"
		read yn;
		if [ "$yn" == 'y' ] || [ "$yn" == 'Y' ]
		then
			sh $HOMEBREW_SH || return 1
			source /Users/$USER/.zshrc
		else
			logp info 'OK Bye!'
			return 1
		fi
	else
		logp info "Brew is already present!"
	fi
}

_criterion()
{
	if [ ! -d /Users/$USER/.brew/Cellar/criterion ]
	then
		logp info "Installing criterion!"
		/Users/$USER/.brew/bin/brew install snaipe/soft/criterion || return 1
	else
		logp info 'Criterion is already present!'
	fi

	if [ "$(grep C_INCLUDE_PATH /Users/$USER/.zshrc | grep ".brew/include")" == "" ] || [ "$(grep LIBRARY_PATH /Users/$USER/.zshrc | grep ".brew/lib")" == "" ]
	then
		logp info "Adding ZSH environment variables C_INCLUDE_PATH and LIBRARY_PATH to .zshrc"
		echo "" >> /Users/$USER/.zshrc
		echo "export C_INCLUDE_PATH=\"/Users/$USER/.brew/include:\$C_INCLUDE_PATH\"" | tee -a /Users/$USER/.zshrc
		echo "export LIBRARY_PATH=\"/Users/$USER/.brew/lib:\$LIBRARY_PATH\"" | tee -a /Users/$USER/.zshrc
		export C_INCLUDE_PATH="/Users/$USER/.brew/include:\$C_INCLUDE_PATH"
		export LIBRARY_PATH="/Users/$USER/.brew/lib:\$LIBRARY_PATH"
		source /Users/$USER/.zshrc
	else
		logp info 'ZSH environment variables C_INCLUDE_PATH and LIBRARY_PATH are already present and set to include .brew libs & includes.'
	fi
}

# LOGIC
_install()
{
	logp info "Starting installation.." && _brew && _criterion && logp info "Finished installation." || logp fatal "Installation failed :("
}

_compile()
{
	logp info "Starting compilation.."
	if [ "$EXT_FLAGS" != "" ]
	then
		logp info "Flags used: \"$EXT_FLAGS\""
	else
		logp info "Flags used: DEFAULTS"
	fi

	export EXT_FLAGS="${EXT_FLAGS}"
	make re
	result=$?

	if [ $result -gt 0 ] && [ "$(echo $EXT_FLAGS | grep sanitize)" != "" ]
	then
		logp warning "--- Don't forget to ADD -fsanitize=address to your flags (like -Wall -Werror -Wextra) in your libft makefile before compiling!"
		exit 1
	elif [ $result -gt 0 ] 
	then
		logp fatal "--- Something went terribly wrong whilst compiling :("
		logp warning "--- Don't forget to REMOVE -fsanitize=address from your flags (like -Wall -Werror -Wextra) in your libft makefile before compiling! (since you are no running with segfault flag (-s))"
		exit 1;
	fi
	logp info "Finished compilation."
}

_run()
{
	logp info "Starting execution.."
	$EXEC --verbose
	logp info "Finished execution."
	exit $?
}

_smokebreak()
{
	logp info "Go for a smokebreak while randomized tests will try to fuck with your code"
	logp info "hit ctrl+c to kill the test"
	logp info_nnl "Smoking in.." && printf "3 " && sleep 1 && printf "2 " && sleep 1 && printf "1\n" && sleep 1 && logp info_nnl "0 : "
	i=0; while (zsh -c "$EXEC"); do i=$((i+1)); logp info_nnl "$i : "; if [ $i -gt $SMOKEBREAK_MAX ]; then break; fi; sleep 0.1; done
	if [ $? -eq 0 ]; then logp info "Smokebreak was succesfull :)"; else logp warning "Have another smoke :("; fi
}

_usage()
{
	logp info "Usage:"
cat<<EOF
Options:
        -m -> size used in call to malloc [0..MAX] -> default : 1000
        -i -> count of iterations in all test-loops [0..MAX]-> default : 5000
        -p -> parts that are active during test [1/2/12] -> default : 12
        -l -> load default test level [1/2/3] 
              -l 1 : small memsize, low iteration count, no randomized tests
              -l 2 : medium memsize, medium iteration count, randomized tests
              -l 3 : fat memsize, fat iteration count, randomized tests, strict checks
               (this will compile and run immediately, all other flags are ignored)
        -s -> for segfault hunting: run code with AddressSanitizer (-fsanitize=address)
               open your libft Makefile add -fsanitize=address to your compile flags
               like this: CC_FLAGS = -Werror -Wextra -Wall -fsanitize=address
        -r -> use randomized tests [1/0]
              -------------------------------------------------------------------------------
Run commands:
        compile     / c	 -> compile your libft and compile tests with your libft.a
        run         / r  -> run tests
        compile-run / cr -> compile and run tests
        smokebreak  / s  -> run tests over and over again in order to find weird edge
                             behaviour with randomized tests
              -------------------------------------------------------------------------------
Examples:
        $callee	install -> will install brew and criterion non-destructively
        $callee	-m 2000 -i 10000 -s -p 1 ->	will run with 2000 as malloc size,
                                      			for 10000 iterations,
                                       			with fsanitize=address flag,
                                       			and will only check part 1
EOF
	exit 1
}

_handle_input()
{
	if [ $# -eq 0 ]; then _usage; fi

	# read run options
	for ARG in $@
	do
		if [ "${ARG}" = "install" ]; then ACTION="install"; break; fi
		if [ "${ARG}" = "compile" ] || [ "${ARG}" = "c" ]; then ACTION="compile"; break; fi
		if [ "${ARG}" = "run" ] || [ "${ARG}" = "r" ]; then ACTION="run"; fi
		if [ "${ARG}" = "compile-run" ] || [ "${ARG}" = "cr" ]; then ACTION="compile-run"; fi
		if [ "${ARG}" = "smokebreak" ] || [ "${ARG}" = "s" ]; then ACTION="smokebreak"; fi
	done

	# read options which detail an amount
		while getopts "m:i:p:l:r:sh" ARG; do
			case "${ARG}" in
				h)
					_usage
					;;
				m)
					((${OPTARG} > 0)) || _usage
					EXT_FLAGS="$EXT_FLAGS -D MEMSIZE=${OPTARG}"
					;;
				i)
					((${OPTARG} > 0)) || _usage
					EXT_FLAGS="$EXT_FLAGS -D ITERATIONS=${OPTARG}"
					;;
				p)
					((${OPTARG} == 1)) && EXT_FLAGS="$EXT_FLAGS -D PART1=1 -D PART2=0"
					((${OPTARG} == 2)) && EXT_FLAGS="$EXT_FLAGS -D PART1=0 -D PART2=1"
					((${OPTARG} == 12)) && EXT_FLAGS="$EXT_FLAGS -D PART1=1 -D PART2=1"
					((${OPTARG} == 1)) || ((${OPTARG} == 2)) || ((${OPTARG} == 12)) || _usage
					;;
				l)
					((${OPTARG} == 1)) && EXT_FLAGS="-D RANDOMIZED_TESTS=0 -D MEMSIZE=200 -D ITERATIONS=100"
					((${OPTARG} == 2)) && EXT_FLAGS="-D MEMSIZE=500 -D ITERATIONS=1000 -D RANDOMIZED_TESTS=1"
					((${OPTARG} == 3)) && EXT_FLAGS="-D STRLCAT_STRICT_SIMILARITY=1 -D MEMSIZE=5000 -D ITERATIONS=10000 -D RANDOMIZED_TESTS=1"
					((${OPTARG} == 1)) || ((${OPTARG} == 2)) || ((${OPTARG} == 3)) || _usage

					# run tests immediately
					_compile
					_run
					;;
				s)
					EXT_FLAGS="$EXT_FLAGS -fsanitize=address -D FSANITIZE_ADDRESS=1"
					logp warning "--- Don't forget to ADD -fsanitize=address to your flags (like -Wall -Werror -Wextra) in your libft makefile before compiling!"
					;;
				r)
					((${OPTARG} == 0)) && EXT_FLAGS="$EXT_FLAGS -D RANDOMIZED_TESTS=0"
					((${OPTARG} == 1)) && EXT_FLAGS="$EXT_FLAGS -D RANDOMIZED_TESTS=1"
					((${OPTARG} == 0)) || ((${OPTARG} == 1)) || _usage

					;;
				*)
					_usage
					;;
			esac
		done
	shift $((OPTIND-1))

	# decide action to take
	case "${ACTION}" in
		install)
			_install
			;;
		compile)
			_compile
			;;
		run)
			_run
			;;
		compile-run)
			_compile && _run
			;;
		smokebreak)
			_compile && _smokebreak
			;;
		*)
			logp fatal "No run command specified! (run $0 -h for usage)"
			;;
	esac

}

_env()
{
	if [ -n "$ZSH_VERSION" ]; then
		source /Users/$USER/.zshrc || logp fatal "Couldn't load /Users/$USER/.zshrc"
	elif [ -n "$BASH_VERSION" ]; then
		logp fatal "Still running bash ? Try something from the 21st century -> zsh"
	else
		logp fatal "What on earth are you running for shell ?"
	fi
}

_banner()
{
clear
zsh -c "echo -e \"\e[31m\e[1m\"
cat<<EOF
      ___           ___           ___       ___           ___           ___           ___      
     /\  \         /\  \         /\__\     /\__\         /\  \         /\  \         /\  \     
    /::\  \       /::\  \       /:/  /    /::|  |       /::\  \       /::\  \       /::\  \    
   /:/\:\  \     /:/\:\  \     /:/  /    /:|:|  |      /:/\:\  \     /:/\:\  \     /:/\ \  \   
  /::\~\:\  \   /:/  \:\  \   /:/  /    /:/|:|__|__   /:/  \:\  \   /::\~\:\  \   _\:\~\ \  \  
 /:/\:\ \:\__\ /:/__/ \:\__\ /:/__/    /:/ |::::\__\ /:/__/ \:\__\ /:/\:\ \:\__\ /\ \:\ \ \__\ 
 \/_|::\/:/  / \:\  \ /:/  / \:\  \    \/__/~~/:/  / \:\  \ /:/  / \/__\:\/:/  / \:\ \:\ \/__/ 
    |:|::/  /   \:\  /:/  /   \:\  \         /:/  /   \:\  /:/  /       \::/  /   \:\ \:\__\   
    |:|\/__/     \:\/:/  /     \:\  \       /:/  /     \:\/:/  /         \/__/     \:\/:/  /   
    |:|  |        \::/  /       \:\__\     /:/  /       \::/  /                     \::/  /    
     \|__|         \/__/         \/__/     \/__/         \/__/                       \/__/     
                                         A LIBFT UNIT TEST - MAY YOUR SEGFAULTS REST IN PEACE
EOF"
logp beginsection
}

#_env
# banners are cool: if you remove the following line the Intergalactic Police will be at your door
_banner
_handle_input $@
