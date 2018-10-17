CC=g++
pwd := $(CURDIR)
pathvar := ${PATH}
seqtest:= ${seq 1 1000}

pmt:
	$(CC) main.cpp -o ../bin/pmt -std=c++14 -O3
pmt2:
	$(CC) main.cpp -o ../bin/pmt2 -std=c++14 -O3 -DtestDefKey

test:
	make pmt && make pmt2 && make test1 && make test2 && make test3

test1:
	number=0
	time while [[ $$number -le 100 ]]; \
		do pmt  -c love /Users/arthurcosta/Desktop/shakespeare.txt  > lixo; \
		((number = number + 1)); \
	done 
	
test2:
	number=0
	time while [[ $$number -le 100 ]]; \
		do pmt2  -c love /Users/arthurcosta/Desktop/shakespeare.txt  > lixo; \
		((number = number + 1)); \
	done 

test3:
	number=0
	time while [[ $$number -le 100 ]]; \
		do grep -c love /Users/arthurcosta/Desktop/shakespeare.txt  > lixo; \
		((number = number + 1)); \
	done 
path: 
	export PATH=$(pathvar):$(pwd)/../bin/
