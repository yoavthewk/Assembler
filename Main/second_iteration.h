#include "first_iteration.h"


/**
 * @brief the second iteration over the file.
 * 
 * @param file_name the file to iterate over.
 * @param fp the file pointer.
 * @param ICF the final IC.
 * @param DCF the final DC.
 * @param head the head of the symbol list.
 * @param command_head the head of the command list.
 * @param flag_register the flag register.
 */
void second_iteration(char *file_name, FILE *fp, int ICF, int DCF, symbol_list *head, command_list *command_head, PSW *flag_register);

/* Helper Functions */

/**
 * @brief returns the next IC.
 * 
 * @param IC the current IC
 * @param head the head of the command list.
 * @return int the next IC.
 */
int get_next_IC(int IC, command_list *head);

/**
 * @brief checks if the line is data declaration.
 *
 * @param line the line to check.
 * @param flag_register the flag register.
 * @return true if the line is data declaration.
 */
bool is_data(char *line, PSW *flag_register);

/**
 * @brief handles entry declaration by updating the symbol list.
 *
 * @param line the line to check.
 * @param head the head of the symbol list.
 * @param flag_register the flag register.
 * @param line_number the current line number.
 */
void handle_entry(char *line, symbol_list *head, PSW *flag_register, int line_number);

/**
 * @brief Open an object file
 * @param file_name the name of the file
 * @return FILE* of the object file
 */
FILE *create_object_file(char *file_name);

/**
 * @brief this function creates a newly allocated substring from [start] to [end]
 * @param line substring origin
 * @param start the start index
 * @param end the end index
 * @return a newly allocated substring
 */
char *get_new_substring_with_indexes(char *line, int start, int end);

/**
 * @brief this function coverts a binary string to special base
 * @param line the binary string
 * @return special base string 
 */
char *special_base(char *line);

/**
 * @brief append a string to the object file
 * @param fp object file pointer
 * @param word the word to append
 */
void append_to_object_file(FILE *fp, char *word);

/**
 * @brief processes each line in the second iteration, it is the main logic function.
 * 
 * @param fp the file pointer of the extern file.
 * @param line the line to process.
 * @param ICF the final IC.
 * @param line_number the current line number.
 * @param head the head of the symbol list.
 * @param command_head the head of the command list.
 * @param flag_register the flag register.
 * @param IC the current IC.
 */
void second_line_process(FILE *fp, char *line, int ICF, int line_number, symbol_list *head, command_list *command_head, PSW *flag_register, int* IC);

/**
 * @brief writes the entries to the entry file.
 * 
 * @param fp the entry file pointer.
 * @param head the head of the symbol list.
 */
void write_entry_to_file(FILE* fp, symbol_list *head);

/**
 * @brief writes the externals to the extern file.
 * 
 * @param fp the extern file pointer.
 * @param name the name of the extern.
 * @param address the address of the word the extern is referenced in.
 * @param second_address the address of the second word the extern is referenced in.
 */
void write_extern_to_file(FILE* fp, char* name, int address, int second_address);

/**
 * @brief checks if a command needs completion to it's encoding.
 * 
 * @param head the head of the command list.
 * @param IC the current IC.
 * @return true if it needs completion.
 */
bool need_completion(command_list *head, int IC);

/**
 * @brief completes the encoding of a certain command in the command list.
 * 
 * @param head the head of the symbol list.
 * @param command_head the head of the command list.
 * @param flag_register the flag register.
 * @param fp the extern file file pointer.
 * @param label the label to fill.
 * @param IC the current IC.
 */
void fill_command_list(symbol_list *head, command_list **command_head, PSW* flag_register, FILE* fp, char* label, int IC);

/**
 * @brief writes the final encoding to the object file.
 * 
 * @param fp the object file pointer.
 * @param IC the IC.
 * @param DC the DC.
 * @param head the command list head.
 */
void format_object_file(FILE *fp, int IC, int DC, command_list *head);