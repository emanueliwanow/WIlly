#ifndef DEFINICOES_SISTEMA_H_INCLUDED
#define DEFINICOES_SISTEMA_H_INCLUDED

#define true  1
#define false 0

#define MAX_TAREFAS 3

#define NUM_ESTADOS 5
#define NUM_EVENTOS 7

// ESTADOS
#define ESPERA   0
#define MOT_DESARMADOS    1
#define MOT_ARMADOS   2
#define MOD_MANUAL  3
#define MOD_AUTOMATICO 4

// EVENTOS
#define NENHUM_EVENTO   -1
#define CONNECT_BT       0
#define DISCONNECT_BT    1
#define ARM              2
#define DISARM           3
#define MANUAL           4
#define AUTOMATICO       5
#define RETURN           6

// ACOES
#define NENHUMA_ACAO -1
#define A01  0
#define A02  1
#define A03  2
#define A04  3
#define A05  4
#define A06  5
#define A07  6


#endif // DEFINICOES_SISTEMA_H_INCLUDED
