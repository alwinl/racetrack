/*
 * shader.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "shader.h"
#include <vector>
#include <stdexcept>

#include <glad/gl.h>

void Shader::init( std::string vs_source, std::string fs_source )
{
    if( vs_source.empty() || fs_source.empty() )
		throw std::runtime_error( std::string( "Shader:init: Need both vertex and fragment shader sources" ) );

    id = glCreateProgram();

    unsigned vs_id = glCreateShader( GL_VERTEX_SHADER );
    unsigned fs_id = glCreateShader( GL_FRAGMENT_SHADER );

    compile( vs_id, vs_source );
    compile( fs_id, fs_source );

    link( vs_id, fs_id );
}

void Shader::compile( unsigned id, std::string source )
{
	const char *source_data = source.c_str();

	glShaderSource( id, 1, &source_data, nullptr );
	glCompileShader( id );

	GLint result = -1;
	glGetShaderiv( id, GL_COMPILE_STATUS, &result );

	if( result != GL_TRUE ) {
		GLint length = -1;
		glGetShaderiv( id, GL_INFO_LOG_LENGTH, &length );

		std::vector<char> info_vector( ( length != 0 ) ? length : 1 );
		glGetShaderInfoLog( id, static_cast<GLsizei>( info_vector.size() ), nullptr, info_vector.data() );

		throw std::runtime_error( "Shader::compile" + std::string( info_vector.begin(), info_vector.end() ) );
	}
}

void Shader::link( unsigned vs_id, unsigned fs_id )
{
	glAttachShader( id, vs_id );
	GLenum error = glGetError();
	if( error != GL_NO_ERROR )
		throw std::runtime_error( std::string( "Shader::Link: Cannot attach vertex shader, GLError: " ) + std::to_string( error ) );

	glAttachShader( id, fs_id );
	error = glGetError();
	if( error != GL_NO_ERROR )
		throw std::runtime_error( std::string( "Shader::Link: Cannot attach fragment shader, GLError: " ) + std::to_string( error ) );

	glLinkProgram( id );

	GLint result = -1;
	glGetProgramiv( id, GL_LINK_STATUS, &result );

	glDetachShader( id, vs_id );
	glDetachShader( id, fs_id );

	if( result != GL_TRUE ) {
		GLint length = -1;
		glGetProgramiv( id, GL_INFO_LOG_LENGTH, &length );

		std::vector<char> info_vector( ( length != 0 ) ? length : 1 );
		glGetProgramInfoLog( id, static_cast<GLsizei>( info_vector.size() ), nullptr, info_vector.data() );

		throw std::runtime_error( "Shader::link" + std::string( info_vector.begin(), info_vector.end() ) );
	}

	is_compiled = true;
}

void Shader::activate()
{
    if( !is_compiled )
		throw std::runtime_error( "Shader::activate: Shader is not compiled ( call init() )" );

    glUseProgram( id );
}

void Shader::set_uniform( const std::string &name, glm::mat4 matrix )
{
    activate();
    
	glUniformMatrix4fv( glGetUniformLocation( id, name.c_str() ), 1, GL_FALSE, &matrix[0][0] );
}

