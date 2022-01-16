#include "character.h"

#include <QDebug>

/* constructor for Character class */
Character::Character(const std::list<DrawnObject> &_objects):objects(_objects) {}

/* destructor for Character class */
Character::~Character() {}

/* checks for segment types and returns deciphered character */
QString Character::decipherLetter() {
    QString s = "";
    QString letter = "";
    letter.clear();

    for (auto o : objects){
        if (o.isDot()) s.append("D");
        for (unsigned int i = 0; i < o.vertices.size()-1; ++i) {
            auto p = o.normalDisplacements(i);
            if (o.isCurved(p.first, p.second)) s.append("C");
            else if (o.isStraight(p.first, p.second)) s.append("S");
        }
    }

    if (s == "C") letter = C();
    else if (s == "CC") CC();
    else if (s == "CSC" || s == "CCC" || s == "SCC") letter = CCC();
    else if (s == "CD") letter = CD();
    else if (s == "CDD") letter = CDD();
    else if (s == "CS") letter = CS();
    else if (s == "CSDD") letter = CSDD();
    else if (s == "CSS") letter = CSS();
    else if (s == "SC") letter = SC();
    else if (s == "SCC" || s == "SCCC" || s == "SCCCC") letter = SCC();
    else if (s == "SCS") letter = SCS();
    else if (s == "SD") letter = SD();
    else if (s == "SS") letter = SS();
    else if (s == "SSS") letter = SSS();
    else if (s == "SSSDD") letter = SSSDD();
    else if (s == "SSSS") letter = SSSS();
    s.clear();
    return letter;

}

//O, e, l, o
QString Character::C() {
    if (abs(objects.front().vertices[1].pos.x() - objects.front().vertices[0].pos.x()) <= 7.0) {
        if (objects.front().vertices[0].pos.y() >= 0.0) return "c";
        else return "C";
    }else {
        if (objects.front().vertices[0].pos.y() <= -5 &&
                abs(objects.front().vertices[1].pos.y() - objects.front().vertices[0].pos.y()) <= 5) return "U";
    }
    return "";
}

//e*, g*, w, y, m, w
QString Character::CC() { return "";}

QString Character::CCC() {
    if (objects.front().vertices[1].pos.x() < objects.front().vertices[0].pos.x()) {
        if (objects.front().vertices[0].pos.y() >= 0.0) return "s";
        else return "S";
    }
    return "";
}

QString Character::CD() {
    if (objects.back().vertices[0].pos.x() > objects.front().vertices[1].pos.x()) return "j";
    return "";
}

//Ö, ö
QString Character::CDD() { return ""; }

//Q, t, f
QString Character::CS() {
    if (objects.front().vertices[0].pos.y() >= -5) {
        if (objects.back().vertices[0].pos.y() <= -5) return "d";
        if (abs(objects.front().vertices[1].pos.x() - objects.front().vertices[0].pos.x()) <= 5) return "a";
        else return "u";
    }
    return ""; }

QString Character::CSDD() {
    if (abs(objects.front().vertices[1].pos.y() - objects.front().vertices[0].pos.y()) >= 10) return "ä";
    else return "ü";
}

QString Character::CSS() {
    if (objects.front().vertices[0].pos.y() < 0) return "G";
    else return "q";
}

QString Character::SC() {
    if (abs(objects.front().vertices[1].pos.y() - objects.front().vertices[0].pos.y()) <= 5) return "J";
    if (objects.front().vertices[0].pos.y() >= -5){
       if (objects.back().vertices[1].pos.x() - objects.back().vertices[0].pos.x() >= 10) {
           if (objects.back().vertices[1].pos.y() - objects.back().vertices[0].pos.y() >= 4) return "n";
           else if (objects.back().vertices[0].pos.y() - objects.back().vertices[1].pos.y() >= 4) return "r";
       }else return "p";
    }else {
        if (objects.back().vertices[0].pos.y() <= -5) return "P";
        else {
            if (abs(objects.back().vertices[1].pos.x() - objects.back().vertices[0].pos.x()) <= 5) return "b";
            else return "h";
        }
    }
    return "";
}

//ß
QString Character::SCC() { return "B"; }

QString Character::SCS() { return "R"; }

QString Character::SD() { return "i"; }

QString Character::SS() {
    if (objects.size() == 1) {
        if (objects.back().vertices[0].pos.x() < objects.back().vertices[2].pos.x()) {
            if (objects.back().vertices[2].pos.y() - objects.back().vertices[0].pos.y() >= 20) return "L";
            else if (abs(objects.back().vertices[2].pos.y() - objects.back().vertices[0].pos.y()) <= 5) {
                if (objects.back().vertices[0].pos.y() > 0) return "v";
                else return "V";
            }
        }
    }
    if (objects.size() == 2) {
        if (abs(objects.front().vertices[1].pos.y() - objects.front().vertices[0].pos.y()) <= 5) return "T";
        else if (abs(objects.front().vertices[1].pos.y() - objects.back().vertices[1].pos.y()) <= 5) {
            if (objects.front().vertices[0].pos.y() < 0) return "X";
            else return "x";
        }else return "Y";
    }
    return ""; }

QString Character::SSS() {
    if (objects.size() == 1) {
        if (objects.front().vertices[0].pos.y() > 0) return "z";
        else return "Z";
    }else if (objects.size() == 2) {
        if (abs(objects.front().vertices[1].pos.y() - objects.front().vertices[0].pos.y()) <= 5) { return "F";
        }else if (objects.back().vertices.size() == 3) {
            if (objects.back().vertices[0].pos.y() < -5) return "K";
            else return "k";
        }else if (objects.front().vertices[0].pos.x() > objects.front().vertices[1].pos.x() &&
                objects.front().vertices[1].pos.x() < objects.front().vertices[2].pos.x()) return "A";
    }else if (objects.size() == 3) {
        if (abs(objects.front().vertices[1].pos.y() - objects.front().vertices[0].pos.y()) <= 5) return "I";
        //else if (abs(objects.back().vertices[1].pos.y() - objects.back().vertices[0].pos.y()) <= 5) return "H";
        else if (abs(objects.back().vertices[1].pos.x() - objects.back().vertices[0].pos.x()) <= 5 &&
                    abs(objects.front().vertices[1].pos.x() - objects.front().vertices[0].pos.x()) <= 5) return "N";
    }
    return "";
}

QString Character::SSSDD() { return "Ä"; }

QString Character::SSSS() {
    if (abs(objects.back().vertices[1].pos.y() - objects.back().vertices[0].pos.y()) <= 5) return "E";
    else if (objects.back().vertices[0].pos.y() >= 0) return "M";
    else if (objects.back().vertices[0].pos.y() <= 0) return "W";
}

