#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search.h"

#if defined(__x86_64__) || defined(_M_X64)
    #include <immintrin.h>
    #define SIMD_ARCH_X86 1
#elif defined(__aarch64__) || defined(__ARM_NEON)
    #include <arm_neon.h>
    #define SIMD_ARCH_ARM 1
#endif

void search_in_file(const char *filepath, const char *keyword) {
    FILE *file = fopen(filepath, "rb");
    if (!file) return;
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (file_size <= 0 || file_size > 50 * 1024 * 1024) {
        fclose(file);
        return;
    }
    
    char *buffer = (char*)malloc(file_size + 1);
    if (!buffer) {
        fclose(file);
        return;
    }
    
    size_t read_bytes = fread(buffer, 1, file_size, file);
    buffer[read_bytes] = '\0';
    
    char *found = NULL;
    size_t kw_len = strlen(keyword);
    
    if (kw_len == 0) { free(buffer); fclose(file); return; }
    
    if (kw_len > 16) {
        found = strstr(buffer, keyword);
    } else {
        size_t i = 0;
        char first_char = keyword[0];
        
        #if defined(SIMD_ARCH_X86)
            __m256i first_char_vec = _mm256_set1_epi8(first_char);
            for (; i + 32 <= read_bytes; i += 32) {
                __m256i chunk = _mm256_loadu_si256((__m256i*)(buffer + i));
                __m256i cmp = _mm256_cmpeq_epi8(chunk, first_char_vec);
                int mask = _mm256_movemask_epi8(cmp);
                if (mask != 0) {
                    int offset = __builtin_ctz(mask);
                    if (memcmp(buffer + i + offset, keyword, kw_len) == 0) {
                        found = buffer + i + offset;
                        break;
                    }
                }
            }
        #elif defined(SIMD_ARCH_ARM)
            uint8x16_t first_char_vec = vdupq_n_u8(first_char);
            for (; i + 16 <= read_bytes; i += 16) {
                uint8x16_t chunk = vld1q_u8((const uint8_t*)(buffer + i));
                uint8x16_t cmp = vceqq_u8(chunk, first_char_vec);
                uint64_t low = vgetq_lane_u64(vreinterpretq_u64_u8(cmp), 0);
                uint64_t high = vgetq_lane_u64(vreinterpretq_u64_u8(cmp), 1);
                if (low || high) {
                    for (int j = 0; j < 16; j++) {
                        if (buffer[i+j] == first_char && memcmp(buffer + i + j, keyword, kw_len) == 0) {
                            found = buffer + i + j;
                            break;
                        }
                    }
                    if (found) break;
                }
            }
        #else
            found = strstr(buffer, keyword);
        #endif
        
        if (!found && i < read_bytes) {
            found = strstr(buffer + i, keyword);
        }
    }
    
    if (found != NULL) {
        char *line_start = found;
        while (line_start > buffer && *(line_start - 1) != '\n') line_start--;
        
        char *line_end = found;
        while (*line_end != '\0' && *line_end != '\n') line_end++;
        
        char old_char = *line_end;
        *line_end = '\0';
        printf("\033[1;32m%s:\033[0m %s\n", filepath, line_start);
        *line_end = old_char;
    }

    free(buffer);
    fclose(file);
}