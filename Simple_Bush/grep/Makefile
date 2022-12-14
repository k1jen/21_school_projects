all: build

build: s21_grep

rebuild: clean build

s21_grep: s21_grep.c
	gcc -std=c11 -D_GNU_SOURCE -Wall -Werror -Wextra ./s21_grep.c -o ./s21_grep

lint:
	cp ../../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../../materials/linters/cpplint.py --extension=c *.c
	python3 ../../materials/linters/cpplint.py --extension=c *.h
	rm -rf *.cfg

test:
	chmod 777 tests.sh
	./tests.sh

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c

install_brew:
	cd ~
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	brew install lcov

install_valgrind:
	brew install --HEAD LouisBrunner/valgrind/valgrind

install_cppcheck:
	brew install cppcheck

leaks:
	CK_FORK=no leaks --atExit -- ./s21_grep -e on4fdase -e twogsdfgsdf s21_grep.c -f pattern.txt

valgrind:
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./s21_grep -e while -e void s21_grep.c Makefile -o -h -n -f pattern.txt

clean:
	rm ./s21_grep