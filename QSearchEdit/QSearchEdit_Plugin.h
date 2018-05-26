#ifndef QSearchEdit_Plugin_H
#define QSearchEdit_Plugin_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class QSearchEdit_Plugin : public QObject, public QDesignerCustomWidgetInterface
{
        Q_OBJECT
        /* 5.8 MACRO GECISI 09.02.2017 BURAK*/
        Q_PLUGIN_METADATA (IID "org.qt-project.Qt.QDesignerCustomWidgetInterface")
        Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
        QSearchEdit_Plugin(QObject *parent = 0);
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
#define QSearchEdit_Plugin_iid "com.adak.framework.QSearchEdit_Plugin"
Q_DECLARE_INTERFACE (QSearchEdit_Plugin, QSearchEdit_Plugin_iid)

#endif // QSearchEdit_Plugin_H
