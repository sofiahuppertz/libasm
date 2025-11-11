NAME        := libasm.a
SRCS        := ft_strlen.s ft_strcpy.s ft_strcmp.s ft_write.s ft_read.s
OBJS        := $(SRCS:.s=.o)

NASM        := nasm
NASMFLAGS   := -f macho64

AR          := ar
ARFLAGS     := rcs

RM          := rm -f

CC          := arch -x86_64 cc
CFLAGS      := -I.
LDFLAGS     := -L.
LDLIBS      := -lasm
TEST_BIN    := test

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.s
	$(NASM) $(NASMFLAGS) -o $@ $<

$(TEST_BIN): $(NAME) main.c
	$(CC) main.c $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME) $(TEST_BIN)

re: fclean all

