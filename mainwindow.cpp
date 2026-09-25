#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>

namespace {

QString RemoveTrailingZeroes(const QString& text) {
    for (qsizetype i = 0; i < text.size(); ++i) {
        if (text[i] != '0') {
            return text.mid(i);
        }
    }
    return "";
}

QString NormalizeNumber(const QString& text) {
    if (text.isEmpty()) {
        return "0";
    }
    if (text.startsWith('.')) {
        // Рекурсивный вызов.
        return NormalizeNumber("0" + text);
    }
    if (text.startsWith('-')) {
        // Рекурсивный вызов.
        QString rest = NormalizeNumber(text.mid(1));
        // Не допускаем отображение -0.
        if (rest == "0") {
            return "0";
        }
        return "-" + rest;
    }
    if (text.startsWith('0') && !text.startsWith("0.")) {
        return NormalizeNumber(RemoveTrailingZeroes(text));
    }
    return text;
}

QString OpToString(Operation op) {
    switch (op) {
    case Operation::NO_OPERATION:
        return "";
    case Operation::ADDITION:
        return "+";
    case Operation::DIVISION:
        return "÷";
    case Operation::MULTIPLICATION:
        return "×";
    case Operation::SUBTRACTION:
        return "−";
    case Operation::POWER:
        return "^";
    }
    return "";
}

}  // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    SetText("0");
    ui->l_memory->setText("");
    ui->l_formula->setText("");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SetText(const QString& text) {
    input_number_ = NormalizeNumber(text);
    active_number_ = input_number_.toDouble();
    ui->l_result->setText(input_number_);
}

void MainWindow::AddText(const QString& suffix) {
    // После получения результата (операция сброшена) начинаем новый ввод и очищаем формулу.
    if (input_number_.isEmpty() && current_operation_ == Operation::NO_OPERATION) {
        ui->l_formula->setText("");
    }
    SetText(input_number_ + suffix);
}

void MainWindow::SetOperation(Operation op) {
    if (current_operation_ == Operation::NO_OPERATION) {
        calculator_.Set(active_number_);
    }
    current_operation_ = op;
    ui->l_formula->setText(QString("%1 %2").arg(calculator_.GetNumber()).arg(OpToString(op)));
    input_number_ = "";
}

void MainWindow::on_tb_zero_clicked() {
    AddText("0");
}

void MainWindow::on_tb_one_clicked() {
    AddText("1");
}

void MainWindow::on_tb_two_clicked() {
    AddText("2");
}

void MainWindow::on_tb_three_clicked() {
    AddText("3");
}

void MainWindow::on_tb_four_clicked() {
    AddText("4");
}

void MainWindow::on_tb_five_clicked() {
    AddText("5");
}

void MainWindow::on_tb_six_clicked() {
    AddText("6");
}

void MainWindow::on_tb_seven_clicked() {
    AddText("7");
}

void MainWindow::on_tb_eight_clicked() {
    AddText("8");
}

void MainWindow::on_tb_nine_clicked() {
    AddText("9");
}

void MainWindow::on_tb_comma_clicked() {
    if (!input_number_.contains('.')) {
        AddText(".");
    }
}

void MainWindow::on_tb_negate_clicked() {
    if (input_number_.isEmpty()) {
        // Нет активного ввода: меняем знак текущего отображаемого числа
        // (результат операции или ранее введённый операнд).
        active_number_ = -active_number_;
        // Избегаем отрицательного нуля.
        if (active_number_ == 0.0) {
            active_number_ = 0.0;
        }
        SetText(QString::number(active_number_));
        // После результата очищаем формулу.
        if (current_operation_ == Operation::NO_OPERATION) {
            ui->l_formula->setText("");
        }
    } else if (input_number_.startsWith("-")) {
        SetText(input_number_.mid(1));
    } else {
        SetText("-" + input_number_);
    }
}

void MainWindow::on_tb_backspace_clicked() {
    if (input_number_.isEmpty()) {
        return;
    }
    input_number_.chop(1);
    SetText(input_number_);
}

void MainWindow::on_tb_add_clicked() {
    SetOperation(Operation::ADDITION);
}

void MainWindow::on_tb_substract_clicked() {
    SetOperation(Operation::SUBTRACTION);
}

void MainWindow::on_tb_multiplicate_clicked() {
    SetOperation(Operation::MULTIPLICATION);
}

void MainWindow::on_tb_divide_clicked() {
    SetOperation(Operation::DIVISION);
}

void MainWindow::on_tb_power_clicked() {
    SetOperation(Operation::POWER);
}

void MainWindow::on_tb_equal_clicked() {
    if (current_operation_ == Operation::NO_OPERATION) {
        return;
    }

    ui->l_formula->setText(QString("%1 %2 %3 =")
                               .arg(calculator_.GetNumber())
                               .arg(OpToString(current_operation_))
                               .arg(active_number_));

    switch (current_operation_) {
    case Operation::ADDITION:
        calculator_.Add(active_number_);
        break;
    case Operation::SUBTRACTION:
        calculator_.Sub(active_number_);
        break;
    case Operation::MULTIPLICATION:
        calculator_.Mul(active_number_);
        break;
    case Operation::DIVISION:
        calculator_.Div(active_number_);
        break;
    case Operation::POWER:
        calculator_.Pow(active_number_);
        break;
    case Operation::NO_OPERATION:
        break;
    }

    active_number_ = calculator_.GetNumber();
    ui->l_result->setText(QString::number(active_number_));
    input_number_ = "";
    current_operation_ = Operation::NO_OPERATION;
}

void MainWindow::on_tb_reset_clicked() {
    current_operation_ = Operation::NO_OPERATION;
    ui->l_formula->setText("");
    SetText("0");
}

void MainWindow::on_tb_ms_clicked() {
    memory_value_ = active_number_;
    has_memory_ = true;
    ui->l_memory->setText("M");
}

void MainWindow::on_tn_mr_clicked() {
    if (!has_memory_) {
        return;
    }
    active_number_ = memory_value_;
    ui->l_result->setText(QString::number(active_number_));
    input_number_ = "";
}

void MainWindow::on_tb_mc_clicked() {
    has_memory_ = false;
    memory_value_ = 0.0;
    ui->l_memory->setText("");
}