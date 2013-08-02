#include <Malachite/ImageIO>

#include "jpegexportform.h"
#include "pngexportform.h"

#include "malachiteformatsupport.h"

namespace PaintField {

MalachiteFormatSupport::MalachiteFormatSupport(QObject *parent) :
	SingleLayerFormatSupport(parent)
{
}

bool MalachiteFormatSupport::readSingleLayer(QIODevice *device, Malachite::Surface *surface, QSize *size)
{
	Malachite::ImageImporter importer;
	if (!importer.load(device))
		return false;
	
	*surface = importer.toSurface();
	*size = importer.size();
	return true;
}

bool MalachiteFormatSupport::writeSingleLayer(QIODevice *device, const Malachite::Surface &surface, const QSize &size, const QVariant &option)
{
	auto settings = option.toHash();
	bool hasAlpha = settings.value("hasAlpha", true).toBool();
	int quality = settings.value("quality", 100).toInt();
	
	PAINTFIELD_DEBUG << "exporing with alpha = " << hasAlpha << "quality = " << quality;
	
	Malachite::ImageExporter exporter(this->malachiteFormat(), hasAlpha);
	exporter.setSurface(surface, size);
	exporter.setQuality(quality);
	
	return exporter.save(device);
}

JpegFormatSupport::JpegFormatSupport(QObject *parent) :
	MalachiteFormatSupport(parent)
{
	setShortDescription(tr("JPEG"));
}

QWidget *JpegFormatSupport::createExportingOptionWidget()
{
	return new JpegExportForm();
}

QVariant JpegFormatSupport::exportingOptionForWidget(QWidget *widget)
{
	auto form = qobject_cast<JpegExportForm *>(widget);
	if (!form)
		return QVariant();
	
	QVariantHash hash;
	hash["quality"] = form->quality();
	return hash;
}

PngFormatSupport::PngFormatSupport(QObject *parent) :
	MalachiteFormatSupport(parent)
{
	setShortDescription(tr("PNG"));
}

QWidget *PngFormatSupport::createExportingOptionWidget()
{
	return new PngExportForm();
}

QVariant PngFormatSupport::exportingOptionForWidget(QWidget *widget)
{
	auto form = qobject_cast<PngExportForm *>(widget);
	if (!form)
		return QVariant();
	
	QVariantHash hash;
	hash["hasAlpha"] = form->isAlphaEnabled();
	
	return hash;
}

} // namespace PaintField
