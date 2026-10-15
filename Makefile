#########################
######  STRUCTURE  ######
#########################

SRC_DIR			=	srcs
ASM_SRC_DIR		=	stub_srcs
INC_DIR			=	includes

LIBFT_DIR		= 	libft

BUILD_DIR		=	build
ASM_OBJ_DIR		=	${BUILD_DIR}/asm_obj
OBJ_DIR			=	${BUILD_DIR}/obj
DEP_DIR			=	${BUILD_DIR}/dep

#######################
######  SOURCES  ######
#######################

SRCS 			=	main.c				\
					data_controller.c	\
					errors.c			\
					file_handling.c		\
					section_slasher.c	\
					segment_slasher.c	\
					stub.c				\
					
HEADERS			=	main.h 				\
					data_controller.h	\
					endian.c			\
					errors.h			\
					file_handling.h		\
					section_slasher.h	\
					segment_slasher.h	\
					stub.h

ASM_SRCS		=	main.asm

###########################
######  COMPILATION  ######
###########################

MAKE			=	make
MAKEFLAGS 		+=	--no-print-directory
RED 			=	$(shell tput setaf 1)
GREEN			=	$(shell tput setaf 2)
CYAN			=	$(shell tput setaf 6)
RESET			=	$(shell tput setaf 255)

OBJS			=	${SRCS:%.c=${OBJ_DIR}/%.o}
ASM_OBJS		=	${ASM_SRCS:%.asm=${ASM_OBJ_DIR}/%.o}

DEPS			=	${SRCS:%.c=${DEP_DIR}/%.d}
GET_DEP_PATH	=	${@:${OBJ_DIR}/%.o=${DEP_DIR}/.%d}

TEST			=	resources/sample.c
TEST_NAME		=	resources/sample

CC				=	clang
CFLAG			=	-Wall -Werror -Wextra \
					-MMD -MP -g3

ASM_COMPILER	=	nasm
ASM_FLAGS		=	-f elf64 -g
					
X32_FLAGS		=	-m32
X64_FLAGS		=	-m64

MKDIR			=	@mkdir -vp
RM				=	@rm -vrf

NAME			= 	woody
LIBFT_LIB		=	libft.a

STUB_NAME		=	stub

#####################
######  RULES  ######
#####################

all: $(LIBFT_LIB) $(NAME) compile_stub

-include $(DEPS)

$(NAME): $(OBJS)
	$(CC) $(CFLAG) $^ -o $@ $(LIBFT_DIR)/$(LIBFT_LIB)

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	$(MKDIR) $(dir $@) $(dir $(GET_DEP_PATH))
	$(CC) $(CFLAG) -I$(INC_DIR) -I$(LIBFT_DIR)/$(INC_DIR) \
		-c $< -o $@ \
		-MF $(DEP_DIR)/$(notdir $(basename $<)).d -MT $@

compile_stub: ${STUB_NAME}

$(STUB_NAME): $(ASM_OBJS)
	$(CC) -g $^ -o $@ 

${ASM_OBJ_DIR}/%.o: $(ASM_SRC_DIR)/%.asm
	$(MKDIR) $(dir $@)
	${ASM_COMPILER} $(ASM_FLAGS) $< -o $@
	
basic:
	$(CC) $(X64_FLAGS) $(TEST) -o $(TEST_NAME) 
		
init:
	git submodule init
	git submodule update
		
$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)
		
clean:
	$(RM) $(BUILD_DIR)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	rm -rf $(NAME)
	$(MAKE) clean -C $(LIBFT_DIR)
	
re: fclean all

.PHONY: init all clean fclean re
