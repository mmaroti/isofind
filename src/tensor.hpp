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

#ifndef __TENSOR_HPP__
#define __TENSOR_HPP__

#include "solver.hpp"
#include <cassert>
#include <stdexcept>

unsigned int getTensorLength(unsigned int size, unsigned int rank);
unsigned int getTensorIndex(unsigned int size, const std::vector<unsigned int>& pos);

template <typename elem_t>
class Tensor {
public:
    const unsigned int size;
    const unsigned int rank;

    Tensor(unsigned int size, unsigned int rank)
        : size(size)
        , rank(rank)
        , elements(getTensorLength(size, rank))
    {
    }

    elem_t get(const std::vector<unsigned int>& pos) const
    {
        assert(rank == pos.size());
        return elements[getTensorIndex(size, pos)];
    }

    elem_t get(unsigned int index) const
    {
        assert(0 <= index && index < elements.size());
        return elements[index];
    }

    void set(const std::vector<unsigned int>& pos, elem_t elem)
    {
        assert(rank == pos.size());
        elements[getTensorIndex(size, pos)] = elem;
    }

    void set(unsigned int index, elem_t elem)
    {
        assert(0 <= index && index < elements.size());
        elements[index] = elem;
    }

    unsigned int getLength() const
    {
        return elements.size();
    }

private:
    std::vector<elem_t> elements;
};

Tensor<int> getVariableTensor(Solver& solver, unsigned int size, unsigned int rank);
Tensor<int> getConstantTensor(const Solver& solver, Tensor<bool> tensor);
Tensor<bool> getSolutionTensor(const Solver& solver, Tensor<int> tensor);

#endif // __TENSOR_HPP__
