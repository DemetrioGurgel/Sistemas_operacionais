/* O arquivo .txt deve estar no mesmo diretorio para o funcionamento adequado da leitura do arquivo */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define QUANTUM 5
#define BUFFER_SIZE 255
#define limiteDeProcessos 100
#define incompleto 0 
#define finalizado 1 

const char token[2] = ";";

typedef struct {
    char ID[5];
    int DURATION;
    int PRIORITY;
    int STATUS;
    char PROGRAM[255];
    char USER[255];
} Process;


Process process[limiteDeProcessos];

void listaDeProcessos(Process *process, int swapA, int size) 
{
    Process aux = process[swapA];
    for(int i = swapA; i < size - 1; i ++)  
    {
        process[i] = process[i+1];
    }
    process[size - 1] = aux;
}

int main(int argc, char const *argv[])
{
    FILE *file;

    Process p;
    char buffer[BUFFER_SIZE];
    char *value;
    int count = 0;

    // Acessa arquivo txt
    // Acessa arquivo txt
		file = fopen("processos.txt", "r");
		
		if (file == NULL) {
		    printf("Erro ao abrir o arquivo\n");
		    return 1;
		}
		
		printf("Arquivo lido corretamente\n");


    printf("Iniciando escalonamento\n\n");
  
	while(fgets(buffer, BUFFER_SIZE, file)) 
	{
	    value = strtok(buffer, token);
	    strcpy(p.ID, value);
	    value = strtok(NULL, token);
	    p.DURATION = atoi(value);
	    value = strtok(NULL, token);
	    p.PRIORITY = atoi(value);
	    value = strtok(NULL, token);
	    strcpy(p.PROGRAM, value);
	    value = strtok(NULL, token);
	    strcpy(p.USER, value);
	    p.STATUS = incompleto;
	    process[count] = p;
	    count++;
	}

    //Fecha o arquivo txt
    fclose(file);

    int naoProcessados = count;
    int currentProcIndex = 0;

    Process *currentProc;

// Enquanto houver processos:
while(naoProcessados > 0) 
{
    int highestPriority = -1;
    int highestPriorityIndex = -1;
    // Seleciona o processo com a maior prioridade
    for(int i = 0; i < count; i++) 
    {
        if(process[i].STATUS == incompleto && process[i].PRIORITY > highestPriority) 
        {
            highestPriority = process[i].PRIORITY;
            highestPriorityIndex = i;
        }
    }

    if(highestPriorityIndex == -1) 
    {
        // Todos os processos foram concluídos
        break;
    }

    // Seleciona o processo com a maior prioridade e o executa
    currentProc = &process[highestPriorityIndex];
    printf("Processo atual: %s\n", currentProc->ID);
    printf("Tempo de execução restante: %i segundos\n", currentProc->DURATION);
    printf("Prioridade: %i\n", currentProc->PRIORITY);
    printf("Comando: %s\n", currentProc->PROGRAM);
		printf("Usuário: %s\n", currentProc->USER);

// Define o status do processo como "executando"
 if(currentProc->DURATION <= QUANTUM)
    {
        sleep(currentProc->DURATION);
        printf("Status: Processo %s completamente executado!\n\n", currentProc->ID);
      
        naoProcessados--;
        currentProc->STATUS = finalizado;
        currentProc->DURATION = 0;
    }
    else 
    {
        sleep(QUANTUM);
        printf("Status: Processo %s incompleto.\n\n", currentProc->ID);
        currentProc->DURATION -= QUANTUM;
        listaDeProcessos(process, highestPriorityIndex, count);
    }
}
    return 0;
}
