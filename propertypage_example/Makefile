
# gcc -Wall -g -shared -fPIC -o propertypage_example.so menu_example.c `pkg-config --cflags --libs libcaja-extension` 

PROGRAM = propertypage_example
TARGET = ${PROGRAM}.so
SOURCE = ${PROGRAM}.c

CFLAGS=-Wall -g -shared -fPIC `pkg-config libcaja-extension --cflags --libs`

propertypage_example: 
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) 

clean:
	rm -f $(TARGET)
