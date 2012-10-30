#ifndef WORKSPACECONTROLLER_H
#define WORKSPACECONTROLLER_H

#include <QObject>
#include <QPointer>

#include "canvascontroller.h"
#include "workspaceview.h"

namespace PaintField
{

class WorkspaceModule;
class ToolManager;
class PaletteManager;
class WorkspaceMdiAreaController;

class WorkspaceController : public QObject
{
	Q_OBJECT
public:
	explicit WorkspaceController(QObject *parent = 0);
	
	ToolManager *toolManager() { return _toolManager; }
	PaletteManager *paletteManager() { return _paletteManager; }
	
	WorkspaceView *createView(QWidget *parent = 0);
	WorkspaceView *view() { return _view; }
	
	void addModules(const WorkspaceModuleList &modules);
	WorkspaceModuleList modules() { return _modules; }
	
	void addActions(const QActionList &actions) { _actions += actions; }
	QActionList actions() { return _actions; }
	
	void addNullCanvasModules(const CanvasModuleList &modules);
	CanvasModuleList nullCanvasModules() { return _nullCanvasModules; }
	
	void addNullCanvasActions(const QActionList &actions) { _nullCanvasActions += actions; }
	QActionList nullCanvasActions() { return _nullCanvasActions; }
	
	void addCanvas(CanvasController *canvas);
	
signals:
	
	void currentCanvasChanged(CanvasController *canvas);
	
	void canvasAdded(CanvasController *canvas);
	
	void canvasRemoved(CanvasController *canvas);
	void focused();
	
public slots:
	
	void setFocus();
	
	void setCurrentCanvas(CanvasController *canvas);
	
	void newCanvas();
	void openCanvas();
	
	bool tryClose();
	
protected:
	
	bool eventFilter(QObject *watched, QEvent *event);
	
private slots:
	
	void onCanvasSholudBeDeleted();
	void removeCanvas(CanvasController *canvas);
	
private:
	
	QActionList currentCanvasActions() { return _currentCanvas ? _currentCanvas->actions() : _nullCanvasActions; }
	CanvasModuleList currentCanvasModules() { return _currentCanvas ? _currentCanvas->modules() : _nullCanvasModules; }
	
	void createWorkspaceItems();
	void createSidebarsInArea(const QVariantList &ids, Qt::DockWidgetArea area);
	void createToolbarsInArea(const QVariantList &ids, Qt::ToolBarArea area);
	void updateWorkspaceItems();
	void updateWorkspaceItemsForCanvas(CanvasController *canvas);
	void createMenuBar();
	void updateMenuBar();
	
	QList<CanvasController *> _canvasControllers;
	QPointer<CanvasController> _currentCanvas;
	
	ToolManager *_toolManager = 0;
	PaletteManager *_paletteManager = 0;
	
	QActionList _actions;
	WorkspaceModuleList _modules;
	
	QActionList _nullCanvasActions;
	CanvasModuleList _nullCanvasModules;
	
	WorkspaceMdiAreaController *_mdiAreaController = 0;
	
	QPointer<WorkspaceView> _view;
};

}

#endif // WORKSPACECONTROLLER_H
