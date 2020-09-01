#include "qt/Application.h"

#ifdef ANDROID
#include <QtSvg/QtSvg>    //Because deployment sometimes just forgets to include this lib otherwise
#endif

int main(int argc, char *argv[])
{
    qt::Application app(argc, argv);
    return app.run();
}
