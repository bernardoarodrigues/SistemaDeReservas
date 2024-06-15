/*
    Autores: 
        - Bernardo Alexandre Alves Rodrigues - N° USP: 15442936
        - Gabriel Campanelli Iamato - N° USP: 15452920
        - Gabriel Phelippe Prado - N° USP: 15453730
*/

// Define (habilita getline() para std=c99)
// Obs.: optamos pelo getline() para não precisar de especificar um tamanho limite de caracteres ao ler a entrada do usuário ou os arquivos salvos
// Sintaxe: getline(char **restrict bufferSaida, size_t *restrict quantidadeDeCaracteres, FILE *restrict bufferEntrada);
// A função basicamente lê o buffer de entrada (stdin ou ponteiro para arquivo, no caso desse programa), caractere por caractere, até a quebra de linha (\n) ou fim do arquivo (EOF).
// Além disso, ela armazena os caracteres lidos em um buffer de saída, que é realocado a cada novo caractere.
#define _GNU_SOURCE

// Bibliotecas
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

// Constantes
const char LINHA[] = "--------------------------------------------------\n";

/*
Struct referente ao voo
    - idVoo: Identificador do voo
    - origem: Aeroporto de origem do voo
    - destino: Aeroporto de destino do voo
    - assentosTotais: Quantidade de assentos no total
    - assentosOcupados: Quantidade de assentos ocupados
    - dia: Dia do voo
    - mes: Mês do voo
    - ano: Ano do voo
    - status: 0 = Voo aberto, 1 = Voo fechado
    - valorEconomica: Valor da passagem na classe econômica
    - valorExecutiva: Valor da passagem na classe executiva
    - valorTotal: Valor total arrecadado com as reservas
*/
typedef struct {
	char *idVoo, *origem, *destino; 
	int assentosTotais, assentosOcupados, dia, mes, ano, status; 
	float valorEconomica, valorExecutiva, valorTotal;
} Voo;

/*
Struct referente às reservas
    - nome: Primeiro nome do passageiro
    - sobrenome: Sobrenome do passageiro
    - cpf: CPF do passageiro
    - assento: Id do assento reservado
    - classe: 0 = Econômica, 1 = Executiva
*/
typedef struct {
	char *nome, *sobrenome, *cpf, *assento;
    int classe; 
} Reserva;

// Protótipos das funções
void aberturaVoo(Voo *voo, Reserva ***reservas);
void realizarReserva(Voo *voo, Reserva ***reservas);
void consultarReserva(Voo *voo, Reserva ***reservas);
void modificarReserva(Voo *voo, Reserva ***reservas);
void cancelarReserva(Voo *voo, Reserva ***reservas);
void fechamentoDia(Voo *voo);
void fechamentoVoo(Voo *voo, Reserva ***reservas);
void lerVoo(Voo *voo);
void salvarVoo(Voo *voo);
void lerReservas(Reserva ***reservas);
void salvarReservas(Voo *voo, Reserva ***reservas);

/*
    Objetivo: realizar a abertura de um voo, armazenando em uma struct Voo
    Parâmetros: endereço da struct Voo, endereço do array de Reservas
*/
void aberturaVoo(Voo *voo, Reserva ***reservas) {
    // Obtém parâmetros
    char *buffer = NULL;
    size_t tamanhoBuffer = 0;
    getline(&buffer, &tamanhoBuffer, stdin);

    // Ignora espaço após o comando
    buffer++;

    // Atribui os valores da struct Voo de acordo com os valores inicializados (string com "-" e inteiros com -1)
    strcpy(voo->idVoo, "-"); strcpy(voo->origem, "-"); strcpy(voo->destino, "-");
    voo->dia=-1; voo->mes=-1; voo->ano=-1;

    // Atribui total de assentos e valores das classes, reseta assentos ocupados e total arrecadado e inicializa status como 0 (aberto)
    voo->assentosTotais=atoi(strtok(buffer, " ")); 
    voo->valorEconomica=atoi(strtok(NULL, " ")); 
    voo->valorExecutiva=atoi(strtok(NULL, "\n")); 
    voo->assentosOcupados=0; 
    voo->valorTotal=0.0;
    voo->status=0;

    // Desaloca memória e reseta array de reservas
    if(*reservas != NULL) {
        for(int i=0; i<voo->assentosOcupados; i++) {
            free((*reservas)[i]->nome);
            free((*reservas)[i]->sobrenome);
            free((*reservas)[i]->cpf);
            free((*reservas)[i]->assento);
            free((*reservas)[i]);
        }
        free(*reservas);
        *reservas=NULL;
    }
}

/*
    Objetivo: realizar a reserva de um assento, armazenando em um espaço do array de structs reservas
    Parâmetros: endereço da struct Voo, endereço do array de Reservas
*/
void realizarReserva(Voo *voo, Reserva ***reservas) {
    // Obtém parâmetros
    char *buffer = NULL;
    size_t tamanhoBuffer = 0;
    getline(&buffer, &tamanhoBuffer, stdin);

    // Ignora espaço após o comando
    buffer++;

    // Incrementa o número de assentos ocupados e realoca o espaço de memória para o array de reservas
    voo->assentosOcupados++;
    *reservas = (Reserva**)realloc(*reservas, voo->assentosOcupados*sizeof(Reserva*));
    
    // Armazena espaço para reserva
    Reserva *reserva = (Reserva*) calloc(1, sizeof(Reserva));

    // Aloca memória para strings da reserva
    reserva->nome = calloc(50, sizeof(char));
    reserva->sobrenome = calloc(50, sizeof(char));
    reserva->cpf = calloc(15, sizeof(char));
    reserva->assento = calloc(10, sizeof(char));

    // Obtém as informações do buffer e armazena nos campos da struct reserva
    strcpy(reserva->nome, strtok(buffer, " "));  
    strcpy(reserva->sobrenome, strtok(NULL, " "));
    strcpy(reserva->cpf, strtok(NULL, " "));

    // Realoca memória utilizada nas strings de acordo com seu tamanho, para evitar uso desnecessário de memória
    reserva->nome = realloc(reserva->nome, (strlen(reserva->nome)+1)*sizeof(char));
    reserva->sobrenome = realloc(reserva->sobrenome, (strlen(reserva->sobrenome)+1)*sizeof(char));
    reserva->assento = realloc(reserva->assento, (strlen(reserva->assento)+1)*sizeof(char));
    
    // Altera os valores do dia, mês, ano e idVoo caso ainda não tenham sido obtidos
    if(voo->dia==-1){
        voo->dia = atoi(strtok(NULL, " "));
        voo->mes = atoi(strtok(NULL, " "));
        voo->ano = atoi(strtok(NULL, " "));
        strcpy(voo->idVoo, strtok(NULL, " "));
    }
    // Caso contrário, ignora-os
    else{
        strtok(NULL, " ");
        strtok(NULL, " ");
        strtok(NULL, " ");
        strtok(NULL, " ");
    }

    // Recebe os valores de assento e classe e atualiza o valor total obtido
    strcpy(reserva->assento, strtok(NULL, " "));
    if(!strcmp(strtok(NULL, " "), "economica")) {
        reserva->classe = 0;
        voo->valorTotal += voo->valorEconomica;
    }
    else {
        reserva->classe = 1;
        voo->valorTotal += voo->valorExecutiva;
    }

    // Ignora o valor da reserva, pois já será calculado no final de acordo com a classe
    strtok(NULL, " ");

    // Recebe os valores de origem e destino, caso ainda não tenham sido recebidos
    if(!strcmp(voo->origem, "-")) {
        strcpy(voo->origem, strtok(NULL, " "));
        strcpy(voo->destino, strtok(NULL, "\n"));
    }
    // Caso contrário, ignora-os
    else {
        strtok(NULL, " ");
        strtok(NULL, "\n");
    }

    // Adiciona reserva ao array
    (*reservas)[voo->assentosOcupados-1] = reserva;
}

/*
    Objetivo: consultar a reserva de um assento pelo CPF e imprimir suas informações
    Parâmetros: endereço da struct Voo, endereço do array de Reservas
*/
void consultarReserva(Voo *voo, Reserva ***reservas) {
    // Obtém CPF
    char cpf[15];
    scanf(" %s", cpf);

    // Percorre o array de reservas
    for(int i = 0; i<voo->assentosOcupados; i++) {
        // Caso tenha encontrado o CPF, imprime as informações da reserva
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
            printf("Classe: %s\n", (*reservas)[i]->classe ? "executiva" : "economica");
            printf("Trecho: %s %s\n", voo->origem, voo->destino);
            printf("Valor: %.2f\n", (*reservas)[i]->classe ? voo->valorExecutiva : voo->valorEconomica);
            printf(LINHA);
            break;
        }
    }
}   

/*
    Objetivo: modificar a reserva de um assento, alterando as informações da reserva
    Parâmetros: endereço da struct Voo, endereço do array de Reservas
*/
void modificarReserva(Voo *voo, Reserva ***reservas) {
    // Obtém parâmetros
    char *buffer = NULL;
    size_t tamanhoBuffer = 0;
    getline(&buffer, &tamanhoBuffer, stdin);
    
    // Ignora espaço após o comando
    buffer++;

    // Percorre o array de reservas
    int pos=0;
    for (; pos < voo->assentosOcupados; pos++) {
        // Caso tenha encontrado o CPF, modifica as informações da reserva
        if(!strcmp(strtok(buffer, " "), (*reservas)[pos]->cpf)) {
            strcpy((*reservas)[pos]->nome, strtok(NULL, " "));
            strcpy((*reservas)[pos]->sobrenome, strtok(NULL, " "));
	        strcpy((*reservas)[pos]->cpf, strtok(NULL, " "));
	        strcpy((*reservas)[pos]->assento, strtok(NULL, "\n"));
            break;
        }
    }

    // Imprime as informações da reserva modificada
    printf("Reserva Modificada:\n%s\n%s %s\n%d/%d/%d\nVoo: %s\nAssento: %s\n", (*reservas)[pos]->cpf, (*reservas)[pos]->nome, (*reservas)[pos]->sobrenome, (voo->dia), (voo->mes), (voo->ano), voo->idVoo, (*reservas)[pos]->assento);
    if((*reservas)[pos]->classe == 1) printf("Classe: executiva\n"); 
    else printf("Classe: economica\n");
    printf("Trecho: %s %s\nValor: %.2f\n", voo->origem, voo->destino, (*reservas)[pos]->classe ? voo->valorExecutiva : voo->valorEconomica);
    printf(LINHA);
}

/*
    Objetivo: cancelar a reserva de um assento, retirando do array de reservas
    Parâmetros: endereço da struct Voo, endereço do array de Reservas
*/
void cancelarReserva(Voo *voo, Reserva ***reservas) {
    // Obtém CPF
    char cpf[15];
    scanf(" %s", cpf);

    // Procura o cpf no array de reservas e obtém sua posição
    int pos=0;
    for (; pos<voo->assentosOcupados; pos++){
        if(!strcmp(cpf, (*reservas)[pos]->cpf)){
            break;
        } 
    }
    
    // Diminui do valor total do Voo o valor da reserva cancelada
    voo->valorTotal -= (*reservas)[pos]->classe ? voo->valorEconomica : voo->valorExecutiva;
    
    // Desloca as posições posteriores do array de reservas para a posição anterior, de forma a remover a reserva cancelada
    for(int j=pos; j<voo->assentosOcupados-1; j++){
        (*reservas)[j]=(*reservas)[j+1];
    }

    // Diminui em 1 a quantidade de assentos ocupados
    voo->assentosOcupados--;

    // Realoca o espaço para as reservas
    *reservas = (Reserva**)realloc(*reservas, voo->assentosOcupados*sizeof(Reserva*));
}

/*
    Objetivo: imprimir o fechamento do dia, com a quantidade de reservas e o valor total arrecadado
    Parâmetros: endereço da struct Voo, endereço do array de Reservas
*/
void fechamentoDia(Voo *voo) {
    printf("Fechamento do dia:\n");
    printf("Quantidade de reservas: %d\n", voo->assentosOcupados);
    printf("Posicao: %.2f\n", voo->valorTotal);
    printf(LINHA);
}

/*
    Objetivo: imprimir o fechamento do voo, com as informações de todas as reservas e o valor total arrecadado
    Parâmetros: endereço da struct Voo, endereço do array de Reservas
*/
void fechamentoVoo(Voo *voo, Reserva ***reservas) {
    // Define o status do voo como fechado
    voo->status=1;

    // Imprime informações do voo
    printf("Voo Fechado!\n\n");
    for(int i=0; i<voo->assentosOcupados; i++) {
        printf("%s\n%s %s\n%s\n\n", (*reservas)[i]->cpf, (*reservas)[i]->nome, (*reservas)[i]->sobrenome, (*reservas)[i]->assento);
    }
    printf("Valor Total: %.2f\n", voo->valorTotal);
    printf(LINHA);
}

/*
    Objetivo: obter as informações do arquivo "voo.txt" e armazenar na struct Voo
    Parâmetros: endereço da struct Voo
*/
void lerVoo(Voo *voo) {
    // Cria um ponteiro para o arquivo
    FILE *arquivoPtr;
    char *linha = NULL;
    size_t tamanhoLinha = 0;

    // Abre o arquivo para leitura
    if((arquivoPtr = fopen("./voo.txt", "r")) != NULL) {
        // Obtém primeiro caractere do arquivo
        int c = fgetc(arquivoPtr);

        /* Se o arquivo estiver vazio, define as informações da struct Voo como 
        "-": para strings
        -1: para inteiros
        -1.0: para floats
        Obs.: valores de inicialização, que serão sobrescritos posteriormente */
        if (c == EOF) {
            voo->idVoo = calloc(5, sizeof(char));
            voo->origem = calloc(4, sizeof(char));
            voo->destino = calloc(4, sizeof(char));
            strcpy(voo->idVoo, "-"); strcpy(voo->origem, "-"); strcpy(voo->destino, "-");
            voo->assentosTotais=-1; voo->assentosOcupados=-1; voo->dia=-1; voo->mes=-1; voo->ano=-1; voo->status=-1;
            voo->valorEconomica=-1.0; voo->valorExecutiva=-1.0; voo->valorTotal=-1.0;
        } 
        // Caso o arquivo exista
        else { 
            // Reposiciona o ponteiro para o início do arquivo
            rewind(arquivoPtr);

            // Obtém as informações do voo e armazena na struct Voo
            if(getline(&linha, &tamanhoLinha, arquivoPtr) != -1) {
                // Aloca memória para strings
                voo->idVoo = calloc(5, sizeof(char));
                voo->origem = calloc(4, sizeof(char));
                voo->destino = calloc(4, sizeof(char));

                // Obtém as informações separadas por ","
                strcpy(voo->idVoo, strtok(linha, ","));
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

        // Fecha o arquivo
        fclose(arquivoPtr);
    } 
    // Caso o arquivo não exista, cria um novo e define as informações iniciais
    else {
        if((arquivoPtr = (FILE*)fopen("./voo.txt", "w")) == NULL) {
            printf("Erro ao criar o arquivo voo.txt\n");
            exit(1);
        } 

        /* Se o arquivo estiver vazio, define as informações da struct Voo como 
        "-": para strings
        -1: para inteiros
        -1.0: para floats
        Obs.: valores de inicialização, que serão sobrescritos posteriormente */
        voo->idVoo = calloc(5, sizeof(char));
        voo->origem = calloc(4, sizeof(char));
        voo->destino = calloc(4, sizeof(char));
        strcpy(voo->idVoo, "-"); strcpy(voo->origem, "-"); strcpy(voo->destino, "-");
        voo->assentosTotais=-1; voo->assentosOcupados=-1; voo->dia=-1; voo->mes=-1; voo->ano=-1; voo->status=-1;
        voo->valorEconomica=-1.0; voo->valorExecutiva=-1.0; voo->valorTotal=-1.0;
    }
}

/*
    Objetivo: salvar as informações da struct Voo no arquivo "voo.txt"
    Parâmetros: endereço da struct Voo
*/
void salvarVoo(Voo *voo) {
    // Cria um ponteiro para o arquivo
    FILE *arquivoPtr;

    // Abre o arquivo voo.txt no modo escrita
    if((arquivoPtr = (FILE*)fopen("./voo.txt", "w")) != NULL) {
        // Armazena as informações separando-as por ","
        fprintf(arquivoPtr,"%s", voo->idVoo); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr,"%s", voo->origem); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr,"%s", voo->destino); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%d", voo->assentosTotais); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%d", voo->assentosOcupados); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%d", voo->dia); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%d", voo->mes); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%d", voo->ano); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%d", voo->status); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%.2f", voo->valorEconomica); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%.2f", voo->valorExecutiva); fprintf(arquivoPtr, ",");
        fprintf(arquivoPtr, "%.2f", voo->valorTotal);

        // Fecha arquivo
        fclose(arquivoPtr);
    } 
    // Caso a operação não tenha sucedido, alerta erro
    else {
        printf("Erro ao abrir o arquivo voo.txt\n");
        exit(1);
    }
}

/*
    Objetivo: pegar as informações do arquivo "reservas.txt" e armazenar em um array de Reservas
    Parâmetros: endereço do array de Reservas
*/
void lerReservas(Reserva ***reservas) {
    // Cria um ponteiro do arquivo
    FILE *arquivoPtr;
    char *arquivo = NULL;
    size_t tamanhoArquivo = 0;

    // Quantidade de reservas
    int quantidade=0;

    // Caso o arquivo exista, abre-o arquivo para leitura
    if((arquivoPtr = (FILE*)fopen("./reservas.txt", "r")) != NULL) {
        int c = fgetc(arquivoPtr);

        // Se o arquivo não estiver vazio
        if (c != EOF) {
            // Retorna o ponteiro para a posição inicial
            rewind(arquivoPtr);

            // Obtém as reservas e armazena-as no array 
            while(getline(&arquivo, &tamanhoArquivo, arquivoPtr) != -1) {
                // Incrementa contador
                quantidade++;

                // Realoca array de reservas
                *reservas = (Reserva**)realloc(*reservas, quantidade*sizeof(Reserva*));
                Reserva *reserva = (*reservas)[quantidade-1] = (Reserva*)calloc(1, sizeof(Reserva));

                // Armazena memória para as strings da reserva
                reserva->nome = calloc(50, sizeof(char));
                reserva->sobrenome = calloc(50, sizeof(char));
                reserva->cpf = calloc(15, sizeof(char));
                reserva->assento = calloc(10, sizeof(char));

                // Por meio do strtok, separa as informações obtidas e armazena-as nos campos da struct reserva
                strcpy(reserva->nome, strtok(arquivo, ","));
                strcpy(reserva->sobrenome, strtok(NULL, ","));
                strcpy(reserva->cpf, strtok(NULL, ","));
                strcpy(reserva->assento, strtok(NULL, ","));
                reserva->classe = atoi(strtok(NULL, ","));

                // Realoca o espaço de memória para os campos de cada struct Reserva de acordo com o tamanho da string, para evitar uso desnecessário de memória
                reserva->nome = realloc(reserva->nome, (strlen(reserva->nome)+1)*sizeof(char));
                reserva->sobrenome = realloc(reserva->sobrenome, (strlen(reserva->sobrenome)+1)*sizeof(char));
                reserva->assento = realloc(reserva->assento, (strlen(reserva->assento)+1)*sizeof(char));
            }
        }
    } 
    // Caso o arquivo não exista, cria um novo
    else {
        if((arquivoPtr = (FILE*)fopen("./reservas.txt", "w")) == NULL) {
            printf("Erro ao criar o arquivo reservas.txt\n");
            exit(1);
        }
    }

    // Fecha o arquivo
    fclose(arquivoPtr);
}

/*
    Objetivo: salvar as informações do array de Reservas no arquivo "reservas.txt"
    Parâmetros: endereço da struct Voo, endereço do array de Reservas
*/
void salvarReservas(Voo *voo, Reserva ***reservas) {
    // Abre o arquivo "reservas.txt" no modo escrita
    FILE *arquivoPtr;
    if((arquivoPtr = fopen("./reservas.txt", "w")) != NULL) {
        // Armazena as informações de cada reserva e separando-as por ",". Na última, imprime um "\n" para a próxima reserva.
        for(int pos=0; pos < voo->assentosOcupados; pos++) {
            Reserva *reserva = (*reservas)[pos];
            
            fprintf(arquivoPtr,"%s",  reserva->nome); fprintf(arquivoPtr, ",");
            fprintf(arquivoPtr,"%s",  reserva->sobrenome); fprintf(arquivoPtr, ",");
            fprintf(arquivoPtr,"%s",  reserva->cpf); fprintf(arquivoPtr, ",");
            fprintf(arquivoPtr,"%s",  reserva->assento); fprintf(arquivoPtr, ",");
            fprintf(arquivoPtr, "%d", reserva->classe); fprintf(arquivoPtr, "\n");
        }

        // Fecha o arquivo
        fclose(arquivoPtr);
    } 
    // Caso a operação não tenha sucedido, alerta erro
    else {
        printf("Erro ao criar o arquivo reservas.txt\n");
        exit(1);
    }
}

/* 
    Objetivo: Ler informações gerais do voo e armazená-las em uma struct e ler as informações das reservas e armazená-las em um array de structs.
    Após isso, realizar as operações ditadas pelos comandos alterando os valores da struct Voo e do array de Reservas.
    Ao realizar o fechamento do dia ou o fechamento do voo, salvar essas informações em arquivos de texto.
    Caso o voo tenha sido fechado, libera somente comandos para Consultar Reserva ou para Fechamento do Voo. 
*/
int main(void) {
    // Cria um voo e um vetor de reservas
    Voo voo;
    Reserva **reservas = NULL;

    // Lê as informações do voo e das reservas
    lerVoo(&voo);
    lerReservas(&reservas); 

    // Caso o voo já esteja fechado, imprime informações relativas ao voo
    if(voo.status == 1) {
        fechamentoVoo(&voo, &reservas);
    }

    // Obtém e executa comandos até Fechamento do Dia ou Fechamento do Voo
    char comando[3] = "";
    while(strcmp(comando, "FD") && strcmp(comando, "FV")) {
        // Recebe o comando: (AV, RR, CR, MR, CA, FD, FV)
        scanf(" %s", comando);

        // Caso o voo esteja fechado, libera execução somente de "CR" e "FV"
        if(voo.status == 1) { 
            if(!strcmp(comando, "CR")) {
                consultarReserva(&voo, &reservas);
            }
            else if(!strcmp(comando, "FV")) {
                fechamentoVoo(&voo, &reservas);
            } else {
                // Termina de ler os parâmetros do comando inválido
                int c;
                while ((c = getchar()) != '\n' && c != EOF) {}
            }
        } 
        // Caso o voo esteja aberto, libera a execução dos seguintes comandos 
        else {
            if(!strcmp(comando, "AV")) {
                aberturaVoo(&voo, &reservas);
            } else if(!strcmp(comando, "CR")) {
                consultarReserva(&voo, &reservas);
            } else if(!strcmp(comando, "RR")) {
                realizarReserva(&voo, &reservas);
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

    // Armazena as informações do voo e das reservas nos arquivos "voo.txt" e "reservas.txt", respectivamente
    salvarVoo(&voo);
    salvarReservas(&voo, &reservas);

    // Libera a memória alocada
    for(int i=0; i<voo.assentosOcupados; i++) {
        free(reservas[i]->nome);
        free(reservas[i]->sobrenome);
        free(reservas[i]->cpf);
        free(reservas[i]->assento);
        free(reservas[i]);
    }
    free(reservas);
    free(voo.idVoo); free(voo.origem); free(voo.destino);
    reservas = NULL;

    return 0;
}