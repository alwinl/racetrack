/*
 * renderer.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

 #include <vector>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "system_renderer.h"
#include "world.h"

#include "shader_point_vs.glsl"
#include "shader_point_fs.glsl"

constexpr int max_objects = 10000;

bool RenderSystem::init()
{
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( 640, 480, "Racetrack", NULL, NULL);
    if( !window )
        return false;

    glfwSwapInterval(1);
    glfwMakeContextCurrent(window);

	if( gladLoadGL( glfwGetProcAddress ) == 0 )
        return false;

    shader.init( point_vs, point_fs);
    {
        glm::mat4 proj = glm::ortho(-10.f, 10.f, -10.f, 10.f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 mvp = proj * view;

        shader.set_uniform( "uMVP", mvp );
    }

    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );

    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    glBufferData( GL_ARRAY_BUFFER, sizeof( float) * 3 * max_objects, nullptr, GL_DYNAMIC_DRAW );

    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0 );

    glBindVertexArray(0);

    return true;
}

void RenderSystem::shutdown()
{
    if (vbo)
        glDeleteBuffers(1, &vbo);

    if (vao) 
        glDeleteVertexArrays(1, &vao);

    if( window )
        glfwDestroyWindow(window);

    glfwTerminate();
}

void RenderSystem::begin_frame()
{
	glClearColor( 0.0F, 0.0F, 0.6F, 1.0F );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void RenderSystem::render( const World& world )
{
    std::vector<float> positions;
    positions.reserve( 3 * world.get_transforms().size() );

    for( auto& [entity, transform] : world.get_transforms() ) {
        positions.push_back( transform.x );
        positions.push_back( transform.y );
        positions.push_back( transform.z );
    }

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferSubData( GL_ARRAY_BUFFER, 0, positions.size() * sizeof(float), (void*)positions.data() );

    shader.activate();

    glBindVertexArray( vao );

    glDrawArrays( GL_POINTS, 0, positions.size() / 3 );

    glBindVertexArray( 0 );
}

void RenderSystem::end_frame()
{
    glfwSwapBuffers(window);        
}

bool RenderSystem::should_close() const
{
	return glfwWindowShouldClose(window);
}
