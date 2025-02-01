#pragma once

#include <QObject>
#include <QtGui/qcolor.h>
#include <QtQml/qqml.h>


Q_DECLARE_OPAQUE_POINTER(QPalette*)

class FlatcontrolsStyle : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor base READ base CONSTANT)
    Q_PROPERTY(QColor alternateBase READ alternateBase CONSTANT)

    Q_PROPERTY(QColor light READ light CONSTANT)
    Q_PROPERTY(QColor midlight READ midlight CONSTANT)
    Q_PROPERTY(QColor dark READ dark CONSTANT)
    Q_PROPERTY(QColor mid READ mid CONSTANT)
    Q_PROPERTY(QColor shadow READ shadow CONSTANT)

    Q_PROPERTY(QColor link READ link CONSTANT)
    Q_PROPERTY(QColor highlight READ highlight CONSTANT)
    Q_PROPERTY(QColor highlightedText READ highlightedText CONSTANT)
    Q_PROPERTY(QColor window READ window CONSTANT)
    Q_PROPERTY(QColor windowText READ window CONSTANT)
    Q_PROPERTY(QColor brightText READ brightText CONSTANT)
    Q_PROPERTY(QColor button READ button CONSTANT)
    Q_PROPERTY(QColor buttonText READ buttonText CONSTANT)
    Q_PROPERTY(QColor placeholderText READ placeholderText CONSTANT)
    Q_PROPERTY(QColor text READ text CONSTANT)
    Q_PROPERTY(QColor toolTipText READ toolTipText CONSTANT)

    Q_PROPERTY(QColor lightShade READ lightShade CONSTANT)
    Q_PROPERTY(QColor darkShade READ darkShade CONSTANT)
    Q_PROPERTY(QColor topShadow READ topShadow CONSTANT)
    Q_PROPERTY(QColor innerContrastLine READ innerContrastLine CONSTANT)
    Q_PROPERTY(QColor outline READ outline CONSTANT)
    Q_PROPERTY(QColor highlightedOutline READ highlightedOutline CONSTANT)
    Q_PROPERTY(QColor tabFrameColor READ tabFrameColor CONSTANT)
    Q_PROPERTY(QColor grooveColor READ grooveColor CONSTANT)

    QML_NAMED_ELEMENT(FlatcontrolsStyle)
    QML_SINGLETON

public:
    explicit FlatcontrolsStyle(QObject *parent = nullptr);

    // Default QPalette colors
    Q_INVOKABLE static QColor base();
    Q_INVOKABLE static QColor alternateBase();

    Q_INVOKABLE static QColor light();
    Q_INVOKABLE static QColor midlight();
    Q_INVOKABLE static QColor dark();
    Q_INVOKABLE static QColor mid();
    Q_INVOKABLE static QColor shadow();
    Q_INVOKABLE static QColor link();
    Q_INVOKABLE static QColor highlight();
    Q_INVOKABLE static QColor highlightedText();
    Q_INVOKABLE static QColor window();
    Q_INVOKABLE static QColor windowText();
    Q_INVOKABLE static QColor brightText();
    Q_INVOKABLE static QColor button();
    Q_INVOKABLE static QColor buttonText();
    Q_INVOKABLE static QColor placeholderText();
    Q_INVOKABLE static QColor text();
    Q_INVOKABLE static QColor toolTipText();

    Q_INVOKABLE static QColor lightShade();
    Q_INVOKABLE static QColor darkShade();
    Q_INVOKABLE static QColor topShadow();
    Q_INVOKABLE static QColor innerContrastLine();

    Q_INVOKABLE static QColor outline();
    Q_INVOKABLE static QColor highlightedOutline();
    Q_INVOKABLE static QColor tabFrameColor();
    Q_INVOKABLE static QColor blendColors(const QColor &colorA, const QColor &colorB, int factor = 50);
    Q_INVOKABLE static QColor grooveColor();
};
