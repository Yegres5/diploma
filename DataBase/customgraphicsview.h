#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>

class customGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    customGraphicsView(QWidget* parent = nullptr);
    QByteArray imageFullSize;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void showPathChoose();

};

#endif // CUSTOMGRAPHICSVIEW_H
