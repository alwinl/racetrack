 /*
  * triangle_renderer.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "triangle_renderer.h"

#include <glad/gl.h>

#include <glm/glm.hpp>

#include "../core/world.h"
#include "../components/triangle_component.h"
#include "../components/transform_component.h"


static const char* triangle_vs = R"(
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

static const char* triangle_fs = R"(
#version 330 core

in vec3 col;
out vec4 FragColor;

void main() {
    FragColor = vec4(col, 1.0);
}
)";

constexpr int max_objects = 10000;

void TriangleRenderer::init()
{
    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );

    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    glBufferData( GL_ARRAY_BUFFER, 3 * max_objects * sizeof(vertex), nullptr, GL_DYNAMIC_DRAW );

    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof( vertex, position) );

    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof( vertex, colour) );

    glBindVertexArray( 0 );

    shader.init(triangle_vs, triangle_fs);
}

void TriangleRenderer::upload( const World& world )
{
    cpu_buffer.clear();

	world.for_each_component<TriangleComponent>( [&]( Entity entity, const TriangleComponent& tri )
    {
        if( const auto* transform = world.get_component<TransformComponent>( entity ) ) {

            for( int i = 0; i < 3; i++ )
                cpu_buffer.push_back( {tri.vertices[i] + transform->translation, tri.colour } );
        }
    } );

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferSubData( GL_ARRAY_BUFFER, 0, cpu_buffer.size() * sizeof(vertex), (void*)cpu_buffer.data() );
}

void TriangleRenderer::draw()
{
    shader.activate();

    glBindVertexArray(vao);
    glDrawArrays( GL_TRIANGLES, 0, cpu_buffer.size() );
    glBindVertexArray(0);

}

void TriangleRenderer::set_mvp( glm::mat4 &mvp )
{
    shader.set_uniform( "uMVP", mvp );
}

void TriangleRenderer::destroy()
{
    if( vbo ) glDeleteBuffers( 1, &vbo );
    if( vao ) glDeleteVertexArrays( 1, &vao );
}
