#ifndef helper_h
#define helper_h

#define EXIT_FAILURE 1

int back_g;
char CURR_JOB[2000];
struct bg_job {
    char job_name[300];
    pid_t PID;  
};
typedef struct bg_job bg_job;
bg_job jobs[500];

// A header file for helpers.c
// Declare any additional functions in this file
/*******************HELPER FUNCTION************************/
int countDirectories(char * path); // return how many backslash exit in a path for indicate directories
int isPipingExist(char *str, char c); // check is pipe symbol present in command line argument
void removeSpaces(char *str); // it will remove spaces from the command
char* copyStr(char* str);
int totalSpace(char* str); // return total Number of arguments
char* concatinate(char* str1, char *str2);
char** parseCommand(char * userCommand);
char* removeCharacter(char* line , char c);// remove character c from start or from end of the command
void removeFirstSpaces(char * str); // remove spaces from start
/************************Build In command*****************************/
int executeCd(char **argv); // it will execute cd command


/********************** REDIRECTION ******************************/
void redirections(job_info* job); //redirection >, < etc
void resetRedirections(job_info* job); // reset redirection <, >

/*************************** PIPING ********************************/
char **pipe_elements(char *input);
void piping(job_info*job, char *command);

void recursiveExecute(job_info * job, char* command); //this function will execute cd, redirection operators, piping and background Jobs
int executeCommand(job_info * job);
/****************** FOREGROUND AND BACKGROUND **********************/
void allJobkill();// kill all jobs
void killjob(char **args, int no_args); // kill a perticular job
void run(job_info* job,char **args, int no_args, int bg); // it will execute back ground and foreground processes

#endif
