/*
    WILLY
*/

#include "definicoes_sistema.h"
#include "motores.h"
#include "BluetoothSerial.h"



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
char message[12] = "";
bool BTisConnected = false;
char stringEvento[4] = "";
int intEvento;
int velL;
int velR;
char stringVel[4];


Motors motors;
BluetoothSerial SerialBT;


void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t*param){
  
  if(event == ESP_SPP_SRV_OPEN_EVT){
    Serial.println("Client Connected");
    BTisConnected = true;
  }

  if(event == ESP_SPP_CLOSE_EVT ){
    BTisConnected = false;
    Serial.println("Client Disconnected");
  }  
}

int executarAcao(int codigoAcao)
{
    int retval;

    retval = NENHUM_EVENTO;
    if (codigoAcao == NENHUMA_ACAO)
        return retval;

    switch(codigoAcao)
    {
    case A01:
        //Serial.print(" Acao: ");
        //Serial.println("A01");
        
        break;
    case A02:
        //Serial.print(" Acao: ");
        //Serial.println("A02");
        motors.disarmMOT();        
        break;
    case A03:
        //Serial.print(" Acao: ");
        //Serial.println("A03 Estado de motores armados");
        motors.armMOT();
        break;
    case A04:
        //Serial.print(" Acao: ");
        //Serial.println("A04 Motores desarmados");
        motors.disarmMOT();        
        break;
    case A05:
        //Serial.print(" Acao: ");
        //Serial.println("A05 Modo manual selecionado");
        motors.setMotR_vel(0);
        motors.setMotL_vel(0);
        break;
    case A06:
        //Serial.print(" Acao: ");
        //Serial.println("A06 Modo automatico selecionado");
        
        break;
    case A07:
        //Serial.print(" Acao: ");
        //Serial.println("A07 Retornando a seleção de modos");
        motors.setMotR_vel(0);
        motors.setMotL_vel(0);
        break;
    case A08:
        //Serial.print(" Acao: ");
        //Serial.println("A08 velocidade mudou");
        motors.setMotR_vel(velR);
        motors.setMotL_vel(velL);
        
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

  proximo_estado_matrizTransicaoEstados[MOD_MANUAL][CHANGE_VEL] = MOD_MANUAL;
  acao_matrizTransicaoEstados[MOD_MANUAL][CHANGE_VEL] = A08;


} // initStateMachine

void iniciaMotores(){
  
  }

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
        //Serial.print(estado);
        //Serial.print(codigoEvento);
        codigoAcao = obterAcao(estado, codigoEvento);
        estado = obterProximoEstado(estado, codigoEvento);
        eventoInterno = executarAcao(codigoAcao);
        //Serial.print("Estado: ");
        //Serial.print(estado);
        //Serial.print(" Evento: ");
        //Serial.print(codigoEvento);
        //Serial.print(" Acao: ");
        //Serial.println(codigoAcao);
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
    
    
    
    if (BTisConnected == true){
      if (estado == ESPERA){
        codigoEvento = A01;
      }
      if (SerialBT.available()){
        for (int i=0;i<11;i++){
          message[i] = SerialBT.read();
          //Serial.println(message);
        }
        //Serial.println(message);
        for (int i=0;i<3;i++){
          stringEvento[i] = message[i];
        }      
        if (stringEvento[2] == '3'){
          codigoEvento = A03;
        }
        if (stringEvento[2] == '4'){
          codigoEvento = A04;
        }
        if (stringEvento[2] == '5'){
          codigoEvento = A05;
        }
        if (stringEvento[2] == '6'){
          codigoEvento = A06;
        }
        if (stringEvento[2] == '7'){
          codigoEvento = A07;
        }
        if (stringEvento[2] == '8'){
          for (int i= 0;i<3;i++){
            stringVel[i] = message[i+4];
          }
          //Serial.println(stringVel);
          if (message[3] == '+'){
            velL = atoi(stringVel);
          }
          if (message[3] == '-'){
            velL = -atoi(stringVel);
          }
          for (int i= 0;i<3;i++){
            stringVel[i] = message[i+8];
          }
          //Serial.println(stringVel);
          if (message[7] == '+'){
            velR = atoi(stringVel);
          }
          if (message[7] == '-'){
            velR = -atoi(stringVel);
          }
          //Serial.println(velL);
          //Serial.println(velR);
          codigoEvento = A08;
        }             
      }
    }
    else {
      // BT DESCONECTADO
      codigoEvento = A02;
    }
    vTaskDelay(xDelay10ms);
      
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
  SerialBT.begin("ESP32");
  SerialBT.register_callback(callback);
  motors.setupMOT();
  delay(1000);
  iniciaSistema();
  
  // configure tasks
  xTaskCreatePinnedToCore(taskMaqEstados, "taskMaqEstados", 10000, NULL, 1, &Task1,0);
  delay(500);
  xTaskCreatePinnedToCore(taskObterEvento, "taskObterEvento", 10000, NULL, 1, &Task2,1);
  delay(500);
  
  
   
  

  
  
} // setup

void loop() {
  
} // loop
