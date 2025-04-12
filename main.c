//ANOTAÇÕES GERAIS:
//No projeto main.c adicionamos o uso do printLinhaVermlha que seria uma função parar chamar a linha vermelha na cor vermelha.
//Usamos Cls e title parar maior organização e melhor estetica no console.
//Durante todo o projeto usamos o if and else como forma de verficação ou debug de erros, casos não saísse como esperávamos teriamos a resposta no console.
//Usamos o pause da system, parar pausar o console e facilitar a leitura do console pelo usuário.
//Possuímos as chamadas de funções na main.c parar exibição do resultados, eu luiz eduardo optei por desenvolver um layout mais organizado e melhor para a leitura.

//No processo.c já usamos bibliotecas diferentes como string.h para manipulação de strings, stdio.h usado para leitura de arquivos, time.h para manipular o tempo entre outras...
//Foi criada uma função dado o nome de csvleitura, que o papel dela é ler o arquivo csv como solicitado no Pdf enviado pelo professor.
//Usamos para ordernar processos por id, uma função em qsort, na qual separa os items por seus ids do csv.
//Temos a tramitação que foi usado a lógica matemática de: dia do processo, até o atual e contados os dias.
//Todos seguem basicamente uma linha de racíocionio onde seria leitura, escaneamento, tratamento, entrega e criação de arquivos/dados.
//Multiplos assuntos acabou sendo um dos mais dificeis por seu nível de complexidade, ele exigia a leitura através de ; do csv, acabamos quebrando um pouco a cabeça, porém no final acabamos usando o for com vetores, antes pelo sscanf estava dando erro.

//No processo.h foi feita a criação da estrutura do cabeçalho do arquivo, definindo as funções para seu funcionamento.
//Fizemos uma declaração das funções, pois os outros arquivos usam como se fosse uma conexão através do processo.h
//Ele irá organizar, alocar e centralizar as definições solicitadas no processo em geral, assim sendo facilitado o reuso em outros arquivos.

#include <stdio.h>
#include <windows.h>
#include "processo.h"

void printLinhaVermelha() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("==================================================\n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

int main() { 
    system("cls"); 
    system("title GERANDO RELATORIO"); 

    Processo processos[MAX_PROCESSOS];
    int total = csvleitura("processo_043_202409032338.csv", processos);

    if (total == 0) {
        printf("Documento não encontrado.\n");
        return 1;
    }

    printf("\n");
    printLinhaVermelha();
    printf("\nIDS ORDENADOS:\n");
    printf("Convertido e enviado para pasta local em .csv\n\n");
    csvid(processos, total);
    printLinhaVermelha();

    printf("\nDATAS ORDENADAS:\n");
    printf("Convertido e enviado para pasta local em .csv\n\n");
    csvdata(processos, total);
    printLinhaVermelha();

    printf("\nClassificados por classe:\n\n");
    classescont(processos, total);
    printf("\n");
    printLinhaVermelha();

    printf("\n");
    assuntounico(processos, total);
    printf("\n");
    printLinhaVermelha();

    multiploassuntos(processos, total);
    printf("\n");
    printLinhaVermelha();
    
    printf("\nDias em tramitacao:\n");
    tramitacao(processos, total);
    printf("\n");
    printLinhaVermelha();

    system("title Relatorio 2025"); 
    system("pause"); 
    return 0;
}
