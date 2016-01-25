/*
	Tap tempo project

	Calculate tempo in BPM from avarage button taps with button ( 4 times / 4 ) from millisecond

	The circuit:
	* Pin 2 button 5V, 10k to ground 
	* Pin 5V to button

	Created Sunday January 2016
	By Adam Luptak
	Modified Sunday January 2016
	By Adam Luptak
	Version: 1.0.

	https://github.com/AdamLuptak/TapTempoProject.git

	Logic for tap tempo calculate: 

	*/
#define TAP_BUTTON_INPUT 2 // attached to interupt
#define INTERUPT_NUMBER 0 

//Variables
volatile long currentTime = 0;
volatile long tempoArray [4] = {0,0,0,0};
const int tempoArraySize = 4;
volatile long tapTempoInterval = 0;

void setup(){
	Serial.begin(9600);
	//initialisation
	pinMode(2, INPUT);
	//Setup intterupt number and ISR and type of checking of intterupt	
	attachInterrupt(0, readTempo, RISING);
}

void readTempo(){
	static unsigned long last_interrupt_time = 0;
	static unsigned int indexOfTempoArray = 0;
	unsigned long interrupt_time = millis();
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
 if (interrupt_time - last_interrupt_time > 200) {
 	tapTempoInterval = ( indexOfTempoArray == 4) ? CalculateTempo(tempoArray, tempoArraySize, 0) : 0;
 	indexOfTempoArray = ( indexOfTempoArray == 4) ? 0 : indexOfTempoArray;
 	//actual time
 	currentTime = interrupt_time - last_interrupt_time;
 	//save actual time to array
 	tempoArray[indexOfTempoArray] = currentTime;
 	//increment the position in array
 	indexOfTempoArray++;
 }
 last_interrupt_time = interrupt_time;
}

long CalculateTempo(volatile long arrayTempos[], int tempoArraySize, int timeDivision){
	//Example 100 BPM  1/4 = (60 * 1000) / 100 = 600ms; =>  60 000 / avarageTempoMs = BPM
	//Example 100 BPM  1/8. = (avarageTempoMs / 4 * ) 3 )  => 450 ms
long sumTempos = 0;
long tempoInterval = 0;
for(int i=0; i < tempoArraySize; i++){
	sumTempos += arrayTempos[i];
}
	//avarage Millis
	long avarageTempoMs = sumTempos / tempoArraySize;
	switch (timeDivision) {
		case 0:
	      // 1/4 tempo 
	      tempoInterval = avarageTempoMs;
	      break;
	      case 1:
	      tempoInterval = (avarageTempoMs / 4) * 3;
	      // 1/8. tempo
	      break;
	      default:
	      // return ZERO
	      tempoInterval = 0;
	  }
	  return tempoInterval;

	}

	long tempoPre = 0;
	void loop(){
	//test print array with times
	if(tempoPre != tapTempoInterval){
		printArray(tempoArray, tempoArraySize);
		Serial.print("tempo:");
		Serial.println(tapTempoInterval);
		tempoPre = tapTempoInterval;
	}
}

/**
 * Function just print array of long tempos
 * @param long array[]
 * @param int sizeOfArray
 */
 void printArray(volatile long array[], int sizeofArray){
 	for(int i=0; i < sizeofArray; i++){
 		Serial.print(array[i]);
 		Serial.print(" , ");   
 	}
 	Serial.println();
 }