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
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#define SIZE 84

//send error meseage to fd 2 - STDERR.
void sendError(char * str){
	write(2,str,strlen(str));
}

//handler function to print the board
void sighandlerPrint(int signum,siginfo_t* info,void* ptr){
	char* buffer[SIZE];
	char* temp;
	int index;
	if(read(0,buffer,SIZE)==-1){
		sendError("can`t read from std-in");
	}

    int* arrFromStr = stringToArray(buffer);
    printBoard(arrFromStr,1);



//	//get the first num
//	temp=strtok(buffer,",");
//
//	//get the whole numbers - 16
//	for(index=0;index<16;index++){
//		if(write(1,"| ",2)<0)
//			sendError("can`t write | to fd 1");
//
//		//end of the line
//		if(index%4==0 && index!=0){
//			if(write(1,"\n| ",3)<0)
//				sendError("can`t write | to fd 1");
//		}
//
//		//check if the num equals 0 or need to be pad with 0`s
//		if(!strcmp("0",temp)){
//			if(write(1,"    ",4)<0)
//				sendError("can`t write 0 to fd 1");
//		}
//		else{
//			//pad with 0
//			if(strlen(temp)==4){
//				if(write(1,temp,4)<0)
//					sendError("can`t write 4 digit number");
//			}
//			else if(strlen(temp)==3){
//				if(write(1,"0",1)<0)
//					sendError("can`t write 0 to pad 3 digit number");
//				if(write(1,temp,3)<0)
//					sendError("can`t write 3 digit number");
//			}
//			else if(strlen(temp)==2){
//				if(write(1,"00",2)<0)
//					sendError("can`t write 00 to pad 2 digit number");
//				if(write(1,temp,2)<0)
//					sendError("can`t write 2 digit number");
//			}
//			else{
//				if(write(1,"000",3)<0)
//					sendError("can`t write 000 to pad 1 digit number");
//				if(write(1,temp,1)<0)
//					sendError("can`t write 1 digit number");
//			}
//		}
//
//		if(write(1," ",1)<0)
//			sendError("can`t write space after a number");
//
//		//get the next number
//		if(index!=14)
//			temp=strtok(NULL,",");
//		else
//			temp=strtok(NULL,"\n");
//
//	}
//
//	//at the end of the print
//	if(write(1,"|\n\n",3)<0)
//		sendError("can`t write at the end of the print");
}

//handler function to finsih the program
//write to std-out BYE BYE or error to the std-err
//if can`t write to screen
void sighandlerEnd(int signum,siginfo_t* info,void* ptr){
	if(write(1,"BYE BYE\n",strlen("BYE BYE\n"))<0)
		write(2,"can not write",strlen("can not write"));
	exit(0);

}

//blokc all signal but two we wwant to handle
//SIUSR1 - handle the print function
//SIGINT - exit the program
//run with an infinite loop and wait to signal
int main(int argc,char* argv[]){

	//set block withour SIGINT - to exit program
	sigset_t block_mask;
	sigfillset(&block_mask);
	sigdelset(&block_mask,SIGINT);

	//sigaction act - block by block_mask
	//when get SIGUSR1 - run the sighandlerPrint function.
	struct sigaction act;
	act.sa_mask=block_mask;
	act.sa_sigaction=sighandlerPrint;
	act.sa_flags=SA_SIGINFO;
	sigaction(SIGUSR1,&act,NULL);

	//finish the program if get SIGINT
	struct sigaction finish;
	sigset_t block_mask2;
	sigfillset(&block_mask2);
	finish.sa_mask=block_mask2;
	finish.sa_sigaction=sighandlerEnd;
	finish.sa_flags=SA_SIGINFO;
	sigaction(SIGINT,&finish,NULL);

	while(1){
		//wait for signal
		pause();
	}

}
void printBoard(int gameBoard[16],int pidPrint) {
    int i;
    for (i = 0; i < 16;) {
        if (gameBoard[i] == 0) {
            printf("|      ");
        } else {
            printf("| %04d", gameBoard[i]);
            printf(" ");
        }
        i++;
        if (i % 4 == 0) {
            printf("|\n");
        }
    }
}

int* stringToArray(char* m) {
    char *pt;
    static int strToArray[16];
    int i = 0;
    pt = strtok(m, ",");
    while (pt != NULL) {
        int a = atoi(pt);
        strToArray[i] = a;
        pt = strtok(NULL, ",");
        i++;
    }
    return strToArray;
}
