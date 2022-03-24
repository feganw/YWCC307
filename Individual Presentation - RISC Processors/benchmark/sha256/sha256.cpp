#include "sha256.h"

// Ch(X,Y,Z) = (X AND Y) XOR (bitwise-complement(X AND Z))
uint32_t __ch(uint32_t x, uint32_t y, uint32_t z){
	return (x & y) ^ (~x & z); 
}

// Maj(X,Y,Z) = (X AND Y) XOR (X AND Z) XOR (Y AND Z)
uint32_t __maj(uint32_t x, uint32_t y, uint32_t z){
	return (x & y) ^ (x & z) ^ (y & z);
}

// big-sigma0(X) = RotR(X,2) XOR RotR(X,13) XOR RotR(X,22)
uint32_t __uSigma0(uint32_t x){
	return __rotr(x, 2) ^ __rotr(x, 13) ^ __rotr(x, 22);
}

// big-sigma1(X) = RotR(x,6) XOR RotR(X,11) XOR RotR(X,25)
uint32_t __uSigma1(uint32_t x){
	return __rotr(x, 6) ^ __rotr(x, 11) ^ __rotr(x, 25);
}

// little-sigma0(X) = RotR(X,7) XOR RotR(X,18) XOR ShR(X,3)
uint32_t __lSigma0(uint32_t x){
	return __rotr(x, 7) ^ __rotr(x, 18) ^ (x >> 3);
}

// little-sigma1(X) = RotR(X,17) XOR RotR(X,19) XOR ShR(X,10)
uint32_t __lSigma1(uint32_t x){
	return __rotr(x, 17) ^ __rotr(x, 19) ^ (x >> 10);
}

// Rotates bits right.
uint32_t __rotr(uint32_t x, uint32_t n){
	return (x >> n) | (x << (32 - n));
}

// Swaps endianess of 32-bit word.
uint32_t __swapEndian32(uint32_t x){
	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint32_t y = x;
		y = (y & 0xffff0000) >> 16 | (y & 0x0000ffff) << 16;
		y = (y & 0xff00ff00) >>  8 | (y & 0x00ff00ff) <<  8;
		return y;
	#else
		return x;
	#endif
}

// Swaps endianess of 64-bit word.
uint64_t __swapEndian64(uint64_t x){
	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint64_t y = x;
		y = (y & 0xffffffff00000000) >> 32 | (y & 0x00000000ffffffff) << 32;
		y = (y & 0xffff0000ffff0000) >> 16 | (y & 0x0000ffff0000ffff) << 16;
		y = (y & 0xff00ff00ff00ff00) >>  8 | (y & 0x00ff00ff00ff00ff) <<  8;
		return y;
	#else
		return x;
	#endif
}

// Generates SHA-256 checksum of a string of an arbitrary length.
std::string sha256hash(unsigned char* msg, size_t length){
	
	uint32_t h0[8] = {
		0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
	};

	uint32_t k[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};

	uint32_t a, b, c, d, e, f, g, h, i, j, x;

	/* Padding */
	uint32_t messageLength = length;	
	messageLength++;	// For appended bit.
	messageLength += 8; // For L
	messageLength += 64 - (messageLength % 64);	// For padding, bring length to multiple of 512 bits.
	
	uint8_t * buf = (uint8_t*) calloc(messageLength, 1);
	
	// Copy message into buffer.
	memcpy(buf, msg, length);

	buf[length] = 0x80;	// Append a bit to the message.
	
	uint64_t L = length * 8;	// Length is in bits.
	L = __swapEndian64(L);
	memcpy(buf + (messageLength - sizeof(uint64_t)), &L, sizeof(uint64_t));	// End message with 64 bit value L.
	
	// Swap endianess of message in buffer.
	uint32_t * messageDWORD = (uint32_t*) buf;
	for(i = 0; i < messageLength / 4; i++){
		*messageDWORD = __swapEndian32(*messageDWORD);
		messageDWORD++;
	}	
	
	/* Block Decomposition */
	// For each block M belonging to set {0, 1}^512, 64 words of 32 bits each are constructed as follows:
		// The first 16 are obtained by splitting M in 32-bit blocks
			// M = W1||W2||...||W15||W16
		// The remaining 48 are obtained with the formula
			// Wi = lSigma1(Wi - 2) + Wi - 7 + lSigma0(Wi - 15) + Wi - 16,		17 <= i <= 64
	
	// Message split into 512 bit blocks.
	// Each block is split and processed as 64 32-bit words.
	
	// 512 bit blocks
	uint32_t w[64];
	for(i = 0; i < (messageLength / 64); i++){		
		for(j = 0; j < 16; j++){
			w[j] = *(((uint32_t*)buf) + (i * 16) + j);
		}
		
		for(j = 16; j < 64; j++){	// words 17-64
			w[j] = __lSigma1(w[j-2]) + w[j-7] + __lSigma0(w[j - 15]) + w[j - 16];
		}
		
		a = h0[0];
		b = h0[1];
		c = h0[2];
		d = h0[3];
		e = h0[4];
		f = h0[5];
		g = h0[6];
		h = h0[7];
				
		for(x = 0; x < 64; x++){				
			uint32_t temp1 = h + __uSigma1(e) + __ch(e, f, g) + k[x] + w[x];
			uint32_t temp2 = __uSigma0(a) + __maj(a, b, c);
			h = g;
			g = f;
			f = e;
			e = d + temp1;
			d = c;
			c = b;
			b = a;
			a = temp1 + temp2;			
		}
		
		h0[0] += a;
		h0[1] += b;
		h0[2] += c;
		h0[3] += d;
		h0[4] += e;
		h0[5] += f;
		h0[6] += g;
		h0[7] += h;		
	}	
	
	std::stringstream stream;
	for(i = 0; i < 8; i++){
		char s[16];
		sprintf(s, "%08x", h0[i]);
		stream << s;
	}
			
	std::string hash = stream.str();	
	free(buf);	
	return hash;
}