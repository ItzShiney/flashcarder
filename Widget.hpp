#pragma once

#include "TestManager.hpp"
#include "FileManager.hpp"
#include <QWidget>
#include <random>

QT_BEGIN_NAMESPACE
namespace Ui
{ class Widget; }
QT_END_NAMESPACE

class Widget: public QWidget
{
    Q_OBJECT

public:
    Widget(const QString& filePath, QWidget* parent = nullptr);
    ~Widget();

private slots:
    void on_AddFlashcardButton_clicked();
    void on_StartButton_clicked();

    void on_TestYesButton_clicked();
    void on_TestNoButton_clicked();
    void on_TestOkButton_clicked();

private:

    void updateProgressBar();
    void updateFlashcardsList();

    bool isFullTest() const;
    bool isQuickTest() const;
    void goToTestStartLayout();
    void startTest();
    void nextFlashcard();
    void goToDoYouRememberQuestion();
    void goToDidYouRememberQuestion();
    void goToTryToRememberQuestion();
    bool isDoYouRememberQuestion() const;
    bool isDidYouRememberQuestion() const;
    bool isTryToRememberQuestion() const;

    void setYesNoLayout();
    void setOkLayout();

    void invalidState();

    Ui::Widget* ui;

    void flashcardsUpdated();
    FileManager fileManager;

    void addToNotLearned();
    Flashcards flashcards;
    NonOwningFlashcards nonLearned;

    std::mt19937 rng;
    TestManager testManager;
};
