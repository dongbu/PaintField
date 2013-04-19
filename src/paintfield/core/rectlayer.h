#pragma once

#include "shapelayer.h"

namespace PaintField {

class RectLayer : public ShapeLayer
{
public:
	
	typedef ShapeLayer super;
	
	enum ShapeType
	{
		ShapeTypeRect,
		ShapeTypeEllipse
	};
	
	RectLayer() : super() {}
	
	Layer *createAnother() const override { return new RectLayer(); }
	
	bool setProperty(const QVariant &data, int role) override;
	QVariant property(int role) const override;
	
	void encode(QDataStream &stream) const override;
	void decode(QDataStream &stream) override;
	
	QVariantMap saveProperies() const override;
	void loadProperties(const QVariantMap &map) override;
	
	void setRect(const QRectF &rect);
	QRectF rect() const { return _rect; }
	
	void setShapeType(ShapeType type);
	ShapeType shapeType() const { return _shapeType; }
	
	void setShapeTypeString(const QString &str);
	QString shapeTypeString() const;
	
private:
	
	void setShapeFromRect();
	
	QRectF _rect;
	ShapeType _shapeType = ShapeTypeRect;
};

class RectLayerFactory : public LayerFactory
{
public:
	
	RectLayerFactory() : LayerFactory() {}
	
	QString name() const override;
	Layer *create() const override { return new RectLayer(); }
	const ::std::type_info &typeInfo() const override { return typeid(RectLayer); }
	
};

} // namespace PaintField
