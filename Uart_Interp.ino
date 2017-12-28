/* Author: Alfonso Breglia
 * Email:  perqualchebitinpiu@gmail.com
 * Date:   28/12/2018
 * Description: interprete di comandi da seriale
 */

// Lunghezza massima della linea di comando includo il terminatore \0
#define MAXCMDSIZE  16
#define MAXOUTBUF  24
#define CMD_LEN      5
// LED integrato
#define LED_PIN    13 


// ultimo comando letto
char command[MAXCMDSIZE]; 
//Buffer da riempire per le risposte
char buf[MAXOUTBUF];

// Legge il successivo carattere dallo stream di input
int get_char(){
  //attende l'arrivo di un nuovo carattere sulla seriale
  while (Serial.available() == 0);  
  return Serial.read();
} 

// Legge al massimo max_size caratteri e ritorna la lunghezza della stringa
// Il carattere \n è scartato

int get_line(char *line, int max_size){
  int i,c;
  for( i =0; i < max_size-1 && (c=get_char()) !='\n'; ++i){
    line[i] = c;    
  }    
  line[i] = '\0';
  return i;  
}

// converte una sequenza di num_digit caratteri esadecimanli in un un uint16
// esempio 000A -> 10
int hex2uint16(char *str, int num_digit){
  uint16_t val = 0;
  int i;
  for (i = 0; i < num_digit; i++){
    uint8_t v = str[i];     
    if (v >= '0' && v <= '9') v = v - '0';
    else if (v >= 'a' && v <='f') v = v - 'a' + 10;
    else if (v >= 'A' && v <='F') v = v - 'A' + 10;            
    val = (val << 4) | (v & 0xF);    
  }
  return val;
}


void setup(){
  pinMode(LED_PIN, OUTPUT);  
  digitalWrite(LED_PIN,LOW);
  Serial.begin(9600);  
  delay(100);  
  //invia il ready
  Serial.write("ready\n",6);
}

void loop(){
  //attende una nuova linea di comando
  int cmd_size = get_line(command,MAXCMDSIZE);    
  int res = 0;
  //se ci sono almeno 5 caratteri nel buffer
  if (cmd_size >= CMD_LEN){
    if(strncmp(command,"HELLO",CMD_LEN)==0){  //stampa una stringa
      int len = sprintf(buf, "UART INTERPRETER\n");
      Serial.write(buf,len);
      res = 1;        
      
    }else if(strncmp(command,"LEDON",CMD_LEN)==0){
      //Accende o spegne il led
      if(cmd_size == 7){
        uint16_t val = hex2uint16(command+6,1);
        digitalWrite(LED_PIN,val);
        res = 1;
      }
    }else if(strncmp(command,"AREAD",CMD_LEN)==0){  
      //Legge il valore analogico da A0 e lo restituisce 
      int sensorValue = analogRead(A0);         
      int len = sprintf(buf, "%04x\n", sensorValue);
      Serial.write(buf,len);
      res = 1;
    }    
  }  
  Serial.write((res==0?"ERR\n":":OK\n"),4);    
}
