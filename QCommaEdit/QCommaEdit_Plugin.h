#ifndef QCommaEdit_Plugin_H
#define QCommaEdit_Plugin_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class QCommaEdit_Plugin : public QObject, public QDesignerCustomWidgetInterface
    {
        Q_OBJECT
        Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
        QCommaEdit_Plugin(QObject *parent = 0);
        bool isContainer() const;
        bool isInitialized() const;
        QIcon icon() const;
        QString domXml() const;
        QString group() const;
        QString includeFile() const;
        QString name() const;
        QString toolTip() const;
        QString whatsThis() const;
        QWidget *createWidget(QWidget *parent);
        void initialize(QDesignerFormEditorInterface *core);

private:
        bool initialized;
    };

/* 5.8 MACRO GECISI 09.02.2017 BURAK*/
#define QCommaEdit_Plugin_iid "com.adak.framework.QCommaEdit_Plugin"
Q_DECLARE_INTERFACE (QCommaEdit_Plugin, QCommaEdit_Plugin_iid)

#endif // QCommaEdit_Plugin_H
