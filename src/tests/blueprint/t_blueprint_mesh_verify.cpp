//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2014, Lawrence Livermore National Security, LLC.
// 
// Produced at the Lawrence Livermore National Laboratory
// 
// LLNL-CODE-666778
// 
// All rights reserved.
// 
// This file is part of Conduit. 
// 
// For details, see https://lc.llnl.gov/conduit/.
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
/// file: conduit_blueprint_mesh_verify.cpp
///
//-----------------------------------------------------------------------------

#include "conduit.hpp"
#include "blueprint.hpp"
#include "relay.hpp"

#include <vector>
#include <string>
#include "gtest/gtest.h"

using namespace conduit;

/// Helper Functions ///

std::vector<std::string> create_basis(const std::string& d1,
                                      const std::string& d2,
                                      const std::string& d3="")
{
    std::vector<std::string> dim_vector;

    dim_vector.push_back(d1);
    dim_vector.push_back(d2);

    if(d3 != "")
    {
        dim_vector.push_back(d3);
    }

    return dim_vector;
}


bool is_valid_basis(bool (*basis_valid_fun)(const Node&, Node&),
                    const std::vector<std::string>& basis)
{
    Node n, info;

    bool is_valid = true;
    for(index_t bi = 0; bi < basis.size(); bi++)
    {
        const std::string& basis_dim = basis[bi];

        n[basis_dim].set("test");
        is_valid &= !basis_valid_fun(n, info);

        n[basis_dim].set(10);
        is_valid &= basis_valid_fun(n, info);

        // TODO(JRC): Determine whether or not the basis (i, k) should be
        // valid for logical coordinates.
        /*
        if( bi > 0 )
        {
            const std::string& prev_dim = basis[bi-1];
            n.remove(prev_dim);
            is_valid &= !basis_valid_fun(n, info);
            n[basis_dim].set(10);
        }
        */
    }

    return is_valid;
}

/// Mesh Coordinate Set Tests ///

//-----------------------------------------------------------------------------
TEST(conduit_blueprint_mesh_verify, coordset_logical_dims)
{
    bool (*verify_coordset_logical)(const Node&, Node&) = blueprint::mesh::logical_dims::verify;

    Node n, info;
    EXPECT_FALSE(verify_coordset_logical(n, info));

    EXPECT_TRUE(is_valid_basis(verify_coordset_logical,create_basis("i","j","k")));

    EXPECT_FALSE(is_valid_basis(verify_coordset_logical,create_basis("x","y","z")));
}


//-----------------------------------------------------------------------------
TEST(conduit_blueprint_mesh_verify, coordset_uniform_origin)
{
    bool (*verify_uniform_origin)(const Node&, Node&) = blueprint::mesh::coordset::uniform::origin::verify;

    Node n, info;
    EXPECT_FALSE(verify_uniform_origin(n, info));

    EXPECT_TRUE(is_valid_basis(verify_uniform_origin,create_basis("x","y","z")));
    EXPECT_TRUE(is_valid_basis(verify_uniform_origin,create_basis("r","theta","phi")));
    EXPECT_TRUE(is_valid_basis(verify_uniform_origin,create_basis("r","z")));

    const std::vector<std::string> logical_basis = create_basis("i","j","k");
    EXPECT_FALSE(is_valid_basis(verify_uniform_origin,logical_basis));
}


//-----------------------------------------------------------------------------
TEST(conduit_blueprint_mesh_verify, coordset_uniform_spacing)
{
    bool (*verify_uniform_spacing)(const Node&, Node&) = blueprint::mesh::coordset::uniform::spacing::verify;

    Node n, info;
    EXPECT_FALSE(verify_uniform_spacing(n, info));

    EXPECT_TRUE(is_valid_basis(verify_uniform_spacing,create_basis("dx","dy","dz")));
    EXPECT_TRUE(is_valid_basis(verify_uniform_spacing,create_basis("dr","dtheta","dphi")));
    EXPECT_TRUE(is_valid_basis(verify_uniform_spacing,create_basis("dr","dz")));

    EXPECT_FALSE(is_valid_basis(verify_uniform_spacing,create_basis("di","dj","dk")));
}


//-----------------------------------------------------------------------------
TEST(conduit_blueprint_mesh_verify, coordset_types)
{
    // TODO(JRC): Implement this test case and give it a name.
}


//-----------------------------------------------------------------------------
TEST(conduit_blueprint_mesh_verify, coordset_uniform)
{
    // TODO(JRC): Implement this test case and give it a name.
}


//-----------------------------------------------------------------------------
TEST(conduit_blueprint_mesh_verify, coordset_rectilinear)
{
    // TODO(JRC): Implement this test case and give it a name.
}


//-----------------------------------------------------------------------------
TEST(conduit_blueprint_mesh_verify, coordset_explicit)
{
    // TODO(JRC): Implement this test case and give it a name.
}


//-----------------------------------------------------------------------------
TEST(conduit_blueprint_mesh_verify, coordset_general)
{
    // TODO(JRC): Implement this test case and give it a name.
}

/// Mesh Topology Tests ///

//-----------------------------------------------------------------------------
TEST(conduit_blueprint_mesh_verify, TOPOLOGY)
{
    // TODO(JRC): Implement this test case and give it a name.
}

/// Mesh Field Tests ///

//-----------------------------------------------------------------------------
TEST(conduit_blueprint_mesh_verify, FIELD)
{
    // TODO(JRC): Implement this test case and give it a name.
}

/// Mesh Index Tests ///

//-----------------------------------------------------------------------------
TEST(conduit_blueprint_mesh_verify, INDEX)
{
    // TODO(JRC): Implement this test case and give it a name.
}

/// Mesh Integration Tests ///

//-----------------------------------------------------------------------------
TEST(conduit_blueprint_mesh_verify, INTEGRATION)
{
    // TODO(JRC): Implement this test case and give it a name.
}