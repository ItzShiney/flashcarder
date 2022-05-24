#include "FileManager.hpp"
#include "Utility.hpp"
#include <QRegularExpression>

FileManager::FileManager(const QString& path)
    : path(path)
{}

QString prepare(QString str)
{
    str.replace('\\', "\\\\");
    str.replace('|', "\\|");
    str.replace('\n', "\\n");

    return str;
}

QString unprepare(QString str)
{
    str.replace("\\n", "\n");
    str.replace("\\|", "|");
    str.replace("\\\\", "\\");

    return str;
}

void FileManager::save(const Flashcards& flashcards)
{
    QFile file(path);
    assert(file.open(QIODevice::WriteOnly | QIODevice::Truncate));
    QTextStream output(&file);

    for (const auto& flashcard: flashcards)
        output
            << prepare(flashcard.word)
            << "|"
            << prepare(flashcard.meaning)
            << "|"
            << prepare(flashcard.notes)
            << '\n';
}

Flashcards FileManager::load()
{
    QFile file(path);
    assert(file.open(QIODevice::ReadOnly));
    QTextStream input(&file);

    Flashcards res;
    while (true)
    {
        QString line = input.readLine();

        if (line.isEmpty())
        {
            if (input.atEnd())
                break;
            continue;
        }

        static const auto splitRegex = QRegularExpression(R"((?<!\\)\|)");

        const auto parts = line.split(splitRegex);
        const auto word = unprepare(std::move(parts.at(0)));
        const auto meaning = unprepare(std::move(parts.at(1)));
        const auto notes = unprepare(std::move(parts.at(2)));

        res.append(Flashcard {word, meaning, notes});
    }
    return res;
}
