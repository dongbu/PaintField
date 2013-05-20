#pragma once

#include "paintfield/core/tool.h"
#include "paintfield/core/rectlayer.h"

namespace PaintField {

class RectTool : public Tool
{
	Q_OBJECT
	
public:
	
	enum AddingType
	{
		NoAdding,
		AddRect,
		AddEllipse,
		AddText
	};
	
	explicit RectTool(AddingType type, Canvas *canvas);
	~RectTool();
	
	void drawLayer(Malachite::SurfacePainter *painter, const LayerConstPtr &layer) override;
	
protected:
	
	void keyPressEvent(QKeyEvent *event);
	void tabletMoveEvent(CanvasTabletEvent *event) override;
	void tabletPressEvent(CanvasTabletEvent *event) override;
	void tabletReleaseEvent(CanvasTabletEvent *event) override;
	
private slots:
	
	void updateSelected();
	void updateLayer(const LayerConstPtr &layer);
	void updateGraphicsItems();
	
private:
	
	enum Mode
	{
		NoOperation,
		Dragging,
		Inserting,
		MovingHandle
	};
	
	void updateHandles();
	void updateFrameRect();
	
	void onHandleMoved(const QPointF &pos, int handleFlags);
	void onHandleMoveFinished();
	void commit();
	
	void addHandle(int handleTypes, qreal zValue);
	
	void startAdding();
	void finishAdding();
	
	friend class RectInserter;
	friend class RectHandleItem;
	
	struct Data;
	Data *d;
};

} // namespace PaintField
