 /*
  * lake_renderer.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "lake_renderer.h"

#include <glad/gl.h>

#include <glm/glm.hpp>

#include "../core/world.h"
#include "../core/view.h"
#include "../components/lake_component.h"

static const char* lake_vs = R"(
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

static const char* lake_fs = R"(
#version 330 core

in vec3 col;
out vec4 FragColor;

void main() {
    FragColor = vec4(col, 1.0);
}
)";

constexpr int max_objects = 10000;

void LakeRenderer::init()
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

    shader.init(lake_vs, lake_fs);
}

#include <iostream>

void LakeRenderer::upload( const World& world )
{
    cpu_buffer.clear();

	for( auto [entity, lake] : world.view<LakeComponent>() )
    {
        glm::vec3 lake_colour( 0.0f, 0.3f, 1.0f);
        glm::vec3 island_colour( 0.2f, 0.8f, 0.2f);

#ifdef DRAW_OUTLINE
        for( const glm::vec2& p : lake.lake_outline )
            cpu_buffer.push_back( { glm::vec3(p.x,p.y,0.0f), lake_colour } );

        for( const glm::vec2& p : lake.island_outline )
            cpu_buffer.push_back( { glm::vec3(p.x,p.y,0.0f), island_colour } );

        lake_vertices = lake.lake_outline.size();
        island_vertices = lake.island_outline.size();
#else
        if (lake.lake_outline.size() >= 3) {

            glm::vec3 p0(lake.lake_outline[0].x, lake.lake_outline[0].y, 0.0f);

            for (size_t i = 1; i + 1 < lake.lake_outline.size(); i++) {

                glm::vec3 p1( lake.lake_outline[i    ].x, lake.lake_outline[i    ].y, 0.0f );
                glm::vec3 p2( lake.lake_outline[i + 1].x, lake.lake_outline[i + 1].y, 0.0f );

                cpu_buffer.push_back( {p0, lake_colour} );
                cpu_buffer.push_back( {p1, lake_colour} );
                cpu_buffer.push_back( {p2, lake_colour} );
            }
        }

        if( lake.island_outline.size() >= 3 ) {

            glm::vec3 p0(lake.island_outline[0].x, lake.island_outline[0].y, 0.0f);

            for (size_t i = 1; i + 1 < lake.island_outline.size(); i++) {

                glm::vec3 p1( lake.island_outline[i    ].x, lake.island_outline[i    ].y, 0.0f );
                glm::vec3 p2( lake.island_outline[i + 1].x, lake.island_outline[i + 1].y, 0.0f );

                cpu_buffer.push_back( {p0, island_colour} );
                cpu_buffer.push_back( {p1, island_colour} );
                cpu_buffer.push_back( {p2, island_colour} );
            }
        }
#endif
    };

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferSubData( GL_ARRAY_BUFFER, 0, cpu_buffer.size() * sizeof(vertex), (void*)cpu_buffer.data() );
}

void LakeRenderer::draw()
{
    shader.activate();

    glBindVertexArray(vao);

#ifdef DRAW_OUTLINE
    glDrawArrays( GL_LINE_LOOP, 0, lake_vertices );
    glDrawArrays( GL_LINE_LOOP, lake_vertices, island_vertices );
#else
    glDrawArrays( GL_TRIANGLES, 0, cpu_buffer.size() );
#endif

    glBindVertexArray(0);

}

void LakeRenderer::set_mvp( glm::mat4 &mvp )
{
    shader.set_uniform( "uMVP", mvp );
}

void LakeRenderer::destroy()
{
    if( vbo ) glDeleteBuffers( 1, &vbo );
    if( vao ) glDeleteVertexArrays( 1, &vao );
}
