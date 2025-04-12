#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

int csvleitura(const char* nome_arquivo, Processo* processos) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo");
        return 0;
    }

    char linha[1024];
    int tamanho = 0;

    fgets(linha, sizeof(linha), arquivo); 

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = '\0';
        char* campos[4];
        int i = 0;

        char* temp = strtok(linha, ",");
        while (temp != NULL && i < 4) {
            campos[i++] = temp;
            temp = strtok(NULL, ",");
        }

        if (i < 4) continue;

        Processo p;
        p.id = atoi(campos[0]);
        strncpy(p.data_ajuizamento, campos[1], 10);
        p.data_ajuizamento[10] = '\0';
        p.id_classe = atoi(campos[2]);

        p.qtd_assuntos = 0;
        char* assuntos_raw = campos[3];
        if (assuntos_raw[0] == '{') {
            assuntos_raw++;
            char* token = strtok(assuntos_raw, ",");
            while (token != NULL && p.qtd_assuntos < MAX_ASSUNTOS) {
                char* fechar = strchr(token, '}');
                if (fechar) *fechar = '\0';

                p.id_assuntos[p.qtd_assuntos++] = atoi(token);
                token = strtok(NULL, ",");
            }
        }

        processos[tamanho++] = p;
        if (tamanho >= MAX_PROCESSOS) break;
    }

    fclose(arquivo);
    return tamanho;
}

int id(const void* a, const void* b) {
    return ((Processo*)a)->id - ((Processo*)b)->id;
}

int data(const void* a, const void* b) {
    return strcmp(((Processo*)b)->data_ajuizamento, ((Processo*)a)->data_ajuizamento);
}

void classescont(Processo* processos, int tamanho) {
    int classe_contagem[MAX_PROCESSOS] = {0};
    int classes[MAX_PROCESSOS];
    int total = 0;

    for (int i = 0; i < tamanho; i++) {
        int classe = processos[i].id_classe;
        int j;
        for (j = 0; j < total; j++) {
            if (classes[j] == classe) {
                classe_contagem[j]++;
                break;
            }
        }
        if (j == total) {
            classes[total] = classe;
            classe_contagem[total++] = 1;
        }
    }

    for (int i = 0; i < total; i++) {
        printf("Classe %d: %d processos\n", classes[i], classe_contagem[i]);
    }
}

void assuntounico(Processo* processos, int tamanho) {
    int assuntos[MAX_PROCESSOS * MAX_ASSUNTOS] = {0};
    int total = 0;

    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            int id = processos[i].id_assuntos[j];
            int k;
            for (k = 0; k < total; k++) {
                if (assuntos[k] == id) break;
            }
            if (k == total) assuntos[total++] = id;
        }
    }

    printf("Assuntos unicos: %d\n", total);
}

void multiploassuntos(Processo* processos, int tamanho) {
    printf("Assunto multiplos:\n");
    for (int i = 0; i < tamanho; i++) {
        if (processos[i].qtd_assuntos > 1) {
            printf("Processo ID %d tem %d assuntos.\n", processos[i].id, processos[i].qtd_assuntos);
        }
    }

    int assunto_contagem[MAX_PROCESSOS * MAX_ASSUNTOS] = {0};
    int assuntos_unicos[MAX_PROCESSOS * MAX_ASSUNTOS];
    int total_assuntos = 0;

    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            int id_assunto = processos[i].id_assuntos[j];
            int k;
            for (k = 0; k < total_assuntos; k++) {
                if (assuntos_unicos[k] == id_assunto) {
                    assunto_contagem[k]++;
                    break;
                }
            }
            if (k == total_assuntos) {
                assuntos_unicos[total_assuntos] = id_assunto;
                assunto_contagem[total_assuntos++] = 1;
            }
        }
    }

    printf("\nAssunto multiplos:\n");
    for (int i = 0; i < total_assuntos; i++) {
        if (assunto_contagem[i] > 1) {
            printf("Assunto Numero %d: %d vezes\n", assuntos_unicos[i], assunto_contagem[i]);
        }
    }
}

void tramitacao(Processo* processos, int tamanho) {
    time_t t = time(NULL);
    struct tm atual = *localtime(&t);

    for (int i = 0; i < tamanho; i++) {
        struct tm data = {0};
        sscanf(processos[i].data_ajuizamento, "%d-%d-%d", &data.tm_year, &data.tm_mon, &data.tm_mday);
        data.tm_year -= 1900;
        data.tm_mon--;
        time_t ajuizamento = mktime(&data);

        double dias = difftime(t, ajuizamento) / (60 * 60 * 24);
        printf("O processo %d possui o total de %.0f dias em tramitacao.\n", processos[i].id, dias);
    }
}

void csvid(Processo* processos, int tamanho) {
    qsort(processos, tamanho, sizeof(Processo), id);
    csvcriar("IDS ORDENADOS.csv", processos, tamanho);
}

void csvdata(Processo* processos, int tamanho) {
    qsort(processos, tamanho, sizeof(Processo), data);
    csvcriar("DATAS ORDENADAS.csv", processos, tamanho);
}

void csvcriar(const char* nome_arquivo, Processo* processos, int tamanho) {
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) return;

    fprintf(arquivo, "id,data_ajuizamento,id_classe,id_assuntos\n");
    for (int i = 0; i < tamanho; i++) {
        fprintf(arquivo, "%d,%s,%d,{", processos[i].id, processos[i].data_ajuizamento, processos[i].id_classe);
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            fprintf(arquivo, "%d", processos[i].id_assuntos[j]);
            if (j < processos[i].qtd_assuntos - 1) fprintf(arquivo, ",");
        }
        fprintf(arquivo, "}\n");
    }

    fclose(arquivo);
}
