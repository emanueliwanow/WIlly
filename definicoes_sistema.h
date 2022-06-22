#ifndef DEFINICOES_SISTEMA_H_INCLUDED
#define DEFINICOES_SISTEMA_H_INCLUDED

#define true  1
#define false 0

#define MAX_TAREFAS 3

#define NUM_ESTADOS 5
#define NUM_EVENTOS 8

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
#define CHANGE_VEL       7

// ACOES
#define NENHUMA_ACAO -1
#define A01  0
#define A02  1
#define A03  2
#define A04  3
#define A05  4
#define A06  5
#define A07  6
#define A08  7


// PINOS E BIBLIOTECAS

#define Rele1 15
#define Rele2 4
#define Rele3 5
#define Rele4 18

#define motor1 33
#define motor2 32
#define MIN_SPEED 1040 // speed just slow enough to turn motor off
#define MAX_SPEED 1240 // speed where my motor drew 3.6 amps at 12v.


#endif // DEFINICOES_SISTEMA_H_INCLUDED
