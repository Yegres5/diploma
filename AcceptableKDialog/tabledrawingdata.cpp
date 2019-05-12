#include "tabledrawingdata.h"

#include <QObject>

tabledrawingdata::tabledrawingdata(double time, double n_y_max, QString errorMessage):
    timeText(QObject::tr("T = ") + QString::number(time)),
    n_y_maxText(QObject::tr("n_y_max = ") + QString::number(n_y_max)),
    errorMessage(errorMessage)
{
}

void tabledrawingdata::paint(QPainter *painter, const QRect &rect, const QPalette &palette) const
{
    painter->save();
    painter->setPen(Qt::black);
    QString text;
    if (errorMessage == ""){
        text = timeText + "\n" + n_y_maxText;
    }else{
        text = errorMessage;
    }
    painter->drawText(rect, Qt::AlignCenter ,text);
    painter->restore();
}

QSize tabledrawingdata::sizeHint() const
{
    return ((timeText.size() > n_y_maxText.size()) ? timeText.size() : n_y_maxText.size()) * QSize(7, 0) + QSize(0, 30) + QSize(6,6);
}
