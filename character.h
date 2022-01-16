#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include <list>


#include "drawnobject.h"

class Character
{
public:
    Character(const std::list<DrawnObject> &_objects);
    ~Character();

    QString decipherLetter();
    QString C();
    QString CC();
    QString CCC();
    QString CD();
    QString CDD();
    QString CS();
    QString CSDD();
    QString CSS();
    QString SC();
    QString SCC();
    QString SCS();
    QString SD();
    QString SS();
    QString SSS();
    QString SSSDD();
    QString SSSS();

    std::list<DrawnObject> objects;

signals:
    void dispLetter(QString s);

};

#endif // CHARACTER_H
