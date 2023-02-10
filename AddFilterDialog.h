#ifndef ADDFILTERDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QLineEdit>
#include "filter.h"

#define ADDFILTERDIALOG_H

class FilterForm;

// This is the main dialog that will open when the user clicks the
// 'New Label' button.
class AddFilterDialog : public QDialog {
    Q_OBJECT
public:
    AddFilterDialog(QWidget* parent = nullptr);

    std::optional<Filter> getResult();

private:
    FilterForm* _currentForm = nullptr;
};



// Filter form is the base class of the forms inside the AddFilterDialog.
// They represent a way for the user to input a certain type of filter.
class FilterForm : public QWidget {
    Q_OBJECT
public:
    inline FilterForm(QWidget* parent = nullptr) : QWidget(parent) {}
    virtual std::optional<Filter> getResult() = 0;
};

// TagForm allows the user to specify a tag name
class TagForm : public FilterForm {
    Q_OBJECT
public:
    TagForm(QWidget* parent = nullptr);
    std::optional<Filter> getResult() override;
private:
    QString _name;
};

// MinRatingForm allow the user to specify a minimum number of stars
class MinRatingForm : public FilterForm {
    Q_OBJECT
public:
    MinRatingForm(QWidget* parent = nullptr);
    std::optional<Filter> getResult();
signals:
    void starsUpdated();
private:
    int _stars = 5;
};

#endif // ADDFILTERDIALOG_H
