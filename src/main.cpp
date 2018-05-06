/*
 * Copyright 2018 Miklos Maroti.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "solver.hpp"
#include <iostream>

int main()
{
    Solver solver;
    int a = solver.addLiteral();
    int b = solver.addLiteral();
    solver.addClause(a, b);
    solver.addClause(solver.negate(a));
    std::cout << solver.solve() << " " << solver.getValue(a) << " " << solver.getValue(b) << std::endl;
    solver.addClause(solver.negate(b));
    std::cout << solver.solve() << " " << solver.getValue(a) << " " << solver.getValue(b) << std::endl;
}
