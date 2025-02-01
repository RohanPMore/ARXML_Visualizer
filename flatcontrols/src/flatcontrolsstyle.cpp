#include "flatcontrolsstyle.h"

#include <QtGui/qcolor.h>
#include <QtGui/qpalette.h>

FlatcontrolsStyle::FlatcontrolsStyle(QObject *parent)
    : QObject(parent)
{
}

// Default QPalette colors

QColor FlatcontrolsStyle::base()
{
    return QColor(49, 49, 58);
}

QColor FlatcontrolsStyle::alternateBase()
{
    return blendColors(base(), text(), 5);
}

QColor FlatcontrolsStyle::light()
{
    return QColor(102, 102, 112);
}

QColor FlatcontrolsStyle::midlight()
{
    return blendColors(button(), light(), 50);
}

QColor FlatcontrolsStyle::dark()
{
    return button().darker(120);
}

QColor FlatcontrolsStyle::mid()
{
    return blendColors(button(), dark(), 50);
}

QColor FlatcontrolsStyle::highlight()
{
    return QColor(86, 86, 245);
}

QColor FlatcontrolsStyle::link()
{
    return QColor(66, 133, 244);
}

QColor FlatcontrolsStyle::shadow()
{
    return QColor(38, 38, 42);
}

QColor FlatcontrolsStyle::highlightedText()
{
    return windowText();
}

QColor FlatcontrolsStyle::window()
{
    return QColor(51, 51, 59);
}

QColor FlatcontrolsStyle::windowText()
{
    return text();
}

QColor FlatcontrolsStyle::brightText()
{
    return QColor(255, 128, 224);
}

QColor FlatcontrolsStyle::button()
{
    return QColor(68, 68, 75);
}

QColor FlatcontrolsStyle::buttonText()
{
    return text();
}

QColor FlatcontrolsStyle::placeholderText()
{
    return blendColors(window(), windowText(), 25);
}

QColor FlatcontrolsStyle::text()
{
    return QColor(229, 229, 237);
}

QColor FlatcontrolsStyle::toolTipText()
{
    return windowText();
}

QColor FlatcontrolsStyle::lightShade()
{
    return QColor(light().red(), light().green(), light().blue(), 90);
}

QColor FlatcontrolsStyle::darkShade()
{
    return QColor(shadow().red(), shadow().green(), shadow().blue(), 90);
}

QColor FlatcontrolsStyle::topShadow()
{
    return QColor(shadow().red(), shadow().green(), shadow().blue(), 18);
}

QColor FlatcontrolsStyle::innerContrastLine()
{
    return QColor(light().red(), light().green(), light().blue(), 30);
}

QColor FlatcontrolsStyle::outline()
{
    return placeholderText();
}

QColor FlatcontrolsStyle::highlightedOutline()
{
    return blendColors(highlight(), window(), 20);
}

QColor FlatcontrolsStyle::tabFrameColor()
{
    return blendColors(button(), window(), 10);
}
QColor FlatcontrolsStyle::blendColors(const QColor &colorA, const QColor &colorB, int factor)
{
    const int maxFactor = 100;
    const auto rgbColorB = colorB.toRgb();
    QColor tmp = colorA.toRgb();
    tmp.setRed((tmp.red() * (maxFactor - factor)) / maxFactor + (rgbColorB.red() * factor) / maxFactor);
    tmp.setGreen((tmp.green() * (maxFactor - factor)) / maxFactor + (rgbColorB.green() * factor) / maxFactor);
    tmp.setBlue((tmp.blue() * (maxFactor - factor)) / maxFactor + (rgbColorB.blue() * factor) / maxFactor);
    return tmp;
}

QColor FlatcontrolsStyle::grooveColor()
{
    QColor color = button().toHsv();
    color.setHsv(color.hue(),
                 qMin(255, color.saturation()),
                 qMin<int>(255, color.value() * 0.9));
    return color;
}
