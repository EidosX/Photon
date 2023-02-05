#ifndef TAG_H
#define TAG_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

class Tag : public QWidget
{
    Q_OBJECT
public:
    inline explicit Tag(QString name, QWidget *parent = nullptr) : QWidget(parent) {
        auto* hl = new QHBoxLayout(this);
        hl->setContentsMargins(10, 0, 0, 0);
        hl->setSpacing(8);
        setLayout(hl);

        label = new QLabel(this);
        label->setText(name);
        label->setStyleSheet("color: black;");
        hl->addWidget(label);

        delBtn = new QPushButton("X", this);
        delBtn->setStyleSheet("color: black; background-color: #9BD2F1; padding: 1px; width: 20px; height: 20px; ");
        delBtn->setCursor(Qt::PointingHandCursor);
        connect(delBtn, &QPushButton::clicked, this, &Tag::delBtnClicked);
        hl->addWidget(delBtn);

        setAttribute(Qt::WA_StyledBackground, true);
        setStyleSheet("background-color: #6CB3DC;");
    }

protected:
    QLabel* label;
    QPushButton* delBtn;

signals:
    void delBtnClicked();
};

class TagButton : public QPushButton {
    Q_OBJECT
public:
    inline explicit TagButton(QString name, QWidget* parent = nullptr) : QPushButton(parent) {
        setText(name);
        setStyleSheet("color: white; padding: 0 10px;");
        setCursor(Qt::PointingHandCursor);
    }

};

#endif // TAG_H
