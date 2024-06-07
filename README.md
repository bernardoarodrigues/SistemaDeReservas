# Sistema de Reservas

## Descrição
Desenvolver um sistema de reserva de passagens, que possui uma série de funcionalidades (abrir um voo, reservar passagens, realizar consultas, cancelar passagens, etc) e requer o armazenamento dos dados gerados em arquivos para posterior utilização. A descrição completa está no [link](https://drive.google.com/file/d/1yVyazjBff_MNUbZtNK-_QDgeaY7r_EQS/view?usp=drive_link).

## Grupo
 - [Bernardo Alexandre Alves Rodrigues](mailto:bernardorodrigues@usp.br)
 - [Gabriel Phelippe Prado](mailto:gabriel.phelippe@usp.br)
 - [Gabriel Campanelli Iamato](mailto:gabriel.c.iamato@usp.br)

## Informações
 - **Disciplina:** Introdução à Ciência da Computação 1
 - **Professor:** Rudinei Goularte
 - **Curso:** Bacharelado em Ciência da Computação
 - **Turma:** BCC-A / 2024 

## Utilização
Para utilizar o programa, basta compilar o arquivo `main.c` com o comando `gcc main.c -o main -Wall -std=c99` e executar o arquivo gerado `./main`.

## Caso de Teste
### Entrada 1
AV 200 1200.00 2500.00\
RR Carlos Massa 555.555.333-99 12 12 2024 V001 A27 economica 1200.00 CGH RAO\
RR Maria Massa 444.555.333-93 12 12 2024 V001 A31 economica 1200.00 CGH RAO\
RR Roberto Carlos 555.333.333-89 12 12 2024 V001 P12 executiva 2500.00 CGH RAO\
MR 555.555.333-99 Carlos Massa 555.555.333-99 A30\
FD

### Entrada 2
RR Euclides Simon 222.111.333-12 12 12 2024 V001 B01 economica 1200.00 CGH RAO\
RR Marta Rocha 999.888.222-21 12 12 2024 V001 C02 executiva 2500.00 CGH RAO\
CR 555.333.333-89\
RR Clara Nunes 111.000.123-45 12 12 2024 V001 C09 executiva 2500.00 CGH RAO\
FV

## Licença
Esse projeto está licenciado sobre a GNU General Public License V3.0, publicada pela Free Software Foundation.