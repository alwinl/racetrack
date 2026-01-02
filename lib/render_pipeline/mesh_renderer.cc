 /*
  * mesh_renderer.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program; if not, write to the Free Software
  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
  * MA 02110-1301, USA.
  */

#include "mesh_renderer.h"

#include <glad/gl.h>

#include <glm/glm.hpp>

#include "../core/world.h"
#include "../components/mesh_component.h"


std::unordered_map<MeshComponent::Topology, GLenum> topology_map = {
    { MeshComponent::Topology::POINTS, GL_POINTS},
    { MeshComponent::Topology::LINE_STRIP, GL_LINE_STRIP},
    { MeshComponent::Topology::LINE_LOOP, GL_LINE_LOOP},
    { MeshComponent::Topology::LINES, GL_LINES},
    { MeshComponent::Topology::TRIANGLE_STRIP, GL_TRIANGLE_STRIP},
    { MeshComponent::Topology::TRIANGLE_FAN, GL_TRIANGLE_FAN},
    { MeshComponent::Topology::TRIANGLES, GL_TRIANGLES}
};

static const char* mesh_vs = R"(
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
uniform mat4 uMVP;

out vec3 col;

void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
    col = aCol;
}
)";

static const char* mesh_fs = R"(
#version 330 core

in vec3 col;
out vec4 FragColor;

void main() {
    FragColor = vec4(col, 1.0);
}
)";

void MeshRenderer::init()
{
    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );
    glGenBuffers( 1, &ibo );

    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );

    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof( vertex, position) );

    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof( vertex, colour) );

    glBindVertexArray( 0 );

    shader.init( mesh_vs, mesh_fs );
}

void MeshRenderer::upload( const World& world )
{
    std::vector<vertex> vertex_buffer;
    std::vector<unsigned int> index_buffer;
    size_t vertex_buffer_idx = 0;

    draw_commands.clear();

    const auto& meshes = world.storage<MeshComponent>();

    for( const auto& [entity, mesh] : meshes.all() )
    {
        DrawCommand draw_command;

        draw_command.mode = topology_map[mesh.topology];

        assert(mesh.vertices.size() == mesh.colours.size());

        for( int i = 0; i < mesh.vertices.size(); ++i )
            vertex_buffer.push_back( { mesh.vertices[i], mesh.colours[i] });

        if( !mesh.indices.empty() ) {

            draw_command.indexed = true;
            draw_command.start = index_buffer.size();
            draw_command.count = mesh.indices.size();

            std::transform( mesh.indices.begin(), mesh.indices.end(), std::back_inserter(index_buffer), [vertex_buffer_idx]( unsigned int index){ return index + vertex_buffer_idx; } );
        } else {
            draw_command.start = vertex_buffer_idx;
            draw_command.count = mesh.vertices.size();
        }

        draw_commands.emplace_back( draw_command );

        vertex_buffer_idx += mesh.vertices.size();
    }

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, vertex_buffer.size() * sizeof(vertex), (void*)vertex_buffer.data(), GL_DYNAMIC_DRAW );

    if( !index_buffer.empty() ) {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, index_buffer.size() * sizeof(unsigned int), (void*)index_buffer.data(), GL_DYNAMIC_DRAW );
    }
}

void MeshRenderer::draw()
{
    shader.activate();

    glBindVertexArray(vao);

    for( auto draw_command : draw_commands ) {
        if( draw_command.indexed )
            glDrawElements( draw_command.mode, draw_command.count, GL_UNSIGNED_INT, (void *)(draw_command.start * sizeof(unsigned int)) );
        else
            glDrawArrays( draw_command.mode, draw_command.start, draw_command.count );
    }

    glBindVertexArray(0);

}

void MeshRenderer::set_mvp( glm::mat4 &mvp )
{
    shader.set_uniform( "uMVP", mvp );
}

void MeshRenderer::destroy()
{
    if( ibo ) glDeleteBuffers( 1, &ibo );
    if( vbo ) glDeleteBuffers( 1, &vbo );
    if( vao ) glDeleteVertexArrays( 1, &vao );
}
