#ifndef ADDFILTERDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QLineEdit>
#include "filter.h"
#include "star.h"

#define ADDFILTERDIALOG_H

class FilterForm : public QWidget {
    Q_OBJECT
public:
    inline FilterForm(QWidget* parent = nullptr) : QWidget(parent) {}
    virtual std::optional<Filter> getResult() = 0;
};

class TagForm : public FilterForm {
    Q_OBJECT
public:
    inline TagForm(QWidget* parent = nullptr) : FilterForm(parent) {
        auto* l = new QVBoxLayout(this);

        auto* textInput = new QLineEdit(this);
        textInput->setPlaceholderText("Enter a tag name");
        connect(textInput, &QLineEdit::textEdited, this, [this](const QString& newText){ _name = newText; });
        l->addWidget(textInput);
    }
    std::optional<Filter> getResult() override { return _name.isEmpty() ? std::nullopt : std::optional(TagFilter { _name }); }
private:
    QString _name;
};

class MinRatingForm : public FilterForm {
    Q_OBJECT
public:
    inline MinRatingForm(QWidget* parent = nullptr) : FilterForm(parent) {
        auto* l = new QHBoxLayout(this);
        for (int i = 1; i <= 5; ++i) {
            auto* star = new Star(this);
            star->setMaximumSize(20, 20);
            connect(star, &Star::clicked, this, [this, i](){ _stars = i; emit starsUpdated(); });
            connect(this, &MinRatingForm::starsUpdated, star, [this, star, i](){ star->enable(_stars >= i); });
            l->addWidget(star);
        }
    }
    std::optional<Filter> getResult() override { return RatingFilter { _stars }; }
signals:
    void starsUpdated();
private:
    int _stars = 5;
};

class AddFilterDialog : public QDialog {
    Q_OBJECT
public:
    inline std::optional<Filter> getResult() { return _currentForm->getResult(); }

    inline AddFilterDialog(QWidget* parent = nullptr) : QDialog(parent) {
        auto* l = new QVBoxLayout(this);
        setLayout(l);

        auto* dropdown = new QComboBox(this);
        dropdown->addItem("Tag");
        dropdown->addItem("Min. Rating");
        auto updateCurrentForm = [this, l, dropdown](){
            qDebug() << "1";
            if (_currentForm != nullptr) delete _currentForm;
            qDebug() << "2";
            if (dropdown->currentIndex() == 0) _currentForm = new TagForm();
            else if (dropdown->currentIndex() == 1) _currentForm = new MinRatingForm();
            else throw std::runtime_error("No matching index!");
            qDebug() << "3";
            l->insertWidget(1, _currentForm);
        };
        connect(dropdown, &QComboBox::activated, this, updateCurrentForm);
        l->addWidget(dropdown);

        updateCurrentForm();
        l->addWidget(_currentForm);

        auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, this);
        l->addWidget(buttonBox);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

private:
    FilterForm* _currentForm = nullptr;
};

#endif // ADDFILTERDIALOG_H
