#include "CustomLabel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

CustomLabel::CustomLabel(QWidget* parent)
    : QLabel(parent), m_drawing(false)
{
}

void CustomLabel::addRectangle(const QRect& rect)
{
    m_rectangles.append(rect);
    update();
}

void CustomLabel::clearRectangles()
{
    m_rectangles.clear();
    update();
}

void CustomLabel::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_currentRect.setTopLeft(event->pos());
        m_currentRect.setBottomRight(event->pos());
        m_drawing = true;
    }
}

void CustomLabel::mouseMoveEvent(QMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton) && m_drawing) {
        m_currentRect.setBottomRight(event->pos());
        update();
    }
}

void CustomLabel::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_drawing) {
        m_drawing = false;
        m_rectangles.append(m_currentRect);
        emit rectangleDrawn(m_currentRect);
        update();
    }
}

void CustomLabel::paintEvent(QPaintEvent* event)
{
    QLabel::paintEvent(event);

    QPainter painter(this);
    painter.setPen(Qt::red);
    painter.setBrush(Qt::NoBrush);
    for (const QRect& rect : m_rectangles) {
        painter.drawRect(rect);
    }
    if (m_drawing) {
        painter.drawRect(m_currentRect);
    }
}
