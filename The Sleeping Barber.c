#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

//MAX representa o número de cadeiras da sala de espera
#define MAX 3

//Utiliza-se MAX+1, pois deve-se levar em conta a cadeira do barbeiro
int begin = 0, end = 0, size = 0, fila[MAX+1];
pthread_mutex_t travaFila;

int vazio(){
    return !size;
}

void enqueue(int cliente){
    if (size < MAX+1){
        if (vazio())
            printf("O cliente %d acorda o barbeiro e senta em sua cadeira.\n", cliente);
        else
            printf("O cliente %d senta em uma das cadeiras vagas da sala de espera.\n", cliente);

        fila[end] = cliente;
        end = (end+1) % (MAX+1);
        size++;
    }else
        printf("Todas as cadeiras estavam ocupadas, o cliente %d foi embora.\n", cliente);
}

int denqueue(){
    if (!vazio()){
        srand(time(NULL));
        sleep(rand() % 4 + 1); //Intervalo de tempo que o barbeiro demora para cortar o cabelo do cliente
        printf("O barbeiro termina de cortar o cabelo do cliente %d, que vai embora.\n", fila[begin]);

        int cliente = fila[begin];
        begin = (begin+1) % (MAX+1);
        size--;

        if (vazio())
            puts("Nao ha clientes para serem atendidos, o barbeiro vai dormir.");
        else
            printf("O cliente %d senta na cadeira do barbeiro.\n", fila[begin]);

        return cliente;
    }else
        //O barbeiro está dormindo, portanto nada ocorre e a função retorna -1 como código de erro
        return -1;
}

void* cliente(void* argumento){
    unsigned long idCliente = *((unsigned long*) argumento);

    pthread_mutex_trylock(&travaFila);
    enqueue(idCliente);
    pthread_mutex_unlock(&travaFila);
}

void* barbeiro(void* argumento){
    for ( ; ; ){
        pthread_mutex_trylock(&travaFila);
        denqueue();
        pthread_mutex_unlock(&travaFila);
    }
}

int main(){
    pthread_mutex_init(&travaFila, NULL);

    int i;
    pthread_t threadBarbeiro, threadCliente;

    pthread_create(&threadBarbeiro, NULL, barbeiro, NULL);
    srand(time(NULL));
    for (i = 0 ; ; ++i){
        sleep(rand() % 4 + 1); //Intervalo de tempo para a chegada de clientes
        pthread_create(&threadCliente, NULL, cliente, &i);
    }

    return 0;
}