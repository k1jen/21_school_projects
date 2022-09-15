GCC = gcc -Wall -Werror -Wextra -g
SOURCE = s21_decimal.c
OBJ = $(SOURCE:.c=.o)
GCOVFLAGS =-L. --coverage
HTML = lcov -t "test" -o rep.info -c -d .
OS = $(shell uname)

ifeq ($(OS), Darwin)
	CHECK_H=-lcheck
else
	CHECK_H=-lcheck -pthread -lrt -lm
endif

all: gcov_report

clean:
	rm -rf *.o *.a *.so *.gcda *.gcno *.gch rep.info *.html *.css test report *.txt *.dSYM

test: s21_decimal.a
	$(GCC) decimal_test.c ls21_decimal.a -o test $(CHECK_H) $(GCOVFLAGS)
	./test

s21_decimal.a: clean $(OBJ)
	ar rcs s21_decimal.a $(OBJ)
	cp s21_decimal.a ls21_decimal.a
	cp s21_decimal.a libs21_decimal.a
	ranlib ls21_decimal.a
	ranlib libs21_decimal.a

gcov_report:
	$(GCC) decimal_test.c $(SOURCE) -o test $(CHECK_H) $(GCOVFLAGS)
	./test
	$(HTML)
	genhtml -o report rep.info
	open report/index.html

check:
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --check-config $(SOURCE) *.h
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../materials/linters/cpplint.py --extensions=c $(SOURCE) *.h
	python3 ../materials/linters/cpplint.py --extensions=c decimal_test.c
	rm -rf CPPLINT.cfg
	make test
ifeq ($(OS), Darwin)
	leaks --atExit -- test
else
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./test
endif

install_brew:
	cd ~
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	brew install lcov

install_valgrind:
	brew install --HEAD LouisBrunner/valgrind/valgrind

install_cppcheck:
	brew install cppcheck