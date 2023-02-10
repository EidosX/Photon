#include "addfilterdialog.h"
#include "star.h"

AddFilterDialog::AddFilterDialog(QWidget* parent) : QDialog(parent) {
    auto* l = new QVBoxLayout(this);
    setLayout(l);

    auto* dropdown = new QComboBox(this);
    dropdown->addItem("Tag");
    dropdown->addItem("Min. Rating");
    auto updateCurrentForm = [this, l, dropdown](){
        if (_currentForm != nullptr) delete _currentForm;
        if (dropdown->currentIndex() == 0) _currentForm = new TagForm();
        else if (dropdown->currentIndex() == 1) _currentForm = new MinRatingForm();
        else throw std::runtime_error("No matching index!");
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

std::optional<Filter> AddFilterDialog::getResult() {
    return _currentForm->getResult();
}



TagForm::TagForm(QWidget* parent) : FilterForm(parent) {
    auto* l = new QVBoxLayout(this);

    auto* textInput = new QLineEdit(this);
    textInput->setPlaceholderText("Enter a tag name");
    connect(textInput, &QLineEdit::textEdited, this, [this](const QString& newText){
        _name = newText;
    });
    l->addWidget(textInput);
}

std::optional<Filter> TagForm::getResult() {
    return _name.isEmpty() ? std::nullopt : std::optional(TagFilter { _name });
}

MinRatingForm::MinRatingForm(QWidget* parent) : FilterForm(parent) {
    auto* l = new QHBoxLayout(this);
    for (int i = 1; i <= 5; ++i) {
        auto* star = new Star(this);
        star->setMaximumSize(20, 20);
        connect(star, &Star::clicked, this, [this, i](){
            _stars = i;
            emit starsUpdated();
        });
        connect(this, &MinRatingForm::starsUpdated, star, [this, star, i](){
            star->enable(_stars >= i);
        });
        l->addWidget(star);
    }
}
std::optional<Filter> MinRatingForm::getResult() {
    return RatingFilter { _stars };
}

