#include <QtGui/QApplication>
#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QTranslator>
#include <QLocale>

#include "qmlapplicationviewer.h"
#include "myclass.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    //QScopedPointer<QApplication> app(createApplication(argc, argv));

    QApplication app(argc, argv);
    QmlApplicationViewer viewer;

    QString locale = QLocale::system().name();
    QTranslator translator;
    if (translator.load("qml-translations." + locale, ":/"))

       app.installTranslator(&translator);

    // Next 4 lines should increase performance
    viewer.setAttribute(Qt::WA_OpaquePaintEvent);
    viewer.setAttribute(Qt::WA_OpaquePaintEvent);
    viewer.viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    viewer.viewport()->setAttribute(Qt::WA_NoSystemBackground);


   // QObject *rootObject = qobject_cast(viewer.rootObject());

    MyClass myClass ;
    viewer.rootContext()->setContextProperty("myObject", &myClass);
    viewer.rootContext()->setContextProperty("myModel",  &myClass.model);
    viewer.rootContext()->setContextProperty("dictModel",  &myClass.dictModel);

    //QObject::connect(&myClass, SIGNAL(invertTheme()), rootObject, SLOT(invertTheme()));

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qrc:///qml/qmlui/main.qml"));
    viewer.showExpanded();



    return app.exec();
//    return app->exec();
}
