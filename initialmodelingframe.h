#ifndef INITIALMODELINGFRAME_H
#define INITIALMODELINGFRAME_H

#include <QObject>
#include <QFrame>
#include <QPushButton>

class laInitialFrame;
class rocketInitialFrame;
class modelingFrameParameters;
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
public slots:
    void StartModeling();
};

#endif // INITIALMODELINGFRAME_H
