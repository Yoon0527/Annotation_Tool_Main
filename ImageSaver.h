#pragma once
#include <QObject>
#include <QPixmap>

class ImageSaver : public QObject
{
    Q_OBJECT

public:
    explicit ImageSaver(QObject* parent = nullptr);
    virtual ~ImageSaver();

public slots:
    void savePixmap(std::string txt_path, std::string filename, const QPixmap& pixmap);
};
