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

#include <fstream>
#include <algorithm>
#include "common.h"
#include "check.h"
#include "omp.h"
#include "memkind.h"
#include "trial_generator.h"


class GBPagesTest: public :: testing::Test
{

protected:
    size_t num_bandwidth;
    int *bandwidth;
    TrialGenerator *tgen;
    void SetUp()
    {
        size_t node;
        char *hbw_nodes_env, *endptr;
        tgen = new TrialGenerator();

        hbw_nodes_env = getenv("MEMKIND_HBW_NODES");
        if (hbw_nodes_env) {
            num_bandwidth = 128;
            bandwidth = new int[num_bandwidth];
            for (node = 0; node < num_bandwidth; node++) {
                bandwidth[node] = 1;
            }
            node = strtol(hbw_nodes_env, &endptr, 10);
            bandwidth[node] = 2;
            while (*endptr == ':') {
                hbw_nodes_env = endptr + 1;
                node = strtol(hbw_nodes_env, &endptr, 10);
                if (endptr != hbw_nodes_env && node >= 0 && node < num_bandwidth) {
                    bandwidth[node] = 2;
                }
            }
        }
        else {
            const char *node_bandwidth_path = "/etc/memkind/node-bandwidth";
            std::ifstream nbw_file;

            nbw_file.open(node_bandwidth_path, std::ifstream::binary);
            nbw_file.seekg(0, nbw_file.end);
            num_bandwidth = nbw_file.tellg()/sizeof(int);
            nbw_file.seekg(0, nbw_file.beg);
            bandwidth = new int[num_bandwidth];
            nbw_file.read((char *)bandwidth, num_bandwidth*sizeof(int));
            nbw_file.close();
        }
    }

    void TearDown()
    {
        delete[] bandwidth;
        delete tgen;
    }

};

TEST_F(GBPagesTest, hbw_memalign_psize_incremental)
{
    tgen->generate_hbw_gb_incremental(HBW_MEMALIGN_PSIZE);
    tgen->run(num_bandwidth, bandwidth);
}

TEST_F(GBPagesTest, memkind_malloc)
{
    tgen->generate_gb_incremental(MEMKIND_MALLOC);
    tgen->run(num_bandwidth, bandwidth);
}

TEST_F(GBPagesTest, memkind_calloc)
{
    tgen->generate_gb_incremental(MEMKIND_CALLOC);
    tgen->run(num_bandwidth, bandwidth);
}

TEST_F(GBPagesTest, memkind_realloc)
{
    tgen->generate_gb_incremental(MEMKIND_REALLOC);
    tgen->run(num_bandwidth, bandwidth);
}

TEST_F(GBPagesTest, memkind_posix_memalign)
{
    tgen->generate_gb_incremental(MEMKIND_POSIX_MEMALIGN);
    tgen->run(num_bandwidth, bandwidth);
}

TEST_F(GBPagesTest, hbw_memalign_psize_incremental_strict)
{
    tgen->generate_hbw_gb_strict(HBW_MEMALIGN_PSIZE);
    tgen->run(num_bandwidth, bandwidth);
}


TEST_F(GBPagesTest, memkind_malloc_regular)
{
    tgen->generate_gb_regular(MEMKIND_MALLOC);
    tgen->run(num_bandwidth, bandwidth);
}

TEST_F(GBPagesTest, memkind_calloc_regular)
{
    tgen->generate_gb_regular(MEMKIND_CALLOC);
    tgen->run(num_bandwidth, bandwidth);
}

TEST_F(GBPagesTest, memkind_realloc_regular)
{
    tgen->generate_gb_regular(MEMKIND_REALLOC);
    tgen->run(num_bandwidth, bandwidth);
}

TEST_F(GBPagesTest, memkind_posix_memalign_regular)
{
    tgen->generate_gb_regular(MEMKIND_POSIX_MEMALIGN);
    tgen->run(num_bandwidth, bandwidth);
}

TEST_F(GBPagesTest, hbw_memalign_psize_incremental_bind)
{
    hbw_set_policy(1);
    EXPECT_EQ(1, hbw_get_policy());
    tgen->generate_hbw_gb_incremental(HBW_MEMALIGN_PSIZE);
    tgen->run(num_bandwidth, bandwidth);
}

TEST_F(GBPagesTest, hbw_memalign_psize_strict_bind)
{
    tgen->generate_hbw_gb_strict(HBW_MEMALIGN_PSIZE);
    tgen->run(num_bandwidth, bandwidth);
}




