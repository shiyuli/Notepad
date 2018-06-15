#ifndef QTEXTEDITEX_H
#define QTEXTEDITEX_H

#include <QTextEdit>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QWidget>
#include <QObject>
#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>

namespace Ui {
class QTextEditEx;
}

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class LineNumberArea;

class QTextEditEx : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit QTextEditEx(QWidget *parent = 0);
    ~QTextEditEx();

    void lineNumberAreaPaintEvent(QPaintEvent *e);
    int lineNumberAreaWidth();

protected:
    virtual void keyPressEvent(QKeyEvent *e) override;
    virtual void keyReleaseEvent(QKeyEvent *e) override;
    virtual void wheelEvent(QWheelEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

private slots:
    void updateLineNumberArea(const QRect &, int);
    void updateLineNumberAreaWidth(int);
    void highlightCurrentLine();

private:
    QWidget *lineNumberArea;
    bool isControlKeyPressed;
};

class LineNumberArea : public QWidget
{
public:
    explicit LineNumberArea(QTextEditEx *parent);
    ~LineNumberArea() = default;

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    QTextEditEx *editor;
};

#endif // QTEXTEDITEX_H
