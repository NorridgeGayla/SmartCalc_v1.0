#include "../headers/mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_credit_calculate_clicked() {
  clear_credit_line();
  double credit_sum = ui->credit_sum->text().toDouble();
  int credit_term = ui->credit_term->text().toInt();
  double procent = ui->credir_procent->text().toDouble();
  if (credit_sum != 0 && credit_term != 0 && procent != 0) {
    int type_term = ui->credit_term_choice->currentIndex();
    if (!type_term) {
      credit_term *= 12;
    }
    procent /= 100;
    bool type_payment = ui->difference->isChecked();
    if (type_payment) {
      difference_calc(credit_sum, credit_term, procent);
    } else {
      annuitet_calc(credit_sum, credit_term, procent);
    }
  } else {
    ui->credit_error->setText(INVALID_DATA_LINE);
  }
}

void MainWindow::clear_credit_line() {
  ui->credit_error->clear();
  ui->credit_payment->clear();
  ui->credit_overpayment->clear();
  ui->credit_total_payment->clear();
}

void MainWindow::annuitet_calc(double credit_sum, const int credit_term,
                               double procent) {
  procent /= 12;
  double payment = credit_sum * procent / (1 - pow(1 + procent, -credit_term));
  double payment_total = payment * credit_term;
  double overpayment = payment_total - credit_sum;
  set_annuitet_result(payment, overpayment, payment_total);
}

void MainWindow::set_annuitet_result(const double payment,
                                     const double overpayment,
                                     const double payment_total) {
  char result_str[SIZE_BUFFER] = {'\0'};
  setlocale(LC_NUMERIC, "C");
  sprintf(result_str, "%.2f", payment);
  ui->credit_payment->setText(result_str);
  memset(result_str, '\0', SIZE_BUFFER);
  sprintf(result_str, "%.2f", overpayment);
  ui->credit_overpayment->setText(result_str);
  memset(result_str, '\0', SIZE_BUFFER);
  sprintf(result_str, "%.2f", payment_total);
  ui->credit_total_payment->setText(result_str);
}

void MainWindow::difference_calc(double credit_sum, const int credit_term,
                                 const double procent) {
  QDate date_start_term = QDate::currentDate();
  double start_payment = 0.0;
  double end_payment = 0.0;
  double principal_payment = credit_sum / credit_term;
  double procent_payment = 0.0;
  double payment = 0.0;
  double total_payment = 0.0;
  double overpayment = -credit_sum;
  for (int i = 0; i < credit_term; ++i, credit_sum -= principal_payment,
           date_start_term = date_start_term.addMonths(1)) {
    procent_payment = credit_sum * procent * date_start_term.daysInMonth() /
                      date_start_term.daysInYear();
    payment = principal_payment + procent_payment;
    if (i == 0) {
      start_payment = payment;
    } else if (i != 0 && i == credit_term - 1) {
      end_payment = payment;
    }
    total_payment += payment;
  }
  overpayment += total_payment;
  if (credit_term == 1) {
    set_annuitet_result(start_payment, overpayment, total_payment);
  } else {
    set_difference_result(start_payment, end_payment, overpayment,
                          total_payment);
  }
}

void MainWindow::set_difference_result(const double start_payment,
                                       const double end_payment,
                                       const double overpayment,
                                       const double payment_total) {
  char result_str[SIZE_BUFFER] = {'\0'};
  setlocale(LC_NUMERIC, "C");
  sprintf(result_str, "%.2f", start_payment);
  strcat(result_str, "...");
  char str[SIZE_BUFFER] = {'\0'};
  sprintf(str, "%.2f", end_payment);
  strcat(result_str, str);
  ui->credit_payment->setText(result_str);
  memset(result_str, '\0', SIZE_BUFFER);
  sprintf(result_str, "%.2f", overpayment);
  ui->credit_overpayment->setText(result_str);
  memset(result_str, '\0', SIZE_BUFFER);
  sprintf(result_str, "%.2f", payment_total);
  ui->credit_total_payment->setText(result_str);
}
