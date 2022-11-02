#include "argparse.h"
#include "encoder.h"
#include "decoder.h"
#include "help.h"
#include "exceptions.h"
#include <iostream>

int main(int argc, char** argv) {
    auto parser = ArgParse(argc, argv);
    try {
        switch (parser.GetMode()) {
            case ArgParse::Mode::ENCODE:
                Encoder(parser.GetArgument("-c"), parser.GetFreeArguments());
                break;
            case ArgParse::Mode::DECODE:
                Decoder(parser.GetArgument("-d"));
                break;
            case ArgParse::Mode::HELP:
                Help();
                break;
        }
    } catch (exceptions::BadReader()) {
        std::cerr << "error: some file does not exists" << std::endl;
        return exceptions::ERROR_CODE;
    } catch (exceptions::BadWriter()) {
        std::cerr << "error: archive can't be opened for writing" << std::endl;
        return exceptions::ERROR_CODE;
    }
}
