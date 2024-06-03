#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char *idVoo, *origem, *destino; 
	int assentosTotais, assentosOcupados, dia, mes, ano, status;
	float valorEconomica, valorExecutiva, valorTotal;
} Voo;

typedef struct {
	char *nome, *sobrenome, *cpf, *assento;
    int classe; // 0=economica 1=executiva
} Reserva;

void aberturaVoo();
void realizarReserva();
void consultarReserva();
void modificarReserva();
void cancelarReserva();
void fechamentoDia();
void fechamentoVoo();
void lerVoo(Voo* voo);
void salvarVoo(Voo* voo);
void lerReservas(Reserva** reservas);
void salvarReservas(Reserva** reservas);

void aberturaVoo() {
    
}
void realizarReserva() {

}
void consultarReserva() {

}
void modificarReserva() {
    
}
void cancelarReserva() {

}
void fechamentoDia() {

}
void fechamentoVoo() {

}

void lerVoo(Voo *voo) {
    FILE *arquivoPtr;
    char arquivo[1000];

    if((arquivoPtr = fopen("./voo.txt", "r")) != NULL) {
        if(fgets(arquivo, 1000, arquivoPtr)) {
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
}

void salvarVoo(Voo *voo) {
    FILE *arquivoPtr;

    if((arquivoPtr = fopen("./voo.txt", "w")) != NULL) {
        fprintf(arquivoPtr, voo->idVoo); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, voo->origem); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, voo->destino); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%d", voo->assentosTotais); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%d", voo->assentosOcupados); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%d", voo->dia); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%d", voo->mes); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%d", voo->ano); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%d", voo->status); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%.2f", voo->valorEconomica); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%.2f", voo->valorExecutiva); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%.2f", voo->valorTotal);
    }
}

int contarReservas() {
    FILE *arquivoPtr;
    char arquivo[1000];
    int linhas=0;

    if((arquivoPtr = fopen("./reservas.txt", "r")) != NULL) {
        while(!feof(arquivoPtr)) {
            char ch = fgetc(arquivoPtr);
            if(ch == '\n') {
                linhas++;
            }
        }
    }

    return linhas;
}

Reserva** lerReservas(Reserva **reservas) {
    FILE *arquivoPtr;
    char arquivo[1000];
    
    int quantidade=0;

    if((arquivoPtr = fopen("./reservas.txt", "r")) != NULL) {
        while(fgets(arquivo, 1000, arquivoPtr)) {
            quantidade++;
            
            reservas = (Reserva**)realloc(reservas, quantidade*sizeof(Reserva*));

            Reserva *reserva = reservas[quantidade-1] = (Reserva*)calloc(1, sizeof(Reserva));

            reserva->nome = calloc(50, sizeof(char));
            reserva->sobrenome = calloc(50, sizeof(char));
            reserva->cpf = calloc(15, sizeof(char));
            reserva->assento = calloc(10, sizeof(char));

            strcpy(reserva->nome, strtok(arquivo, ","));
            strcpy(reserva->sobrenome, strtok(NULL, ","));
            strcpy(reserva->cpf, strtok(NULL, ","));
            strcpy(reserva->assento, strtok(NULL, ","));
            reserva->classe = atoi(strtok(NULL, ","));

            reserva->nome = realloc(reserva->nome, (strlen(reserva->nome)+1)*sizeof(char));
            reserva->sobrenome = realloc(reserva->sobrenome, (strlen(reserva->sobrenome)+1)*sizeof(char));
            reserva->assento = realloc(reserva->assento, (strlen(reserva->assento)+1)*sizeof(char));
        }
        return reservas;
    }
    
    return NULL;
}

void salvarReservas(Reserva **reservas) {
    FILE *arquivoPtr;

    int quantidade = contarReservas(), pos=0;

    if((arquivoPtr = fopen("./reservas.txt", "w")) != NULL) {
        while(pos < quantidade) {
            Reserva *reserva = reservas[pos];
            
            fprintf(arquivoPtr, reserva->nome); fprintf(arquivoPtr, ",");
            fprintf(arquivoPtr, reserva->sobrenome); fprintf(arquivoPtr, ",");
            fprintf(arquivoPtr, reserva->cpf); fprintf(arquivoPtr, ",");
            fprintf(arquivoPtr, reserva->assento); fprintf(arquivoPtr, ",");
            fprintf(arquivoPtr, "%d", reserva->classe); fprintf(arquivoPtr, "\n");
           
            pos++;
        }
    }
}

int main(void) {
    Voo *voo = (Voo*)calloc(1, sizeof(Voo));
    Reserva **reservas;

    lerVoo(voo);
    salvarVoo(voo);
    reservas = lerReservas(reservas);
    salvarReservas(reservas);

    char comando[3];

    while(strcmp(comando, "FD") || strcmp(comando, "FV")) {
        scanf(" %s ", comando);

        if(!strcmp(comando, "AV")) {
            aberturaVoo(voo, reservas);
        } else if(!strcmp(comando, "RR")) {
            realizarReserva(voo, reservas);
        } else if(!strcmp(comando, "CR")) {
            consultarReserva(voo, reservas);
        } else if(!strcmp(comando, "MR")) {
            modificarReserva(voo, reservas);
        } else if(!strcmp(comando, "CA")) {
            cancelarReserva(voo, reservas);
        } else if(!strcmp(comando, "FD")) {
            fechamentoDia(voo, reservas);
        } else if(!strcmp(comando, "FV")) {
            fechamentoVoo(voo, reservas);
        }
    }

    return 0;
}