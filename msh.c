//Maya Akins
// Git push to save
// The MIT License (MIT)
// 
// Copyright (c) 2023 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

/*  2400 commands   (fork and exec)  
    quit exit cd    (Built in shell)
    while(1){

        read input

        if(strcmp(token[0],"cd")==0){
            chdir(token[1]);
        }
        else if(strcmp(token[0],"exit")==0){
            EXITFUNCTION(token[1]);
        }
        else if(strcmp(token[0],"quit")==0){
            QUITFUNCTION(token[1]);
        }
        else{
            fork 
            call exec on that
        }
    }
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 128    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 12     // Mav shell currently only supports one argument

int main()
{

  char * command_string = (char*) malloc( MAX_COMMAND_SIZE );

  while( 1 )
  {
    // Print out the msh prompt
    printf ("msh> ");
    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (command_string, MAX_COMMAND_SIZE, stdin) );

    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];

    for( int i = 0; i < MAX_NUM_ARGUMENTS; i++ )
    {
      token[i] = NULL;
    }

    int   token_count = 0;                                 
                                                           
    // Pointer to point to the token
    // parsed by strsep
    char *argument_ptr = NULL;                                         
                                                           
    char *working_string  = strdup( command_string );                

    // we are going to move the working_string pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *head_ptr = working_string;

    // Tokenize the input strings with whitespace used as the delimiter
    while(((argument_ptr = strsep(&working_string, WHITESPACE))!=NULL)&&(token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( argument_ptr, MAX_COMMAND_SIZE );
      if(strlen(token[token_count]) == 0)
      {
        token[token_count] = NULL;
      }
        token_count++;
    }

    // Now print the tokenized input as a debug check
    //TODO Remove this for loop and replace with your shell functionality
   /*int token_index  = 0;
    for( token_index = 0; token_index < token_count; token_index ++ ) 
    {
      printf("token[%d] = %s\n", token_index, token[token_index] );  
    }
   */ 
    if(token[0] != NULL){   // This avoids SEGFAULT 
      if(strcmp(token[0],"exit")==0){   //EXIT
        return(0);
      }
      else if(strcmp(token[0],"quit")==0){  //QUIT
        return(0);
      }
      else if(strcmp(token[0],"cd")==0){  // covers cases of just cd or cd [directory location]
        if(token[1]==NULL){
          chdir("..");
        }
        else{
          //int success = 
          chdir(token[1]);
          //if(success==-1){      //if chdir doesnt work print
            //printf("cd function not successful\n");
          //}
        }
      }
      else{
        pid_t pid = fork();

        if(pid == 0){ // if you are child
          int ret = execvp( token[0], &token[0] ); 
          if(ret == -1 ){
            printf("%s: Command not found.\n",token[0]);
            return(1);
          }
        }
        else{
          int status;
          wait(&status);
        }
      }
    }
    else{
      continue;
    }
    // Cleanup allocated memory
    for( int i = 0; i < MAX_NUM_ARGUMENTS; i++ )
    {
      if( token[i] != NULL )
      {
        free( token[i] );
      }
    }

    free( head_ptr );

  }

  free( command_string );

  return 0;
  // e1234ca2-76f3-90d6-0703ac120003
}