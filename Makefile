ssl:
	gcc -I/usr/local/Cellar/openssl/1.0.2j/include/ -lssl -lcrypto -o sslconnect sslconnect.c

.PHONY dtmf: dtmf.c
	gcc -o dtmf -I/usr/local/freeswitch/include/freeswitch -L/usr/local/freeswitch/lib -lfreeswitch dtmf.c
