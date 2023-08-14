# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/02 16:33:15 by rbetz             #+#    #+#              #
#    Updated: 2023/08/14 13:00:36 by rbetz            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:=	ircserv

###			###			COMPABILITY		###			###
OS		:=	$(shell uname)
SHELL	:=	/bin/bash

###			###			COMPILER		###			###
CXX		:=	c++
CXXFLAGS	:=	-Wall -Wextra -Werror
CXXFLAGS	+=	-MMD --std=c++98
CXXFLAGS	+=	-Wunused -Wunreachable-code -Wshadow -pedantic -Wno-conversion
# CXXFLAGS	+=	-g #-fsanitize=address

###			###			SOURCES			###			###
VPATH	:=	src/

SRC_F	:=	ircserv.cpp Server.cpp Client.cpp Channel.cpp
SRC_F	+=	OverallFunctions.cpp Commands.cpp Parsing.cpp
SRC_F	+=	SendMessages.cpp

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

KVIRC	:=	~/Desktop/KVIrc-5.0.0.dmg

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

run:
	@$(MAKE) -j $(NAME)
	./ircserv 6667 pw

lsan: clean_lsan $(OBJ_D) $(LSAN_F)
	@$(MAKE) -j $(NAME)

kvirc:
	@echo "$(GREEN)Download $(KVIRC) ...$(WHITE)"
	@/usr/bin/curl -O "ftp://ftp.kvirc.net/pub/kvirc/5.0.0/binary/macosx/KVIrc-5.0.0.dmg" --output $(KVIRC)

$(NAME): $(OBJ_D) $(OBJ_F)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ_F) $(LIB)
	@echo "$(RED)--->$(BLUE)ircserv $(GREEN)is now compiled .....$(WHITE)"

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
	@echo -e "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@$(RED)  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%* $(WHITE)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@$(RED) %                                         % $(WHITE)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@$(RED) %        #  #   ##  #######   #####       % $(WHITE)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@$(RED) %     ########  ##  #     #  ##           % $(WHITE)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@$(RED) %       #  #    ##  #  ##/   #            % $(WHITE)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@$(RED) %    ########   ##  #     #  ##           % $(GREEN)((((((((((((((((((((((((((((($(WHITE) @@"
	@echo -e "@@@$(RED) %      #  #     ##  #      #  #####       % $(GREEN)(((((((((((((((($(YELL)by$(GREEN)(((((((((((($(WHITE) @"
	@echo -e "@@@$(RED) %%%                                     %%% $(GREEN)(((((((((((((((((((((((((((((($(WHITE) @"
	@echo -e "@@@@@$(RED)  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% $(GREEN)((((($(YELL)INTERNET$(GREEN)(((((($(YELL)fkernbac$(GREEN)(((((($(WHITE) @"
	@echo -e "@@@@@@@@@@@@@@$(RED)%%%%%%%%%%%&$(WHITE)@$(BLUE)###$(WHITE)@$(BLUE)####$(WHITE)@@ $(GREEN)(((((((((((((((((((((((((((((((((((((((($(WHITE) @"
	@echo -e "@@@@@@@@@@@@@@@@$(RED)%%%%%%%%$(WHITE)@$(BLUE)######$(WHITE)@@@$(BLUE)### $(GREEN)(((((((((((($(YELL)RELAY$(GREEN)((((((((($(YELL)rbetz$(GREEN)((((((((($(WHITE) @"
	@echo -e "@@@@@@@@@@@@@@@@@@@$(RED)%%%%$(WHITE)@$(BLUE)###$(WHITE)@@@@$(BLUE)#$(WHITE)@$(BLUE)#### $(GREEN)(((((((((((((((((((((((((((((((((((((((($(WHITE) @"
	@echo -e "@@@@@@@@@@@@@@@@@@@$(RED)%%%$(WHITE)@@@@$(BLUE)#######$(WHITE)@$(BLUE)### $(GREEN)(((((((((((($(YELL)CHAT$(GREEN)(((((((((($(YELL)lsordo$(GREEN)(((((((($(WHITE) @"
	@echo -e "@@@@@@@@@@@@@@@@@@@@$(RED)%%$(WHITE)@$(BLUE)###########$(WHITE)@$(BLUE)### $(GREEN)(((((((((((((((((((((((((((((((((((((($(WHITE) @@"
	@echo -e "@@@@@@@@@@@@@@@@@@$(BLUE)####$(WHITE)@$(BLUE)############$(WHITE)@$(BLUE)#####$(WHITE)@@@@$(BLUE)##$(WHITE)@@@$(GREEN)(((((((((((($(WHITE)@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@@@@@@@@@@@@@@@@$(BLUE)####$(WHITE)@$(BLUE)############$(WHITE)@@@@@$(BLUE)#########$(WHITE)@@$(GREEN)((((((((($(WHITE)@@@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@@@@@@@@@@@@@@@@$(BLUE)#####$(WHITE)@$(BLUE)##$(WHITE)@@@@@@@@@$(BLUE)##$(WHITE)@$(BLUE)###########$(WHITE)@@$(GREEN)((((((($(WHITE)@@@@@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@@@@@@@@@@@@@@@@@@@@@@@@$(BLUE)############$(WHITE)@$(BLUE)########$(WHITE)@@$(GREEN)(((((($(WHITE)@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@@@@@@@@@@@@@@@@@@$(BLUE)#####$(WHITE)@$(BLUE)#############$(WHITE)@$(BLUE)###$(WHITE)@@@$(BLUE)#$(WHITE)@$(GREEN)(((($(WHITE)@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@@@@@@@@@@@@@@@@@@@$(BLUE)######$(WHITE)@$(BLUE)############$(WHITE)@@$(BLUE)####$(WHITE)@$(GREEN)((($(WHITE)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@@@@@@@@@@@@@@@@@@@@@$(BLUE)######$(WHITE)@$(BLUE)#####$(WHITE)@@@@$(BLUE)##$(WHITE)@$(BLUE)##$(WHITE)@$(GREEN)(($(WHITE)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@@@@@@@@@@@@@@@@@@@@@@@$(BLUE)###$(WHITE)@@@@@$(BLUE)#########$(WHITE)@@$(GREEN)($(WHITE)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@@@@@@@@@@@@@@@@@@@@@@@@@@$(BLUE)#####$(WHITE)@$(BLUE)######$(WHITE)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo -e "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"

clean_lsan:
	@rm -rf $(OBJ_D)

clean:
	@rm -rf $(OBJ_D)
	@rm -f $(LSAN_F)
	@echo "$(RED)Objects cleaned.$(WHITE)"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(KVIRC)
	@echo "$(BLUE)--->$(GREEN)Cleaning $(NAME) .....$(WHITE)"
	@echo "$(BLUE)--->$(GREEN)Cleaning $(KVIRC) .....$(WHITE)"
	@echo "$(RED)All is cleaned$(WHITE)"

re: fclean all

bonus: all

.PHONY: all fclean clean re lsan bonus clean_lsan
