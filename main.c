#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "string_parser.h"
#include "command.h"

void executeCommand(char** tokens, int num_tokens);

int main(int argc, char *argv[]) 
{
    if (argc > 1 && strcmp(argv[1], "-f") == 0) 
    {
        // File mode
        if (argc != 3) 
        {
            char error[] = "Usage: ";
            write(2, error, strlen(error));
            write(2, argv[0], strlen(argv[0]));
            write(2, " -f <filename>\n", 15);
            exit(EXIT_FAILURE);
        }

        FILE* file = fopen(argv[2], "r");
        if (file == NULL) 
        {
            char error[] = "Error opening file\n";
            write(2, error, strlen(error));
            exit(EXIT_FAILURE);
        }

        freopen("output.txt", "w", stdout); // Redirect stdout to "output.txt"

        // Read commands from the file
        char* line = NULL;
        size_t len = 0;
        ssize_t read;

        while ((read = getline(&line, &len, file)) != -1) 
        {   


           command_line cmd_line = str_filler(line, ";"); // parse the line into tokens

            for (int i = 0; i < cmd_line.num_token; ++i)  // execute each command separated by ";"
            {
                command_line sub_cmd_line = str_filler(cmd_line.command_list[i], " "); // further tokenize each command using space as delimiter
                
                executeCommand(sub_cmd_line.command_list, sub_cmd_line.num_token);  // execute the command
               
                free_command_line(&sub_cmd_line);
            }

            free_command_line(&cmd_line);
        }


        // Close the file and free line buffer
        fclose(file);
        if (line) 
        {
            free(line);
        }
    }

    else 
    {
        // Interactive mode
        char* line = NULL;
        size_t len = 0;

        while (1) 
        {
            write(1, ">>> ", 4);
            ssize_t read = getline(&line, &len, stdin);

            if (read == -1 || strcmp(line, "exit\n") == 0) 
            {
                break;
            }
    
            command_line cmd_line = str_filler(line, ";"); // parse the line into tokens

            for (int i = 0; i < cmd_line.num_token; ++i)  // execute each command separated by ";"
            {
                command_line sub_cmd_line = str_filler(cmd_line.command_list[i], " "); // further tokenize each command using space as delimiter
                
                executeCommand(sub_cmd_line.command_list, sub_cmd_line.num_token);  // execute the command
               
                free_command_line(&sub_cmd_line);
            }

            free_command_line(&cmd_line);
        }
      
        if (line) 
        {
            free(line);
        }
    }

    return 0;
}

void executeCommand(char** tokens, int num_tokens) 
{
    if (num_tokens < 1) 
    {
        return;
    }

    if (strcmp(tokens[0], "ls") == 0) /*for the ls command*/
    {
        listDir();
    } 

    else if (strcmp(tokens[0], "pwd") == 0) /*for the pwd command*/
    {
        showCurrentDir();
    } 

    else if (strcmp(tokens[0], "mkdir") == 0) /*for the mkdir command*/
    {
        if (num_tokens != 2) 
        {
            char error[] = "Usage: mkdir <dirName>\n";
            write(1, error, strlen(error));
        } 
        else 
        {
            makeDir(tokens[1]);
        }
    } 
    
    else if (strcmp(tokens[0], "cd") == 0) /*for the cd command*/
    {
        if (num_tokens != 2) 
        {
            char error[] = "Usage: cd <dirName>\n";
            write(1, error, strlen(error));
        } 
        else 
        {
            changeDir(tokens[1]);
        }
    } 
    
    else if (strcmp(tokens[0], "cp") == 0) /*for the cp command*/
    {
        if (num_tokens != 3) 
        {
            char error[] = "Usage: cp <sourcePath> <destinationPath>\n";
            write(1, error, strlen(error));
        } 
        else 
        {
            copyFile(tokens[1], tokens[2]);
        }
    } 

    else if (strcmp(tokens[0], "mv") == 0) /*for the mv command*/
    {
        if (num_tokens != 3) 
        {
            char error[] = "Usage: mv <sourcePath> <destinationPath>\n";
            write(1, error, strlen(error));
        } 
        else 
        {
            moveFile(tokens[1], tokens[2]);
        }
    } 
    
    else if (strcmp(tokens[0], "rm") == 0) /*for the rm command*/
    {
        if (num_tokens != 2) 
        { 
            char error[] = "Usage: rm <filename>\n";
            write(1, error, strlen(error));
        } 
        else 
        {
            deleteFile(tokens[1]);
        }
    } 
    
    else if (strcmp(tokens[0], "cat") == 0) /*for the cat command*/
    {
        if (num_tokens != 2) 
        {
            char error[] = "Usage: cat <filename>\n";
            write(1, error, strlen(error));
        } 
        else 
        {
            displayFile(tokens[1]);
            write(1, "\n", 1);
            //printf("\n");
        } 
    }

    else if (strcmp(tokens[0], "exit") == 0) /*to exit*/
    {
        exit(0);
    }

    else /* for unknown commands */
    {
        char error[] = "Error! Unsupported parameters for command: ";
        write(1, error, strlen(error));

        write(1, tokens[0], strlen(tokens[0]));
        write(1, "\n", 1);
    }
}
