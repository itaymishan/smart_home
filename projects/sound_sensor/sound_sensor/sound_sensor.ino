
int soundDetectedPin = 5; // Use Pin 10 as our Input
int soundDetectedVal = 0;
void setup ()
{
  Serial.begin(9600);  
  pinMode (soundDetectedPin, INPUT) ; // input from the Sound Detection Module
}
void loop ()
{
  soundDetectedVal = analogRead (soundDetectedPin) ; // read the sound alarm time
  if(soundDetectedVal > 500)
  {
     Serial.println(soundDetectedVal); 
  }
}
