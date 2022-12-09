// issue with simd, we need to define instruction based on val type

#ifndef LONG_MESSAGE_CONFIG
#define LONG_MESSAGE_CONFIG
#include <stdint.h>
#include "numbers_shorthands.h"
#include "confg_math_func.h"

/*          Hash function specifications       */
/* change it only when you change the function */
#define WORD_SIZE 4 // bytes = 32 bit
#define WORD_TYPE u32
#define NWORDS_DIGEST 8
#define NWORDS_STATE 8
#define NWORDS_INPUT 16
#define HASH_INPUT_SIZE NWORDS_INPUT*WORD_SIZE /* in bytes */

// The attack parameter n can be chosen below s.t.
// n <= 8 * WORD_SIZE * NWORDS_DIGEST

// -------------------------------------------------------------------------+
//                               Assumptions                                |
// 1- First word of hash is enough to hold the distinguish point and nserver|
// e.g. with sha256 32bits is enough.                                       |
// - if this doesn't apply to your case edit to_which_server function       |
// see type of variable ones and ... in phase i functions                   |
// -------------------------------------------------------------------------+


// -------------------------------------------------------------------------+
//                     Long message attack main parameters                  |
// -------------------------------------------------------------------------+
// Apologies: only n ≡ 0 mod 8 is allowed. This is not a feature.

/*****************************************************/
// Let N := n / 8.                                 // *
#define N 12 /* bytes i.e n := 8*N bits */         // *
#define L 34 /* store 2^L elements in the dictionary  */

/*****************************************************/

/* if each server has a different memory size then their candidates           */
/* will have a different false positive probability. The following define     */
/* scale the number according to the false positive. The main obstacle in     */
/* writing a formula as a macro is I don't know a way to get the ram size     */
/* macros. maybe a python script will overwrite the definintion below  */
#define NNEEDED_CND_THIS_SERVER 1000 /* todo write formula to compute it */
#define MAX_CND_PER_SERVER 1005 /* make a dynamic estimation */
/*  ceil(log2(NSERVERS)) =  */
#define NSERVERS 128
#define LOG2_NSERVERS BITS_TO_REPRESENT(NSERVERS)
/* nhashes per server, we take the maximum */
#define NHASHES (1LL<<34)

/* how big is our counter */
#define CTR_TYPE u64

/* How many bytes can accommedate L */
#define L_IN_BYTES CEILING(L, 8) 

#define DIFFICULTY 4 // bits are zero

#if N > WORD_SIZE*NWORDS_DIGEST
  #error "you are trying to attack more bits than the hash function provides"
#endif




#ifndef LONG_MESSAGE_MPI_CONFIG
#define LONG_MESSAGE_MPI_CONFIG
// MPI configurations
// MPI sending and receiving tags
#define TAG_DICT_SND 0 /* send digest tag */
#define TAG_RANDOM_MESSAGE 1
#define TAG_SND_DGST 2
#define TAG_MESSAGES_CANDIDATES 3
#define ARCHIVE_SERVER NSERVERS

#define BUFF_SIZE 1000  // holds `BUFF_SIZE` elements.

#define PROCESS_QUOTA 100 // i.e. send 10 digests to each server

// #define NWORDS_OFFSET 4 // use 128 bits as offsets to find message

#define NSERVERS 128
#define LOG2_NSERVERS BITS_TO_REPRESENT(NSERVERS)

#define DEFINED_BITS (LOG2_NSERVERS + DIFFICULTY)
#define DEFINED_BYTES CEILING(DEFINED_BITS, 8) 
#endif // LONG_MESSAGE_MPI_CONFIG




// Assumptions that need to be changed manually

// Dictionary Configurations:
// Recall: input -> store_as_idx, store_as_val
// we limit |store_as_val| <= 32bit

//  What is the minimum dictionary size we will use?
//#define L 32 /* i.e. it can hold a dictionary with nslots = 2^L */
#define MAX_VAL_SIZE 32 // we are not going to hold more than 32 bits

// because the smallest step is one byte, we will not bother with shifting
// we will discard these bits. The program will work.
// e.g. imagine L = 34, then we have
// B0, B1, ..., B7, B8 mod 2^L will neglect 6 bits

// #define IDX_DISCARDED_BITS (L % 8) /* Values */
// Define VAL_SIZE and decide what is the best type to use
/* Every thing will be stored as idx */
// #if N*8 <= L this case

#if N <= L_IN_BYTES
  #pragma message ("Please decrease MAX_VAL_SIZE")
  #error "The code is not flexible to store every bit as index!"

// we store everything as index except one byte
//+ @todo when changing MAX_VAL_SIZE adopt the conditions below
#elif (N  < L_IN_BYTES + 1) && (N >= L_IN_BYTES) 
  #define VAL_SIZE 1 /* byte */
  #define VAL_TYPE u8  /* unsigned char */

  // SIMD instructions has to be adapted according to the size
  #define SIMD_SET1_VALTYPE _mm256_set1_epi8
  #define SIMD_CMP_VALTYPE _mm256_cmpeq_epi8

#elif (N < L_IN_BYTES + 2) && (N >= L_IN_BYTES + 1) 
  #define VAL_SIZE 2 /* byte */
  #define VAL_TYPE u16 /* unsigned char */
  // SIMD instructions has to be adapted according to the size
  #define SIMD_SET1_VALTYPE _mm256_set1_epi16
  #define SIMD_CMP_VALTYPE _mm256_cmpeq_epi16


// Typical condition when in terms of MAX_VAL_SIZE
// #elif (N  < L_IN_BYTES + MAX_VAL_SIZE_BYTES) && (N >= L_IN_BYTES + (MAX_VAL_SIZE_BYTES/2) )

#elif (N  < L_IN_BYTES + 4) && (N >= L_IN_BYTES + 2 )
  #define VAL_SIZE 4 /* byte */
  #define VAL_TYPE u32 /* unsigned char */
  // SIMD instructions has to be adapted according to the size
  #define SIMD_SET1_VALTYPE _mm256_set1_epi32
  #define SIMD_CMP_VALTYPE _mm256_cmpeq_epi32


#else /* The MAX_VAL_SIZE */
  #define VAL_SIZE 4 /* byte */
  #define VAL_TYPE u32 /* unsigned char */
  // SIMD instructions has to be adapted according to the size
  #define SIMD_SET1_VALTYPE _mm256_set1_epi32
  #define SIMD_CMP_VALTYPE _mm256_cmpeq_epi32

#endif // define VAL_SIZE

// @todo do we need this?
#define DISCARDED_BITS (8*(N - VAL_SIZE - L_IN_BYTES))


 
// depending on avx register length, on my laptop 256
//  Change them together, @todo write a code to automate writing these values
// -------------------------------------------- +
#define AVX_SIZE  256                        // |
#define ALIGNMENT 32                         // |
// Number of elements per simd register         |
#define SIMD_LEN  (AVX_SIZE / (8*VAL_SIZE))  // |
// -------------------------------------------- +





#endif




