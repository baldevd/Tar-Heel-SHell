
/* COMP 530: Tar Heel SHell
 *
 * This file implements a table of builtin commands.
 */
#include "thsh.h"
#include <stdlib.h>
#include <string.h>

struct builtin {
  const char * cmd;
  int (*func)(char *args[MAX_ARGS], int stdin, int stdout);
};


static char old_path[MAX_INPUT];
static char cur_path[MAX_INPUT];

/* This function needs to be called once at start-up to initialize
 * the current path.  This should populate cur_path.
 *
 * Returns zero on success, -errno on failure.
 */
int init_cwd(void) {

  // Lab 1: Your code here
	memset(old_path, '0', MAX_INPUT);
        memset(cur_path, '0', MAX_INPUT);
	char path[MAX_INPUT];
	getcwd(path, MAX_INPUT);
	strcpy(cur_path, path);
  return 0;
}

/* Handle a cd command.  */
int handle_cd(char *args[MAX_INPUT], int stdin, int stdout) {

  // Note that you need to handle special arguments, including:
  // "-" switch to the last directory
  // "." switch to the current directory.  This should change the
  //     behavior of a subsequent "cd -"
  // ".." go up one directory
  //
  // Hint: chdir can handle "." and "..", but saving
  //       these results may not be the desired outcome...

  // XXX: Be sure to write test cases for ., .., and weirdness
  // XXX: Test for errors in the output if a cd fails

  // Lab 1: Your code here
  //
	int dirResult = -1;
	if(args[1] == NULL){
		args[1] = getenv("HOME");
	}
	if (strcmp(args[1], ".") == 0){
		//Handle switch current
		strcpy(old_path, cur_path);
		dirResult = 0;
	} else if (strcmp(args[1], "..") == 0){
		//Handle switch up
		char path[MAX_INPUT];
		int index = -1;
		char * last;

		getcwd(path, MAX_INPUT);
		last = strrchr(path, '/');
		index = last - path;
		strcpy(old_path, path);

		if(index > -1){
			path[index] = '\0';
			dirResult = chdir(path);
			strcpy(cur_path, path);
		}
	} else if (strcmp(args[1], "-") == 0){
		//Handle switch last
                char temp[MAX_INPUT];
		strcpy(temp, cur_path);
		strcpy(cur_path, old_path);
		strcpy(old_path, temp);
		dirResult = chdir(cur_path);
	} else {
		strcpy(old_path, cur_path);
		strcpy(cur_path, args[1]);
		dirResult = chdir(args[1]);
	}

  // Remove the following two lines once implemented.  These
  // just suppress the compiler warning around an unused variable
  //(void) cur_path;
  //(void) old_path;
	if(dirResult < 0){
		printf("Error: Directory %s is not a valid directory.\n", args[1]);
	}

  return dirResult;
}

/* Handle an exit command. */
int handle_exit(char *args[MAX_ARGS], int stdin, int stdout) {
  exit(0);
  return 0; // Does not actually return
}

int handle_heels(char *args[MAX_ARGS], int stdin, int stdout) {
	puts("\n"
"                                _____                    _____                    _____          \n"
"                               /\\    \\                  /\\    \\                  /\\    \\         \n"
"                              /::\\    \\                /::\\    \\                /::\\    \\        \n"
"                              \\:::\\    \\              /::::\\    \\              /::::\\    \\       \n"
"                               \\:::\\    \\            /::::::\\    \\            /::::::\\    \\      \n"
"                                \\:::\\    \\          /:::/\\:::\\    \\          /:::/\\:::\\    \\     \n"
"                                 \\:::\\    \\        /:::/__\\:::\\    \\        /:::/__\\:::\\    \\    \n"
"                                 /::::\\    \\      /::::\\   \\:::\\    \\      /::::\\   \\:::\\    \\   \n"
"                                /::::::\\    \\    /::::::\\   \\:::\\    \\    /::::::\\   \\:::\\    \\  \n"
"                               /:::/\\:::\\    \\  /:::/\\:::\\   \\:::\\    \\  /:::/\\:::\\   \\:::\\____\\ \n"
"                              /:::/  \\:::\\____\\/:::/  \\:::\\   \\:::\\____\\/:::/  \\:::\\   \\:::|    |\n"
"                             /:::/    \\::/    /\\::/    \\:::\\  /:::/    /\\::/   |::::\\  /:::|____|\n"
"                            /:::/    / \\/____/  \\/____/ \\:::\\/:::/    /  \\/____|:::::\\/:::/    / \n"
"                           /:::/    /                    \\::::::/    /         |:::::::::/    /  \n"
"                          /:::/    /                      \\::::/    /          |::|\\::::/    /   \n"
"                          \\::/    /                       /:::/    /           |::| \\::/____/    \n"
"                           \\/____/                       /:::/    /            |::|  ~|          \n"
"                                                        /:::/    /             |::|   |          \n"
"                                                       /:::/    /              \\::|   |          \n"
"                                                       \\::/    /                \\:|   |          \n"
"                                                        \\/____/                  \\|___|          \n"
	);

	puts(
"          _____                    _____                    _____                    _____            _____          \n"
"         /\\    \\                  /\\    \\                  /\\    \\                  /\\    \\          /\\    \\         \n"
"        /::\\____\\                /::\\    \\                /::\\    \\                /::\\____\\        /::\\    \\        \n"
"       /:::/    /               /::::\\    \\              /::::\\    \\              /:::/    /       /::::\\    \\       \n"
"      /:::/    /               /::::::\\    \\            /::::::\\    \\            /:::/    /       /::::::\\    \\      \n"
"     /:::/    /               /:::/\\:::\\    \\          /:::/\\:::\\    \\          /:::/    /       /:::/\\:::\\    \\     \n"
"    /:::/____/               /:::/__\\:::\\    \\        /:::/__\\:::\\    \\        /:::/    /       /:::/__\\:::\\    \\    \n"
"   /::::\\    \\              /::::\\   \\:::\\    \\      /::::\\   \\:::\\    \\      /:::/    /        \\:::\\   \\:::\\    \\   \n"
"  /::::::\\    \\   _____    /::::::\\   \\:::\\    \\    /::::::\\   \\:::\\    \\    /:::/    /       ___\\:::\\   \\:::\\    \\  \n"
" /:::/\\:::\\    \\ /\\    \\  /:::/\\:::\\   \\:::\\    \\  /:::/\\:::\\   \\:::\\    \\  /:::/    /       /\\   \\:::\\   \\:::\\    \\ \n"
"/:::/  \\:::\\    /::\\____\\/:::/__\\:::\\   \\:::\\____\\/:::/__\\:::\\   \\:::\\____\\/:::/____/       /::\\   \\:::\\   \\:::\\____\\\n"
"\\::/    \\:::\\  /:::/    /\\:::\\   \\:::\\   \\::/    /\\:::\\   \\:::\\   \\::/    /\\:::\\    \\       \\:::\\   \\:::\\   \\::/    /\n"
" \\/____/ \\:::\\/:::/    /  \\:::\\   \\:::\\   \\/____/  \\:::\\   \\:::\\   \\/____/  \\:::\\    \\       \\:::\\   \\:::\\   \\/____/ \n"
"          \\::::::/    /    \\:::\\   \\:::\\    \\       \\:::\\   \\:::\\    \\       \\:::\\    \\       \\:::\\   \\:::\\    \\     \n"
"           \\::::/    /      \\:::\\   \\:::\\____\\       \\:::\\   \\:::\\____\\       \\:::\\    \\       \\:::\\   \\:::\\____\\    \n"
"           /:::/    /        \\:::\\   \\::/    /        \\:::\\   \\::/    /        \\:::\\    \\       \\:::\\  /:::/    /    \n"
"          /:::/    /          \\:::\\   \\/____/          \\:::\\   \\/____/          \\:::\\    \\       \\:::\\/:::/    /     \n"
"         /:::/    /            \\:::\\    \\               \\:::\\    \\               \\:::\\    \\       \\::::::/    /      \n"
"        /:::/    /              \\:::\\____\\               \\:::\\____\\               \\:::\\____\\       \\::::/    /       \n"
"        \\::/    /                \\::/    /                \\::/    /                \\::/    /        \\::/    /        \n"
"         \\/____/                  \\/____/                  \\/____/                  \\/____/          \\/____/         \n"

	);
	return 0;
}

static struct builtin builtins[] = {{"cd", handle_cd},
				    {"exit", handle_exit},
				    {'\0', NULL}};

/* This function checks if the command (args[0]) is a built-in.
 * If so, call the appropriate handler, and return 1.
 * If not, return 0.
 *
 * stdin and stdout are the file handles for standard in and standard out,
 * respectively. These may or may not be used by individual builtin commands.
 *
 * Places the return value of the command in *retval.
 *
 * stdin and stdout should not be closed by this command.
 *
 * In the case of "exit", this function will not return.
 */
int handle_builtin(char *args[MAX_ARGS], int stdin, int stdout, int *retval) {
  int rv = 0;
  // Lab 0: Your Code Here
  // Comment this line once implemented.  This just suppresses
  // the unused variable warning from the compiler.
  (void) builtins;

	if(strcmp(args[0],"cd") == 0){
		*retval = handle_cd(args, stdin, stdout);
		rv = *retval;
	}

	if(strcmp(args[0],"exit") == 0){
		handle_exit(args, stdin, stdout);
		rv = -1; //Should never be set to this.
	}

	if(strcmp(args[0],"goheels") == 0){
		*retval = handle_heels(args, stdin, stdout);
		rv = *retval;
	}

  return rv;
}

/* This function initially prints a default prompt of:
 * thsh>
 *
 * In Lab 1, Exercise 3, you will add the current working
 * directory to the prompt.  As in, if you are in "/home/foo"
 * the prompt should be:
 * [/home/foo] thsh>
 *
 * Returns the number of bytes written
 */
#define MAX_RENDERED_PATH 40

int print_prompt(void) {
  int ret = 0;
  // Print the prompt
  // file descriptor 1 -> writing to stdout
  // print the whole prompt string (write number of
  // bytes/chars equal to the length of prompt)
  //
  const char *prompt = "thsh> ";

  // Lab 1: Your code here
	char current[MAX_INPUT];
	getcwd(current, MAX_INPUT);
	char toRender[MAX_RENDERED_PATH] = "[";
	char endPath[MAX_INPUT];
	char * bracket = "] ";

	getcwd(endPath, MAX_INPUT);
	strncat(toRender, endPath, strlen(endPath));
	strncat(toRender, bracket, 2);
	strncat(toRender, prompt, strlen(prompt));



  ret = write(1, toRender, strlen(toRender));
  return ret;
}
