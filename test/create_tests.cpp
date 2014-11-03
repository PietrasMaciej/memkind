/*
 * Copyright (C) 2014 Intel Corporation.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice(s),
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice(s),
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
 * EVENT SHALL THE COPYRIGHT HOLDER(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <memkind.h>
#include <gtest/gtest.h>

static const int NUM_BAD_OPS = 9;
static const struct memkind_ops MEMKIND_BAD_OPS[NUM_BAD_OPS]= {{
    .create = NULL,
    .destroy = memkind_default_destroy,
    .malloc = memkind_default_malloc,
    .calloc = memkind_default_calloc,
    .posix_memalign = memkind_default_posix_memalign,
    .realloc = memkind_default_realloc,
    .free = memkind_default_free,
    .get_size = memkind_default_get_size
}, {
    .create = memkind_default_create,
    .destroy = NULL,
    .malloc = memkind_default_malloc,
    .calloc = memkind_default_calloc,
    .posix_memalign = memkind_default_posix_memalign,
    .realloc = memkind_default_realloc,
    .free = memkind_default_free,
    .get_size = memkind_default_get_size
}, {
    .create = memkind_default_create,
    .destroy = memkind_default_destroy,
    .malloc = NULL,
    .calloc = memkind_default_calloc,
    .posix_memalign = memkind_default_posix_memalign,
    .realloc = memkind_default_realloc,
    .free = memkind_default_free,
    .get_size = memkind_default_get_size
}, {
    .create = memkind_default_create,
    .destroy = memkind_default_destroy,
    .malloc = memkind_default_malloc,
    .calloc = NULL,
    .posix_memalign = memkind_default_posix_memalign,
    .realloc = memkind_default_realloc,
    .free = memkind_default_free,
    .get_size = memkind_default_get_size
}, {
    .create = memkind_default_create,
    .destroy = memkind_default_destroy,
    .malloc = memkind_default_malloc,
    .calloc = memkind_default_calloc,
    .posix_memalign = NULL,
    .realloc = memkind_default_realloc,
    .free = memkind_default_free,
    .get_size = memkind_default_get_size
}, {
    .create = memkind_default_create,
    .destroy = memkind_default_destroy,
    .malloc = memkind_default_malloc,
    .calloc = memkind_default_calloc,
    .posix_memalign = memkind_default_posix_memalign,
    .realloc = NULL,
    .free = memkind_default_free,
    .get_size = memkind_default_get_size
}, {
    .create = memkind_default_create,
    .destroy = memkind_default_destroy,
    .malloc = memkind_default_malloc,
    .calloc = memkind_default_calloc,
    .posix_memalign = memkind_default_posix_memalign,
    .realloc = memkind_default_realloc,
    .free = NULL,
    .get_size = memkind_default_get_size
}, {
    .create = memkind_default_create,
    .destroy = memkind_default_destroy,
    .malloc = memkind_default_malloc,
    .calloc = memkind_default_calloc,
    .posix_memalign = memkind_default_posix_memalign,
    .realloc = memkind_default_realloc,
    .free = memkind_default_free,
    .get_size = NULL
}, {
    .create = memkind_default_create,
    .destroy = memkind_default_destroy,
    .malloc = memkind_default_malloc,
    .calloc = memkind_default_calloc,
    .posix_memalign = memkind_default_posix_memalign,
    .realloc = memkind_default_realloc,
    .free = memkind_default_free,
    .get_size = memkind_default_get_size,
    .init_once = memkind_hbw_init_once
}};

class MemkindCreate: public :: testing :: Test { };

TEST_F(MemkindCreate, bad_ops)
{
    int i, err;
    memkind_t *kind;
    for (i = 0; i < NUM_BAD_OPS; ++i) {
        err = memkind_create(MEMKIND_BAD_OPS + i, "bad_ops", &kind);
        EXPECT_TRUE(err = MEMKIND_ERROR_BADOPS);
        EXPECT_TRUE(kind == NULL);
    }
}

TEST_F(MemkindCreate, rep_name)
{
    int i, err;
    memkind_t *kind;
    for (i = 0; i < NUM_BAD_OPS; ++i) {
        err = memkind_create(MEMKIND_DEFAULT_OPS, "memkind_default", &kind);
        EXPECT_TRUE(err = MEMKIND_ERROR_REPNAME);
        EXPECT_TRUE(kind == NULL);
    }
}
