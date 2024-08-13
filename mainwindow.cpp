#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "calculator.h"

#include <string>
#include <Qstring>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    ui->txtDec1->setPlaceholderText("Introducir primer operando");
    ui->txtDec2->setPlaceholderText("Introducir segundo operando");

}

MainWindow::~MainWindow()
{
    delete ui;
}

float first_number, second_number;

void MainWindow::on_txtDec1_editingFinished()
{
    first_number= ui->txtDec1->text().toFloat();
    std::string result = Calculator::decimal_to_binary(first_number);
    ui->txtBin1->setText(QString::fromStdString(result));

}


void MainWindow::on_txtDec2_editingFinished()
{
    second_number= ui->txtDec2->text().toFloat();
    std::string result = Calculator::decimal_to_binary(second_number);
    ui->txtBin2->setText(QString::fromStdString(result));
}


void MainWindow::on_pushButton_clicked()
{
    std::string result_first = Calculator::decimal_to_binary(first_number);
    std::string result_second = Calculator::decimal_to_binary(second_number);
    std::string result_bin = Calculator::addition_algorithm(result_first, result_second, false);
    ui->txtBinResult->setText(QString::fromStdString(result_bin));
    ui->txtDecResult->setText(QString::fromStdString(Calculator::binary_to_decimal(result_bin)));
}


void MainWindow::on_pushButton_2_clicked()
{
    std::string result_first = Calculator::decimal_to_binary(first_number);
    std::string result_second = Calculator::decimal_to_binary(second_number);
    std::string result_bin = Calculator::product_algorithm(result_first,result_second, false);
    ui->txtBinResult->setText(QString::fromStdString(result_bin));
    ui->txtDecResult->setText(QString::fromStdString(Calculator::binary_to_decimal(result_bin)));
}
