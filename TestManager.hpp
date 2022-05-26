#pragma once

#include "Flashcards.hpp"
#include <random>

class TestManager
{
public:
    TestManager(std::mt19937& rng);

    void start(NonOwningFlashcards);

    Flashcard& currentFlashcard();
    const Flashcard& currentFlashcard() const;

    bool hasNext() const;
    void skipFlashcard();

    int getPercentage() const;

private:
    std::mt19937& rng;

    NonOwningFlashcards flashcards;
    qsizetype currentFlashcardIdx;
};
