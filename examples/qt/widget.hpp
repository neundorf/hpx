//  Copyright (c) 2012-2014 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <QtGui/QDialog>


class QHpx;

class QWidget;
class QListWidget;
class QPushButton;

class widget
    : public QDialog
{
    Q_OBJECT

    public:
        widget(QWidget *parent = nullptr);
        ~widget();


    public slots:
        void add_label(std::size_t i, double t);
        void runFinishedSlot();
        void set_threads(int no);

        void run_clicked(bool);
        void init_hpx_clicked();
        void stop_hpx_clicked();

    private:
        QHpx* qhpx;
        std::size_t no_threads;
        QListWidget *list;
        QPushButton * run_button;
        QPushButton * init_hpx_button;
        QPushButton * stop_hpx_button;
};
