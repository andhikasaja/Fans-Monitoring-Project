

//int laser = 8;
int pushbutton = 12;
int i = 0;
//int myCounter = 0; 

void setup() 
{ 
  Serial.begin(9600);
    pinMode(pushbutton,INPUT_PULLUP); // Provide pull-up
} 

void loop() 
{

  for (int i; i<=50; i++){
   
  }

  if(digitalRead(pushbutton) == LOW) // When button pressed
  {
    i = 1; // Set counter to 0
  }
delay(200);
  Serial.print(i);
}
