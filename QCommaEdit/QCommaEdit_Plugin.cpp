#include <QtPlugin>
#include "QCommaEdit.h"
#include "QCommaEdit_Plugin.h"

#include "../UTILS/virgul_ekle.cpp"
#include "../UTILS/round.cpp"

QCommaEdit_Plugin::QCommaEdit_Plugin(QObject *parent) : QObject(parent)
{
        initialized = false;
}

void QCommaEdit_Plugin::initialize(QDesignerFormEditorInterface * )
{
    if (initialized) {
            return;
    }
        initialized = true;
}

bool QCommaEdit_Plugin::isInitialized() const
{
        return initialized;
}

QWidget *QCommaEdit_Plugin::createWidget(QWidget *parent)
{
        return new QCommaEdit(parent);
}

QString QCommaEdit_Plugin::name() const
{
        return "QCommaEdit";
}

QString QCommaEdit_Plugin::group() const
{
        return tr("Input Widgets");
}

QIcon QCommaEdit_Plugin::icon() const
{
        return QIcon();
}

QString QCommaEdit_Plugin::toolTip() const
{
        return tr("Number takes as a comma and write. ");//Sayilari virgullu olarak alir, yazar
}

QString QCommaEdit_Plugin::whatsThis() const
{
        return "";
}

bool QCommaEdit_Plugin::isContainer() const
{
        return false;
}

QString QCommaEdit_Plugin::domXml() const
{
     return "<widget class=\"QCommaEdit\" name=\"commaEdit\">\n"
            " <property name=\"QCommaEdit\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>100</width>\n"
            "   <height>100</height>\n"
            "  </rect>\n"
            " </property>\n"
            " <property name=\"whatsThis\" >\n"
            "  <string>Sayilari formatina(Or:1,000,234.50) donusturur. "
            "</string>\n"
            " </property>\n"
            "</widget>\n";

}

QString QCommaEdit_Plugin::includeFile() const
{
        return "QCommaEdit.h";
}


Q_EXPORT_PLUGIN2(QCommaEdit_Plugin, QCommaEdit_Plugin)


