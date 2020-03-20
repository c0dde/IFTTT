int led1 = D7; 
int photoresistor = A5;
const int TOTAL_SAMPLES = 5;
int analogValue;
int currentIndex = 0;
int sum, avg;
int values[TOTAL_SAMPLES];
int isLightOn;
bool lightnotified=false;
bool nolightnotified=false;


void setup() {
    
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(photoresistor, INPUT);

}

bool isEnoughSamples()
{
    if(currentIndex < TOTAL_SAMPLES)
    {
        return false;
    }
    else
    {
        return true;
    }
}


void loop() {
    
    
    analogValue=analogRead(photoresistor);

    
    if (isEnoughSamples())
    {
        //sample is enough
        
        for(int i =0; i < currentIndex-1; i++)
        {
            values[i] = values[i+1];
        }
        values[TOTAL_SAMPLES-1]=analogValue;
        
        sum=0;
        avg=0;
        
        for(int i =0; i < currentIndex; i++)
        {
            sum += values[i];
        }
        
        avg = sum / currentIndex;
        
        if(avg > 15)
        {
            //average sample no light, 
            digitalWrite(led1,HIGH);
            
            if(nolightnotified ==false)
            {
                //notify
                Particle.publish("sunlight", "off");
                Serial.println("published off");
                nolightnotified=true;
                lightnotified=false;
            }
        }
        else
        {
            //average sample has light
            digitalWrite(led1,LOW);
            
            if(lightnotified ==false)
            {
                //notify

                Particle.publish("sunlight", "on");
                Serial.println("published on");
                lightnotified=true;
                nolightnotified=false;
                
            }
        }
        
        Serial.println(avg);
    }
    else
    {
        //collect more samples
        values[currentIndex] = analogValue;
        currentIndex = currentIndex +1;

    }

    delay (1000);

    

}

