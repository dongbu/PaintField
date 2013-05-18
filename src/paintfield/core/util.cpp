#include <QAction>
#include <QFile>
#include <QWidget>
#include <QTreeView>
#include <QApplication>
#include <QDesktopWidget>
#include <QMenu>

#include <supportlib/qjson/parser.h>
#include <supportlib/qjson/serializer.h>

#include "util.h"

namespace PaintField
{

namespace Util
{

QString platformName()
{
#if defined(Q_OS_WIN32)
	return "windows";
#elif defined(Q_OS_MAC)
	return "mac";
#elif defined(Q_OS_LINUX)
	return "linux";
#elif defined(Q_OS_FREEBSD)
	return "freebsd";
#else
	return "other";
#endif
}

QAction *createAction(const QString &id, QObject *parent)
{
	auto action = new QAction(parent);
	action->setObjectName(id);
	return action;
}

QAction *createAction(const QString &id, QObject *receiver, const char *onTriggeredSlot, QObject *parent)
{
	auto action = createAction(id, parent);
	QObject::connect(action, SIGNAL(triggered()), receiver, onTriggeredSlot);
	return action;
}

QVariant loadJsonFromFile(const QString &path)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		PAINTFIELD_DEBUG << "failed to open file";
		return QVariant();
	}
	
	QJson::Parser parser;
	return parser.parse(&file);
}

bool saveJsonToFile(const QString &path, const QVariant &data)
{
	QJson::Serializer serializer;
	serializer.setIndentMode(QJson::IndentFull);
	bool ok;
	
	QFile file(path);
	
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		PAINTFIELD_DEBUG << "failed to save file";
		return false;
	}
	serializer.serialize(data, &file, &ok);
	
	return ok;
}

void applyMacSmallSize(QWidget *widget)
{
	// does not apply to QMenu objects
	if (qobject_cast<QMenu *>(widget))
		return;
	
	widget->setAttribute(Qt::WA_MacSmallSize);
	
	for (QObject *object : widget->children())
	{
		QWidget *widget = qobject_cast<QWidget *>(object);
		if (widget)
			applyMacSmallSize(widget);
	}
}

QString unduplicatedName(const QStringList &existingNames, const QString &name)
{
	if (!existingNames.contains(name))
		return name;
	
	int i = 0;
	forever
	{
		QString newName = name + " #" + QString::number(++i);
		if (!existingNames.contains(newName))
			return newName;
	}
}

QString fileDialogFilterFromExtensions(const QStringList &extensions)
{
	QString ret;
	
	ret += "(";
	
	for (auto ext : extensions)
		ret += "* " + ext;
	
	ret += ")";
	
	return ret;
}

QTransform makeTransform(double scale, double rotation, const QPointF &translation)
{
	QTransform transform;
	
	if (translation != QPointF(0, 0))
		transform.translate(translation.x(), translation.y());
	
	if (rotation)
		transform.rotate(rotation);
	
	if (scale != 1.0)
		transform.scale(scale, scale);
	
	return transform;
}

void maximizeWindowSize(QWidget *widget)
{
	auto availableGeom = qApp->desktop()->availableGeometry();
	
	QPoint offset = widget->geometry().topLeft() - widget->pos();
	
	QSize size(availableGeom.width() - offset.x(), availableGeom.height() - offset.y());
	widget->move(availableGeom.topLeft());
	widget->resize(size);
}

void setExpandTreeViewRecursive(QTreeView *view, const QModelIndex &index, bool expanded)
{
	view->setExpanded(index, expanded);
	
	int rowCount = view->model()->rowCount(index);
	
	for (int i = 0; i < rowCount; ++i)
		setExpandTreeViewRecursive(view, view->model()->index(i, 0, index), expanded);
}

QPoint mapToWindow(QWidget *widget, const QPoint &pos)
{
	//return widget->mapToGlobal(pos) - widget->window()->geometry().topLeft();
	
	QPoint result = pos;
	
	auto widgetIsTopLevel = [&widget]()->bool
	{
		if (!widget->parent())
			return true;
		
		if ((widget->windowFlags() & Qt::Tool) == Qt::Tool)
			return true;
		
		return false;
	};
	
while (!widgetIsTopLevel())
	{
		result = widget->mapToParent(result);
		widget = widget->parentWidget();
	}
	
	return result;
}

}

}
