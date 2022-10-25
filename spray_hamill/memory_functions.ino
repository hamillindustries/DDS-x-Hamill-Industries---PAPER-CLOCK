// Functions for EEPROM memory
// Copyright: Canye Oest 2022

void save_all_memory() {
  int mem_pos = 0;
  EEPROM.put(mem_pos, maximum_position);
  mem_pos += sizeof(maximum_position);
  EEPROM.put(mem_pos, minutes);
  mem_pos += sizeof(minutes);
  EEPROM.put(mem_pos, initial_pos);
  mem_pos += sizeof(initial_pos);
}

void read_all_memory() {
  int mem_pos = 0;
  EEPROM.get(mem_pos, maximum_position);
  mem_pos += sizeof(maximum_position);
  EEPROM.get(mem_pos, minutes);
  mem_pos += sizeof(minutes);
  EEPROM.get(mem_pos, initial_pos);
  mem_pos += sizeof(initial_pos);
}