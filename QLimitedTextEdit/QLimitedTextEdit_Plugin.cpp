#include <QtPlugin>
#include "QLimitedTextEdit.h"
#include "QLimitedTextEdit_Plugin.h"


QLimitedTextEdit_Plugin::QLimitedTextEdit_Plugin(QObject *parent) : QObject(parent)
{
        initialized = false;
}

void QLimitedTextEdit_Plugin::initialize(QDesignerFormEditorInterface * )
{
    if (initialized) {
            return;
    }
        initialized = true;
}

bool QLimitedTextEdit_Plugin::isInitialized() const
{
        return initialized;
}

QWidget *QLimitedTextEdit_Plugin::createWidget(QWidget *parent)
{
        return new QLimitedTextEdit(parent);
}

QString QLimitedTextEdit_Plugin::name() const
{
        return "QLimitedTextEdit";
}

QString QLimitedTextEdit_Plugin::group() const
{
        return "Input Widgets";
}

QIcon QLimitedTextEdit_Plugin::icon() const
{
        return QIcon();
}

QString QLimitedTextEdit_Plugin::toolTip() const
{
        return "";
}

QString QLimitedTextEdit_Plugin::whatsThis() const
{
        return "";
}

bool QLimitedTextEdit_Plugin::isContainer() const
{
        return false;
}

QString QLimitedTextEdit_Plugin::domXml() const
{
     return "<widget class=\"QLimitedTextEdit\" name=\"limitedTextEdit\"></widget>";

}

QString QLimitedTextEdit_Plugin::includeFile() const
{
        return "QLimitedTextEdit.h";
}

/* 5.8 MACRO GECISI 09.02.2017 BURAK*/
Q_PLUGIN_METADATA ("com.adak.framework.QLimitedTextEdit_Plugin")
//Q_EXPORT_PLUGIN2(QLimitedTextEdit_Plugin, QLimitedTextEdit_Plugin)


