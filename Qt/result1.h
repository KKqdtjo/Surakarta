#ifndef RESULT1_H
#define RESULT1_H

#include <QWidget>

namespace Ui {
class result1;
}

class result1 : public QWidget
{
    Q_OBJECT

public:
    explicit result1(QWidget *parent = nullptr);
    ~result1();

private:
    Ui::result1 *ui;
};

#endif // RESULT1_H
