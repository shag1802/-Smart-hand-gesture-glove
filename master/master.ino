#include "Wire.h" // This library allows you to communicate with I2C devices.
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int f1=A0;
int f2= A1;
int f3= A2;
int16_t ax, ay, az; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data
int16_t flex1, flex2,flex3;
int mode = -1;
int oncount[4]={0};
bool l[3]= {true,false,false};
bool f[3] ={true,false,false};
bool s1[3]= {true,false,false};
bool s2[3]= {false,true,false};
bool s3[3]= {false,false,true};
int templ[3]={0};
int flag=0;
char tmp_str[7]; // temporary variable used in convert function
char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

bool validate(int f1,int f2,int f3,bool la[]){
    // off off off
    //off =false
    //on = true
    // all 8 cases have diff comparision
    if(la[0] == true  && la[1] == false && la[2] == false) 
    {
        if(f1<150 && f2>190 && f3>190) 
        {
            return true;
        }
        else{
            return false;
        }
    }
    if(la[0] == false  && la[1] == true && la[2] == false) 
    {
        if(f1>190 && f2<150 && f3>190) 
        {
            return true;
        }
        else{
            return false;
        }
    }
    if(la[0] == false  && la[1] == false && la[2] == true) 
    {
        if(f1>190 && f2>190 && f3<150) 
        {
            return true;
        }
        else{
            return false;
        }
    }
        
}

void setup() {
    Serial.begin(9600);
    Wire.begin();
    Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
    Wire.write(0x6B); // PWR_MGMT_1 register
    Wire.write(0); // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
    pinMode(f1,INPUT);
    pinMode(f2,INPUT);
    
 
}
void loop() {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
    Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
    Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
    ax = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
    ay = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
    az = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
    temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
    gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
    gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
    gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
    
    flex1 = analogRead(f1);
    flex2 = analogRead(f2);
    flex3 = analogRead(f3);

    //For changing mode   
    if(flex1<150 && flex2<150 && flex3 <150){
        mode++;
        delay(1500);
        if(mode==4){
            mode=0;
        }
        Serial.write(mode + 15);
  
    }
 
    switch(mode){
        case 0:
            if(flex1 <150 && flex2 <150 && flex3 >190){
                if(oncount[0]==0){
                    delay(1500);
                    Serial.write(0);
                    oncount[0]=1;
                    delay(1500); 
                }
                else if(oncount[0]==1){
                    delay(1500);
                    Serial.write(1);
                    oncount[0]=0;
                    delay(1500); 
                }
            }
            break;
        case 1: // light mode
            if(validate(flex1,flex2,flex3,l)){
                if(oncount[1]==0){
                    delay(1500);
                    Serial.write(2);
                    oncount[1]=1;
                    delay(1500); 
                }
                else if(oncount[1]==1){
                    delay(1500);
                    Serial.write(3);
                    oncount[1]=0;
                    delay(1500); 
                }
            }
            //gestures changing mode
            else if(flex1 <150 && flex2 <150 && flex3>190){
                Serial.write(60);
                delay(1500);
                int store ;
                Serial.write(10);
                //Serial.write("Now register your input");
                delay(4000);
                templ[0]= analogRead(f1);
                templ[1]=analogRead(f2);
                templ[2]=analogRead(f3);
                Serial.write(11);
                //Serial.write("Input registered");
                delay(1500);
                for(int i=0;i<3;i++){
                  if(templ[i]<150){
                      store=i;
                      //Serial.write("The fingure registered:");
                      Serial.write(50+i+1);
                  }
                }
                for(int i=0;i<3;i++){
                  if(i==store){
                      l[i]=true;
                  }
                  else{
                      l[i]=false;
                  }
                }
            }
            else if(flex1>190 && flex2 <150 && flex3 <150){
                if(flag==0){
                  flag=1;
                  delay(1500);
                  Serial.write(88);
                  delay(1500);
                }
                else{
                  flag=0;
                  delay(1500);
                  Serial.write(89);
                  delay(1500);
                }
            }
            
            else if(az>-16000 && az<-14000 && oncount[1]==1 && flag==1){
                delay(1500);
                Serial.write(255);
                delay(1500);
            }
            else if(az>-7000 && az < -5000 && oncount[1]==1 && flag==1){
                delay(1500);
                Serial.write(254);
                delay(1500);
            }
            else if(az>0 && oncount[1]==1 && flag==1){
                delay(1500);
                Serial.write(253);
                delay(1500);
            }
            break;
        case 2: // fan  mode
            if(validate(flex1,flex2,flex3,f)){
                if(oncount[2]==0){
                    delay(1500);
                    Serial.write(4);
                    oncount[2]=1;
                    delay(1500); 
                }
                else if(oncount[2]==1){
                    delay(1500);
                    Serial.write(5);
                    oncount[2]=0;
                    delay(1500); 
                }
            }
            else if(flex1 <150 && flex2 <150 && flex3>190){
                delay(1500);
                int store ;
                Serial.write(20);
                //Serial.write("Now register your input");
                delay(4000);
                templ[0]= analogRead(f1);
                templ[1]=analogRead(f2);
                templ[2]=analogRead(f3);
                Serial.write(21);
                //Serial.write("Input registered");
                delay(1500);
                for(int i=0;i<3;i++){
                  if(templ[i]<150){
                      store=i;
                      //Serial.write("The fingure registered:");
                      Serial.write(50+i+1);
                  }
                }
                for(int i=0;i<3;i++){
                  if(i==store){
                      f[i]=true;
                  }
                  else{
                      f[i]=false;
                  }
                }
            }
            else if(flex1>190 && flex2 <150 && flex3 <150){
                if(flag==0){
                  flag=1;
                  delay(1500);
                  Serial.write(88);
                  delay(1500);
                }
                else{
                  flag=0;
                  delay(1500);
                  Serial.write(89);
                  delay(1500);
                }
            }
            
            else if(az>-16000 && az<-14000 && oncount[2]==1 && flag==1){
                delay(1500);
                Serial.write(245);
                delay(1500);
            }
            else if(az>-7000 && az < -5000 && oncount[2]==1 && flag==1){
                delay(1500);
                Serial.write(244);
                delay(1500);
            }
            else if(az>0 && oncount[2]==1 && flag==1){
                delay(1500);
                Serial.write(243);
                delay(1500);
            }
            break;
       
        case 3:
            //for pause and play
            if(validate(flex1,flex2,flex3,s1)){
                if(oncount[3]==0){
                    delay(1500);
                    Serial.write(6);
                    oncount[3]=1;
                    delay(1500);
                }
                else if(oncount[3]==1){
                    delay(1500);
                    Serial.write(7);
                    oncount[3]=0;
                    delay(1500); 
                }
            }
            if(validate(flex1,flex2,flex3,s2)){
                delay(1500);
                Serial.write(8);
                delay(1500);
            }
            if(validate(flex1,flex2,flex3,s3)){
                delay(1500);
                Serial.write(9);
                delay(1500);
            }
            else if(flex1>190 && flex2 <150 && flex3 <150){
                if(flag==0){
                  flag=1;
                  delay(1500);
                  Serial.write(88);
                  delay(1500);
                }
                else{
                  flag=0;
                  delay(1500);
                  Serial.write(89);
                  delay(1500);
                }
            }
            else if(flex1 <150 && flex2 <150 && flex3>190){
                delay(1500);
                int store ;
                Serial.write(30);
                //Serial.write("Now register your input for play and pause");
                delay(4000);
                templ[0]= analogRead(f1);
                templ[1]=analogRead(f2);
                templ[2]=analogRead(f3);
                Serial.write(31);
                //Serial.write("Input registered");
                delay(1500);
                for(int i=0;i<3;i++){
                  if(templ[i]<150){
                      store=i;
                      //Serial.write("The fingure registered:");
                      Serial.write(50+i+1);
                  }
                }
                for(int i=0;i<3;i++){
                  if(i==store){
                      s1[i]=true;
                  }
                  else{
                      s1[i]=false;
                  }
                }

                delay(1500);
                Serial.write(40);
                //Serial.write("Now register your input to select go forward in music app");
                delay(4000);
                templ[0]= analogRead(f1);
                templ[1]=analogRead(f2);
                templ[2]=analogRead(f3);
                Serial.write(41);
                //Serial.write("Input registered");
                delay(1500);
                for(int i=0;i<3;i++){
                  if(templ[i]<150){
                      store=i;
                      //Serial.write("The fingure registered:");
                      Serial.write(50+i+1);
                  }
                }
                for(int i=0;i<3;i++){
                  if(i==store){
                      s2[i]=true;
                  }
                  else{
                      s2[i]=false;
                  }
                }

                delay(1500);
                Serial.write(70);
                //Serial.write("Now register your input to select go backward in music app");
                delay(4000);
                templ[0]= analogRead(f1);
                templ[1]=analogRead(f2);
                templ[2]=analogRead(f3);
                Serial.write(71);
                //Serial.write("Input registered");
                delay(1500);
                for(int i=0;i<3;i++){
                  if(templ[i]<150){
                      store=i;
                      //Serial.write("The fingure registered:");
                      Serial.write(50+i+1);
                  }
                }
                for(int i=0;i<3;i++){
                  if(i==store){
                      s3[i]=true;
                  }
                  else{
                      s3[i]=false;
                  }
                }
            }
            else if(az>-16000 && az<-14000 && oncount[3]==1 && flag==1){
                delay(1500);
                Serial.write(235);
                delay(1500);
            }
            else if(az>-7000 && az < -5000 && oncount[3]==1 && flag==1){
                delay(1500);
                Serial.write(234);
                delay(1500);
            }
            else if(az>0 && oncount[3]==1 && flag==1 ){
                delay(1500);
                Serial.write(233);
                delay(1500);
            }
            break;  
        }
    }
