#include "TestManager.hpp"
#include "Utility.hpp"

TestManager::TestManager(std::mt19937& rng)
    : rng(rng)
    , currentFlashcardIdx(0)
{}

void TestManager::start(NonOwningFlashcards flashcards)
{
    currentFlashcardIdx = 0;
    this->flashcards = shuffled(flashcards, rng);
}

Flashcard& TestManager::currentFlashcard()
{ return *flashcards.at(currentFlashcardIdx); }

const Flashcard& TestManager::currentFlashcard() const
{ return *flashcards.at(currentFlashcardIdx); }

bool TestManager::hasNext() const
{ return currentFlashcardIdx < flashcards.size(); }

void TestManager::skipFlashcard()
{ currentFlashcardIdx++; }

int TestManager::getPercentage() const
{ return 100 * currentFlashcardIdx / flashcards.size(); }
