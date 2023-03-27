#ifndef MYSTYLE_H
#define MYSTYLE_H

#include <QProxyStyle>
class myStyle: public QProxyStyle
{
public:
    int pixelMetric(PixelMetric metric, const QStyleOption * option = 0, const QWidget * widget = 0 ) const {
        int s = QProxyStyle::pixelMetric(metric, option, widget);
        if (metric == QStyle::PM_SmallIconSize) {
            s = 32;
        }
        return s;
    }
};

#endif // MYSTYLE_H
