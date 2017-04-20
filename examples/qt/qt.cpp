//  Copyright (c) 2012-2014 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <cstddef>

#include <QtGui/QApplication>
#include "widget.hpp"
#include "qhpx.hpp"

/* void qt_main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    using hpx::util::placeholders::_1;
    using hpx::util::placeholders::_2;
    widget main(hpx::util::bind(run, _1, _2));
    main.show();

    app.exec();
} */


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QHpx::self()->setArgcArgv(argc, argv);

    widget main;
    main.show();

    app.exec();
//     return hpx::init(argc, argv);
}
