#pragma once

#include <QWidget>
#include "canvasviewportinterface.h"

namespace PaintField {

class CanvasViewportSoftware : public QWidget, public CanvasViewportInterface
{
	Q_OBJECT
	Q_INTERFACES(PaintField::CanvasViewportInterface)
	
public:
	
	explicit CanvasViewportSoftware(QWidget *parent = 0);
	~CanvasViewportSoftware();
	
	void setScrollBarValue(Qt::Orientation orientation, int value);
	void setScrollBarRange(Qt::Orientation orientation, int max, int min);
	void setScrollBarPageStep(Qt::Orientation orientation, int value);
	
	void setDocumentSize(const QSize &size);
	void setTransform(const Malachite::Affine2D &transform, bool hasTranslation, bool hasScaling, bool hasRotation) override;
	void updateTile(const QPoint &tileKey, const Malachite::Image &image, const QPoint &offset);
	void beforeUpdateTile() {}
	void afterUpdateTile() {}
	void update() {}
	bool isReady() { return true; }
	
signals:
	
	void scrollBarXChanged(int value);
	void scrollBarYChanged(int value);
	
protected:
	
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	
private:
	
	class Data;
	Data *d;
};


}