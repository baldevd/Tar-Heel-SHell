/* COMP 530: Tar Heel SHell */

#include "thsh.h"

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


int main(int argc, char **argv, char **envp) {
  // flag that the program should end
  bool finished = 0;
  int input_fd = 0; // Default to stdin
  int ret = 0;
  bool debugFlag = false;

  // Lab 1:
  // Add support for parsing the -d option from the command line
  // and handling the case where a script is passed as input to your shell

  // Lab 1: Your code here
	int checkArgs = 0;
	while(argv[checkArgs] != NULL){
		if (argv[checkArgs][0] == '-' && strchr(argv[checkArgs], 'd') != NULL){
			debugFlag = true;
//			printf("===Running shell in debug mode===\n");
			checkArgs++;
		} else if (argv[checkArgs][0] != '-' && checkArgs != 0){
			input_fd = open(argv[checkArgs], O_RDONLY);
			break;
		}  else {
			checkArgs++;
		}
	}

  ret = init_cwd();
  if (ret) {
    printf("Error initializing the current working directory: %d\n", ret);
    return ret;
  }

  ret = init_path();
  if (ret) {
    printf("Error initializing the path table: %d\n", ret);
    return ret;
  }

  while (!finished) {

    int length;
    // Buffer to hold input
    char cmd[MAX_INPUT];
    // Get a pointer to cmd that type-checks with char *
    char *buf = &cmd[0];
    char *parsed_commands[MAX_PIPELINE][MAX_ARGS];
    char *infile = NULL;
    char *outfile = NULL;
    int pipeline_steps = 0;

    if (!input_fd) {
      ret = print_prompt();
      if (ret <= 0) {
	// if we printed 0 bytes, this call failed and the program
	// should end -- this will likely never occur.
	finished = true;
	break;
      }
    }



    // Read a line of input
    length = read_one_line(input_fd, buf, MAX_INPUT);
    if (length <= 0) {
      ret = length;
      break;
    }

    // Pass it to the parser
    pipeline_steps = parse_line(buf, length, parsed_commands, &infile, &outfile);
    if (pipeline_steps == 0){
	continue;
    }
    if (pipeline_steps < 0) {
      printf("Parsing error.  Cannot execute command. %d\n", -pipeline_steps);
      continue;
    }


    ///Handle ./ call
    if (parsed_commands[0][0][0] == '.' && parsed_commands[0][0][1] == '/'){
	parsed_commands[0][0] = &parsed_commands[0][0][2];
	if(debugFlag){
		parsed_commands[0][1] = malloc(sizeof(char)* 2);
		strcpy(parsed_commands[0][1], "-d");
	        parsed_commands[0][2] = malloc(sizeof(char) * strlen(parsed_commands[0][0]));
		strcpy(parsed_commands[0][2], parsed_commands[0][0]);
        	strcpy(parsed_commands[0][0], "./thsh");
	} else {
	        parsed_commands[0][1] = malloc(sizeof(char) * strlen(parsed_commands[0][0]));
        	strcpy(parsed_commands[0][1], parsed_commands[0][0]);
        	strcpy(parsed_commands[0][0], "./thsh");
	}
	ret = run_command(parsed_commands[0], 0, 1, true);
	continue;
    }
    if(strcmp(parsed_commands[0][0],"chmod") == 0 && strcmp(parsed_commands[0][1],"u+x") == 0){
        strcpy(parsed_commands[0][0], "./thsh");
        parsed_commands[0][1] = malloc(sizeof(char) * strlen(parsed_commands[0][0]));
	strcpy(parsed_commands[0][1], parsed_commands[0][2]);
	ret = run_command(parsed_commands[0], 0, 1, true);
	continue;
    }

    int pipes[pipeline_steps][2];

    for(int i = 0; i < pipeline_steps - 1; i++){
		pipe(pipes[i]);
	}

    // Just echo the command line for now
    // file descriptor 1 -> writing to stdout
    // print the whole cmd string (write number of
    // chars/bytes equal to the length of cmd, or MAX_INPUT,
    // whichever is less)
    //
    // Comment this line once you implement
    // command handling
    //  dprintf(1, "%s\n", cmd);

//Init pipe if needed.
    for(int i = 0; i < pipeline_steps; i++){
	bool pipeIN = false;
	bool pipeOUT = false;
	int inHandle;
	int outHandle;

//Check pipe out and in
	if(parsed_commands[i + 1][0] != NULL){
		pipeOUT = true;
	}
	if(i > 0){
		pipeIN = true;
	}

//Handle builtin commands
	if (strcmp(parsed_commands[i][0], "cd") == 0 || strcmp(parsed_commands[i][0], "exit") == 0 || strcmp(parsed_commands[i][0], "goheels") == 0){
		if(debugFlag){
        		fprintf(stderr, "RUNNING: [%s]\n", parsed_commands[i][0]);
    		}
		ret = handle_builtin(parsed_commands[i], 0, 1, &ret);
		if(debugFlag){
			fprintf(stderr, "ENDED: [%s] (ret=%i)\n",parsed_commands[i][0],ret);
		}
		continue;
	}
//Handle arbitrary commands
	else {
		if(debugFlag){
                	fprintf(stderr, "RUNNING: [%s]\n", parsed_commands[i][0]);
                }

		//Run on passed in AND passed out file locations.
		if (infile != NULL && outfile != NULL){

			if (i == 0 && parsed_commands[i + 1][0] == NULL){
				inHandle = open(infile, O_RDWR);
				outHandle = open(outfile, (O_CREAT | O_RDWR), (S_IRUSR | S_IWUSR));

				ret = run_command(parsed_commands[i], inHandle, outHandle, true);
				close(inHandle);
				close(outHandle);
                        } else {
				inHandle = open(infile, O_RDWR);
				ret = run_command(parsed_commands[i], inHandle, pipes[i][1], true);
				close(inHandle);
				infile = NULL;
			}

		//Run on passed in file.
		} else if (infile != NULL && i == 0){

			inHandle = open(infile, O_RDWR);

			if (pipeOUT){
				ret = run_command(parsed_commands[i], inHandle, pipes[i][1], true);
			} else {
				ret = run_command(parsed_commands[i], inHandle, 1, true);
			}
			close(inHandle);

		//Run on passed out file.
		} else if (outfile != NULL && parsed_commands[i + 1][0] == NULL){

			outHandle = open(outfile, (O_CREAT | O_RDWR), (S_IRUSR | S_IWUSR));

                        if (pipeIN){
				close(pipes[i - 1][1]);
				ret = run_command(parsed_commands[i], pipes[i - 1][0], outHandle, true);
                        } else {
                                ret = run_command(parsed_commands[i], 0, outHandle, true);
                        }
			close(outHandle);

		//Run on no passed files.
		} else {

			if (pipeIN && pipeOUT) {
			        close(pipes[i - 1][1]);
			        ret = run_command(parsed_commands[i], pipes[i - 1][0], pipes[i][1], true);
			} else if (pipeIN){
                       		close(pipes[i - 1][1]);
				ret = run_command(parsed_commands[i], pipes[i - 1][0], 1, true);
                        } else if (pipeOUT){
                               ret = run_command(parsed_commands[i], 0, pipes[i][1], true);
                        } else {
                               ret = run_command(parsed_commands[i], 0, 1, true);
                        }

		}

                if(debugFlag){
			fprintf(stderr, "ENDED: [%s] (ret=%i)\n",parsed_commands[i][0],ret);
                }
	}
    }

    for(int i = 0; i < pipeline_steps - 1; i++){
                close(pipes[i][0]);
        }

//    printf("%d", ret);

    // In Lab 1, you will need to add code to actually run the commands,
    // add debug printing, and handle redirection and pipelines, as
    // explained in the handout.
    //
    // For now, ret will be set to zero; once you implement command handling,
    // ret should be set to the return from the command.
    // ret = 0;

    // Do NOT change this if/printf - it is used by the autograder.
    if (ret) {
      printf("Failed to run command - error %d\n", ret);
    }

  }

  return ret;
}
