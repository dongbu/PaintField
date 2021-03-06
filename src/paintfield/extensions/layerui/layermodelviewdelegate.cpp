#include <QMouseEvent>
#include <QMenu>
#include <QLineEdit>
#include <QItemSelectionModel>

#include "paintfield/core/layeritemmodel.h"
#include "paintfield/core/layerscene.h"
#include "paintfield/core/thumbnail.h"
#include "paintfield/core/widgets/simplebutton.h"

#include "layeruicontroller.h"
#include "layermodelviewdelegate.h"

namespace PaintField
{

struct LayerModelViewDelegate::Data
{
	LayerUIController *actionController = 0;
	LayerScene *layerScene = 0;
	QIcon visibleIcon, lockedIcon;
	
	static constexpr int buttonMargin = 8;
	static constexpr int buttonWH = 16;
	static constexpr int pixmapMargin = 0;
	static constexpr int pixmapWH = Thumbnail::Margin * 2 + Thumbnail::PixmapSize;
	static constexpr int textVerticalMargin = 8, textHorizontalMargin = 8;
	static constexpr int height = pixmapWH + 2 * pixmapMargin;
	static constexpr int buttonBackgroundRadius = 4;
	
	static QRect buttonRect(const QRect &wholeRect, const int indexFromRight)
	{
		const int y = wholeRect.y() + (wholeRect.height() - buttonWH) / 2;
		const int x = wholeRect.x() + wholeRect.width() - (buttonMargin + buttonWH) * (indexFromRight + 1);
		return QRect(x, y, buttonWH, buttonWH);
	}
	
	static QRect textRect(const QRect &wholeRect)
	{
		constexpr int leftOffset = pixmapWH + pixmapMargin;
		constexpr int rightOffset = (buttonMargin + buttonWH) * 2 + buttonMargin;
		return wholeRect.adjusted(leftOffset + textHorizontalMargin, textVerticalMargin, -rightOffset - textHorizontalMargin, -textVerticalMargin);
	}
	
	static QRect pixmapRect(const QRect &wholeRect)
	{
		return QRect(wholeRect.x() + pixmapMargin, wholeRect.y() + pixmapMargin, pixmapWH, pixmapWH);
	}
	
	static void drawIcon(QPainter *painter, const QRect &wholeRect, const int countFromRight, const QIcon &icon, const bool on)
	{
		auto rect = buttonRect(wholeRect, countFromRight);
		icon.paint(painter, rect, Qt::AlignCenter, on ? QIcon::Normal : QIcon::Disabled, on ? QIcon::On : QIcon::Off);
	}
};

LayerModelViewDelegate::LayerModelViewDelegate(LayerUIController *actionController, QObject *parent) :
	QStyledItemDelegate(parent),
	d(new Data)
{
	d->actionController = actionController;
	d->layerScene = actionController->document()->layerScene();
	
	auto onColor = QColor(0x20, 0x20, 0x20);
	auto offColor = QColor(0xa0, 0xa0, 0xa0);
	
	d->visibleIcon = SimpleButton::createIcon(":/icons/16x16/visible.svg", onColor, offColor);
	d->lockedIcon = SimpleButton::createIcon(":/icons/16x16/locked.svg", onColor, offColor);
}

LayerModelViewDelegate::~LayerModelViewDelegate()
{
	delete d;
}

bool LayerModelViewDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	Q_UNUSED(option);
	
	switch (event->type())
	{
		case QEvent::MouseButtonPress:
		case QEvent::MouseButtonDblClick:
		{
			auto mouseEvent = static_cast<QMouseEvent *>(event);
			
			if (mouseEvent->button() == Qt::LeftButton)
			{
				auto toggle = [&](int buttonIndex, int role)->bool
				{
					if (d->buttonRect(option.rect, buttonIndex).contains(mouseEvent->pos()))
					{
						bool visible = index.data(role).toBool();
						d->layerScene->setLayerProperty(d->layerScene->itemModel()->layerForIndex(index), visible ? false : true, role);
						return true;
					}
					return false;
				};
				
				if (toggle(0, RoleVisible))
					return true;
				if (toggle(1, RoleLocked))
					return true;
			}
			break;
		}
		default:
			break;
	}
	
	return super::editorEvent(event, model, option, index);
}

void LayerModelViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	painter->save();
	
	const bool selected = option.state & QStyle::State_Selected;
	const bool current = ( index == d->layerScene->itemSelectionModel()->currentIndex() );
	
	if (selected)
		painter->fillRect( option.rect, option.palette.highlight() );
	
	const auto wholeRect = option.rect;
	
	// draw pixmap
	{
		const auto p = index.data(RoleThumbnail).value<QPixmap>();
		const auto r = d->pixmapRect(wholeRect);
		painter->drawPixmap(r.topLeft(), p);
	}
	
	// draw text
	{
		auto title = index.data(RoleName).toString();
		
		const auto r = d->textRect(wholeRect);
		
		QFont font;
		if (current)
		{
			font.setBold(true);
			//font.setWeight(QFont::Black);
			font.setUnderline(true);
			//title = "[" + title + "]";
		}
		
		if (selected)
			painter->setPen(option.palette.highlightedText().color());
		
		painter->setFont(font);
		painter->drawText(r, Qt::AlignTop | Qt::AlignLeft, title);
	}
	
	// draw buttons
	{
		const auto visible = index.data(RoleVisible).toBool();
		const auto locked = index.data(RoleLocked).toBool();
		
		d->drawIcon(painter, wholeRect, 0, d->visibleIcon, visible);
		d->drawIcon(painter, wholeRect, 1, d->lockedIcon, locked);
	}
	
	painter->restore();
}

QSize LayerModelViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)
	return QSize(1, d->height);
}

QWidget *LayerModelViewDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	auto rect = d->textRect(option.rect);
	auto lineEdit = new QLineEdit(parent);
	lineEdit->setGeometry(rect);
	lineEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	lineEdit->setText(index.data(RoleName).toString());
	return lineEdit;
}

void LayerModelViewDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
	Q_ASSERT(lineEdit);
	auto layer = d->layerScene->itemModel()->layerForIndex(index);
	d->layerScene->setLayerProperty(layer, lineEdit->text(), RoleName);
}

}
