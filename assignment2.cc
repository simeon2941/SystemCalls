//***************************************************************************
//
//  assignment1.cc
//  CSCI 480Assignment 2      Due Date: 02/15/2021
//
//  Section : PE1
//
//  TA: Theresa Li and Satya Keerthi Challa
//
//  Created by Simeon Lico (z1885981)
//
//  Purpose: The purpose of this program is to start child processes and
//  use pipe() to do inter-process communication
//
//***************************************************************************
#include <iostream>
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1
int main(void)
{
    // write_msg will hold the message that will be transmited through pipe()
    char write_msg[BUFFER_SIZE] = "Go do some choores.";
    // read message will hold the message that we will read on the read side of the pipe
    char read_msg[BUFFER_SIZE];
    // call pipe_a
    int pipe_a[2];
    pipe(pipe_a);
    // check to see if pipe_a failed
    if (pipe(pipe_a) == -1)
    {
        fprintf(stderr, "Pipe A failed");
        return 1;
    }
    // call pipe_b
    int pipe_b[2];
    pipe(pipe_b);
    // check to see if pipe_b failed
    if (pipe(pipe_b) == -1)
    {
        fprintf(stderr, "Pipe B failed");
        return 1;
    }
    // for a child process
    pid_t intermediateParent = fork();
    // check if forked failed
    if (intermediateParent < 0)
    {
        fprintf(stderr, "Fork failed");
        return 1;
    }
    // fork a child process
    pid_t child = fork();
    // check to see if teh fork failed
    if (child < 0)
    {
        fprintf(stderr, "Fork failed");
        return 1;
    }

    // retrive parent info
    if (intermediateParent > 0 && child > 0)
    {
      //print out information about the parent
       cout<< "Parent: My PID is " << getpid() << " my parents PID is " << getppid()
            << " My children are " << intermediateParent << " and " << child
            << " I write to pip A: God do some chores.\n";
            //create the string to hold the command
       ostringstream oss;
        oss << "/bin/ps -f --ppid " << getppid() << "," << getpid() << "," << intermediateParent
            << "," << child;
       string s = oss.str();
       cout<< "Parent: Issuing command:" << s <<endl << endl;
        //close the unused end of the pipe
        close(pipe_a[READ_END]);
        // write to pipe_a
        write(pipe_a[WRITE_END], write_msg, strlen(write_msg) + 1); // sends the message to child
        //close the write end of the pipe_a
        close(pipe_a[WRITE_END]);
        // start another process using system
        system(s.c_str());
        // wait for processes to finish
        wait(0);
        //print this message when child processes are done with their work
        cout << "Child processes finished their work" << endl;
    }
  //get child info
    if (child == 0)
    {
        //use setbuf so the output wont be garbled
       // setbuf(stdout, NULL);
        if (intermediateParent != 0)
        {
            //close the unused end of pipe_B
            close(pipe_b[WRITE_END]);
            //read from pipe_b
            read(pipe_b[READ_END], read_msg, BUFFER_SIZE);
            //print out info about child
            cout << "Child: "
                 << "My PID is " << getpid() << " my parents PID is " << getppid()
                 << " I read from pipe B " << read_msg << endl
                 << endl;
            //close the read end of pipe_B
            close(pipe_b[READ_END]);
        }
        else
        {
            // close the unused end of the pipe
            close(pipe_a[WRITE_END]);
            //read from the pipe
            read(pipe_a[READ_END], read_msg, BUFFER_SIZE);
            //print info about the child
            cout << "Child: "
                 << "My PID is " << getpid() << " my parents PID is " << getppid()
                 << " I read from pipe A " << read_msg << endl
                 << endl;
                 //close the read end of the pipe
            close(pipe_a[READ_END]);
        }
        // sleep for 3 second
        sleep(3);
        //then print child is awake
        cout << "Child: " << getpid() << " I am awake.\n";
        //exit
        exit(0);
    }
    // get info about intermediateParent parent
    if (intermediateParent == 0)
    {
        //print info about the intermedaite Parent
        cout << "Intermediate Parent: My PID is " << getpid() << ", my parents PID is "
             << getppid() << " my child is " << child << "I write to pipe B: Go Do some Chores"
             << endl
             << endl;
        //close the unused end of the pipe
        close(pipe_b[READ_END]);
        //write to the pipe
        write(pipe_b[WRITE_END], write_msg, strlen(write_msg) + 1); // sends the message to child
        //close the write end of the pipe
        close(pipe_b[READ_END]);
        //sleep for 3 seconds the print out intermediate Parent is awake
        sleep(3);
        cout << "\nIntermediateParent Parent" << getpid() << " : I am awake.\n";
        //exit
        exit(0);
    }
    return 0;
}
