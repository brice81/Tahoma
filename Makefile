all: libtahoma.a test
lib: libtahoma.a
test: Test

clean:
	rm -f src/JSon.o src/Operation.o src/RollerShutter.o src/Session.o src/String.o src/Test.o
	
mr-proper: clean
	rm -f libtahoma.a Test

%.o : %.cpp
	gcc -Iinclude -c -o $@ $< `wx-config --cxxflags`

libtahoma.a: src/JSon.o src/Operation.o src/RollerShutter.o src/Session.o src/String.o
	ar rcs libtahoma.a src/JSon.o src/Operation.o src/RollerShutter.o src/Session.o src/String.o

Test: lib src/Test.o
	gcc -o Test src/Test.o src/JSon.o src/Operation.o src/RollerShutter.o src/Session.o src/String.o  -lwxcode_unknownu_wxjson-3.0 `wx-config --libs` -lstdc++ -lcurl

