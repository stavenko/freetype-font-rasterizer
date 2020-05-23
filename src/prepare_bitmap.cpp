#include "prepare-bitmaps.hpp"
#include <iostream>
#include <map>
#include <math.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

std::map<wchar_t, Bitmap> prepare_alphabet(std::u16string &alphabet, std::string &font_face_path, size_t size) {
  FT_Library library;
  FT_Face font_face;
  auto error = FT_Init_FreeType(&library);
  
  if (error) {
    std::cout << "cannot init freetype" << std::endl;
  }

  error = FT_New_Face(library, font_face_path.c_str(), 0, &font_face);
  if (error) {
    std::cout << "file open error " << error << std::endl;
  }

  std::cout << "charmaps " << font_face->family_name << ' '<< font_face->num_charmaps <<'\n';
  
  error = FT_Set_Charmap(font_face, font_face->charmaps[2]);
  if (error) {
    std::cout << "select charmap error " << error << std::endl;
  }
  // FT_Set_Char_Size(font_face, 0, 1000, 128, 128);
  
  FT_Set_Pixel_Sizes(font_face, size, size);
  unsigned int max_width = 0;
  unsigned int max_height = 0;
  std::map<wchar_t, Bitmap> charmap;

  for (auto chr: alphabet) {

    auto glyph_index = FT_Get_Char_Index(font_face, chr);
    
    // FT_Glyph glyph;
    error = FT_Load_Glyph(font_face, glyph_index, FT_LOAD_DEFAULT);
    if (error) {
      std::cout << "error loading glyph " << error << std::endl;
    }

    // FT_Get_Glyph(font_face->glyph, &glyph);

    // FT_Vector origin;
    // origin.x = 320;
    // origin.y = 320;
    error= FT_Render_Glyph(font_face->glyph, FT_RENDER_MODE_NORMAL);
    if (error) {
      std::cout << "rendering glyph " << error << std::endl;
    }
    
    // FT_BitmapGlyph bit= (FT_BitmapGlyph) glyph;
    FT_Bitmap bitmap = font_face->glyph->bitmap;
    
     // std::cout << "num_subglyph " << font_face->glyph->num_subglyphs << "\n";
    max_width = std::max(max_width, bitmap.width + font_face->glyph->bitmap_left);
    max_height = std::max(max_height, bitmap.rows + font_face->glyph->bitmap_top);
    std::vector<uint8_t> map;
    uint8_t bm_width = bitmap.width;
    uint8_t bm_height = bitmap.rows/ 8 + ( (bitmap.rows % 8) > 0 ? 1 : 0);; 
    std:: cout << "dims: " << bitmap.width << 'x' << bitmap.rows << '\n';

    for (int i = 0; i < bitmap.width; i++) {
      uint8_t bytes[] = {0x00, 0x00, 0x00, 0x00};
      uint8_t *byte;
      int bytes_processed = 0;
      for (int j = 0; j < bitmap.rows; j++) {
        if (j % 8 ==0 ) {
          byte = &bytes[(j / 8)];
          bytes_processed++;
        }
        auto windex = i; //bitmap.width - (i+1); 
        auto hindex = bitmap.rows - (j+1);
        auto ix = windex + hindex * bitmap.pitch; 
        auto value = bitmap.buffer[ix];
        if (value > 0) {
          *byte |= (0x01 << (j % 8)); // set bit
          std::cout << '*';
        } else 
          std::cout << ' ';
      }
      std:: cout<< '\n';
      for (int c = 0; c < bytes_processed; c++) {
        map.push_back(bytes[bytes_processed - (c+1)]);
      }
    }

    charmap[chr] = { bm_width, bm_height, map};
  }
  return charmap;
}
