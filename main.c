// Assignment 4 : Batch Downloading
// Created by Russ Sobti on 5/15/23.
//
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
struct file_args {
	char *url;
	char *filename;
	int seconds; // this is the timeout, can be null (if null, set to 10 because funny).
	int DONE;
};

int download_file( struct file_args args);
struct file_args* process_file(const char* filename){
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	int num_lines = 0;
	char fname[256];
	char url[256];
	int timeout = 10;
	struct file_args *args_array = NULL;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error: could not open file %s\n", filename);
		return NULL;
	}
	while ((getline(&line, &len, fp)) != -1) {

		// parse the line for the url, filename, and timeout. If the timeout is not specified, set it to 69420.
		if (sscanf(line, "%255s %255s %d", fname, url, &timeout) != 3) {
			// there's no timeout specified, so set it to 69420
			timeout = 69420;
		}
		if (timeout <= 0) {
			timeout = 69420;
		}
		struct file_args args = {strdup(url), strdup(fname), timeout, 0};
		num_lines++;
		args_array = realloc(args_array, sizeof(struct file_args) * (num_lines+1));
		if (args_array == NULL) {
			printf("Error: could not allocate memory for args_array\n");
			fclose(fp);
			free(line);
			return NULL;
		}
		args_array[num_lines - 1] = args;
	}
	// After the while loop in process_file function, null the last element of the args_array
	args_array[num_lines] = (struct file_args) {NULL, NULL, -1, 0};


	fclose(fp);
	free(line);
	return args_array;
}


int main(int argc, char *argv[]) {
	// Check for correct number of arguments
	if (argc < 1) {
		printf("Error: not enough arguments\n");
		return 1;
	}
	// Set default max processes to 2 if not specified
	int max_processes = 2;
	if (argc >= 4) {
		// Warn user that anything after the first 2 arguments will be ignored
		printf("Warning: only the first 2 arguments will be used. The rest will be ignored.\n");
	}
	if (argc == 3) {
		char *endptr;
		max_processes = strtol(argv[2], &endptr, 10);
		if (*endptr != '\0' || max_processes <= 0) {
			printf("Error: max processes must be a positive integer\n");
			return 1;
		}
	}

	// Process the file and store the file_args struct array
	struct file_args *args_array = process_file(argv[1]);
	if (args_array == NULL) {
		printf("Error: failed to read file\n");
		return 1;
	}

	// Create child processes to download the files
	int num_files = 0;
	while (args_array[num_files].seconds >= 0) {
		num_files++;
	}

	while(num_files > 0) {
		int num_processes = num_files > max_processes ? max_processes : num_files;
		pid_t pid[num_processes];
		for (int i = 0; i < num_processes; i++) {
			pid[i] = fork();
			if (pid[i] == -1) {
				printf("Error: failed to create process\n");
				return 1;
			}
			if (pid[i] == 0) {
				// Child process
//				printf("Child process %d started\n", getpid());
				int start = i * (num_files / num_processes);
				int end = (i + 1) * (num_files / num_processes) - 1;
				if (i == num_processes - 1) {
					end = num_files - 1;
				}
				for (int j = start; j <= end; j++) {
//					printf("Downloading file %s from %s with timeout %d\n", args_array[j].filename, args_array[j].url,
//					       args_array[j].seconds);
					int status = download_file(args_array[j]);
					if (status != 0) {
						printf("Error: could not download file %s from %s\n", args_array[j].filename,
						       args_array[j].url);
					}
				}
				printf("Child process %d completed\n", getpid());
				exit(0);
			}
		}

		// Parent process waits for child processes to complete
		int status;
		for (int i = 0; i < num_processes; i++) {
			waitpid(pid[i], &status, 0);
			if (WIFEXITED(status)) {
				printf("Child process %d exited with status %d\n", pid[i], WEXITSTATUS(status));

			}
		}
		num_files-=num_processes;
		// Shift the array to the left by the number of processes that just completed
		for (int j = 0; j < num_files; j++) {
			args_array[j] = args_array[j + num_processes];
		}
	}

	// Free memory and exit
	free(args_array);
	return 0;
}
int download_file(struct file_args args) {
	// Download file from url and save it to filename using curl. Wait for a maximum of seconds before timing out.
	printf("Downloading file: %s\n", args.filename);
	if (args.DONE) return 0;
	char seconds_str[10];
	sprintf(seconds_str, "-m %d", args.seconds);

	char command[512];
	sprintf(command, "curl %s %s -o %s", args.url, seconds_str, args.filename);

	int status = system(command);
	printf("Status: %d\n", status);
	// set the DONE flag
	args.DONE = 1;
	return 0;
}