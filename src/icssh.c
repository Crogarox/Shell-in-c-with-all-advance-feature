#include "icssh.h"
#include "helpers.h"
#include <readline/readline.h>
#include<readline/history.h>

void shift(int i) { // delete job from the job list
     for(int j = i; j <= back_g +2; j++) {
          strcpy(jobs[i].job_name, jobs[i+1].job_name);
          jobs[i].PID = jobs[i+1].PID;
	 }

     back_g--;
}

void sigchildHandler(int sig) {
     int x;
     int pid = waitpid(-1, &x, WNOHANG);

     if(pid > 0) {
		char str[200];
		int status =0;
		for(int i=0; i < back_g; i++) {
			if(jobs[i].PID == pid) {
				strcpy(str, jobs[i].job_name); // for showing the status of the command on counsole
				shift(i);
				status = 1;      
				break;
			}
		}

    	printf(BG_TERM, pid, str);	
		printf("\n");
		printf(SHELL_PROMPT);
		fflush(stdout);
	}
	return;
}

int main(int argc, char* argv[]) {
	int exec_result;
	char* line;
#ifdef GS
    rl_outstream = fopen("/dev/null", "w");
#endif

	struct sigaction sa;
    	sa.sa_handler = &sigchildHandler;
    	sa.sa_flags = SA_RESTART;
    	sigaction(SIGCHLD,&sa, NULL);

	// Setup segmentation fault handler
	if (signal(SIGSEGV, sigsegv_handler) == SIG_ERR) {
		perror("Failed to set signal handler");
		exit(EXIT_FAILURE);
	}
	

    // print the prompt & wait for the user to enter commands string
	while (1) {
		
		line = readline(SHELL_PROMPT);
		add_history(line);
		// MAGIC HAPPENS! Command string is parsed into a job struct
		// Will print out error message if command string is invalid
		if(line != NULL){
			job_info* job = validate_input(line);
			
			
			if (job == NULL) { // Command was empty string or invalid
				free(line);
				continue;
			}
			//debug_print_job(job);
			// example built-in: exit
			if (strcmp(job->procs->cmd, "exit") == 0) {
				// Terminating the shell
				free(line);
				allJobkill();
				free_job(job);
				validate_input(NULL);
				return 0;
			}
			executeCommand(job); //this function will execute cd, redirection operators, piping and background Jobs
			
			//free_job(job);  // if a foreground job, we no longer need the data
			resetRedirections(job); // reset directions
		}
	}
    // calling validate_input with NULL will free the memory it has allocated
    validate_input(NULL);

#ifndef GS
	fclose(rl_outstream);
#endif
	return 0;
}
