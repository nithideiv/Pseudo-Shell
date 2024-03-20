#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"

#include <libgen.h>           /* definitions for pattern mathcing functions */

#include <dirent.h>

#include <sys/stat.h>

#include <fcntl.h>           /* Definition of AT_* constants */
#include <errno.h>

#include <unistd.h>

/*
STDIN = 0
STDOUT = 1
STDERR = 2

make sure to subtract 1 to exclude the null terminator for the error messages
*/


void listDir() /*for the ls command*/
{
    
    char *currentDir = getcwd(NULL, 0);
    if (currentDir == NULL)
    {
        char error[] ="Error getting current directory\n";
        write(2, error, strlen(error));
    
        return;
    }
    
    
    DIR *dir = opendir(currentDir);
    if (dir == NULL)
    {
        char error[] = "Error opening directory\n";
        write(2, error, strlen(error));

        free(currentDir); 
        return;
    }


    struct dirent *dirEntry;
    while ((dirEntry = readdir(dir)) != NULL)
    {
        write(1, dirEntry->d_name, strlen(dirEntry->d_name));
        write(1, "\n", 1);
    }

    closedir(dir);

    free(currentDir);
}


void showCurrentDir() /*for the pwd command*/
{
    char *buf = getcwd(NULL, 0);

    if (buf != NULL)
    {
        size_t len = strlen(buf);

        // Use the write function to print the current directory to standard output (usually 1)
        write(1, buf, len);
        write(1, "\n", 1);

        free(buf);
    }
    else
    {
        char error[] = "Error getting current working directory\n";
        write(2, error, strlen(error));
        return;
    }
}


void makeDir(char *dirName) /*for the mkdir command*/
{
    struct stat dir;

    if (stat(dirName, &dir) == -1) 
    {
        // The directory doesn't exist, so create it
        if (mkdir(dirName, 0777) == 0) 
        {
            char mesg[] = "Directory created successfully.\n";
            write(1, mesg, strlen(mesg));
        } 
        else 
        {
            char error[] = "mkdir failed\n";
            write(2, error, strlen(error));
        }
    } 
    
    else 
    {
        if (S_ISDIR(dir.st_mode)) 
        {
            char error[] = "Directory already exists.\n";
            write(2, error, strlen(error));
        } 
        
        else {
            char error[] = "A file with the same name already exists.\n";
            write(2, error, strlen(error));
        }
    }
}


void changeDir(char *dirName) /*for the cd command*/
{
     if (chdir(dirName) == 0) //change working directory
     {
        char mesg[] = "Changed directory successfully.\n";
        write(1, mesg, sizeof(mesg) - 1); 
    } 
    
    else 
    {
        char error[] = "Error changing directory.\n";
        write(2, error, sizeof(error) - 1); 
    }
}


void copyFile(char *sourcePath, char *destinationPath) /*for the cp command*/
{
    int sourceFile, destFile;
    char buffer[4096]; //adjust the buffer size as needed

    // Open the source file for reading
    sourceFile = open(sourcePath, O_RDONLY);
    if (sourceFile == -1)
    {
        char error[] = "Error opening source file\n";
        write(2, error, strlen(error));
        return;
    }

    // Check if the destination path is a directory
    int isDestDir = 0;
    if (access(destinationPath, F_OK) == 0) //checks wheather the calling process can access the file pathname
    {
        struct stat destStat;
        if (stat(destinationPath, &destStat) == 0 && S_ISDIR(destStat.st_mode))
        {
            isDestDir = 1;
        }
    }

    // If the destination is a directory, append the source file name
    if (isDestDir)
    {
        char *sourceFileName = basename(sourcePath);

        size_t destPathLen = strlen(destinationPath);
        size_t fileNameLen = strlen(sourceFileName);

        char destWithFile[destPathLen + fileNameLen + 2]; // 1 for '/', 1 for null terminator

        // Manually concatenate strings
        strcpy(destWithFile, destinationPath);
        strcat(destWithFile, "/");
        strcat(destWithFile, sourceFileName);

        // Open or create the destination file for writing
        destFile = open(destWithFile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }
    else
    {
        // Open or create the destination file for writing
        destFile = open(destinationPath, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }

    if (destFile == -1)
    {
        char error[] = "Error opening destination file\n";
        write(2, error, strlen(error));
        close(sourceFile);
        return;
    }

    // Read from source and write to destination
    ssize_t bytesRead, bytesWritten;

    while ((bytesRead = read(sourceFile, buffer, sizeof(buffer))) > 0)
    {
        bytesWritten = write(destFile, buffer, bytesRead);
        if (bytesWritten != bytesRead)
        {
            char error[] = "Error writing to destination file\n";
            write(2, error, strlen(error));

            close(sourceFile);
            close(destFile);

            return;
        }
    }

    // Close the source file
    close(sourceFile);

    // Close the destination file
    close(destFile);


    char successMsg[] = "File copied successfully.\n";
    write(1, successMsg, sizeof(successMsg) - 1);
}


void moveFile(char *sourcePath, char *destinationPath) /*for the mv command*/
{
    int sourceFile, destFile;
    char buffer[4096]; //adjust the buffer size as needed

    // Open the source file for reading
    sourceFile = open(sourcePath, O_RDONLY);
    if (sourceFile == -1)
    {
        char error[] = "Error opening source file\n";
        write(2, error, strlen(error));
        return;
    }

    // Check if the destination path is a directory
    int isDestDir = 0;
    if (access(destinationPath, F_OK) == 0) //checks wheather the calling process can access the file pathname
    {
        struct stat destStat;
        if (stat(destinationPath, &destStat) == 0 && S_ISDIR(destStat.st_mode))
        {
            isDestDir = 1;
        }
    }

    // If the destination is a directory, append the source file name
    if (isDestDir)
    {
        char *sourceFileName = basename(sourcePath);

        size_t destPathLen = strlen(destinationPath);
        size_t fileNameLen = strlen(sourceFileName);

        char destWithFile[destPathLen + fileNameLen + 2]; // 1 for '/', 1 for null terminator

        // Manually concatenate strings
        strcpy(destWithFile, destinationPath);
        strcat(destWithFile, "/");
        strcat(destWithFile, sourceFileName);

        // Open or create the destination file for writing
        destFile = open(destWithFile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }
    else
    {
        // Open or create the destination file for writing
        destFile = open(destinationPath, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }

    if (destFile == -1)
    {
        char error[] = "Error opening destination file\n";
        write(2, error, strlen(error));
        close(sourceFile);
        return;
    }

    // Read from source and write to destination
    ssize_t bytesRead, bytesWritten;

    while ((bytesRead = read(sourceFile, buffer, sizeof(buffer))) > 0)
    {
        bytesWritten = write(destFile, buffer, bytesRead);
        if (bytesWritten != bytesRead)
        {
            char error[] = "Error writing to destination file\n";
            write(2, error, strlen(error));

            close(sourceFile);
            close(destFile);

            return;
        }
    }

    // Close the source file
    close(sourceFile);

    // Close the destination file
    close(destFile);

    // Remove the source file
    if (unlink(sourcePath) == -1)
    {
        char error[] = "Error removing source file\n";
        write(1, error, strlen(error));
        return;
    }


    char success[] = "File moved successfully\n";
    write(1, success, strlen(success));
}


void deleteFile(char *filename) /*for the rm command*/
{
    if (unlink(filename) == 0) 
    {
        // File was deleted successfully.
        char mesg[] = "File deleted successfully\n";
        write(1, mesg, strlen(mesg));
  
    } 

    else 
    {
        if (errno == ENOENT) 
        {
            // File does not exist.
            char error[] = "File does not exist\n";
            write(2, error, strlen(error));
       
        } 

        else 
        {
            // Error occurred during deletion.
            char error[] = "Error deleting file\n";
            write(2, error, strlen(error));

        }
    }
}


void displayFile(char *filename) /*for the cat command*/
{
    int fd = open(filename, O_RDONLY);

    if (fd == -1) 
    {
        char error[] = "Error opening file\n";
        write(2, error, strlen(error));
        return;
    }

    char buffer[4096]; // You can adjust the buffer size as needed

    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) 
    {
        if (write(1, buffer, bytesRead) != bytesRead) 
        {
            char error[] = "Error writing to stdout\n";
            write(2, error, strlen(error));
            break;
        }
    }

    if (bytesRead == -1) 
    {
        char error[] = "Error reading file\n";
        write(2, error, strlen(error));
    }

    close(fd);
}