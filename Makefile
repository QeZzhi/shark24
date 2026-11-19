CC = gcc
CFLAGS = -Wall -Wextra -O3 -march=native -flto -Iinclude -MMD -MP

TARGET = shark24
OBJS = src/main.o src/walker.o src/search.o
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

asan: CFLAGS := -Wall -Wextra -g -O1 -fsanitize=address,undefined -Iinclude -MMD -MP
asan: clean $(TARGET)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

.PHONY: all clean asan

-include $(DEPS)
