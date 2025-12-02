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

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "system_renderer.h"
#include "../world.h"

#include "point_renderer.h"
#include "triangle_renderer.h"

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

    glEnable(GL_PROGRAM_POINT_SIZE);

    renderers.push_back( std::make_unique<PointRenderer>() );
    renderers.push_back( std::make_unique<TriangleRenderer>() );

    for( auto& renderer : renderers )
        renderer->init();

    return true;
}

void RenderSystem::shutdown()
{
    for( auto& renderer : renderers )
        renderer->destroy();

    if( window )
        glfwDestroyWindow(window);

    glfwTerminate();
}

void RenderSystem::update( World& world, double dt )
{
    for( auto& renderer : renderers )
        renderer->upload( world );
}

void RenderSystem::draw( World& world )
{
    begin_frame();

    for( auto& renderer : renderers )
        renderer->draw();

    end_frame();
}

void RenderSystem::begin_frame()
{
	glClearColor( 0.0F, 0.0F, 0.6F, 1.0F );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void RenderSystem::end_frame()
{
    glfwSwapBuffers(window);        
}

bool RenderSystem::should_close() const
{
	return glfwWindowShouldClose(window);
}
