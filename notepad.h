#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QFontDialog>
//#include <QPrintDialog>

#include "constants.h"
#include "utils.h"
#include "DialogAbout.h"

namespace Ui
{
    class Notepad;
}

class LineNumberArea;

class Notepad : public QMainWindow
{
    Q_OBJECT

public: //public functions
    explicit Notepad(QWidget *parent = 0);
    ~Notepad();

private slots: //events
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_actionPrint_triggered();
    void on_actionExit_triggered();
    void on_actionCopy_triggered();
    void on_actionCut_triggered();
    void on_actionPaste_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionFont_triggered();
    void on_actionAbout_triggered();

    void on_textEdit_selectionChanged();
    void on_textEdit_textChanged();
    void on_textEdit_undoAvailable(bool b);
    void on_textEdit_redoAvailable(bool b);
    void on_textEdit_copyAvailable(bool b);

    void mousePressEvent(QMouseEvent *event);

private: //private function
    bool save(const QString fileName);
    void ChangeTextModified();

private: //private variables
    Ui::Notepad *ui;
    QString currentFileName;

private: //private properties
    bool _isTextModified;
    bool get_isTextModified();
    void set_isTextModified(bool isTextModified);
    __declspec(property(get = get_isTextModified, put = set_isTextModified)) bool isTextModified;
};

#endif // NOTEPAD_H
