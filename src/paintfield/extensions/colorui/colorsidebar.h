#pragma once

#include <QWidget>
#include <Malachite/Color>

class QVBoxLayout;
class QHBoxLayout;
class QFormLayout;
class QButtonGroup;
class QLineEdit;
class QComboBox;
class QGridLayout;
class QLabel;

namespace PaintField
{

class SimpleButton;
class ColorButton;
class ColorWheel;
class ColorSlider;
class LooseSpinBox;
class WidgetGroup;

class ColorSliderPanel : public QWidget
{
	Q_OBJECT
public:
	explicit ColorSliderPanel(QWidget *parent = 0);
	
	Malachite::Color color() const { return _color; }
	
public slots:
	void setColor(const Malachite::Color &color);
	
signals:
	void colorChanged(const Malachite::Color &color);
	void rgbSelectedChanged(bool selected);
	void rgb8SelectedChanged(bool selected);
	void hsvSelectedChanged(bool selected);
	
private slots:
	void onComboBoxActivated(int index);
	
private:
	
	Malachite::Color _color;
};

class WebColorPanel : public QWidget
{
	Q_OBJECT
public:
	explicit WebColorPanel(QWidget *parent = 0);
	
	Malachite::Color color() const { return _color; }
	
public slots:
	void setColor(const Malachite::Color &color);
	
signals:
	void colorChanged(const Malachite::Color &color);
	
private slots:
	void onLineEditEditingFinished();
	
private:
	QLineEdit *_lineEdit;
	
	Malachite::Color _color;
};

class ColorSideBar : public QWidget
{
	Q_OBJECT
	
public:
	
	explicit ColorSideBar(QWidget *parent = 0);
	
	QList<ColorButton *> colorButtons() { return _colorButtons; }
	
signals:
	
	void currentColorChanged(const Malachite::Color &color);
	void colorButtonColorChanged(int index, const Malachite::Color &color);
	void colorButtonClicked(int index);
	void currentOpacityChanged(double opacity);
	
public slots:
	
	void setColorButtonColor(int index, const Malachite::Color &color);
	void setCurrentColor(const Malachite::Color &color);
	
private slots:
	
	void onColorButtonToggled(bool on);
	void onColorButtonChanged(const Malachite::Color &color);
	void setCurrentOpacity(double opacity);
	
private:
	
	QList<ColorButton *> _colorButtons;
	Malachite::Color _color;
};

}

