//  Copyright (c) 2012-2014 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// #include <hpx/config.hpp>

#include <cstddef>
#include <functional>

#include "widget.hpp"
#include "qhpx.hpp"

#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSpinBox>
#include <QtGui/QPushButton>
#include <QtGui/QListWidget>


widget::widget(QWidget *parent)
    : QDialog(parent)
    , qhpx(nullptr)
    , no_threads(50)
    , list(nullptr)
    , run_button(nullptr)
    , init_hpx_button(nullptr)
    , stop_hpx_button(nullptr)
{
    QHBoxLayout * init_layout = new QHBoxLayout;

    init_hpx_button = new QPushButton("Init HPX");
    stop_hpx_button = new QPushButton("Stop HPX");

    QObject::connect(init_hpx_button, SIGNAL(clicked()), this, SLOT(init_hpx_clicked()));
    QObject::connect(stop_hpx_button, SIGNAL(clicked()), this, SLOT(stop_hpx_clicked()));

    init_layout->addWidget(init_hpx_button);
    init_layout->addWidget(stop_hpx_button);

    QHBoxLayout * layout = new QHBoxLayout;

    QSpinBox * thread_number_widget = new QSpinBox;
    thread_number_widget->setValue(50);
    thread_number_widget->setRange(1, 100000);
    QObject::connect(thread_number_widget, SIGNAL(valueChanged(int)),
        this, SLOT(set_threads(int)));

    run_button = new QPushButton("Run");
    QObject::connect(run_button, SIGNAL(clicked(bool)), this, SLOT(run_clicked(bool)));

    layout->addWidget(new QLabel("Number of threads: "));
    layout->addWidget(thread_number_widget);
    layout->addWidget(run_button);

    QVBoxLayout * main_layout = new QVBoxLayout;
    main_layout->addLayout(init_layout);
    main_layout->addLayout(layout);

    list = new QListWidget;
    main_layout->addWidget(list);

    setLayout(main_layout);

    qhpx = QHpx::self();

    connect(qhpx, SIGNAL(runFinished()), this, SLOT(runFinishedSlot()));
    connect(qhpx, SIGNAL(runThreadFinished(std::size_t, double)), this, SLOT(add_label(std::size_t, double)));
}

widget::~widget()
{
}

void widget::add_label(std::size_t i, double t)
{
    QString txt("Thread ");
    txt.append(QString::number(i))
       .append(" finished in ")
       .append(QString::number(t))
       .append(" seconds");
    list->addItem(txt);
}

void widget::runFinishedSlot()
{
    run_button->setEnabled(true);
}

void widget::set_threads(int no)
{
    no_threads = std::size_t(no);
}

void widget::run_clicked(bool)
{
    run_button->setEnabled(false);
    list->clear();
    qhpx->run(no_threads);
}

void widget::init_hpx_clicked()
{
    qhpx->start();
}

void widget::stop_hpx_clicked()
{
    qhpx->stop();
}
