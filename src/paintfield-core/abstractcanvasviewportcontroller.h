#pragma once
#include <QObject>
#include <Malachite/Affine2D>

#include "global.h"

class QPoint;
class QSize;
class QTransform;

namespace Malachite
{
class Image;
}

namespace PaintField
{

class AbstractCanvasViewportController : public QObject
{
	Q_OBJECT
	
public:
	
	explicit AbstractCanvasViewportController(QObject *parent = 0);
	~AbstractCanvasViewportController();
	
	virtual QWidget *view() = 0;
	
	virtual void setScrollBarValue(Qt::Orientation orientation, int value) = 0;
	virtual void setScrollBarRange(Qt::Orientation orientation, int max, int min) = 0;
	virtual void setScrollBarPageStep(Qt::Orientation orientation, int value) = 0;
	
	virtual void setDocumentSize(const QSize &size) = 0;
	virtual void setTransform(const Malachite::Affine2D &transform, bool hasTranslation, bool hasScaling, bool hasRotation) = 0;
	virtual void updateTile(const QPoint &tileKey, const Malachite::Image &image, const QPoint &offset) = 0;
	virtual void beforeUpdateTile() {}
	virtual void afterUpdateTile() {}
	virtual void update() = 0;
	virtual bool isReady() = 0;
	
signals:
	
	void ready();
	
private:
	
	class Data;
	Data *d;
};

}
