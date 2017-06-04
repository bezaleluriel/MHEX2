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
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#define SIZE 80

int gotoFlag = 0;

//global variable
int flag=0;

//send error meseage to fd 2 - STDERR.
void sendError(char * str){
    write(2,str,strlen(str));
}


// get char from user by read it from fd 0
//get it without need to "enter"
char getch() {
    char buf = 0;
    struct termios old = {0};

    //it should write the settings of stdin to old
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");

    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;

    //change attribute now
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");

    if (read(0, &buf, 1) < 0 && errno!=EINTR)
        perror ("read()");

    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;

    //restore the deafult setting after finish all writing
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");

    return buf;
}

//the function get the board of the game and
//pid of the print proccess.
//the function print a string of the cells in the
//board and send signal to print proceess
void printBoard(int gameBoard[4][4],int pidPrint){
    int i,j;
    char str [4];
    int notZeroCounter = 0;


    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            sprintf(str,"%d",gameBoard[i][j]);
            if(write(1,str,strlen(str))<0)
                sendError("can not write");
            //last cell dont put ",";
            if(!(i==3 && j==3)){
                if(write(1,",",1)<0)
                    sendError("can not write");
            }
        }
    }

    if(gotoFlag == 1){

        sendError(" Game Over!\n");
       // kill(pidPrint,SIGINT);
        //kill the processes by send SIGINT
        if (kill(0,SIGKILL)!=0)
            sendError("can`t kill pid1 - ex2_inp.out");

        exit(1);
       // kill(pidPrint, SIGTERM);

    }

    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            if(gameBoard[i][j] != 0){
                notZeroCounter ++;
            }
            if(gameBoard[i][j] == 2048){
                sendError("Congratulations!\n");
                if (kill(0,SIGKILL)!=0)
                    sendError("can`t kill pid1 - ex2_inp.out");

                exit(1);
            }
        }
    }
    if (notZeroCounter == 16){
        //printf("BLAAAAAAAAAAAAAAAAAAA \n");
        gotoFlag = 1;

    }


    if(write(1,"\n",1)<0)
        sendError("can not write");

    if(kill(pidPrint,SIGUSR1)!=0){
        sendError("error");
        exit(1);
    }
}

//get random waiting time
//1<x<5
int setWaitTime(){
    return (rand() % 3  +2);
}

//the function get the board game
//and move left and make the union
void moveLeft(int gameBoard[4][4]){


    //Array to save the last index merged for each row.
    int mergedIndexesArray[4]={0,0,0,0};
    //Saving the last index where occured a merge.
    int currentIndex = 1;
    //Pointer to the board as 1D array.
    int* gameBoard2 = &gameBoard[0][0];
    //The current rotation (loop run on it).
    int rotations = 0;
    //For each cell , moving it to left at most 4 times.
    for(rotations = 0; rotations < 4; rotations++){
        //Starting from index 1 cause index 0 dont have left cells.
        currentIndex = 1;
        while(currentIndex < 16 ){
            int mergedInd = currentIndex/4;

            //If the cell is zero - move to the next.
            if( (gameBoard2[currentIndex] != 0) && ((currentIndex-1)%4)!=3){
                //If the left cell is zero - move the current one to left.
                if(gameBoard2[currentIndex-1]==0){
                    gameBoard2[currentIndex-1] = gameBoard2[currentIndex];
                    //put 0 at the current index.
                    gameBoard2[currentIndex] = 0;
                    //If the left equals to the current, and wasnt merge on it.
                }else if( (gameBoard2[currentIndex-1]==gameBoard2[currentIndex])&&
                          (currentIndex-1>=mergedIndexesArray[mergedInd])){
                    gameBoard2[currentIndex-1]*= 2;
                    gameBoard2[currentIndex]=0;
                    //Saving the index of the merge
                    mergedIndexesArray[mergedInd] = currentIndex;
                    //Checking if the last merge make a win.
//                    if(gameBoard[currentIndex-1] == gameBoard){
//                        currentGame.gameFinished = WIN;
//                    }
                }
            }
            currentIndex++;
        }
    }











/*    int i, j, temp;*/
/*    for (i = 0; i<4; i++){*/
/*        for (j = 1; j<4; j++){*/
/*            temp = j;*/
/*            //move left as much as we can*/
/*            while (temp > 0){*/
/*                //move left*/
/*                if (gameBoard[i][temp - 1] == 0 && gameBoard[i][temp] != 0){*/
/*                    gameBoard[i][temp - 1] = gameBoard[i][temp];*/
/*                    gameBoard[i][temp] = 0;*/
/*                }*/
/*                //union*/
/*                if (gameBoard[i][temp - 1] == gameBoard[i][temp]){*/
/*                    gameBoard[i][temp - 1] = 2* gameBoard[i][temp];*/
/*                    gameBoard[i][temp] = 0;*/
/*                }*/
/*                temp--;*/
/*            }*/
/*        }*/
/*    }*/
}

//the function get the board game
//and move right and make the union
void moveRight(int gameBoard[4][4]){





    RotateBoardRight(gameBoard);
    RotateBoardRight(gameBoard);
    moveLeft(gameBoard);
    //Restore the board to the original state.
    RotateBoardRight(gameBoard);
    RotateBoardRight(gameBoard);











//    int i, j ,temp;
//    for (i = 0; i<4; i++){
//        for (j = 2; j>=0; j--){
//            temp = j;
//            //move as much as we can
//            while (temp < 3){
//                //move up
//                if (gameBoard[i][temp + 1] == 0 && gameBoard[i][temp] != 0){
//                    gameBoard[i][temp + 1] = gameBoard[i][temp];
//                    gameBoard[i][temp] = 0;
//                }
//                //union
//                if (gameBoard[i][temp + 1] == gameBoard[i][temp]){
//                    gameBoard[i][temp + 1] = 2 * gameBoard[i][temp];
//                    gameBoard[i][temp] = 0;
//                }
//                temp++;
//            }
//        }
//    }
}

//the function get the board game
//and move up and make the union
void moveUp(int gameBoard[4][4]){




    //Rotating 3 times to right
    RotateBoardRight(gameBoard);
    RotateBoardRight(gameBoard);
    RotateBoardRight(gameBoard);
    moveLeft(gameBoard);
    //Restore the board to the original state.
    RotateBoardRight(gameBoard);

//    int i, j, temp;
//    for (i = 1; i < 4; i++){
//        for (j = 0; j < 4; j++){
//            temp = i;
//            while (temp > 0){
//                //move up
//                if (gameBoard[temp - 1][j] == 0 && gameBoard[temp][j] != 0){
//                    gameBoard[temp - 1][j] = gameBoard[temp][j];
//                    gameBoard[temp][j] = 0;
//                }
//                //union
//                if (gameBoard[temp - 1][j] == gameBoard[temp][j]){
//                    gameBoard[temp - 1][j] = 2*gameBoard[temp][j];
//                    gameBoard[temp][j] = 0;
//                }
//                temp--;
//            }
//        }
//    }
}

//the function get the board game
//and move down and make the union
void moveDown(int gameBoard[4][4]){





    //Rotating to right
    RotateBoardRight(gameBoard);
    moveLeft(gameBoard);
    //Restore the board to the original state.
    RotateBoardRight(gameBoard);
    RotateBoardRight(gameBoard);
    RotateBoardRight(gameBoard);
//    int i, j, temp;
//    for (i = 2; i >=0 ; i--){
//        for (j = 0; j < 4; j++){
//            temp = i;
//            while (temp <3){
//                //move down
//                if (gameBoard[temp + 1][j] == 0 && gameBoard[temp][j] != 0){
//                    gameBoard[temp + 1][j] = gameBoard[temp][j];
//                    gameBoard[temp][j] = 0;
//                }
//                //union
//                if (gameBoard[temp + 1][j] == gameBoard[temp][j]){
//                    gameBoard[temp + 1][j] = 2 * gameBoard[temp][j];
//                    gameBoard[temp][j] = 0;
//                }
//                temp++;
//            }
//        }
//    }
}

//the function get the board and fill it with
//0 except 2 cells of 2 - initialize the board.
void start(int gameBoard [4][4]){
    int row1,col1,row2,col2,i,j;
    //zeros the board
    for (i=0;i<4;i++){
        for (j=0;j<4;j++){
            gameBoard[i][j]=0;
        }
    }
    //insert two squers of 2
    row1=rand() %4;
    col1=rand() %4;
    do{
        row2=rand() %4;
        col2=rand() %4;
    }while(col1==col2 && row1==row2);
    gameBoard[row1][col1]=2;
    gameBoard[row2][col2]=2;
}

/************************************************
* function name: sighandler  			*
* the input: signum, info and ptr		*
* the output: none								*
* the function operation: turn on the flag      *
************************************************/
void sighandler(int signum,siginfo_t* info,void* ptr){
    //mark that this signal calls
    flag=1;
}

//the function get signum, info and ptr
//the function end the program
void sighandlerEnd(int signum,siginfo_t* info,void* ptr){
    exit(0);
}

//the function add a random cell of 2 to the board
void addSqure(int gameBoard [4][4] ){
    int col,row;
    int i,j,full=1;
    //check if the board full
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            //empty
            if(!gameBoard[i][j]){
                full=0;
                break;
            }
        }
    }
    if(!full){
        //add 2 to random cell that equals 0
        do{
            row=rand() %4;
            col=rand() %4;
        }while(gameBoard[row][col]!=0);
        gameBoard[row][col]=2;
    }

}

//the function get the board and a char from the user
//the function move the board by the selected char;
void move(int gameBoard [4][4] ,char letter){
    switch(letter){
        int i,j,temp;
        case 'A':
            moveLeft(gameBoard);
            break;
        case 'D':
            moveRight(gameBoard);
            break;
        case 'W':
            moveUp(gameBoard);
            break;
        case 'X':
            moveDown(gameBoard);
            break;
        case 'S':
            start(gameBoard);
            break;
        default:
            break;
    }
}

int main(int argc,char* argv[]){
    int waitTime;
    int readResult;
    int pidPrint=atoi(argv[1]);
    char letter;
    int gameBoard[4][4];
    srand(time(0));
    //struct for the signal of wait time

    sigset_t block_mask;
    //block all but sigint to exit the prog
    sigfillset(&block_mask);
    sigdelset(&block_mask,SIGINT);
    struct sigaction stop;
    stop.sa_mask=block_mask;
    stop.sa_sigaction=sighandler;
    stop.sa_flags=SA_SIGINFO;

    sigaction(SIGALRM,&stop,NULL);

    //struct for finish
    struct sigaction end;
    sigset_t block_mask2;
    //block all signal while work on it
    sigfillset(&block_mask2);
    end.sa_mask=block_mask2;
    end.sa_sigaction=sighandlerEnd;
    end.sa_flags=SA_SIGINFO;
    sigaction(SIGINT,&end,NULL);

    //init the board
    start(gameBoard);
    //print the board
    printBoard(gameBoard,pidPrint);
    //set a new waiting time
    waitTime=setWaitTime();
    while(1){
        alarm(waitTime);
        letter=getch();
        //the x seconds pass
        if(flag){
            //the program get the alarm signal
            flag=0;
            addSqure(gameBoard);
        }
            //get a move from user
        else{
            //cancel the alarm - get char
            alarm(0);
            move(gameBoard,letter);
        }
        //print the board
        printBoard(gameBoard,pidPrint);
        //set a new waiting time
        waitTime=setWaitTime();                                                                                                                                                                                                          ;
    }
    return 0;
}


void RotateBoardRight(int gameBoard[4][4])
{
    int n = 4;
    int i , j;
    for(i=0; i<n/2; i++) {
        for (j = 0; j < (n + 1) / 2; j++) {
            int temp = gameBoard[i][j];
            gameBoard[i][j] = gameBoard[n - 1 - j][i];
            gameBoard[n - 1 - j][i] =
                    gameBoard[n - 1 - i][n - 1 - j];
            gameBoard[n - 1 - i][n - 1 - j] =
                    gameBoard[j][n - 1 - i];
            gameBoard[j][n - 1 - i] = temp;
        }
    }
}