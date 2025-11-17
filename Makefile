# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/26 23:19:13 by dzapata           #+#    #+#              #
#    Updated: 2025/11/15 16:07:53 by dzapata          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Variables ====================================================================

NAME		=	webserv

COMPILER	=	c++

FLAGS		=	-Wall -Wextra -Werror -std=c++98

EXTRA_FLAGS	=	-Wpedantic -Wshadow -Wsign-conversion -Wstrict-aliasing -Wformat=2 \
				-Wdouble-promotion -Wconversion -Wcast-align -Wnull-dereference \
				-Wfloat-equal -Wvla -Woverloaded-virtual -Wdisabled-optimization\
				-Wwrite-strings -Wpointer-arith -Winit-self -Wnon-virtual-dtor \
				#-Wduplicated-cond -Wduplicated-branches -Wunsafe-loop-optimizations \
				-Wstack-usage=8192 -Wuseless-cast

DEP_FLAGS	=	-MMD -MP

LOG_COLORS	=	

ROOT		:=	$(realpath $(dir $(lastword $(MAKEFILE_LIST))))

SRC_DIR		=	src

# Main

MAIN_CPP	=	main.cpp

MAIN_DIR	=	main

MAIN_PATH	=	$(addprefix $(MAIN_DIR)/, $(MAIN_CPP))

MAIN_SRC	=	$(addprefix $(SRC_DIR)/, $(MAIN_PATH))

# Core

CORE_CPP	=	Http.cpp Listen.cpp Location.cpp PropertiesSet.cpp Returnable.cpp \
				Server.cpp WebServ.cpp Log.cpp Types.cpp Return.cpp CGI.cpp

CORE_DIR	=	core

CORE_HPP	=	$(addprefix $(HEADERS_DIR)/, $(CORE_DIR))

CORE_PATH	=	$(addprefix $(CORE_DIR)/, $(CORE_CPP))

CORE_SRC	=	$(addprefix $(SRC_DIR)/, $(CORE_PATH))

# Parser

PARSER_CPP	=	AInstruction.cpp SingleInstruction.cpp BlockInstruction.cpp \
				ConfigParser.cpp Verify.cpp Token.cpp ParserUtils.cpp Parser.cpp

PARSER_DIR	=	parser

PARSER_HPP	=	$(addprefix $(HEADERS_DIR)/, $(PARSER_DIR))

PARSER_PATH	=	$(addprefix $(PARSER_DIR)/, $(PARSER_CPP))

PARSER_SRC	=	$(addprefix $(SRC_DIR)/, $(PARSER_PATH))

# Server

SERVER_CPP	=	server.cpp FdGuard.cpp serverUtils.cpp FatalException.cpp cgi.cpp

SERVER_DIR	=	server

SERVER_HPP	=	$(addprefix $(HEADERS_DIR)/, $(SERVER_DIR))

SERVER_PATH	=	$(addprefix $(SERVER_DIR)/, $(SERVER_CPP))

SERVER_SRC	=	$(addprefix $(SRC_DIR)/, $(SERVER_PATH))

# Methods

METHODS_CPP		=	Request.cpp Response.cpp Response_cgi.cpp Methods.cpp Utils.cpp

METHODS_DIR		=	methods

METHODS_HPP		=	$(addprefix $(HEADERS_DIR)/, $(METHODS_DIR))

METHODS_PATH	=	$(addprefix $(METHODS_DIR)/, $(METHODS_CPP))

METHODS_SRC		=	$(addprefix $(SRC_DIR)/, $(METHODS_PATH))

# Headers

HEADERS_DIR	=	includes

HEADERS		=	$(CORE_HPP) $(PARSER_HPP) $(SERVER_HPP) $(METHODS_HPP)

FLAGS		+=	$(addprefix -I,$(HEADERS))

# Build

BUILD_DIR	=	build

SRC_FILES	=	$(MAIN_PATH) $(CORE_PATH) $(PARSER_PATH) $(SERVER_PATH) $(METHODS_PATH)

BUILD_O		=	$(addprefix $(BUILD_DIR)/, $(SRC_FILES:.cpp=.o))

BUILD_DEP	=	$(BUILD_O:.o=.d)

# Templates

TEMPLATES_DIR		=	templates

TEMPLATES_EXT		=	template

TEMPLATES_BASE		=	file full test_page tester

TEMPLATES_NEW_EX	=	conf

TEMPLATES_NEW_DIR	=	configs

SED_ROOT			=	{{ROOT}}

TEMPLATES_FILES		=	$(addprefix $(TEMPLATES_DIR)/, $(TEMPLATES_BASE))

TEMPLATES_FULL		=	$(addsuffix .$(TEMPLATES_FILES), $(TEMPLATES_EXT))

TEMPLATES_CONVERT	=	$(addprefix $(TEMPLATES_NEW_DIR)/, $(addsuffix .$(TEMPLATES_NEW_EX), $(notdir $(TEMPLATES_BASE))))

# Compilation flags

COLORS ?= $(shell test -t 1 && echo 1 || echo 0)

ifeq ($(COLORS),1)
	LOG_COLORS = -DPRINT_COLORS=1
endif

# Functions ====================================================================

$(BUILD_DIR)/%.o	:	$(SRC_DIR)/%.cpp
						$(COMPILER) $(FLAGS) $(EXTRA_FLAGS) $(DEP_FLAGS) $(LOG_COLORS) -c $< -o $@

all					:	$(NAME)

$(BUILD_DIR)		:
						@mkdir -p $(BUILD_DIR)
						@mkdir -p $(BUILD_DIR)/$(MAIN_DIR)
						@mkdir -p $(BUILD_DIR)/$(CORE_DIR)
						@mkdir -p $(BUILD_DIR)/$(PARSER_DIR)
						@mkdir -p $(BUILD_DIR)/$(SERVER_DIR)
						@mkdir -p $(BUILD_DIR)/$(METHODS_DIR)

$(NAME)				:	$(BUILD_DIR) $(BUILD_O)
						@echo	"Building" $(NAME)"..."
						@$(COMPILER) $(FLAGS) $(EXTRA_FLAGS) $(DEP_FLAGS) $(LOG_COLORS) $(BUILD_O) -o $(NAME)
						@echo	$(NAME) "compiled."

-include $(BUILD_DEP)

$(TEMPLATES_NEW_DIR)/%.$(TEMPLATES_NEW_EX):	$(TEMPLATES_DIR)/%.$(TEMPLATES_EXT)
											@sed 's|$(SED_ROOT)|$(ROOT)|g' $< > $@

$(TEMPLATES_NEW_DIR):
						@mkdir -p $(TEMPLATES_NEW_DIR)

conf				:	$(TEMPLATES_NEW_DIR) $(TEMPLATES_CONVERT)

clean_conf			:
						@rm -fdr $(TEMPLATES_NEW_DIR)
						@echo	"Configs cleaned"

conf_clean			:	clean_conf

youpi_banane		:
						@echo "Y O U P I"
						@mkdir YoupiBanane
						@mkdir ./YoupiBanane/nop
						@mkdir ./YoupiBanane/Yeah
						@touch ./YoupiBanane/youpi.bad_extension
						@touch ./YoupiBanane/youpi.bla
						@touch ./YoupiBanane/nop/other.pouic
						@touch ./YoupiBanane/nop/youpi.bad_extension
						@touch ./YoupiBanane/Yeah/not_happy.bad_extension
						@chmod +x ./YoupiBanane/youpi.bla
						@echo "B A N A N E"

clean_banane		:
						@rm -fdr YoupiBanane
						@echo "N O   B A N A N E"

clean				:
						@echo "Cleaning build files."
						@rm -rf $(BUILD_DIR)

fclean				:	clean
						@echo $(NAME) "deleted."
						@rm -f $(NAME)

re					:	fclean all

.PHONY				:	all clean fclean re config clean_conf youpi_banane clean_banane