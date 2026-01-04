/*
 * view.h Copyright 2026 Alwin Leerling dna.leerling@gmail.com
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

#pragma once

#include <tuple>

#include "world.h"

template<typename... Ts>
class View
{
public:
    View( const World& world ) : world(world) {}

    struct Iterator
	{
        using Primary = std::tuple_element_t<0, std::tuple<Ts...>>;

		Iterator( const World& world ) : world(world )
		{
 			world.for_each_entity<Primary>(
				[&]( Entity e ) {
                	if( (world.get_component<Ts>(e) && ...) )
                    	entities.push_back(e);
            	});
		}

        auto operator*() const { return std::tuple<Entity, Ts&...>(entities[index], *world.get_component<Ts>(entities[index])...); }
        Iterator& operator++() { ++index; return *this; }
        bool operator!=(const Iterator& other) const { return index != other.index; }

		void set_end() { index = entities.size(); }

	private:
        const World& world;
		std::vector<Entity> entities;
		size_t index = 0;
    };

    Iterator begin() { return Iterator(world); }

	Iterator end() const
	{
        Iterator it(world);
        it.set_end();
        return it;
    }
private:
    const World& world;
};

template<typename... Ts> View<Ts...> World::view() { return View<Ts...>(*this); }
template<typename... Ts> View<const Ts...> World::view() const { return View<const Ts...>(*this); }
