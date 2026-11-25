#ifndef DATAS_H
#define DATAS_H

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

// Data de hoje (relogio do computador)
Data dataHoje(void);

// 1 se a data existe no calendario (ano >= 1970)
int dataValida(Data d);

// Quantidade de dias entre 01/01/1970 e a data (serve para subtrair datas)
long dataParaDias(Data d);

// Dias de "a" ate "b" (positivo se b e depois de a)
int diferencaDias(Data a, Data b);

// Data d + n dias
Data somarDias(Data d, int n);

// Escreve "dd/mm/aaaa" em saida (precisa de pelo menos 11 chars)
void formatarData(Data d, char *saida);

// Pede uma data ao usuario; digitar 0 usa a data de hoje
void lerData(const char *mensagem, Data *destino);

#endif