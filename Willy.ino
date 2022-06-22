/*
    ALARME RESIDENCIAL
*/

#include "definicoes_sistema.h"




TaskHandle_t Task1;
TaskHandle_t Task2;

/***********************************************************************
 Estaticos
 ***********************************************************************/
int codigoEvento = NENHUM_EVENTO;
int eventoInterno = NENHUM_EVENTO;
int estado = ESPERA;
int codigoAcao;
int acao_matrizTransicaoEstados[NUM_ESTADOS][NUM_EVENTOS];
int proximo_estado_matrizTransicaoEstados[NUM_ESTADOS][NUM_EVENTOS];
char input;

/************************************************************************
 executarAcao
 Executa uma acao
 Parametros de entrada:
    (int) codigo da acao a ser executada
 Retorno: (int) codigo do evento interno ou NENHUM_EVENTO
*************************************************************************/
int executarAcao(int codigoAcao)
{
    int retval;

    retval = NENHUM_EVENTO;
    if (codigoAcao == NENHUMA_ACAO)
        return retval;

    switch(codigoAcao)
    {
    case A01:

        
        break;
    case A02:

        
        break;
    case A03:

        
        break;
    case A04:

        
        break;
    case A05:

        
        break;
    case A06:

        
        break;
    case A07:

        
        break;
    } // switch

    return retval;
} // executarAcao

/************************************************************************
 iniciaMaquina de Estados
 Carrega a maquina de estados
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void iniciaMaquinaEstados(){
  int i;
  int j;

  for (i=0; i < NUM_ESTADOS; i++) {
    for (j=0; j < NUM_EVENTOS; j++) {
       acao_matrizTransicaoEstados[i][j] = NENHUMA_ACAO;
       proximo_estado_matrizTransicaoEstados[i][j] = i;
    }
  }
  proximo_estado_matrizTransicaoEstados[ESPERA][CONNECT_BT] = MOT_DESARMADOS;
  acao_matrizTransicaoEstados[ESPERA][CONNECT_BT] = A01;

  proximo_estado_matrizTransicaoEstados[MOT_DESARMADOS][DISCONNECT_BT] = ESPERA;
  acao_matrizTransicaoEstados[MOT_DESARMADOS][DISCONNECT_BT] = A02;

  proximo_estado_matrizTransicaoEstados[MOT_DESARMADOS][ARM] = MOT_ARMADOS;
  acao_matrizTransicaoEstados[MOT_DESARMADOS][ARM] = A03;

  proximo_estado_matrizTransicaoEstados[MOT_ARMADOS][DISARM] = MOT_DESARMADOS;
  acao_matrizTransicaoEstados[MOT_ARMADOS][DISARM] = A04;

  proximo_estado_matrizTransicaoEstados[MOT_ARMADOS][MANUAL] = MOD_MANUAL;
  acao_matrizTransicaoEstados[MOT_ARMADOS][MANUAL] = A05;

  proximo_estado_matrizTransicaoEstados[MOT_ARMADOS][AUTOMATICO] = MOD_AUTOMATICO;
  acao_matrizTransicaoEstados[MOT_ARMADOS][AUTOMATICO] = A06;

  proximo_estado_matrizTransicaoEstados[MOD_MANUAL][RETURN] = MOT_ARMADOS;
  acao_matrizTransicaoEstados[MOD_MANUAL][RETURN] = A07;

  proximo_estado_matrizTransicaoEstados[MOD_MANUAL][DISARM] = MOT_DESARMADOS;
  acao_matrizTransicaoEstados[MOD_MANUAL][DISARM] = A04;

  proximo_estado_matrizTransicaoEstados[MOD_MANUAL][DISCONNECT_BT] = ESPERA;
  acao_matrizTransicaoEstados[MOD_MANUAL][DISCONNECT_BT] = A02;

  proximo_estado_matrizTransicaoEstados[MOD_AUTOMATICO][DISCONNECT_BT] = ESPERA;
  acao_matrizTransicaoEstados[MOD_AUTOMATICO][DISCONNECT_BT] = A02;

  proximo_estado_matrizTransicaoEstados[MOD_AUTOMATICO][RETURN] = MOT_ARMADOS;
  acao_matrizTransicaoEstados[MOD_AUTOMATICO][RETURN] = A07;

  proximo_estado_matrizTransicaoEstados[MOD_AUTOMATICO][DISARM] = MOT_DESARMADOS;
  acao_matrizTransicaoEstados[MOD_AUTOMATICO][DISARM] = A04;


} // initStateMachine

/************************************************************************
 iniciaSistema
 Inicia o sistema ...
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void iniciaSistema()
{
   iniciaMaquinaEstados();
} // initSystem


/************************************************************************
 obterEvento
 Obtem um evento, que pode ser da IHM ou do alarme
 Parametros de entrada: nenhum
 Retorno: codigo do evento
*************************************************************************/


/************************************************************************
 obterAcao
 Obtem uma acao da Matriz de transicao de estados
 Parametros de entrada: estado (int)
                        evento (int)
 Retorno: codigo da acao
*************************************************************************/
int obterAcao(int estado, int codigoEvento) {
  return acao_matrizTransicaoEstados[estado][codigoEvento];
} // obterAcao


/************************************************************************
 obterProximoEstado
 Obtem o proximo estado da Matriz de transicao de estados
 Parametros de entrada: estado (int)
                        evento (int)
 Retorno: codigo do estado
*************************************************************************/
int obterProximoEstado(int estado, int codigoEvento) {
  return proximo_estado_matrizTransicaoEstados[estado][codigoEvento];
} // obterAcao



/***********************************************************************
 Tasks
 ***********************************************************************/

/************************************************************************
 taskMaqEstados
 Task principal de controle que executa a maquina de estados
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void taskMaqEstados(void *pvParameters) {
  const TickType_t xDelay10ms = pdMS_TO_TICKS(10);
  
  for(;;){
    if (eventoInterno != NENHUM_EVENTO) {
        codigoEvento = eventoInterno;
    }
    if (codigoEvento != NENHUM_EVENTO)
    {
        Serial.print(estado);
        Serial.print(codigoEvento);
        codigoAcao = obterAcao(estado, codigoEvento);
        estado = obterProximoEstado(estado, codigoEvento);
        eventoInterno = executarAcao(codigoAcao);
        Serial.print("Estado: ");
        Serial.print(estado);
        Serial.print(" Evento: ");
        Serial.print(codigoEvento);
        Serial.print(" Acao: ");
        Serial.println(codigoAcao);
    }
    vTaskDelay(xDelay10ms);
  }
}

/************************************************************************
 taskObterEvento
 Task que faz pooling de eventos
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void taskObterEvento(void *pvParameters) {       // EH AQUI QUE VAI FICAR O BLUETOOTH
  const TickType_t xDelay10ms = pdMS_TO_TICKS(10);
  for(;;){
    if(Serial.available()){
          input = Serial.read();
          Serial.print("You typed: " );
          Serial.println(input);
          if (input == 'q') {
            codigoEvento = CONNECT_BT;
           
          }
          if (input == 'w') {
            codigoEvento = DISCONNECT_BT;
            
          }
          if (input == 'e') {
            codigoEvento = ARM;
            
          }
          if (input == 'r') {
            codigoEvento = DISARM;
            
          }
          if (input == 't') {
            codigoEvento = MANUAL;
            
          }
          if (input == 'y') {
            codigoEvento = AUTOMATICO;
            
          }
          if (input == 'u') {
            codigoEvento = RETURN;
            
          }
          vTaskDelay(xDelay10ms);
      }
  }
  
  
}



/************************************************************************
 Main
 Setup e Loop principal de controle que executa a maquina de estados
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/

void setup() {
  Serial.begin(115200);
  delay(5000);
  iniciaSistema();
  
  // configure tasks
  xTaskCreatePinnedToCore(taskMaqEstados, "taskMaqEstados", 10000, NULL, 1, &Task1,0);
  delay(500);
  xTaskCreatePinnedToCore(taskObterEvento, "taskObterEvento", 10000, NULL, 1, &Task2,1);
  delay(500);
  
  
   
  

  
  
} // setup

void loop() {
  
} // loop
