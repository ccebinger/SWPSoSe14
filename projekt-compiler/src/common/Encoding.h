/*
 * Helper.h
 *
 *  Created on: 19.05.2014
 *      Author: mubu
 */

//FIXME to be replaced with codecvt (gcc lacks atm)


#ifndef ENCODING_H_
#define ENCODING_H_

#include <deque>
#include <string>

class Encoding {
private:
	Encoding();
	virtual ~Encoding();
	
public:

	
	enum Utf8 {
		MaxBytes = 4,
	};
	
	enum Unicode {
		RuneMin = 0x0,		// Lowest Unicode CodePoint	
		RuneMax = 0x10FFFF,	// Highest Unicode CodePoint
		RuneError = 0xFFFD,	// Unicode decoding error rune
	};
	
	enum {
		T1 = 0x0,			// 0000 0000
		T2 = 0xC0,			// 1100 0000
		T3 = 0xE0,			// 1110 0000
		T4 = 0xF0,			// 1111 0000
		T5 = 0xF8,			// 1111 1000
		Tx = 0x80,			// 1000 0000
		
		Rune1 = 0x7F,		// 0000000001111111
		Rune2 = 0x7FF,		// 0000011111111111
		Rune3 = 0xFFFF,		// 1111111111111111
		Rune4 = 0x1FFFFF,	// 000111111111111111111111
		
		MaskSuffix1 = 0x7F,	// 0111 1111 
		MaskSuffix2 = 0x1F,	// 0001 1111
		MaskSuffix3 = 0xF,	// 0000 1111
		MaskSuffix4 = 0x7,	// 0000 0111
		
		TestX = 0xC0,		// 1100 0000
		MaskX = 0x3F,		// 0011 1111
	};
	
	static uint8_t unicodeToUtf8(uint32_t unicode, std::string& result) {
		
		uint32_t rune = unicode;
		
		// 1 byte (0 - 0x7F)
		if(rune <= Rune1) {
			result += (char)rune;
			return 1;
		}
		
		// 2 bytes (0x80 - 0x7FF => T2 Tx)
		if(rune <= Rune2) {
			result += (T2 | (rune >> 1*6));
			result += (Tx | (rune & MaskX));
			return 2;
		}
		
		// Error handling: Rune out of range => error rune.
		if(rune > RuneMax) {
			rune = RuneError;
		}
		
		// 3 bytes (0x800 - 0xFFFF => T3 Tx Tx)
		if(rune <= Rune3) {
			result += (T3 | (rune >> 2*6));
			result += (Tx | (MaskX & (rune >> 1*6)));
			result += (Tx | (MaskX & rune));
			return 3;
		}
		
		// 4 bytes (0x10000 - 0x1FFFFF => T4 Tx Tx Tx)
		result += (T4 | (rune >> 3*6));
		result += (Tx | (MaskX & (rune >> 2*6)));
		result += (Tx | (MaskX & (rune >> 1*6)));
		result += (Tx | (MaskX & rune));
		return 4;
	}
	
	
	static std::string unicodeToUtf8(uint32_t unicode) {
		std::string result;
		Encoding::unicodeToUtf8(unicode, result);
		return result;
	}



	static void unicodeToUtf8String(std::deque<int32_t>* unicode, std::string& result) {
		for(auto i=unicode->begin(); i<unicode->end(); ++i) {
			unicodeToUtf8(*i, result);
		}
	}
	
	
	
	
	static uint8_t utf8ToUnicode(std::string str, int32_t start, int32_t* rune) {
		
		// 1 byte
		uint8_t a = str.at(start);
		if(a < Tx) {
			*rune = a;
			return 1;
		}
		
		
		if((uint32_t)(start+1) < str.length()) {
			
			// 2 bytes
			uint8_t b = str.at(start+1) ^ Tx;
			if(b & TestX) {
				//FIXME show warning: byte within sequence not in range 0x80 - 0xBF
				*rune = RuneError;
				return 1;
			}
			if(a < T3) {
				*rune = ((a << 6) | b) & Rune2;
				if(*rune <= Rune1) {
					//FIXME show warning
					*rune = RuneError;
					return 1;
				}
				return 2;
			}
			
			
			
			if((uint32_t)(start+2) < str.length()) {
				
				// 3 bytes
				uint8_t c = str.at(start+2) ^ Tx;
				if(c & TestX) {
					//FIXME show warning: byte within sequence not in range 0x80 - 0xBF
					std::cerr << "#3" << std::endl;
					*rune = RuneError;
					return 1;
				}
				if(a < T4) {
					*rune = ((((a << 6) | b) << 6) | c) & Rune3;
					if(*rune <= Rune2) {
						//FIXME show warning
						std::cerr << "#4" << std::endl;
						*rune = RuneError;
						return 1;
					}
					return 3;
				}
				
				
				if((uint32_t)(start+3) < str.length()) {
					
					// 4 bytes
					uint8_t d = str.at(start+3) ^ Tx;
					if(d & TestX) {
						//FIXME show warning: byte within sequence not in range 0x80 - 0xBF
						std::cerr << "#5" << std::endl;
						*rune = RuneError;
						return 1;
					}
					if(a < T5) {
						*rune = ((((((a << 6) | b) << 6) | c) << 6) | d) & Rune4;
						if(*rune <= Rune3) {
							//FIXME show warning
							std::cerr << "#6" << std::endl;
							*rune = RuneError;
							return 1;
						}
						return 4;
					}
				}
			}
		}
		
		
		// 5 bytes or more do not exist in unicode
		//FIXME show warning
		*rune = RuneError;
		return 1;
	}
	
	


	static void utf8StringToUnicode(std::string input, std::deque<int32_t>* result, uint32_t begin=0) {
		int32_t rune;
		for(size_t i=begin; i < input.size(); ) {
			i += utf8ToUnicode(input, i, &rune);
			result->push_back(rune);
		}
	}
	

	/**
	 * Returns the number of bytes a given rune will require in UTF-8
	 */
	static uint8_t getRuneByteCount(uint32_t rune) {
		if(rune <= Rune1) { return 1; }
		if(rune <= Rune2) { return 2; }
		if(rune <= Rune3) { return 3; }
		return 4;
	}


	/**
	 * Returns the highest Unicode value for a given byte sequence length
	 */
	static uint32_t getRuneLimitByCount(uint8_t count) {
		switch(count) {
			case 1: return Rune1;
			case 2: return Rune2;
			case 3: return Rune3;
			default: return Rune4;
		}
	}

};

#endif /* ENCODING_H_ */
