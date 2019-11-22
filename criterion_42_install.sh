#!/bin/sh
_brew()
{
	if [ ! -d /Users/$USER/.brew ]
	then
		HOMEBREW_SH='/tmp/homebrew_install.sh'
		echo "--- Let's set you up with brew first.."
		curl -fsSL https://rawgit.com/gcamerli/42brew/master/set.sh > $HOMEBREW_SH
		echo "------> READ THE FOLLOWING BEFORE YOU AGREE" && sleep 2
		less $HOMEBREW_SH
		echo "Agreed? : y/n"
		read yn;
		if [ "$yn" == 'y' ] || [ "$yn" == 'Y' ]
		then
			sh $HOMEBREW_SH
			return $?
		else
			echo 'OK Bye!'
			return 1
		fi
	else
		echo "--- Brew is already present!"
	fi
}

_criterion()
{
	if [ ! -d /Users/$USER/.brew/Cellar/criterion ]
	then
		echo "--- Installing criterion!"
		/Users/$USER/.brew/bin/brew install snaipe/soft/criterion
	else
		echo '--- Criterion is already present!'
	fi

	if [ "$(grep C_INCLUDE_PATH /Users/$USER/.zshrc)" == "" ] || [ "$(grep LIBRARY_PATH /Users/$USER/.zshrc)" == "" ]
	then
		echo "--- Adding ZSH environment variables C_INCLUDE_PATH and LIBRARY_PATH to .zshrc"
		echo "export C_INCLUDE_PATH=\"/Users/$USER/.brew/include:\$C_INCLUDE_PATH\"" >> /Users/$USER/.zshrc
		echo "export LIBRARY_PATH=\"/Users/$USER/.brew/lib:\$LIBRARY_PATH\"" >> /Users/$USER/.zshrc
	else
		echo '--- ZSH environment variables C_INCLUDE_PATH and LIBRARY_PATH are already present.'
		echo "Manually add the following to your ~/.zshrc if necessary (or set these variables when needed):"
		echo "export C_INCLUDE_PATH=\"/Users/$USER/.brew/include:\$C_INCLUDE_PATH\""
		echo "export LIBRARY_PATH=\"/Users/$USER/.brew/lib:\$LIBRARY_PATH\""
	fi
}

_input()
{
	if [ "$1" != "install" ]
	then
		echo "--- This script will install the awesomeness of the packagemanager called 'brew' and using brew it will install the 'criterion' unit-test libraries."
		echo "To install run ->"
		echo "$0 install"
	else
		_brew
		_criterion
	fi
}

_input $@
