std::vector<std::string> activeMacAddressList{};
std::vector<std::string> rollingMacAddressList{};

bool isAddressInList(std::vector<std::string> listIn, std::string macAddrIn) {
  //checks if a string is present in a list of strings. In this case it checks if a macaddress is present in the given list
  bool isAddressFound = false;

  for (auto listEntry : macAddrIn) {  //iterates through list elements as "listEntry". If the element matches the madAddress testcase it returns a true state else the function will return a false state
    if (listIn[listEntry] == macAddrIn) {
      isAddressFound = true;
      break;
    }
  }
  return isAddressFound;
}

//--------------ActiveMacAddressTracking
void updateActiveList(std::string macAddrIn) {
  //calls isAddressInList in order to determine if given macAddress is already present. If not, it is added to both the active and rolling list
  //---activelist
  if (!isAddressInList(activeMacAddressList, macAddrIn)) {
    activeMacAddressList.push_back(macAddrIn);
  }
  //---rolling list
  if (!isAddressInList(rollingMacAddressList, macAddrIn)) {
    rollingMacAddressList.push_back(macAddrIn);
  }
}

void clearList(std::vector<std::string> listIn) {
  if (!listIn.empty()) {
    for (auto listElement : listIn) {
      listIn.erase(std::remove(listIn.begin(), listIn.end(), listElement), listIn.end());
    }
  }
}

void compareActiveRollingLists() {
  //Iterates through each active list element and checks if it is present in rolling list. If not present, it is popped.
  //Printing
  /*
  for (int i = 0; i < sizeof(activeMacAddressList); i++) {
    Serial.print("Active: ");
    Serial.println(activeMacAddressList.at(i).c_str());
  }

  for (int j = 0; j < sizeof(rollingMacAddressList); j++) {
    Serial.print("Rolling: ");
    Serial.println(rollingMacAddressList.at(j).c_str());
  }

  */

  if ((currentCompareMillis - previousCompareMillis) >= (timeoutTime * 1000)) {
    for (auto activeElement : activeMacAddressList) {
      bool inRolling = false;
      for (auto rollingElement : rollingMacAddressList) {
        if (rollingElement == activeElement) {
          inRolling = true;
        }
      }
      if (!inRolling) {
        activeMacAddressList.erase(std::remove(activeMacAddressList.begin(), activeMacAddressList.end(), activeElement), activeMacAddressList.end());
      }
    }
    userCountP2P = activeMacAddressList.size();
    previousCompareMillis = currentCompareMillis;
    clearList(rollingMacAddressList);
  }
}

/*
SAFEKEEPING FOR LOGIC
void compareArrayContents(char * active, char * rolling){ //name of function may be changed since its not entirely descriptive of what its doing
  //on millis timer. Run every X amount of seconds (effectively max timout length)
  //compare  the contents of activelist and rollinglist. remove any mac addresses that appear in active list but not in rolling 
  //reset rolling list
  currentMillis = millis();

  if ((currentMillis - previousMillis) >= (timoutTime * 1000)){
    for (int i = 0; i < sizeof(active); i++){
      bool foundMatch = false;
      for (int x = 0; x < sizeof(rolling); x++){
        if (active[i] == rolling[x]){
          foundMatch = true;
          break;
       }
      if (!foundMatch){
        active[i] = 0;
        }
      }
    }
  }
}
*/
