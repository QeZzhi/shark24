CC = gcc
CFLAGS = -Wall -Wextra -O3 -march=native -flto -Iinclude
TARGET = shark24
OBJS = src/main.o src/walker.o src/search.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)