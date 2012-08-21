#include <QtGui>
#include "fssimplebutton.h"

QIcon fsCreateSimpleIconSet(const QString &basePixmapFile)
{
	return fsCreateSimpleIconSet(QPixmap(basePixmapFile));
}

void fsPaintPixmapWithColor(QPixmap *pixmap, const QColor &color)
{
	QPainter painter(pixmap);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.setPen(Qt::NoPen);
	painter.setBrush(color);
	painter.drawRect(pixmap->rect());
}

QIcon fsCreateSimpleIconSet(const QPixmap &basePixmap)
{
	if (basePixmap.isNull())
	{
		qWarning() << "fsCreateSimpleIconSet: basePixmap is null";
		return QIcon();
	}
	
	QPixmap pixmapOff = basePixmap;
	//QPixmap pixmapActiveOff = basePixmap;
	QPixmap pixmapOn = basePixmap;
	//QPixmap pixmapActiveOn = basePixmap;
	
	fsPaintPixmapWithColor(&pixmapOff, QColor(102, 102, 102));
	//fsPaintPixmapWithColor(&pixmapActiveOff, QColor(77, 77, 77));
	fsPaintPixmapWithColor(&pixmapOn, QColor(39, 96, 209));
	//fsPaintPixmapWithColor(&pixmapOn, QColor(255, 60, 60));
	//fsPaintPixmapWithColor(&pixmapActiveOn, QColor(35, 84, 184));
	
	QIcon icon;
	icon.addPixmap(pixmapOff, QIcon::Normal, QIcon::Off);
	//icon.addPixmap(pixmapActiveOff, QIcon::Active, QIcon::Off);
	icon.addPixmap(pixmapOn, QIcon::Normal, QIcon::On);
	//icon.addPixmap(pixmapActiveOn, QIcon::Active, QIcon::On);
	
	return icon;
}

FSSimpleButton::FSSimpleButton(QWidget *parent) :
	QToolButton(parent)
{
	commonInit();
}

FSSimpleButton::FSSimpleButton(const QIcon &icon, QWidget *parent) :
	QToolButton(parent)
{
	setIcon(icon);
	commonInit();
}

FSSimpleButton::FSSimpleButton(const QString &basePixmapFile, QWidget *parent) :
	QToolButton(parent)
{
	QIcon icon = fsCreateSimpleIconSet(basePixmapFile);
	
	if (!icon.isNull())
	{
		setIcon(fsCreateSimpleIconSet(basePixmapFile));
		setIconSize(icon.availableSizes().at(0));
	}
	
	commonInit();
}

void FSSimpleButton::commonInit()
{
	_active = false;
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setPopupMode(QToolButton::InstantPopup);
}

QSize FSSimpleButton::sizeHint() const
{
	return QSize(_margins.left() + iconSize().width() + _margins.right(), _margins.top() + iconSize().height() + _margins.bottom());
}

void FSSimpleButton::enterEvent(QEvent *)
{
	_active = true;
	update();
}

void FSSimpleButton::leaveEvent(QEvent *)
{
	_active = false;
	update();
}

void FSSimpleButton::paintEvent(QPaintEvent *)
{
	QIcon::Mode mode;
	
	if (isEnabled())
	{
		mode = _active ? QIcon::Active : QIcon::Normal;
	}
	else
	{
		mode = QIcon::Disabled;
	}
	
	QPixmap pixmap = icon().pixmap(iconSize(), mode, isChecked() || isDown() ? QIcon::On : QIcon::Off);
	QPainter painter(this);
	painter.drawPixmap(_margins.left(), _margins.top(), pixmap);
}
