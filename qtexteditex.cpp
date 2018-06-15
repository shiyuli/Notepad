#include "qtexteditex.h"

QTextEditEx::QTextEditEx(QWidget *parent) :
    QPlainTextEdit(parent)
{
    isControlKeyPressed = false;
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

QTextEditEx::~QTextEditEx()
{
}

void QTextEditEx::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Control)
    {
        isControlKeyPressed = true;
    }

    QPlainTextEdit::keyPressEvent(e);
}

void QTextEditEx::keyReleaseEvent(QKeyEvent *e)
{
    isControlKeyPressed = false;

    QPlainTextEdit::keyReleaseEvent(e);
}

void QTextEditEx::wheelEvent(QWheelEvent *e)
{
    if(isControlKeyPressed)
    {
        if(e->delta() > 0)
        {
            this->zoomIn();
        }
        else
        {
            this->zoomOut();
        }
    }

    QPlainTextEdit::wheelEvent(e);
}

int QTextEditEx::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());

    while(max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void QTextEditEx::updateLineNumberArea(const QRect &rect, int dy)
{
    if(dy)
    {
        lineNumberArea->scroll(0, dy);
    }
    else
    {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }

    if(rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth(0);
    }
}

void QTextEditEx::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void QTextEditEx::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void QTextEditEx::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if(!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();

        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void QTextEditEx::lineNumberAreaPaintEvent(QPaintEvent *e)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(e->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingRect(block).height();

    while(block.isValid() && top <= e->rect().bottom())
    {
        if(block.isVisible() && bottom >= e->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }
}

LineNumberArea::LineNumberArea(QTextEditEx *parent) :
    QWidget(parent)
{
    editor = parent;
}

QSize LineNumberArea::sizeHint() const
{
    return QSize(editor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *e)
{
    editor->lineNumberAreaPaintEvent(e);
}
