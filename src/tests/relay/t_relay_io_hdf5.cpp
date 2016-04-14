//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2014-2016, Lawrence Livermore National Security, LLC.
// 
// Produced at the Lawrence Livermore National Laboratory
// 
// LLNL-CODE-666778
// 
// All rights reserved.
// 
// This file is part of Conduit. 
// 
// For details, see: http://software.llnl.gov/conduit/.
// 
// Please also read conduit/LICENSE
// 
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the disclaimer below.
// 
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the disclaimer (as noted below) in the
//   documentation and/or other materials provided with the distribution.
// 
// * Neither the name of the LLNS/LLNL nor the names of its contributors may
//   be used to endorse or promote products derived from this software without
//   specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL LAWRENCE LIVERMORE NATIONAL SECURITY,
// LLC, THE U.S. DEPARTMENT OF ENERGY OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
// DAMAGES  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.
// 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//-----------------------------------------------------------------------------
///
/// file: t_relay_io_hdf5.cpp
///
//-----------------------------------------------------------------------------

#include "relay.hpp"
#include "hdf5.h"
#include <iostream>
#include "gtest/gtest.h"

using namespace conduit;
using namespace conduit::relay;


//-----------------------------------------------------------------------------
TEST(conduit_io_hdf5, conduit_hdf5_write_read_by_file_name)
{
    uint32 a_val = 20;
    uint32 b_val = 8;
    uint32 c_val = 13;

    Node n;
    n["a"] = a_val;
    n["b"] = b_val;
    n["c"] = c_val;

    EXPECT_EQ(n["a"].as_uint32(), a_val);
    EXPECT_EQ(n["b"].as_uint32(), b_val);
    EXPECT_EQ(n["c"].as_uint32(), c_val);

    // write our node as a group @ "myobj"
    io::hdf5_write(n,"tout_hdf5_wr.hdf5:myobj");

    // directly read our object
    Node n_load;
    io::hdf5_read("tout_hdf5_wr.hdf5:myobj",n_load);
    
    EXPECT_EQ(n_load["a"].as_uint32(), a_val);
    EXPECT_EQ(n_load["b"].as_uint32(), b_val);
    EXPECT_EQ(n_load["c"].as_uint32(), c_val);

    Node n_load_2;
    // read from root of hdf5 file
    io::hdf5_read("tout_hdf5_wr.hdf5",n_load_2);
    
    EXPECT_EQ(n_load_2["myobj/a"].as_uint32(), a_val);
    EXPECT_EQ(n_load_2["myobj/b"].as_uint32(), b_val);
    EXPECT_EQ(n_load_2["myobj/c"].as_uint32(), c_val);


    Node n_load_generic;
    // read from root of hdf5 file
    io::load("tout_hdf5_wr.hdf5",n_load_generic);
    
    EXPECT_EQ(n_load_generic["myobj/a"].as_uint32(), a_val);
    EXPECT_EQ(n_load_generic["myobj/b"].as_uint32(), b_val);
    EXPECT_EQ(n_load_generic["myobj/c"].as_uint32(), c_val);
    
    
    // save load from generic io interface 
    io::save(n_load_generic,"tout_hdf5_wr_generic.hdf5:myobj");
    io::load_merged("tout_hdf5_wr_generic.hdf5",n_load_generic);
    
    EXPECT_EQ(n_load_generic["myobj/a"].as_uint32(), a_val);
    EXPECT_EQ(n_load_generic["myobj/b"].as_uint32(), b_val);
    EXPECT_EQ(n_load_generic["myobj/c"].as_uint32(), c_val);
    
    

}

//-----------------------------------------------------------------------------
// This variant tests when a caller code has already opened a HDF5 file
// and has a handle ready.
TEST(conduit_io_hdf5, conduit_hdf5_write_read_by_file_handle)
{
    uint32 a_val = 20;
    uint32 b_val = 8;
    uint32 c_val = 13;

    Node n;
    n["a"] = a_val;
    n["b"] = b_val;
    n["c"] = c_val;

    EXPECT_EQ(n["a"].as_uint32(), a_val);
    EXPECT_EQ(n["b"].as_uint32(), b_val);
    EXPECT_EQ(n["c"].as_uint32(), c_val);

    std::string test_file_name = "tout_hdf5_write_read_by_file_handle.hdf5";
    hid_t h5_file_id, h5_group_id;
    herr_t status;

    // Set up hdf5 file and group that caller code would already have.
    h5_file_id = H5Fcreate(test_file_name.c_str(),
                           H5F_ACC_TRUNC,
                           H5P_DEFAULT,
                           H5P_DEFAULT);

    // Prepare group that caller code wants conduit to save it's tree to that
    // group. (could also specify group name for conduit to create via
    // hdf5_path argument to write call.
    h5_group_id = H5Gcreate(h5_file_id,
                            "sample_group_name",
                            H5P_DEFAULT,
                            H5P_DEFAULT,
                            H5P_DEFAULT);

    // It looks like the hdf5_write call modifies the passed in hid_t???
    io::hdf5_write(n,h5_group_id,".");

    // I can't close the group, the hdf5_write appears to have modified the h5_group_id.
    // I get an error about it not being a group anymore.  If I comment out the hdf5_write
    // call, I'm able to close the file.
    status = H5Gclose(h5_group_id);
    status = H5Fclose(h5_file_id);

    h5_file_id = H5Fopen(test_file_name.c_str(),
                         H5F_ACC_RDONLY,
                         H5P_DEFAULT);

    // Caller code switches to group it wants to read in. (could also
    // specify group name for conduit to read out via hdf5_path arg to read
    // call)
    h5_group_id = H5Gopen(h5_file_id, "sample_group_name", 0);
                          
    Node n_load;

    io::hdf5_read(h5_group_id, ".", n_load);
    
    status = H5Gclose(h5_group_id);
    status = H5Fclose(h5_file_id);

    EXPECT_EQ(n_load["a"].as_uint32(), a_val);
    EXPECT_EQ(n_load["b"].as_uint32(), b_val);
    EXPECT_EQ(n_load["c"].as_uint32(), c_val);
    
}

//-----------------------------------------------------------------------------
TEST(conduit_io_hdf5, conduit_hdf5_write_read_string)
{
    uint32 a_val = 20;
    
    std::string s_val = "{string value!}";

    Node n;
    n["a"] = a_val;
    n["s"] = s_val;

    EXPECT_EQ(n["a"].as_uint32(), a_val);
    EXPECT_EQ(n["s"].as_string(), s_val);

    // write our node as a group @ "myobj"
    io::hdf5_write(n,"tout_hdf5_wr_string.hdf5:myobj");

    Node n_out;
    
    io::hdf5_read("tout_hdf5_wr_string.hdf5:myobj",n_out);
    
    EXPECT_EQ(n_out["a"].as_uint32(), a_val);
    EXPECT_EQ(n_out["s"].as_string(), s_val);
    

}

//-----------------------------------------------------------------------------
TEST(conduit_io_hdf5, conduit_hdf5_write_read_array)
{
    Node n_in(DataType::float64(10));
    
    float64_array val_in = n_in.value();
    
    for(index_t i=0;i<10;i++)
    {
        val_in[i] = 3.1415 * i;
    }

    // write our node as a group @ "myobj"
    io::hdf5_write(n_in,"tout_hdf5_wr_array.hdf5:myobj");

    Node n_out;
    
    io::hdf5_read("tout_hdf5_wr_array.hdf5:myobj",n_out);
    
    float64_array val_out = n_out.value();
    
    
    for(index_t i=0;i<10;i++)
    {
        EXPECT_EQ(val_in[i],val_out[i]);
    }

}






