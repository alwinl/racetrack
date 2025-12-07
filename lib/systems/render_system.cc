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
#include <glm/gtc/matrix_transform.hpp>

#include "render_system.h"
#include "../world.h"
#include "../engine.h"

#include "rendering/point_renderer.h"
#include "rendering/triangle_renderer.h"
#include "rendering/track_renderer.h"
#include "rendering/lake_renderer.h"

void RenderSystem::init( Engine& engine )
{
    this->engine = &engine;

    if( !make_window() ) {
        engine.stop_running();
        return;
    }

    make_renderers();
    set_camera( glm::mat4( 0.5f ), glm::ortho( -20.f, 20.f, -20.f, 20.f ) );
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
    if( glfwWindowShouldClose(window) ) {
        engine->stop_running();
        return;
    }

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
	glClearColor( 0.2F, 0.8F, 0.2F, 1.0F );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void RenderSystem::end_frame()
{
    glfwSwapBuffers(window);        
}

void RenderSystem::set_camera( const glm::mat4 &view, const glm::mat4 &proj )
{
    mvp = view * proj;

    for( auto& r : renderers )
        r->set_mvp(mvp);
}

bool RenderSystem::make_window()
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

    return true;
}

void RenderSystem::make_renderers()
{
    renderers.push_back( std::make_unique<LakeRenderer>() );
    renderers.push_back( std::make_unique<TrackRenderer>() );
    renderers.push_back( std::make_unique<PointRenderer>() );
    renderers.push_back( std::make_unique<TriangleRenderer>() );

    for( auto& renderer : renderers )
        renderer->init();
}
