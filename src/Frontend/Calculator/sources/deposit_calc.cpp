#include "../headers/mainwindow.h"
#include "ui_mainwindow.h"

#define EPS 1e-07
#define INCOME_TAX 13
#define SUM_FOR_TAX_CALCULATION 1e+06

void MainWindow::on_deposit_calculation_clicked() {
  clear_deposit_line();
  double deposit_sum = ui->deposit_sum->text().toDouble();
  double deposit_term = ui->deposit_term->text().toDouble();
  double deposit_procent = ui->deposit_procent->text().toDouble();
  double deposit_procent_CB = ui->deposit_procent_CB->text().toDouble();
  if (deposit_sum != 0 && deposit_term != 0 && deposit_procent != 0 &&
      deposit_procent_CB != 0) {
    deposit_calculation();

  } else {
    ui->deposit_error->setText(INVALID_DATA_LINE);
  }
}

void MainWindow::deposit_calculation() {
  double deposit_sum = ui->deposit_sum->text().toDouble();
  QDate date_start_term = ui->deposit_start_term->date();
  QDate date_end_term = get_end_date_term_deposit();
  bool capitalization = ui->deposit_capitalization->isChecked();
  QDate date_replanishment = ui->deposit_date_replanishment->date();
  double sum_replanishment = ui->deposit_sum_replanishment->text().toDouble();
  int period_replanishment = ui->deposit_period_replanishment->currentIndex();
  check_date_replanishment_or_withdraw(date_start_term, sum_replanishment,
                                       period_replanishment,
                                       &date_replanishment);
  QDate date_withdraw = ui->deposit_date_withdraw->date();
  double sum_withdraw = ui->deposit_sum_withdraw->text().toDouble();
  int period_withdraw = ui->deposit_period_withdraw->currentIndex();
  check_date_replanishment_or_withdraw(date_start_term, sum_withdraw,
                                       period_withdraw, &date_withdraw);
  double sum_procent_of_period = 0.0;
  double sum_procent_of_year = 0.0;
  double total_sum_procent = 0.0;
  double sum_tax = 0.0;
  QDate date_payment = get_next_date_payment(date_start_term);
  date_start_term = date_start_term.addDays(1);
  QDate new_year = QDate(date_start_term.year(), 12, 31);
  for (; date_start_term <= date_end_term && deposit_sum != 0;
       date_start_term = date_start_term.addDays(1)) {
    if (sum_replanishment != 0 && date_start_term == date_replanishment) {
      add_replanishment(&deposit_sum, &date_replanishment);
    }
    if (sum_withdraw != 0 && date_start_term == date_withdraw) {
      sub_withdraw(&deposit_sum, &date_withdraw);
    }
    if (capitalization && date_start_term == date_payment &&
        date_start_term != date_end_term) {
      deposit_sum += sum_procent_of_period;
      add_sum_procent_of_year(&sum_procent_of_year, &sum_procent_of_period);
    }
    procent_calculation(deposit_sum, date_start_term, &sum_procent_of_period,
                        &total_sum_procent);
    if (date_start_term == date_payment || date_start_term == date_end_term) {
      if (capitalization) {
        deposit_sum += sum_procent_of_period;
      }
      add_sum_procent_of_year(&sum_procent_of_year, &sum_procent_of_period);
      date_payment = get_next_date_payment(date_start_term);
    }
    if (date_start_term == new_year || date_start_term == date_end_term) {
      add_sum_tax(&sum_tax, &sum_procent_of_year);
      new_year = new_year.addYears(1);
    }
  }
  set_deposit_result(total_sum_procent, sum_tax, deposit_sum);
}

void MainWindow::clear_deposit_line() {
  ui->deposit_procent_result->clear();
  ui->deposit_tax_result->clear();
  ui->deposit_sum_result->clear();
  ui->deposit_error->clear();
}

void MainWindow::on_deposit_start_term_editingFinished() {
  QDate date_today = QDate::currentDate();
  if (ui->deposit_start_term->date() < date_today) {
    ui->deposit_start_term->setDate(date_today);
  }
}

void MainWindow::on_deposit_date_replanishment_editingFinished() {
  QDate date_today = QDate::currentDate();
  if (ui->deposit_date_replanishment->date() < date_today) {
    ui->deposit_date_replanishment->setDate(date_today);
  }
}

void MainWindow::on_deposit_date_withdraw_editingFinished() {
  QDate date_today = QDate::currentDate();
  if (ui->deposit_date_withdraw->date() < date_today) {
    ui->deposit_date_withdraw->setDate(date_today);
  }
}

QDate MainWindow::get_end_date_term_deposit() {
  QDate start_date = ui->deposit_start_term->date();
  double deposit_term = ui->deposit_term->text().toDouble();
  int deposit_term_type = ui->deposit_term_type->currentIndex();
  if (deposit_term_type == 0) {
    start_date = start_date.addDays(deposit_term);
  } else if (deposit_term_type == 1) {
    start_date = start_date.addMonths(deposit_term);
  } else {
    start_date = start_date.addYears(deposit_term);
  }
  return start_date;
}

QDate MainWindow::get_next_date_payment(QDate date_payment) {
  int period_payment = ui->deposit_period_payment->currentIndex();
  switch (period_payment) {
    case 0:
      date_payment = date_payment.addDays(1);
      break;
    case 1:
      date_payment = date_payment.addDays(7);
      break;
    case 2:
      date_payment = date_payment.addMonths(1);
      break;
    case 3:
      date_payment = date_payment.addMonths(3);
      break;
    case 4:
      date_payment = date_payment.addMonths(6);
      break;
    case 5:
      date_payment = date_payment.addYears(1);
      break;
    case 6:
      QDate end_date = get_end_date_term_deposit();
      int days = ui->deposit_start_term->date().daysTo(end_date);
      date_payment = ui->deposit_start_term->date().addDays(days);
      break;
  }
  return date_payment;
}

void MainWindow::get_next_date_replanishment_or_withdraw(QDate *date,
                                                         const int type) {
  switch (type) {
    case 1:
      *date = date->addMonths(1);
      break;
    case 2:
      *date = date->addMonths(2);
      break;
    case 3:
      *date = date->addMonths(3);
      break;
    case 4:
      *date = date->addMonths(6);
      break;
    case 5:
      *date = date->addYears(1);
      break;
  }
}

void MainWindow::add_replanishment(double *deposit_sum,
                                   QDate *date_replanishment) {
  *deposit_sum += ui->deposit_sum_replanishment->text().toDouble();
  int period_replanishment = ui->deposit_period_replanishment->currentIndex();
  get_next_date_replanishment_or_withdraw(date_replanishment,
                                          period_replanishment);
}

void MainWindow::sub_withdraw(double *deposit_sum, QDate *date_withdraw) {
  double sum_withdraw = ui->deposit_sum_withdraw->text().toDouble();
  *deposit_sum = (*deposit_sum - sum_withdraw >= EPS)
                     ? *deposit_sum - sum_withdraw
                     : *deposit_sum;
  int period_withdraw = ui->deposit_period_withdraw->currentIndex();
  get_next_date_replanishment_or_withdraw(date_withdraw, period_withdraw);
}

void MainWindow::procent_calculation(const double deposit_sum, const QDate date,
                                     double *sum_procent_of_period,
                                     double *total_sum_procent) {
  double deposit_procent = ui->deposit_procent->text().toDouble() / 100;
  double sum_procent_of_day = deposit_sum * deposit_procent / date.daysInYear();
  *sum_procent_of_period += sum_procent_of_day;
  *total_sum_procent += sum_procent_of_day;
}

void MainWindow::add_sum_tax(double *sum_tax, double *sum_procent_of_year) {
  double deposit_procent_CB = ui->deposit_procent_CB->text().toDouble() / 100;
  double sum_tax_free = SUM_FOR_TAX_CALCULATION * deposit_procent_CB;
  if (*sum_procent_of_year > sum_tax_free) {
    *sum_tax += (*sum_procent_of_year - sum_tax_free) * INCOME_TAX / 100;
  }
  *sum_procent_of_year = 0.0;
}

void MainWindow::add_sum_procent_of_year(double *sum_procent_of_year,
                                         double *sum_procent_of_period) {
  *sum_procent_of_year += *sum_procent_of_period;
  *sum_procent_of_period = 0.0;
}

void MainWindow::check_date_replanishment_or_withdraw(
    const QDate date_start_term, const double sum, const int period,
    QDate *date_replanishment_or_withdraw) {
  if (sum != 0 && *date_replanishment_or_withdraw == date_start_term) {
    get_next_date_replanishment_or_withdraw(date_replanishment_or_withdraw,
                                            period);
  }
}

void MainWindow::set_deposit_result(const double total_sum_procent,
                                    const double sum_tax,
                                    const double deposit_sum) {
  char result_str[SIZE_BUFFER] = {'\0'};
  setlocale(LC_NUMERIC, "C");
  sprintf(result_str, "%.2f", total_sum_procent);
  ui->deposit_procent_result->setText(result_str);
  memset(result_str, '\0', SIZE_BUFFER);
  sprintf(result_str, "%.2f", sum_tax);
  ui->deposit_tax_result->setText(result_str);
  memset(result_str, '\0', SIZE_BUFFER);
  sprintf(result_str, "%.2f", deposit_sum);
  ui->deposit_sum_result->setText(result_str);
}
