#ifndef SHA256_H
#define SHA256_H

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>

// Ch(X,Y,Z) = (X AND Y) XOR (bitwise-complement(X AND Z))
uint32_t __ch(uint32_t x, uint32_t y, uint32_t z);

// Maj(X,Y,Z) = (X AND Y) XOR (X AND Z) XOR (Y AND Z)
uint32_t __maj(uint32_t x, uint32_t y, uint32_t z);

// big-sigma0(X) = RotR(X,2) XOR RotR(X,13) XOR RotR(X,22)
uint32_t __uSigma0(uint32_t x);

// big-sigma1(X) = RotR(x,6) XOR RotR(X,11) XOR RotR(X,25)
uint32_t __uSigma1(uint32_t x);

// little-sigma0(X) = RotR(X,7) XOR RotR(X,18) XOR ShR(X,3)
uint32_t __lSigma0(uint32_t x);

// little-sigma1(X) = RotR(X,17) XOR RotR(X,19) XOR ShR(X,10)
uint32_t __lSigma1(uint32_t x);

// Rotates bits right.
uint32_t __rotr(uint32_t x, uint32_t n);

// Swaps endianess of 32-bit word.
uint32_t __swapEndian32(uint32_t x);

// Swaps endianess of 64-bit word.
uint64_t __swapEndian64(uint64_t x);

// Generates SHA-256 checksum of a string of an arbitrary length.
std::string sha256hash(unsigned char* msg, size_t length);

#endif