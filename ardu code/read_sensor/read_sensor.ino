float A[10];
void setup(){
  Serial.begin(9600);
  }

int loop_counter = 0;
void loop(){
  float r_A = loop_counter+0.5;
  A[loop_counter]=r_A;

  if(loop_counter < 9){
    loop_counter = loop_counter+1;
    }
  else{
    loop_counter = 0;
    for(int i=0;i<=9;i++){
      Serial.print(A[i]);
      Serial.print(";");
      }
    }
  Serial.println(loop_counter);
  delay(500);
  }
