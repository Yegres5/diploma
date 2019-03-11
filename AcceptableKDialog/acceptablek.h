#ifndef ACCEPTABLEK_H
#define ACCEPTABLEK_H

#include <QDialog>
#include <QMap>

class AcceptableK:public QDialog
{
    Q_OBJECT
public:
    AcceptableK(QMap<QString, QVariant>* iniParam, QWidget* parent = nullptr);
    ~AcceptableK();

public slots:
    void hideDia();
    void showDia();
};

#endif // ACCEPTABLEK_H
