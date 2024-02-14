#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 32 // Buffer size

void copyFile(const char *inputFileName, const char *outputFileName) {
    int inputFile // Descriptor of source file
    int outputFile; // Descriptor of destination file
    char buffer[BUFFER_SIZE]; // Buffer for copying

    // Open file for reading
    if ((inputFile = open(inputFileName, O_RDONLY)) == -1) {
        printf("Error! File for reading can not be opened\n");
        exit(EXIT_FAILURE);
    }

    // Get file stats
    struct stat stat_;
    if (fstat(inputFile, &stat_) == -1) {
        perror("Error! Cannot get the input file characteristics\n");
      close(inputFile);
      exit(EXIT_FAILURE);
    }
  
    // Open file for writing (if file does not exist, create file with writing rules of the source file)
    outputFile = open(outputFileName, O_WRONLY | O_CREAT, stat_.st_mode);
    if (outputFile == -1) {
        printf("Error! File for writing cannot be opened\n");
        close(inputFile);
        exit(EXIT_FAILURE);
    }

    ssize_t bytesRead;

    // Read and write bytes
    // Continue, while it is possible to read and write
    while ((bytesRead = read(inputFile, buffer, BUFFER_SIZE)) > 0) {
        if (write(outputFile, buffer, bytesRead) != bytesRead) {
            printf("Cannot write all the bytes from the buffer to file\n");
            close(inputFile);
            close(outputFile);
            exit(EXIT_FAILURE);
        }
    }

    // Close files
    close(inputFile);
    close(outputFile);
}

int main(int argc, char *argv[]) {
    // Check arguments
    if (argc != 3) {
        printf("Please provide arguments: 'source file' 'destination file'\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Copy files
    copyFile(argv[1], argv[2]);

    printf("File was copied successfully.\n");

    return 0;
}
