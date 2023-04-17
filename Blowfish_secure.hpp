/**
 * file: Blowfish_secure.hpp
 *
 * Based on original code copyright 2007 Jim Wigginton
 * https://github.com/phpseclib/phpseclib/blob/master/phpseclib/Crypt/Blowfish.php
 *
 * MIT License:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
**/

#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <stdio.h>

class Blowfish {

  public:
  Blowfish();
  std::string encrypt( std::string *plain );
  std::string encrypt( std::string &plain );
  std::string decrypt( std::string *encoded );
  std::string decrypt( std::string &encoded );

  private:
  int blockSize = 8;
//// INSERT ENCODED P-BOX AND S-BOX ARRAYS/LISTS AFTER THIS LINE ////


//// INSERT ENCODED P-BOX AND S-BOX ARRAYS/LISTS BEFORE THIS LINE ////
  void encodeBlock(uint64_t *leftptr, uint64_t *rightptr);
  void pad(std::string *text);
  void unpad(std::string *text);

};

Blowfish::Blowfish() {
}

std::string Blowfish::encrypt(std::string &plain) {
  return encrypt(&plain);
}

std::string Blowfish::encrypt(std::string *plain) {
  uint64_t buffer = 0;
  std::stringstream resultStream; //This way worked for me
  uint64_t left  = 0;
  uint64_t right = 0;

  //Pad text to match block size
  pad(plain);

  for (int b = 0; b < plain->length(); b+=blockSize) {
    for (int i = 0; i < 8; ++i) {
      buffer = (buffer << 8) | int(plain->at((b+i)));
    }
    left ^= ((buffer & 0xFFFFFFFF00000000) >> 32);
    right ^= (buffer & 0x00000000FFFFFFFF);
    encodeBlock(&left,&right);
    buffer = (left << 32);
    buffer += right;
    //Maybe not the way to do it!
    resultStream << std::setfill('0') << std::setw(16) << std::hex << buffer;
    buffer = 0;
  }
  //Maybe not the way to do it!
  std::string result = resultStream.str();
  return result;
}

std::string Blowfish::decrypt(std::string &encoded) {
  return decrypt(&encoded);
}

std::string Blowfish::decrypt(std::string *encoded) {
  int hexBlockSize = blockSize*2;
  uint64_t buffer = 0;
  std::stringstream resultStream;
  uint64_t left  = 0;
  uint64_t right = 0;
  uint64_t leftbuffer  = 0;
  uint64_t rightbuffer = 0;
  uint64_t leftvector  = 0;
  uint64_t rightvector = 0;

  //You could pad it up - but the input text should be an encoded string from the encrypt function
  if ((encoded->length() % hexBlockSize) != 0) {
    std::stringstream error_string;
    error_string << "Hex coded input length is wrong (length: " << encoded->length() << ")";
    throw std::length_error(error_string.str());
  }

  for (int b = 0; b < encoded->length(); b+=hexBlockSize) {
    for (int i = 0; i < hexBlockSize; i+=2) {
      buffer = (buffer << blockSize) | std::stoul(encoded->substr((b+i),2), nullptr, 16);
    }
    left = ((buffer & 0xFFFFFFFF00000000) >> 32);
    right = (buffer & 0x00000000FFFFFFFF);
    leftbuffer = left;
    rightbuffer = right;
    std::swap(left,right);
    right ^= (uint64_t)pBuffer[17];
    for (int i = 16; i > 0; i-=2) {
      left  ^= ((((int64_t)sBox0[((right >> 24) & 0xFF)] + (int64_t)sBox1[((right >> 16) & 0xFF)]) ^ (int64_t)sBox2[((right >> 8) & 0xFF)]) + (int64_t)sBox3[(right & 0xFF)]) ^ (uint64_t)pBuffer[i];
      right ^= ((((int64_t)sBox0[((left  >> 24) & 0xFF)] + (int64_t)sBox1[((left  >> 16) & 0xFF)]) ^ (int64_t)sBox2[((left  >> 8) & 0xFF)]) + (int64_t)sBox3[(left  & 0xFF)]) ^ (uint64_t)pBuffer[(i-1)];
    }
    left = (left & 0xFFFFFFFF) ^ (uint64_t)pBuffer[0];
    right = (right & 0xFFFFFFFF);
    buffer = ((left^leftvector) << 32);
    buffer += (right^rightvector);
    leftvector = leftbuffer;
    rightvector = rightbuffer;
    for (int i = 7; i >= 0; --i) {
      resultStream << (char)((buffer >> (i*8)) & 0xFF);
    }
  }
  std::string result = resultStream.str();
  unpad(&result);

  return result;
}

//The Blowfish block encode helper
void Blowfish::encodeBlock(uint64_t *leftptr, uint64_t *rightptr) {
  (*leftptr)  ^= pBuffer[0];
  for (int i = 1; i < 16; i+=2) {
    (*rightptr) ^= ((((int64_t)sBox0[(((*leftptr) & 0xFF000000) >> 24)]  + (int64_t)sBox1[(((*leftptr) & 0xFF0000) >> 16)])  ^ (int64_t)sBox2[(((*leftptr) & 0xFF00) >> 8)])  + (int64_t)sBox3[((*leftptr) & 0xFF)])  ^ (int64_t)pBuffer[i];
    (*leftptr)  ^= ((((int64_t)sBox0[(((*rightptr) & 0xFF000000) >> 24)] + (int64_t)sBox1[(((*rightptr) & 0xFF0000) >> 16)]) ^ (int64_t)sBox2[(((*rightptr) & 0xFF00) >> 8)]) + (int64_t)sBox3[((*rightptr) & 0xFF)]) ^ (int64_t)pBuffer[(i+1)];
  }
  (*rightptr) = ((*rightptr) & 0xFFFFFFFF) ^ (int64_t)pBuffer[17];
  (*leftptr)  = ((*leftptr)  & 0xFFFFFFFF);
  std::swap((*leftptr),(*rightptr));
}

//Pad with 0x01 if text is short of 1 character
//Pad with 0x0202 if text is short of 2 characters
//Strange way to do it in my opinion....
void Blowfish::pad(std::string *text) {
  //Plus 1 to make sure we are padding, else we get problems when unpadding
  if (((text->length()+1) % blockSize) != 0) {
    int pad = blockSize - (text->length() % blockSize);
    text->insert(text->length(), pad, (char)pad);
  }
}

//Tried unpadding the same amount of characters as last character - couldn't get it working
//This worked....
void Blowfish::unpad(std::string *text) {
  std::string buffer = (*text);
  char lastBuffer = buffer[(buffer.length()-1)];
  //Ambiguous when we ARE padding, but I like it
  if ((int)lastBuffer < 9) {
    (*text) = "";
    for (int i = 0; i < (buffer.length()-1); ++i) {
      if (buffer[i] != lastBuffer)
        (*text) += buffer[i];
    }
  }
}


int main() {
  Blowfish blowfish;
  std::string string = "Hello World!";
  std::cout << "Encrypt: " << string << "\n";
  std::string encrypted = blowfish.encrypt(string);
  std::cout << "Encrypted: " << encrypted << "\n";
  std::cout << "Decrypt: " << encrypted << "\n";
  std::string decrypted = blowfish.decrypt(&encrypted);
  std::cout << "Decrypted: " << decrypted << "\n";
  return 0;
}
