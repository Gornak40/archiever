#include "help.h"
#include <iostream>

Help::Help() {
    std::cerr << "Usage for compression: archive -c ARCHIVE_NAME [FILES...]" << std::endl;
    std::cerr << "Usage for decompression: archive -d ARCHIVE_NAME" << std::endl;
    std::cerr << "Usage for getting this note: archive -h" << std::endl;
    std::cerr << "Made by Gornak40" << std::endl;
}
