CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -Wno-unused-result
TARGET=proj1
SRCS=*.c

all:
	rm -f $(TARGET)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)
	./$(TARGET)

clean:
	rm -f $(TARGET)

run:
	./$(TARGET)

valgrind:
	valgrind --leak-check=full ./$(TARGET)

.PHONY: all clean run valgrind

test:
	./$(TARGET) < test.in > test.out
	diff test.out test.exp
	rm test.out