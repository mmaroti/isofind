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
#include <minisat/simp/SimpSolver.h>

Solver::Solver()
    : solver(NULL)
    , solvable(true)
{
    solver = new Minisat::SimpSolver();
}

Solver::~Solver()
{
    if (solver != NULL) {
        delete solver;
        solver = NULL;
    }
}

void Solver::reset()
{
    if (solver != NULL) {
        delete solver;
        solver = new Minisat::SimpSolver();
    }
}

int Solver::addLiteral()
{
    return Minisat::toInt(Minisat::mkLit(solver->newVar(), false));
}

int Solver::negate(int literal)
{
    return Minisat::toInt(~Minisat::toLit(literal));
}

void Solver::addClause(int literal1)
{
    solvable &= solver->addClause(Minisat::toLit(literal1));
}

void Solver::addClause(int literal1, int literal2)
{
    solvable &= solver->addClause(Minisat::toLit(literal1), Minisat::toLit(literal2));
}

void Solver::addClause(int literal1, int literal2, int literal3)
{
    solvable &= solver->addClause(Minisat::toLit(literal1), Minisat::toLit(literal2), Minisat::toLit(literal3));
}

void Solver::addClause(const std::vector<int>& literals)
{
    Minisat::vec<Minisat::Lit> clause(literals.size());
    for (unsigned int i = 0; i < literals.size(); i++)
        clause[i] = Minisat::toLit(literals[i]);
    solvable &= solver->addClause_(clause);
}

void Solver::simplify()
{
    solvable &= solver->simplify();
}

bool Solver::solve()
{
    if (solvable)
        solvable = solver->solve(false, false);
    return solvable;
}

bool Solver::getValue(int literal)
{
    if (solvable) {
        int v = Minisat::toInt(solver->modelValue(Minisat::toLit(literal)));
        return v == 0; // 0 true, 1 false, 2 anything

    } else
        return false;
}
