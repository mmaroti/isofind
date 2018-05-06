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
    TRUE = addLiteral();
    addClause(TRUE);
    FALSE = negate(FALSE);
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
        TRUE = addLiteral();
        addClause(TRUE);
        FALSE = negate(FALSE);
    }
}

int Solver::addLiteral()
{
    return Minisat::toInt(Minisat::mkLit(solver->newVar(), false));
}

int Solver::negate(int literal) const
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

bool Solver::getValue(int literal) const
{
    if (solvable) {
        int v = Minisat::toInt(solver->modelValue(Minisat::toLit(literal)));
        return v == 0; // 0 true, 1 false, 2 anything

    } else
        return false;
}

int Solver::logicAnd(int literal1, int literal2)
{
    if (literal1 == FALSE || literal2 == FALSE)
        return FALSE;
    else if (literal1 == TRUE)
        return literal2;
    else if (literal2 == TRUE)
        return literal1;

    int literal3 = addLiteral();
    addClause(literal1, negate(literal3));
    addClause(literal2, negate(literal3));
    addClause(negate(literal1), negate(literal2), literal3);
    return literal3;
}

int Solver::logicOr(int literal1, int literal2)
{
    if (literal1 == TRUE || literal2 == TRUE)
        return TRUE;
    else if (literal1 == FALSE)
        return literal2;
    else if (literal2 == FALSE)
        return literal1;

    int literal3 = addLiteral();
    addClause(negate(literal1), literal3);
    addClause(negate(literal2), literal3);
    addClause(literal1, literal2, negate(literal3));
    return literal3;
}

int Solver::logicAll(const std::vector<int>& literals)
{
    unsigned int trueCount = 0;
    int lastNotTrue = TRUE;
    for (unsigned int i = 0; i < literals.size(); i++) {
        if (literals[i] == FALSE)
            return FALSE;
        else if (literals[i] == TRUE)
            trueCount += 1;
        else
            lastNotTrue = literals[i];
    }

    if (trueCount + 1 >= literals.size())
        return lastNotTrue;

    int result = addLiteral();
    int negated = negate(result);
    std::vector<int> clause;
    for (unsigned int i = 0; i < literals.size(); i++) {
        if (literals[i] != TRUE) {
            addClause(literals[i], negated);
            clause.push_back(negate(literals[i]));
        }
    }
    clause.push_back(result);
    addClause(clause);
    return result;
}

int Solver::logicAny(const std::vector<int>& literals)
{
    unsigned int falseCount = 0;
    int lastNotFalse = FALSE;
    for (unsigned int i = 0; i < literals.size(); i++) {
        if (literals[i] == TRUE)
            return TRUE;
        else if (literals[i] == FALSE)
            falseCount += 1;
        else
            lastNotFalse = literals[i];
    }

    if (falseCount + 1 >= literals.size())
        return lastNotFalse;

    int result = addLiteral();
    int negated = negate(result);
    std::vector<int> clause;
    for (unsigned int i = 0; i < literals.size(); i++) {
        if (literals[i] != FALSE) {
            addClause(negate(literals[i]), result);
            clause.push_back(literals[i]);
        }
    }
    clause.push_back(negated);
    addClause(clause);
    return result;
}
