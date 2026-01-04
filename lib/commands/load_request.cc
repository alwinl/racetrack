/*
 * load_request.cc Copyright 2026 Alwin Leerling dna.leerling@gmail.com
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

#include "load_request.h"

#include "../core/engine.h"
#include "../core/world.h"
#include "../core/registry.h"

#include <fstream>

#include "nlohmann/json.hpp"

void LoadRequest::execute( Engine &engine )
{
	auto& registry = engine.get_registry();

    std::ifstream datafile( filename );

    if( !datafile.is_open() )
        return;

    nlohmann::json data;
    datafile >> data;

	registry.clear();

    for( auto& ent : data["entities"] ) {

        Entity e = registry.create_entity();

        for( auto [name, component_data] : ent["components"].items() ) {
            registry.create_component( e, name );
			// registry.set_component_data<JSON>( e, component_data);
		}
    }
}
