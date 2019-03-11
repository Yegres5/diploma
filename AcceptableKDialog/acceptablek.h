#ifndef ACCEPTABLEK_H
#define ACCEPTABLEK_H

#include <QWidget>
#include <QMap>

class AcceptableK: public QWidget
{
    Q_OBJECT
public:
    AcceptableK(QMap<QString, QVariant>* iniParam, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~AcceptableK();

public slots:
    void hideDia();
    void showDia();
};

#endif // ACCEPTABLEK_H
