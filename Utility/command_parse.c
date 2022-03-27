#include "exec.h"

/**
* @brief getting a number from a string
* @param num the number
* @param flag_register a pointer to the special flag register
* @return the number from the string
*/
int get_number(char *num, PSW *flag_register)
{
	int i = 0;
	int j = 0;
	char number[MAX_LEN] = {0};
	while ((isdigit(num[i]) || num[i] == '-' || num[i] == '+') && num[i] != ',')
		number[j++] = num[i++];

	if (j == 0 || num[i] == '.' || isalpha(num[i]))
	{
		flag_register->ERR = 1;
		return -1;
	}

	return atoi(number);
}

/**
* @brief checks if an operand is using immediate addressing
* @param line pointer to the line to check
* @param number pointer to the number addressed
* @param flag_register pointer to the flag register
* @return true if addressing method is immediate
*/
bool is_immediate(char *line, int *number, PSW *flag_register, int line_number)
{
	char *binary_line;

	/* check if it starts with a # */
	if (line[0] != '#')
		return false;
	memmove(line, line + 1, strlen(line)); /* skip over it */

	*number = get_number(line, flag_register); /* get the argument */

	if(*number > MAX_NUM || *number < MIN_NUM){
		flag_register->ERR = 1;
		throw_error("Immediate is too big!", line_number);
	}

	if (flag_register->ERR)
	{
		return false;
	}

	binary_line = encode_immediate(*number);
	free(binary_line);
	return true;
}

/**
* @brief checks if an operand is using direct addressing
* @param line pointer to the line to check
* @param head pointer to the head of the symbol list
* @return true if addressing method is direct
*/
bool is_direct(char *line, symbol_list *head)
{
	if (line)
	{
		int i;
		bool valid = true;
		/* first we check that the first character is a letter */
		valid = isalpha(line[0]) ? valid : false;

		for (i = 1; i < strlen(line) - 1; i++)
		{
			if (!isalpha(line[i]) && !isdigit(line[i]))
				valid = false;
		}

		if (valid)
			return true;
	}

	return false;
}

/**
* @brief checks if an operand is using index addressing
* @param line pointer to the line to check
* @param flag_register pointer to the special flag register
* @param line_number number of current line
* @return true if addressing method is index
*/
bool is_index(char *line, int *index, PSW *flag_register, int line_number)
{
	char *tok;
	tok = strtok(line, "[");
	if (tok != NULL)
	{
		/* if it is: */
		tok = strtok(NULL, "]");
		if (tok && tok[0] == 'r')
		{
			memmove(tok, tok + 1, strlen(tok));
			*index = get_number(tok, flag_register);
			if (flag_register->ERR == 1)
			{
				/* index is illegal */
				/* throw an error and flag */
				throw_error("Invalid Index!", line_number);
				return false;
			}
			return true;
		}
		else if (tok)
		{
			flag_register->ERR = 1;
			throw_error("Index must be a register!", line_number);
			return false;
		}
	}
	return false;
}

/**
* @brief checks if an operand is using register-direct addressing
* @param line pointer to the line to check
* @param number the number addressed
* @param flag_register pointer to the special flag register
* @return true if operand is using register-direct addressing
*/
bool is_register_direct(char *line, int *number, PSW *flag_register)
{
	if (line[0] == 'r')
	{
		memmove(line, line + 1, strlen(line));
		*number = get_number(line, flag_register);

		if (flag_register->ERR || (*number < 0 || *number > 15))
		{
			return false;
		}
		return true;
	}
	return false;
}

/**
* @brief throws an error message
* @param message the error
* @param line_number the no. of the line in which the error occured
*/
void throw_error(char *message, int line_number)
{
	printf("Line %d: %s\n", line_number, message);
}

/**
* @brief encoding an immediate operand
* @param num the immediate number
* @return the encoded operand
*/
char *encode_immediate(int num)
{
	const size_t BITS = 16;
	const size_t ARE_SIZE = 3;
	const size_t A = 1;
	char *bin_str = (char *)calloc(WORD_SIZE + 1, sizeof(char));
	unsigned int i, mask = 0;

	/* insert ARE */
	for (i = 0; i < ARE_SIZE + 1; i++)
	{
		bin_str[i] = i == A ? '1' : '0';
	}

	/* insert number */
	for (; i < WORD_SIZE; i++)
	{
		mask = 1u << (BITS - 1 - (i - (ARE_SIZE + 1)));
		bin_str[i] = (num & mask) ? '1' : '0';
	}

	bin_str[WORD_SIZE] = 0;
	return bin_str;
}

/**
* @brief encoding the opcode
* @param action_index index of the action in the action table
* @return the encoding of the opcode
*/
char *encode_command_opcode(int action_index)
{
	const size_t ARE_SIZE = 3;
	const size_t A = 1;
	char *bin_str = (char *)calloc(WORD_SIZE + 1, sizeof(char));
	unsigned int i;

	/* insert ARE */
	for (i = 0; i < ARE_SIZE + 1; i++)
	{
		bin_str[i] = i == A ? '1' : '0';
	}

	/* insert opcode */
	for (; i < WORD_SIZE; i++)
	{
		bin_str[i] = (i == WORD_SIZE - action_table[action_index].op_code - 1) ? '1' : '0';
	}

	/* insert terminator */
	bin_str[WORD_SIZE] = 0;

	return bin_str;
}

/**
* @brief encoding the label value.
* @param value the value to encode.
* @param external whether the label is external or not.
* @return the encoded line.
*/
char *encode_label_value(int value, bool external)
{
    int mask = 0;
    const size_t BITS = 16;
    const size_t ARE_SIZE = 3;
    const size_t ARE = external ? 3 : 2;
    char *bin_str = (char *)calloc(WORD_SIZE + 1, sizeof(char));
    unsigned int i;
	
    /* insert ARE */
    for (i = 0; i < ARE_SIZE + 1; i++)
    {
        bin_str[i] = i == ARE ? '1' : '0';
    }

    /* insert opcode */
    for (; i < WORD_SIZE; i++)
    {
        mask = 1u << (BITS - 1 - (i - (ARE_SIZE + 1)));
        bin_str[i] = (value & mask) ? '1' : '0';
    }

    /* insert terminator */
    bin_str[WORD_SIZE] = 0;

    return bin_str;
}

/**
* @brief encoding the label offset.
* @param offset the offset to encode.
* @param external whether the label is external or not.
* @return the encoded line.
*/
char *encode_label_offset(int offset, bool external)
{
    int mask = 0;
    const size_t BITS = 16;
	const size_t ARE_SIZE = 3;
	const size_t ARE = external ? 3 : 2;
	char *bin_str = (char *)calloc(WORD_SIZE + 1, sizeof(char));
	unsigned int i;

	/* insert ARE */
	for (i = 0; i < ARE_SIZE + 1; i++)
	{
		bin_str[i] = i == ARE ? '1' : '0';
	}

	/* insert opcode */
	for (; i < WORD_SIZE; i++)
	{
        mask = 1u << (BITS - 1 - (i - (ARE_SIZE + 1)));
        bin_str[i] = (offset & mask) ? '1' : '0';
	}

	/* insert terminator */
	bin_str[WORD_SIZE] = 0;

	return bin_str;
}

/**
* @brief encoding the registers.
* @param src the source register number.
* @param dst the destination register number.
* @param action_index index of the action in the action table
* @param src_addressing the source addressing mode.
* @param dst_addressing the destination addressing mode.
* @param two_operands true if the command has two operands.
*/
char *encode_command_registers(int src, int dst, int action_index, int src_addressing, int dst_addressing, bool two_operands)
{
	const size_t ADDRESSING = 2;
	const size_t REGISTER = 4;
	const size_t FUNCT = 4;
	const size_t ARE_SIZE = 3;
	const size_t A = 1;
	char *bin_str = (char *)malloc(WORD_SIZE + 1);
	unsigned int i, mask, j;
	
	src_addressing = src_addressing == 3 ? 1 : src_addressing == 1 ? 3 : src_addressing;
    dst_addressing = dst_addressing == 3 ? 1 : dst_addressing == 1 ? 3 : dst_addressing;
    if (two_operands) {
        src ^= dst;
        dst ^= src;
        src ^= dst;
        src_addressing ^= dst_addressing;
        dst_addressing ^= src_addressing;
        src_addressing ^= dst_addressing;
    }
	
	

	/* insert ARE */
	for (i = 0; i < ARE_SIZE + 1; i++)
	{
		bin_str[i] = i == A ? '1' : '0';
	}

	/* insert funct */
	for (j = 0; j < FUNCT; i++, j++)
	{
		mask = 1u << (FUNCT - 1 - j);
		bin_str[i] = (action_table[action_index].funct & mask) ? '1' : '0';
	}

	/* insert src */
	for (j = 0; j < REGISTER; i++, j++)
	{
		mask = 1u << (REGISTER - 1 - j);
		bin_str[i] = (src & mask) ? '1' : '0';
	}

	/* insert addressing */
	for (j = 0; j < ADDRESSING; i++, j++)
	{
		mask = 1u << (ADDRESSING - 1 - j);
		bin_str[i] = (src_addressing & mask) ? '1' : '0';
	}

	/* insert dst */
	for (j = 0; j < REGISTER; i++, j++)
	{
		mask = 1u << (REGISTER - 1 - j);
		bin_str[i] = (dst & mask) ? '1' : '0';
	}

	/* insert addressing */
	for (j = 0; j < ADDRESSING; i++, j++)
	{
		mask = 1u << (ADDRESSING - 1 - j);
		bin_str[i] = (dst_addressing & mask) ? '1' : '0';
	}

	bin_str[WORD_SIZE] = 0;
	return bin_str;
}

/**
* @brief checks whether a line is empty
* @param line pointer to the line to check
* @return true if empty
*/
bool is_empty_line(char *line)
{
	int i;
	for (i = 0; i < strlen(line); i++)
	{
		if (!isspace(line[i]))
			return false;
	}
	return true;
}
