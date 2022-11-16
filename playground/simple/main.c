#include <stdio.h>
#include <stdint.h>

int main(int argc, char* argv[]){
  unsigned char bytes[8] = {255, 2, 3, 4, 250, 5, 6, 7};//{1, 2, 3, 4, 5, 6, 7, 8};

  uint32_t* word = (uint32_t*) bytes;
  uint64_t* dword = (uint64_t*) bytes;
  // it uses little endian
  


  uint32_t state[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
  };

  word = (uint32_t*) state;
  dword = (uint64_t*) state;
  printf("As uint32=0x%X, sizeof(bytes)=%lu\n", word[0], sizeof(bytes));
  printf("As uint32=0x%lX, sizeof(bytes)=%lu\n", dword[0], sizeof(bytes));
  for (int i=0; i<8; i++) {
    printf("%02x", ((unsigned char*) state)[i]);
  }
  puts("");
  
  return 0;
}
