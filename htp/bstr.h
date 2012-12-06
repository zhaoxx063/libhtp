/***************************************************************************
 * Copyright (c) 2009-2010, Open Information Security Foundation
 * Copyright (c) 2009-2012, Qualys, Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * * Neither the name of the Qualys, Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************************************/

/**
 * @file
 * @author Ivan Ristic <ivanr@webkreator.com>
 */

#ifndef _BSTR_H
#define	_BSTR_H

typedef struct bstr_t bstr_t;
typedef bstr_t bstr;

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "bstr_builder.h"

#ifdef __cplusplus
extern "C" {
#endif

// Data structures
    
struct bstr_t {
    /** The length of the string stored in the buffer. */
    size_t len;

    /** The current size of the buffer. If the buffer is bigger than the
     *  string then it will be able to expand without having to reallocate.
     */
    size_t size;

    /** Optional buffer pointer. If this pointer is NULL (as it currently is
     *  in virtually all cases) the string buffer will immediately follow
     *  this structure. If the pointer is not NUL, it points to the actual
     *  buffer used, and there's no data following this structure.
     */
    char *ptr;
};


// Defines

#define bstr_len(X) ((*(bstr_t *)(X)).len)
#define bstr_size(X) ((*(bstr_t *)(X)).size)
#define bstr_ptr(X) ( ((*(bstr_t *)(X)).ptr == NULL) ? ((char *)(X) + sizeof(bstr_t)) : (char *)(*(bstr_t *)(X)).ptr )


// Functions

/**
 * Append source bstring to destination bstring, growing destination if
 * necessary. If the destination bstring is expanded, the pointer will change.
 * You must replace the original destination pointer with the returned one.
 * Destination is not changed on memory allocation failure.
 *
 * @param[in] bdestination
 * @param[in] bsource
 * @return Updated bstring, or NULL on memory allocation failure.
 */
bstr *bstr_add(bstr *bdestination, const bstr *bsource);

/**
 * Append a NUL-terminated source to destination, growing destination if
 * necessary. If the string is expanded, the pointer will change. You must 
 * replace the original destination pointer with the returned one. Destination
 * is not changed on memory allocation failure.
 *
 * @param[in] b
 * @param[in] cstr
 * @return Updated bstring, or NULL on memory allocation failure.
 */
bstr *bstr_add_c(bstr *b, const char *cstr);

/**
 * Append as many bytes from the source to destination bstring. The
 * destination storage will not be expanded if there is not enough space in it
 * already to accommodate all of the data.
 *
 * @param[in] b
 * @param[in] cstr
 * @return The destination bstring.
 */
bstr *bstr_add_c_noex(bstr *b, const char *cstr);

/**
 * Append a memory region to destination, growing destination if necessary. If
 * the string is expanded, the pointer will change. You must replace the
 * original destination pointer with the returned one. Destination is not
 * changed on memory allocation failure.
 *
 * @param[in] b
 * @param[in] data
 * @param[in] len
 * @return Updated bstring, or NULL on memory allocation failure.
 */
bstr *bstr_add_mem(bstr *, const char *, size_t);

/**
 * Append as many bytes from the source to destination bstring. The
 * destination storage will not be expanded if there is not enough space in it
 * already to accommodate all of the data.
 *
 * @param[in] b
 * @param[in] data
 * @param[in] len
 * @return The destination bstring.
 */
bstr *bstr_add_mem_noex(bstr *b, const char *data, size_t len);

/**
 * Append as many bytes from the source bstring to destination bstring. The
 * destination storage will not be expanded if there is not enough space in it
 * already to accommodate all of the data.
 *
 * @param[in] bdestination
 * @param[in] bsource
 * @return The destination bstring.
 */
bstr *bstr_add_noex(bstr *bdestination, const bstr *bsource);

/**
 * Allocate a zero-length bstring, reserving space for at least size bytes.
 *
 * @param[in] size
 * @return New string instance
 */
bstr *bstr_alloc(size_t size);

/**
 * Checks whether bstring begins with another bstring. Case sensitive.
 * 
 * @param[in] bhaystack
 * @param[in] bneedle
 * @return 1 if true, otherwise 0.
 */
int bstr_begins_with(const bstr *bhaystack, const bstr *bneedle);

/**
 * Checks whether bstring begins with NUL-terminated string. Case sensitive.
 *
 * @param[in] bhaystack
 * @param[in] cneedle
 * @return
 */
int bstr_begins_with_c(const bstr *bhaystack, const char *cneedle);

/**
 * Checks whether bstring begins with NUL-terminated string. Case insensitive.
 *
 * @param[in] bhaystack
 * @param[in] cneedle
 * @return 1 if true, otherwise 0.
 */
int bstr_begins_with_c_nocase(const bstr *bhaystack, const char *cneedle);

/**
 * Checks whether the bstring begins with the given memory block. Case sensitive.
 *
 * @param[in] bhaystack
 * @param[in] data
 * @param[in] len
 * @return 1 if true, otherwise 0.
 */
int bstr_begins_with_mem(const bstr *bhaystack, const char *data, size_t len);
/**
 * Checks whether bstring begins with memory block. Case insensitive.
 *
 * @param[in] bhaystack
 * @param[in] data
 * @param[in] len
 * @return 1 if true, otherwise 0.
 */
int bstr_begins_with_mem_nocase(const bstr *bhaystack, const char *data, size_t len);

/**
 * Checks whether bstring begins with another bstring. Case insensitive.
 *
 * @param[in] bhaystack
 * @param[in] cneedle
 * @return 1 if true, otherwise 0.
 */
int bstr_begins_with_nocase(const bstr *bhaystack, const bstr *cneedle);

/**
 * Return the byte at the given position.
 *
 * @param[in] b
 * @param[in] pos
 * @return The character at the given location, or -1 if the position is out
 *         of range.
 */
int bstr_char_at(const bstr *b, size_t pos);

/**
 * Remove the last byte from bstring, assuming it contains at least one byte.
 *
 * @param[in] b
 */
void bstr_chop(bstr *b);

/**
 * Return the first position of the provided character (byte).
 *
 * @param[in] b
 * @param[in] c
 * @return The first position of the character, or -1 if it could not be found
 */
int bstr_chr(const bstr *b, int c);

/**
 * Case-sensitive comparison of two bstrings.
 *
 * @param[in] b1
 * @param[in] b2
 * @return Zero on string match, 1 if b1 is greater than b2, and -1 if b2 is
 *         greater than b1.
 */
int bstr_cmp(const bstr *b1, const bstr *b2);

/**
 * Case-insensitive comparison two bstrings.
 *
 * @param[in] b1
 * @param[in] b2
 * @return Zero on string match, 1 if b1 is greater than b2, and -1 if b2 is
 *         greater than b1.
 */
int bstr_cmp_nocase(const bstr *b1, const bstr *b2);
  
/**
 * Case-sensitive comparison of a bstring and a NUL-terminated string.
 *
 * @param[in] b
 * @param[in] cstr
 * @return Zero on string match, 1 if b is greater than cstr, and -1 if cstr is
 *         greater than b.
 */
int bstr_cmp_c(const bstr *b, const char *cstr);

/**
 * Case-insensitive comparison of a bstring with a NUL-terminated string.
 *
 * @param[in] b
 * @param[in] cstr
 * @return Zero on string match, 1 if b is greater than cstr, and -1 if cstr is
 *         greater than b.
 */
int bstr_cmp_c_nocase(const bstr *b, const char *cstr);
 
/**
 * Case-sensitive comparison of two memory regions.
 *
 * @param[in] data1
 * @param[in] len1
 * @param[in] data2
 * @param[in] len2
 * @return Zero if the memory regions are identical, 1 if data1 is greater than
 *         data2, and -1 if data2 is greater than data1.
 */
int bstr_cmp_ex(const char *data1, size_t len1, const char *data2, size_t len2);
 
/**
 * Case-insensitive comparison of two memory regions.
 *
 * @param[in] data1
 * @param[in] len1
 * @param[in] data2
 * @param[in] len2
 * @return Zero if the memory regions are identical, 1 if data1 is greater than
 *         data2, and -1 if data2 is greater than data1.
 */
 int bstr_cmp_nocase_ex(const char *data1, size_t len1, const char *data2, size_t len2);

/**
 * Create a new bstring by copying the provided bstring.
 *
 * @param[in] b
 * @return New bstring, or NULL if memory allocation failed
 */
bstr *bstr_dup(const bstr *b);

/**
 * Create a new bstring by copying the provided NUL-terminated string.
 *
 * @param[in] cstr
 * @return New bstring, or NULL if memory allocation failed
 */
bstr *bstr_dup_c(const char *cstr);

/**
 * Create a new bstring by copying a part of the provided bstring.
 *
 * @param[in] b
 * @param[in] offset
 * @param[in] len
 * @return New bstring, or NULL if memory allocation failed
 */
bstr *bstr_dup_ex(const bstr *b, size_t offset, size_t len);

/**
 * Create a copy of the provided bstring, then convert it to lowercase.
 *
 * @param[in] b
 * @return New bstring, or NULL if memory allocation failed
 */
bstr *bstr_dup_lower(const bstr *b);

/**
 * Create a new bstring by copying the provided memory region.
 *
 * @param[in] data
 * @param[in] len
 * @return New bstring, or NULL if memory allocation failed
 */
bstr *bstr_dup_mem(const char *data, size_t len);

/**
 * Expand internal bstring storage to support at least newsize bytes. The input
 * string is not changed if it is already big enough to accommodate the desired
 * size. If the input string is smaller, however, it is expanded. The pointer to
 * the string may change. If the expansion fails, the original string
 * is left untouched.
 *
 * @param[in] b
 * @param[in] newsize
 * @return Updated string instance, or NULL if memory allocation failed
 */
bstr *bstr_expand(bstr *b, size_t newsize);

/**
 * Deallocate the supplied bstring instance and set it to NULL. Allows NULL on
 * input.
 *
 * @param[in] b
 */
void bstr_free(bstr **b);



/**
 * Find the needle in the haystack.
 *
 * @param[in] bhaystack
 * @param[in] bneedle
 * @return Position of the match, or -1 if the needle could not be found.
 */
int bstr_index_of(const bstr *bhaystack, const bstr *bneedle);

/**
 * Find the needle in the haystack, ignoring case differences.
 *
 * @param[in] bhaystack
 * @param[in] bneedle
 * @return Position of the match, or -1 if the needle could not be found.
 */
int bstr_index_of_nocase(const bstr *bhaystack, const bstr *bneedle);

/**
 * Find the needle in the haystack, with the needle being a NUL-terminated
 * string.
 *
 * @param[in] bhaystack
 * @param[in] cneedle
 * @return Position of the match, or -1 if the needle could not be found.
 */
int bstr_index_of_c(const bstr *bhaystack, const char *cneedle);

/**
 * Find the needle in the haystack, with the needle being a NUL-terminated
 * string. Ignore case differences.
 *
 * @param[in] bhaystack
 * @param[in] cneedle
 * @return Position of the match, or -1 if the needle could not be found.
 */
int bstr_index_of_c_nocase(const bstr *bhaystack, const char *cneedle);

/**
 * Find the needle in the haystack, with the needle being a memory region.
 *
 * @param[in] bhaystack
 * @param[in] data
 * @param[in] len
 * @return Position of the match, or -1 if the needle could not be found.
 */
int bstr_index_of_mem(const bstr *bhaystack, const char *data, size_t len);

/**
 * Find the needle in the haystack, with the needle being a memory region.
 * Ignore case differences.
 *
 * @param[in] bhaystack
 * @param[in] data
 * @param[in] len
 * @return Position of the match, or -1 if the needle could not be found.
 */
int bstr_index_of_mem_nocase(const bstr *bhaystack, const char *data, size_t len);

/**
 * Return the last position of a character (byte).
 *
 * @param[in] b
 * @param[in] c
 * @return The last position of the character, or -1 if it could not be found.
 */
int bstr_rchr(const bstr *b, int c);

/**
 * Convert bstring to lowercase.
 *
 * @param[in] b
 * @return The same bstring received on input
 */
bstr *bstr_to_lowercase(bstr *b);

/**
 * Adjust bstring length. You will need to use this method whenever
 * you work directly with the string contents, and end up changing
 * its length by direct structure manipulation.
 *
 * @param[in] b
 * @param[in] newlen
 */
void bstr_util_adjust_len(bstr *b, size_t newlen);

/**
 * Convert contents of a memory region to a positive integer.
 *
 * @param[in] data.
 * @param[in] len
 * @param[in] base The desired number base.
 * @param[in] lastlen Points to the first unused byte in the region
 * @return If the conversion was successful, this function returns the
 *         number. When the conversion fails, -1 will be returned when not
 *         one valid digit was found, and -2 will be returned if an overflow
 *         ocurred.
 */   
int64_t bstr_util_mem_to_pint(const char *data, size_t len, int base, size_t *lastlen);

/**
 * Take the provided memory region, allocate a new memory buffer, and construct
 * a NUL-terminated string, replacing each NUL byte with "\0".
 *
 * @param[in] data
 * @param[in] len
 * @return The newly created NUL-terminated string, or NULL in case of memory
 *         allocation failure.
 */
char *bstr_util_memdup_to_c(const char *data, size_t len);

/**
 * Create a new NUL-terminated string out of the provided bstring.
 *
 * @param[in] b
 * @return The newly created NUL-terminated string, or NULL in case of memory
 *         allocation failure.
 */
char *bstr_util_strdup_to_c(const bstr *b);
  
/**
 * Create a new bstring from the provided NUL-terminated string and without
 * copying the data. The caller must ensure that the input string continues
 * to point to a valid memory location for as long as the bstring is used.
 * 
 * @param[in] cstr
 * @return New bstring, or NULL on memory allocation failure.
 */
bstr *bstr_wrap_c(const char *cstr);

#ifdef __cplusplus
}
#endif

#endif	/* _BSTR_H */
