#ifndef QSearchEdit_Plugin_H
#define QSearchEdit_Plugin_H

#include <QDesignerCustomWidgetInterface>

class QSearchEdit_Plugin : public QObject, public QDesignerCustomWidgetInterface
{
        Q_OBJECT
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

#endif // QSearchEdit_Plugin_H
