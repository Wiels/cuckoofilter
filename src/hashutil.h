#ifndef CUCKOO_FILTER_HASHUTIL_H_
#define CUCKOO_FILTER_HASHUTIL_H_

#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#include <string>


#include <openssl/evp.h>
#include <random>

namespace cuckoofilter {
typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;

class HashUtil {
  
 public:
  // Bob Jenkins Hash
  static uint32_t BobHash(const void *buf, size_t length, uint32_t seed = 0);
  static uint32_t BobHash(const std::string &s, uint32_t seed = 0);

  // Bob Jenkins Hash that returns two indices in one call
  // Useful for Cuckoo hashing, power of two choices, etc.
  // Use idx1 before idx2, when possible. idx1 and idx2 should be initialized to seeds.
  static void BobHash(const void *buf, size_t length, uint32_t *idx1,
                      uint32_t *idx2);
  static void BobHash(const std::string &s, uint32_t *idx1, uint32_t *idx2);

  // MurmurHash2
  static uint32_t MurmurHash(const void *buf, size_t length, uint32_t seed = 0);
  static uint32_t MurmurHash(const std::string &s, uint32_t seed = 0);

  // SuperFastHash
  static uint32_t SuperFastHash(const void *buf, size_t len);
  static uint32_t SuperFastHash(const std::string &s);

  // Null hash (shift and mask)
  static uint32_t NullHash(const void *buf, size_t length, uint32_t shiftbytes);

  // Wrappers for MD5 and SHA1 hashing using EVP
  static std::string MD5Hash(const char *inbuf, size_t in_length);
  static std::string SHA1Hash(const char *inbuf, size_t in_length);

 private:
  HashUtil();
};

// See Martin Dietzfelbinger, "Universal hashing and k-wise independent random
// variables via integer arithmetic without primes".
class TwoIndependentMultiplyShift {
  //unsigned __int128 multiply_, add_;
  unsigned __int64 mult1;
  unsigned __int64 mult2;
  unsigned __int64 add1;
  unsigned __int64 add2;
  
  

 public:
   TwoIndependentMultiplyShift() {
     mult1 = 0x01104B182AC685B7;
     mult2 = 0x021AD88EFC3D5D70;
     add1 = 0x0216C6EFAF2B2ABF;
     add2 = 0x0304EE4FFE1CB0BE;

  }

  
  

  uint64_t operator()(uint64_t key) const {
    unsigned __int64 temp1;
    unsigned __int64 temp2;
    temp1 = (add1 + mult1*key) >> 32;
    temp2 = (add2 + mult2*key) >> 32;
    unsigned __int64 result;
    result = temp1;
    result = result << 32;
    result |= temp2;
    return result;

    //cast key to the type of multiply
   // return (add_ + multiply_ * static_cast<decltype(multiply_)>(key)) >> 64;

  }
 /*
  TwoIndependentMultiplyShift() {
    ::std::random_device random;
    for (auto v : {&multiply_, &add_}) {
      *v = random();
      for (int i = 1; i <= 4; ++i) {
        *v = *v << 32;
        *v |= random();
      }
    }
    
   multiply_ = create_uint128(0x7D104B182AC685B7,0x381AD88EFC3D5D70);
   add_ = create_uint128(0x5216C6EFAF2B2ABF,0x3F04EE4FFE1CB0BE);

  }

  
  

  uint64_t operator()(uint64_t key) const {
    //cast key to the type of multiply
    return (add_ + multiply_ * static_cast<decltype(multiply_)>(key)) >> 64;

  }
  */
  private:
  unsigned __int128 create_uint128(uint64_t val1, uint64_t val2){
    unsigned __int128 result;
    result = val1;
    result = result << 64;
    result |= val2;
    return result;
  }
};

// See Patrascu and Thorup's "The Power of Simple Tabulation Hashing"
class SimpleTabulation {
  uint64_t tables_[sizeof(uint64_t)][1 << CHAR_BIT];

 public:
  SimpleTabulation() {
    ::std::random_device random;
    for (unsigned i = 0; i < sizeof(uint64_t); ++i) {
      for (int j = 0; j < (1 << CHAR_BIT); ++j) {
        tables_[i][j] = random() | ((static_cast<uint64_t>(random())) << 32);
      }
    }
  }

  uint64_t operator()(uint64_t key) const {
    uint64_t result = 0;
    for (unsigned i = 0; i < sizeof(key); ++i) {
      result ^= tables_[i][reinterpret_cast<uint8_t *>(&key)[i]];
    }
    return result;
  }
};
}

#endif  // CUCKOO_FILTER_HASHUTIL_H_
