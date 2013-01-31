#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <QTransform>
#include <QModelIndex>

#include "global.h"

class QAction;
class QTreeView;
typedef QList<QAction *> QActionList;

namespace PaintField
{

/**
 * Creates an action.
 * @param id The object name
 * @param parent
 * @return 
 */
QAction *createAction(const QString &id, QObject *parent);

/**
 * Creates an action and connect the signal "triggered()".
 * @param id The object name
 * @param receiver The object which receives the slot
 * @param onTriggeredSlot The slot which receives the slot
 * @param parent
 * @return 
 */
QAction *createAction(const QString &id, QObject *receiver, const char *onTriggeredSlot, QObject *parent);

/**
 * Creates an action and connect the signal "triggered()".
 * The parent is set to receiver.
 * @param id
 * @param receiver
 * @param onTriggeredSlot
 * @return 
 */
inline QAction *createAction(const QString &id, QObject *receiver, const char *onTriggeredSlot) { return createAction(id, receiver, onTriggeredSlot, receiver); }

/**
 * Finds an QObject with objectName == id from the list.
 * @param list
 * @param id
 * @return found object or 0 if not found
 */
template <class T>
T *findQObject(const QList<T *> &list, const QString &id)
{
	QListIterator<T *> iter(list);
	
	while (iter.hasNext())
	{
		T *p = iter.next();
		
		if (p->objectName() == id)
			return p;
	}
	return 0;
}

template <class T>
T *findQObjectReverse(const QList<T *> &list, const QString &id)
{
	QListIterator<T *> iter(list);
	iter.toBack();
	
	while (iter.hasPrevious())
	{
		T *p = iter.previous();
		
		if (p->objectName() == id)
			return p;
	}
	return 0;
}

/**
 * Load a JSON from a file.
 * @param path
 * @return Parsed data (tree of QVariantList and QVariantMap)
 */
QVariant loadJsonFromFile(const QString &path);

bool saveJsonToFile(const QString &path, const QVariant &data);

void applyMacSmallSize(QWidget *widget);

QString unduplicatedName(const QStringList &existingNames, const QString &newName);

QString fileDialogFilterFromExtensions(const QStringList &extensions);

QTransform makeTransform(double scale, double rotation, const QPointF &translation);

inline void connectMutual(QObject *object1, const char *signal, QObject *object2, const char *slot)
{
	QObject::connect(object1, signal, object2, slot);
	QObject::connect(object2, signal, object1, slot);
}

void maximizeWindowSize(QWidget *widget);

void setExpandTreeViewRecursive(QTreeView *view, const QModelIndex &index, bool expanded);

}

#endif // UTIL_H
