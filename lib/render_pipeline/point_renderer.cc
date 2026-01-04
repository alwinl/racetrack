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

#include <glad/gl.h>

#include <glm/glm.hpp>

#include "../core/world.h"
#include "../components/point_component.h"
#include "../components/transform_component.h"

#include "../core/view.h"

static const char* point_vs = R"(
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
uniform mat4 uMVP;

out vec3 col;

void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
    gl_PointSize = 10.0;
    col = aCol;
}
)";

static const char* point_fs = R"(
#version 330 core

in vec3 col;
out vec4 FragColor;

void main() {
    FragColor = vec4(col, 1.0);
}
)";

constexpr int max_objects = 10000;

void PointRenderer::init()
{
    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );

    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    glBufferData( GL_ARRAY_BUFFER, sizeof(vertex) * max_objects, nullptr, GL_DYNAMIC_DRAW );

    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof( vertex, position ) );

    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof( vertex, colour ) );

    glBindVertexArray( 0 );

    glEnable(GL_PROGRAM_POINT_SIZE);

    shader.init( point_vs, point_fs );
}

void PointRenderer::upload( const World& world )
{
    cpu_buffer.clear();

	// world.for_each_component<PointComponent>( [&]( Entity entity, const PointComponent& point )
	// {
    //     if( auto* transform = world.get_component<TransformComponent>( entity ) )
    //         cpu_buffer.push_back( {transform->translation, point.colour } );
    // });

	for( const auto [entity, point, transform] : world.view<PointComponent,TransformComponent>() )
        cpu_buffer.push_back( {transform.translation, point.colour } );

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferSubData( GL_ARRAY_BUFFER, 0, cpu_buffer.size() * sizeof( vertex ), (void*)cpu_buffer.data() );
}

void PointRenderer::draw()
{
    shader.activate();

    glBindVertexArray( vao );
    glDrawArrays( GL_POINTS, 0, cpu_buffer.size() );
    glBindVertexArray( 0 );
}

void PointRenderer::set_mvp( glm::mat4 &mvp )
{
    shader.set_uniform( "uMVP", mvp );
}

void PointRenderer::destroy()
{
    if( vbo ) glDeleteBuffers( 1, &vbo );
    if( vao ) glDeleteVertexArrays( 1, &vao );
}
