
static int taskCore = 0;

void MultiCore( void * pvParameters ) {

  String taskMessage = "Task running on core ";
  taskMessage = taskMessage + xPortGetCoreID();
  
  int j = 0;
  while (true) {
    Serial.println(taskMessage);

    Serial.print("j ");
    Serial.println(j++);
    delay(3000);

  }

}
