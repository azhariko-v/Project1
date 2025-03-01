#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <cstring>

using namespace std;

void producer(int pipe1) {//produce writes data to pipe

    vector<int> data = {1, 2, 3, 4, 5};//data written
   
    for (int num : data) {//write function
        if (write(pipe1, &num, sizeof(num)) == -1) {
            cerr << "Write to pipe failed." << endl;
            exit(1);
        }
        cout << "Producer wrote: " << num << endl;
        usleep(1000);//better visualization of process in terminal
    }
   
    close(pipe1);
}

void consumer(int pipe1) {//consumer reads data from pipe
    int data;
   
    while (read(pipe1, &data, sizeof(data)) > 0) {//read functiom
        cout << "Consumer received: " << data << endl;
    }
   
    close(pipe1);
}

int main() {
    int pipe1[2];  //initialize pipe. pipe[0] is the read end pipe[1] is the write end
    if (pipe(pipe1) == -1) {
        cerr << "Pipe creation failed." << endl;//error handling
        return 1;
    }

    pid_t pid = fork();//fork initialization

    if (pid == -1) {
        cerr << "Fork failed." << endl;//error handling
        return 1;
    }

    if (pid == 0) {
        close(pipe1[1]);//close write end
       
        consumer(pipe1[0]); //consumer:child process: reads from pipe
    } else {
        close(pipe1[0]);//close read end
       
        producer(pipe1[1]); //parent process writes to pipe

        wait(nullptr);//wait for child process
    }

    return 0;
}
