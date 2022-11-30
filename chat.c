#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
    pid_t Pid;
    char mensaje[150];
    int pipe_padre[2], pipe_hijo[2], hijo, padre;
    pipe(pipe_padre);
    pipe(pipe_hijo);
    Pid = fork();
    switch (Pid){
    case 0:
        close(pipe_padre[1]);
        close(pipe_hijo[0]);
        
        while (read(pipe_padre[0],mensaje,150) >0 && strcmp(mensaje,"FIN\n")!=0)
        {
            hijo = read(pipe_padre[1], mensaje, sizeof(mensaje));
            mensaje[hijo] = '\0';
            printf("(pipe_hijo)-> %s",mensaje);
            printf("Escriba su mensaje (desde pipe_hijo): \n");
            scanf("%c",&mensaje);
            if(strcomp(mensaje,"FIN")){
                exit(0);
            }
            //fgets(mensaje, sizeof(mensaje), stdin);
            if (write(pipe_hijo[1], mensaje, strlen(mensaje)) == -1){
                exit(0);
            }
            if(strcmp(mensaje,"FIN\n")==0){
                exit(0);
            }
        }
        close(pipe_padre[1]);
        close(pipe_hijo[0]); 
        break;
    case -1:
        printf("Error");
        exit(0);
        break;
    default:
        close(pipe_padre[0]);
        close(pipe_hijo[1]);
        do
        {
            printf("Escriba mensaje (desde pipe_padre): \n");
            scanf("%c",&mensaje);
            if(strcomp(mensaje,"FIN")){
                exit(0);
            }
            //fgets(mensaje, sizeof(mensaje),stdin);
            if (write(pipe_padre[1],mensaje, strlen(mensaje)) == -1)
            {
                printf("Error");
                exit(0);
            }
            if(strcmp(mensaje,"FIN\n")==0){
                exit(-1);
            }
            padre = read(pipe_hijo[0],mensaje, sizeof(mensaje));
            mensaje[padre]='\0';
            printf("(pipe_padre)-> %s",mensaje);
        } while (strcmp(mensaje,"FIN\n")!=0);
        close(pipe_padre[1]);
        close(pipe_hijo[0]);
        break;
    }
    return 0;
}