# Blowfish CBC

**This is a _translation_ of the following Blowfish encrypter/decrypter written in PHP**

https://github.com/phpseclib/phpseclib/blob/master/phpseclib/Crypt/Blowfish.php

## A little story

From a ehtical hacking job, where I needed a blowfish encrypter/decrypter written in C++, supporting CBC mode with self-padding function, I was not able to find something (I probably make all the wrong search terms) but find this script in PHP and decided to translate it to my needs for C++.

## Credits

**All credits for the functionality goes to the authors of the PHP script!**

Jim Wigginton <terrafrost@php.net>

Hans-Juergen Petrich <petrich@tronic-media.com>

# Security alert

As part of the job, I discovered the passphrase in plain-text in a library file, and was able to decrypt the data transfered by the application to the server, I was also able to find the API functions names and variables in the same library file, to get extended functionallity beyond the designated user functionallity.

**So if you use, or want to use this simple but reliable way of encryption, update the P-Box and S-Box arrays/lists with the a passphrase encoded arrays/lists, which will make it very hard to find the passphrase from the compiled code _as I was able to_!!**

# How to use this

The script compiles by including the hpp-file in your cpp files, initialize the class and call the function you need.

Encrypts plain text to hex-encoded text.

Decrypts hex-encoded to plain text.

Works both with `std::string` and a `std::string`-reference, but always return a `std::string` variable.

**The less secure method is in the file Blowfish_lesssecure.hpp**

**Your passphrase has to be a length of a multiply of 8 UTF-8 characters, technically max 144 characters**

## The more secure way

_I my opinion!_

+ Provide your passphrase in the `encode_passphrase.cpp`-file.

+ Compile the `encode_passphrase.cpp`-file and run it, **locally**. _Only need to be done once!_

+ Copy the C++ code provided and paste in to the `Blowfish_secure.hpp`-file and now you are ready.

+ Continue to [In both ways](#in-both-ways)

## The less secure way

_I my opinion!_

+ Provide your passphrase in the `Blowfish_lesssecure.hpp`-file and now you are ready.

+ Continue to [In both ways](#in-both-ways)

## In both ways

+ include the hpp-file:

```
#include "Blowfish_secure.hpp"
```

or 

```
#include "Blowfish_lesssecure.hpp"
```

+ Initialize class:

```
Blowfish blowfish;
```

+ To encrypt:

```
std::string encrypted_hex = blowfish.encrypt(&decrypted_text);
```

or

```
std::string encrypted_hex = blowfish.encrypt(decrypted_text);
```

+ To decrypt:

```
std::string decrypted_text = blowfish.decrypt(&encrypted_hex);
```

or

```
std::string decrypted_text = blowfish.decrypt(encrypted_hex);
```

## Example Usage

```
#include "Blowfish_secure.hpp"

int main() {
  Blowfish blowfish;
  std::cout << "------------------\n";
  std::string original_text = "Hello World!";
  std::cout << "Encrypt: " << original_text << "\n";
  std::string encrypted_hex = blowfish.encrypt(&original_text);
  std::cout << "Encrypted: " << encrypted_hex << "\n";
  std::cout << "------------------\n";
  std::cout << "Decrypt: " << encrypted_hex << "\n";
  std::string decrypted_text = blowfish.decrypt(&encrypted_hex);
  std::cout << "Decrypted: " << decrypted_text << std::endl;
  return 0;
}
```

# Disclamer

**This is all based on personal experience and I am a _self-learned ehtical hacker_, and I take all comments and suggestions to consideration.**

_Haters will be ignored, because haters always gonna hate!_

**The company this script was based on, is not discovered in the code or here and is not using this code, but they adapted the suggestion!**
