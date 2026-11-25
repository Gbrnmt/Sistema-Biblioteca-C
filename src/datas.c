#include <stdio.h>
#include <time.h>
#include "datas.h"
#include "utils.h"

Data dataHoje(void) {
    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);
    Data d;
    d.dia = t->tm_mday;
    d.mes = t->tm_mon + 1;
    d.ano = t->tm_year + 1900;
    return d;
}
static int anoBissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0;
}

static int diasNoMes(int mes, int ano) {
    static const int dias[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2 && anoBissexto(ano)) {
        return 29;
    }
    return dias[mes - 1];
}

int dataValida(Data d) {
    if (d.ano < 1970 || d.ano > 2200) return 0;
    if (d.mes < 1 || d.mes > 12) return 0;
    if (d.dia < 1 || d.dia > diasNoMes(d.mes, d.ano)) return 0;
    return 1;
}
long dataParaDias(Data d) {
    long total = 0;
    for (int a = 1970; a < d.ano; a++) {
        total += anoBissexto(a) ? 366 : 365;
    }
    for (int m = 1; m < d.mes; m++) {
        total += diasNoMes(m, d.ano);
    }
    return total + d.dia - 1;
}

int diferencaDias(Data a, Data b) {
    return (int)(dataParaDias(b) - dataParaDias(a));
}

Data somarDias(Data d, int n) {
    while (n > 0) {
        d.dia++;
        if (d.dia > diasNoMes(d.mes, d.ano)) {
            d.dia = 1;
            d.mes++;
            if (d.mes > 12) {
                d.mes = 1;
                d.ano++;
            }
        }
        n--;
    }
    return d;
}
void formatarData(Data d, char *saida) {
    snprintf(saida, 11, "%02d/%02d/%04d", d.dia, d.mes, d.ano);
}

void lerData(const char *mensagem, Data *destino) {
    char texto[TAM_TEXTO];

    while (1) {
        printf("%s (dd/mm/aaaa ou 0 para hoje)\n", mensagem);
        lerTexto("> ", texto, TAM_TEXTO);

        if (texto[0] == '0' && texto[1] == '\0') {
            *destino = dataHoje();
            return;
        }

        Data d;
        if (sscanf(texto, "%d/%d/%d", &d.dia, &d.mes, &d.ano) == 3 && dataValida(d)) {
            *destino = d;
            return;
        }
        printf("Data invalida. Exemplo: 25/11/2026\n");
    }
}