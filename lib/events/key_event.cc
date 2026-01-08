/*
 * key_event.cc Copyright 2026 Alwin Leerling dna.leerling@gmail.com
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

#include "key_event.h"

#include "../core/engine.h"

#include <memory>
#include "../commands/load_request.h"

#include <unordered_map>

void KeyPressEvent::process( Engine &engine )
{
}

void KeyReleaseEvent::process( Engine &engine )
{
	std::unordered_map<char,std::function<void()>> despatchers = {
		{ 0x1B, [&](){engine.stop_running();} },				//escape
		{ '0', [&](){engine.push_command( std::make_unique<LoadRequest>( "../data/data.json" ) );}},
		{ '1', [&](){engine.push_command( std::make_unique<LoadRequest>( "../data/data simple.json" ) );}},
		{ '2', [&](){engine.push_command( std::make_unique<LoadRequest>( "../data/data copy.json" ) );}}
	};

	auto it = despatchers.find( key);
	if( it != despatchers.end() )
		(it->second)();
}
