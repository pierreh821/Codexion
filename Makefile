.SILENT:

# Colors

GREEN   = \033[0;32m
RED     = \033[0;31m
YELLOW  = \033[0;33m
BLUE    = \033[0;34m
MAGENTA = \033[0;35m
CYAN    = \033[0;36m
RESET   = \033[0m

# Output Name
NAME = codexion

MODELS = models
INCLUDES = includes

# Compilation
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -I. -I$(INCLUDES) -I$(MODELS)
RM = rm -rf

SRCS = src/main.c \
	src/utils.c \
	src/parser.c \
	src/dev_utils.c \
	src/table_utils.c \
	src/dongle/dongle_init.c \
	src/dongle/dongle_waitlist.c \
	src/dongle/dongle_heap.c \
	src/dongle/dongle_schedule.c \
	src/monitor/monitor_init.c \
	src/monitor/monitor_utils.c \
	src/logger/logger_init.c \
	src/logger/logger_utils.c \
	src/coders/coders_init.c \
	src/coders/coders_utils.c \
	src/coders/coders_waiter.c \
	src/coders/coders_work.c \
	src/coders/coders_sync.c \

OBJ_DIR = obj
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

TOTAL := $(words $(OBJS))
CURRENT := 0

all: $(NAME)

help:
	echo "Usage: ./codexion [number_of_coders] [time_to_burnout] [time_to_compile] [time_to_debug] [time_to_compile] [number_of_compiles_required] [dongle_cooldown] [scheduler (fifo/edf)]"

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(eval CURRENT=$(shell echo $$(($(CURRENT)+1))))
	printf "$(CYAN)[%2d/%2d]$(RESET) %-40s" $(CURRENT) $(TOTAL) "$<"
	if $(CC) $(CFLAGS) -c $< -o $@ 2>/dev/null; then \
		echo " $(GREEN)✓$(RESET)"; \
	else \
		echo " $(RED)✗$(RESET)"; \
		$(CC) $(CFLAGS) -c $< -o $@; \
	fi

clean:
	printf "$(CYAN)Cleaning $(OBJ_DIR)...$(RESET) "
	if [ -d "$(OBJ_DIR)" ]; then \
		$(RM) $(OBJ_DIR) && \
		echo "$(GREEN)✓ Object files removed $(RESET)"; \
	else \
		echo "$(YELLOW)⚠ Nothing to clean $(RESET)"; \
	fi

fclean: clean
	printf "$(CYAN)Cleaning $(NAME)...$(RESET) "
	if rm $(NAME) 2>/dev/null; then \
		echo "$(GREEN)✓$(RESET)"; \
	else \
		echo "$(YELLOW)⚠ Already cleaned up$(RESET)"; \
	fi

re: fclean all

.PHONY: all clean fclean re
