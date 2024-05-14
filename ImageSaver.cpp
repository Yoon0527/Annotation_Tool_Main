#include "imagesaver.h"
#include <QImage>
#include <QDebug>

ImageSaver::ImageSaver(QObject* parent) : QObject(parent)
{
    // ������ �ڵ�
}

ImageSaver::~ImageSaver()
{
    // �Ҹ��� �ڵ�
}

void ImageSaver::savePixmap(std::string txt_path, std::string filename, const QPixmap& pixmap)
{
    QString pixmap_save_path = QString::fromStdString(txt_path + "Images\\" + filename + "\\" + filename + "_result" + ".png");
    //QFile file(pixmap_save_path);

    QImage save_img = pixmap.toImage();
    save_img.save(pixmap_save_path, "PNG");
}