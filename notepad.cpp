#include "notepad.h"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    setWindowTitle(CONSTANT_UNTITLED);

    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    
    //connect(ui->actionAbout, SIGNAL(clicked()), new DialogAbout(this), SLOT(exec()));
}

Notepad::~Notepad()
{
    delete ui;
}

void Notepad::on_actionNew_triggered()
{
    currentFileName.clear();
    ui->textEdit->setPlainText(QString());
    setWindowTitle(CONSTANT_UNTITLED);
}

void Notepad::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    if(fileName == NULL)
    {
        return;
    }

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "warning", "Cannot open file: " + file.errorString());
        return;
    }

    setWindowTitle(fileName);

    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setPlainText(text);
    currentFileName = fileName;

    file.close();

    isTextModified = false;
}

void Notepad::on_actionSave_triggered()
{
    QString fileName;

    //If we don't have a fileName from before, get one.
    if(currentFileName.isEmpty())
    {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        currentFileName = fileName;
    }
    else
    {
        fileName = currentFileName;
    }

    if(!save(fileName))
    {
        return;
    }

    setWindowTitle(fileName);
}

void Notepad::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");

    if(!save(fileName))
    {
        return;
    }

    currentFileName = fileName; //todo: Will "save as" change the "curentFileName"?
    setWindowTitle(fileName);
}

void Notepad::on_actionPrint_triggered()
{
    QMessageBox::warning(this, "Warning", "Function print haven't supported yet.");

//#if QT_CONFIG(printer)
//    QPrinter printDev;
//#if QT_CONFIG(printdialog)
//    QPrintDialog dialog(&printDev, this);
//    if(dialog.exec() == QDialog::Rejected)
//    {
//        return;
//    }
//#endif // QT_CONFIG(printdialog)
//    ui->textEdit->print(&printDev);
//#endif // QT_CONFIG(printer)
}

void Notepad::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

void Notepad::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void Notepad::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void Notepad::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void Notepad::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void Notepad::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void Notepad::on_actionFont_triggered()
{
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, ui->textEdit->font(), this, "Font", QFontDialog::FontDialogOption::DontUseNativeDialog);
    
    if(fontSelected)
    {
        ui->textEdit->setFont(font);
    }
}

void Notepad::on_actionAbout_triggered()
{
    DialogAbout *dialogAbout = new DialogAbout(this);
    dialogAbout->show();
}

void Notepad::mousePressEvent(QMouseEvent *event)
{
}

void Notepad::on_textEdit_selectionChanged()
{
    if(ui->textEdit->textCursor().selectedText().length() > 0)
    {
        ui->actionCopy->setEnabled(true);
        ui->actionCut->setEnabled(true);
    }
    else
    {
        ui->actionCopy->setEnabled(false);
        ui->actionCut->setEnabled(false);
    }
}

void Notepad::on_textEdit_textChanged()
{
    isTextModified = true;

    if(!currentFileName.isEmpty())
    {
//        save(currentFileName);
    }
}

void Notepad::on_textEdit_undoAvailable(bool b)
{
    if(b)
    {
        ui->actionUndo->setEnabled(true);
    }
    else
    {
        ui->actionUndo->setEnabled(false);
    }
}

void Notepad::on_textEdit_redoAvailable(bool b)
{
    if(b)
    {
        ui->actionRedo->setEnabled(true);
    }
    else
    {
        ui->actionRedo->setEnabled(false);
    }
}

void Notepad::on_textEdit_copyAvailable(bool b)
{
}

bool Notepad::get_isTextModified()
{
    return _isTextModified;
}

void Notepad::set_isTextModified(bool isTextModified)
{
    _isTextModified = isTextModified;
    ui->actionSave->setEnabled(isTextModified);
}

bool Notepad::save(const QString fileName)
{
    if(fileName == NULL | fileName.isEmpty())
    {
        return false;
    }

    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return false;
    }

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;

    file.close();

    isTextModified = false;

    return true;
}
