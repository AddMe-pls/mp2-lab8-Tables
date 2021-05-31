#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    over_layout_ = new QHBoxLayout;
    main_layout_ = new QVBoxLayout;
    table_size_layout_ = new QHBoxLayout;
    table_control_buttons_layout_ = new QHBoxLayout;
    operation_type_buttons_layout_ = new QHBoxLayout;
    record_value_layout_ = new QHBoxLayout;

    history_list_ = new QListWidget;

    table_type_combobox_ = new QComboBox;
    table_creation_label_ = new QLabel("TABLE NOT CREATED");
    table_size_label_ = new QLabel("TABLE SIZE: ");
    table_size_spinbox_ = new QSpinBox;
    create_table_button_ = new QPushButton("CREATE TABLE");
    delete_table_button_ = new QPushButton("DELETE TABLE");
    generate_random_table_ = new QPushButton("GENERATE RANDOM");
    operation_insert_radiobuttons_ = new QPushButton("INSERT");
    operation_find_radiobuttons_ = new QPushButton("FIND");
    operation_delete_radiobuttons_ = new QPushButton("DELETE");
    random_value_checkbox_ = new QCheckBox("RANDOM VALUE");
    key_spinbox_ = new QSpinBox;
    value_spinbox_ = new QSpinBox;
    efficiency_label_ = new QLabel("NO CALCULATIONS");
    eff_clear_button_ = new QPushButton("CLEAR EFF");
    print_table_button_ = new QPushButton("PRINT");
    exit_button_ = new QPushButton("EXIT");

    table_ = nullptr;

    hash_table_step_spinbox_ = new QSpinBox;
    hash_table_step_spinbox_->setRange(0, SHRT_MAX);

    srand(time(nullptr));

    history_list_->setMinimumWidth(300);

    QFont labels_font("Arial", 16, QFont::Bold);
    table_creation_label_->setFont(labels_font);
    table_creation_label_->setAlignment(Qt::AlignCenter);
    efficiency_label_->setFont(labels_font);
    efficiency_label_->setAlignment(Qt::AlignCenter);

    table_type_combobox_->addItem("SCAN TABLE");
    table_type_combobox_->addItem("SORT TABLE");
    table_type_combobox_->addItem("TREE TABLE");
    table_type_combobox_->addItem("HASH TABLE");
    table_type_combobox_->addItem("BALANCED TREE TABLE");

    table_size_spinbox_->setRange(0, SHRT_MAX);
    key_spinbox_->setRange(0, SHRT_MAX);
    value_spinbox_->setRange(SHRT_MIN, SHRT_MAX);
    table_size_spinbox_->setValue(10);

    random_value_checkbox_->setChecked(true);
    key_spinbox_->setReadOnly(true);
    value_spinbox_->setReadOnly(true);

    connect(create_table_button_, &QPushButton::clicked, this, &MainWindow::CreateTable);
    connect(delete_table_button_, &QPushButton::clicked, this, &MainWindow::DeleteTable);
    connect(generate_random_table_, &QPushButton::clicked, this, &MainWindow::GenerateRandom);
    connect(eff_clear_button_, &QPushButton::clicked, this, &MainWindow::ClearEff);
    connect(print_table_button_, &QPushButton::clicked, this, &MainWindow::PrintTable);
    connect(random_value_checkbox_, &QCheckBox::stateChanged, this, &MainWindow::ChangeRandomState);
    connect(operation_insert_radiobuttons_, &QPushButton::clicked, this, &MainWindow::Insert);
    connect(operation_find_radiobuttons_, &QPushButton::clicked, this, &MainWindow::Find);
    connect(operation_delete_radiobuttons_, &QPushButton::clicked, this, &MainWindow::Delete);
    connect(exit_button_, &QPushButton::clicked, qApp, &QApplication::exit);

    table_size_layout_->addWidget(table_size_label_);
    table_size_layout_->addWidget(table_size_spinbox_);
    table_size_layout_->addWidget(hash_table_step_spinbox_);
    table_control_buttons_layout_->addWidget(create_table_button_);
    table_control_buttons_layout_->addWidget(delete_table_button_);
    operation_type_buttons_layout_->addWidget(operation_insert_radiobuttons_);
    operation_type_buttons_layout_->addWidget(operation_find_radiobuttons_);
    operation_type_buttons_layout_->addWidget(operation_delete_radiobuttons_);
    record_value_layout_->addWidget(key_spinbox_);
    record_value_layout_->addWidget(value_spinbox_);
    main_layout_->addWidget(table_type_combobox_);
    main_layout_->addWidget(table_creation_label_);
    main_layout_->addLayout(table_size_layout_);
    main_layout_->addLayout(table_control_buttons_layout_);
    main_layout_->addWidget(generate_random_table_);
    main_layout_->addLayout(operation_type_buttons_layout_);
    main_layout_->addWidget(random_value_checkbox_);
    main_layout_->addLayout(record_value_layout_);
    main_layout_->addWidget(efficiency_label_);
    main_layout_->addWidget(eff_clear_button_);
    main_layout_->addWidget(print_table_button_);
    main_layout_->addWidget(exit_button_);

    over_layout_->addLayout(main_layout_);
    over_layout_->addWidget(history_list_);

    //setLayout(main_layout_);
    setLayout(over_layout_);
}

void MainWindow::CreateTable() {
    if (!table_) {
        table_creation_label_->setText("TABLE CREATION");
        switch (table_type_combobox_->currentIndex()) {
            case 0: table_ = new TScanTable(table_size_spinbox_->value()); break;
            case 1: table_ = new TSortTable(table_size_spinbox_->value()); break;
            case 2: table_ = new TTreeTable(table_size_spinbox_->value()); break;
            case 3: table_ = new HashTable(table_size_spinbox_->value(), hash_table_step_spinbox_->value()); break;
            case 4: table_ = new TBalanceTreeTable(table_size_spinbox_->value()); break;
            default: break;
        }
        ChangeEff();
        table_creation_label_->setText("TABLE CREATED");
        table_size_spinbox_->setReadOnly(true);
        AddToHistory(kTableCreated);
    } else {
        AddToHistory(kTableCreationError);
    }
}

void MainWindow::DeleteTable() {
    if (table_) {
        table_creation_label_->setText("TABLE DELETION");
        delete table_;
        table_ = nullptr;
        ChangeEff();
        table_creation_label_->setText("TABLE DELETED");
        table_size_spinbox_->setReadOnly(false);
        AddToHistory(kTableDeleted);
    } else {
        AddToHistory(kTableDeletionError);
    }
}

void MainWindow::GenerateRandom() {
    if (!table_)
        return;
    bool random_state = random_value_checkbox_->isChecked();
    random_value_checkbox_->setChecked(true);
    int key_state = key_spinbox_->value(), value_state = value_spinbox_->value();
    while (table_->GetDataCount() != table_size_spinbox_->value()) {
        Insert();
    }
    key_spinbox_->setValue(key_state);
    value_spinbox_->setValue(value_state);
    random_value_checkbox_->setChecked(random_state);
}

void MainWindow::ChangeRandomState() {
    key_spinbox_->setReadOnly(!key_spinbox_->isReadOnly());
    value_spinbox_->setReadOnly(!value_spinbox_->isReadOnly());
}

void MainWindow::Insert() {
    if (!table_) {
        AddToHistory(kTableNotExists);
        return;
    }
    TKey key;
    TVal value;
    if (random_value_checkbox_->isChecked()) {
        key = rand() % (SHRT_MAX + 1);
        //value = rand() % (SHRT_MAX - SHRT_MIN + 1) + SHRT_MIN;
        value = rand() % (SHRT_MAX + 1);
        key_spinbox_->setValue(key);
        value_spinbox_->setValue(value);
    } else {
        key = key_spinbox_->value();
        value = value_spinbox_->value();
    }
    if (table_->Insert(TRecord(key, value))) {
        AddToHistory(kRecordInserted);
    } else {
        AddToHistory(kRecordNotInserted);
    }
    ChangeEff();
}

void MainWindow::Find() {
    if (!table_) return;
    TKey key;
    if (random_value_checkbox_->isChecked()) {
        key = rand() % (SHRT_MAX + 1);
        key_spinbox_->setValue(key);
        value_spinbox_->setValue(0);
    } else {
        key = key_spinbox_->value();
    }
    if (table_->Find(key)) {
        AddToHistory(kRecordFinded);
    } else {
        AddToHistory(kRecordNotFinded);
    }
    ChangeEff();
}

void MainWindow::Delete() {
    if (!table_) return;
    TKey key;
    if (random_value_checkbox_->isChecked()) {
        key = rand() % (SHRT_MAX + 1);
        key_spinbox_->setValue(key);
        value_spinbox_->setValue(0);
    } else {
        key = key_spinbox_->value();
    }
    if (table_->Delete(key)) {
        AddToHistory(kRecordDeleted);
    } else {
        AddToHistory(kRecordNotDeleted);
    }
    ChangeEff();
}

void MainWindow::ChangeEff() {
    if (table_) efficiency_label_->setText(QString("EFF: ") + QString::number(table_->GetEff()));
    else efficiency_label_->setText("NO TABLE");
}

void MainWindow::ClearEff() {
    if (table_) {
        table_->ClearEff();
        AddToHistory(kEffCleared);
    }
    ChangeEff();
}

void MainWindow::PrintTable() {
    if (table_) {
        table_->Print();
        AddToHistory(kTablePrinted);
    }
    else efficiency_label_->setText("NO TABLE");
}

void MainWindow::AddToHistory(action_result result) {
    switch (result) {
        case kTableCreated:
            history_list_->insertItem(0, QString::number(history_list_->count() + 1) + QString(") TABLE WITH TYPE ") + table_type_combobox_->currentText() + QString(" CREATED"));
            break;
        case kTableCreationError:
            history_list_->insertItem(0, QString::number(history_list_->count() + 1) + ") TABLE CREATION ERROR");
            break;
        case kTableDeleted:
            history_list_->insertItem(0, QString::number(history_list_->count() + 1) + ") TABLE DELETED");
            break;
        case kTableDeletionError:
            history_list_->insertItem(0, QString::number(history_list_->count() + 1) + ") TABLE DELETION ERROR");
            break;
        case kTableNotExists:
            history_list_->insertItem(0, QString::number(history_list_->count() + 1) + ") TABLE NOT EXISTS");
            break;
        case kRecordInserted:
            history_list_->insertItem(0, QString::number(history_list_->count() + 1) + QString(") RECORD KEY: ") + QString::number(key_spinbox_->value()) + QString(" VALUE: ") + QString::number(value_spinbox_->value()) + QString(" INSERTED"));
            break;
        case kRecordNotInserted:
            history_list_->insertItem(0, QString::number(history_list_->count() + 1) + QString(") RECORD KEY: ") + QString::number(key_spinbox_->value()) + QString(" VALUE: ") + QString::number(value_spinbox_->value()) + QString(" NOT INSERTED"));
            break;
        case kRecordFinded:
            history_list_->insertItem(0, QString::number(history_list_->count() + 1) + QString(") RECORD KEY: ") + QString::number(key_spinbox_->value()) + QString(" FINDED"));
            break;
        case kRecordNotFinded:
            history_list_->insertItem(0, QString::number(history_list_->count() + 1) + QString(") RECORD KEY: ") + QString::number(key_spinbox_->value()) + QString(" NOT FINDED"));
            break;
        case kRecordDeleted:
            history_list_->insertItem(0, QString::number(history_list_->count() + 1) + QString(") RECORD KEY: ") + QString::number(key_spinbox_->value()) + QString(" DELETED"));
            break;
        case kRecordNotDeleted:
            history_list_->insertItem(0, QString::number(history_list_->count() + 1) + QString(") RECORD KEY: ") + QString::number(key_spinbox_->value()) + QString(" NOT DELETED"));
            break;
        case kEffCleared:
            history_list_->insertItem(0, QString::number(history_list_->count() + 1) + ") EFFECTIVENESS CLEARED");
            break;
        case kTablePrinted:
            history_list_->insertItem(0, QString::number(history_list_->count() + 1) + ") TABLE PRINTED TO THE FILE");
            break;
    }
}
