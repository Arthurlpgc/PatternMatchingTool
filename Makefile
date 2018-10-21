CC=g++
pwd := $(CURDIR)
pathvar := ${PATH}
seqtest := 1

pmt:
	$(CC) src/main.cpp -o ./bin/pmt -std=c++14 -O3
pmt2:
	$(CC) src/main.cpp -o ./bin/pmt2 -std=c++14 -O3 -DtestDefKey

test:
	make pmt && make pmt2 && make test1 && make test1so && make test1wu && make test2 && make test3 && make test4 && make test5 && rm lixo

test1:
	number=0
	time while [[ $$number -le $(seqtest) ]]; \
		do pmt  -c -p data/pattern.txt data/shakespeare.txt  ; \
		((number = number + 1)); \
	done 

test1so:
	number=0
	time while [[ $$number -le $(seqtest) ]]; \
		do pmt -a ShiftOr -c -p data/pattern.txt data/shakespeare.txt  ; \
		((number = number + 1)); \
	done 

test1wu:
	number=0
	time while [[ $$number -le $(seqtest) ]]; \
		do pmt -a WuManber -c -p data/pattern.txt data/shakespeare.txt  ; \
		((number = number + 1)); \
	done 
	
test2:
	number=0
	time while [[ $$number -le $(seqtest) ]]; \
		do pmt2  -c -p data/pattern.txt data/shakespeare.txt  ; \
		((number = number + 1)); \
	done 

test3:
	number=0
	time while [[ $$number -le $(seqtest) ]]; \
		do grep -c -p data/pattern.txt data/shakespeare.txt  ; \
		((number = number + 1)); \
	done 

test4:
	number=0
	time while [[ $$number -le $(seqtest) ]]; \
		do ggrep -c --file=data/pattern.txt data/shakespeare.txt  ; \
		((number = number + 1)); \
	done 

test5:
	number=0
	time while [[ $$number -le $(seqtest) ]]; \
		do pmt -c -a AhoCorasik -p data/pattern.txt data/shakespeare.txt  ; \
		((number = number + 1)); \
	done


path: 
	export PATH=$(pathvar):$(pwd)/bin/
