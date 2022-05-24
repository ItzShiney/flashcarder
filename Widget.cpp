#include "Widget.hpp"
#include "ui_Widget.h"
#include "Utility.hpp"

Widget::Widget(const QString& path, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)

    , fileManager(path)
    , flashcards()
    , rng(std::chrono::high_resolution_clock::now().time_since_epoch().count())
    , testManager(rng)
{
    ui->setupUi(this);
    flashcards = fileManager.load();
    updateFlashcardsList();
}

Widget::~Widget()
{ delete ui; }

void Widget::on_AddFlashcardButton_clicked()
{
    const auto word = ui->WordEdit->toPlainText();
    const auto meaning = ui->MeaningEdit->toPlainText();
    const auto examples = ui->ExamplesEdit->toPlainText();

    flashcards.push_back(Flashcard {word, meaning, examples});

    flashcardsUpdated();

    ui->WordEdit->clear();
    ui->MeaningEdit->clear();
    ui->ExamplesEdit->clear();
}

void Widget::on_StartButton_clicked()
{ startTest(); }

void Widget::on_TestYesButton_clicked()
{
    if (isDoYouRememberQuestion())
        goToDidYouRememberQuestion();
    else if (isDidYouRememberQuestion())
        nextFlashcard();
    else
        invalidState();
}

void Widget::on_TestNoButton_clicked()
{
    if (isDoYouRememberQuestion())
    {
        goToTryToRememberQuestion();
        addToNotLearned();
    }
    else if (isDidYouRememberQuestion())
    {
        addToNotLearned();
        nextFlashcard();
    }
    else
        invalidState();
}

void Widget::on_TestOkButton_clicked()
{
    if (!isTryToRememberQuestion())
    {
        invalidState();
        return;
    }

    nextFlashcard();
}

void Widget::addToNotLearned()
{ nonLearned.push_back(&testManager.currentFlashcard()); }

void Widget::updateProgressBar()
{ ui->TestProgressBar->setValue(testManager.getPercentage()); }

void Widget::updateFlashcardsList()
{
    auto& table = *ui->FlashcardsList;

    table.setRowCount(flashcards.size());

    for (qsizetype i = 0; i < flashcards.size(); i++)
    {
        const auto& flashcard = flashcards[i];

        table.setItem(i, 0, new QTableWidgetItem(flashcard.word));
        table.setItem(i, 1, new QTableWidgetItem(flashcard.meaning));
        table.setItem(i, 2, new QTableWidgetItem(flashcard.notes));
    }

    table.sortItems(0);
}

bool Widget::isFullTest() const
{ return ui->FullTestRadioButton->isChecked(); }

bool Widget::isQuickTest() const
{ return ui->QuickTestRadioButton->isChecked(); }

void Widget::goToTestStartLayout()
{ ui->TestPages->setCurrentWidget(ui->TestStartLayout); }

void Widget::startTest()
{
    if (isFullTest())
        testManager.start(toNonOwning(flashcards));
    else if (isQuickTest())
        testManager.start(nonLearned);
    else
        invalidState();

    nonLearned.clear();

    if (!testManager.hasNext())
    {
        if (isFullTest())
            ui->Tabs->setCurrentWidget(ui->AddFlashcardTab);
        else if (isQuickTest())
            ui->FullTestRadioButton->setChecked(true);
        else
            invalidState();
        return;
    }

    updateProgressBar();
    goToDoYouRememberQuestion();
    ui->TestPages->setCurrentWidget(ui->TestLayout);
}

void Widget::nextFlashcard()
{
    testManager.skipFlashcard();
    updateProgressBar();

    if (testManager.hasNext())
        goToDoYouRememberQuestion();
    else
        goToTestStartLayout();
}

void Widget::goToDoYouRememberQuestion()
{
    ui->TestQuestion->setCurrentWidget(ui->WordToMeaningDoYouRemember);
    setYesNoLayout();
    ui->TestInformation->setPlainText(testManager.currentFlashcard().word);
}

void Widget::goToDidYouRememberQuestion()
{
    ui->TestQuestion->setCurrentWidget(ui->DidYouRemember);
    setYesNoLayout();
    ui->TestInformation->document()->setHtml(testManager.currentFlashcard().formatted());
}

void Widget::goToTryToRememberQuestion()
{
    ui->TestQuestion->setCurrentWidget(ui->TryToRemember);
    setOkLayout();
    ui->TestInformation->document()->setHtml(testManager.currentFlashcard().formatted());
}

void Widget::setYesNoLayout()
{ ui->TestButtons->setCurrentWidget(ui->TestYesNoLayout); }

void Widget::setOkLayout()
{ ui->TestButtons->setCurrentWidget(ui->TestOkLayout); }

bool Widget::isDoYouRememberQuestion() const
{ return ui->TestQuestion->currentWidget() == ui->WordToMeaningDoYouRemember || ui->TestQuestion->currentWidget() == ui->MeaningToWordDoYouRemember; }

bool Widget::isDidYouRememberQuestion() const
{ return ui->TestQuestion->currentWidget() == ui->DidYouRemember; }

bool Widget::isTryToRememberQuestion() const
{ return ui->TestQuestion->currentWidget() == ui->TryToRemember; }

void Widget::invalidState()
{ assert(!"Invalid state"); }

void Widget::flashcardsUpdated()
{
    fileManager.save(flashcards);
    updateFlashcardsList();
}