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
#include "../core/world.h"
#include "../core/engine.h"

#include "../render_pipeline/point_renderer.h"
#include "../render_pipeline/triangle_renderer.h"
#include "../render_pipeline/lake_renderer.h"
#include "../render_pipeline/mesh_renderer.h"

#include "../events/key_event.h"
#include "../events/mouse_event.h"

void RenderSystem::init()
{
    if( !make_window() ) {
        engine->stop_running();
        return;
    }

    make_renderers();

    for( auto& renderer : renderers )
        renderer->init();

    set_camera( glm::mat4( 0.5f ), glm::ortho( -20.f, 20.f, -20.f, 20.f ) );
}

void RenderSystem::input()
{
	glfwPollEvents();
}

void RenderSystem::shutdown()
{
    for( auto& renderer : renderers )
        renderer->destroy();

    if( window )
        glfwDestroyWindow(window);

    glfwTerminate();
}

void RenderSystem::update( double elapsed )
{
    if( glfwWindowShouldClose(window) ) {
        engine->stop_running();
        return;
    }

	auto& world = engine->get_world();

    for( auto& renderer : renderers )
        renderer->upload( world );
}

void RenderSystem::draw()
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

    glfwSetWindowUserPointer( window, engine );

    glfwSetFramebufferSizeCallback 	( window, [](GLFWwindow* window, int width, int height )
    {
        glViewport( 0, 0, width, height );
    });

	glfwSetKeyCallback( window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Engine * engine = static_cast<Engine*>( glfwGetWindowUserPointer( window ) );
		engine->push_event( std::make_unique<KeyEvent>( key, scancode, action, mods) );
	});

	glfwSetCursorPosCallback(window, []( GLFWwindow* window, double xpos, double ypos )
	{
		Engine * engine = static_cast<Engine*>( glfwGetWindowUserPointer( window ) );

		engine->push_event( std::make_unique<MouseEvent>(xpos, ypos, -1, -1, -1) );
	});

	glfwSetMouseButtonCallback(window, []( GLFWwindow* window, int button, int action, int mods )
	{
		Engine * engine = static_cast<Engine*>( glfwGetWindowUserPointer( window ) );

		double xpos;
		double ypos;
		glfwGetCursorPos( window, &xpos, &ypos );

		engine->push_event( std::make_unique<MouseEvent>(xpos, ypos, button, action, mods) );
	});

	// glfwSetCharCallback( window, [](GLFWwindow* window, unsigned int codepoint)
	// {
	// 	Engine * engine = static_cast<Engine*>( glfwGetWindowUserPointer( window ) );
	// });

	return true;
}

void RenderSystem::make_renderers()
{
    // renderers will be drawn in the order of insertion
    renderers.push_back( std::make_unique<LakeRenderer>() );
    renderers.push_back( std::make_unique<PointRenderer>() );
    renderers.push_back( std::make_unique<TriangleRenderer>() );
    renderers.push_back( std::make_unique<MeshRenderer>() );
}
