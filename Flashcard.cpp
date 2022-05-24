#include "Flashcard.hpp"

QString Flashcard::formatted() const
{
    QString res = "<b>СЛОВО</b>\n" + word + "\n\n<b>ЗНАЧЕНИЕ</b>\n" + meaning;

    if (!notes.isEmpty())
        res += "\n\n<b>ПРИМЕЧАНИЯ</b>\n" + notes;

    res.replace('\n', "<br/>");
    return res;
}

QDebug& operator<<(QDebug& l, Flashcard r)
{ return l << r.word << r.meaning << r.notes; }