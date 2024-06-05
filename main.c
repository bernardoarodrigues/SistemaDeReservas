#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char TRACO[] = "--------------------------------------------------\n";

typedef struct {
	char *idVoo, *origem, *destino; 
	int assentosTotais, assentosOcupados, dia, mes, ano, status; // 0=aberto 1=fechado
	float valorEconomica, valorExecutiva, valorTotal;
} Voo;

typedef struct {
	char *nome, *sobrenome, *cpf, *assento;
    int classe; // 0=economica 1=executiva
} Reserva;

void aberturaVoo(Voo *voo, Reserva ***reservas);
void realizarReserva(Voo *voo, Reserva ***reservas);
void consultarReserva(Voo *voo, Reserva ***reservas);
void modificarReserva(Voo *voo, Reserva ***reservas);
void cancelarReserva(Voo *voo, Reserva ***reservas);
void fechamentoDia(Voo *voo);
void fechamentoVoo(Voo *voo, Reserva ***reservas);
void lerVoo(Voo *voo);
void salvarVoo(Voo *voo);
void lerReservas(Reserva*** reservas);
void salvarReservas(Voo *voo, Reserva** reservas);

void aberturaVoo(Voo *voo, Reserva ***reservas) {
    char *buffer = NULL;
    size_t tamanhoBuffer = 0;

    getline(&buffer, &tamanhoBuffer, stdin);

    voo->idVoo="-"; voo->origem="-"; voo->destino="-";
    voo->dia=-1; voo->mes=-1; voo->ano=-1;
    voo->assentosTotais=atoi(strtok(buffer, " ")); voo->assentosOcupados=0; voo->status=0;
    voo->valorEconomica=atoi(strtok(NULL, " ")); voo->valorExecutiva=atoi(strtok(NULL, "\n")); voo->valorTotal=0.0;

    for(int i=0; i<voo->assentosOcupados; i++) {
        free(&((*reservas)[i]->nome));
        free(&((*reservas)[i]->sobrenome));
        free(&((*reservas)[i]->cpf));
        free(&((*reservas)[i]->assento));
        free(&((*reservas)[i]));
    }
    free(*reservas);
    *reservas=NULL;
}

void realizarReserva(Voo *voo, Reserva ***reservas) {
    voo->assentosOcupados++;
    *reservas = (Reserva**)realloc(*reservas, voo->assentosOcupados*sizeof(Reserva));

    char *buffer = NULL;
    size_t tamanhoBuffer = 0;

    getline(&buffer, &tamanhoBuffer, stdin);

    Reserva *reserva = (*reservas)[voo->assentosOcupados-1];
    
    reserva->nome = calloc(50, sizeof(char));
    reserva->sobrenome = calloc(50, sizeof(char));
    reserva->cpf = calloc(15, sizeof(char));
    reserva->assento = calloc(10, sizeof(char));

    strcpy(reserva->nome, strtok(buffer, " "));  
    strcpy(reserva->sobrenome, strtok(NULL, " "));
    strcpy(reserva->cpf, strtok(NULL, " "));

    if(voo->dia==-1){
        voo->dia = atoi(strtok(NULL, " "));
        voo->mes, atoi(strtok(NULL, " "));
        voo->ano, atoi(strtok(NULL, " "));
        strcpy(voo->idVoo, strtok(NULL, " "));
    }
    else{
        strtok(NULL, " ");
        strtok(NULL, " ");
        strtok(NULL, " ");
        strtok(NULL, " ");
    }

    strcpy(reserva->assento, strtok(NULL, " "));
    if(!strcmp(strtok(NULL, " "), "economica")) reserva->classe=0;
    else reserva->classe = 1;
    strtok(NULL, " ");
    
    if(voo->origem=="-") {
        strcpy(voo->origem, strtok(NULL, " "));
        strcpy(voo->destino, strtok(NULL, "\n"));
    } else {
        strtok(NULL, " ");
        strtok(NULL, "\n");
    }
}

void consultarReserva(Voo *voo, Reserva ***reservas) {
    char cpf[15];
    scanf(" %s", cpf);
    
    for(int i = 0; i<voo->assentosOcupados; i++) {
        if(!strcmp(cpf, (*reservas)[i]->cpf)) {
            printf("%s\n", (*reservas)[i]->cpf);
            printf("%s %s\n", (*reservas)[i]->nome, (*reservas)[i]->sobrenome);
            if(voo->dia < 10) printf("0");
            printf("%d/", voo->dia);
            if(voo->mes < 10) printf("0");
            printf("%d/", voo->mes);
            printf("%d\n", voo->ano);
            printf("Voo: %s\n", voo->idVoo);
            printf("Assento: %s\n", (*reservas)[i]->assento);
            printf("Classe: %s\n", (*reservas)[i]->classe ? "economica" : "executiva");
            printf("Trecho: %s %s\n", voo->origem, voo->destino);
            printf("Valor: %.2f\n", (*reservas)[i]->classe ? voo->valorEconomica : voo->valorExecutiva);
            printf(TRACO);
            break;
        }
    }
}   

void modificarReserva(Voo *voo, Reserva ***reservas) {
    char *buffer = NULL;
    size_t tamanhoBuffer = 0;

    getline(&buffer, &tamanhoBuffer, stdin);

    for (int i=0; i<voo->assentosOcupados; i++) {
        if(!strcmp(strtok(buffer, " "), (*reservas)[i]->cpf)) {
            strcpy((*reservas)[i]->nome, strtok(NULL, " "));
            strcpy((*reservas)[i]->sobrenome, strtok(NULL, " "));
	        strcpy((*reservas)[i]->cpf, strtok(NULL, " "));
	        strcpy((*reservas)[i]->assento, strtok(NULL, "\n"));
            break;
        }
    }
}

void cancelarReserva(Voo *voo, Reserva ***reservas) {
    int i=0;

    char cpf[15];
    scanf(" %s", cpf);

    for (i=0; i<voo->assentosOcupados; i++){
        if(!strcmp(cpf, (*reservas)[i]->cpf)){
            break;
        } 
    }

    voo->valorTotal -= (*reservas)[i]->classe ? voo->valorEconomica : voo->valorExecutiva;
    
    for(int j=i; j<voo->assentosOcupados-1; j++){
        (*reservas)[j]=(*reservas)[j+1];
    }

    voo->assentosOcupados--;
    *reservas = (Reserva**)realloc(*reservas, voo->assentosOcupados*sizeof(Reserva*));
}

void fechamentoDia(Voo *voo) {
    printf("Fechamento do dia:\n");
    printf("Quantidade de reservas: %d\n", voo->assentosOcupados);
    printf("Posicao: %.2f\n", voo->valorTotal);
    printf(TRACO);
}

void fechamentoVoo(Voo *voo, Reserva ***reservas) {
    voo->status=1;
    printf("Voo Fechado!\n\n");
    for(int i=0; i<voo->assentosOcupados; i++) {
        printf("%s\n%s %s\n%s\n\n", (*reservas)[i]->cpf, (*reservas)[i]->nome, (*reservas)[i]->sobrenome, (*reservas)[i]->assento);
    }
    printf("Valor Total: %.2f\n", voo->valorTotal);
    printf(TRACO);
}

void lerVoo(Voo *voo) {
    FILE *arquivoPtr;
    char *arquivo = NULL;
    size_t tamanhoArquivo = 0;
    
    if((arquivoPtr = fopen("./voo.txt", "r")) != NULL) {
        int c = fgetc(arquivoPtr);
        if (c == EOF) {
            voo->idVoo = calloc(5, sizeof(char));
            voo->origem = calloc(4, sizeof(char));
            voo->destino = calloc(4, sizeof(char));
            voo->idVoo="-"; voo->origem="-"; voo->destino="-";
            voo->assentosTotais=-1; voo->assentosOcupados=-1; voo->dia=-1; voo->mes=-1; voo->ano=-1; voo->status=-1;
            voo->valorEconomica=-1.0; voo->valorExecutiva=-1.0; voo->valorTotal=-1.0;
        } else {
            rewind(arquivoPtr);
            if(getline(&arquivo, &tamanhoArquivo, arquivoPtr) != -1) {
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

    fclose(arquivoPtr);
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

        fclose(arquivoPtr);
    }
}

void lerReservas(Reserva ***reservas) {
    FILE *arquivoPtr;
    char *arquivo = NULL;
    size_t tamanhoArquivo = 0;
    
    int quantidade=0;

    if((arquivoPtr = fopen("./reservas.txt", "r")) != NULL) {
        int c = fgetc(arquivoPtr);
        if (c != EOF) {
            rewind(arquivoPtr);
            while(getline(&arquivo, &tamanhoArquivo, arquivoPtr) != -1) {
                quantidade++;
                
                *reservas = (Reserva**)realloc(*reservas, quantidade*sizeof(Reserva*));

                Reserva *reserva = (*reservas)[quantidade-1] = (Reserva*)calloc(1, sizeof(Reserva));

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
        }
        fclose(arquivoPtr);
    }
}

void salvarReservas(Voo *voo, Reserva **reservas) {
    FILE *arquivoPtr;

    int pos=0;

    if((arquivoPtr = fopen("./reservas.txt", "w")) != NULL) {
        while(pos < voo->assentosOcupados) {
            Reserva *reserva = reservas[pos];
            
            fprintf(arquivoPtr, reserva->nome); fprintf(arquivoPtr, ",");
            fprintf(arquivoPtr, reserva->sobrenome); fprintf(arquivoPtr, ",");
            fprintf(arquivoPtr, reserva->cpf); fprintf(arquivoPtr, ",");
            fprintf(arquivoPtr, reserva->assento); fprintf(arquivoPtr, ",");
            fprintf(arquivoPtr, "%d", reserva->classe); fprintf(arquivoPtr, "\n");
           
            pos++;
        }
        fclose(arquivoPtr);
    }
}

int main(void) {
    Voo voo;
    Reserva **reservas = NULL;

    lerVoo(&voo);
    lerReservas(&reservas);
    char comando[3] = "";

    while(strcmp(comando, "FD") && strcmp(comando, "FV")) {
        scanf(" %s", comando);

        if(!strcmp(comando, "AV")) {
            aberturaVoo(&voo, &reservas);
        } 
        else {
            if(voo.status) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF) { }
                fechamentoVoo(&voo, &reservas);
            }
            else {
                if(!strcmp(comando, "RR")) {
                    realizarReserva(&voo, &reservas);
                } else if(!strcmp(comando, "CR")) {
                    consultarReserva(&voo, &reservas);
                } else if(!strcmp(comando, "MR")) {
                    modificarReserva(&voo, &reservas);
                } else if(!strcmp(comando, "CA")) {
                    cancelarReserva(&voo, &reservas);
                } else if(!strcmp(comando, "FD")) {
                    fechamentoDia(&voo);
                } else if(!strcmp(comando, "FV")) {
                    fechamentoVoo(&voo, &reservas);
                }
            }
        }
    }

    salvarVoo(&voo);
    salvarReservas(&voo, reservas);

    for(int i=0; i<voo.assentosOcupados; i++) {
        free(&(reservas[i]->nome));
        free(&(reservas[i]->sobrenome));
        free(&(reservas[i]->cpf));
        free(&(reservas[i]->assento));
        free(&(reservas[i]));
    }
    free(reservas);
    free(voo.idVoo); free(voo.origem); free(voo.destino);
    reservas = NULL;

    return 0;
}