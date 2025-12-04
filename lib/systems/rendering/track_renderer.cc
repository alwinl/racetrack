/*
 * track_renderer.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "track_renderer.h"

#include <glad/gl.h>

#include <glm/glm.hpp>

#include "../../world.h"
#include "../../components/track.h"
#include "../../components/transform.h"

static const char* track_vs = R"(
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

static const char* track_fs = R"(
#version 330 core

in vec3 col;
out vec4 FragColor;

void main() {
    FragColor = vec4(col, 1.0);
}
)";


#include <iostream>

constexpr int max_objects = 10000;

void TrackRenderer::init()
{
    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );

    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    glBufferData( GL_ARRAY_BUFFER, sizeof(vertex) * max_objects, nullptr, GL_DYNAMIC_DRAW );

    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position) );

    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, colour) );

    glBindVertexArray( 0 );

    shader.init( track_vs, track_fs );
}

void TrackRenderer::upload( const World& world )
{
    cpu_buffer.clear();

    const auto& tracks = world.storage<TrackComponent>();

    for( const auto& [entity, track] : tracks.all() ) {

        float half_width = 0.5f;

        for( size_t i = 0; i+1 < track.centreline_points.size(); ++i ) {

            glm::vec3 p0 = track.centreline_points[i];
            glm::vec3 p1 = track.centreline_points[i+1];

            glm::vec3 t = glm::normalize( p1 - p0 );
            glm::vec3 n = glm::normalize( glm::vec3(-t.y, t.x, 0) );

            glm::vec3 left0  = p0 + n * half_width;
            glm::vec3 right0 = p0 - n * half_width;

            glm::vec3 left1  = p1 + n * half_width;
            glm::vec3 right1 = p1 - n * half_width;

            glm::vec3 track_colour(0,0,0);

            cpu_buffer.push_back( {left0, track_colour} );
            cpu_buffer.push_back( {right0, track_colour} );
            cpu_buffer.push_back( {left1, track_colour} );

            cpu_buffer.push_back( {right0, track_colour} );
            cpu_buffer.push_back( {right1, track_colour} );
            cpu_buffer.push_back( {left1, track_colour} );
        }

    }

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferSubData( GL_ARRAY_BUFFER, 0, cpu_buffer.size() * sizeof( vertex ), (void*)cpu_buffer.data() );

}

void TrackRenderer::draw()
{
    shader.activate();

    glBindVertexArray( vao );
    glDrawArrays( GL_TRIANGLES, 0, cpu_buffer.size() );
    glBindVertexArray( 0 );
}

void TrackRenderer::set_mvp( glm::mat4 &mvp )
{
    shader.set_uniform( "uMVP", mvp );
}

void TrackRenderer::destroy()
{
    if( vbo ) glDeleteBuffers( 1, &vbo );
    if( vao ) glDeleteVertexArrays( 1, &vao );
}
