#ifndef PAINTFIELD_NAVIGATORCONTROLLER_H
#define PAINTFIELD_NAVIGATORCONTROLLER_H

#include <QObject>
#include "navigatorview.h"

namespace PaintField {

class Canvas;

class NavigatorController : public QObject
{
	Q_OBJECT
public:
	NavigatorController(Canvas *canvas, QObject *parent = 0);
	
	NavigatorView *view() { return _view.data(); }
	
signals:
	
public slots:
	
private:
	
	QScopedPointer<NavigatorView> _view;
};

} // namespace PaintField

#endif // PAINTFIELD_NAVIGATORCONTROLLER_H