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

#include "../../world.h"
#include "../../components/mesh_component.h"


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

constexpr int max_objects = 10000;

void MeshRenderer::init()
{
    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );

    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    glBufferData( GL_ARRAY_BUFFER, max_objects * sizeof(vertex), nullptr, GL_DYNAMIC_DRAW );

    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof( vertex, position) );

    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof( vertex, colour) );

    glBindVertexArray( 0 );

    shader.init( mesh_vs, mesh_fs );
}

void MeshRenderer::upload( const World& world )
{
    cpu_outline_buffer.clear();
    cpu_filled_buffer.clear();

    const auto& shapes = world.storage<MeshComponent>();

    for( const auto& [entity, shape] : shapes.all() )
    {
        if( !shape.filled || (shape.outline.size() < 3) ) {
            for( const glm::vec2& p : shape.outline )
                cpu_outline_buffer.push_back( { glm::vec3(p.x,p.y,0.0f), shape.colour } );
        } else {

            glm::vec3 p0(shape.outline[0].x, shape.outline[0].y, 0.0f);

            for (size_t i = 1; i + 1 < shape.outline.size(); i++) {

                glm::vec3 p1( shape.outline[i    ].x, shape.outline[i    ].y, 0.0f );
                glm::vec3 p2( shape.outline[i + 1].x, shape.outline[i + 1].y, 0.0f );

                cpu_filled_buffer.push_back( {p0, shape.colour} );
                cpu_filled_buffer.push_back( {p1, shape.colour} );
                cpu_filled_buffer.push_back( {p2, shape.colour} );
            }
        }
    }

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferSubData( GL_ARRAY_BUFFER, 0, cpu_filled_buffer.size() * sizeof(vertex), (void*)cpu_filled_buffer.data() );
    glBufferSubData( GL_ARRAY_BUFFER, cpu_filled_buffer.size() * sizeof(vertex), cpu_outline_buffer.size() * sizeof(vertex), (void*)cpu_outline_buffer.data() );
}

void MeshRenderer::draw()
{
    shader.activate();

    glBindVertexArray(vao);

    if( cpu_filled_buffer.size() )
        glDrawArrays( GL_TRIANGLES, 0, cpu_filled_buffer.size() );

    if( cpu_outline_buffer.size() )
        glDrawArrays( GL_LINE_LOOP, cpu_filled_buffer.size(), cpu_outline_buffer.size() );

    glBindVertexArray(0);

}

void MeshRenderer::set_mvp( glm::mat4 &mvp )
{
    shader.set_uniform( "uMVP", mvp );
}

void MeshRenderer::destroy()
{
    if( vbo ) glDeleteBuffers( 1, &vbo );
    if( vao ) glDeleteVertexArrays( 1, &vao );
}
