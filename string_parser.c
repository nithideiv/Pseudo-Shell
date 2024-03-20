/*
 * string_parser.c
 *
 *
 * 
 * Lab: Nithi Deivanayagam
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_parser.h"



int count_token(char* buf, const char* delim) 
{
    //TODO：
	/*
	*	#1.	Check for NULL string
	*	#2.	iterate through string counting tokens
	*		Cases to watchout for
	*			a.	string start with delimeter
	*			b. 	string end with delimeter
	*			c.	account NULL for the last token
	*	#3. return the number of token (note not number of delimeter)
	*/

    if (buf == NULL) 
    {
        return 0;
    }

    char *buf_copy = strdup(buf);
    if (buf_copy == NULL) 
    {
        return 0;  // Memory allocation failed
    }
        
    char* token;
    char* saveptr = NULL;
    int i = 0;

    token = strtok_r(buf_copy, delim, &saveptr);

    while (token != NULL) 
    {
        i++;
        token = strtok_r(NULL, delim, &saveptr);
    }

    free(buf_copy);

    return i;
}

command_line str_filler(char* buf, const char* delim) 
{
    //TODO：
	/*
	*	#1.	create command_line variable to be filled and returned
	*	#2.	count the number of tokens with count_token function, set num_token. 
    *           one can use strtok_r to remove the \n at the end of the line.
	*	#3. malloc memory for token array inside command_line variable
	*			based on the number of tokens.
	*	#4.	use function strtok_r to find out the tokens 
    *   #5. malloc each index of the array with the length of tokens,
	*			fill command_list array with tokens, and fill last spot with NULL.
	*	#6. return the variable.
	*/

    command_line cmd;
    cmd.num_token = 0;
    cmd.command_list = NULL;

    if (buf == NULL || delim == NULL) 
    {
        return cmd;
    }

    cmd.num_token = count_token(buf, delim);
    //printf("Token %d\n", cmd.num_token);


    cmd.command_list = (char**)malloc((cmd.num_token + 1) * sizeof(char*));

    if (cmd.command_list == NULL) 
    {
        cmd.num_token = 0;
        return cmd;
    }

    char* token;
    char* saveptr = NULL;
    int i = 0;


    strtok_r(buf, "\n", &saveptr);

    token = strtok_r(buf, delim, &saveptr);

    while (token != NULL) 
    {
        cmd.command_list[i] = strdup(token); // Use strdup to simplify memory allocation

        if (cmd.command_list[i] == NULL) 
        {
            // Memory allocation failed, clean up before returning
            free_command_line(&cmd);
            return cmd;
        }

        i++;
        token = strtok_r(NULL, delim, &saveptr);
        //printf("Token %d %s\n", i - 1, cmd.command_list[i - 1]);
    }

    //printf("IM DONE WITH WHILE LOOP %d %s\n", i, buf);

    cmd.command_list[i] = NULL;

    return cmd;
}

void free_command_line(command_line* command) 
{
    //TODO：
	/*
	*	#1.	free the array base num_token
	*/

    if (command != NULL) 
    {
        if (command->command_list != NULL) 
        {
            for (int i = 0; i < command->num_token; i++) 
            {
                free(command->command_list[i]);
            }

            free(command->command_list);

            command->command_list = NULL; // Set the pointer to NULL after freeing
        }

        command->num_token = 0;
    }
}