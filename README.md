# ampel-web

## A PlatformIO Project
near field communication using wifi ,esp8266_NODE_MCU 

Trafic Light Example

> Make sure your EEPROM is clean, else clean it up
> you can use the code below to erase all date from the eeprom.
> ```cpp 
  // delete eeprom
  EEPROM.begin(512);
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  EEPROM.end();
```

# Free to use, edit and modify

