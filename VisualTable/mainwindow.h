
#include <QApplication>
#include <QWidget>
#include <QComboBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <QFont>
#include <QCheckBox>
#include <QSpinBox>
#include <QListWidget>
#include <cstdlib>
#include <ctime>

#include "TTable.h"
#include "TArrayTable.h"
#include "TTreeTable.h"
#include "HashTable.h"

#ifndef ANDREW_TABLES_MAINWINDOW_H
#define ANDREW_TABLES_MAINWINDOW_H

class MainWindow : public QWidget {

    Q_OBJECT

protected:
    QHBoxLayout *over_layout_;
    QVBoxLayout *main_layout_;
    QHBoxLayout *table_size_layout_;
    QHBoxLayout *table_control_buttons_layout_;
    QHBoxLayout *operation_type_buttons_layout_;
    QHBoxLayout *record_value_layout_;

    QListWidget *history_list_;

    QComboBox *table_type_combobox_;
    QLabel *table_creation_label_;
    QLabel *table_size_label_;
    QSpinBox *table_size_spinbox_;
    QPushButton *create_table_button_;
    QPushButton *delete_table_button_;
    QPushButton *generate_random_table_;
    QPushButton *operation_insert_radiobuttons_;
    QPushButton *operation_find_radiobuttons_;
    QPushButton *operation_delete_radiobuttons_;
    QCheckBox *random_value_checkbox_;
    QSpinBox *key_spinbox_;
    QSpinBox *value_spinbox_;
    QLabel *efficiency_label_;
    QPushButton *eff_clear_button_;
    QPushButton *exit_button_;

    TTable *table_;

    QSpinBox* hash_table_step_spinbox_;

    typedef enum {kTableCreated, kTableCreationError, kTableDeleted, kTableDeletionError, kTableNotExists, kRecordInserted, kRecordNotInserted, kRecordFinded, kRecordNotFinded, kRecordDeleted, kRecordNotDeleted} action_result;
protected slots:
    void CreateTable();
    void DeleteTable();
    void GenerateRandom();
    void ChangeRandomState();
    void Insert();
    void Find();
    void Delete();
    void ChangeEff();
    void ClearEff();

    void AddToHistory(action_result result);
public:
    explicit MainWindow(QWidget *parent = nullptr);
};

#endif
