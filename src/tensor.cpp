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

#include "tensor.hpp"

unsigned int getTensorLength(unsigned int size, unsigned int rank)
{
    unsigned int length = 1;
    for (unsigned int i = 0; i < rank; i++) {
        unsigned int a = length * size;
        if (size > 0 && a / size != length)
            throw std::invalid_argument("too large size and rank");
        length = a;
    }
    return length;
}

unsigned int getTensorIndex(unsigned int size, const std::vector<unsigned int>& pos)
{
    unsigned int index = 0;
    for (unsigned int i = 0; i < pos.size(); i++) {
        assert(pos[i] < size);
        index = index * size + pos[i];
    }
    return index;
}

Tensor<int> getVariableTenor(Solver& solver, unsigned int size, unsigned int rank)
{
    Tensor<int> tensor(size, rank);

    unsigned int n = tensor.getLength();
    for (unsigned int i = 0; i < n; i++)
        tensor.set(i, solver.addLiteral());

    return tensor;
}

Tensor<int> getConstantTensor(const Solver& solver, Tensor<bool> tensor)
{
    int a = solver.getTrue();
    int b = solver.getFalse();

    Tensor<int> tensor2(tensor.size, tensor.rank);

    unsigned int n = tensor.getLength();
    for (unsigned int i = 0; i < n; i++)
        tensor2.set(i, tensor.get(i) ? a : b);

    return tensor2;
}

Tensor<bool> getSolutionTensor(const Solver& solver, Tensor<int> tensor)
{
    Tensor<bool> tensor2(tensor.size, tensor.rank);

    unsigned int n = tensor.getLength();
    for (unsigned int i = 0; i < n; i++)
        tensor2.set(i, solver.getValue(tensor.get(i)));

    return tensor2;
}
