#pragma once

#include <QString>
#include <QDebug>

struct Flashcard
{
    QString word;
    QString meaning;
    QString notes;

    QString formatted() const;
};

QDebug& operator<<(QDebug&, Flashcard);