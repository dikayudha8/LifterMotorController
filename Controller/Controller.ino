#include "ServerLifter.h"

ServerLifter *serverLifter;
void setup() {
  serverLifter = new ServerLifter();
}

void loop() {
  serverLifter->RunServerLifter();
}

