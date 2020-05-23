#ifndef _SAVE_CHAR_MAP_H
#define _SAVE_CHAR_MAP_H
#include <vector>
#include <stdio.h>
#include <map>
struct Bitmap {
  uint8_t width;
  uint8_t height;
  std::vector<uint8_t> buffer;
};

void save_char_map(std::map<wchar_t, Bitmap> & bm, std::string &filename);
#endif

