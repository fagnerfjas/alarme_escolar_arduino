//Programa: Data e hora com modulo RTC DS1302
//Alteracoes e adaptacoes: Arduino e Cia
//
//Baseado no programa original de Krodal e na biblioteca virtuabotixRTC
//Fagner Jefferson, Agosto 2023, fagnerfjas@gmail.com
//ECIT Serra Branca

//Carrega a biblioteca virtuabotixRTC
#include <virtuabotixRTC.h>           

//Determina os pinos ligados ao modulo
//myRTC(clock, data, rst)
virtuabotixRTC myRTC(3, 2, 1);

int tempoSirene = 10; //Tempo total de ativação da sirene em segundos
int delaySirene = 3;  //Tempo de pausa para segunda ativação - tempo de tolerância em minutos

// Lista com os horários para alarmar
String horarios[] = {
  "07:30",
  "08:20",
  "09:10",
  "09:30",
  "10:20",
  "11:10",
  "12:00",
  "13:20",
  "14:10",
  "15:00",
  "15:20",
  "16:10",
  "17:00"
};

// Lista com horários de intervalo
String intervalos[] = {
  "09:10",
  "12:00",
  "15:00",
  "17:00"
};

void setup()  
{      
  
  Serial.begin(9600);
  //Informacoes iniciais de data e hora
  //Apos setar as informacoes, comente a linha abaixo
  //(segundos, minutos, hora, dia da semana, dia do mes, mes, ano)
  //myRTC.setDS1302Time(00, 23, 14, 5, 17, 8, 2023);  //Configuração do calendário

  pinMode(13, OUTPUT);   // Pino da sirene 
}


// Método com loop para execução 
void loop()  
{
  //Le as informacoes do CI
  myRTC.updateTime(); 

  String horario = horaAtual();   //Retorna hora e minuto atual
  noHorario(horario); //Verifica se é hora de acionar a sirene

  printDataHora();
  delay(1000);
}

// Retorna uma String com o horário e minuto atual 
String horaAtual(){
  String zeroHora = "";
  String zeroMin = "";
  if (myRTC.hours < 10)
  {
    zeroHora = "0";
  }      

  if (myRTC.minutes < 10)
  {
    zeroMin = "0";
  }
  String horario = zeroHora+(String)myRTC.hours+":"+zeroMin+(String)myRTC.minutes;
  Serial.println(horario);
  return horario;
}


//** Compara o horário atual e ativa a sirene
// Caso seja horário de aula, 5 min depois é tocado sinal de tolerância
// Caso seja intervalo ou fim de dia não toca sinal de tolerância
void noHorario(String horario){
  int rangeHorarios   = (sizeof(horarios)/sizeof(String));
  int rangeIntervalos = (sizeof(intervalos)/sizeof(String));
  for (byte i = 0; i < rangeHorarios; i = i + 1) {
    if( horario == horarios[i] ){
      Serial.println("Alarme em : " + horario);
      ativaSirene(tempoSirene);
      delay(delaySirene * 60000);
      
      for (byte x = 0; x < rangeIntervalos; x = x + 1) {
        if( horario != intervalos[x] ){
          Serial.println("Alarme de tolerância 5min depois de " + horario);
          ativaSirene(tempoSirene/2);
          break;          
        }
      }     
      break;
    }else{
      //Serial.println("Não está no horário");
    }
  }
}


// Ativa o Relê que liga a sirete
void ativaSirene(int segundos)
{
  int tempo = segundos*1000;
  digitalWrite(13, HIGH);   //Ativa a sirene
  delay(tempo);             //Aguarda intervalo de tempo 
  digitalWrite(13, LOW);    //Desativa a sirene
  delay(80000);             // Espera passar um minuto e 10 segundos para retomar a conferência de horário
}



// imprime uma linha no console com o nome do dia da semana
// a data e em seguida a hora com minutos e segundos
// "Data : DIA_SEMANA, dis/mes/ano Hora : hora:minuto:segundo"
void printDataHora()
{
  //Imprime as informacoes no serial monitor
  Serial.print("Data : ");
  //Chama a rotina que imprime o dia da semana
  imprime_dia_da_semana(myRTC.dayofweek);
  Serial.print(", ");
  Serial.print(myRTC.dayofmonth);
  Serial.print("/");
  Serial.print(myRTC.month);
  Serial.print("/");
  Serial.print(myRTC.year);
  Serial.print("  ");
  Serial.print("Hora : ");
  //Adiciona um 0 caso o valor da hora seja <10
  if (myRTC.hours < 10)
  {
    Serial.print("0");
  }
  Serial.print(myRTC.hours);
  Serial.print(":");
  //Adiciona um 0 caso o valor dos minutos seja <10
  if (myRTC.minutes < 10)
  {
    Serial.print("0");
  }
  Serial.print(myRTC.minutes);
  Serial.print(":");
  //Adiciona um 0 caso o valor dos segundos seja <10
  if (myRTC.seconds < 10)
  {
    Serial.print("0");
  }
  Serial.println(myRTC.seconds);
}



// imprime o nome do dia da semana 
void imprime_dia_da_semana(int dia)
{
  switch (dia)
  {
    case 1:
    Serial.print("Domingo");
    break;
    case 2:
    Serial.print("Segunda");
    break;
    case 3:
    Serial.print("Terca");
    break;
    case 4:
    Serial.print("Quarta");
    break;
    case 5:
    Serial.print("Quinta");
    break;
    case 6:
    Serial.print("Sexta");
    break;
    case 7:
    Serial.print("Sabado");
    break;
  }
}
