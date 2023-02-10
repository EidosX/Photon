#ifndef TAG_H
#define TAG_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

// This is the widget that displays a removable tag.
// It is also used to display filters.
class Tag : public QWidget
{
    Q_OBJECT
public:
    inline Tag(QString name, QWidget *parent = nullptr, QString color = "#6CB3DC", QString delBtnColor = "#9BD2F1") : QWidget(parent) {
        auto* hl = new QHBoxLayout(this);
        hl->setContentsMargins(10, 0, 0, 0);
        hl->setSpacing(8);
        setLayout(hl);

        label = new QLabel(this);
        label->setText(name);
        label->setStyleSheet("color: black;");
        hl->addWidget(label);

        delBtn = new QPushButton("X", this);
        delBtn->setStyleSheet(QString("color: black; background-color: ")
                              .append(delBtnColor)
                              .append("; padding: 1px; width: 20px; height: 20px; "));
        delBtn->setCursor(Qt::PointingHandCursor);
        connect(delBtn, &QPushButton::clicked, this, &Tag::delBtnClicked);
        hl->addWidget(delBtn);

        setAttribute(Qt::WA_StyledBackground, true);
        setStyleSheet(QString("background-color: ").append(color).append(";"));
    }

protected:
    QLabel* label;
    QPushButton* delBtn;

signals:
    void delBtnClicked();
};

// This is used by the Add Tag and the Add Filter buttons.
class TagButton : public QPushButton {
    Q_OBJECT
public:
    inline explicit TagButton(QString name, QWidget* parent = nullptr) : QPushButton(parent) {
        setText(name);
        setStyleSheet("background-color: white; padding: 3px 10px; color : black;");
        setCursor(Qt::PointingHandCursor);
    }

};

#endif // TAG_H
