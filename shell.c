*/
    Alunas:Bruna Scafutto (800365) e Vitoria Borreli (800203)
    Diciplina: Sistemas Operacionais
    Descrição: Esse programa implementa um shell simplificado
    Professor: Prof. Dr. Helio Guardia
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

void exec_in_foreground(char *programa, char *parametros[]) {
  pid_t pidFilho;
  int status;

  pidFilho = fork();

  if (pidFilho <0) {
    perror("Falha na criação do processo filho");
  }

  else if (pidFilho == 0) {
    printf("Processo filho criado");
    execvp(programa, parametros);

    status = execvp(programa, parametros);
    if(status==-1){
      perror("Falha na execução do programa");
      exit(1);
    }
  } else {
    wait(NULL); // espera o processo filho terminar
  }
}


void zombies(int sinal){
  pid_t pid;
  int status;
   while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) 
            printf("O processo de pid %d terminou normalmente\n",pid);
        else if (WIFSIGNALED(status)) 
            printf("O processo de pid %d terminou com o recebimento de um sinal.\n",pid);  
    }
  
}

void exec_in_background(char *programa, char *parametros[]) {
  pid_t pidFilho;
  pidFilho = fork();
  int status;
  signal(SIGCHLD, zombies);

  if (pidFilho == -1) {
    perror("Falha na criação do processo filho");
  }

  else if (pidFilho == 0) {
    printf("processo filho criado");
    status= execvp(programa, parametros);
    printf("processo sendo execuado in background");
    if(status==-1){
       perror("Falha na execução do programa");
       exit(1);
    }
   
  }
}

int main(void) {
  //Loop do shell
while(1){
  printf(" $ ");
    char comando[100];
    int readStatus;
    //size_t tam;
    int i = 1;
    int j;
  if(fgets(comando, sizeof(comando), stdin)!=NULL){
   int tam=strlen(comando);
   if(tam>0 && comando[tam-1]=='\n'){
     comando[tam-1] ='\0';
   }
   
    char *programa = strtok(comando, " "); // primeira palavra
    if(strcmp(programa,"Ctrl + c")==0)
      break;
   

   char *tokens[20]; 
   tokens[1] = strtok(NULL," "); //segunda palavra
   while (tokens[i] != NULL) {
        i++;
        tokens[i] = strtok(NULL, " ");
    }
    
  char *parametros[20] ;
  memset(parametros,0, sizeof(parametros));
  parametros[0]= programa;
    
  if(strcmp(tokens[i-1],"&")==0){ //verifica se a execucao é em background
     for (j = 1; j < i-1; j++) {
        parametros[j] = tokens[j]; // exclui o "&" da lista de parametros
    }
    exec_in_background(programa,parametros);
  }
  else{
    for (j = 1; j < i; j++) {
        parametros[j] = tokens[j]; 
    }
    exec_in_foreground(programa,parametros);
  }
        if (strcmp(programa, "cd") == 0) {

      // nenhum parâmetro passado: manda para home
      if (parametros[1] == NULL){
        
        // chamando o trocador de diretórios e checando se houveram erros na execução
        if (chdir(getenv("HOME")) != 0)
          perror("Erro ao transferir para a home.\n");
      }
      else {
        if ((chdir(tokens[1]) != 0))
          perror("Erro ao transferir para o diretório.\n");
      }
      continue;
    }
   }
  }
  return 0;
}
