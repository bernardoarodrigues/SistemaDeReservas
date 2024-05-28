#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void aberturaVoo();
void realizarReserva();
void consultarReserva();
void modificarReserva();
void cancelarReserva();
void fechamentoDia();
void fechamentoVoo();

typedef struct Voo {
	char *idVoo, *origem, *destino; 
	int assentosTotais, assentosOcupados, dia, mes, ano, status;
	float valorEconomica, valorExecutiva, valorTotal;
} Voo;

typedef struct Reserva {
	char *nome, *sobrenome, *cpf, *assento, *classe; 
} Reserva;

void lerVoo(Voo *voo) {
    FILE *arquivoPtr;
    char arquivo[1000] = NULL;

    arquivoPtr = fopen("voo.txt", "r");
    fgets(arquivo, 1000, arquivoPtr);

    if(strlen(arquivo) > 0) {
        voo = calloc(1, sizeof(Voo));
        voo->idVoo = calloc(5, sizeof(char));
        voo->origem = calloc(4, sizeof(char));
        voo->destino = calloc(4, sizeof(char));

        strcpy(voo->idVoo, strtok(arquivo, ","));
        strcpy(voo->origem, strtok(NULL, ","));
        strcpy(voo->destino, strtok(NULL, ","));
        voo->assentosTotais = atoi(strtok(NULL, ","));
        voo->assentosOcupados = atoi(strtok(NULL, ","));
        voo->dia = atoi(strtok(NULL, ","));
        voo->mes = atoi(strtok(NULL, ","));
        voo->ano = atoi(strtok(NULL, ","));
        voo->status = atoi(strtok(NULL, ","));
        voo->valorEconomica = atof(strtok(NULL, ","));
        voo->valorExecutiva = atof(strtok(NULL, ","));
        voo->valorTotal = atof(strtok(NULL, ","));
    }
}

int main(void) {
    Voo voo;
    Reserva *reservas;

    lerVoo(&voo);

    char *comando[3];

    while(comando != "FD" || comando != "FV") {
        scanf(" %s ", comando);

        if(!strcmp(comando, "AV")) {
            aberturaVoo();
        } else if(!strcmp(comando, "RR")) {
            realizarReserva();
        } else if(!strcmp(comando, "CR")) {
            consultarReserva();
        } else if(!strcmp(comando, "MR")) {
            modificarReserva();
        } else if(!strcmp(comando, "CA")) {
            cancelarReserva();
        } else if(!strcmp(comando, "FD")) {
            fechamentoDia();
        } else if(!strcmp(comando, "FV")) {
            fechamentoVoo();
        }
    }

    return 0;
}