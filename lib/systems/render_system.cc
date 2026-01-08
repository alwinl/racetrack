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

#include <array>

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

std::array<char, GLFW_KEY_LAST> key_mapping;

void init_key_mapping()
{
	key_mapping[GLFW_KEY_SPACE]			= ' '; //              32
	key_mapping[GLFW_KEY_APOSTROPHE]	= '\''; //         39  /* ' */
	key_mapping[GLFW_KEY_COMMA]			= ','; //              44  /* , */
	key_mapping[GLFW_KEY_MINUS]			= '-'; //              45  /* - */
	key_mapping[GLFW_KEY_PERIOD]		= '.'; //             46  /* . */
	key_mapping[GLFW_KEY_SLASH]			= '/'; //              47  /* / */
	key_mapping[GLFW_KEY_0]				= '0'; //                  48
	key_mapping[GLFW_KEY_1]				= '1'; //                  49
	key_mapping[GLFW_KEY_2]				= '2'; //                  50
	key_mapping[GLFW_KEY_3]				= '3'; //                  51
	key_mapping[GLFW_KEY_4]				= '4'; //                  52
	key_mapping[GLFW_KEY_5]				= '5'; //                  53
	key_mapping[GLFW_KEY_6]				= '6'; //                  54
	key_mapping[GLFW_KEY_7]				= '7'; //                  55
	key_mapping[GLFW_KEY_8]				= '8'; //                  56
	key_mapping[GLFW_KEY_9]				= '9'; //                  57
	key_mapping[GLFW_KEY_SEMICOLON]		= ';'; //          59  /* ; */
	key_mapping[GLFW_KEY_EQUAL]			= '='; //              61  /* = */
	key_mapping[GLFW_KEY_A]				= 'A'; //                  65
	key_mapping[GLFW_KEY_B]				= 'B'; //                  66
	key_mapping[GLFW_KEY_C]				= 'C'; //                  67
	key_mapping[GLFW_KEY_D]				= 'D'; //                  68
	key_mapping[GLFW_KEY_E]				= 'E'; //                  69
	key_mapping[GLFW_KEY_F]				= 'F'; //                  70
	key_mapping[GLFW_KEY_G]				= 'G'; //                  71
	key_mapping[GLFW_KEY_H]				= 'H'; //                  72
	key_mapping[GLFW_KEY_I]				= 'I'; //                  73
	key_mapping[GLFW_KEY_J]				= 'J'; //                  74
	key_mapping[GLFW_KEY_K]				= 'K'; //                  75
	key_mapping[GLFW_KEY_L]				= 'L'; //                  76
	key_mapping[GLFW_KEY_M]				= 'M'; //                  77
	key_mapping[GLFW_KEY_N]				= 'N'; //                  78
	key_mapping[GLFW_KEY_O]				= 'O'; //                  79
	key_mapping[GLFW_KEY_P]				= 'P'; //                  80
	key_mapping[GLFW_KEY_Q]				= 'Q'; //                  81
	key_mapping[GLFW_KEY_R]				= 'R'; //                  82
	key_mapping[GLFW_KEY_S]				= 'S'; //                  83
	key_mapping[GLFW_KEY_T]				= 'T'; //                  84
	key_mapping[GLFW_KEY_U]				= 'U'; //                  85
	key_mapping[GLFW_KEY_V]				= 'V'; //                  86
	key_mapping[GLFW_KEY_W]				= 'W'; //                  87
	key_mapping[GLFW_KEY_X]				= 'X'; //                  88
	key_mapping[GLFW_KEY_Y]				= 'Y'; //                  89
	key_mapping[GLFW_KEY_Z]				= 'Z'; //                  90
	key_mapping[GLFW_KEY_LEFT_BRACKET]	= '['; //       91  /* [ */
	key_mapping[GLFW_KEY_BACKSLASH]		= '\\'; //          92  /* \ */
	key_mapping[GLFW_KEY_RIGHT_BRACKET]	= ']'; //      93  /* ] */
	key_mapping[GLFW_KEY_GRAVE_ACCENT]	= '`'; //       96  /* ` */
	// key_mapping[GLFW_KEY_WORLD_1]		= ; //            161 /* non-US #1 */
	// key_mapping[GLFW_KEY_WORLD_2]		= ; //            162 /* non-US #2 */
	key_mapping[GLFW_KEY_ESCAPE]		= 0x1B; //             256
	key_mapping[GLFW_KEY_ENTER]			= 0x0D; //              257
	key_mapping[GLFW_KEY_TAB]			= 0x09; //                258
	key_mapping[GLFW_KEY_BACKSPACE]		= 0x08; //          259
	// key_mapping[GLFW_KEY_INSERT]		= ; //             260
	key_mapping[GLFW_KEY_DELETE]		= 0x7F; //             261
	// key_mapping[GLFW_KEY_RIGHT]			= ; //              262
	// key_mapping[GLFW_KEY_LEFT]			= ; //               263
	// key_mapping[GLFW_KEY_DOWN]			= ; //               264
	// key_mapping[GLFW_KEY_UP]			= ; //                 265
	// key_mapping[GLFW_KEY_PAGE_UP]		= ; //            266
	// key_mapping[GLFW_KEY_PAGE_DOWN]		= ; //          267
	// key_mapping[GLFW_KEY_HOME]			= ; //               268
	// key_mapping[GLFW_KEY_END]			= ; //                269
	// key_mapping[GLFW_KEY_CAPS_LOCK]		= ; //          280
	// key_mapping[GLFW_KEY_SCROLL_LOCK]	= ; //        281
	// key_mapping[GLFW_KEY_NUM_LOCK]		= ; //           282
	// key_mapping[GLFW_KEY_PRINT_SCREEN]	= ; //       283
	// key_mapping[GLFW_KEY_PAUSE]			= ; //              284
	// key_mapping[GLFW_KEY_F1]			= ; //                 290
	// key_mapping[GLFW_KEY_F2]			= ; //                 291
	// key_mapping[GLFW_KEY_F3]			= ; //                 292
	// key_mapping[GLFW_KEY_F4]			= ; //                 293
	// key_mapping[GLFW_KEY_F5]			= ; //                 294
	// key_mapping[GLFW_KEY_F6]			= ; //                 295
	// key_mapping[GLFW_KEY_F7]			= ; //                 296
	// key_mapping[GLFW_KEY_F8]			= ; //                 297
	// key_mapping[GLFW_KEY_F9]			= ; //                 298
	// key_mapping[GLFW_KEY_F10]			= ; //                299
	// key_mapping[GLFW_KEY_F11]			= ; //                300
	// key_mapping[GLFW_KEY_F12]			= ; //                301
	// key_mapping[GLFW_KEY_F13]			= ; //                302
	// key_mapping[GLFW_KEY_F14]			= ; //                303
	// key_mapping[GLFW_KEY_F15]			= ; //                304
	// key_mapping[GLFW_KEY_F16]			= ; //                305
	// key_mapping[GLFW_KEY_F17]			= ; //                306
	// key_mapping[GLFW_KEY_F18]			= ; //                307
	// key_mapping[GLFW_KEY_F19]			= ; //                308
	// key_mapping[GLFW_KEY_F20]			= ; //                309
	// key_mapping[GLFW_KEY_F21]			= ; //                310
	// key_mapping[GLFW_KEY_F22]			= ; //                311
	// key_mapping[GLFW_KEY_F23]			= ; //                312
	// key_mapping[GLFW_KEY_F24]			= ; //                313
	// key_mapping[GLFW_KEY_F25]			= ; //                314
	key_mapping[GLFW_KEY_KP_0]			= '0'; //               320
	key_mapping[GLFW_KEY_KP_1]			= '1'; //               321
	key_mapping[GLFW_KEY_KP_2]			= '2'; //               322
	key_mapping[GLFW_KEY_KP_3]			= '3'; //               323
	key_mapping[GLFW_KEY_KP_4]			= '4'; //               324
	key_mapping[GLFW_KEY_KP_5]			= '5'; //               325
	key_mapping[GLFW_KEY_KP_6]			= '6'; //               326
	key_mapping[GLFW_KEY_KP_7]			= '7'; //               327
	key_mapping[GLFW_KEY_KP_8]			= '8'; //               328
	key_mapping[GLFW_KEY_KP_9]			= '9'; //               329
	key_mapping[GLFW_KEY_KP_DECIMAL]	= '.'; //         330
	key_mapping[GLFW_KEY_KP_DIVIDE]		= '\\'; //          331
	key_mapping[GLFW_KEY_KP_MULTIPLY]	= '*'; //        332
	key_mapping[GLFW_KEY_KP_SUBTRACT]	= '-'; //        333
	key_mapping[GLFW_KEY_KP_ADD]		= '+'; //             334
	key_mapping[GLFW_KEY_KP_ENTER]		= 0x0D; //           335
	key_mapping[GLFW_KEY_KP_EQUAL]		= '='; //           336
	// key_mapping[GLFW_KEY_LEFT_SHIFT]	= ; //         340
	// key_mapping[GLFW_KEY_LEFT_CONTROL]	= ; //       341
	// key_mapping[GLFW_KEY_LEFT_ALT]		= ; //           342
	// key_mapping[GLFW_KEY_LEFT_SUPER]	= ; //         343
	// key_mapping[GLFW_KEY_RIGHT_SHIFT]	= ; //        344
	// key_mapping[GLFW_KEY_RIGHT_CONTROL]	= ; //      345
	// key_mapping[GLFW_KEY_RIGHT_ALT]		= ; //          346
	// key_mapping[GLFW_KEY_RIGHT_SUPER]	= ; //        347
	// key_mapping[GLFW_KEY_MENU]			= ; //               348
}

void RenderSystem::init()
{
    if( !make_window() ) {
        engine->stop_running();
        return;
    }

	init_key_mapping();

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
		if( (action == GLFW_PRESS) || (action == GLFW_REPEAT) )
			engine->push_event( std::make_unique<KeyPressEvent>( key_mapping[key], scancode, mods) );
		else
			engine->push_event( std::make_unique<KeyReleaseEvent>( key_mapping[key], scancode, mods) );
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
