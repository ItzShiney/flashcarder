#pragma once

#include <random>
#include <algorithm>
#include <QSize>

template<class C>
void shuffle(C& container, std::mt19937& rng)
{
    const auto random0to = [&rng](qsizetype n)
    { return std::uniform_int_distribution<qsizetype>(0, n - 1)(rng); };

    std::random_shuffle(std::begin(container), std::end(container), random0to);
}

template<class C>
C shuffled(C container, std::mt19937& rng)
{
    shuffle(container, rng);
    return container;
}

template<class... Args>
QString replaced(QString str, Args... args)
{
    str.replace(args...);
    return str;
}

template<template<class> class C, class T>
C<T*> toNonOwning(C<T>& container)
{
    C<T*> res;
    for (auto& x: container)
        res.push_back(&x);
    return res;
}