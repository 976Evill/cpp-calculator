#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QComboBox>
#include <QApplication>
#include <QPalette>
#include <QColor>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Цифры
    connect(ui->tb_zero, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->tb_one, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->tb_two, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->tb_three, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->tb_four, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->tb_five, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->tb_six, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->tb_seven, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->tb_eight, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->tb_nine, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);

    // Операции
    connect(ui->tb_add, &QPushButton::clicked, this, &MainWindow::OnOperationClicked);
    connect(ui->tb_substract, &QPushButton::clicked, this, &MainWindow::OnOperationClicked);
    connect(ui->tb_multiplicate, &QPushButton::clicked, this, &MainWindow::OnOperationClicked);
    connect(ui->tb_divide, &QPushButton::clicked, this, &MainWindow::OnOperationClicked);
    connect(ui->tb_power, &QPushButton::clicked, this, &MainWindow::OnOperationClicked);

    // Управляющие кнопки
    connect(ui->tb_equal, &QPushButton::clicked, this, &MainWindow::OnControlClicked);
    connect(ui->tb_reset, &QPushButton::clicked, this, &MainWindow::OnControlClicked);
    connect(ui->tb_ms, &QPushButton::clicked, this, &MainWindow::OnControlClicked);
    connect(ui->tn_mr, &QPushButton::clicked, this, &MainWindow::OnControlClicked);
    connect(ui->tb_mc, &QPushButton::clicked, this, &MainWindow::OnControlClicked);
    connect(ui->tb_negate, &QPushButton::clicked, this, &MainWindow::OnControlClicked);
    connect(ui->tb_backspace, &QPushButton::clicked, this, &MainWindow::OnControlClicked);
    connect(ui->tb_extra, &QPushButton::clicked, this, &MainWindow::OnControlClicked);

    // Выбор типа контроллера
    connect(ui->cmb_controller, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::OnControllerChanged);

    // Начальные значения
    ui->l_formula->setText("");
    ui->l_result->setText("0");
    ui->l_memory->setText("");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SetInputText(const std::string& text) {
    // Восстанавливаем цвет по умолчанию
    ui->l_result->setStyleSheet("");
    QPalette palette = QApplication::palette();
    ui->l_result->setPalette(palette);
    ui->l_result->setText(QString::fromStdString(text));
}

void MainWindow::SetErrorText(const std::string& text) {
    QPalette palette = ui->l_result->palette();
    palette.setColor(QPalette::WindowText, Qt::red);
    ui->l_result->setPalette(palette);
    // Также через stylesheet для надёжности
    ui->l_result->setStyleSheet("color: #ff0000;");
    ui->l_result->setText(QString::fromStdString(text));
}

void MainWindow::SetFormulaText(const std::string& text) {
    ui->l_formula->setText(QString::fromStdString(text));
}

void MainWindow::SetMemText(const std::string& text) {
    ui->l_memory->setText(QString::fromStdString(text));
}

void MainWindow::SetExtraKey(const std::optional<std::string>& key) {
    if (key.has_value()) {
        ui->tb_extra->setText(QString::fromStdString(key.value()));
        ui->tb_extra->show();
    } else {
        ui->tb_extra->hide();
    }
}

void MainWindow::SetDigitKeyCallback(std::function<void(int key)> cb) {
    digit_cb_ = std::move(cb);
}

void MainWindow::SetProcessOperationKeyCallback(std::function<void(Operation key)> cb) {
    operation_cb_ = std::move(cb);
}

void MainWindow::SetProcessControlKeyCallback(std::function<void(ControlKey key)> cb) {
    control_cb_ = std::move(cb);
}

void MainWindow::SetControllerCallback(std::function<void(ControllerType controller)> cb) {
    controller_cb_ = std::move(cb);
}

void MainWindow::OnDigitClicked() {
    if (!digit_cb_) {
        return;
    }
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) {
        return;
    }
    bool ok = false;
    int digit = button->text().toInt(&ok);
    if (ok) {
        digit_cb_(digit);
    }
}

void MainWindow::OnOperationClicked() {
    if (!operation_cb_) {
        return;
    }
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) {
        return;
    }
    const QString text = button->text();
    if (text == "+") {
        operation_cb_(Operation::ADDITION);
    } else if (text == "−") {
        operation_cb_(Operation::SUBTRACTION);
    } else if (text == "×") {
        operation_cb_(Operation::MULTIPLICATION);
    } else if (text == "÷") {
        operation_cb_(Operation::DIVISION);
    } else if (text == "xʸ") {
        operation_cb_(Operation::POWER);
    }
}

void MainWindow::OnControlClicked() {
    if (!control_cb_) {
        return;
    }
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) {
        return;
    }
    if (button == ui->tb_equal) {
        control_cb_(ControlKey::EQUALS);
    } else if (button == ui->tb_reset) {
        control_cb_(ControlKey::CLEAR);
    } else if (button == ui->tb_ms) {
        control_cb_(ControlKey::MEM_SAVE);
    } else if (button == ui->tn_mr) {
        control_cb_(ControlKey::MEM_LOAD);
    } else if (button == ui->tb_mc) {
        control_cb_(ControlKey::MEM_CLEAR);
    } else if (button == ui->tb_negate) {
        control_cb_(ControlKey::PLUS_MINUS);
    } else if (button == ui->tb_backspace) {
        control_cb_(ControlKey::BACKSPACE);
    } else if (button == ui->tb_extra) {
        control_cb_(ControlKey::EXTRA_KEY);
    }
}

void MainWindow::OnControllerChanged(int index) {
    if (!controller_cb_) {
        return;
    }
    switch (index) {
    case 0:
        controller_cb_(ControllerType::DOUBLE);
        break;
    case 1:
        controller_cb_(ControllerType::FLOAT);
        break;
    case 2:
        controller_cb_(ControllerType::UINT8_T);
        break;
    case 3:
        controller_cb_(ControllerType::INT);
        break;
    case 4:
        controller_cb_(ControllerType::INT64_T);
        break;
    case 5:
        controller_cb_(ControllerType::SIZE_T);
        break;
    case 6:
        controller_cb_(ControllerType::RATIONAL);
        break;
    default:
        break;
    }
}