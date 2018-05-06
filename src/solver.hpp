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

#ifndef __SOLVER_HPP__
#define __SOLVER_HPP__

#include <vector>

// forward declaration
namespace Minisat {
class SimpSolver;
}

class Solver {
private:
    Minisat::SimpSolver* solver;
    bool solvable; // for immediate failures when adding clauses

public:
    Solver();
    ~Solver();
    void reset();

    int addLiteral();
    int negate(int literal);
    void addClause(int literal1);
    void addClause(int literal1, int literal2);
    void addClause(int literal1, int literal2, int literal3);
    void addClause(const std::vector<int>& lits);

    void simplify();
    bool solve();
    bool getValue(int literal);
};

#endif // __SOLVER_HPP__
