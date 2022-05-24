#pragma once

#include "Flashcards.hpp"
#include <QFile>

class FileManager
{
public:
    FileManager(const QString& path);

    void save(const Flashcards&);
    Flashcards load();

private:
    QString path;
};
