
#include <Servo.h>
#include <math.h>

Servo thumb, fingers;


//массив для определения разброса значений 
double * mas;
int massSize = 100;

void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  pinMode(10, INPUT); // Setup for leads off detection LO +
  pinMode(11, INPUT); // Setup for leads off detection LO -
  pinMode(9, OUTPUT);  
  
  mas = new double[massSize];
}
//функция расчёта разброса значений в массиве
double dispers(){
    double MatOj = 0;
    double Disp = 0;
    for(int i=0; i<massSize; i++)
        MatOj = mas[i] + MatOj;
    MatOj = MatOj/(double)massSize; //Математическое ожидание
 
    for(int i=0; i<massSize; i++)
        Disp = pow((mas[i] - MatOj),2.0) + Disp;
    Disp = Disp/(massSize-1.0); //Дисперсия   
    return Disp;
}
//функция очистки массива
void clearMass(){
  for(int i=0; i<massSize; i++)mas[i] = 0;
}

void loop() {
  int count = 0;//сброс счётчика
  while(count < massSize){ //накапливаем массив
    if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
      Serial.println('!');
      count = 0;
      clearMass();
    }
    else{
      // send the value of analog input 0:
        mas[count] = analogRead(A0);
    }
    //Wait for a bit to keep serial data from saturating
    delay(1);
    count++;
  }
  double disp = dispers(); //считаем отклонение
  clearMass();
  if(disp <= 0){
    Serial.println(0);
  }else Serial.println(disp);
  int val = map((int)disp, 1000, 50000, 0, 255);
  analogWrite(9, val);
}
/*
int analogpin = 3;
int val = 0;
  void setup()
  {
    fingers.attach(9);  //Attach the servo for the four fingers to pin9
    thumb.attach(8);   // Attach the servo for the thumb to pin 8
    Serial.begin(9600);  // this is for serial moniter to see the muscle sensor value you're getting
  }
  void loop()
{
  val = analogRead(analogpin); //muscle sensor connected to pin A3 being stated as val
  Serial.println(val);
  if( val < 550) //if you flex and the sensor value is greater than 550 then close servos--adjust this value to your muscle sensor value
  {
    thumb.write(180);
    fingers.write(180);
    delay(100);
  }
else{
    thumb.write(0);
    fingers.write(0);
    delay(100);
  }
}
*/
