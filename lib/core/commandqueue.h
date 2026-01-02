/*
 * commandqueue.h Copyright 2026 Alwin Leerling dna.leerling@gmail.com
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

#include "../commands/command.h"

class CommandQueue
{
public:
	void push( std::unique_ptr<ICommand> cmd )
		{ command_queue.push( std::move(cmd) ); }

	std::unique_ptr<ICommand> pop()
	{
		if( empty() )
			return nullptr;

		auto cmd = std::move(command_queue.front());
		command_queue.pop();
		return cmd;
	}

	bool empty() const { return command_queue.empty(); };

private:
	std::queue<std::unique_ptr<ICommand>> command_queue;
};
