#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 32
#define INITIAL_SIZE 32

char * replaceWord(char * replacement) // returns a copy of string 'replacement'
{
    int l = strlen(replacement);
    char * newWord;
    newWord = (char *) malloc(l); // allocate 'l' amount of chars
    strcpy(newWord,replacement);
    // newWord[l] = '\0' // is this necessary?
    return newWord;
}

char * findWord(char * fileName, char * word) // find the line that starts with 'word' in dictionary 'filename'
{
    FILE *f;
    int i = 0, l = strlen(word), wordFound = 0, mismatch = 0;
    char * line = (char *) malloc(1024);

    f = fopen(fileName,"r");

    if (f == NULL) // check that file exists
    {
        perror("Error while opening file.");
        exit(EXIT_FAILURE);
    }

    while (( fgets (line, 1024, f)!=NULL )) // read a line from file, maximum 1024 characters?
    {
            mismatch = 0;
            for (i = 0 ; i < l ; i ++) { // compare 'word' to the start of 'line', character by character
                if (word[i] != line[i]) {
                    mismatch = 1; // if a mismatch is found, move to next line
                    break;
                }
            }
            // if character ':' is reached, it means we have found 'word' in dictionary
            if ((line[l]==':') && (mismatch == 0))
            {
                wordFound = 1;
                break;
            }
    }
    if (wordFound) { // if the 'word' is found
        line += l + 1; // slide pointer to the position after the ':' in dictionary line
        line[strlen(line)-1]='\0'; // replace the '\n' character at the end of the line with '\0'
    }
    // if the while loop finished reading all lines, but 'word' was not found
    // then surround 'word' with <> and return it.
    else
    {
        char * result = (char *) malloc(2);
        result[0] = '<';
        result[1] = '\0';
        const char * newword = replaceWord(word); // TO DO: Clear newword after use, so it is not lodged in memory forever
        strcat(result,newword);
        strcat(result,">");
        return result;
    }

    fclose(f); // TO DO: should we close the file after searching each word?
    return line;
}

int checkChar(char c, char * delims) // checks if char 'c' is a separater
{
    if (strchr(delims,c)!=NULL) // strchr returns NULL if 'c' is not found in 'delims'
    {
        return 1; // char is a letter or number
    }
    else
    {
        return 0; // char is a seperator
    }
}



int main() {
    char *currentWord = (char *) malloc(INITIAL_SIZE);
    char * delims1 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"; // non-separators
    char *newBuffer;
    char ch;
    int charCounter = 0, currentBufferSize = INITIAL_SIZE;

    // TO DO: replace '~' with EOF (CLION does not see EOF)
    while ((ch = getc(stdin)) != '~') // read characters one by one, stop at '~'
    {
        if (!checkChar(ch,delims1)) // check if 'ch' is a separator
        {
            currentWord[charCounter] = '\0'; // finalize 'currentWord' once a separator is read
            charCounter = 0;

            if (strlen(currentWord)>0) //if 'currentWord' is not empty
            {
                printf(findWord("demo.wb",currentWord)); // find the corresponding word in dictionary and print it
                printf("%c",ch);
                currentBufferSize = INITIAL_SIZE;
                currentWord = realloc(currentWord,INITIAL_SIZE);
            }
            else // if 'currentWord' is empty, then last 'ch' was a separator
            {
                printf("%c",ch);
            }
        }
        // if 'ch' is not a separator, append it to 'currentWord'
        else
        {
            currentWord[charCounter] = ch;
            charCounter++;

            if (charCounter >= currentBufferSize) { // if 'currentWord' needs more memory, reallocate it
                // use 'newBuffer' instead of 'currentWord' for realloc
                // this way if realloc() fails, the value of 'currentWord' is not lost
                newBuffer = realloc(currentWord, currentBufferSize + BUFFER_SIZE);
                currentBufferSize += BUFFER_SIZE;
                if (newBuffer == NULL) {
                    perror("reallocation for 'currentWord' failed.");
                    //TO DO: handle realloc error
                }
                currentWord = newBuffer;
                free(newBuffer);
                //
            }
        }
    }

    // currentWord[charCounter] = '\0'; // is this needed?

    return 0;
}