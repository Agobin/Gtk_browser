BrowserMain: BrowserMain.o browser.o 
	gcc -o BrowserMain BrowserMain.o browser.o `pkg-config --cflags --libs gtk+-2.0 webkit-1.0`

BrowserMain.o: BrowserMain.c browser.h
	gcc -c BrowserMain.c `pkg-config --cflags --libs gtk+-2.0 webkit-1.0`

browser.o:	browser.c browser.h
	gcc -c browser.c `pkg-config --cflags --libs gtk+-2.0 webkit-1.0`
