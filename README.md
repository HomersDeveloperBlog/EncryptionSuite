# EncryptionSuite
An implementation of the popular public key encryption scheme, RSA (and more!), gratuitously applied as a block cipher for file encryption. Obviously, as far as practical cryptography goes, there is no reason to ever encrypt a file using an asymmetric cryptosystem. Nonetheless, this was the simplest way I could demonstrate the code.

There are further apologies. The code will not compile in its current form. Although there are unit tests that prove the correctness of the block ciphers, the full package is untested. I will get around to correcting this at some point. On the other hand, the style of the code is representative of the code I produce professionally. If you are an employer looking to assess my work, I can recommend looking at this piece without reservation.

I tried to do a few things well in this project. In order of priority, they are: 
- A modular architecture for encryption that would support differences in block cipher, padding, block mode, direction (encryption or decryption), source (buffer or file). This was the project's biggest challenge.
- The skillful use of the STL library and C++ 11/14 language features.
- Robustness against portability issues, exceptions, invalid user input. This includes strict standards conformance. 
- Good performance.

Unfortunately, I made concessions to my busy life with respect to the program's portability. Shamelessly, the code depends on the availability of a 64-bit integer size. This is on my short list of issues to correct. I've also haven't profiled the code, so to say that it was optimized to any degree would be inaccurate.
