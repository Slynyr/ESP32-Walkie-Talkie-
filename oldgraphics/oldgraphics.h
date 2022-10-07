void channelCount(bool isConnected, int posX, int posY, int currentChannel){
  char msgBuffer[64] = {0};
  sprintf(msgBuffer, "CH%d", currentChannel);

  if (isConnected){
    drawText(posX, posY, 3.5, msgBuffer);
  } else {
    drawText(posX, posY, 3.5, "NO CON");
  }
}

void drawText(int cursX, int cursY, int textSize, char * message){
    display.setTextSize(textSize);
    display.setTextColor(WHITE);
    display.setCursor(cursX,cursY);
    display.println(message);
}