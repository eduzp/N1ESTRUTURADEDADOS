#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_ASSUNTOS 10
#define MAX_PROCESSOS 1000

typedef struct {
    int id;
    char data_ajuizamento[11];
    int id_classe;
    int id_assuntos[MAX_ASSUNTOS];
    int qtd_assuntos;
} Processo;

int csvleitura(const char* nome_arquivo, Processo* processos);
void csvid(Processo* processos, int tamanho);
void csvdata(Processo* processos, int tamanho);
void csvcriar(const char* nome_arquivo, Processo* processos, int tamanho);
void classescont(Processo* processos, int tamanho);
void assuntounico(Processo* processos, int tamanho);
void multiploassuntos(Processo* processos, int tamanho);
void tramitacao(Processo* processos, int tamanho);

#endif
