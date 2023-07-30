CC = gcc -std=c++17
WWW = -Wall -Werror -Wextra
OS := $(shell uname)

ifeq ($(OS),Linux)
	CHFLAGS = -lpthread -lrt -lm -lsubunit -lstdc++
else
	CHFLAGS = -lm -lstdc++
endif

all: clean install

clean:
	rm -rf *.o

install: uninstall
	mkdir install_release
	mkdir build
	cd install_release && \
		cmake -DCMAKE_BUILD_TYPE=Release -D EXECUTABLE_OUTPUT_PATH="../build" ../ &&  \
		cmake --build ../install_release/. && \
		rm -rf ../install_release

uninstall:
	rm -rf install_release
	rm -rf build

dist: install
	tar -cf AlgorithmicTrading.tar.gz build/

dvi:
	open dvi.html

style:
	cp ../materials/linters/.clang-format .
	clang-format -n *.cc */*.cc */*.h */*/*.cc */*/*.h
	@rm -rf .clang-format