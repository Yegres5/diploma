#ifndef INITIALMODELINGFRAME_H
#define INITIALMODELINGFRAME_H

#include <QObject>
#include <QFrame>
#include <QPushButton>
#include <QMap>

class laInitialFrame;
class rocketInitialFrame;
class modelingFrameParameters;

namespace Ui {
    class InitialModelingFrame;
}
class InitialModelingFrame: public QFrame
{
    Q_OBJECT
private:
    laInitialFrame* m_laFrame;
    rocketInitialFrame* m_rocketFrame;
    modelingFrameParameters* m_modelFrame;
    QPushButton* m_StartModelingButton;

public:
    explicit InitialModelingFrame(QWidget* parent = nullptr);
    virtual ~InitialModelingFrame() {}
    QMap<QString, QVariant> getInitialParametrs();
    void setInitialParametrs(QMap<QString, QVariant> parametrs);

public slots:
    void StartModeling();

signals:
    void startModelingPressed(QMap<QString, QVariant>* paramentr);

};

#endif // INITIALMODELINGFRAME_H
