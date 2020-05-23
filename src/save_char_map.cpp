#include "save_char_map.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

void save_char_map(std::map<wchar_t, Bitmap> & bm, std::string &filename) {
  std::ofstream ss;
  ss.open(filename);
  std::vector<uint16_t> symbols;
  std::vector<uint16_t> index;
  std::vector<uint8_t> data;
  uint32_t pointer = 0;

  for (auto &pair: bm) {
    symbols.push_back(pair.first);
    index.push_back(pointer);
    pointer += 2 + pair.second.buffer.size();
    data.push_back(pair.second.width);
    data.push_back(pair.second.height);
    for (uint8_t chr: pair.second.buffer) {
      data.push_back(chr);
    }
  }

  ss << "#include <avr/pgmspace.h>\n";
  ss << "#include <stdint.h>\n";
  ss << "extern const uint16_t SYMBOLS_AMOUNT = " << symbols.size() <<";\n";
  ss << "extern const uint16_t symbols_index[] PROGMEM = {\n";
  for (int i = 0; i < symbols.size(); i++) {
    if (i != 0) {
      ss << ", ";
    }
    ss << "0x" <<std::hex <<std::setfill('0') << std::setw(4) << symbols[i];
  }
  ss << "\n};\n";
  ss << "extern const uint16_t pointer_index[] PROGMEM = {\n";
  for (int i = 0; i < index.size(); i++) {
    if (i != 0) {
      ss << ", ";
    }
    ss << "0x" <<std::hex <<std::setfill('0') << std::setw(4) << index[i];
  }
  ss << "\n};\n";
  ss << "extern const uint8_t data_array[] PROGMEM = {\n";
  for (int i = 0; i < data.size(); i++) {
    if (i != 0) {
      ss << ", ";
    }
    ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<unsigned>(data[i]);
  }
  ss << "\n};";

  ss.close();
}

