all: logger acmonitor test_aclog

logger: logger.c
	gcc -Wall -fPIC -shared -o logger.so logger.c -lcrypto -ldl 

acmonitor: acmonitor.c 
	gcc acmonitor.c -o acmonitor

test_aclog: test_aclog.c 
	gcc test_aclog.c -o test_aclog

run: logger.so test_aclog
	sudo touch test1.txt
	sudo touch test2.txt
	sudo touch test3.txt
	sudo touch test4.txt
	sudo touch test5.txt
	LD_PRELOAD=./logger.so ./test_aclog

clean:
	rm -rf file_* 
	rm -rf test1.txt
	rm -rf test2.txt
	rm -rf test3.txt
	rm -rf test4.txt
	rm -rf test5.txt
	rm -rf logger.so
	rm -rf test_aclog
	rm -rf acmonitor

