CFLAGS += -I.. -DFWBOARDDEBUG

test_firewall: firewall_board.o test_firewall.o
	cc -o $@ $^

firewall_board.o: firewall_board.c firewall_board.h

test_firewall.o: test_firewall.c

.PHONY: clean

clean:
	rm *.o test_firewall
