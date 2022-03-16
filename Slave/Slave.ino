int l=10;
int led =9;
int f=11;
void setup() {
  Serial.begin(9600);
  pinMode(l,OUTPUT);
  pinMode(f,OUTPUT);
  pinMode(led,OUTPUT);
  digitalWrite(l,HIGH);
  
}

void loop() {
  if(Serial.available() > 0){
    int data = Serial.read();
    //Serial.write(data);
    if(data == 2 ){
      digitalWrite(led,HIGH);
      digitalWrite(l,LOW);
    }
    else if(data == 3 ){
      digitalWrite(led,LOW);
      digitalWrite(l,HIGH);
    }
     // light off
    else if(data == 255){
      analogWrite(led,255);
    }
    else if(data ==254){
      analogWrite(led,160);
    }
    else if(data ==253){
      analogWrite(led,80);
    }
    else if(data == 4 )
      digitalWrite(f,HIGH);
    //fan on
    else if(data == 5)
      digitalWrite(f,LOW);
    // fan off
    else if(data ==245){
      analogWrite(f,255);
    }
    else if(data ==244){
      analogWrite(f,180);
    }
    else if(data == 243){
      analogWrite(f,100);
    }
  }
    
  
  
}
