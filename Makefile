NAME		= ircserv

CC			= gcc
CFLAGS		=  #-Wall -Werror -Wextra -g
CXX			= c++
CXXFLAGS		=  #-Wall -Werror -Wextra -g

SDIR		= src/*
SRCS		= main.cpp \
				Client.cpp \
				SimpleSocket.cpp \
				ServerSocket.cpp \
				GateKeeper.cpp \
				Responder.cpp \
				RequestParser.cpp \
				Input.cpp \
				ErrorHandler.cpp \
				Commander.cpp \
				CommanderTOPIC.cpp \
				CommanderMODE.cpp \
				CommanderINVITE.cpp \
				CommanderNICK.cpp \
				CommanderPASS.cpp \
				CommanderPRIVMSG.cpp \
				CommanderUSER.cpp \
				CommandKICK.cpp \
				CommandJOIN.cpp \
				CommanderPART.cpp \
				CommanderQUIT.cpp \
				Channel.cpp \

INCLUDES	= -I./include/
# LIBS		= -lc++
ODIR		= obj
OBJS		= $(addprefix $(ODIR)/, $(SRCS:.cpp=.o))

# COLORS
COM_COLOR   = \033[0;34m
OBJ_COLOR   = \033[0;36m
OK_COLOR    = \033[0;32m
ERROR_COLOR = \033[0;31m
WARN_COLOR  = \033[0;33m
NO_COLOR    = \033[m
UP = "\033[A"
CUT = "\033[K"

# **************************************************************************** #
#	RULES																	   #
# **************************************************************************** #

.PHONY: all
all: $(NAME)

header:
	@printf "$(COM_COLOR)==================== $(OBJ_COLOR)$(NAME)$(COM_COLOR) ====================$(NO_COLOR)\n"

# Linking
$(NAME): $(ODIR) $(OBJS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) $(LIBS) -o $(NAME)
	@printf $(UP)$(CUT)
	@printf "%-52b %b" "$(OK_COLOR)$(NAME) compiled successfully!" "$(G)[✓]$(X)$(NO_COLOR)\n"

# Compiling
$(ODIR)/%.o: $(SDIR)/%.cpp
	@$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@printf $(UP)$(CUT)
	@printf "%-59b %b" "$(COM_COLOR)compiling: $(OBJ_COLOR)$@" "$(OK_COLOR)[✓]$(NO_COLOR)\n"


$(ODIR):
	@mkdir -p $(ODIR)

.PHONY: clean
clean: header
	@$(RM) -r $(ODIR)
	@printf "%-51b %b" "$(ERROR_COLOR)$(NAME) cleaned!" "$(OK_COLOR)[✓]$(NO_COLOR)\n"

.PHONY: fclean
fclean: header clean
	@$(RM) $(NAME)
	@$(RM) -r src/*.dSYM

.PHONY: re
re: fclean all

# norm:
# 	@norminette -R CheckForbiddenSourceHeader include/*.h | grep --color=always 'Error!\|Error:' || echo "$(OK_COLOR)Everything is OK!$(NO_COLOR)" >&1
# 	@norminette -R CheckForbiddenSourceHeader src/*/*.c | grep --color=always 'Error!\|Error:' || echo "$(OK_COLOR)Everything is OK!$(NO_COLOR)" >&1
