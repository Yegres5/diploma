#include "customgraphicsview.h"

#include <QMouseEvent>
#include <QMenu>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QBuffer>

customGraphicsView::customGraphicsView(QWidget *parent):
    QGraphicsView (parent)
{
}

void customGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton){
        emit customContextMenuRequested(event->pos());
    }
}

void customGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu=new QMenu(this);
    QAction* action = new QAction(tr("Change image"), this);

    connect(action, SIGNAL(triggered()),
            this, SLOT(showPathChoose()));
    menu->addAction(action);
    menu->popup(event->globalPos());
}

void customGraphicsView::showPathChoose()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAttribute(Qt::WA_DeleteOnClose);

    QString fileName = dialog.getOpenFileName(this, tr("Open Image"), QDir::currentPath(), tr("Image Files (*.png *.jpg *.bmp)"));
    scene()->removeItem(scene()->items().first());

    QImage im(fileName);

    QGraphicsScene* scene = new QGraphicsScene;
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem((QPixmap::fromImage(im)));
    setScene( scene );
    scene->addItem(item);
    item->setPos(0,0);

    QByteArray arr;
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    im.save(&buffer, "PNG");
    imageFullSize = arr;

    this->scene()->addItem(new QGraphicsPixmapItem(QPixmap::fromImage(im)));

    fitInView(this->scene()->sceneRect(), Qt::IgnoreAspectRatio);
}
