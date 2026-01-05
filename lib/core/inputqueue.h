/*
 * inputqueue.h Copyright 2025 Alwin Leerling <dna.leerling@gmail.com>
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

#include <memory>
#include <queue>

#include "../events/event.h"

class InputQueue
{
public:
	void push( std::unique_ptr<IEvent> event )
		{ events.push( std::move(event) ); }

	void process( Engine& engine )
	{
		auto event = pop();

		while( event ) {
			event->process( engine );
			event = pop();
		}
	}

private:
	std::queue<std::unique_ptr<IEvent>> events;

	std::unique_ptr<IEvent> pop()
	{
		if( events.empty() )
			return nullptr;

		auto event = std::move(events.front());
		events.pop();
		return event;
	}
};
