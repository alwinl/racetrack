 /*
  * pointrenderer.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "point_renderer.h"

#include <string>

#include <glad/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


std::string point_vs = R"(
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 uMVP;

void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
    gl_PointSize = 10.0;
}
)";

std::string point_fs = R"(
#version 330 core

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
)";

constexpr int max_objects = 10000;

void PointRenderer::init()
{
    shader.init( point_vs, point_fs );
    {
        glm::mat4 proj = glm::ortho( -10.f, 10.f, -10.f, 10.f );
        glm::mat4 view = glm::mat4( 1.0f );
        glm::mat4 mvp = proj * view;

        shader.set_uniform( "uMVP", mvp );
    }

    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );

    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 3 * max_objects, nullptr, GL_DYNAMIC_DRAW );

    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), (void *)0 );

    glBindVertexArray( 0 );
}

void PointRenderer::upload( const World& world )
{
    cpu_buffer.clear();
    cpu_buffer.reserve( 3 * world.get_transforms().size() );

    for( auto& [entity, transform] : world.get_transforms() ) {
        cpu_buffer.push_back( transform.x );
        cpu_buffer.push_back( transform.y );
        cpu_buffer.push_back( transform.z );
    }

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferSubData( GL_ARRAY_BUFFER, 0, cpu_buffer.size() * sizeof(float), (void*)cpu_buffer.data() );
}

void PointRenderer::draw()
{
    shader.activate();

    glBindVertexArray( vao );
    glDrawArrays( GL_POINTS, 0, cpu_buffer.size() / 3 );
    glBindVertexArray( 0 );
}

void PointRenderer::destroy()
{
    if( vbo )
        glDeleteBuffers( 1, &vbo );

    if( vao ) 
        glDeleteVertexArrays( 1, &vao );
}
