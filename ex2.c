/******************************************
* Uriel Bezalel
* 304836497
*
* Exercise 2
******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

//send error meseage to fd 2 - STDERR.
void sendError(char * str){
	write(2,str,strlen(str));
}
//the main function make two sons of ex2_inp and
//ex2_upd and run them togther
int main(int argc,char* argv[]){
	pid_t pid1,pid2;
	//allocate 2 int to send to the pipe
	//the first for read the second for write
	int fd[2];
	char* args [3];
	char pidNum [5];
	int y= atoi(argv[1]);
	//create the pipe
	//fd[0] = read;
	//fd[1] = write;
	if(pipe(fd)==-1){
		sendError("Pipe Error - ex2");
		exit(1);
	}
	//create the first process
	pid1=fork();
	//error case
	if(pid1 <0)
		sendError("firs fork (pid1) Error");

	//children process
	if(0==pid1){
		//I-O redrection - close the original read fd;
		//get the game from the other process - ex2_upd.c
		if(close(0)<0)
				sendError("Error to close the fd 0 - for STD-IN");
		//read from the fd[0] instead of the original read fd;
		if(dup(fd[0])<0)
				sendError("Error while trying to dup with reading");

		//I-O redrection - close the fd;
		if(close(fd[1])<0)
			sendError("Error close fd[1]");
		if(close(fd[0])<0)
			sendError("Error close fd[0]");
		args[0]="./ex2_inp.out";
		args[1]=NULL;

		//run the process
		if (execvp("./ex2_inp.out",args)==-1)
			sendError("can not execute ex2_inp");
	}
	// father process
	else{
		pid2=fork();
		if(pid2<0)
			sendError("second fork (pid2) error");
		//child process
		if(0==pid2){
			//write the string of the game to the other
			//process - ex2_inp.c
			if(close(1)<0)
				sendError("Error close the write std-out");
			if(dup(fd[1])<0)
				sendError("Error while dup the write fd");

			//I-O redrection - close the fd;
			if(close(fd[1])<0)
				sendError("Error close fd[1]");
			if(close(fd[0])<0)
				sendError("Error close fd[0]");

			args[0]="./ex2_upd.out";
			sprintf(pidNum,"%d",pid1);
			args[1]=pidNum;
			args[2]=NULL;
			//sleep to be sure the other pid (ex2_inp.out) is ready
			//and already running
			sleep(2);
			//run the proccess
			if (execvp("./ex2_upd.out",args)==-1)
				 sendError("can not execute ex2_upd");
		}

		//wait time of the process finish
		sleep(y);

		//kill the processes by send SIGINT
		if (kill(pid2,SIGINT)!=0)
		 	sendError("can`t kill pid2 - ex2_upd.out");
		sleep(2);
		if (kill(pid1,SIGINT)!=0)
			sendError("can`t kill pid1 - ex2_inp.out");

		//close the pipe
		if(close(fd[0])<0)
				sendError("can`t close the fd[0] - read of the pipe");
		if(close(fd[1])<0)
			sendError("can`t close the fd[1] - write of the pipe");
	}
}
