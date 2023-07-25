# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/02 16:33:15 by rbetz             #+#    #+#              #
#    Updated: 2023/07/25 14:41:15 by rbetz            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:=	ircserv

###			###			COMPABILITY		###			###
OS		:=	$(shell uname)

###			###			COMPILER		###			###
CXX		:=	c++
CXXFLAGS	:=	-Wall -Wextra -Werror
CXXFLAGS	+=	-MMD --std=c++98
CXXFLAGS	+=	-Wunused -Wunreachable-code -Wshadow -pedantic -Wno-conversion
#CXXFLAGS	+=	-g #-fsanitize=address

###			###			SOURCES			###			###
VPATH	:=	src/

SRC_F	:=	ircserv.cpp
# SRC_F	+=	

###			###			OBJECTS			###			###
OBJ_D	:=	./obj
OBJ_F	:=	$(patsubst %.cpp,$(OBJ_D)/%.o,$(SRC_F))
DEP_F	:=	$(patsubst %.cpp,$(OBJ_D)/%.d,$(SRC_F))

###			###			COLORS			###			###
RED		:=	\033[1;31m
GREEN	:=	\033[1;32m
YELL	:=	\033[1;33m
BLUE	:=	\033[1;34m
WHITE	:=	\033[0m

###			###			LIBRARIES		###			###
LSAN_D	:=	./lib/LeakSanitizer
LSAN_F	:=	$(LSAN_D)/liblsan.a
LSAN_U	:=	https://www.github.com/mhahnFr/LeakSanitizer

LIB		:=	

#Linux/Mac Compability for Leaksanitizer
ifeq ($(OS), Darwin)
ifeq ($(shell test -d $(LSAN_D) && test -f $(LSAN_F) && echo exists), exists)
	INC_F	+=	-Wno-gnu-include-next -I $(LSAN_D)/include
	LIB		+=	-L $(LSAN_D) -llsan -lc++
endif
else ifeq ($(OS), Linux)
ifeq ($(shell test -d $(LSAN_D) && test -f $(LSAN_F) && echo exists), exists)
	INC_F	+=	-Wno-gnu-include-next -I $(LSAN_D)/include
	LIB		+=	-rdynamic -L $(LSAN_D) -llsan -ldl -lstdc++
endif
endif

###			###			COMPABILITY		###			###

###			###			RULES			###			###
#make lsan is downloading lsan if its not.
#every make will be with lsan until a clean occurs.

all: message
	@$(MAKE) -j $(NAME)

lsan: clean_lsan $(OBJ_D) $(LSAN_F)
	@$(MAKE) -j $(NAME)

$(NAME): $(OBJ_D) $(OBJ_F)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ_F) $(LIB)

$(OBJ_D)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEP_F)

$(OBJ_D):
	@git config advice.detachedHead false
	@mkdir -p $@

$(LSAN_F):
ifneq ($(shell test -d $(LSAN_D) && echo exists), exists)
	@echo "$(GREEN)Clone LeakSanitizer ...$(WHITE)"
	@git clone -q --branch v1.4 --recursive $(LSAN_U) $(LSAN_D)
endif
	@echo "$(GREEN)Make LeakSanitizer ...$(WHITE)"
	@$(MAKE) -s -C $(LSAN_D)

message:
	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo "@                                                            @"
	@echo "@    ___   _   _   _    ___   _   _   _   _    ___   _   _   @"
	@echo "@   /   \ ( ) ( ) ( )  /   \ ( ) ( ) ( ) ( )  /   \ ( ) ( )  @"
	@echo "@  (  ___ ) $(YELL)\_/ \_/$(WHITE) /  (  ___ ) $(YELL)\_/ \_/ \_/$(WHITE) /  (  ___ ) $(YELL)\_/$(WHITE)  @"
	@echo "@   )|   |)   _   (    )|   |)   _   _   (    )|   |)   _    @"
	@echo "@  ( )___(  /   \  )  ( )___(  /   ) (   )  ( )___(  /   )   @"
	@echo "@   )|   | (  _  ) (   )|   | (   ( )   ) (   )|   | (   (   @"
	@echo "@  ( )___(  )/ \(   ( )___(  )   (   ) (   ( )___(  )   )    @"
	@echo "@$(BLUE)     ___  $(WHITE)  $(YELL)\_/$(WHITE) $(GREEN)    ___  $(WHITE)  \ ) ( ) ( ) (   $(RED)  ___   $(WHITE) $(YELL)\ /$(WHITE)     @"
	@echo "@$(BLUE)    / _ \       $(GREEN)   / _ \  $(WHITE)  $(YELL)\ / \ / \ /$(WHITE)    $(RED) / _ \  $(WHITE)  $(YELL)V$(WHITE)      @"
	@echo "@$(BLUE)   / /_\ \      $(GREEN)  / /_\ \\ $(WHITE)   $(YELL)V   V   V$(WHITE)     $(RED)/ /_\ \\ $(WHITE)         @"
	@echo "@$(BLUE)   |  _  |      $(GREEN)  |  _  | $(WHITE)  ______ ______  $(RED)|  _  | $(WHITE)         @"
	@echo "@$(BLUE)   | | | |      $(GREEN)  | | | | $(WHITE) |  __  |  __  | $(RED)| | | | $(WHITE)         @"
	@echo "@$(BLUE)   | | | |      $(GREEN)  | | | | $(WHITE) | |  | | |  | | $(RED)| | | | $(WHITE)         @"
	@echo "@$(BLUE)   | |/  |      $(GREEN)  | |/  | $(WHITE) | |  | | |  | | $(RED)| |/  | $(WHITE)         @"
	@echo "@$(BLUE)   \_____/      $(GREEN)  \_____/ $(WHITE) |_|  |_| |_|  |_$(RED)| \___/ $(WHITE)         @"
	@echo "@                                                            @"
	@echo "@                                                            @"
	@echo "@                 $(YELL)by$(WHITE): $(BLUE)fkernbac$(WHITE), $(GREEN)rbetz$(WHITE), $(RED)lsordo$(WHITE)                @"
	@echo "@                                                            @"
	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"

clean_lsan:
	@rm -rf $(OBJ_D)

clean:
	@rm -rf $(OBJ_D)
	@rm -f $(LSAN_F)
	@echo "$(RED)Objects cleaned.$(WHITE)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(BLUE)--->$(GREEN)Cleaning $(NAME) .....$(WHITE)"
	@echo "$(RED)All is cleaned$(WHITE)"

re: fclean all

bonus: all

.PHONY: all fclean clean re lsan bonus clean_lsan
