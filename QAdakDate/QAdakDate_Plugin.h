#ifndef QAdakDate_Plugin_H
#define QAdakDate_Plugin_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class QAdakDate_Plugin : public QObject, public QDesignerCustomWidgetInterface
{
        Q_OBJECT
        /* 5.8 MACRO GECISI 09.02.2017 BURAK*/
        Q_PLUGIN_METADATA (IID "org.qt-project.Qt.QDesignerCustomWidgetInterface")
        Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
        QAdakDate_Plugin(QObject *parent = 0);
        bool        isContainer() const;
        bool        isInitialized() const;
        QIcon       icon() const;
        QString     domXml() const;
        QString     group() const;
        QString     includeFile() const;
        QString     name() const;
        QString     toolTip() const;
        QString     whatsThis() const;
        QWidget *   createWidget(QWidget *parent);
        void        initialize(QDesignerFormEditorInterface *core);

private:
        bool        initialized;
};



/* 5.8 MACRO GECISI 09.02.2017 BURAK*/
#define QQAdakDate_Plugin_Plugin_iid "com.adak.framework.QAdakDate_Plugin"
Q_DECLARE_INTERFACE (QAdakDate_Plugin, QQAdakDate_Plugin_Plugin_iid)

#endif // QAdakDate_Plugin_H
