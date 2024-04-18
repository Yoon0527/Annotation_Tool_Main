#pragma once
#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QVector>

class CustomLabel : public QLabel
{
    Q_OBJECT

public:
    CustomLabel(QWidget* parent = nullptr);

    void addRectangle(const QRect& rect);
    void clearRectangles();

signals:
    void rectangleDrawn(const QRect& rect);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<QRect> m_rectangles;
    QRect m_currentRect;
    bool m_drawing;
};

#endif // CUSTOMLABEL_H
