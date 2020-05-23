#include <iostream>
#include <stdio.h>
#include <ft2build.h>
#include <vector>
#include <codecvt>
#include <map>
#include <iomanip>
#include "prepare-bitmaps.hpp"
#include "save_char_map.hpp"
#include <boost/program_options.hpp>

#include FT_FREETYPE_H

namespace po = boost::program_options;

int main (int ac, const char *av[]){
  po::options_description desc("");
  desc.add_options()
    ("alphabet", po::value<std::string>(), "Alphabet to print")
    ("output", po::value<std::string>(), "Header file with result")
    ("fontface", po::value<std::string>(), "Fontface to use")
    ("size", po::value<size_t>()->default_value(16), "Fontsize")
  ;


  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);
  if ((vm.count("alphabet") == 0) || (vm.count("output") == 0) || (vm.count("fontface") == 0)) {
    std::cout << 
      desc << "  " 
      << ((vm.count("alphabet") == 0) || 
         (vm.count("output") == 0) || 
         (vm.count("fontface")))
      << "\n";
    return 1;
  }
  std::string alphabet = vm["alphabet"].as<std::string>();
  std::string fontface = vm["fontface"].as<std::string>();
  std::string header_file = vm["output"].as<std::string>();
  size_t size = vm["size"].as<size_t>();
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16_conv;
  std::u16string utf16 = utf16_conv.from_bytes(alphabet);
  std::cout << vm["alphabet"].as<std::string>() << alphabet.size() << ' ' << utf16.size() << "\n";
  std::cout << vm["fontface"].as<std::string>() << "\n";
  auto charmap = prepare_alphabet(utf16, fontface, size);
  save_char_map(charmap, header_file);
  return 0;
}


