#include <vector>
#include <exception>
#include <iostream>
#include <string>
#include <fstream>

#include <unistd.h>
#include <getopt.h>

#include "huffman.h"


int main(int argc, char* argv[]) {
    try {
        char mode = '\0';
        std::string input_path; 
        std::string output_path;

        const char short_opts[] = ":cuf:o:";
        const option long_opts[] = {
            {"file", required_argument, nullptr, 'f'},
            {"output", required_argument, nullptr, 'o'}
        };
        
        opterr = 0;
        char opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
        while (opt != -1) {
            if (opt == 'c' || opt == 'u') {
                if (mode != opt && mode != '\0') {
                    throw HuffmanArchiver::CL_options_error(
                                                      "incompatible arguments");
                }
                mode = opt;
            } else if (opt == 'f') {
                input_path = optarg; 
            } else if (opt == 'o') {
                output_path = optarg;
            } else if (opt == ':') {
                throw HuffmanArchiver::CL_options_error( "option -" + 
                                std::string(1, optopt) + " requires argument");
            } else {
                throw HuffmanArchiver::CL_options_error("uknown option: " + 
                                                        std::string(1, optopt));
            }
            opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
        }
        
        if (input_path == "" || output_path == "" || mode == '\0') {
            throw HuffmanArchiver::CL_options_error(
                        "missing mandatory options");
        }

        std::ifstream in_stream(input_path, std::ifstream::binary);
        std::ofstream out_stream(output_path, std::ofstream::binary);
        
        if (!in_stream.is_open()) {
            throw HuffmanArchiver::IO_error("can't open file");
        }

        std::uint64_t in_size, out_size;

        if (mode == 'c') {
            HuffmanArchiver::encode(in_stream, out_stream, in_size, out_size);
        } else {
            HuffmanArchiver::decode(in_stream, out_stream, in_size, out_size);
        }

        std::cout << in_size << '\n' << out_size << '\n' 
                  << HuffmanArchiver::HEADER_SIZE << '\n';

    } catch (const HuffmanArchiver::IO_error& excep) {
        std::cerr << "I/O Error:\n"
                  << excep.what() << '\n'; 
    } catch (const HuffmanArchiver::CL_options_error& excep) {
        std::cerr << "Options Error:\n"
                  << excep.what() << '\n';
    }

    return 0;
}
