NAME        := libasm.a

# Mandatory sources
SRCS        := mandatory/ft_strlen.s mandatory/ft_strcpy.s mandatory/ft_strcmp.s \
               mandatory/ft_strdup.s mandatory/ft_write.s mandatory/ft_read.s
OBJS        := $(SRCS:.s=.o)

# Bonus sources
BONUS_SRCS  := bonus/ft_atoi_base/ft_atoi_base.s \
               bonus/ft_atoi_base/atoi_base_recursive.s \
               bonus/ft_atoi_base/ft_strchr.s \
               bonus/ft_atoi_base/ft_iswhitespace.s \
               bonus/ft_atoi_base/has_duplicates.s \
               bonus/ft_atoi_base/get_digit_value.s
BONUS_OBJS  := $(BONUS_SRCS:.s=.o)

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
TEST_ATOI   := test_atoi_base

.PHONY: all clean fclean re bonus

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

bonus: $(OBJS) $(BONUS_OBJS)
	$(AR) $(ARFLAGS) $(NAME) $^

%.o: %.s
	$(NASM) $(NASMFLAGS) -o $@ $<

$(TEST_BIN): $(NAME) main.c
	$(CC) main.c $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@

$(TEST_ATOI): bonus test_atoi_base.c
	$(CC) test_atoi_base.c $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@

clean:
	$(RM) $(OBJS) $(BONUS_OBJS)

fclean: clean
	$(RM) $(NAME) $(TEST_BIN) $(TEST_ATOI)

re: fclean all

