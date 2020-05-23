#ifndef _PREPARE_BITMAPS_H
#define _PREPARE_BITMAPS_H
#include <map>
#include <string>
#include "save_char_map.hpp"

std::map<wchar_t, Bitmap> prepare_alphabet(std::u16string &alphabet, std::string &font_face_path, size_t size); 
#endif
