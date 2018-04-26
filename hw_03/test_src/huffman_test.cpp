#include <ctime>
#include <sstream>
#include <iostream>

#include "huffman.h"
#include "huffman_test.h"

void HuffmanArchiverTest::RunAllTests() {
    frequencies_add_test();
    frequencies_save_load_test();

    codes_test();

    encode_decode_test_1();
    encode_decode_test_2();
}

namespace {
    const std::stringstream::openmode bit_mask = std::stringstream::binary 
                                               | std::stringstream::in 
                                               | std::stringstream::out;
}
void HuffmanArchiverTest::frequencies_add_test() {
    HuffmanArchiver::Frequencies frequencies;
    
    std::stringstream archiver_input(bit_mask);
    for (std::uint_fast16_t i = 0; i < HuffmanArchiver::NUM_OF_BYTES; ++i) {
        for (std::uint_fast16_t j = 0; j < i; ++j) {
            char c = i;
            archiver_input.write(&c, 1);
        }
    }

    frequencies.add(archiver_input);

    for (std::uint_fast16_t i = 0; i < HuffmanArchiver::NUM_OF_BYTES; ++i) {
        CHECK(frequencies[i] == i);  
    }
}

void HuffmanArchiverTest::frequencies_save_load_test() {
    HuffmanArchiver::Frequencies frequencies;

    for (std::uint_fast16_t i = 0; i < HuffmanArchiver::NUM_OF_BYTES; ++i) {
        frequencies[i] = rand();
    }

    std::stringstream stream(bit_mask);
    frequencies.save(stream);

    HuffmanArchiver::Frequencies second_frequencies;
    stream.clear();
    second_frequencies.load_saved(stream);

    for (std::uint_fast16_t i = 0; i < HuffmanArchiver::NUM_OF_BYTES; ++i) {
        CHECK(frequencies[i] == second_frequencies[i]);
    }
}

void HuffmanArchiverTest::codes_test() {
    HuffmanArchiver::Frequencies frequencies;
    
    for (std::uint_fast16_t i = 0; i < HuffmanArchiver::NUM_OF_BYTES; ++i) {
        frequencies[i] = 1;
    }
    frequencies[0] = 1e18;

    HuffmanArchiver::Codes codes(frequencies);

    CHECK(codes[0].size() == 1);

}

void HuffmanArchiverTest::encode_decode_test_1() {
    
    std::stringstream input_stream(bit_mask);
    std::stringstream encoder_stream(bit_mask);
    std::stringstream decoder_stream(bit_mask);
     
    std::uint64_t input_size;
    std::uint64_t output_size;

    try {
        HuffmanArchiver::encode(input_stream, encoder_stream, 
                                input_size, output_size);
    } catch(...) {
        CHECK(0 == 1);
    }
    CHECK(input_size == 0);
    
    try {
        HuffmanArchiver::decode(encoder_stream, decoder_stream, 
                                input_size, output_size);
    } catch(...) {
        CHECK(0 == 1);
    }
    CHECK(output_size == 0);
}

void HuffmanArchiverTest::encode_decode_test_2() {
    std::stringstream input_stream(bit_mask);
    std::stringstream encoder_stream(bit_mask);
    std::stringstream decoder_stream(bit_mask);

    std::uint64_t input_size;
    std::uint64_t output_size;
    
    const std::size_t TEST_SIZE = 100000;
    for (std::size_t i = 0; i < TEST_SIZE; ++i) {
        unsigned char c = rand();
        input_stream.write(reinterpret_cast<char*>(&c), 1);
    }

    try {
        HuffmanArchiver::encode(input_stream, encoder_stream, 
                                input_size, output_size);
    } catch(...) {
        CHECK(0 == 1);
    }
    CHECK(input_size == TEST_SIZE);
    

    try {
        HuffmanArchiver::decode(encoder_stream, decoder_stream, 
                                input_size, output_size);
    } catch(...) {
        CHECK(0 == 1);
    }
    CHECK(output_size == TEST_SIZE);
    
    input_stream.clear();
    input_stream.seekg(0);
    decoder_stream.seekg(0);
    for (std::size_t i = 0; i < TEST_SIZE; ++i) {
        char a, b;
        input_stream.read(&a, 1);
        decoder_stream.read(&b, 1);
        CHECK(a == b);
    }
}


