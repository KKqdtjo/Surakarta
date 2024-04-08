#ifndef RESULT2_H
#define RESULT2_H

#include <QWidget>

namespace Ui {
class result2;
}

class result2 : public QWidget
{
    Q_OBJECT

public:
    explicit result2(QWidget *parent = nullptr);
    ~result2();

private:
    Ui::result2 *ui;
};

#endif // RESULT2_H
