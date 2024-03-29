// A sample systems program

int main(int argc, char *argv[]){
    if(argc < 4){
        fprintf(stderr, "Invalid input.\n");
        fprintf(stderr, "Usage: dir_name [file] [command] \n");
        exit(0);
    }

    char pattern_string = (char)argv[2];
    char file_path      = (char)argv[3];

    if(!(strcmp(argv[1], "@"))){
        // Implementing @ operator
        int fd[2];
        if(pipe(fd) < 0)
            perror("pipe");

        unsigned int pid = fork();
        if(pid == 0){   // Child will run wc -l
            dup2(fd[0], 0);     // as wc -l will take it's input from pipe
            close(fd[0]);
            close(fd[1]);
            char* arguments[] = {"wc", "-l", NULL};
            execvp("wc", arguments);
        }
        else{  // Parent will run grep command
            dup2(fd[1], 1);         // as grep -rF will push it's output to pipe
            close(fd[1]);
            close(fd[0]);
            char* arguments[] = {"grep", "-rF", pattern_string, file_path, (char*)NULL} ;
            execvp("grep", arguments);
        }
    }
    return 0;
}
