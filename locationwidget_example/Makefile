
# gcc -Wall -g -shared -fPIC -o locationwidget_example.so menu_example.c `pkg-config --cflags --libs libcaja-extension` 

PROGRAM = locationwidget_example
TARGET = ${PROGRAM}.so
SOURCE = ${PROGRAM}.c

CFLAGS=-Wall -g -shared -fPIC `pkg-config libcaja-extension --cflags --libs`

locationwidget_example: 
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) 

clean:
	rm -f $(TARGET)
