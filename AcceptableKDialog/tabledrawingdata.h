#ifndef TABLEDRAWINGDATA_H
#define TABLEDRAWINGDATA_H

#include <QMetaType>
#include <QPainter>

class tabledrawingdata
{
public:
    explicit tabledrawingdata(double time = 0, double n_y_max = 0);
    void paint(QPainter *painter, const QRect &rect,
               const QPalette &palette) const;
    QSize sizeHint() const;

private:
    QString timeText;
    QString n_y_maxText;
};

Q_DECLARE_METATYPE(tabledrawingdata);

#endif // TABLEDRAWINGDATA_H
