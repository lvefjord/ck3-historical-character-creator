#ifndef CHARACTERTOKEN_H
#define CHARACTERTOKEN_H

#include <QGraphicsObject>
#include "characterdata.h"

class CharacterToken : public QGraphicsObject
{
    Q_OBJECT
public:
    CharacterToken(CharacterData *character, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    CharacterData *getCharacterData() const;

signals:
    void relationshipsChanged();
    void spousesSet(QString character1Id, QString character2Id);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    CharacterData *character;
};

#endif // CHARACTERTOKEN_H